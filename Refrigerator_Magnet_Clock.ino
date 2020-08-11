//==========================================================================
// Refrigerator Magnet Clock - Main Loop
//
// Copyright (c) 2020, Craig Colvin
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================

#include "hardware.h"
#include "coreXY.h"
#include "menu.h"
#include "ISR.h"
#include "Clock.h"
#include "RTClib.h"


typedef struct {
  int minutes;
  int tensOfMinutes;
  int hours;
  int tensOfHours;
}myDateTime;

//========================================
// s e t u p
//========================================
void setup()
{
   //Setup SerialUSB
    SerialUSB.begin(9600);
    SerialUSB.print("Stepper Test\n");

  // Declare pins as Outputs
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(ALPHA_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(BETA_STEP_PIN, OUTPUT);
  pinMode(NOT_ENABLE_PIN, OUTPUT);
  pinMode(ZERO_X_SWITCH_PIN, INPUT_PULLUP);
  pinMode(ZERO_Y_SWITCH_PIN, INPUT_PULLUP);
 
  digitalWrite(NOT_ENABLE_PIN, LOW);   //Enable motors

  RTCInit();    //Initialize Real time clock and set time if not set

}
//========================================
// l o o p
//========================================
void loop() {
  char chMenuSelection;
  bool bEnable;

  //This will display the menu and wait for user input 
  //then based upon the input will do the correct command

  chMenuSelection = ShowMenu();
  switch(chMenuSelection)
    {
    case 'i' :
        StoreDigits();
        break;
    case 's' :
        StartClock();
        break;
    case 'z' :
        ZeroXandY();
        break;
   default :
        SerialUSB.print("selected unknown\n");
        break;
    }

}
//=======================================
// R u n C l o c k
//=======================================
void RunClock() {

  while(1) {
    UpdateDisplayTime();
    delay(1000);
  }
}
//=======================================
// S t a r t C l o c k
//=======================================
void StartClock() {
  myDateTime dtUserInput;

  //make sure we are zeroed
  DisengageMagnet();
  ZeroXandY();

  //ask user for current displayed time and return numbers to their home positions
  dtUserInput = InputTime();
  ResetDisplayedTime(dtUserInput.tensOfHours, dtUserInput.hours, dtUserInput.tensOfMinutes, dtUserInput.minutes);

  //Place initial digits into clock position
  InitializeClockDisplay();
  
  //Start main loop and change clock every minute
  RunClock();
}
//=======================================
// I n p u t T i m e 
//=======================================
myDateTime InputTime() {
  myDateTime dtInput;
  char chInputbuffer[20];
  int iHour,iMins;

  SerialUSB.print("\nInput current displayed time :");


  for(int i=0; i<4; i++) {
    while (SerialUSB.available() == 0) {}; // (SerialUSB.available()) {
      char inChar = SerialUSB.read();
      if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      chInputbuffer[0] = inChar;
      chInputbuffer[1] = 0x00;
      SerialUSB.print(chInputbuffer);

      switch (i) {
        case 0:
            dtInput.tensOfHours = atoi(chInputbuffer);
            break;
        case 1:
            dtInput.hours = atoi(chInputbuffer);
            SerialUSB.print(":");
            break;
        case 2:
            dtInput.tensOfMinutes = atoi(chInputbuffer);
            break;
        case 3:
            dtInput.minutes = atoi(chInputbuffer);
            break;
        }
      }
    }
//  }
  return (dtInput);
}
//========================================
// S t o r e D i g i t s
//
// This is used to initially store the
// numbers on the blank board.
//
// It will prompt for what type and digit 
// will then move it to it's home position
//========================================
void StoreDigits() {
  char chDigitRange, chDigit;
  int iDigit;

  while(1) {
    SerialUSB.print("\nMoving into position...");
    MoveTo(COL_7, UPPER_XFER_ROW, 70);
    EngageMagnet();

    SerialUSB.print("\nSelect one: 1) Tens of Hours, 2) Hours, 3) Tens of Minutes, 4) Minutes, 0) Exit:");
    chDigitRange = InputOneCharFromSerial();
    if (chDigitRange == '0') return;

    SerialUSB.print("\nDigit to place:");
    chDigit = InputOneCharFromSerial();
    iDigit = (int)(chDigit - '0');

    SerialUSB.print("\nDigit : ");
    SerialUSB.print(chDigit);

    switch(chDigitRange){
      case '0' :
          return;
          break;
      case '1' :
          SerialUSB.print("\nTens of Hours\n");
          MoveDigit(tensOfHours, iDigit, WAYPOINT_B, WAYPOINT_HOME);
          break;
      case '2' :
          SerialUSB.print("\nHours\n");
           MoveDigit(hours, iDigit, WAYPOINT_B, WAYPOINT_HOME);
         break;
      case '3' :
          SerialUSB.print("\nTens of Minutes\n");
          MoveDigit(tensOfMinutes, iDigit, WAYPOINT_B, WAYPOINT_HOME);
         break;
      case '4' :
          SerialUSB.print("\nMinutes\n");
         MoveDigit(minutes, iDigit, WAYPOINT_B, WAYPOINT_HOME);
          break;
    }
  }
}

