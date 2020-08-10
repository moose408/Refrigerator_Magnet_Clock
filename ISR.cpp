//==========================================================================
// Refrigerator Magnet Clock - ISR.cpp
//
// This handles all of the interrupts used to control the stepper and
// servo motors.
//
// Copyright (c) 2020, Craig Colvin
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================

#include "hardware.h"
#include "avdweb_SAMDtimer.h"
#include "ISR.h"
#include "AccelerationTable.h"
#include "coreXY.h"


//========================================
// ISR Globals
//========================================
volatile long g_lAlphaTickCount;     //we use this to initialize the count at the start of the ISR
volatile long g_lAlphaSteps;
volatile long g_lAlphaAccelIndex;    //pointer into Acceleration array
volatile long g_lAlphaFullspeedTicks;
volatile bool g_bAlphaComplete;      //Monitor this to know if motor run is completed

volatile long g_lBetaTickCount;     //we use this to initialize the count at the start of the ISR
volatile long g_lBetaSteps;
volatile long g_lBetaAccelIndex;    //pointer into Acceleration array
volatile long g_lBetaFullspeedTicks;
volatile bool g_bBetaComplete;      //Monitor this to know if motor run is completed

//Other Globals
#define START 1
#define ACCEL 2
#define FULLSPEED 3
#define DECEL 4
#define DONE 0

int alphaMotorCurrentState, betaMotorCurrentState;

//========================================================
// This is a hack to get rid of forward reference issues.
// because I call timer_Alpha and timer_Beta inside
// ISR. *** TODO ***
//========================================================
void ISR_Dummy(struct tc_module *const module_inst) 
{ 
}
//========================================
// G l o b a l T i m e r s
//========================================
// ------------- Timers with ISR -------------
SAMDtimer timer_Alpha = SAMDtimer(ALPHA_TIMER, ISR_Dummy, PERIOD_USEC); //5e3
SAMDtimer timer_Beta = SAMDtimer(BETA_TIMER, ISR_Dummy, PERIOD_USEC); 

SAMDtimer magnetServoTimer = SAMDtimer(SERVO_TIMER, TC_COUNTER_SIZE_16BIT, MAGNET_SERVO_PIN, PERIOD_20MS, SERVO_STARTING_PULSE_WIDTH);    

//=============================================
// I S R _ A l p h a _ A c c e l e r a t i o n
//=============================================
void ISR_Alpha_Acceleration(struct tc_module *const module_inst) {   

    //Write low whenever we come into ISR
    digitalWrite(ALPHA_STEP_PIN, 0);

    //Each time this ISR is called is a single tick. We countdown ticks and
    //when it reaches zero we raise the step pin high. That represents one step.
    //We then determine what the new tick count should be.
    g_lAlphaTickCount--;

    if (g_lAlphaTickCount == 0) {
        //time period has elapsed so do a step
        digitalWrite(ALPHA_STEP_PIN,1);

        //decrement step count
        g_lAlphaSteps--;

        //Check if we are done with current step count
        if (g_lAlphaSteps <= 0) {
            g_bAlphaComplete = true;            //done
            digitalWrite(ALPHA_STEP_PIN, 0);
            //timer_Alpha.enableInterrupt(0);   //disable interrupts they are already I believe
            //timer_Alpha.enableTimer(0);         //DEBUG DEBUG Check if this is allowed inside ISR ################### BUG ##########################
            return;             
        }

        //if not done fetch new tick count
        g_lAlphaTickCount = iAccelTable[g_lAlphaAccelIndex];
        g_lAlphaAccelIndex++;
    }

}
//=============================================
// I S R _ A l p h a _ D e c c e l e r a t i o n
//=============================================
void ISR_Alpha_Deceleration(struct tc_module *const module_inst) {   

    //Write low whenever we come into ISR
    digitalWrite(ALPHA_STEP_PIN, 0);

    //Each time this ISR is called is a single tick. We countdown ticks and
    //when it reaches zero we raise the step pin high. That represents one step.
    //We then determine what the new tick count should be.
    g_lAlphaTickCount--;

    if (g_lAlphaTickCount == 0) {
        //time period has elapsed so do a step
        digitalWrite(ALPHA_STEP_PIN,1);

        //decrement step count
        g_lAlphaSteps--;

        //Check if we are done with current step count
        if (g_lAlphaSteps <= 0) {
            g_bAlphaComplete = true;            //done
            digitalWrite(ALPHA_STEP_PIN, 0);

            //timer_Alpha.enableInterrupt(0);   //disable interrupts they are already I believe
            //timer_Alpha.enableTimer(0);         //DEBUG DEBUG Check if this is allowed inside ISR ################### BUG ##########################
            return;             
        }

        //if not done fetch new tick count
        g_lAlphaAccelIndex--;
        g_lAlphaTickCount = iAccelTable[g_lAlphaAccelIndex];
    }

}
//=============================================
// I S R _ A l p h a _ F u l l s p e e d
//=============================================
void ISR_Alpha_Fullspeed(struct tc_module *const module_inst) {   

    //Write low whenever we come into ISR
    digitalWrite(ALPHA_STEP_PIN, 0);

    //Each time this ISR is called is a single tick. We countdown ticks and
    //when it reaches zero we raise the step pin high. That represents one step.
    //We then determine what the new tick count should be.
    g_lAlphaTickCount--;

    if (g_lAlphaTickCount == 0) {
        //time period has elapsed so do a step
        digitalWrite(ALPHA_STEP_PIN,1);

        //decrement step count
        g_lAlphaSteps--;
 
         //Check if we are done with current step count
        if (g_lAlphaSteps <= 0 ) {
            g_bAlphaComplete = true;            //done
            digitalWrite(ALPHA_STEP_PIN, 0);
            //timer_Alpha.enableInterrupt(0);   //disable interrupts they are already I believe
            //timer_Alpha.enableTimer(0);
           return;             
        }

        //if more steps reset tick count
        g_lAlphaTickCount = g_lAlphaFullspeedTicks;
     }
}//=============================================
// I S R _ B e t a _ A c c e l e r a t i o n
//=============================================
void ISR_Beta_Acceleration(struct tc_module *const module_inst) {   

    //Write low whenever we come into ISR
    digitalWrite(BETA_STEP_PIN, 0);

    //Each time this ISR is called is a single tick. We countdown ticks and
    //when it reaches zero we raise the step pin high. That represents one step.
    //We then determine what the new tick count should be.
    g_lBetaTickCount--;

    if (g_lBetaTickCount == 0) {
        //time period has elapsed so do a step
        digitalWrite(BETA_STEP_PIN,1);

        //decrement step count
        g_lBetaSteps--;

        //Check if we are done with current step count
        if (g_lBetaSteps <= 0) {
            g_bBetaComplete = true;            //done
            digitalWrite(BETA_STEP_PIN, 0);
            //timer_Beta.enableInterrupt(0);   //disable interrupts they are already I believe
            //timer_Beta.enableTimer(0);         //DEBUG DEBUG Check if this is allowed inside ISR ################### BUG ##########################
            return;             
        }

        //if not done fetch new tick count
        g_lBetaTickCount = iAccelTable[g_lBetaAccelIndex];
        g_lBetaAccelIndex++;
    }

}
//=============================================
// I S R _ B e t a _ D e c c e l e r a t i o n
//=============================================
void ISR_Beta_Deceleration(struct tc_module *const module_inst) {   

    //Write low whenever we come into ISR
    digitalWrite(BETA_STEP_PIN, 0);

    //Each time this ISR is called is a single tick. We countdown ticks and
    //when it reaches zero we raise the step pin high. That represents one step.
    //We then determine what the new tick count should be.
    g_lBetaTickCount--;

    if (g_lBetaTickCount == 0) {
        //time period has elapsed so do a step
        digitalWrite(BETA_STEP_PIN,1);

        //decrement step count
        g_lBetaSteps--;

        //Check if we are done with current step count
        if (g_lBetaSteps <= 0) {
            g_bBetaComplete = true;            //done
             digitalWrite(BETA_STEP_PIN, 0);
           //timer_Beta.enableInterrupt(0);   //disable interrupts they are already I believe
            //timer_Beta.enableTimer(0);         //DEBUG DEBUG Check if this is allowed inside ISR ################### BUG ##########################
            return;             
        }

        //if not done fetch new tick count
        g_lBetaAccelIndex--;
        g_lBetaTickCount = iAccelTable[g_lBetaAccelIndex];
    }

}
//=============================================
// I S R _ B e t a _ F u l l s p e e d
//=============================================
void ISR_Beta_Fullspeed(struct tc_module *const module_inst) {   

    //Write low whenever we come into ISR
    digitalWrite(BETA_STEP_PIN, 0);

    //Each time this ISR is called is a single tick. We countdown ticks and
    //when it reaches zero we raise the step pin high. That represents one step.
    //We then determine what the new tick count should be.
    g_lBetaTickCount--;

    if (g_lBetaTickCount == 0) {
        //time period has elapsed so do a step
        digitalWrite(BETA_STEP_PIN,1);

        //decrement step count
        g_lBetaSteps--;

        //Check if we are done with current step count
        if (g_lBetaSteps <= 0) {
            g_bBetaComplete = true;            //done
            digitalWrite(BETA_STEP_PIN, 0);
            //timer_Beta.enableInterrupt(0);   //disable interrupts they are already I believe
            //timer_Beta.enableTimer(0);
           return;             
        }

        //if more steps reset tick count
        g_lBetaTickCount = g_lBetaFullspeedTicks;
     }
}
//=============================================================================
// S t a r t A l p h a A c c e l e r a t i o n D e c  e l e r a t i o n 
//
// This does all of the initialization for the Acceleration ISR and then 
// starts the interrupt which will send the steps to the Alpha motor.
//=============================================================================
void StartAlphaAccelerationDeceleration(bool bAccelerate, long lSteps) {

    timer_Alpha.enableInterrupt(0);     //disable interrupts

    //Initial variables
    g_lAlphaSteps = lSteps;
    g_bAlphaComplete = false;

    // Are we accelerating or decelerating?
    if (bAccelerate) {
        g_lAlphaAccelIndex = 1;                  //start at beginning of array
        g_lAlphaTickCount = iAccelTable[0];

        //attach Acceleration Interrupt
        timer_Alpha.attachInterrupt(ISR_Alpha_Acceleration); 
    }
    else {
        //Setup for deceleration
        g_lAlphaAccelIndex = lSteps;                  //start at end of array
        g_lAlphaTickCount = iAccelTable[lSteps];

        //attach Acceleration Interrupt
        timer_Alpha.attachInterrupt(ISR_Alpha_Deceleration); 
    }
    
    //Start the timer
    timer_Alpha.enableTimer(1);

    //Interrupts on
    timer_Alpha.enableInterrupt(1);
}
//=============================================================================
// S t a r t A l p h a F u l l s p e e d
//=============================================================================
void StartAlphaFullspeed(long lFullspeedSteps, long lFullspeedTicks) {

    //Interrupts off
    timer_Alpha.enableInterrupt(0);

    //Initial variables
    g_lAlphaSteps = lFullspeedSteps;
    g_bAlphaComplete = false;

    g_lAlphaFullspeedTicks = lFullspeedTicks;
    g_lAlphaTickCount = lFullspeedTicks;

    //attach Fullspeed Interrupt
    timer_Alpha.attachInterrupt(ISR_Alpha_Fullspeed); 

    //Start the timer
    timer_Alpha.enableTimer(1);

    //Interrupts on
    timer_Alpha.enableInterrupt(1);

}
//=============================================================================
// S t a r t B e t a A c c e l e r a t i o n D e c c e l e r a t i o n 
//
// This does all of the initialization for the Acceleration ISR and then 
// starts the interrupt which will send the steps to the Beta motor.
//=============================================================================
void StartBetaAccelerationDeceleration(bool bAccelerate, long lSteps) {

    timer_Beta.enableInterrupt(0);     //disable interrupts

    //Initial variables
    g_lBetaSteps = lSteps;
    g_bBetaComplete = false;

    // Are we accelerating or decelerating?
    if (bAccelerate) {
        g_lBetaAccelIndex = 1;                  //start at beginning of array
        g_lBetaTickCount = iAccelTable[0];

        //attach Acceleration Interrupt
        timer_Beta.attachInterrupt(ISR_Beta_Acceleration); 
    }
    else {
        //Setup for deceleration
        g_lBetaAccelIndex = lSteps;                  //start at end of array
        g_lBetaTickCount = iAccelTable[lSteps];

        //attach Acceleration Interrupt
        timer_Beta.attachInterrupt(ISR_Beta_Deceleration); 
    }
    
    //Start the timer
    timer_Beta.enableTimer(1);

    //Interrupts on
    timer_Beta.enableInterrupt(1);
}
//=============================================================================
// S t a r t B e t a F u l l s p e e d
//=============================================================================
void StartBetaFullspeed(long lFullspeedSteps, long lFullspeedTicks) {

    //Interrupts off
    timer_Beta.enableInterrupt(0);

    //Initial variables
    g_lBetaSteps = lFullspeedSteps;
    g_bBetaComplete = false;

    g_lBetaFullspeedTicks = lFullspeedTicks;
    g_lBetaTickCount = lFullspeedTicks;

    //attach Fullspeed Interrupt
    timer_Beta.attachInterrupt(ISR_Beta_Fullspeed); 

    //Start the timer
    timer_Beta.enableTimer(1);

    //Interrupts on
    timer_Beta.enableInterrupt(1);
}
//========================================
// R u n M o t o r I S R 
//========================================
void RunMotorISR(long lAlphaSteps, long lBetaSteps, int iSpeed) {
    long lAlphaNumberOfAccelSteps = 0, lBetaNumberOfAccelSteps = 0;
    long lAlphaNumberOfFullSpeedSteps = 0, lBetaNumberOfFullSpeedSteps = 0;
    long lAlphaRunTime, lBetaRunTime;
    long lAlphaFullspeedTicksPerStep, lBetaFullspeedTicksPerStep;

    CalculateAccelerationValues(&lAlphaNumberOfAccelSteps,&lAlphaNumberOfFullSpeedSteps,lAlphaSteps);
    CalculateAccelerationValues(&lBetaNumberOfAccelSteps,&lBetaNumberOfFullSpeedSteps,lBetaSteps);

    //-------------------------------------------------------
    // Determine which has more steps and make that primary
    // then set secondary ticks/step to match the total 
    // time used by the primary.
    //
    // This will result in both motors running at different
    // speeds but the total travel time being equal.
    //-------------------------------------------------------
    if (lAlphaSteps > lBetaSteps) {
        //------------------------
        //Alpha motor is primary
        //------------------------
        //SerialUSB.print("==== Alpha is Primary ====\n");
        lAlphaFullspeedTicksPerStep = iSpeed / PERIOD_USEC; 
        lAlphaRunTime = ((iAccelCummulativeTime[lAlphaNumberOfAccelSteps] * PERIOD_USEC * 2) + (lAlphaNumberOfFullSpeedSteps * iSpeed));
        lBetaFullspeedTicksPerStep = (lAlphaRunTime - (2 * iAccelCummulativeTime[lAlphaNumberOfAccelSteps] * PERIOD_USEC)) / lBetaNumberOfFullSpeedSteps / PERIOD_USEC;
        //lBetaFullspeedTicksPerStep++;
        lBetaRunTime = ((2 * iAccelCummulativeTime[lAlphaNumberOfAccelSteps]) + (lBetaFullspeedTicksPerStep * lBetaNumberOfFullSpeedSteps)) * PERIOD_USEC;
     }
    else {
        //----------------------
        //Beta motor is primary
        //----------------------
        //SerialUSB.print("==== Beta is Primary ====\n");
        lBetaFullspeedTicksPerStep = iSpeed / PERIOD_USEC; 
        lBetaRunTime = ((iAccelCummulativeTime[lBetaNumberOfAccelSteps] * PERIOD_USEC * 2) + (lBetaNumberOfFullSpeedSteps * iSpeed));
        lAlphaFullspeedTicksPerStep = (lBetaRunTime - (2 * iAccelCummulativeTime[lBetaNumberOfAccelSteps]) * PERIOD_USEC) / lAlphaNumberOfFullSpeedSteps / PERIOD_USEC;
        //lAlphaFullspeedTicksPerStep++;
        lAlphaRunTime = ((2 * iAccelCummulativeTime[lBetaNumberOfAccelSteps]) + (lAlphaFullspeedTicksPerStep * lAlphaNumberOfFullSpeedSteps)) * PERIOD_USEC;
     }
#if 0     
    SerialUSB.print("\nTotal Steps : "); SerialUSB.print(lAlphaSteps); SerialUSB.print(","); SerialUSB.print(lBetaSteps);
    SerialUSB.print("\nAcceleraton Steps : "); SerialUSB.print(lAlphaNumberOfAccelSteps);SerialUSB.print(","); SerialUSB.print(lBetaNumberOfAccelSteps);
    SerialUSB.print("\nAcceleraton Time from table : "); SerialUSB.print(iAccelCummulativeTime[lAlphaNumberOfAccelSteps]); SerialUSB.print(","); SerialUSB.print(iAccelCummulativeTime[lBetaNumberOfAccelSteps]);
    SerialUSB.print("\nFull Speed Steps : "); SerialUSB.print(lAlphaNumberOfFullSpeedSteps);SerialUSB.print(",");SerialUSB.print(lBetaNumberOfFullSpeedSteps);
    SerialUSB.print("\n");
    SerialUSB.print("Run Time (prim/secondary): ");  SerialUSB.print(lAlphaRunTime); SerialUSB.print(",");SerialUSB.print(lBetaRunTime);
    SerialUSB.print(" uSec\nFullspeed ticks/step : ");  SerialUSB.print(lAlphaFullspeedTicksPerStep);SerialUSB.print(",");SerialUSB.print(lBetaFullspeedTicksPerStep);
    SerialUSB.print("\n");
#endif

    //Initialze Motor State
    alphaMotorCurrentState = START;
    betaMotorCurrentState = START;

    //Make sure we process initial START state 
    g_bAlphaComplete = true;
    g_bBetaComplete = true;

    //If the motor isn't doing any steps then it is done 
    if (lAlphaSteps == 0) {
        alphaMotorCurrentState = DONE;
    }    

    if (lBetaSteps == 0) {
        betaMotorCurrentState = DONE;
    //g_bBetaComplete = false;
    }

    //SerialUSB.print("\nCurrent Motor States: "); SerialUSB.print(alphaMotorCurrentState); SerialUSB.print(",");SerialUSB.print(betaMotorCurrentState); SerialUSB.print("\n");


    //Main Loop 
    while ((alphaMotorCurrentState != DONE) || (betaMotorCurrentState != DONE)) {

        //Run Alpha Motor State Machine    
        if (g_bAlphaComplete) {
            switch(alphaMotorCurrentState) {
                case START:
                     StartAlphaAccelerationDeceleration(true, lAlphaNumberOfAccelSteps);
                    alphaMotorCurrentState = ACCEL; 
                    digitalWrite(12,LOW);       //DEBUG DEBUG
                    break;
                case ACCEL:
                     //if no fullspeed steps then skip to Decel.
                    if (lAlphaNumberOfFullSpeedSteps == 0) {
                        StartAlphaAccelerationDeceleration(false, lAlphaNumberOfAccelSteps);
                        alphaMotorCurrentState = DECEL; 
                        }
                    else {
                        StartAlphaFullspeed(lAlphaNumberOfFullSpeedSteps,lAlphaFullspeedTicksPerStep);
                        alphaMotorCurrentState = FULLSPEED;
                        digitalWrite(12,HIGH);       //DEBUG DEBUG
                   }
                    break;
                case FULLSPEED:
                    StartAlphaAccelerationDeceleration(false, lAlphaNumberOfAccelSteps);
                    alphaMotorCurrentState = DECEL;
                    digitalWrite(12,LOW);       //DEBUG DEBUG
                    break;
                case DECEL:
                    alphaMotorCurrentState = DONE;
                    break;
            }
        }

        //Run Beta Motor State Machine
        if (g_bBetaComplete) {
            switch(betaMotorCurrentState) {
                case START:
                    StartBetaAccelerationDeceleration(true, lBetaNumberOfAccelSteps);
                    betaMotorCurrentState = ACCEL;
                    digitalWrite(12,LOW);       //DEBUG DEBUG
                    break;
                case ACCEL:
                    //if no fullspeed steps then skip to Decel.
                    if (lBetaNumberOfFullSpeedSteps == 0) {
                        StartBetaAccelerationDeceleration(false, lBetaNumberOfAccelSteps);
                        betaMotorCurrentState = DECEL; 
                        }
                    else {
                        StartBetaFullspeed(lBetaNumberOfFullSpeedSteps,lBetaFullspeedTicksPerStep);
                        betaMotorCurrentState = FULLSPEED;
                        digitalWrite(12,HIGH);       //DEBUG DEBUG
                   }
                    break;
                case FULLSPEED:
                    StartBetaAccelerationDeceleration(false, lBetaNumberOfAccelSteps);
                    betaMotorCurrentState = DECEL;
                    digitalWrite(12,LOW);       //DEBUG DEBUG
                    break;
                case DECEL:
                    betaMotorCurrentState = DONE;
                    break;
            }
        }


    } // while alphaMotorCurrentState && betaMotorCurrentState

}
//========================================
// S e t S e r v o P o s i t i o n
//========================================
void SetServoPosition(int iServoPosDegrees) {
    int i_uSec;

    //------------------
    // 0 deg = .5 ms
    // 90 deg = 1.5ms
    // 180 deg = 2.5ms 
    //------------------

    //----------------------------------------
    // The constant 111 is derived as follows
    // (180* value - 0* value) = 2000us
    // 2000us / 180 = 11.1
    // multiply by 10 to get rid of decimal
    //----------------------------------------

    i_uSec = ((iServoPosDegrees * 111) / 10) + 500;    
    magnetServoTimer.setPulseWidth(i_uSec);

}
