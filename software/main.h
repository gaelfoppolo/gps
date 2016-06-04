
/**
 *	@file main.h
 *	@author Gaël Foppolo (gaelfoppolo)
 *
 *	@brief File containing the main functions
 */

#ifndef MAIN_H
#define MAIN_H

/** @brief Mode menu */
#define M_MENU 0
/** @brief Mode compass */
#define M_COMPASS 1
/** @brief Mode navigation */
#define M_NAVIG 2
/** @brief Mode record */
#define M_RECORD 3
/** @brief Mode shutdown */
#define M_SHUTDOWN 4

/** @brief Communication with GPS module */
#define COMM_GPS 0
/** @brief Communication with USB */
#define COMM_USB 1

/** @brief YES */
#define YES 1
/** @brief NO */
#define NO 0

/**
 * Mode selected by the user, @see M_MENU, etc.
 */
extern unsigned int modeSelected;

/**
 * @brief Configure the external clock
 */
void configureClock(void);

/**
 * @brief Toogle the communication (P4.2, CMD_SWITCH)
 * @details 1 = USB, 0 = GPS
 * 
 * @param state The new state
 */
void toggleCommunication(unsigned int state);

/**
 * @brief Wait for x sec
 * 
 * @param x The time to wait (in sec ~)
 */
void delay(float x);

#endif