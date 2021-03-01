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


void render();
void handleOption();
void physics();

//Holds all the global vars, list of nums is here
Global g;
X11 x11(g.xres,g.yres);
void swap(int& a, int& b) {
	int tmp = a;
	a = b;
	b = tmp;
	render();
	x11.swapBuffers();
}

int main() {
	g.init();
	srand((unsigned)time(NULL));
	int done = 0;
	printf("\x1b[1m	MENU OPTIONS ON TOP OF SCREEN\n");
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
			g.sorting = true;
			handleOption();
		} else {
			g.sorting = false;
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
			}
			if(list[i] < list[i-1]) {
				swap(list[i], list[i-1]);
				swapped = true;
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
			}
		}
		for(i-=1; i>startbound; i--) {
			if (list[i] < list[i-1]) {
				swap(list[i], list[i-1]);
				swapped = true;
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
			}
	}
}

int partition (int* list, int low, int high) {  
	int pivot = list[high];
	int i = (low-1);
  
	for (int j = low; j <= high - 1; j++) {
		if (list[j] < pivot) {
			i++;
			swap(list[i], list[j]);  
			render();
			x11.swapBuffers();	
		}  
	}  
    swap(list[i + 1], list[high]); 	
	return (i + 1);  
}  
  
void quicksort(int* list, int low, int high)  
{  
    if (low < high)  
    {  
        int pi = partition(list, low, high);  
  
        quicksort(list, low, pi - 1);  
        quicksort(list, pi+1, high);  
    }  
}

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

void biSelectionsort(int* list, int amount) {
	for(int start=0, end=amount-1; start<end; start++,end--) {
		for(int i=start, j=end; i<=j; i++,j--) {
			if(list[i] > list[j]) {
				swap(list[i], list[j]);
			}
			if(list[i] < list[start]) {
				swap(list[i], list[start]);
			}
			if(list[j] > list[end]) {
				swap(list[j], list[end]);
			}
		}
	}
}

void merge(int* list, int l, int m, int r) { 
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 =  r - m; 

	int L[n1], R[n2]; 

	for(i = 0; i < n1; i++) 
		L[i] = list[l+i]; 
	for(j = 0; j < n2; j++) 
		R[j] = list[m+1+j]; 

	i = 0; 
	j = 0; 
	k = l; 
	while (i < n1 && j < n2) 
	{ 
		if (L[i] <= R[j]) { 
			list[k] = L[i]; 
			i++; 
		} else { 
			list[k] = R[j]; 
			j++; 
		} 
		k++; 
		render();
		x11.swapBuffers();
	} 

	while(i < n1) { 
		list[k] = L[i]; 
		i++; 
		k++; 
		render();
		x11.swapBuffers();
	} 

	while(j < n2) { 
		list[k] = R[j]; 
		j++; 
		k++; 
		render();
		x11.swapBuffers();
	} 
} 
  
void mergesort(int* list, int l, int r) { 
	if (l < r) { 
		int m = l+(r-l)/2; 

		mergesort(list, l, m); 
		mergesort(list, m+1, r); 

		merge(list, l, m, r); 
	} 
}

void heapify(int* list, int n, int i) {
	int largest = i;
	int child1 = 2*i + 1;
	int child2 = 2*i + 2;

	if (child1<n && list[child1] > list[largest])
		largest = child1;

	if (child2<n && list[child2] > list[largest])
		largest = child2;

	if (largest != i) {
		swap(list[i], list[largest]);
		heapify(list, n, largest);
	}
}

void heapsort(int* list, int amount) {
	int n = amount;
	for(int i=n/2-1; i>=0; i--) 
		heapify(list, n, i);
	for(int i=n-1; i>=0; i--) {
		swap(list[0], list[i]);
		heapify(list, i, 0);
	}
}


void handleOption() {
	if(g.curOption == NULL) return;
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
		case SHELLSORT:
			shellsort(g.list, g.amount);
			break;
		case BISELECTIONSORT:
			biSelectionsort(g.list, g.amount);
			break;
		case COCKTAILSORT:
			cocktailsort(g.list, g.amount);
			break;
		case MERGESORT:
			mergesort(g.list, 0, g.amount);
			break;
		case HEAPSORT:
			heapsort(g.list, g.amount);
			break;
		case QUICKSORT:
			quicksort(g.list, 0, g.amount);
			break;
	}
	g.curOption = NULL;
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
	g.renderMenu(x11);
}
