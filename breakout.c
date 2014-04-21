//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

//define paddle height and width
#define PADDLEHEIGHT 15
#define PADDLEWIDTH 75

#define BRICKHEIGHT 15
#define BRICKWIDTH 35

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define DIAMETER 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
void initLives(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);
    
    // instantiate bricks
    initBricks(window);
    
    // instantiate life counter
    GOval lifeball1 = newGOval(WIDTH - 30, 20, DIAMETER, DIAMETER);
    setFilled(lifeball1, true);
    setColor(lifeball1, "#000000");
    add(window, lifeball1);

    GOval lifeball2 = newGOval(WIDTH - 50, 20, DIAMETER, DIAMETER);
    setFilled(lifeball2, true);
    setColor(lifeball2, "#000000");
    add(window, lifeball2);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel scoreboard = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    //initialize ball velocities
    double xvelocity = drand48();
    double yvelocity = 2.0;

    // keep playing until game over
    
    if (lives > 0)
    {
        waitForClick();
    }
    while (lives > 0 && bricks > 0)
    {
        // TODO/////////////////////////////////////////////////////////////////
        //all work goes in this while loop apparently//////////////////////////

        // compel paddle to follow mouse along x-axis by listening for mouse_event
        GEvent paddlemove = getNextEvent(MOUSE_EVENT);
        // if there was an event
        if (paddlemove != NULL)
            {
                // if the event was movement
                if (getEventType(paddlemove) == MOUSE_MOVED)
                {
                    // make paddle follow mouse pointer's motion along the x axis only
                    double paddlex = getX(paddlemove) - getWidth(paddle) / 2;
                    double paddley = 565;
                    setLocation(paddle, paddlex, paddley);
                }
            }
        

        // move ball along both x and y
        move(ball, xvelocity, yvelocity);
        
        GObject collided = detectCollision(window, ball);
        
        // Correct code to make ball bounce - this does not work, despite working exactly as so in bounce.c
        //perhaps a switch is in order using detect collision for bouncing
        if (getX(ball) + DIAMETER >= WIDTH)
        {
            xvelocity = -xvelocity;
        }

        // bounce off left edge of window
        else if (getX(ball) <= 0)
        {
            xvelocity = -xvelocity;
        }
        
        else if (getY(ball) <= 0)
        {
            yvelocity = -yvelocity;
        }
        
        else if (collided == paddle)
        {
            yvelocity = -yvelocity;
            xvelocity = xvelocity + (drand48() - drand48());
        }
        else if ((((collided != NULL) && (collided != paddle))
        && (collided != scoreboard)) && (strcmp(getType(collided), "GOval") != 0))
        {
            yvelocity = -yvelocity;
            xvelocity = xvelocity + (drand48() - drand48());
            removeGWindow(window, collided);
            points++;
            bricks = bricks - 1;
            updateScoreboard(window, scoreboard, points);
        }
        else if (getY(ball) + DIAMETER >= HEIGHT)
        {
            yvelocity = 2.0;
            xvelocity = drand48();
            lives = lives - 1;
            if (lives == 2)
            {
                removeGWindow(window, lifeball2);
            }
            else if (lives == 1)
            {
                removeGWindow(window, lifeball1);
            }
            
            setLocation(ball, (WIDTH / 2) - (DIAMETER / 2), (HEIGHT / 2) - (DIAMETER / 2));
            setLocation(paddle, (WIDTH / 2) - (PADDLEWIDTH / 2), HEIGHT - 35);
            if (lives > 0)
            {
                waitForClick();
            }
        }

        // linger before moving again
        pause(10);
    }
    // game over
    if (lives > 0 && bricks == 0)
    {
        GLabel winner = newGLabel("YOU WON!");
        setFont(winner, "SansSerif-22");
        double x = (WIDTH / 2) - (getWidth(winner) / 2);
        double y = HEIGHT - 250;
        setLocation(winner, x, y);
        setColor(winner, "#0033FF");
        add(window, winner);
    }
    else
    {
        GLabel gameover = newGLabel("GAME OVER");
        setFont(gameover, "SansSerif-22");
        double x = (WIDTH / 2) - (getWidth(gameover) / 2);
        double y = HEIGHT - 250;
        setLocation(gameover, x, y);
        setColor(gameover, "#990000");
        add(window, gameover);
    }
    waitForClick();
    return 0;
}



/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    for (int i = 0, x = 10; i < COLS; i++, x = x + 38)
    {
        GRect brick = newGRect(x, 80, BRICKWIDTH, BRICKHEIGHT);
        setFilled(brick, true);
        setColor(brick, "#AA0000");
        add(window, brick);
    }
    for (int i = 0, x = 10; i < COLS; i++, x = x + 38)
    {
        GRect brick = newGRect(x, 100, BRICKWIDTH, BRICKHEIGHT);
        setFilled(brick, true);
        setColor(brick, "#880000");
        add(window, brick);
    }
    for (int i = 0, x = 10; i < COLS; i++, x = x + 38)
    {
        GRect brick = newGRect(x, 120, BRICKWIDTH, BRICKHEIGHT);
        setFilled(brick, true);
        setColor(brick, "#880088");
        add(window, brick);
    }
    for (int i = 0, x = 10; i < COLS; i++, x = x + 38)
    {
        GRect brick = newGRect(x, 140, BRICKWIDTH, BRICKHEIGHT);
        setFilled(brick, true);
        setColor(brick, "#000088");
        add(window, brick);
    }
    for (int i = 0, x = 10; i < COLS; i++, x = x + 38)
    {
        GRect brick = newGRect(x, 160, BRICKWIDTH, BRICKHEIGHT);
        setFilled(brick, true);
        setColor(brick, "#0000AA");
        add(window, brick);
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // Initialize the ball at the center of the window
    GOval ball = newGOval(195, 295, DIAMETER, DIAMETER);
    setFilled(ball, true);
    setColor(ball, "#000000");
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // Create the paddle and place it in the bottom center of GWindow. 400x600 game dimensions
    // pretty sure I need to use getBounds in some way to ensure paddle doesn't go off screen when at edge. will have to look
    // into it.
    GRect paddleobj = newGRect((WIDTH / 2) - (PADDLEWIDTH / 2), HEIGHT - 35, PADDLEWIDTH, PADDLEHEIGHT);
    setFilled(paddleobj, true);
    setColor(paddleobj, "#A90000");
    add(window, paddleobj);
    return paddleobj;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // TODO
    GLabel scoreboard = newGLabel("0");
    setFont(scoreboard, "SansSerif-22");
    double x = (getWidth(window) - getWidth(scoreboard)) / 2;
    double y = (getHeight(window) - getHeight(scoreboard)) / 2;
    setLocation(scoreboard, x, y);
    setColor(scoreboard, "#7700AA");
    add(window, scoreboard);
    return scoreboard;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + DIAMETER, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + DIAMETER);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + DIAMETER, y + DIAMETER);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}

