//==========================================================================
// Refrigerator Magnet Clock - ClockCoordinates.cpp
//
// Copyright (c) 2020, Craig Colvin
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================


#include <avr/pgmspace.h>
#include "ClockCoordinates.h"

//========================================
// Waypoint Arrays
//
// Look in header file for description
//========================================
const Coordinates minutes[][NUMBER_OF_WAYPOINTS] PROGMEM = {
  {{COL_4,ROW_1},{COL_4,ROW_1},{COL_4,ROW_4},{COL_4,ROW_4}},                        // digit 0 waypoints
  {{COL_3,ROW_1},{COL_3,UPPER_XFER_ROW},{COL_4,UPPER_XFER_ROW},{COL_4,ROW_4}},      // digit 1 waypoints
  {{COL_2,ROW_1},{COL_2,UPPER_XFER_ROW},{COL_4,UPPER_XFER_ROW},{COL_4,ROW_4}},      // digit 2 waypoints
  {{COL_1,ROW_2},{COL_4,ROW_2},{COL_4,ROW_4},{COL_4,ROW_4}},                        // digit 3 waypoints
  {{COL_1,ROW_3},{COL_4,ROW_3},{COL_4,ROW_4},{COL_4,ROW_4}},                        // digit 4 waypoints
  {{COL_1,ROW_5},{COL_4,ROW_5},{COL_4,ROW_4},{COL_4,ROW_4}},                        // digit 5 waypoints
  {{COL_1,ROW_6},{COL_4,ROW_6},{COL_4,ROW_4},{COL_4,ROW_4}},                        // digit 6 waypoints
  {{COL_2,ROW_7},{COL_2,LOWER_XFER_ROW},{COL_4,LOWER_XFER_ROW},{COL_4,ROW_4}},     // digit 7 waypoints
  {{COL_3,ROW_7},{COL_3,LOWER_XFER_ROW},{COL_4,LOWER_XFER_ROW},{COL_4,ROW_4}},     // digit 8 waypoints
  {{COL_4,ROW_7},{COL_4,ROW_4},{COL_4,ROW_4},{COL_4,ROW_4}},                        // digit 9 waypoints
};

const Coordinates tensOfMinutes[][NUMBER_OF_WAYPOINTS] PROGMEM = {
  {{COL_7,ROW_1},{COL_7,UPPER_XFER_ROW},{COL_5,UPPER_XFER_ROW},{COL_5,ROW_4}},      // digit 0 waypoints
  {{COL_6,ROW_1},{COL_6,UPPER_XFER_ROW},{COL_5,UPPER_XFER_ROW},{COL_5,ROW_4}},      // digit 1 waypoints
  {{COL_5,ROW_1},{COL_5,ROW_1},{COL_5,ROW_4},{COL_5,ROW_4}},                        // digit 2 waypoints
  {{COL_7,ROW_7},{COL_7,LOWER_XFER_ROW},{COL_5,LOWER_XFER_ROW},{COL_5,ROW_4}},      // digit 3 waypoints
  {{COL_6,ROW_7},{COL_6,LOWER_XFER_ROW},{COL_5,LOWER_XFER_ROW},{COL_5,ROW_4}},      // digit 4 waypoints
  {{COL_5,ROW_7},{COL_5,ROW_7},{COL_5,ROW_4},{COL_5,ROW_4}},                        // digit 5 waypoints
};

const Coordinates hours[][NUMBER_OF_WAYPOINTS] PROGMEM = {
  {{COL_10,ROW_1},{COL_10,UPPER_XFER_ROW},{COL_7,UPPER_XFER_ROW},{COL_7,ROW_4}},    // digit 0 waypoints
  {{COL_9,ROW_1},{COL_9,UPPER_XFER_ROW},{COL_7,UPPER_XFER_ROW},{COL_7,ROW_4}},      // digit 1 waypoints
  {{COL_8,ROW_1},{COL_8,UPPER_XFER_ROW},{COL_7,UPPER_XFER_ROW},{COL_7,ROW_4}},      // digit 2 waypoints
  {{COL_11,ROW_2},{COL_7,ROW_2},{COL_7,ROW_4},{COL_7,ROW_4}},                       // digit 3 waypoints
  {{COL_11,ROW_3},{COL_7,ROW_3},{COL_7,ROW_4},{COL_7,ROW_4}},                       // digit 4 waypoints
  {{COL_11,ROW_5},{COL_7,ROW_5},{COL_7,ROW_4},{COL_7,ROW_4}},                       // digit 5 waypoints
  {{COL_11,ROW_6},{COL_7,ROW_6},{COL_7,ROW_4},{COL_7,ROW_4}},                       // digit 6 waypoints
  {{COL_10,ROW_7},{COL_10,LOWER_XFER_ROW},{COL_7,LOWER_XFER_ROW},{COL_7,ROW_4}},    // digit 7 waypoints
  {{COL_9,ROW_7},{COL_9,LOWER_XFER_ROW},{COL_7,LOWER_XFER_ROW},{COL_7,ROW_4}},      // digit 8 waypoints
  {{COL_8,ROW_7},{COL_8,LOWER_XFER_ROW},{COL_7,LOWER_XFER_ROW},{COL_7,ROW_4}},      // digit 9 waypoints
};

const Coordinates tensOfHours[][NUMBER_OF_WAYPOINTS] PROGMEM = {
  {{COL_8,ROW_4},{COL_8,ROW_4},{COL_8,ROW_4},{COL_8,ROW_4}},        // there is no digit 0
  {{COL_11,ROW_4},{COL_11,ROW_4},{COL_11,ROW_4},{COL_8,ROW_4}},     // digit 1 waypoints
};
