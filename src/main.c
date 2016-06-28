#include <gb/gb.h>
#include <gb/drawing.h>
#include <gb/rand.h>

#include "snake-tiles.c"
#include "snake-map.c"

#define mapWidth 20
#define mapHeight 18
#define mapBank 0

#define SCREEN_WIDTH  160
#define SCREEN_HEIGHT 144

#define NUM_SPRITES   40
#define SPRITE_SIZE   8
#define SPRITE_X_COORD(x) (x + 1) * SPRITE_SIZE
#define SPRITE_Y_COORD(y) (y + 2) * SPRITE_SIZE

#define SNAKE_SPRITE  0x25
#define PILL_SPRITE   0x26
#define EMPTY_SPRITE  0x27

unsigned char spriteIdx;
#define SET_NEXT_SPRITE(sprite) set_sprite_tile(spriteIdx++, sprite)

//SNAKE INFO
#define SNAKE_SPRITE_SIZE 8
#define MAX_SNAKE_SIZE    35
#define SnakeHead         snakeCoords[0]

unsigned char snakeCoords[MAX_SNAKE_SIZE][2];
unsigned char snakeCleanCoords[2];
unsigned char snakeSize;
unsigned char x, y;

//GAME
#define MAX_LEVEL 10
#define MAX_PILLS 8

unsigned char pillsLive = 0;
unsigned char pillsPositions[MAX_PILLS][2];

const unsigned char levelBoundaries[4] = {
  2, 4, 17, 15
};

const unsigned char pointsPerLevel[MAX_LEVEL] = {
    2, 5, 10, 20, 30, 40, 50, 100, 200, 255
};

const unsigned char pillsPerLevel[MAX_LEVEL] = {
    2, 2, 3, 3, 4, 4, 5, 6, 7, 8
};

const unsigned char speed[MAX_LEVEL] = {
    15, 12, 10, 9, 8, 8, 7, 6, 5, 4
}; 

unsigned char score = 0;
unsigned char scoreParts[4];

unsigned char level = 0;
unsigned char levelParts[2];

unsigned char pad;
unsigned char gameover = 1;
unsigned char i, j, k, l, m;
unsigned char vFrameCount;
unsigned char numSprites;


void clearSprites()
{
  for (i = 0; i < spriteIdx; i++) {
    set_sprite_tile(i, EMPTY_SPRITE);
  }
  spriteIdx = 0;
}


void drawUI()
{
  //Draw score
  for (i = 0; i < 4; i++) {
    SET_NEXT_SPRITE(scoreParts[i] == 0 ? 0x23 : scoreParts[i] + 0x19);
    move_sprite(spriteIdx - 1, SPRITE_X_COORD(15 + i), SPRITE_Y_COORD(1));
  }

  //Draw level
  SET_NEXT_SPRITE(levelParts[0] == 0 ? 0x23 : 0x19 + levelParts[0] );
  move_sprite(spriteIdx - 1, SPRITE_X_COORD(4), SPRITE_Y_COORD(1));
  SET_NEXT_SPRITE(levelParts[1] == 0 ? 0x23 : 0x19 + levelParts[1] );
  move_sprite(spriteIdx - 1, SPRITE_X_COORD(5), SPRITE_Y_COORD(1));

  if (gameover) {
      SET_NEXT_SPRITE('G' - 0x41); move_sprite(spriteIdx - 1, SPRITE_X_COORD(8), SPRITE_Y_COORD(8));
      SET_NEXT_SPRITE('A' - 0x41); move_sprite(spriteIdx - 1, SPRITE_X_COORD(9), SPRITE_Y_COORD(8));
      SET_NEXT_SPRITE('M' - 0x41); move_sprite(spriteIdx - 1, SPRITE_X_COORD(10), SPRITE_Y_COORD(8));
      SET_NEXT_SPRITE('E' - 0x41); move_sprite(spriteIdx - 1, SPRITE_X_COORD(11), SPRITE_Y_COORD(8));

      SET_NEXT_SPRITE('O' - 0x41); move_sprite(spriteIdx - 1, SPRITE_X_COORD(8), SPRITE_Y_COORD(10));
      SET_NEXT_SPRITE('V' - 0x41); move_sprite(spriteIdx - 1, SPRITE_X_COORD(9), SPRITE_Y_COORD(10));
      SET_NEXT_SPRITE('E' - 0x41); move_sprite(spriteIdx - 1, SPRITE_X_COORD(10), SPRITE_Y_COORD(10));
      SET_NEXT_SPRITE('R' - 0x41); move_sprite(spriteIdx - 1, SPRITE_X_COORD(11), SPRITE_Y_COORD(10));
  }
}


void drawSnake()
{
  if (gameover) {
    l = EMPTY_SPRITE;
    //Clean all snake graphics on gameover
    for (i = 1; i < snakeSize; i++) {
      j = snakeCoords[i][0];
      k = snakeCoords[i][1];
      set_bkg_tiles(j, k, 1, 1, &l);
    }
    j = snakeCleanCoords[0];
    k = snakeCleanCoords[1];
    set_bkg_tiles(j, k, 1, 1, &l);
  } else {
    //Draw only head, because is the one moving
    l = SNAKE_SPRITE;
    j = snakeCoords[0][0];
    k = snakeCoords[0][1];
    set_bkg_tiles(j, k, 1, 1, &l);

    //Clean empty zones
    l = EMPTY_SPRITE;
    j = snakeCleanCoords[0];
    k = snakeCleanCoords[1];
    set_bkg_tiles(j, k, 1, 1, &l);
  }
}


void drawPills()
{
  for (i = 0; i < pillsPerLevel[level]; ++i) {
      if (pillsPositions[i][0] != 0xFF && pillsPositions[i][1] != 0xFF && !gameover) {
          SET_NEXT_SPRITE(PILL_SPRITE);
          move_sprite(spriteIdx - 1, SPRITE_X_COORD(pillsPositions[i][0]), SPRITE_Y_COORD(pillsPositions[i][1]));
      } else {
          SET_NEXT_SPRITE(EMPTY_SPRITE);
      }
  }
}


void resetGame()
{
    gameover = 1;
    drawSnake();
    drawPills();

    vFrameCount = 0;
    snakeSize = 1;
    SnakeHead[0] = 11;
    SnakeHead[1] = 9;
    x = 1;
    y = 0;
    level = 0xFF;
    levelParts[0] = 0x00;
    levelParts[1] = 0x00;
    pillsLive = 0;
    score = 0;
    scoreParts[0] = 0;
    scoreParts[1] = 0;
    scoreParts[2] = 0;
    scoreParts[3] = 0;
}


void enter()
{
  pad = joypad();
  if (pad & J_START 
    || (pad & J_A) 
    || (pad & J_B)) {
    gameover = 0;
  }
}


void game()
{
  pad = joypad();
  if (pad & J_LEFT) {
    gameover = snakeSize > 1 && (x == 1);
    x = 0xFF;
    y = 0;
  } else if (pad & J_RIGHT) {
    gameover = snakeSize > 1 && (x == 0xFF);
    x = 1;
    y = 0;
  } else if (pad & J_UP) {
    gameover = snakeSize > 1 && (y == 1);
    x = 0;
    y = 0xFF;
  } else if (pad & J_DOWN) {
    gameover = snakeSize > 1 && (y == 0xFF);
    x = 0;
    y = 1;
  }

  /*
  GAMELOOP CODE
  */

  //Spawn pills
  if (pillsLive == 0) {
      ++level;
      level = level > MAX_LEVEL ? MAX_LEVEL : level;
      levelParts[0] = (level + 1) / 10;
      levelParts[1] = (level + 1) - levelParts[0] * 10;
      for (i = 0; i < pillsPerLevel[level]; ++i) {
          j = (_rand() & 7) % (levelBoundaries[2] - levelBoundaries[0]);
          k = (_rand() & 7) % (levelBoundaries[3] - levelBoundaries[1]);
          pillsPositions[i][0] = levelBoundaries[0] + j;
          pillsPositions[i][1] = levelBoundaries[1] + k;
          ++pillsLive;
      }
  //Eat pill
  } else {
    for (i = 0; i < MAX_PILLS; i++) {  
        j = pillsPositions[i][0];
        k = pillsPositions[i][1];
        if (SnakeHead[0] == j && SnakeHead[1] == k) {
            pillsPositions[i][0] = 0xFF;
            pillsPositions[i][1] = 0xFF;
            --pillsLive;
            
            //Grow snake
            j = snakeSize;
            ++snakeSize;
            snakeSize = (snakeSize > MAX_SNAKE_SIZE - 1)? MAX_SNAKE_SIZE - 1 : snakeSize;
            if (j != snakeSize) {
                snakeCoords[snakeSize - 1][0] = snakeCoords[snakeSize - 2][0];
                snakeCoords[snakeSize - 1][1] = snakeCoords[snakeSize - 2][1];
            }
            score += pointsPerLevel[level];
            scoreParts[0] = score / 1000;
            scoreParts[1] = score / 100 - scoreParts[0] * 10;
            scoreParts[2] = score / 10 - (scoreParts[0] * 100 + scoreParts[1] * 10);
            scoreParts[3] = score - (scoreParts[0] * 1000 + scoreParts[1] * 100 + scoreParts[2] * 10);
        }
    }
  }
  

  //Move snake
  i = vFrameCount / speed[level] * speed[level];
  if (i == vFrameCount) {
      snakeCleanCoords[0] = snakeCoords[snakeSize - 1][0];
      snakeCleanCoords[1] = snakeCoords[snakeSize - 1][1];
      
      for (i = snakeSize - 1; i > 0; --i) {
          snakeCoords[i][0] = snakeCoords[i-1][0];
          snakeCoords[i][1] = snakeCoords[i-1][1];
      }
      
      SnakeHead[0] += x;
      SnakeHead[1] += y;
  }


  //Check gameover
  gameover = gameover || (SnakeHead[0] < levelBoundaries[0]);
  gameover = gameover || (SnakeHead[0] > levelBoundaries[2]);
  gameover = gameover || (SnakeHead[1] < levelBoundaries[1]);
  gameover = gameover || (SnakeHead[1] > levelBoundaries[3]);
  for (i = 2; i < snakeSize; i++) {
      gameover = gameover || (snakeCoords[i][0] == SnakeHead[0] &&  snakeCoords[i][1] == SnakeHead[1]);
      if (gameover) {
          break;
      }
  }
  
  drawSnake();
  drawPills();

  if (gameover) {
    resetGame();
  }
}


void main() 
{  
  SPRITES_8x8;

  set_sprite_data(0, NUM_SPRITES, tiles);
  set_bkg_data(0, NUM_SPRITES, tiles);
  set_bkg_tiles(0, 0, mapWidth, mapHeight, map);

  SHOW_BKG;
  SHOW_SPRITES;

  wait_vbl_done();
  resetGame();

  while (1) {
    wait_vbl_done();
    vFrameCount++;

    clearSprites();
    if (gameover) {
      enter();
    } else {
      game();
    }
    drawUI();

  }
}