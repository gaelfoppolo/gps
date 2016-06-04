/**
 *  @file parser_nmea.h
 *  @author Gaël Foppolo (gaelfoppolo)
 *
 *  @brief File containing the NMEA parser functions
 */

#ifndef PARSER_NMEA_H
#define PARSER_NMEA_H

/** @brief Maximum length for NMEA sentences */
#define NMEA_MAX_LENGTH 100 

/**
 * The sentence identifier
 */
typedef enum nmea_sentence_id {
    NMEA_INVALID = -1,
    NMEA_UNKNOWN = 0,
    NMEA_SENTENCE_RMC,
    NMEA_SENTENCE_GGA,
    NMEA_SENTENCE_GSA,
    NMEA_SENTENCE_GLL,
    NMEA_SENTENCE_GST,
    NMEA_SENTENCE_GSV,
    NMEA_SENTENCE_VTG
};

/**
 * The structure that contains the data of RMC sentences
 */
typedef struct nmea_sentence_rmc {
    /** @brief Sentence validity */
    int valid;
    /** @brief Latitude */
    float latitude;
    /** @brief Longitude */
    float longitude;
    /** @brief Speed */
    float speed;
    /** @brief Heading */
    float heading;
} nmea_sentence_rmc;

/**
 * The structure that contains the data of GGA sentences
 */
typedef struct nmea_sentence_gga {
    /** @brief Latitude */
    float latitude;
    /** @brief Longitude */
    float longitude;
    /** @brief Quality */
    int fix_quality;
    /** @brief Number of satellites */
    int satellites_tracked;
    /** @brief HDOP */
    float hdop;
    /** @brief Altitude */
    float altitude;
    /** @brief Altitude unit */
    char altitude_units;
    /** @brief Height */
    float height;
    /** @brief Height unit */
    char height_units;
    /** @brief Age */
    int dgps_age;
} nmea_sentence_gga;

/**
 * @brief      Transform hexa to integer
 *
 * @param[in]  c     An integer (char)
 *
 * @return     An integer (hex)
 */
int hex2int(char c);

 /**
  * @brief Scanf-like processor for NMEA sentences
  * @details Supports the following formats:
  * c - single character (char *)
  * d - direction, returned as 1/-1, default 0 (int *)
  * f - float (float *)
  * o - longitude value, transform all in degrees (float *)
  * a - latitude value, transform all in degrees (float *)
  * i - decimal (integer), default zero (int *)
  * s - string (char *)
  * t - talker identifier and type (char *)
  * 
  * @param sentence The sentence to parse
  * @param format The format of the sentence
  * @see nmea_parse_*** functions for further explanations
  * 
  * @return 1 on success, 1 if not
  */
int nmea_scan(const char *sentence, const char *format, ...);

/**
 * @brief Check if the char is part of the field
 * @details Aka char isn't a comma or star
 * 
 * @param c The char to test
 * @return 1 is valid, 0 if not
 */
int nmea_isfield(char c);

/**
 * @brief Determine sentence identifier.
 * 
 * @param sentence Then sentence to test
 * @return The type of sentence
 */
enum nmea_sentence_id nmea_sentence_id(char *sentence);

/**
 * @brief Check sentence validity and checksum.
 * @details Calculate checksum and compare it   
 * 
 * @param sentence The sentence to test
 * @param strict Accept or not sentence with checksum
 * 
 * @return 1 for valid sentences.
 */
 int nmea_check(const char *sentence, int strict);

/**
 * @brief Parse a RMC sentence
 * 
 * @param frame The struct where to put the parsed data
 * @param sentence The sentence to parse
 * 
 * @return 1 on success
 */
int nmea_parse_rmc(nmea_sentence_rmc *frame, const char *sentence);

/**
 * @brief Parse a GGA sentence
 * 
 * @param frame The struct where to put the parsed data
 * @param sentence The sentence to parse
 * 
 * @return 1 on success
 */
int nmea_parse_gga(nmea_sentence_gga *frame, const char *sentence);
// int nmea_parse_gsa(nmea_sentence_gsa *frame, const char *sentence);
// int nmea_parse_gll(nmea_sentence_gll *frame, const char *sentence);
// int nmea_parse_gst(nmea_sentence_gst *frame, const char *sentence);
// int nmea_parse_gsv(nmea_sentence_gsv *frame, const char *sentence);
// int nmea_parse_vtg(nmea_sentence_vtg *frame, const char *sentence);

#endif
