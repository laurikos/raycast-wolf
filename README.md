# Raycasting Wolfenstein 3D style

---

### What and Why

### What:
Raycasting "engine". Creates a small map with some walls and player movement.
All the walls are drawn with the raycasting techniques originated from the video game Wolfenstein 3D.

Draws a minimap with the player and ray positions.

### Why:
I had spare time. Haven't coded in C in few years.
Found a nice tutorial by Gustavo Pezzi which was true to the original game by Romero and Carmack.
Wanted to refresh memories of coding and playing the Wolfenstein with a 30kg laptop with a black/white 320x200 screen that I got from my father back in the day.

---

## js/

This a javascript implementation based on great tutorial by Gustavo Pezzi, which is based on Wolfenstein 3D video game.

### RUNNING:
This project should work just by copying all files in directory and opening index.html from filesystem with browser.

--- 

## c/

Here I wanted to refresh my memory how to C
So the implementation is now more me (and little bit of copilot, obviously)

### RUNNING:
This uses SDL2 and SDL2\_image:
```

$ sudo apt install libsdl2-image-dev
$ sudo apt install libsdl2-dev

```

Makefile should work with clang or gcc:

```
$ CC=clang make
$ CC=gcc make
```

Running make should place all the build stuff into ./bin/ dir
from where the ./raycast executable should be found.

Only ever tested on my two different linux machines.

---
