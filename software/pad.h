
/**
 *  @file pad.h
 *  @author Gaël Foppolo (gaelfoppolo)
 *
 *  @brief File containing the PAD functions
 */

#ifndef PAD_H
#define PAD_H

/** @brief PUSH position */
#define PUSH 0x1E
/** @brief TOP position */
#define TOP 0x1D
/** @brief RIGHT position */
#define RIGHT 0x0F
/** @brief BOTTOM position */
#define BOTTOM 0x1B
/** @brief LEFT position */
#define LEFT 0x17

/**
 * @brief Init LED (P2.0 -> P2.4)
 * @details All ready to use and state cleared
 */
void initPAD(void);
/**
 * @brief      Interrupt function for PAD
 */
void padInterrupt(void);

#endif