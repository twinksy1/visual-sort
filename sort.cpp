/* ORIGINAL CREATOR: Juan Villasenor
 * Feel free to use & modify :)
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/Xdbe.h>
#include <chrono>

int amount = 100;
#define BARWIDTH 15
#define SWAP(x,y) {int temp=x; x=y; y=temp;}

class Global {
	public:
		int xres = 1280;
		int yres = 720;

		int* list;
		double width;

		bool sorting = false;
		
		Global()
		{
			list = new int[amount];
			srand((unsigned)time(NULL));
			for(int i=0; i<amount; i++)
				list[i] = rand() % yres;
			width = xres/amount;
		}

		void init()
		{
			srand((unsigned)time(NULL));
			amount = xres / BARWIDTH;
			list = new int[amount];
			for(int i=0; i<amount; i++)
				list[i] = rand() % yres;
			width = xres/amount;
		}

		~Global()
		{
			delete list;
			list = NULL;
		}
} g;

class X11_wrapper {
private:
        Display *dpy;
        Window win;
        GC gc;
        XdbeBackBuffer backBuffer;
        XdbeSwapInfo swapInfo;
public:
        X11_wrapper() {
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
		g.xres = main.width;
		g.yres = main.height;
		g.init();
                //Create a window
                win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
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
                setWindowTitle();
                XMapWindow(dpy, win);
                XRaiseWindow(dpy, win);
	}
        ~X11_wrapper() {
                if(!XdbeDeallocateBackBufferName(dpy, backBuffer)) {
                        fprintf(stderr,"Error : unable to deallocate back buffer.\n");
                }
                XFreeGC(dpy, gc);
                XDestroyWindow(dpy, win);
                XCloseDisplay(dpy);
        }
        void swapBuffers() {
                XdbeSwapBuffers(dpy, &swapInfo, 1);
                usleep(4000);
        }
        bool getPending() {
                return XPending(dpy);
        }
        void getNextEvent(XEvent *e) {
                XNextEvent(dpy, e);
        }
        void set_color_3i(int r, int g, int b) {
                unsigned long cref = 0L;
                cref += r;
                cref <<= 8;
                cref += g;
                cref <<= 8;
                cref += b;
                XSetForeground(dpy, gc, cref);
        }
        void setWindowTitle() {
                char ts[256];
                sprintf(ts, "dbeCloth  R=Replay  %i x %i", g.xres, g.yres);
                XStoreName(dpy, win, ts);
        }
        void clear_screen() {
                //XClearWindow(dpy, backBuffer);
                XSetForeground(dpy, gc, 0x00050505);
                XFillRectangle(dpy, backBuffer, gc, 0, 0, g.xres, g.yres);
        }
        void drawString(int x, int y, const char *message) {
                XDrawString(dpy, backBuffer, gc, x, y, message, strlen(message));
        }
        void drawPoint(int x, int y) {
                XDrawPoint(dpy, backBuffer, gc, x, y);
        }
        void drawLine(int x0, int y0, int x1, int y1) {
                XDrawLine(dpy, backBuffer, gc, x0, y0, x1, y1);
        }
        void drawRectangle(double x, double y, int w, int h) {
                //x,y is upper-left corner
                XDrawRectangle(dpy, backBuffer, gc, x, y, w, h);
        }
        void fillRectangle(double x, double y, int w, int h) {
                //x,y is upper-left corner
                XFillRectangle(dpy, backBuffer, gc, x, y, w, h);
	}
} x11;

void render();
void physics();
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);

int main()
{
        srand((unsigned)time(NULL));
        int done = 0;
	printf("\x1b[1m	MENU OPTIONS ON TOP LEFT OF SCREEN\n");
	sleep(2);
        while (!done) {
                //Handle all events in queue
                while (x11.getPending()) {
                        XEvent e;
                        x11.getNextEvent(&e);
                        check_resize(&e);
                        check_mouse(&e);
                        done = check_keys(&e);
                }
                //Process physics and rendering every frame
                physics();
                render();
                x11.swapBuffers();
        }
        return 0;
}

void check_resize(XEvent *e)
{
        if (e->type != ConfigureNotify)
                return;
        XConfigureEvent xce = e->xconfigure;
        g.xres = xce.width;
        g.yres = xce.height;
	g.init();
        x11.setWindowTitle();
}

void check_mouse(XEvent *e)
{
        static int savex = 0;
        static int savey = 0;
        //
        if (e->type == ButtonRelease) {
                return;
        }
        if (e->type == ButtonPress) {
                if (e->xbutton.button==1) {
                        //Left button is down
                }
                if (e->xbutton.button==3) {
                        //Right button is down
                }
        }
        if (savex != e->xbutton.x || savey != e->xbutton.y) {
                //Mouse moved
                savex = e->xbutton.x;
                savey = e->xbutton.y;
        }
}

void shuffle()
{
	for(int i=0; i<amount; i++) {
		int idx1 = rand() % amount;
		int idx2 = rand() % amount;
		SWAP(g.list[idx1], g.list[idx2]);

		render();
		x11.swapBuffers();
	}
}

void bubblesort()
{
	bool swapped = true;
	auto start = std::chrono::high_resolution_clock::now();
	while(swapped) {
		swapped = false;
		for(int i=0; i<amount; i++) {
			if (g.list[i] > g.list[i+1]) {
				int temp = g.list[i];
				g.list[i] = g.list[i+1];
				g.list[i+1] = temp;
				swapped = true;
			}
		}
		render();
		x11.swapBuffers();
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;
	printf("\nBubble sort elapsed time was %.2f seconds\n", elapsed.count()/1000.0);
}

void insertionsort()
{
	int i, key, j;
	auto start = std::chrono::high_resolution_clock::now();
	for(i=1; i<amount; i++) {
		key = g.list[i];
		j = i-1;

		while(j >= 0 && g.list[j] > key) {
			g.list[j+1] = g.list[j];
			j--;
		}
		g.list[j+1] = key;
		render();
		x11.swapBuffers();
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;
	printf("\nInsertion sort elapsed time was %.2f seconds\n", elapsed.count()/1000.0);
}

void selectionsort()
{
	auto start = std::chrono::high_resolution_clock::now();
	for(int i=0; i<amount-1; i++) {
		for(int j=i+1; j<amount; j++) 
			if (g.list[j] < g.list[i]) SWAP(g.list[i], g.list[j]);
		
		render();
		x11.swapBuffers();
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;
	printf("\nSelection sort elapsed time was %.2f seconds\n", elapsed.count()/1000.0);
}

int partition (int low, int high)  
{  
    int pivot = g.list[high];
    int i = (low-1);
  
    for (int j = low; j <= high - 1; j++) {
        if (g.list[j] < pivot) {
            i++;
            SWAP(g.list[i], g.list[j]);  
        }  
    }  
    SWAP(g.list[i + 1], g.list[high]); 
    return (i + 1);  
}  
  
void quicksort(int low, int high)  
{  
    if (low < high)  
    {  
        int pi = partition(low, high);  
  
        quicksort(low, pi - 1);  
        render();
        x11.swapBuffers();	
        quicksort(pi+1, high);  
        render();
        x11.swapBuffers();	
    }  
}

void count()
{
	auto start = std::chrono::high_resolution_clock::now();
	int temp[g.yres];
	for(int i=0; i<g.yres; i++)
		temp[i] = 0;
	for(int i=0; i<amount; i++) {
		temp[g.list[i]] += 1;	
	}

	int j=0;
	for(int i=0; i<g.yres; i++) {
		while(temp[i] > 0) {
			g.list[j] = i;
			j++;
			if (j >= amount) break;
			temp[i]--;
			render();
			x11.swapBuffers();
		}
		if (j >= amount) break;
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;
	printf("\nCount sort elapsed time was %.2f seconds\n", elapsed.count()/1000.0);
}

void doubleSelectionsort()
{
	//EXPERIMENTAL SORTING FUNCTION*
 	auto start = std::chrono::steady_clock::now();
        
	int front=0, end=amount-1, min, max, minIdx, maxIdx;

	for(int i=front; i<=end; i++) {
		min = 99999;
		max = -1;
		for(int j=front; j<=end; j++) {
			if (g.list[j] >= max) {
				max = g.list[j];
				maxIdx = j;
			}
			else if (g.list[j] <= min) {
				min = g.list[j];
				minIdx = j;
			}
		}
		if (g.list[minIdx] == g.list[end] && g.list[front] == g.list[maxIdx]) {
			SWAP(g.list[minIdx], g.list[maxIdx]);
		}
		else if (g.list[maxIdx] == g.list[front]) {
			SWAP(g.list[end], g.list[maxIdx]);
			SWAP(g.list[front], g.list[minIdx]);
		}
		else {
			SWAP(g.list[front], g.list[minIdx]);
			SWAP(g.list[end], g.list[maxIdx]);
		}
		front++;
		end--;
		render();
		x11.swapBuffers();
	}

	auto endTime = std::chrono::steady_clock::now();
	std::chrono::duration<double, std::milli> elapsed = endTime - start;
	printf("\nDouble Selection sort elapsed time was %.2f seconds\n", elapsed.count()/1000.0);
        return;
}

void merge(int l, int m, int r) 
{ 
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 =  r - m; 

	int L[n1], R[n2]; 

	for (i = 0; i < n1; i++) 
		L[i] = g.list[l+i]; 
	for (j = 0; j < n2; j++) 
		R[j] = g.list[m+1+j]; 

	i = 0; 
	j = 0; 
	k = l; 
	while (i < n1 && j < n2) 
	{ 
		if (L[i] <= R[j]) { 
			g.list[k] = L[i]; 
			i++; 
		} else { 
			g.list[k] = R[j]; 
			j++; 
		} 
		k++; 
	} 

	while (i < n1) { 
		g.list[k] = L[i]; 
		i++; 
		k++; 
	} 

	while (j < n2) { 
		g.list[k] = R[j]; 
		j++; 
		k++; 
	} 
	render();
	x11.swapBuffers();
} 
  
void mergesort(int l, int r) 
{ 
	if (l < r) { 
		int m = l+(r-l)/2; 

		mergesort(l, m); 
		mergesort(m+1, r); 

		merge(l, m, r); 
	} 
}

void heapify(int n, int i)
{
	int largest = i;
	int child1 = 2*i + 1;
	int child2 = 2*i + 2;

	if(child1<n && g.list[child1] > g.list[largest])
		largest = child1;

	if(child2<n && g.list[child2] > g.list[largest])
		largest = child2;

	if(largest != i) {
		SWAP(g.list[i], g.list[largest]);
		heapify(n, largest);
	}
}

void heapsort(int n)
{
	for(int i=n/2-1; i>=0; i--) 
		heapify(n, i);
	for(int i=n-1; i>=0; i--) {
		SWAP(g.list[0], g.list[i]);
		heapify(i, 0);
		render();
		x11.swapBuffers();
	}
}

int check_keys(XEvent *e)
{
        if (e->type != KeyPress && e->type != KeyPress)
                return 0;
        int key = XLookupKeysym(&e->xkey, 0);
        switch(key) {
		case XK_1:
			shuffle();
			break;
		case XK_c:
			g.sorting = true;
			count();
			g.sorting = false;
			break;
		case XK_q: {
			g.sorting = true;
			auto start = std::chrono::high_resolution_clock::now();
			quicksort(0, amount-1);
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> elapsed = end - start;
			printf("\nQuick sort elapsed time was %.2f seconds\n", elapsed.count()/1000.0);
			g.sorting = false;
			break;
			   }
		case XK_b:
			g.sorting = true;
			bubblesort();
			g.sorting = false;
			break;
		case XK_i:
			g.sorting = true;
			insertionsort();
			g.sorting = false;
			break;
		case XK_s:
			g.sorting = true;
			selectionsort();
			g.sorting = false;
			break;
		case XK_d:
			g.sorting = true;
			doubleSelectionsort();
			g.sorting = false;
			break;
		case XK_m: {
			g.sorting = true;
			auto start = std::chrono::high_resolution_clock::now();
			mergesort(0,amount);
			g.sorting = false;
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> elapsed = end - start;
			printf("\nMerge sort elapsed time was %.2f seconds\n", elapsed.count()/1000.0);
			break;
			   }
		case XK_h: {
			g.sorting = true;
			auto start = std::chrono::high_resolution_clock::now();
			heapsort(amount);
			g.sorting = false;
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> elapsed = end - start;
			printf("\nHeap sort elapsed time was %.2f seconds\n", elapsed.count()/1000.0);
			break;
			   }
		case XK_Escape:
			return(1);
	}

	return(0);
}

void showmenu()
{
	int y = 20;
	int inc = 16;
	x11.set_color_3i(0, 255, 0);
	if(!g.sorting) {
		x11.drawString(10, y, "Menu");
		y+= inc;
		x11.drawString(10, y, "======");
		y+= inc;
		x11.drawString(10, y, "ESC - Exit program");
		y+= inc;
		x11.drawString(10, y, "1 - Shuffle list");
		y+= inc;
		x11.drawString(10, y, "B - Bubble sort");
		y+= inc;
		x11.drawString(10, y, "S - Selection sort");
		y+= inc;
		x11.drawString(10, y, "I - Insertion sort");
		y+= inc;
		x11.drawString(10, y, "M - Merge sort");
		y+= inc;
		x11.drawString(10, y, "H - Heap sort");
		y+= inc;
		x11.drawString(10, y, "Q - Quick sort");
		y+= inc;
		x11.drawString(10, y, "D - Double Selection sort");
		y+= inc;
		x11.drawString(10, y, "C - Counting sort");
	}
	else {
		x11.drawString(10, y, "Currently sorting....");
		y+= inc;
		x11.drawString(10, y, "Wait until list is sorted for button input");
	}
}

void physics() {}

void render()
{
	x11.clear_screen();
	double xpos = 0;
	for(int i=0; i<amount; i++) {
		x11.set_color_3i(255, 0, 0);
		x11.fillRectangle(xpos, g.yres-g.list[i], g.width, g.yres);
		x11.set_color_3i(255, 255, 255);
		x11.drawRectangle(xpos, g.yres-g.list[i], g.width, g.yres);
		xpos += g.width;
	}
	showmenu();
}
