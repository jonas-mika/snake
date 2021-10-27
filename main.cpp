//  main.cpp
//  A classic SNAKE game written in C++ for macOS using NCURSES
#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>

bool game_over; // boolean variable; false if game is lost, else true
const int width = 40, height = 20; // initial size of the window to play in
int x, y, fruit_x, fruit_y, score; // curr position of player (x,y), curr position of next treat (fruit_x, fruit_y), score
double delay;
enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN}; // enumerator type that gives direction, default to stop=0 (at start)
eDirection dir;

int tail_x[100], tail_y[100]; // integer array of for snake tail (100 long, for both x and y)
int tail_len = 0; // initial length of tail

void settings() {
    initscr();
    clear();
    noecho();
    curs_set(0);
    
    mvprintw(0,0, "--- WELCOME TO SNAKE! ---");
    mvprintw(1,0, "Choose your settings!");

    mvprintw(3,0, "Difficulty \n[1] Easy\n[2] Intermediate\n[3] Expert");
    int answer = getch();
    switch(answer){
        case '1':
           delay = 0.5; 
           break;
        case '2':
           delay = 100;
           break;
        case '3': 
           delay = 1000000000;
           break;
        default:
           delay = 100;
           break;
    }

    clear();
    endwin();

}

void setup() {
    // initial settings for screen
    initscr(); // initialisation routine that must be called (part of ncurses library)
    clear(); // clear screen (general maintenance)
    noecho(); // surpresses automatic echoing of typed characters (otherwise input would be drawn on screen) 
    cbreak(); // setting to immediately recognises input from getch(), would otherwise wait for newline char
    curs_set(0); 
    
    // global game settings
    game_over = false;
    dir = STOP;
    x = width/2; // start in middle
    y = height/2; 
    fruit_x = (rand() % width) + 1; // random fruit position on screen
    fruit_y = (rand() % height) + 1;
    score = 0; 
    
}

void draw() {
    clear();
   
    for (int i = 0; i <= height + 2; i++)
    {
        for (int j = 0; j <= width + 2; j++)
        {
            // draw outer frame
            if ((i == 0 & j == 0) | (i==0 & j==width+2) | (i==width+2 & j==0) | (i==width+2 & j==width+2)) {
                mvprintw(i, j, "+");
            } else if (i == 0 | i == height+2) {
                mvprintw(i, j, "-");
            } else if (j == 0 | j == width+2) {
                mvprintw(i, j, "|");
            // draw snake
            }
            else if (i == y && j == x) { // head of snake
                mvprintw(i, j, "O");
            }
            else if (i == fruit_y && j == fruit_x) { // fruit
                mvprintw(i, j, "X");
            }
            else // draw tail of snake
                for (int k = 0; k < tail_len; k++) {
                    if (tail_x[k] == j && tail_y[k] == i)
                        mvprintw(i, j, "o");
                }
        }
    }
    
    mvprintw(23, 0, "Score %d", score);
    
    refresh();
    
}


void input() {
    
    keypad(stdscr, TRUE); // catch arrow keys
    halfdelay(1);
    
    int c = getch();
    
    switch(c)
    {
    case KEY_LEFT:
        dir = LEFT;
        break;
    case KEY_RIGHT:
        dir = RIGHT;
        break;
    case KEY_UP:
        dir = UP;
        break;
    case KEY_DOWN:
        dir = DOWN;
        break;
    case 'q':
        draw();
        mvprintw(24,0, "Q was pressed. Quitting Game...");
        refresh();
        usleep(1000000 / 2);
        game_over = true;
        break;
    }
    
}

void logic() {
    
    int prev_x = tail_x[0];
    int prev_y = tail_y[0];
    int prev_2x, prev_2y;
    

    // moving old current position into first tail piece 
    tail_x[0] = x;
    tail_y[0] = y;
    
    for (int i = 1; i < tail_len; i++)
    {
        prev_2x = tail_x[i]; // safe second old second position in tail
        prev_2y = tail_y[i];
        tail_x[i] = prev_x; // overwrite old second position with old first position
        tail_y[i] = prev_y;
        prev_x = prev_2x; // update prev_x/y
        prev_y = prev_2y;
    }
    
    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }
    // bump into borders (ends game) 
    if (x > width+1 || y > height+1 || x < 1 || y < 1){
        game_over = true;
    }
    
    // bump into own tail (ends game)
    for (int i = 0; i < tail_len; i++)
        if (tail_x[i] == x && tail_y[i] == y)
        {
            game_over = true;
        }

    // catch food
    if (x == fruit_x && y == fruit_y)
    {
        score++;
        fruit_x = (rand() % width)+1;
        fruit_y = (rand() % height)+1;
        tail_len++;
    }
    
}

int main() {

    settings();
    setup();
    
    while(!game_over)
    {
        draw();
        input();
        logic();
        refresh();
        usleep(100000 / delay); // sleep for half a second
    } 
    clear();

    mvprintw(0, 0, "Game ended.");
    mvprintw(1, 0, "Your Final Score: %d", score);
    refresh();
    usleep(1000000 * 5); // sleep for 2 secs
    endwin();
    
    return 0;
}
