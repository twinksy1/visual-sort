#ifndef GUI_H
#define GUI_H

#include "display.h"
const int SLIDER_RADIUS=20;

template <class T>
class Button {
private:
    int x,y;
    float w,h;
    T val;
public:
    Button();
    Button(int,int,float,float,T);
    ~Button();
    void init(int,int,float,float,T);
    bool checkMouse(int,int);
    int getX();
    int getY();
    float getWidth();
    float getHeight();
    T getValue();
};

class Slider {
private:
    int x,y;        // original position
    int offsetx;    // slid position
    int lastX;
    int value;
    int min,max;
    int inc;
    bool clicked;
    bool hovering;
public:
    Slider();
    Slider(int,int,int,int,int);    // x, y, min, max, inc
    ~Slider();
    bool checkMouse(int,int);
    bool slide(int);
    void render(X11&);
    void setClicked(bool);
    void setHovering(bool);
    int getValue();
    int getClicked();
    int getLastX();
    int getHovering();
};

#endif