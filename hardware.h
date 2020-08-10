//==========================================================================
// Refrigerator Magnet Clock - hardware.h
//
// Copyright (c) 2020, Craig Colvin
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================




// Pinout and motor constants
//
// Using A4988 Stepper Driver Board
//

// Arduino pin usage
const int ALPHA_STEP_PIN = 2;
const int X_DIR_PIN = 7;

const int BETA_STEP_PIN = 3;
const int Y_DIR_PIN = 6;

const int NOT_ENABLE_PIN = 8;    //Active low ENABLE pin to turn on stepper drivers

const int ZERO_X_SWITCH_PIN = 9;
const int ZERO_Y_SWITCH_PIN = 10;

//----------------------------------------------------------------------
// The timer library I'm using only supports timers 3,4, & 5.
// I use two timers to trigger interrupts for the stepper motor
// The 3rd timer is used to create a PWM signal on a pin for the
// servo motor.
//
// The SAMD21G chip that I'm using only allows the timers to drive
// a pin for timers 3 and 5. For timer 5 the pin it uses is the SCK
// pin which is only available on the ISCP header. This was my 
// original setup  which worked OK for the breadboard but is
// not easily wired for the final hardware. So I swapped the timers
// between the Alpha Timer and the Servo Timer so that the Servo uses
// timer 3 which is tied to I/O pin D5 on the RobotDyn board.
//----------------------------------------------------------------------
#define SERVO_USE_TIMER_FIVE 0  
#if SERVO_USE_TIMER_FIVE
const int ALPHA_TIMER = 3;
const int BETA_TIMER = 4;
const int SERVO_TIMER = 5;
const int MAGNET_SERVO_PIN = 20; // PB11/SCK pin 20 on SAMD21G for Timer 5
#else
const int ALPHA_TIMER = 5;
const int BETA_TIMER = 4;
const int SERVO_TIMER = 3;
const int MAGNET_SERVO_PIN = 5; // PA15 pin 24 (D5) on SAMD21G for Timer 3
#endif


//CoreXY constants
#define CCW HIGH
#define CW LOW

// Motor parameters
const int FULL_STEPS_PER_REVOLUTION = 200;
const int MICROSTEPS = 16;
const int STEPS_PER_REVOLUTION = FULL_STEPS_PER_REVOLUTION * MICROSTEPS;

const int MAXSPEED = 20;       //this is the delay in microseconds between pulses sent to the step pin on the A4988
