#include "global.h"

int windowX = 800;
int windowY = 600;
int toolsH = 100;

bool firstClick = false;
int startX = 0;
int startY = 0;
int type = 0;

int activeShape = -1;

int selectedShape = -1;
int lastX = 0;
int lastY = 0;

bool activeColor = false;
unsigned char activeR = 255;
unsigned char activeG = 0;
unsigned char activeB = 0;

Shape* drawingShape = nullptr;
std::vector<Shape*> shapes;

const unsigned char colors[10][3] =
{
    {255, 0, 0},     // 0: Red
    {0, 255, 0},     // 1: Green
    {0, 0, 255},     // 2: Blue
    {255, 255, 0},   // 3: Yellow
    {0, 255, 255},   // 4: Cyan
    {255, 0, 255},   // 5: Magenta
    {255, 165, 0},   // 6: Orange
    {128, 0, 128},   // 7: Purple
    {255, 255, 255}, // 8: White
    {0, 0, 0}        // 9: Black
};
