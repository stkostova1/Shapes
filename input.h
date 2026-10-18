#ifndef INPUT_H
#define INPUT_H

#include "global.h"

void mymouse(int button, int state, int mouseX, int mouseY);
void mousemove(int x, int y);
void mykey(unsigned char key, int mouseX, int mouseY);

void selectColor(int index);
int getWorldY(int mouseY);

#endif
