#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

struct Players
{
  public:
    int x;
    int y;
    boolean walking;
    byte direction;
    byte frame;
    unsigned long score;
};

Players player = { .x = 20, .y = 20, .walking = false, .direction = FACING_SOUTH, .frame = 0 };

void drawPlayer()
{
  if (arduboy.everyXFrames(6) && player.walking) player.frame = (++player.frame) % 4;
  sprites.drawPlusMask(player.x, player.y, player4D_plus_mask, player.frame + 4 * player.direction);
}

#endif
