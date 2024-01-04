/*
 * Quanser_servo_framework.h
 *
 *  Created on: Mar 27, 2023
 *      Author: dgt
 */

#ifndef QUANSER_SERVO_FRAMEWORK_H_
#define QUANSER_SERVO_FRAMEWORK_H_

// initialization functions
void initializeMicrocontroller (float32);
void initializePlant (void);

// input-output functions
void setPlantLED (Uint16, Uint16, Uint16);
void setMotorVoltage (float32);
float32 getMotorCurrent (void);
float32 getMotorSpeed (void);
float32 getMotorPosition (void);
float32 getPendulumPosition (void);
Uint32 getCommunicationStatus (void);
char getAmplifierStatus (void);

// communication functions
void exchangePacketSPI (void);
void transmitFloatSCI (float32);
void transmitByteSCI (char);

// watch dog function
void feedWD (void);

#endif /* QUANSER_SERVO_FRAMEWORK_H_ */
