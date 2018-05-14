#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

extern byte checkDeath(byte index);
extern void killBall(byte index);
extern void deactivateDead();
extern void checkRoots();
extern byte checkSurrounding(byte index);

class Ball {
public:
  byte state; // state of the ball position
  // 0000 0000
  // |||| |||└-\
  // |||| ||└--\ Type of ball, the sub-image. (6 is dead/falling)
  // |||| |└---/
  // |||| └----/
  // |||└------- Active
  // ||└-------- Connect to root during root expansion
  // |└--------- Unused
  // └---------- Unused
};

byte launcherAngle = 90;
Ball balls[TOTAL_BALLS];

void setBallType(byte ballIndex, byte type) {
  if (type > 6)
    type = 0;
  balls[ballIndex].state &= 0xF0;
  balls[ballIndex].state |= (type & 0x0F);
}

byte getBallType(byte ballIndex) {
  return (balls[ballIndex].state & 0x0F);
}

byte getBallIndex(byte x, byte y) {
  if (x < 32 || x >= 94)
    return 255;
  byte row = y / 5;
  byte col;
  if (row % 2 == alignType) {
    col = (x - 32 - 2) / 6;
    if (col >= 10)
      return 255;
  }
  else
    col = (x - 32) / 6;
  if (row >= 11 || col >= 11)
    return 255;
  return (col + row * 11);
}

byte getBall(byte row, byte col) {
  if (row >= 11 || col >= 11)
    return 255;

  if (row % 2 == alignType && col >= 10)
    return 255;

  return (col + row * 11);
}

void fallingBalls() {
  if (falling) {
    if (fallOffset < 64) {
      fallOffset += 2;
    }
    else {
      falling = false;
      fallOffset = 0;
      deactivateDead();
    }
  }
}

void updateMovingBall() {
  if (aBall != 255) {
    if (aBallX <= 34 || aBallX >= 94) {
      aBallRad = PI - aBallRad;
    }
    aBallX += cos(aBallRad) * aBallSpeed;
    aBallY -= sin(aBallRad) * aBallSpeed;
    byte forwardIndex = getBallIndex((byte)(aBallX + cos(aBallRad) * 3), (byte)(aBallY - sin(aBallRad) * 3));
    if (forwardIndex != 255 && bitRead(balls[forwardIndex].state, ACTIVE_BIT)) {
      byte indx = getBallIndex((byte)aBallX, (byte)aBallY);
      setBallType(indx, aBall);
      bitSet(balls[indx].state, ACTIVE_BIT);
      bitClear(balls[indx].state, ROOT_BIT);
      aBall = 255;
      if (checkSurrounding(indx)) {
        falling = true;
        checkRoots();
      }
    }

    if (aBallY <= 3) {
      byte indx = getBallIndex((byte)aBallX, (byte)aBallY);
      if (indx != 255) {
        setBallType(indx, aBall);
        bitSet(balls[indx].state, ACTIVE_BIT);
        bitClear(balls[indx].state, ROOT_BIT);
      }
      aBall = 255;
      if (checkSurrounding(indx)) {
        falling = true;
        checkRoots();
      }
    }
  }
}

void drawBalls() {
  for (byte i = TOTAL_BALLS-1; i < TOTAL_BALLS; i--) {
    if (bitRead(balls[i].state, ACTIVE_BIT)) {
      byte col = i % 11;
      byte row = i / 11;
      sprites.drawErase(32 + col * 6 + ((row % 2 == alignType) ? 3 : 0), 5 * row + 1 + ((getBallType(i) == 6) ? fallOffset : 0), sprBalls, getBallType(i));
    }
  }
}

void fillBallQueue() {
  for (byte i = 0; i < 6; i++) {
    ballQueue[i] = generateRandomNumber(6);
  }
}

void shiftBallQueue(bool newball) {
  for (byte i = 0; i < 5; i++) {
    ballQueue[i] = ballQueue[i + 1];
  }
  if (newball)
    ballQueue[5] = generateRandomNumber(6);
  else
    ballQueue[5] = 255;
}

void drawBallQueue() {
  for (byte i = 1; i < 6; i++) {
    if (ballQueue[i] != 255)
      sprites.drawErase(56 - (i - 1) * 6, 59, sprBalls, ballQueue[i]);
  }
}

void shiftBallsDown(bool fillTopRow) {
  alignType ^= 1;
  for (byte i = 10; i > 0; i--) { // row
    for (byte j = 10; j < 11; j--) { // column
      balls[j + i * 11] = balls[j + (i - 1) * 11];
    }
  }
  if (fillTopRow) {
    for (byte i = 10; i < 11; i--) {
      balls[i].state = generateRandomNumber(6);
      bitSet(balls[i].state, ACTIVE_BIT);
      bitSet(balls[i].state, ROOT_BIT);
    }
    if (!alignType)
      bitClear(balls[10].state, ACTIVE_BIT); 
  }
  else {
    for (byte i = 10; i < 11; i--) {
      bitClear(balls[i].state, ACTIVE_BIT);
    }
  }
}

bool checkBottomRow() {
  for (byte i = TOTAL_BALLS-1; i >= TOTAL_BALLS-12; i--) {
    if (bitRead(balls[i].state, ACTIVE_BIT))
      return true;
  }
  return false;
}

void clearBalls() {
  for (byte i = TOTAL_BALLS-1; i < TOTAL_BALLS; i--) {
    balls[i].state = 0;
  }
}

#endif
