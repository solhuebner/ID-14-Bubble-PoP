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
  level++;
  gameState = STATE_GAME_PLAYING;
};


void stateGamePlaying()
{
  checkInputs();
  checkCollisions();
};

void stateGamePause()
{
  if (arduboy.justPressed(A_BUTTON)) gameState = STATE_GAME_PLAYING;
};

void stateGameOver()
{

};



#endif
