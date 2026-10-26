#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#include "form.h"

extern int windowX;
extern int windowY;
extern int toolsH;
extern int functionW;
extern int mode;

extern bool firstClick;
extern int startX;
extern int startY;
extern int type;

extern int activeShape;
extern int selectedShape;
extern int lastX;
extern int lastY;

extern bool activeColor;
extern unsigned char activeR;
extern unsigned char activeG;
extern unsigned char activeB;

extern Shape* drawingShape;
extern std::vector<Shape*> shapes;
extern const unsigned char colors[10][3];
#endif
