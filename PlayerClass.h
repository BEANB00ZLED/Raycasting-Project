//PlayerClass.h
#ifndef PLAYERCLASS_H_INCLUDED
#define PLAYERCLASS_H_INCLUDED

class Player
{
private:
    float posX, posY, deltaX, deltaY, angle;
public:
    //Constructor
    Player(void);

    //Getter functions
    float getPosX(void);
    float getPosY(void);
    float getDeltaX(void);
    float getDeltaY(void);
    float getAngle(void);

    //Setter functions
    void setPosX(float);
    void setPosY(float);
    void setDeltaX(float);
    void setDeltaY(float);
    void setAngle(float);

    //Drawing player function
    void draw(void);
};


#endif // PLAYERCLASS_H_INCLUDED
