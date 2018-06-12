#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "inputs.h"
#include "player.h"
#include "enemies.h"
#include "elements.h"
#include "levels.h"

const unsigned char PROGMEM textXY[][2] = {{1,0},{2,0},{3,1},{3,2},{2,3},{1,3},{0,2},{0,1}};

byte textSequence = 0;

void stateMenuPlay()
{
  arduboy.fillScreen(1);
  if (arduboy.everyXFrames(4)) textSequence = (++textSequence) %8;
  
  byte coordinateX = pgm_read_byte(&textXY[textSequence][0]);
  byte coordinateY = pgm_read_byte(&textXY[textSequence][1]);

  sprites.drawPlusMask(34 + (coordinateX * (modeSelect - 1)), 12 + (coordinateY * (modeSelect - 1)), textArcade_plus_mask, 0);
  sprites.drawPlusMask(26 + (coordinateX * modeSelect), 36 + (coordinateY * modeSelect), textInfinite_plus_mask, 0);
  
  if (arduboy.justPressed(UP_BUTTON)) modeSelect = false;
  if (arduboy.justPressed(DOWN_BUTTON)) modeSelect = true;
  if (arduboy.justPressed(B_BUTTON)) gameState = STATE_GAME_ARCADE + modeSelect;
};


void stateGameArcade()
{
  gameState = STATE_GAME_NEXT_LEVEL;
}

void stateGameInfinite()
{
  gameState = STATE_GAME_NEXT_LEVEL;
}

void stateGameNextLevel()
{
  clearBalls();
  fillBallQueue();
  shiftBallsDown(true);
  shiftBallsDown(true);
  dropTimer = TIMER_AMOUNT;
  level++;
  gameState = STATE_GAME_PLAYING;
};


void stateGamePlaying()
{
  // Shift balls down periodically
  if (--dropTimer == 0) {
    shiftBallsDown(true);
    dropTimer = TIMER_AMOUNT;
  }

  if (checkBottomRow())
    gameState = STATE_GAME_OVER;
    
  checkInputs();
  checkCollisions();

  fallingBalls();
  updateMovingBall();

  arduboy.fillScreen(WHITE);

  drawBackground();

  // Draw guide
  if (aBall == 255)
    arduboy.drawLine(LAUNCHER_X, LAUNCHER_Y, LAUNCHER_X + cos(radAngle) * 20, LAUNCHER_Y - sin(radAngle) * 20, BLACK);

  // Draw Balls
  drawBalls();
  if (aBall != 255) {
    sprites.drawErase(byte(aBallX) - 2, byte(aBallY) - 2, sprBalls, aBall);
  }
  drawBallQueue();
  // Draw launcher
  sprites.drawPlusMask(LAUNCHER_X - 6, LAUNCHER_Y - 8, sprLauncher, (launcherAngle - 22) / 28);
  if (ballQueue[0] != 255)
    sprites.drawErase(LAUNCHER_X - 2, LAUNCHER_Y - 3, sprBalls, ballQueue[0]);
};

void stateGamePause()
{
  if (arduboy.justPressed(A_BUTTON)) gameState = STATE_GAME_PLAYING;
};

void stateGameOver()
{
  gameState = STATE_MENU_INTRO;
};



#endif
