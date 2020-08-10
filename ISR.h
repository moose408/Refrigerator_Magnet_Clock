//==========================================================================
// Refrigerator Magnet Clock - ISR.h
//
// Copyright (c) 2020, Craig Colvin
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================


const unsigned PERIOD_USEC = 10;

//Servo parameters
const unsigned PERIOD_20MS = 20e3;                      //Period is measured in uSec so this = 20 ms
const unsigned SERVO_STARTING_PULSE_WIDTH = 15e2;       //1.5ms


void RunMotorISR(long lAlphaSteps, long lBetaSteps, int iSpeed);
void SetServoPosition(int iServoPosDegrees);
