#ifndef GAME_INFO_H
#define GAME_INFO_H

#define FIELD_H 20
#define FIELD_W 10

typedef struct {
  int **field;
  int **next;
  int score;
  int highScore;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef enum { NONE, GameStart, GamePause } Banner;

#endif  // GAME_INFO_H