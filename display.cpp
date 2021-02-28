#include "display.h"

X11::X11(int& xres, int& yres) {
	XSetWindowAttributes attributes;
	XdbeBackBufferAttributes *backAttr;
	dpy = XOpenDisplay(NULL);
	attributes.event_mask = ExposureMask | StructureNotifyMask |
		PointerMotionMask | ButtonPressMask |
		ButtonReleaseMask | KeyPressMask | KeyReleaseMask;
	attributes.backing_store = Always;
	attributes.save_under = True;
	attributes.override_redirect = False;
	attributes.background_pixel = 0x00000000;
	Window root = DefaultRootWindow(dpy);
	XWindowAttributes main;
	XGetWindowAttributes(dpy, root, &main);
	xres = main.width;
	yres = main.height;
//	g.init();
	//Create a window
	win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
			CopyFromParent, InputOutput, CopyFromParent,
			CWBackingStore | CWOverrideRedirect | CWEventMask |
			CWSaveUnder | CWBackPixel, &attributes);
	//Create gc
	gc = XCreateGC(dpy, win, 0, NULL);
	//Get back buffer and attributes
	backBuffer = XdbeAllocateBackBufferName(dpy, win, XdbeUndefined);
	backAttr = XdbeGetBackBufferAttributes(dpy, backBuffer);
	swapInfo.swap_window = backAttr->window;
	swapInfo.swap_action = XdbeUndefined;
	XFree(backAttr);
	//Map and raise window
	setWindowTitle("VISUAL SORT");
	XMapWindow(dpy, win);
	XRaiseWindow(dpy, win);
}
X11::~X11() {
	if(!XdbeDeallocateBackBufferName(dpy, backBuffer)) {
		fprintf(stderr,"Error : unable to deallocate back buffer.\n");
	}
	XFreeGC(dpy, gc);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}
void X11::swapBuffers() {
	XdbeSwapBuffers(dpy, &swapInfo, 1);
	usleep(4000);
}
bool X11::getPending() {
	return XPending(dpy);
}
void X11::getNextEvent(XEvent *e) {
	XNextEvent(dpy, e);
}
void X11::setColor(int r, int g, int b) {
	unsigned long cref = 0L;
	cref += r;
	cref <<= 8;
	cref += g;
	cref <<= 8;
	cref += b;
	XSetForeground(dpy, gc, cref);
}
void X11::setWindowTitle(std::string title) {
	XStoreName(dpy, win, title.c_str());
}
void X11::clearScreen(int xres, int yres) {
	//XClearWindow(dpy, backBuffer);
	XSetForeground(dpy, gc, 0x00050505);
	XFillRectangle(dpy, backBuffer, gc, 0, 0, xres, yres);
}
void X11::drawString(float x, float y, std::string word) {
	XDrawString(dpy, backBuffer, gc, x, y, word.c_str(), word.length());
}
void X11::drawPoint(float x, float y) {
	XDrawPoint(dpy, backBuffer, gc, x, y);
}
void X11::drawLine(float x0, float y0, float x1, float y1) {
	XDrawLine(dpy, backBuffer, gc, x0, y0, x1, y1);
}
void X11::drawRectangle(float x, float y, float w, float h) {
	//x,y is upper-left corner
	XDrawRectangle(dpy, backBuffer, gc, x, y, w, h);
}
void X11::fillRectangle(float x, float y, float w, float h) {
	//x,y is upper-left corner
	XFillRectangle(dpy, backBuffer, gc, x, y, w, h);
}
void X11::drawArc(int xc, int yc, int x, int y) {
		drawPoint(xc+x, yc+y);
		drawPoint(xc-x, yc+y);
		drawPoint(xc+x, yc-y);
		drawPoint(xc-x, yc-y);
		drawPoint(xc+y, yc+x);
		drawPoint(xc-y, yc+x);
		drawPoint(xc+y, yc-x);
		drawPoint(xc-y, yc-x);
}
void X11::drawCircle(int xc, int yc, int r) {
		int x = 0, y = r;
		int d = 3 - 2 * r;
		drawArc(xc, yc, x, y);
		while (y >= x) {
				// for each pixel we will 
				// draw all eight pixels 

				x++;

				// check for decision parameter 
				// and correspondingly  
				// update d, x, y 
				if (d > 0) {
					y--;
					d = d + 4 * (x - y) + 10;
				}
				else
					d = d + 4 * x + 6;
				drawArc(xc, yc, x, y);
		}
}
void X11::fillArc(int xc, int yc, int x, int y) {
		drawLine(xc+x, yc+y, xc-x, yc+y);
		drawLine(xc+x, yc-y, xc-x, yc-y);
		drawLine(xc+y, yc+x, xc-y, yc+x);
		drawLine(xc+y, yc-x, xc-y, yc-x);
}
void X11::fillCircle(int xc, int yc, int r) {
		int x = 0, y = r;
		int d = 3 - 2 * r;
		fillArc(xc, yc, x, y);
		while (y >= x) {
				// for each pixel we will 
				// draw all eight pixels 

				x++;

				// check for decision parameter 
				// and correspondingly  
				// update d, x, y 
				if (d > 0) {
					y--;
					d = d + 4 * (x - y) + 10;
				}
				else
					d = d + 4 * x + 6;
				fillArc(xc, yc, x, y);
		}
}