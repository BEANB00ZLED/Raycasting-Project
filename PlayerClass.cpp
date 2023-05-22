//PlayerClass.cpp
#include "PlayerClass.h"
#include <GL/glut.h>
#include "Constants.h"
#include <cmath>

//Constructor
Player::Player(void)
{
    posX, posY, deltaX, deltaY, angle = 0.0;
}

//Getter functions
float Player::getPosX(void)
{
    return posX;
}
float Player::getPosY(void)
{
    return posY;
}
float Player::getDeltaX(void)
{
    return deltaX;
}
float Player::getDeltaY(void)
{
    return deltaY;
}
float Player::getAngle(void)
{
    return angle;
}

//Setter functions
void Player::setPosX(float value)
{
    posX = value;
}
void Player::setPosY(float value)
{
    posY = value;
}
void Player::setDeltaX(float value)
{
    deltaX = value;
}
void Player::setDeltaY(float value)
{
    deltaY = value;
}
void Player::setAngle(float value)
{
    angle = value;

    //Allows the angle to loop around the unit circle
    if(angle < 0)
    {
        angle += 2*PI;
    }
    else if(angle > 2 * PI)
    {
        angle -= 2 * PI;
    }
}


void Player::draw(void)
{
    //Sets the current color to yellow
    glColor3f(1.0, 1.0, 0);
    //Sets the point diameter size
    glPointSize(8);
    //Creates points at veriticies
    glBegin(GL_POINTS);
    //Specifies a point
    glVertex2i(posX, posY);
    //Ends point creation and delimits veritices
    glEnd();

    //Creates a line with a specified width from player
    //out in the direction they are facing
    glLineWidth(LINEWIDTH);
    glBegin(GL_LINES);
    glVertex2i(posX, posY);
    glVertex2i(posX + (deltaX * MOVEAMOUNT), posY + (deltaY * MOVEAMOUNT));
    glEnd();
}



