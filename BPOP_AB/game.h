#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "inputs.h"
#include "player.h"
#include "enemies.h"
#include "elements.h"
#include "levels.h"

void stateMenuPlay()
//void stateGamePrepareLevel()
{
  
  gameState = STATE_GAME_NEXT_LEVEL;
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
    arduboy.drawLine(64, 57, 64 + cos(radAngle) * 20, 57 - sin(radAngle) * 20, BLACK);

  // Draw Balls
  drawBalls();
  if (aBall != 255) {
    sprites.drawErase(byte(aBallX) - 2, byte(aBallY) - 2, sprBalls, aBall);
  }
  drawBallQueue();
  // Draw launcher
  sprites.drawPlusMask(58, 50, sprLauncher, launcherAngle / 60);
  if (ballQueue[0] != 255)
    sprites.drawErase(58 + 4, 50 + 5, sprBalls, ballQueue[0]);
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
