#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <Arduboy2.h>
#include <ATMlib.h>
#include "bitmaps.h"
#include "songs.h"

//define menu states (on main menu)
#define STATE_MENU_INTRO             0
#define STATE_MENU_MAIN              1
#define STATE_MENU_HELP              2
#define STATE_MENU_PLAY              3
#define STATE_MENU_INFO              4
#define STATE_MENU_SOUNDFX           5

//define game states (on main menu)
#define STATE_GAME_NEXT_LEVEL        6
#define STATE_GAME_PLAYING           7
#define STATE_GAME_PAUSE             8
#define STATE_GAME_OVER              9

#define GAME_TOP                     0
#define GAME_BOTTOM                  48
#define GAME_LEFT                    0
#define GAME_RIGHT                   112

#define FACING_SOUTH                 0
#define FACING_WEST                  1
#define FACING_NORTH                 2
#define FACING_EAST                  3

#define FIRST_LEVEL                  0

#define ACTIVE_BIT                   5
#define ROOT_BIT                     6
#define TOTAL_BALLS                  121 // 11 rows X 11 columns
#define DEAD_BALL                    6

void drawBackground();
void setBallType(byte ballIndex, byte type);
byte getBallType(byte ballIndex);
void drawBalls();
void killBall(byte index);

float aBallX = 0.0;
float aBallY = 0.0;
float aBallRad;
byte aBallSpeed = 1;
byte aBall = 255;
byte alignType = 1;         // This should be toggled (0 -> 1, 1 -> 0, XOR'd with 1) each time the balls are shifted down.
byte randomCounter = 0;
byte fallOffset = 0;
bool falling = false;
//byte nextBall = 0;
float radAngle;
byte ballQueue[6];


Arduboy2Base arduboy;
Sprites sprites;
ATMsynth ATM;

byte gameState = STATE_MENU_INTRO;   // start the game with the TEAM a.r.g. logo
byte menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected
byte globalCounter = 0;
byte level = FIRST_LEVEL;

/*
   !!!Maximum input value is 26!!!
*/
byte generateRandomNumber(byte maxValue)
{
  randomCounter += arduboy.frameCount;
  unsigned int nr = randomCounter;
  nr *= maxValue * 10;
  nr /= 2560;

  return nr;
}

#endif
