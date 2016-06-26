#include <gb/gb.h>
#include <gb/drawing.h>
#include <gb/rand.h>

#define gprintf_xy(x, y, str) gotogxy(x, y);gprintf(str);

const unsigned char snake_tiles[] =
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x7E,0x7E,0x81,0x66,0x99,0x5A,0xA5,
  0x5A,0xA5,0x66,0x99,0x7E,0x81,0x00,0x7E,
  0x00,0x3C,0x32,0x4E,0x73,0x8F,0x63,0x9F,
  0x03,0xFF,0x07,0xFF,0x7E,0x7E,0x3C,0x3C,
  0x90,0xE0,0x88,0xF0,0x84,0xF8,0xC2,0xFC,
  0x60,0x7F,0x30,0x3F,0x18,0x1F,0x0F,0x0F
};

#define mapWidth 20
#define mapHeight 18
#define mapBank 0

const unsigned char map[] =
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,
  0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
  0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
  0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
  0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
  0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
  0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
  0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
  0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
  0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
  0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

#define SCREEN_WIDTH  160
#define SCREEN_HEIGHT 144

const unsigned char levelBoundaries[4] = {
  2, 5, 17, 13
};

#define EMPTY_SPRITE 0x00

//SNAKE INFO
#define SNAKE_SPRITE_SIZE 8
#define SNAKE_SPRITE      0x02
#define MAX_SNAKE_SIZE    35
#define SnakeHead         snakeCoords[0]

unsigned char snakeCoords[MAX_SNAKE_SIZE][2];
unsigned char snakeCleanCoords[2];
unsigned char snakeSize;
unsigned char x, y;

//PILLS INFO
#define PILL_SPRITE_SIZE    8
#define PILL_SPRITE         0x03
#define MAX_PILLS           8
#define PILL_SPRITE_X_COORD(x) (x + 1) * PILL_SPRITE_SIZE
#define PILL_SPRITE_Y_COORD(y) (y + 2) * PILL_SPRITE_SIZE

unsigned char pillsLive = 0;
unsigned char pillsPositions[MAX_PILLS][2];

//GAME
#define MAX_LEVEL 10


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
unsigned char pad;
unsigned char gameover = 1;
unsigned char i, j, k, l, m;
unsigned char vFrameCount;
unsigned char playing;
unsigned char numSprites;


void drawUI()
{
  //TODO
}


void drawSnake()
{
  //Draw snake code
  l = gameover ? EMPTY_SPRITE : SNAKE_SPRITE;
  i = gameover ? 1 : 0;
  for (i; i < snakeSize; ++i) {
      j = snakeCoords[i][0];
      k = snakeCoords[i][1];
      set_bkg_tiles(j, k, 1, 1, &l);  
  }

  //Clean empty zones
  l = EMPTY_SPRITE;
  j = snakeCleanCoords[0];
  k = snakeCleanCoords[1];
  set_bkg_tiles(j, k, 1, 1, &l);
}


void drawPills()
{
  for (i = 0; i < pillsPerLevel[level]; ++i) {
      if (pillsPositions[i][0] != 0xFF && pillsPositions[i][1] != 0xFF && !gameover) {
          set_sprite_tile(i, PILL_SPRITE);
          move_sprite(i, PILL_SPRITE_X_COORD(pillsPositions[i][0]), PILL_SPRITE_Y_COORD(pillsPositions[i][1]));
      } else {
          set_sprite_tile(i, EMPTY_SPRITE);
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
    pillsLive = 0;
    score = 0;
    scoreParts[0] = 0;
    scoreParts[1] = 0;
    scoreParts[2] = 0;
    scoreParts[3] = 0;
    score = 0;
}


void enter()
{
  resetGame();
  pad = joypad();
  if (pad & J_START 
    || (pad & J_A) 
    || (pad & J_B)) {
    gameover = 0;
  }
  drawUI();
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
  drawUI();
}


void main() 
{  
  SPRITES_8x8;

  set_sprite_data(0, 4, snake_tiles);

  set_bkg_data(0, 4, snake_tiles);
  set_bkg_tiles(0, 0, mapWidth, mapHeight, map);

  SHOW_BKG;
  SHOW_SPRITES;

  while (1) {
    wait_vbl_done();
    vFrameCount++;

    if (gameover) {
      enter();
    } else {
      game();
    }
  }
}