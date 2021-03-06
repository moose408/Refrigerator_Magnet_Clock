//==========================================================================
// Refrigerator Magnet Clock - Clock.cpp
//
// Copyright (c) 2020, Craig Colvin
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "ClockCoordinates.h"
#include "Clock.h"
#include "coreXY.h"
#include <Wire.h>
#include "ISR.h"
#include "RTClib.h"
#include "hardware.h"

RTC_PCF8523 rtcClock;
DateTime dtCurrentDisplayedTime;

const int MINS_GETDIGIT           = 0;
const int TENS_OF_MINS_GETDIGIT   = 1;
const int HOURS_GETDIGIT          = 2;
const int TENS_OF_HOURS_GETDIGIT  = 3;


//=========================================
// E n g a g e M a g n e t
//=========================================
void EngageMagnet() {
    SetServoPosition(MAGNET_ENGAGED_SERVO_POSTION);  
    delay(500);
}
//=========================================
// D i s e n g a g e M a g n e t
//=========================================
void DisengageMagnet() {
    SetServoPosition(MAGNET_DISENGAGED_SERVO_POSTION);  
    delay(1000);
  
}
//=========================================
// M o v e D i g i t
//
// This uses a Waypoint array to move
// from one series of waypoints to another
//=========================================
void MoveDigit(const Coordinates waypointsArray[][NUMBER_OF_WAYPOINTS], int iNumberToMove, int iStartPos, int iEndPos) {
    int iWaypointIndex;
    long x,y;


    SerialUSB.print("\nMoving digit: ");
    SerialUSB.print(iNumberToMove);
#if 0
    SerialUSB.print("\nFrom Waypoint: ");
    SerialUSB.print(iStartPos);
    SerialUSB.print(" to ");
    SerialUSB.print(iEndPos);
    SerialUSB.print("\n");
#endif

    if (iStartPos < iEndPos) {
      //count up
      for(iWaypointIndex=iStartPos; iWaypointIndex <= iEndPos; iWaypointIndex++) {
          x = waypointsArray[iNumberToMove][iWaypointIndex].x;
          y = waypointsArray[iNumberToMove][iWaypointIndex].y;
          MoveTo(waypointsArray[iNumberToMove][iWaypointIndex].x, waypointsArray[iNumberToMove][iWaypointIndex].y,MOVEDIGITSPEED);

          if (iWaypointIndex == iStartPos) {
            EngageMagnet(); 
          }
          if (iWaypointIndex == iEndPos) {
            DisengageMagnet(); 
          }
      }
    }
    else {
      //count down
       for(iWaypointIndex=iStartPos; iWaypointIndex >= iEndPos; iWaypointIndex--) {
          x = waypointsArray[iNumberToMove][iWaypointIndex].x;
          y = waypointsArray[iNumberToMove][iWaypointIndex].y;
          MoveTo(waypointsArray[iNumberToMove][iWaypointIndex].x, waypointsArray[iNumberToMove][iWaypointIndex].y,MOVEDIGITSPEED);
          if (iWaypointIndex == iStartPos) {
            EngageMagnet(); 
          }
          if (iWaypointIndex == iEndPos) {
            DisengageMagnet(); 
          }
      }
    }

}
//=========================================
// H a s A M i n u t e P a s s e d
//
// This expects both times to be in 24h
// notation
//=========================================
bool HasAMinutePassed() {
  DateTime dtNow = rtcClock.now();

  if ((dtNow.unixtime() - dtCurrentDisplayedTime.unixtime()) >= 60) 
    {
    return true;
    }
  else
    {
    return false; 
    }

}
//=========================================
// G e t D i g i t
//
// Returns a single digit out of the DateTime
//=========================================
int GetDigit(DateTime dtTime,int iDigitPos)
{
  switch(iDigitPos) 
  {
    case MINS_GETDIGIT:
      return dtTime.minute() % 10;
      break;
    case TENS_OF_MINS_GETDIGIT:
     return dtTime.minute() / 10;
     break;
    case HOURS_GETDIGIT:
      return dtTime.hour() % 10;
      break;
    case TENS_OF_HOURS_GETDIGIT:
      return dtTime.hour() / 10;
      break;  
  }
}
//==============================================
// R e s e t D i s p l a y e d T i m e
//
// Called at startup and will move the
// current displayed time digits back to their
// home position. 
// 
//==============================================
void ResetDisplayedTime(int iTensOfHours, int iHours, int iTensOfMinutes, int iMinutes) {

    dtCurrentDisplayedTime = DateTime(2020,1,1,(iTensOfHours * 10) + iHours,(iTensOfMinutes * 10) + iMinutes, 0 );
    if (iTensOfHours == 1) {
      MoveDigit(tensOfHours, iTensOfHours, WAYPOINT_CLOCK, WAYPOINT_HOME);
      }
    MoveDigit(hours, iHours, WAYPOINT_CLOCK, WAYPOINT_HOME);
    MoveDigit(tensOfMinutes, iTensOfMinutes, WAYPOINT_CLOCK, WAYPOINT_HOME);
    MoveDigit(minutes, iMinutes, WAYPOINT_CLOCK, WAYPOINT_HOME);

}
//=========================================
// R T C I n i t 
//=========================================
void RTCInit() {
    //------------
    // Setup RTC
    //------------
    rtcClock.begin();

 if (! rtcClock.initialized()) {
        SerialUSB.println("RTC is NOT running!");
      }
  rtcClock.adjust(DateTime(__DATE__, __TIME__));

    //dtCurrentDisplayedTime = rtcClock.now();
}
//===============================================
// C o n v e r t 2 4 h T i m e T o 1 2 h T i m e
//===============================================
DateTime Convert24hTimeTo12hTime(DateTime dtTime) {

      //Display time as 12h time as opposed to 24h
    if (dtTime.hour() >= 13 ) {
      return(DateTime(dtTime.year(),dtTime.month(),dtTime.day(),dtTime.hour() - 12,dtTime.minute(),dtTime.second()));
    }
}
//=========================================
// P r i n t T i m e
//=========================================
void PrintTime(DateTime dtTime) {
    SerialUSB.print(GetDigit(dtTime,TENS_OF_HOURS_GETDIGIT));
    SerialUSB.print(GetDigit(dtTime,HOURS_GETDIGIT));
    SerialUSB.print(":");
    SerialUSB.print(GetDigit(dtTime,TENS_OF_MINS_GETDIGIT));
    SerialUSB.print(GetDigit(dtTime,MINS_GETDIGIT));
    SerialUSB.print("\n");
}
//=============================================
// I n i t i a l i z e C l o c k D i s p l a y
//
// This is used at the beginning to populate
// the clock initially
//=============================================
void InitializeClockDisplay() {
  DateTime dtNewDisplayedTime;  
  
  dtCurrentDisplayedTime = rtcClock.now();

  //Display time as 12h time as opposed to 24h
  dtNewDisplayedTime = Convert24hTimeTo12hTime(dtCurrentDisplayedTime);

  SerialUSB.print("\n\nSetting initial time: ");
  PrintTime(dtNewDisplayedTime);
  
  MoveDigit(minutes, GetDigit(dtNewDisplayedTime,MINS_GETDIGIT),WAYPOINT_HOME,WAYPOINT_CLOCK);
  MoveDigit(tensOfMinutes, GetDigit(dtNewDisplayedTime,TENS_OF_MINS_GETDIGIT),WAYPOINT_HOME,WAYPOINT_CLOCK);
  MoveDigit(hours, GetDigit(dtNewDisplayedTime,HOURS_GETDIGIT),WAYPOINT_HOME,WAYPOINT_CLOCK);
  MoveDigit(tensOfHours, GetDigit(dtNewDisplayedTime,TENS_OF_HOURS_GETDIGIT),WAYPOINT_HOME,WAYPOINT_CLOCK);
}
//=========================================
// U p d a t e D i s p l a y T i m e
//
// This will get current time and move 
// any digits that have changed 
//=========================================
void UpdateDisplayTime() {
    DateTime dtNow, dtNewDisplayedTime;

    dtNow = rtcClock.now(); 

    // Check if minute has passed 
    if (! HasAMinutePassed()) {
      return;
    }

    //Display time as 12h time as opposed to 24h
    dtNewDisplayedTime = Convert24hTimeTo12hTime(dtNow);
    dtCurrentDisplayedTime = Convert24hTimeTo12hTime(dtCurrentDisplayedTime);

    //Just to clarify, at this point we have 3 DateTime variables
    //                 dtNow = current time read from RTC in 24h format
    //    dtNewDisplayedTime = current time read from RTC in 12h format
    //dtCurrentDisplayedTime = time that is currently being displayed and is going to be replaced

    SerialUSB.print("\n\nCurrently displayed time: ");
    PrintTime(dtCurrentDisplayedTime);
    SerialUSB.print("New time: ");
    PrintTime(dtNewDisplayedTime);
    
    // We know the minute has changed 
    // so swap digits
    MoveDigit(minutes, GetDigit(dtCurrentDisplayedTime,MINS_GETDIGIT),WAYPOINT_CLOCK,WAYPOINT_HOME);
    MoveDigit(minutes, GetDigit(dtNewDisplayedTime,MINS_GETDIGIT),WAYPOINT_HOME,WAYPOINT_CLOCK);

    //compare current 10s of minutes 
    if (GetDigit(dtNewDisplayedTime,TENS_OF_MINS_GETDIGIT) != GetDigit(dtCurrentDisplayedTime,TENS_OF_MINS_GETDIGIT)) {
        MoveDigit(tensOfMinutes, GetDigit(dtCurrentDisplayedTime,TENS_OF_MINS_GETDIGIT),WAYPOINT_CLOCK,WAYPOINT_HOME);
        MoveDigit(tensOfMinutes, GetDigit(dtNewDisplayedTime,TENS_OF_MINS_GETDIGIT),WAYPOINT_HOME,WAYPOINT_CLOCK);
        }

    //compare current hours 
    if (GetDigit(dtNewDisplayedTime,HOURS_GETDIGIT) != GetDigit(dtCurrentDisplayedTime,HOURS_GETDIGIT)) {
        MoveDigit(hours, GetDigit(dtCurrentDisplayedTime,HOURS_GETDIGIT),WAYPOINT_CLOCK,WAYPOINT_HOME);
        MoveDigit(hours, GetDigit(dtNewDisplayedTime,HOURS_GETDIGIT),WAYPOINT_HOME,WAYPOINT_CLOCK);
        }

    //compare current 10s of hours 
    if (GetDigit(dtNewDisplayedTime,TENS_OF_HOURS_GETDIGIT) != GetDigit(dtCurrentDisplayedTime,TENS_OF_HOURS_GETDIGIT)) {
        MoveDigit(tensOfHours, GetDigit(dtCurrentDisplayedTime,TENS_OF_HOURS_GETDIGIT),WAYPOINT_CLOCK,WAYPOINT_HOME);
        MoveDigit(tensOfHours, GetDigit(dtNewDisplayedTime,TENS_OF_HOURS_GETDIGIT),WAYPOINT_HOME,WAYPOINT_CLOCK);
        }

    //Set the current time to the new time in 24h format
    dtCurrentDisplayedTime = dtNow;


}
