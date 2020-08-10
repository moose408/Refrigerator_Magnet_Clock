//==========================================================================
// Refrigerator Magnet Clock - CoreXY.h
//
// Copyright (c) 2020, Craig Colvin
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================

#define ACCELERATION_PERCENTAGE     20
#define DECCELERATION_PERCENTAGE    20
#define MIN_ACCELERATION_STEPS      15
#define STARTING_SPEED              4000
#define MAX_SPEED_MS                20

#define MAX_X     40500
#define MAX_Y     30000

void CalculateAccelerationValues(long *lNumberOfAccelSteps, long *lFullSpeedSteps, long lSteps);

void SetAlphaDirection(int iDirection);
void SetBetaDirection(int iDirection);
void EnableSteppers();
void DisableSteppers();

void RunMotors(long lSteps, int iSpeed);
void MoveTo(long ulNewX, long ulNewY, int iSpeed);
void MoveLeft(long lSteps, int iSpeed);
void MoveRight(long lSteps, int iSpeed);
void MoveUp(long lSteps, int iSpeed);
void MoveDown(long lSteps, int iSpeed);

void ZeroXandY();
bool ReadXZeroSwitch();
bool ReadYZeroSwitch();
