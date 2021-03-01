#include "global.h"

Global::Global() {
	xres = 720;
        curOption = NULL;
	yres = 1280;
	sorting = rButtonDown = lButtonDown = false;
	dropMenu = true;
        listAmountSlider = new Slider(50, SLIDER_RADIUS, 10, 500, 10);
	list = new int[amount];
	srand((unsigned)time(NULL));
	for(int i=0; i<amount; i++)
		list[i] = rand() % yres;
	width = xres/amount;

        int sx = listAmountSlider->getLastX() + SLIDER_RADIUS*3;
        float h = SLIDER_RADIUS * 2;
        float w = 100.0f;
        for(int i=0; i<NUMSORTS; i++) {
                options[i].init(sx,0,w,h,i);
                sx += 1.5*w;
        }
}
Global::~Global() {
        delete listAmountSlider;
        listAmountSlider = NULL;
	delete list;
	list = NULL;
}
void Global::init() {
	srand((unsigned)time(NULL));
        int prevAmount = amount;
	amount = listAmountSlider->getValue();
        if(amount == prevAmount) return;
	list = new int[amount];
	for(int i=0; i<amount; i++)
		list[i] = rand() % yres;
	width = (float)xres/(float)amount;
}
void Global::setMouseCoords(int mousex, int mousey) {
        this->mousex = mousex;
        this->mousey = mousey;
}
void Global::processSettings() {
        if(listAmountSlider->slide(mousex)) {
                init();
        }
}
void Global::renderMenu(X11& x11) {
        if(!sorting) {
		x11.setColor(150,150,150);
		x11.fillRectangle(0,0,xres, SLIDER_RADIUS*2);
		listAmountSlider->render(x11);
                Button<int>* cur;
                for(int i=0; i<NUMSORTS; i++) {
                        cur = &options[i];
                        x11.setColor(200,200,200);
                        x11.fillRectangle(cur->getX(),cur->getY(),cur->getWidth(),cur->getHeight());
                        x11.setColor(0,0,0);
                        // Title on buttons
                        switch(cur->getValue()) {
                                case SHUFFLE:
                                        x11.drawString(cur->getX() + 10, cur->getY() + (cur->getHeight()/2), "SHUFFLE");
                                        break;
                                case BUBBLESORT:
                                        x11.drawString(cur->getX() + 10, cur->getY() + (cur->getHeight()/2), "BUBBLE");
                                        break;
                                case SELECTIONSORT:
                                        x11.drawString(cur->getX() + 10, cur->getY() + (cur->getHeight()/2), "SELECTION");
                                        break;
                                case INSERTIONSORT:
                                        x11.drawString(cur->getX() + 10, cur->getY() + (cur->getHeight()/2), "INSERTION");
                                        break;
                                case SHELLSORT:
                                        x11.drawString(cur->getX() + 10, cur->getY() + (cur->getHeight()/2), "SHELL");
                                        break;
                                case BISELECTIONSORT:
                                        x11.drawString(cur->getX() + 10, cur->getY() + (cur->getHeight()/2), "BI-SELECTION");
                                        break;
                                case COCKTAILSORT:
                                        x11.drawString(cur->getX() + 10, cur->getY() + (cur->getHeight()/2), "COCKTAIL");
                                        break;
                                case MERGESORT:
                                        x11.drawString(cur->getX() + 10, cur->getY() + (cur->getHeight()/2), "MERGE");
                                        break;
                                case HEAPSORT:
                                        x11.drawString(cur->getX() + 10, cur->getY() + (cur->getHeight()/2), "HEAP");
                                        break;
                                case QUICKSORT:
                                        x11.drawString(cur->getX() + 10, cur->getY() + (cur->getHeight()/2), "QUICK");
                                        break;
                        }
                }
	}
	else {
		
	}
}

void checkResize(XEvent *e, Global& g) {
        if (e->type != ConfigureNotify)
                return;
        XConfigureEvent xce = e->xconfigure;
        g.xres = xce.width;
        g.yres = xce.height;
        g.init();
//        x11.setWindowTitle();
}

void checkMouse(XEvent *e, Global& g) {
        Slider* slider = g.listAmountSlider;
        if (e->type == ButtonRelease) {
                g.lButtonDown = g.rButtonDown = false;
                slider->setClicked(false);
                return;
        }
        if (e->type == ButtonPress) {
                if (e->xbutton.button==1 && !g.sorting) {
                        //Left button is down
                        g.lButtonDown = true;
                        if(!g.sorting) {
                                slider->setClicked(slider->checkMouse(g.mousex, g.mousey));
                                Button<int>* cur;
                                for(int i=0; i<NUMSORTS; i++) {
                                        cur = &g.options[i];
                                        if(cur->checkMouse(g.mousex, g.mousey)) {
                                                g.curOption = cur;
                                                break;
                                        }
                                }
                        }
                }
                if (e->xbutton.button==3) {
                        //Right button is down
                }
        }
        if (g.mousex != e->xbutton.x || g.mousey != e->xbutton.y) {
                //Mouse moved
                g.setMouseCoords(e->xbutton.x, e->xbutton.y);
        }
}
int checkKeys(XEvent *e, Global& g) {
        if (e->type != KeyPress && e->type != KeyPress)
                return 0;
        int key = XLookupKeysym(&e->xkey, 0);
        switch(key) {
                case XK_Escape:
                        return(1);
        }

        return(0);
}