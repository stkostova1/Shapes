#ifndef SHAPE_MANAGER_H
#define SHAPE_MANAGER_H

#include "global.h"

void createRandomShape(
    int type, int x, int y, int xSize, int ySize
);

void drawAll();

int findShapeAt(float mouseX, float mouseY);

void deleteShape(int index);

#endif
