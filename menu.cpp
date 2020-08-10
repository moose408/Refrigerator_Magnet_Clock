//==========================================================================
// Refrigerator Magnet Clock - Menu.cpp
//
// Copyright (c) 2020, Craig Colvin
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================


#include <arduino.h>

//========================================================
// I n p u t O n e C h a r F r o m S e r i a l
//========================================================
char InputOneCharFromSerial() {
    char receivedChar;

    do {
        //delayMilliseconds(10);
    } while (SerialUSB.available() == 0);

    if (SerialUSB.available() > 0) {
        receivedChar = SerialUSB.read();
        SerialUSB.flush();
    }
    return receivedChar;
}

//========================================================
// C l e a r S c r e e n
//========================================================
void ClearScreen() {
  SerialUSB.write(27);       // ESC command
  SerialUSB.print("[2J");    // clear screen command
  SerialUSB.write(27);
  SerialUSB.print("[H");     // cursor to home command

}
//========================================================
// S h o w M e n u
//========================================================
char ShowMenu() {
    ClearScreen();

    SerialUSB.print(F("Refrigerator Magnet Clock\n"));    
    SerialUSB.print(F("i - Initialize (Store Digits)\n"));
    SerialUSB.print(F("s - Start Clock\n"));
    SerialUSB.print(F("z - Zero X and Y\n"));

    return InputOneCharFromSerial();

}
