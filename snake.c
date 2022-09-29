//
//  snake.c
//  Snake
//
//  Created by pc on 11.07.22.
//

#include <stdio.h>
#include "snake.h"
#include <time.h>

DIR nextDir;
DIR currentDir;
POS fruit = {-1, -1};
int snakeSize;
int mapWidth;
int mapHeight;
bool gameOver;
bool fruitEaten;
const int t_movement = 150000;
pthread_t inputThread;
POS* body;

void Play(int _mapWidth, int _mapHeight){
    if(_mapWidth < MINMAP || _mapHeight < MINMAP)
        return;

    mapWidth = _mapWidth;
    mapHeight = _mapHeight;
    
    snakeSize = 3;
    nextDir = RIGHT;
    currentDir = RIGHT;
    body = (POS*) malloc(snakeSize * sizeof(POS));
    gameOver = false;
    POS snakeTail = {.x = _mapHeight / 2, .y = _mapWidth / 2};  // I spawn the snake Tail in the center, but you can spawn it everywhere you do like (you can do it randomly if you wish)
    
    clear();
    for(int i = 0; i < mapHeight; i++)
        for(int j = 0; j < mapWidth; j++){
            POS p = {i, j};
            Draw(p, '.', 1);
        }
    
    for(int i = 0; i < snakeSize; i++){
        *(body + i) = (POS){.x = snakeTail.x, .y = snakeTail.y + i};
        Draw(*(body + i), '*', 3);
    }
    SpawnFruit();
    refresh();
    
    pthread_create(&inputThread, NULL, UserInput, NULL);

    do {
        usleep(t_movement);
        Move();
        if (fruit.x == -1 && fruit.y == -1)
            SpawnFruit();
        refresh();
    } while(!gameOver);

    free(body);
}

void* UserInput(void* args){
    do {
        switch(getch()){
            case KEY_LEFT: if(currentDir != RIGHT) nextDir = LEFT; break;
            case KEY_RIGHT: if(currentDir != LEFT) nextDir = RIGHT; break;
            case KEY_DOWN: if(currentDir != UP) nextDir = DOWN; break;
            case KEY_UP: if(currentDir != DOWN) nextDir = UP; break;
        }
    } while(!gameOver);

    return NULL;
}

void Move(){
    POS headpos = *(body + snakeSize - 1);
    
    switch(nextDir){
        case LEFT: headpos.y--; break;
        case RIGHT: headpos.y++; break;
        case UP: headpos.x--; break;
        case DOWN: headpos.x++; break;
    }
    currentDir = nextDir;

    if(CanMove(headpos)){
        if(fruitEaten){
            snakeSize++;
            Draw(headpos, '*', 3);
        
            POS* tmp = (POS*) malloc((snakeSize - 1) * sizeof(POS));
            memcpy(tmp, body, (snakeSize - 1) * sizeof(POS));
            body = (POS*) malloc(snakeSize * sizeof(POS));
            memcpy(body, tmp, (snakeSize - 1) * sizeof(POS));
            
            *(body + snakeSize - 1) = headpos;
            fruitEaten = false;
        }
        else{
            Draw(*body, '.', 1);
            Draw(headpos, '*', 3);
            
            for(int i = 0; i < snakeSize - 1; i++)
                *(body+i) = *(body + i + 1);
            
            *(body + snakeSize - 1) = headpos;
        }

        if(headpos.y == fruit.y && headpos.x == fruit.x){
            fruitEaten = true;
            fruit.y = -1;
            fruit.x = -1;
        }
    }
    else
        gameOver = true;
}

bool CanMove(POS p){
    if(p.x > mapHeight - 1 || p.y > mapWidth - 1 || p.x < 0 || p.y < 0)
        return false;

    for(int i = 0; i < snakeSize-1; i++)
        if((body + i)->x == p.x && (body + i)->y == p.y)
            return false;

    return true;
}

void SpawnFruit(){
    int x;
    int y;

    do {
        x = rand() % mapHeight;
        y = rand() % mapWidth;
    } while(!FruitCanSpawn(x, y));

    fruit.x = x;
    fruit.y = y;
    Draw(fruit, 'o', 5);
}

bool FruitCanSpawn(int x, int y){
    for(int i = 0; i < snakeSize; i++){
        if((body + i)->x == x && (body + i)->y == y){
            return false;
        }
    }
    return true;
}

void Draw(POS p, char c, int color){
    attron(COLOR_PAIR(color));
    mvaddch(p.x, p.y, c);
    attroff(COLOR_PAIR(color));
}
