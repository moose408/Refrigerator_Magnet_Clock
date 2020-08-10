//==========================================================================
// Refrigerator Magnet Clock - ClockCoordinates.h
//
// Copyright (c) 2020, Craig Colvin
//
// Licensed under Creative Commons License
// Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
//==========================================================================



#ifndef CLOCKCOORDINATES_H
#define CLOCKCOORDINATES_H

typedef struct 
{
    long x;
    long y;
} Coordinates;

//===========================================================
// W a y p o i n t s
//
// There are 4 waypoint coordinates for a number 
// the first one is it's Home position, the last one
// is it's display position in the Clock. The middle
// two are moves used to get the digit from Home to
// the Clock position.
//
// Some digits don't need 3 moves to from Home to Clock
// so setting the middle waypoints to the same coordinates
// as the previous move will cause them to be skipped
//===========================================================

#define NUMBER_OF_WAYPOINTS 4
#define WAYPOINT_HOME 0
#define WAYPOINT_A 1
#define WAYPOINT_B 2
#define WAYPOINT_CLOCK 3

// There is a seperate array for each clock digit
extern const Coordinates minutes[][NUMBER_OF_WAYPOINTS];
extern const Coordinates tensOfMinutes[][NUMBER_OF_WAYPOINTS];
extern const Coordinates hours[][NUMBER_OF_WAYPOINTS];
extern const Coordinates tensOfHours[][NUMBER_OF_WAYPOINTS];

//===========================================================
// Waypoint Columns and Row Coordinates
//===========================================================
#define COL_1       1750
#define COL_2       5103
#define COL_3       8456
#define COL_4       11809
#define COL_5       15162
#define COL_6       18515
#define COL_7       21868
#define COL_8       25221
#define COL_9       28574
#define COL_10      31927
#define COL_11      35280

#define ROW_1       1270
#define ROW_2       5383
#define ROW_3       9496
#define ROW_4       14060
#define ROW_5       17722
#define ROW_6       21835
#define ROW_7       25950

#define UPPER_XFER_ROW      10000
#define LOWER_XFER_ROW      20000

#endif //CLOCKCOORDINATES_H
