/**
 *  @file led.h
 *  @author Gaël Foppolo (gaelfoppolo)
 *
 *  @brief File containing the LED functions
 */

#ifndef LED_H
#define LED_H

/**
 * @brief Init LED (P1.0 -> P1.4)
 * @details All ready to use and state cleared
 */
void initLED(void);

/**
 * @brief Toogle the state of the choosen LED for a choosen time
 * @details duration = 0 -> stay in the state choosen
 * 
 * @param n   The LED to toogle
 * @param state The new state
 * @param duration The time to toogle the state of the LED
 */
void toggleLED(int n, unsigned int state, unsigned int duration);

#endif