//main.cpp
#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include "PlayerClass.h"
#include "Constants.h"
#include "Util.h"


using namespace std;


//Global player variable
Player player;

int mapX = 8, mapY = 8, mapSize = 64;
int map[8][8] =
{
    {1,1,1,1,1,1,1,1},
    {1,0,1,0,0,0,0,1},
    {1,0,1,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,0,1},
    {1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1}
};


void drawMap2D(void)
{
    int coordX, coordY, baseX, baseY = 0;
    //Loop through each spot in the array
    for(coordY = 0; coordY < mapY; coordY++)
    {
        for(coordX = 0; coordX < mapX; coordX++)
        {
            if(map[coordY][coordX] == 1)
                //Set color to white
                glColor3f(1, 1, 1);
            else
                //Set color to black
                glColor3f(0, 0, 0);

            //Gets to base point of square on display window
            baseX = coordX * mapSize;
            baseY = coordY * mapSize;

            //Create the vertexes for the shape, start at top left and draw CC
            glBegin(GL_QUADS);
            glVertex2i(baseX + 1, baseY + 1);
            glVertex2i(baseX + 1, baseY + mapSize - 1);
            glVertex2i(baseX + mapSize - 1, baseY + mapSize - 1);
            glVertex2i(baseX + mapSize - 1, baseY + 1);
            glEnd();
        }
    }
}


//Uses DDA algorithm
void drawRays2D(void)
{
    //Initial starting info
    int x1 = player.getPosX();
    int y1 = player.getPosY();
    float rayAngle = player.getAngle();

    //For casting multiple rays
    rayAngle -= RADDEG * (NUMRAYS / 2.0);

    for(int i = 0; i < NUMRAYS; i++)
    {
        if(rayAngle < 0)
        {
            rayAngle += 2 * PI;
        }
        else if(rayAngle > 2 * PI)
        {
            rayAngle -= 2 * PI;
        }
        int depthOfField = 0;

        //***************************
        //Checking for vertical lines
        //***************************

        //Checking for first vertical line intersections
        float rayXVert = 0.0;
        float rayYVert = 0.0;
        float deltaY = 0.0;
        int directionX = 0;

        //Looking to the right
        if((rayAngle < PI / 2) || (rayAngle > 1.5 * PI))
        {
            rayXVert = x1 + (mapSize - (x1 % mapSize));
            directionX = 1;
        }
        //Looking to the left
        else if((rayAngle > PI / 2) && (rayAngle <= 1.5 * PI))
        {
            rayXVert = x1 - (x1 % mapSize);
            directionX = -1;
        }
        else
        {
            rayXVert = x1;
            depthOfField = 8;
            directionX = 0;
        }
        //Finish calculating first point of intersection
        rayYVert = y1 + tan(rayAngle) * (rayXVert - x1);

        //Calculate the slope
        deltaY = tan(rayAngle) * (64 * directionX);


        //Loop to extend ray until it hits a wall
        while(depthOfField < 8)
        {
            //The .001 is so that it will round correctly while looking either way
            int tileX = pixelToTile(rayXVert + (.001 * directionX));
            int tileY = pixelToTile(rayYVert);
            if(tileX < 0 || tileX > 8 || tileY < 0 || tileY > 8 || map[tileY][tileX] == 1)
            {
                depthOfField = 8;
            }
            else
            {
                rayYVert += deltaY;
                rayXVert += 64 * directionX;
                depthOfField++;
            }
        }

        //*****************************
        //Checking for horizontal lines
        //*****************************

        depthOfField = 0;

        //Checking for first horizontal line intersections
        float rayXHori = 0.0;
        float rayYHori = 0.0;
        float deltaX = 0.0;
        int directionY = 0;

        //Looking up
        if(rayAngle > 0 && rayAngle < PI)
        {
            rayYHori = y1 + (mapSize - (y1 % mapSize));
            directionY = -1;
        }
        //Looking down
        else if(rayAngle > PI)
        {
            rayYHori = y1 - (y1 % mapSize);
            directionY = 1;
        }
        else
        {
            rayYHori = y1;
            depthOfField = 8;
            directionY = 0;
        }
        //Finish calculating the first point of intersection
        rayXHori = x1 + ((rayYHori - y1) / tan(rayAngle));

        //Calculate the slope
        deltaX = ((64 * directionY) / tan(rayAngle));


        while(depthOfField < 8)

        {
            //The .001 is so that it will round correctly while looking either way
            int tileX = pixelToTile(rayXHori);
            int tileY = pixelToTile(rayYHori  + (.001 * directionY));

            if(tileX < 0 || tileX >= 8 || tileY < 0 || tileY >= 8 || map[tileY][tileX] == 1)
            {
                depthOfField = 8;
            }
            else
            {
                rayYHori += 64 * directionY;
                rayXHori += deltaX;
                depthOfField++;
            }
        }

        /*

        //Draw the shorter ray
        glColor3f(1, 0, 0);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2i(x1, y1);
        if(dist(x1, y1, rayXHori, rayYHori) <= dist(x1, y1, rayXVert, rayYVert))
        {
            glVertex2i(rayXHori, rayYHori);
        }
        else
        {
            glVertex2i(rayXVert, rayYVert);
        }
        glEnd();
        */

        glColor3f(1, 0, 0);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2i(x1, y1);
        glVertex2i(rayXHori, rayYHori);
        glEnd();

        //Increment the ray angle so that the next ray can be cast
        rayAngle += RADDEG;
    }
}


static void display(void)
{
    //Clears buffers to preset values
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Draw the map
    drawMap2D();
    //Draw the rays
    drawRays2D();
    //Draw the player
    player.draw();
    //Swaps frame buffers
    glutSwapBuffers();
}


//Player movement function
void buttons(unsigned char key, int x, int y)
{
    if(key == 'a')
    {
        player.setAngle(player.getAngle() - TURNAMOUNT);
        player.setDeltaX(cos(player.getAngle()) * MOVEAMOUNT);
        player.setDeltaY(sin(player.getAngle()) * MOVEAMOUNT);
    }
    if(key == 'd')
    {
        player.setAngle(player.getAngle() + TURNAMOUNT);
        player.setDeltaX(cos(player.getAngle()) * MOVEAMOUNT);
        player.setDeltaY(sin(player.getAngle()) * MOVEAMOUNT);
    }
    if(key == 'w')
    {
        player.setPosX(player.getPosX() + player.getDeltaX());
        player.setPosY(player.getPosY() + player.getDeltaY());
    }
    if(key == 's')
    {
        player.setPosX(player.getPosX() - player.getDeltaX());
        player.setPosY(player.getPosY() - player.getDeltaY());
    }

    //Updates the display with the movement
    glutPostRedisplay();
}



//Initial setup of things on display
void init()
{
    //Set background to dark grey
    glClearColor(0.3, 0.3, 0.3, 0);
    //Creates an 2D orthographic matrix
    gluOrtho2D(0, WIDTH, HEIGHT, 0);
    player.setPosX(300);
    player.setPosY(300);
    player.setDeltaX(cos(player.getAngle()) * MOVEAMOUNT);
    player.setDeltaY(sin(player.getAngle()) * MOVEAMOUNT);
}


int main(int argc, char* argv[])
{
    //Initialize GLUT library
    glutInit(&argc, argv);
    //Initializes display to be double buffered and use RGBA color model
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    //Sets size of the window
    glutInitWindowSize(WIDTH, HEIGHT);
    //Creates a top level window
    glutCreateWindow("Raycasting-Project");
    init();
    //Redraws window when it is resized/moved/hidden
    glutDisplayFunc(display);
    //Sets the keyboard callback for the current window
    glutKeyboardFunc(buttons);
    //Calls display callback repeatedly (glutDisplayFunc)
    glutMainLoop();
}
