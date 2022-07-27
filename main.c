//
//  main.c
//  Snake
//
//  Created by pc on 11.07.22.
//

#include "snake.h"
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

bool GameOver();

//set the window (setup ncurses) try to draw the snake
//save an array of the snake
//save the size of the snake, update it
//set up the user input, and respond correspondingly
//inorder to move the snake every 1 sek, you need to have a thread for the input.
//whenever the snake moves remove the tail and just add a new tile of the body ahead, which will be then the new head.
//you have to use malloc whenever the snake eats a fruit, as the snake moves, add a new head position without erasing the tail.
//if the snake moves into itself, the game is over. (if the next tile is the snake's body)


int main(int argc, const char * argv[]) {
    srand((unsigned)time(NULL));
    initscr();
    curs_set(0);

    if(has_colors() == FALSE){
        endwin();
        printf("Your terminal does not support color\n");
        getch();
        return 0;
    }
   
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
   
    keypad(stdscr,true);
    noecho();
    cbreak();

    refresh();
    bool quit;
    
    do {
        Play(30,20);
        quit = GameOver();
    } while(!quit);

    clear();
    endwin();
    return 0;
}

bool GameOver(){
    char output;
    attron(COLOR_PAIR(2));
    mvaddstr(10,(30) / 2 - 4,"You Lost!");
    attroff(COLOR_PAIR(2));
    while(getch() != ' '){}
    
    do {
        attron(COLOR_PAIR(4));
        mvaddstr(10,30 / 2 - 13,"Do you want to play again?");
        attroff(COLOR_PAIR(4));
        output = getch();
    } while(output != 'y' && output != 'n');

    if(output == 'y')
        return false;
    
    return true;
}
