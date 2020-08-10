//==========================================================================
// Refrigerator Magnet Clock - Clock.h
//
// Copyright (c) 2020, Craig Colvin
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================

#include "ClockCoordinates.h"

#ifndef CLOCK_H
#define CLOCK_H

// This is the speed that a magnetic number will be moved 
// this was found empirically and slowed down until horizontal
// movements didn't cause the digit to drag
#define MOVEDIGITSPEED      250

// Servo degrees for engaging and diengaging the magnet
#define MAGNET_ENGAGED_SERVO_POSTION        25
#define MAGNET_DISENGAGED_SERVO_POSTION     135

void MoveDigit(const Coordinates waypointsArray[][NUMBER_OF_WAYPOINTS], int iNumberToMove, int iStartPos, int iEndPos);
void ResetDisplayedTime(int iTensOfHours, int iHours, int iTensOfMinutes, int iMinutes);
void InitializeClockDisplay();
void UpdateDisplayTime();
void RTCInit();
void EngageMagnet();
void DisengageMagnet();


#endif //CLOCK_H
