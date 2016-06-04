
/**
 *  @file parser_nmea.c
 *  @author Gaël Foppolo (gaelfoppolo)
 *
 *  @brief File containing the NMEA parser functions
 */

#include <__cross_studio_io.h>
#include <msp430x16x.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>
#include <stdlib.h>

#include "parser_nmea.h"

int hex2int(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return -1;
}

enum nmea_sentence_id nmea_sentence_id(char* sentence) {

    char type[2];
    unsigned int iParse;
    memset(type, 0, sizeof type);
    for(iParse = 0; iParse < 3; iParse++)
    type[iParse] = sentence[iParse+3];
    
    if (!strcmp(type, "RMC"))
        return NMEA_SENTENCE_RMC;
    if (!strcmp(type, "GGA"))
        return NMEA_SENTENCE_GGA;
    if (!strcmp(type, "GSA"))
        return NMEA_SENTENCE_GSA;
    if (!strcmp(type, "GLL"))
        return NMEA_SENTENCE_GLL;
    if (!strcmp(type, "GST"))
        return NMEA_SENTENCE_GST;
    if (!strcmp(type, "GSV"))
        return NMEA_SENTENCE_GSV;
    if (!strcmp(type, "VTG"))
        return NMEA_SENTENCE_VTG;
    return NMEA_UNKNOWN;
}


int nmea_isfield(char c) {
    return isprint((unsigned char) c) && (c != ',') && (c != '*');
}

int nmea_scan(const char *sentence, const char *format, ...) {
    const char *field;
    int result = 0;
    int optional = 0;
    va_list ap;
    va_start(ap, format);

    field = sentence;

    while (*format) {
        char type = *format++;
        if (type == ';') {
            // All further fields are optional.
            optional = 1;
            continue;
        }

        if (!field && !optional) {
            // Field requested but we ran out if input. Bail out.
            goto parse_error;
        }

        switch (type) {
            case 'c': { // Single character field (char).
                char value = '\0';

                if (field && nmea_isfield(*field))
                    value = *field;

                *va_arg(ap, char *) = value;
            } break;

            case 'd': { // Single character direction field (int).
                int value = 0;

                if (field && nmea_isfield(*field)) {
                    switch (*field) {
                        case 'N':
                        case 'E':
                            value = 1;
                            break;
                        case 'S':
                        case 'W':
                            value = -1;
                            break;
                        default:
                            goto parse_error;
                    }
                }

                *va_arg(ap, int *) = value;
            } break;

            case 'f': { // Float value
                char buff[50];
                int cpt = 0;
                float value;

                if (field) {
                    while (nmea_isfield(*field)) {
                        buff[cpt++] = *field;
                        field++;
                    }
                    buff[cpt++] = '\0';
                    value = atof(buff);
                } else {
                    value = 0;
                }

                *va_arg(ap, float *) = value;
            } break;

            case 'i': { // Integer value, default 0 (int).
                int value = 0;

                if (field) {
                    char *endptr;
                    value = strtol(field, &endptr, 10);
                    if (nmea_isfield(*endptr))
                        goto parse_error;
                }

                *va_arg(ap, int *) = value;
            } break;

            case 'a':
            case 'o': { // longitude, get + translate data
                int latOrLon = (type == 'a') ? 2 : 3;
                char degrees[3];
                int cptDegree = 0, cptDeci = 0;
                float value;
                char deci[50];
                memset(deci, 0, sizeof deci);
                memset(degrees, 0, sizeof degrees);
                if (field) {
                    while (nmea_isfield(*field)) {
                        if (cptDegree < latOrLon) {
                            degrees[cptDegree++] = *field;
                        } else {
                            deci[cptDeci++] = *field;
                        }
                        field++;
                    }
                    
                    // the first two char are in degrees
                    // need to translate the rest in degrees
                    value = atof(deci);
                    value /= 60.0;
                    value += atof(degrees);

                } else {
                    value = 0;
                }

                *va_arg(ap, float *) = value;
            } break;

            case 's': { // String value (char *).
                char *buf = va_arg(ap, char *);

                if (field) {
                    while (nmea_isfield(*field))
                        *buf++ = *field++;
                }

                *buf = '\0';
            } break;

            case 't': { // NMEA talker+sentence identifier (char *).
                // This field is always mandatory.
                char *buf;
                unsigned int iParse;
                if (!field) {
                    goto parse_error;
                }
                if (field[0] != '$') {
                    goto parse_error;
                }
                for (iParse=0; iParse<5; iParse++) {
                    if (!nmea_isfield(field[1+iParse])) {
                        goto parse_error;
                    }
                }
                buf = va_arg(ap, char *);
                memcpy(buf, field+1, 5);
                buf[5] = '\0';
            } break;

            case '_': { // Ignore the field.
            } break;

            default: { // Unknown.
                goto parse_error;
            } break;
        }

        /* Progress to the next field. */
        while (nmea_isfield(*sentence)) {
            sentence++;
        }
        /* Make sure there is a field there. */ 
        if (*sentence == ',') { 
            sentence++; 
            field = sentence; 
        } else { 
            field = NULL; 
        }
    }

    result = 1;

    parse_error:
    
    va_end(ap);
    return result;
}

int nmea_parse_rmc(struct nmea_sentence_rmc *frame, const char *sentence)
{
    // $GPRMC,081836,A,3751.65,S,14507.36,E,000.0,360.0,130998,011.3,E*62
    char type[6];
    char validity;
    int latitude_direction;
    int longitude_direction;
    if (!nmea_scan(sentence, "t_cadodff___",
        type,
        &validity,
        &frame->latitude, 
        &latitude_direction,
        &frame->longitude, 
        &longitude_direction,
        &frame->speed,
        &frame->heading)) {
        return 0;
    }
    
    if (strcmp(type+2, "RMC")) 
    return 0;

    frame->valid = (validity == 'A');
    frame->latitude *= latitude_direction;
    frame->longitude *= longitude_direction;

    return 1;
}

int nmea_parse_gga(struct nmea_sentence_gga *frame, const char *sentence) {
    // $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
    char type[6];
    int latitude_direction;
    int longitude_direction;

    if (!nmea_scan(sentence, "t_fdfdiiffcfci_",
            type,
            &frame->latitude,
            &latitude_direction,
            &frame->longitude,
            &longitude_direction,
            &frame->fix_quality,
            &frame->satellites_tracked,
            &frame->hdop,
            &frame->altitude, 
            &frame->altitude_units,
            &frame->height, 
            &frame->height_units,
            &frame->dgps_age))
        return 0;
    if (strcmp(type+2, "GGA"))
        return 0;

    frame->latitude *= latitude_direction;
    frame->longitude *= longitude_direction;

    return 1;
}

int nmea_check(const char *sentence, int strict)
{
    unsigned int checksum = 0x00;
    int upper, lower, expected;
    // Sequence length is limited.
    if (strlen(sentence) > NMEA_MAX_LENGTH + 3)
        return 0;

    // A valid sentence starts with "$".
    if (*sentence++ != '$')
        return 0;

    // The optional checksum is an XOR of all bytes between "$" and "*".
    while (*sentence && *sentence != '*' && isprint((unsigned char) *sentence))
        checksum ^= *sentence++;

    // If checksum is present...
    if (*sentence == '*') {
        // Extract checksum.
        sentence++;
        upper = hex2int(*sentence++);
        if (upper == -1)
            return 0;
        lower = hex2int(*sentence++);
        if (lower == -1)
            return 0;
        expected = upper << 4 | lower;

        // Check for checksum mismatch.
        if (checksum != expected)
            return 0;
    } else if (strict) {
        // Discard non-checksummed frames in strict mode.
        return 0;
    }

    // The only stuff allowed at this point is a newline.
    if (*sentence && strcmp(sentence, "\n") && strcmp(sentence, "\r\n"))
        return 0;

    return 1;
}
