#ifndef GLOBAL_H
#define GLOBAL_H

#include "gui.h"
#include "display.h"
#include <cstdlib>
#include <ctime>

enum {SHUFFLE=0,BUBBLESORT,SELECTIONSORT,INSERTIONSORT,SHELLSORT,BISELECTIONSORT,COCKTAILSORT,MERGESORT,HEAPSORT,QUICKSORT,NUMSORTS};

class Global {
public:
        int xres, yres;
        int mousex, mousey;
        int BARWIDTH=30;
        int amount=100;
        Slider* listAmountSlider;
        Button<int> options[NUMSORTS];
        Button<int>* curOption;
        int* list;
        float width;
        bool sorting;
        bool dropMenu;
        bool lButtonDown, rButtonDown;

        Global();
        ~Global();
        void init();
        void setMouseCoords(int,int);
        void processSettings();
        void renderMenu(X11&);
};

void checkResize(XEvent*, Global&);
void checkMouse(XEvent*, Global&);
int checkKeys(XEvent*, Global&);
#endif
