//
//  snake.h
//  Snake
//
//  Created by pc on 11.07.22.
//

#ifndef snake_h
#define snake_h


#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

#define MINMAP 10

typedef enum DIR{
 LEFT,RIGHT,DOWN,UP
}DIR;

typedef struct POS{
    int x,y;
}POS;

extern POS* body;
extern POS fruit;
extern DIR dir;
extern DIR prevDir;
extern int snakeSize;
extern int mapWidth;
extern int mapHeight;
extern bool gameOver;
extern bool fruitEaten;
extern pthread_t inputThread;


void Play(int, int);
void* UserInput(void* );
void Move();
bool CanMove();
void SpawnFruit();
bool FruitCanSpawn(int, int);
void Draw(POS, char, int);

#endif /* snake_h */
