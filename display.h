#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/Xdbe.h>


class X11 {
private:
	Display *dpy;
        Window win;
        GC gc;
        XdbeBackBuffer backBuffer;
        XdbeSwapInfo swapInfo;
public:
	X11(int&,int&);
	~X11();

	void swapBuffers();
	bool getPending();
	void getNextEvent(XEvent*);
	void setColor(int,int,int);
	void setWindowTitle(std::string);
	void clearScreen(int,int);
	void drawString(float,float,std::string);
	void drawPoint(float,float);
	void drawLine(float,float,float,float);
	void drawRectangle(float,float,float,float);
	void fillRectangle(float,float,float,float);
	void drawArc(int,int,int,int);
	void drawCircle(int,int,int);
	void fillArc(int,int,int,int);
	void fillCircle(int,int,int);
};

#endif
