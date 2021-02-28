#include "gui.h"
#include <iostream>
#include <cmath>

Slider::Slider() {}
Slider::~Slider() {}
Slider::Slider(int x, int y, int min, int max, int inc) {
    this->x = x;
    offsetx = x;
    this->y = y;
    this->min = min;
    this->max = max;
    this->inc = inc;
    hovering = clicked = false;
    lastX = (max / inc) * SLIDER_RADIUS;
    value = min;
}
bool Slider::checkMouse(int mousex, int mousey) {
    float xdiff = mousex - offsetx;
    float ydiff = mousey - y;
    float dist = sqrt(xdiff*xdiff + ydiff*ydiff);
    if(dist <= SLIDER_RADIUS) {
        hovering = true;
        return true;
    } else {
        hovering = false;
        return false;
    }
}
bool Slider::slide(int mousex) {
    if(clicked) {
        if(mousex > offsetx) {
            // Sliding right
            while(offsetx + SLIDER_RADIUS < mousex && value < max) {
                offsetx += SLIDER_RADIUS;
                value += inc;
            }
        } else if(mousex < offsetx) {
            // Sliding left
            while(offsetx - SLIDER_RADIUS > mousex && value > min) {
                offsetx -= SLIDER_RADIUS;
                value -= inc;
            }
        }
        return true;
    }
    return false;
}
void Slider::render(X11& x11) {
    x11.setColor(255,255,255);
    x11.drawLine(x, y, x + lastX, y);
    x11.setColor(0,255,0);
    x11.fillCircle(offsetx, y, SLIDER_RADIUS);
    std::string val = std::to_string(value);
    x11.setColor(0,0,0);
    x11.drawString(offsetx-(SLIDER_RADIUS/2), y, val);
}
void Slider::setClicked(bool b) {
    clicked = b;
}
void Slider::setHovering(bool b) {
    hovering = b;
}
int Slider::getValue() {
    return value;
}
int Slider::getClicked() {
    return clicked;
}
int Slider::getHovering() {
    return hovering;
}
int Slider::getLastX() {
    return lastX;
}

template <class T>
Button<T>::Button() {

}
template <class T>
Button<T>::Button(int x,int y,float w,float h,T val) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->val = val;
}
template <class T>
Button<T>::~Button() {

}
template <class T>
void Button<T>::init(int x,int y,float w,float h,T val) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->val = val;
}
template <class T>
bool Button<T>::checkMouse(int mousex,int mousey) {
    if(mousex >= x && mousex <= x + w && mousey >= y && mousey <= y + h) {
        return true;
    } else {
        return false;
    }
}
template <class T>
int Button<T>::getX() {
    return x;
}
template <class T>
int Button<T>::getY() {
    return y;
}
template <class T>
T Button<T>::getValue() {
    return val;
}
template <class T>
float Button<T>::getWidth() {
    return w;
}
template <class T>
float Button<T>::getHeight() {
    return h;
}

template class Button<int>;