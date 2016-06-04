
/**
 *	@file gps.h
 *	@author Gaël Foppolo (gaelfoppolo)
 *
 *	@brief File containing the GPS functions
 */

#ifndef GPS_H
#define GPS_H
 
/** @brief How many sentences do we want to receive in interrupt mode */
#define NUMBERS_OF_SENTENCE 4
/** @brief How many sentences do we want to receive in interrupt mode (MAX) */
#define NUMBERS_OF_SENTENCE_MAX 10

// PMTK300 : controls how often the module checks the satellites and updates its position registers
// PMTK220 : controls how often the GPS module emits NMEA sentences
// PMTK314 : controls how often the GPS module emits NMEA each sentences (17 supported NMEA sentences individually)

// send sentences every X seconds

/** @brief GPS emits NMEA sentences every 1 sec */
#define RATE_1SEC "$PMTK220,1000*1F\r\n"
/** @brief GPS emits NMEA sentences every 2 sec */
#define RATE_2SEC "$PMTK220,2000*1C\r\n"
/** @brief GPS emits NMEA sentences every 5 sec */
#define RATE_5SEC "$PMTK220,5000*1B\r\n"
/** @brief GPS emits NMEA sentences every 10 sec */
#define RATE_10SEC "$PMTK220,10000*2F\r\n"
/** @brief Tell the GPS to not send sentences */
#define DISABLE_ALL "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"
/** @brief Tell the GPS to send all sentences */
#define TURN_ALL "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"
/** @brief Tell the GPS to send GGA and RMC sentences only */
#define GGA_RMC "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n"
/** @brief Tell the GPS to send RMC sentences only */
#define RMC_ONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n"

/**
 *  Structure that contains useful GPS data
 */
typedef struct gps_data {
   	/** @brief The latitude */
    float latitude;
   	/** @brief The longitude */
    float longitude;
   	/** @brief The speed */   	
    float speed;
   	/** @brief The heading */   
    float heading;
} gps_data;

/**
 * Data are valid or not?
 */
extern unsigned int dataValid;

/**
 * @brief      Useful data received and valid
 */
extern struct gps_data GPSData;

/**
 * @brief Toggle GPS (P4.0, ENABLE_GPS)
 * @details 1 = enable, 0 = disable
 * 
 * @param state The new state
 */
void toggleGPS(unsigned int state);

/**
 * @brief Toggle GPS interrupt
 * @details 1 = interrupt enable for GPS, 0 = disable
 * 
 * @param state The new state
 */
void toggleGPSInterrupt(unsigned int state);

/**
 * @brief      Enable and config USART for GPS
 */
void enableUSARTforGPS(void);

/**
 * @brief      Send sentences to GPS to configure it (interrupt mode)
 *
 * @param      message  Message to send
 */
void gpsSend(char* message);

/**
 * @brief      Receive function for GPS data (USART0, interrupt mode)
 */
void usart0_rx(void);

/**
 * @brief Calculate the distance between two points (Haversine formula)
 * @details @see Wikipedia
 * 
 * @param lat1 The latitude of the first point
 * @param lon1 The longitude of the first point
 * @param lat2 The latitude of the second point
 * @param lon2 The longitude of the second point
 * @return The distance (in km)
 */
float calcDistance(float lat1, float lon1, float lat2, float lon2);

/**
 * @brief Degrees to radians converter
 * @details
 * 
 * @param deg The angle in degrees
 * @return The angle in radians
 */
float deg2rad(float deg);

#endif

