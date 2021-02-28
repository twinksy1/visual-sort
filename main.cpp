/* ORIGINAL CREATOR: Juan Villasenor
 * Feel free to use & modify :)
*/
#include "display.h"
#include "global.h"

#include <unistd.h>
#include <cmath>
#include <ctime>
#include <chrono>

//int amount = 100;
// Bigger BARWIDTH number = wider rectangles
// Smaller BARWIDTH number = less wide
//int BARWIDTH=30;
void swap(int& a, int& b) {
	int tmp = a;
	a = b;
	b = tmp;
}

//Holds all the global vars, list of nums is here
Global g;
X11 x11(g.xres,g.yres);

void render();
void handleOption();
void physics();

int main() {
	g.init();
	srand((unsigned)time(NULL));
	int done = 0;
	printf("\x1b[1m	MENU OPTIONS ON TOP LEFT OF SCREEN\n");
	sleep(2);
	while(!done) {
		g.processSettings();
		while(x11.getPending()) {
			XEvent e;
			x11.getNextEvent(&e);
			checkResize(&e,g);
			checkMouse(&e,g);
			done = checkKeys(&e,g);
		}
		if(g.curOption != NULL) {
			handleOption();
		} else {
			//rendering every frame
			render();
			x11.swapBuffers();
		}
	}
	return 0;
}

void shuffle(int* list, int amount) {
	for(int i=0; i<amount; i++) {
		int idx1 = rand() % amount;
		int idx2 = rand() % amount;
		swap(list[idx1], list[idx2]);

		render();
		x11.swapBuffers();
	}
}

void bubblesort(int* list, int amount) {
	bool swapped = true;

	while(swapped) {
		swapped = false;
		for(int i=0; i<amount-1; i++) {
			if(list[i] > list[i+1]) {
				swap(list[i], list[i+1]);
				swapped = true;
				render();
				x11.swapBuffers();
			}
			if(list[i] < list[i-1]) {
				swap(list[i], list[i-1]);
				swapped = true;
				render();
				x11.swapBuffers();
			}
		}
	}	
}

void cocktailsort(int* list, int amount) {
	bool swapped = true;
	int endbound = amount, startbound = 0;
	int i=0;

	while(swapped) {
		swapped = false;
		for(i=startbound; i<endbound; i++) {
			if (list[i] > list[i+1]) {
				swap(list[i], list[i+1]);
				swapped = true;
				render();
				x11.swapBuffers();
			}
		}
		for(i-=1; i>startbound; i--) {
			if (list[i] < list[i-1]) {
				swap(list[i], list[i-1]);
				swapped = true;
				render();
				x11.swapBuffers();
			}
		}
		startbound++;
		endbound--;
	}
}

void insertionsort(int* list, int amount) {
	int i, key, j;
	for(i=1; i<amount; i++) {
		key = list[i];
		j = i-1;

		while(j >= 0 && list[j] > key) {
			list[j+1] = list[j];
			j--;
			render();
			x11.swapBuffers();
		}
		list[j+1] = key;
		render();
		x11.swapBuffers();
	}
}

void selectionsort(int* list, int amount) {
	for(int i=0; i<amount-1; i++) {
		for(int j=i+1; j<amount; j++) 
			if (list[j] < list[i]) {
				swap(list[i], list[j]);
				render();
				x11.swapBuffers();
			}
	}
}
/*
int partition (int low, int high)  
{  
	int pivot = g.list[high];
	int i = (low-1);
  
	for (int j = low; j <= high - 1; j++) {
		if (g.list[j] < pivot) {
			i++;
			SWAP(g.list[i], g.list[j]);  
			render();
			x11.swapBuffers();	
        	}  
    	}  
    	SWAP(g.list[i + 1], g.list[high]); 
	render();
	x11.swapBuffers();	
    	return (i + 1);  
}  
  
void quicksort(int low, int high)  
{  
    if (low < high)  
    {  
        int pi = partition(low, high);  
  
        quicksort(low, pi - 1);  
        quicksort(pi+1, high);  
    }  
}
*/
void shellsort(int* list, int amount) {
	int h = amount / 2;
	int key, j;

	for(h; h>0; h /= 2) {
		for(int i=h; i<amount; i++) {
			key = list[i];
			for(j=i; j>=h && list[j-h] > key; j-=h) {
				list[j] = list[j-h];
				render();
				x11.swapBuffers();
			}
			list[j] = key;
			render();
			x11.swapBuffers();
		}
	}	
}

void doubleSelectionsort(int* list, int amount) {
/* 	auto start = std::chrono::steady_clock::now();
        
	int front=0, end=amount-1, min, max, minIdx, maxIdx;

	for(front; front<end;) {
		min = g.list[front];
		max = g.list[end];
		maxIdx = end;
		minIdx = front;
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
			render();
			x11.swapBuffers();
		} else if (g.list[maxIdx] == g.list[front]) {
			SWAP(g.list[end], g.list[maxIdx]);
			render();
			x11.swapBuffers();
			SWAP(g.list[front], g.list[minIdx]);
			render();
			x11.swapBuffers();
		} else {
			SWAP(g.list[front], g.list[minIdx]);
			render();
			x11.swapBuffers();
			SWAP(g.list[end], g.list[maxIdx]);
			render();
			x11.swapBuffers();
		}
		front++;
		end--;
	}

	auto endTime = std::chrono::steady_clock::now();
	std::chrono::duration<double, std::milli> elapsed = endTime - start;
	printf("\nDouble Selection sort elapsed time was %.2f seconds\n", elapsed.count()/1000.0);
*/
}
/*
void merge(int l, int m, int r) 
{ 
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 =  r - m; 

	int L[n1], R[n2]; 

	for(i = 0; i < n1; i++) 
		L[i] = g.list[l+i]; 
	for(j = 0; j < n2; j++) 
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
		render();
		x11.swapBuffers();
	} 

	while(i < n1) { 
		g.list[k] = L[i]; 
		i++; 
		k++; 
		render();
		x11.swapBuffers();
	} 

	while(j < n2) { 
		g.list[k] = R[j]; 
		j++; 
		k++; 
		render();
		x11.swapBuffers();
	} 
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

	if (child1<n && g.list[child1] > g.list[largest])
		largest = child1;

	if (child2<n && g.list[child2] > g.list[largest])
		largest = child2;

	if (largest != i) {
		SWAP(g.list[i], g.list[largest]);
		render();
		x11.swapBuffers();
		heapify(n, largest);
	}
}

void heapsort()
{
	int n = amount;
	for(int i=n/2-1; i>=0; i--) 
		heapify(n, i);
	for(int i=n-1; i>=0; i--) {
		SWAP(g.list[0], g.list[i]);
		render();
		x11.swapBuffers();
		heapify(i, 0);
	}
}
*/

void handleOption() {
	switch(g.curOption->getValue()) {
		case SHUFFLE:
			shuffle(g.list, g.amount);
			break;
		case BUBBLESORT:
			bubblesort(g.list, g.amount);
			break;
		case SELECTIONSORT:
			selectionsort(g.list, g.amount);
			break;
		case INSERTIONSORT:
			insertionsort(g.list, g.amount);
			break;
	}
	g.curOption = NULL;
}

void showmenu() {
	g.renderMenu(x11);
}

void render() {
	x11.clearScreen(g.xres, g.yres);
	double xpos = 0;
	for(int i=0; i<g.amount; i++) {
		x11.setColor(255, 0, 0);
		x11.fillRectangle(xpos, g.yres-g.list[i], g.width, g.yres);
		x11.setColor(255, 255, 255);
		x11.drawRectangle(xpos, g.yres-g.list[i], g.width, g.yres);
		xpos += g.width;
	}
	showmenu();
}
