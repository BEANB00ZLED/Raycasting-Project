//main.cpp
#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include "PlayerClass.h"
#include "Constants.h"


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

//Using DDA algorithm
void drawRays2D(void)
{

    //mx and my are for the map tile positions
    //depthOfField is so that we aren't checking for intersections beyond map bounds
    int mx, my , depthOfField = 0;
    //rayX and rayY make the position that it is currently looking at
    //offsetX and offsetY is the distance to the next intersection
    float rayX, rayY, rayAngle, offsetX, offsetY = 0.0;
    //Lining up ray angle and player angle
    rayAngle = player.getAngle();

    //Loops for casting rays
    for(int i = 0; i < 1; i++)
    {
        depthOfField = 0;
        float aTan = -1/tan(rayAngle);

        //********************************************
        //Check for horizontal grid line intersections
        //********************************************

        //If looking down
        if(rayAngle > PI)
        {
            //Finds the first point of horizontal grid line intersection
            rayY = (((int)player.getPosY()>>6)<<6) - 0.0001;
            rayX = (player.getPosY() - rayY) * aTan+player.getPosX();
            offsetY -=64;
            offsetX = -offsetY * aTan;
        }
        //If looking up
        else if(rayAngle < PI)
        {
            //Finds the first point of horizontal grid line intersection
            rayY = (((int)player.getPosY()>>6)<<6) + 64;
            rayX = (player.getPosY() - rayY) * aTan+player.getPosX();
            offsetY = 64;
            offsetX = -offsetY * aTan;
        }
        //If looking straight left or right
        else if(rayAngle == 0 || rayAngle == PI)
        {
            rayX = player.getPosX();
            rayY = player.getPosY();
            depthOfField = 8;
        }

        while(depthOfField < 8)
        {
            //Calculate the the spot in the map array based on the end point of the ray
            mx = (int)rayX >> 6;
            my = (int)rayY >> 6;
            //Makes sure the value is withing map bounds and is wall
            if((mx >= 0) && (mx < 8) && (my >= 0) && (my < 8) && (map[my][mx] == 1))
            {
                depthOfField = 8;

            }
            else
            {
                rayX += offsetX;
                rayY += offsetY;
                depthOfField += 1;
            }
        }
        //Draw the ray
        glColor3f(0, 1, 0);
        glLineWidth(6);
        glBegin(GL_LINES);
        glVertex2i(player.getPosX(), player.getPosY());
        glVertex2i(rayX, rayY);
        glEnd();

        //******************************************
        //Check for Vertical grid line intersections
        //******************************************

        //Reset variables because if not it breaks
        int mx, my , mapPos, depthOfField = 0;
        float rayX, rayY, rayAngle, offsetX, offsetY = 0.0;
        //Lining up ray angle and player angle
        rayAngle = player.getAngle();
        depthOfField = 0;
        float nTan = -tan(rayAngle);

        //If looking left
        if((rayAngle > NORTH) && (rayAngle < SOUTH))
        {
            //Finds the first point of horizontal grid line intersection
            rayX = (((int)player.getPosX()>>6)<<6) - 0.0001;
            rayY = (player.getPosX() - rayX) * nTan + player.getPosY();
            offsetX -=64;
            offsetY = -offsetX * nTan;
        }
        //If looking right
        else if(rayAngle < NORTH || rayAngle > SOUTH)
        {
            //Finds the first point of horizontal grid line intersection
            rayX = (((int)player.getPosX()>>6)<<6) + 64;
            rayY = (player.getPosX() - rayX) * nTan + player.getPosY();
            offsetX = 64;
            offsetY = -offsetX * nTan;
        }
        //If looking straight up or down
        else if(rayAngle == NORTH || rayAngle == SOUTH)
        {
            rayX = player.getPosX();
            rayY = player.getPosY();
            depthOfField = 8;
        }

        while(depthOfField < 8)
        {
            //Calculate the the spot in the map array based on the end point of the ray
            mx = (int)rayX >> 6;
            my = (int)rayY >> 6;
            //Makes sure the value is within map bounds and is wall
            if((mx >= 0) && (mx < 8) && (my >= 0) && (my < 8) && (map[my][mx] == 1))
            {
                depthOfField = 8;
            }
            else
            {
                rayX += offsetX;
                rayY += offsetY;
                depthOfField += 1;
            }
        }
        //Draw the ray
        glColor3f(1, 0, 0);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2i(player.getPosX(), player.getPosY());
        glVertex2i(rayX, rayY);
        glEnd();
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
        //In radian circle, go below 0 leads to 2pi
        if(player.getAngle() < 0)
        {
            player.setAngle(player.getAngle() + (2 *  PI));
        }
        player.setDeltaX(cos(player.getAngle()) * MOVEAMOUNT);
        player.setDeltaY(sin(player.getAngle()) * MOVEAMOUNT);
    }
    if(key == 'd')
    {
        player.setAngle(player.getAngle() + TURNAMOUNT);
        //In radian circle, go above 2pi leads to 0
        if(player.getAngle() > (2 * PI))
        {
            player.setAngle(player.getAngle() - (2 * PI));
        }
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
