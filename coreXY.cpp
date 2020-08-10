//==========================================================================
// Refrigerator Magnet Clock - CoreXY.cpp
//
// Copyright (c) 2020, Craig Colvin
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================

// Routines for moving the motors in the CoreXY setup

#include <Arduino.h>
#include "hardware.h"
#include "coreXY.h"
#include "AccelerationTable.h"
#include "ISR.h"
#include "Clock.h"

long xCoordinate;
long yCoordinate;

int iAlphaDirection;
int iBetaDirection;


//========================================
// S e t A l p h a D i r e c t i o n
//========================================
void SetAlphaDirection(int iDirection) {
      digitalWrite(X_DIR_PIN, iDirection);  
      iAlphaDirection = iDirection; 
}
//========================================
// S e t B e t a D i r e c t i o n 
//========================================
void SetBetaDirection(int iDirection) {
      digitalWrite(Y_DIR_PIN, iDirection); 
      iBetaDirection = iDirection;
}
//========================================
// R e a d X Z e r o S w i t c h
//========================================
bool ReadXZeroSwitch() {
    return (bool) digitalRead(ZERO_X_SWITCH_PIN);
}
//========================================
// R e a d Y Z e r o S w i t c h
//========================================
bool ReadYZeroSwitch() {
    return (bool) digitalRead(ZERO_Y_SWITCH_PIN);
}
//========================================
// E n a b l e S t e p p e r s
//========================================
void EnableSteppers() {
      digitalWrite(NOT_ENABLE_PIN, LOW);   
}
//========================================
// D i s a b l e S t e p p e r s
//========================================
void DisableSteppers() {
      digitalWrite(NOT_ENABLE_PIN, HIGH);   
}
//========================================
// R u n M o t o r s
// used by zero XY
//========================================
void RunMotors(long lSteps, int iSpeed) {
    long lNumberOfAccelSteps;
    long lNumberOfFullSpeedSteps;

if (lSteps < (ACCEL_TABLE_SIZE*2)) {
        lNumberOfAccelSteps = lSteps / 2;
        lNumberOfFullSpeedSteps = 0;
    }
    else
    {
        lNumberOfAccelSteps = ACCEL_TABLE_SIZE;
        lNumberOfFullSpeedSteps = lSteps - (ACCEL_TABLE_SIZE * 2);
    }
    
    //EnableSteppers();

    // Accelerate
     for (long x = 0; x < lNumberOfAccelSteps-1; x++) {
        digitalWrite(ALPHA_STEP_PIN, HIGH);
        digitalWrite(BETA_STEP_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(ALPHA_STEP_PIN, LOW);
        digitalWrite(BETA_STEP_PIN, LOW);
        delayMicroseconds(iAccelTable[x]);
        //SerialUSB.print(iAccelTable[x]);
        //SerialUSB.print(",");
    }

    // Run at speed
    for(long x = 0; x < lNumberOfFullSpeedSteps; x++) {
        digitalWrite(ALPHA_STEP_PIN, HIGH);
        digitalWrite(BETA_STEP_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(ALPHA_STEP_PIN, LOW);
        digitalWrite(BETA_STEP_PIN, LOW);
        delayMicroseconds(iSpeed);
       // SerialUSB.print(iSpeed);
        //SerialUSB.print(",");
   }

    // Deccelerate
    for (long x = lNumberOfAccelSteps-1; x > 0; x--) {
        digitalWrite(ALPHA_STEP_PIN, HIGH);
        digitalWrite(BETA_STEP_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(ALPHA_STEP_PIN, LOW);
        digitalWrite(BETA_STEP_PIN, LOW);
        delayMicroseconds(iAccelTable[x]);
        //SerialUSB.print(iAccelTable[x]);
        //SerialUSB.print(",");
    
    }
 
   DisableSteppers();
}
//=======================================================
// C a l c u l a t e A c c e l e r a t i o n V a l u e s
//=======================================================
void CalculateAccelerationValues(long *lNumberOfAccelSteps, long *lFullSpeedSteps, long lSteps) {

    if (lSteps< (ACCEL_TABLE_SIZE*2)) {
            *lNumberOfAccelSteps = lSteps / 2;
            *lFullSpeedSteps = 0;
        }
        else
        {
            *lNumberOfAccelSteps = ACCEL_TABLE_SIZE;
            *lFullSpeedSteps = lSteps - (ACCEL_TABLE_SIZE * 2);
        }
    
}
//========================================
// R u n M o t o r s 2
//========================================
void RunMotors2(long lPrimarySteps, long lPrimaryAccel, int iPrimaryPin,
               int iSecondaryPin, float fStepRatio, int iSpeed) {

    long lDebugPrimarySteps, lDebugSecondarySteps;

    lDebugPrimarySteps = 0;
    lDebugSecondarySteps = 0;

    //Check for 0 because we do a modulo with this and don't want
    //to divide by 0
    if (fStepRatio == 0) {
        fStepRatio = MAX_X;
    }
    
    //Acceleration
    for (long i=1;i < lPrimaryAccel+1; i++) {
        //Step primary
        digitalWrite(iPrimaryPin, HIGH);
        if (fmod(i,fStepRatio) != 0) {
            digitalWrite(iSecondaryPin, HIGH); 
            lDebugSecondarySteps++; 
        }
        delayMicroseconds(10);
        digitalWrite(iPrimaryPin, LOW);
        digitalWrite(iSecondaryPin, LOW);
        delayMicroseconds(iAccelTable[i]);
        lDebugPrimarySteps++;
    }

    //Run at Speed
    for (long i=1;i < lPrimarySteps+1; i++) {
        //Step primary
        digitalWrite(iPrimaryPin, HIGH);
        if (fmod(i,fStepRatio) != 0) {
            digitalWrite(iSecondaryPin, HIGH); 
            lDebugSecondarySteps++; 
        }
        delayMicroseconds(10);
        digitalWrite(iPrimaryPin, LOW);
        digitalWrite(iSecondaryPin, LOW);
        delayMicroseconds(iSpeed);
        lDebugPrimarySteps++;
    }

    //Decceleration
    for (long i=lPrimaryAccel;i > 0; i--) {
        //Step primary
        digitalWrite(iPrimaryPin, HIGH);
        if (fmod(i,fStepRatio) != 0) {
            digitalWrite(iSecondaryPin, HIGH);
            lDebugSecondarySteps++;  
        }
        delayMicroseconds(10);
        digitalWrite(iPrimaryPin, LOW);
        digitalWrite(iSecondaryPin, LOW);
        delayMicroseconds(iAccelTable[i]);
         lDebugPrimarySteps++;
   }

    SerialUSB.print("\nTotal Primary Steps : ");
    SerialUSB.print(lDebugPrimarySteps);
    SerialUSB.print("\nTotal Secondary Steps : ");
    SerialUSB.print(lDebugSecondarySteps);
    SerialUSB.print("\n");

}
//========================================
// R u n M o t o r s I n d e p e n d e n t
//========================================
void RunMotorsIndependent(long lAlphaSteps, long lBetaSteps, int iSpeed) {
    long lAlphaNumberOfAccelSteps = 0, lBetaNumberOfAccelSteps = 0;
    long lAlphaNumberOfFullSpeedSteps = 0, lBetaNumberOfFullSpeedSteps = 0;
    float fStepRatio;

 
    CalculateAccelerationValues(&lAlphaNumberOfAccelSteps,&lAlphaNumberOfFullSpeedSteps,lAlphaSteps);
    CalculateAccelerationValues(&lBetaNumberOfAccelSteps,&lBetaNumberOfFullSpeedSteps,lBetaSteps);

 #if 0  
    SerialUSB.print("\n\nRunMotorsIndependent() - Total Steps : ");
    SerialUSB.print(lAlphaSteps);
    SerialUSB.print(",");
    SerialUSB.print(lBetaSteps);
    SerialUSB.print("\nAcceleraton Steps : ");
    SerialUSB.print(lAlphaNumberOfAccelSteps);
    SerialUSB.print(",");
    SerialUSB.print(lBetaNumberOfAccelSteps);
    SerialUSB.print("\nFull Speed Steps : ");
    SerialUSB.print(lAlphaNumberOfFullSpeedSteps);
    SerialUSB.print(",");
    SerialUSB.print(lBetaNumberOfFullSpeedSteps);
    SerialUSB.print("\n");
    DebugPrintDirection(lAlphaSteps,lBetaSteps);
#endif

    //Determine which has more steps and make that primary
    if (lAlphaSteps > lBetaSteps) {
        //Alpha motor is primary
        fStepRatio = (float)lAlphaSteps / (float)lBetaSteps;
        SerialUSB.print("Alpha is primary. StepRatio = ");
        SerialUSB.print(fStepRatio);
        //RunMotors(long lPrimarySteps, long lPrimaryAccel, int iPrimaryPin, int iSecondaryPin, long lStepRatio, int iSpeed)
        RunMotors2(lAlphaNumberOfFullSpeedSteps, lAlphaNumberOfAccelSteps, ALPHA_STEP_PIN, BETA_STEP_PIN, fStepRatio, iSpeed);
    }
    else {
        //Beta motor is primary
        fStepRatio = (float)lBetaSteps / (float)lAlphaSteps;
        SerialUSB.print("Beta is primary. StepRatio = ");
        SerialUSB.print(fStepRatio);
        RunMotors2(lBetaNumberOfFullSpeedSteps, lBetaNumberOfAccelSteps, BETA_STEP_PIN, ALPHA_STEP_PIN, fStepRatio, iSpeed);
    }

   DisableSteppers();

}
//========================================
// M o v e L e f t
//========================================
void MoveLeft(long lSteps, int iSpeed) {
    SetAlphaDirection(CW);
    SetBetaDirection(CW);
 
    yCoordinate = yCoordinate - lSteps;

   RunMotors(lSteps, iSpeed);
}
//========================================
// M o v e R i g h t
//========================================
void MoveRight(long lSteps, int iSpeed) {
    SetAlphaDirection(CCW);
    SetBetaDirection(CCW);

    yCoordinate = yCoordinate + lSteps;

    RunMotors(lSteps, iSpeed);
}
//========================================
// M o v e U p 
//========================================
void MoveUp(long lSteps, int iSpeed) {
    SetAlphaDirection(CCW);
    SetBetaDirection(CW);

    xCoordinate = xCoordinate + lSteps;

    RunMotors(lSteps, iSpeed);
}
//========================================
// M o v e D o w n
//========================================
void MoveDown(long lSteps, int iSpeed) {
    SetAlphaDirection(CW);
    SetBetaDirection(CCW);

    xCoordinate = xCoordinate - lSteps;

    RunMotors(lSteps, iSpeed);
}
//========================================
// Z e r o X a n d Y
//========================================
void ZeroXandY() {

    SerialUSB.print("Zeroing\n");
    DisengageMagnet();
    MoveRight(1500,100);

    //--------------------
    // Zero X 
    //--------------------
    //quick move
    while(ReadXZeroSwitch() == 1) {
        MoveDown(10,100);
    }

    // Now move off switch 1 step at a time
   while(ReadXZeroSwitch() == 0) {
        MoveUp(10,100);
    }
    SerialUSB.print("Found X Zero\n");

   //--------------------
    // Zero Y  
    //--------------------
    //quick move
    while(ReadYZeroSwitch() == 1) {
        MoveLeft(10,100);
    }

    // Now move off switch 1 step at a time
   while(ReadYZeroSwitch() == 0) {
        MoveRight(10,100);
    }
    SerialUSB.print("Found Y Zero\n");


    xCoordinate = 0;
    yCoordinate = 0;
}
//========================================
// M o v e T o 
//========================================
void MoveTo(long lNewX, long lNewY, int iSpeed) {
    long lXSteps, lYSteps;
    long lDeltaAlpha, lDeltaBeta;

    lXSteps = xCoordinate - lNewX;
    lYSteps = yCoordinate - lNewY;

    lDeltaAlpha = lXSteps + lYSteps;
    lDeltaBeta = lYSteps - lXSteps;

#if 0 
    SerialUSB.print("\n\n---------------------\nDesired x,y: ");
    SerialUSB.print(lNewX);
    SerialUSB.print(",");
    SerialUSB.print(lNewY);
    SerialUSB.print("  starting x,y: ");
    SerialUSB.print(xCoordinate);
    SerialUSB.print(",");
    SerialUSB.print(yCoordinate);
    SerialUSB.print("  Delta x,y: ");
    SerialUSB.print(lXSteps);
    SerialUSB.print(",");
    SerialUSB.print(lYSteps);

    SerialUSB.print("\nDeltaAlpha:");
    SerialUSB.print(lDeltaAlpha);
    SerialUSB.print("  DeltaBeta:");
    SerialUSB.print(lDeltaBeta);
    SerialUSB.print("\n");
#endif

    //Determine rotation direction for Alpha motor
    if (lDeltaAlpha < 0) {
        SetAlphaDirection(CCW);   
    }
    else
    {
        SetAlphaDirection(CW);
    }


    //Determine rotation direction for Beta motor
    if (lDeltaBeta < 0) {
        SetBetaDirection(CCW);
    }
    else
    {
        SetBetaDirection(CW);
    }
    
    //RunMotorsIndependent(abs(lDeltaAlpha), abs(lDeltaBeta), iSpeed);
    RunMotorISR(abs(lDeltaAlpha), abs(lDeltaBeta), iSpeed);

    //Save new coordinates
    xCoordinate = lNewX;
    yCoordinate = lNewY;
 
}
