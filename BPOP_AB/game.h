#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "inputs.h"
#include "player.h"
#include "enemies.h"
#include "elements.h"
#include "levels.h"

void stateMenuPlay()
{
  sprites.drawPlusMask(34, 12, textArcade_plus_mask, 0);
  sprites.drawPlusMask(26, 36, textInfinite_plus_mask, 0);
  if (arduboy.justPressed(B_BUTTON)) gameState = STATE_GAME_NEXT_LEVEL;
};


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
