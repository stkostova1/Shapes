
#include <GL/glut.h>
#include "form.h"
#include "shapeManager.h"
#include "input.h"
#include "global.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

// Set the active color using an index from the palette.
void selectColor(int index)
{
    if (index < 0 || index >= 10)
        return;

    activeR = colors[index][0];
    activeG = colors[index][1];
    activeB = colors[index][2];

    activeColor = true;
    glutPostRedisplay();
}

// Draw the color palette in the toolbox.
void drawPalette()
{
    int boxSize = 35;
    int gap = 8;
    int paletteX = 180;
    int paletteY = 50;

    for (int i = 0; i < 10; i++)
    {
        int x = paletteX + i * (boxSize + gap);

        // Draw the color box.
        glColor3ub(colors[i][0],
                   colors[i][1],
                   colors[i][2]);

        glBegin(GL_QUADS);
        glVertex2i(x, paletteY);
        glVertex2i(x + boxSize, paletteY);
        glVertex2i(x + boxSize, paletteY + boxSize);
        glVertex2i(x, paletteY + boxSize);
        glEnd();

        // Draw the box border.
        glColor3f(0.2f, 0.2f, 0.2f);

        glBegin(GL_LINE_LOOP);
        glVertex2i(x, paletteY);
        glVertex2i(x + boxSize, paletteY);
        glVertex2i(x + boxSize, paletteY + boxSize);
        glVertex2i(x, paletteY + boxSize);
        glEnd();
    }



}

void drawShapesPalette()
{
    int boxSize = 35;
    int gap = 8;
    int paletteX = 180;
    int paletteY = 10;

    for (int i = 0; i < 6; i++)
    {
        int x = paletteX + i * (boxSize + gap);

        // Draw the color box.
        glColor3f(1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);
        glVertex2i(x, paletteY);
        glVertex2i(x + boxSize, paletteY);
        glVertex2i(x + boxSize, paletteY + boxSize);
        glVertex2i(x, paletteY + boxSize);
        glEnd();

        // Draw the box border.
        glColor3f(0.2f, 0.2f, 0.2f);

        glBegin(GL_LINE_LOOP);
        glVertex2i(x, paletteY);
        glVertex2i(x + boxSize, paletteY);
        glVertex2i(x + boxSize, paletteY + boxSize);
        glVertex2i(x, paletteY + boxSize);
        glEnd();

         switch (i)
    {
    case 0: // Rectangle
        glBegin(GL_LINE_LOOP);
        glVertex2f(x + 5,  paletteY + 10);
        glVertex2f(x + 30, paletteY + 10);
        glVertex2f(x + 30, paletteY + 25);
        glVertex2f(x + 5,  paletteY + 25);
        glEnd();
        break;

    case 1: // Triangle
        glBegin(GL_LINE_LOOP);
        glVertex2f(x + 17, paletteY + 30);
        glVertex2f(x + 5,  paletteY + 4);
        glVertex2f(x + 30, paletteY + 4);
        glEnd();
        break;

    case 2: // Line
        glBegin(GL_LINES);
        glVertex2f(x + 5,  paletteY + 8);
        glVertex2f(x + 30, paletteY + 27);
        glEnd();
        break;

    case 3: // Square
        glBegin(GL_LINE_LOOP);
        glVertex2f(x + 8,  paletteY + 8);
        glVertex2f(x + 27, paletteY + 8);
        glVertex2f(x + 27, paletteY + 27);
        glVertex2f(x + 8,  paletteY + 27);
        glEnd();
        break;

    case 4: // Circle
        glBegin(GL_LINE_LOOP);
        for (int a = 0; a < 360; a += 10)
        {
            float angle = a * 3.14159265f / 180.0f;
            glVertex2f(
                x + 17.5f + 12 * cos(angle),
                paletteY + 17.5f + 12 * sin(angle)
            );
        }
        glEnd();
        break;

         case 5: // Equilateral Triangle

            glBegin(GL_LINE_LOOP);
            glVertex2f(x + 17.5f, paletteY + 27.5f);
            glVertex2f(x + 5, paletteY + 8);
            glVertex2f(x + 30, paletteY + 8);
            glEnd();
            break;
    }
    }
}

    void drawFunction()
{
    int boxSize = 35;
    int gap = 8;
    int paletteX = 10;
    int paletteY = 50;

    for (int i = 0; i < 4; i++)
    {
        int y = paletteY + i * (boxSize + gap);

        // Draw the white box
        glColor3f(1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);
        glVertex2i(paletteX, y);
        glVertex2i(paletteX + boxSize, y);
        glVertex2i(paletteX + boxSize, y + boxSize);
        glVertex2i(paletteX, y + boxSize);
        glEnd();

        // Draw the  border

        if (mode == i)
    glColor3f(1.0f, 0.0f, 0.0f);
else
    glColor3f(0.0f, 0.0f, 0.0f);

        glBegin(GL_LINE_LOOP);
        glVertex2i(paletteX, y);
        glVertex2i(paletteX + boxSize, y);
        glVertex2i(paletteX + boxSize, y + boxSize);
        glVertex2i(paletteX, y + boxSize);
        glEnd();
    }
}

// =========================
// Display
// =========================

void mydisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up the DRAWING AREA below the toolbox.
    glViewport(0, 0, windowX-functionW, windowY - toolsH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowX-functionW, 0, windowY - toolsH);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Clip all shapes to the drawing area.


    drawAll();



    // Set the viewport to the right side of the window.
         glViewport(windowX - functionW, 0, functionW, windowY - toolsH);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0, functionW, 0, windowY - toolsH);

glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

// Draw the function toolbox background.
glColor3f(0.7f, 0.7f, 0.7f);

glBegin(GL_QUADS);
glVertex2f(0, 0);
glVertex2f(functionW, 0);
glVertex2f(functionW, windowY - toolsH);
glVertex2f(0, windowY - toolsH);
glEnd();
drawFunction();

    // Set up the TOOLBOX at the top of the window.
    //0,500-100,600,100
    glViewport(0, windowY - toolsH, windowX, toolsH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowX, 0, toolsH);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw the toolbox background.
    glColor3f(0.7f, 0.7f, 0.7f);

    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(windowX, 0);
    glVertex2f(windowX, toolsH);
    glVertex2f(0, toolsH);
    glEnd();



    // Draw the separator between the indicators and the palette.
    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);
    glVertex2f(155, 10);
    glVertex2f(155, toolsH - 10);
    glEnd();

    // Show the currently selected color.
    if (activeColor)
    {
        glColor3ub(activeR, activeG, activeB);

        glBegin(GL_QUADS);
        glVertex2f(10, 20);
        glVertex2f(70, 20);
        glVertex2f(70, 80);
        glVertex2f(10, 80);
        glEnd();
    }

    // Show the currently selected shape.
    if (activeShape != -1)
    {
        // Draw the background of the shape indicator.
        glColor3f(1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);
        glVertex2f(80, 20);
        glVertex2f(140, 20);
        glVertex2f(140, 80);
        glVertex2f(80, 80);
        glEnd();

        // Draw a symbol representing the selected shape.
        glColor3f(0.0f, 0.0f, 0.0f);

        switch (activeShape)
        {
        case 0: // Rectangle
            glBegin(GL_LINE_LOOP);
            glVertex2f(90, 35);
            glVertex2f(130, 35);
            glVertex2f(130, 65);
            glVertex2f(90, 65);
            glEnd();
            break;

        case 1: // Triangle
            glBegin(GL_LINE_LOOP);
            glVertex2f(110, 70);
            glVertex2f(90, 30);
            glVertex2f(130, 30);
            glEnd();
            break;

        case 2: // Line
            glBegin(GL_LINES);
            glVertex2f(90, 35);
            glVertex2f(130, 65);
            glEnd();
            break;

        case 3: // Square
            glBegin(GL_LINE_LOOP);
            glVertex2f(95, 35);
            glVertex2f(125, 35);
            glVertex2f(125, 65);
            glVertex2f(95, 65);
            glEnd();
            break;

        case 4: // Circle

            glBegin(GL_LINE_LOOP);

            for (int i = 0; i < 360; i += 10)
            {
                float angle = i * 3.14159f / 180.0f;

                glVertex2f(
                    110 + 20 * cos(angle),
                    50 + 20 * sin(angle));
            }

            glEnd();
            break;

        case 5: // Equilateral Triangle

            glBegin(GL_LINE_LOOP);
            glVertex2f(110, 65);
            glVertex2f(90, 35);
            glVertex2f(130, 35);
            glEnd();
            break;
        }
    }

    // Draw the  palette.
    drawPalette();
    drawShapesPalette();

    glFlush();
}

// =========================
// Initialization
// =========================

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

// =========================
// Window resizing
// =========================

void myReshape(int w, int h)
{
    windowX = w;
    windowY = h;

    glViewport(0, 0, w, h);

    glutPostRedisplay();
}

// =========================
// Main
// =========================

int main(int argc, char **argv)
{
    //make the colors random each time the program is run
    srand((unsigned int)time(nullptr));

    glutInit(&argc, argv);
    glutInitWindowSize(windowX, windowY);
    glutCreateWindow("Shapes");

    // Add some initial shapes to the drawing.


    init();

    // Register the functions that handle drawing and user input.
    glutDisplayFunc(mydisplay);
    glutReshapeFunc(myReshape);
    glutMouseFunc(mymouse);
    glutMotionFunc(mousemove);
    glutPassiveMotionFunc(mousemove);
    glutKeyboardFunc(mykey);

    glutMainLoop();

    return 0;
}
