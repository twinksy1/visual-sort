## ABOUT
> This is a graphical application that visually represents the way popular sorting algorithms function. Sorting algorithms can be difficult to understand and being able to see the ways they swap help visual learners, like myself, understand most of them much better.
## BUILDING
* X11 library needs to be installed
	* **Linux**: *sudo apt install libx11-dev*
* X11 extensions must also be installed
	* **Linux**: *sudo apt-get install libxtst-dev*
---
Then in cloned directory:
* **Build**: *make*
* **Build & run**: *make run*
---
Execution: ***./sort***
## FEATURES
* ~~List is generated dynamically depending on current window size~~ List amount is now generated with the green slider
* ~~Changing macro BARWIDTH changes how wide or small rectangles should be, 30 is default good size~~ Width of rectangles now only depends on the amount and X resolution
* ~~terminal shows how long each sort took in seconds~~ Removed elapsed time
* Click & drag slider to change the amount of elements in the list (*amount displayed on slider*)
* Clickable buttons to choose between sorting algorithms
* Shuffle button to quickly shuffle the list
## GIF OF PROGRAM EXECUTION
![GIF](./visualsort.gif)
