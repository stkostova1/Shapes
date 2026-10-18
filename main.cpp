
#include <GL/glut.h>
#include "form.h"
#include "shapeManager.h"
#include "input.h"
#include "global.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

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
    int paletteY = 32;

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

// =========================
// Display
// =========================

void mydisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up the DRAWING AREA above the toolbox.
    glViewport(0, toolsH, windowX, windowY - toolsH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowX, 0, windowY - toolsH);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw all shapes.

    drawAll();

    // Set up the TOOLBOX at the bottom of the window.
    glViewport(0, 0, windowX, toolsH);

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
            glVertex2f(110, 65);
            glVertex2f(90, 35);
            glVertex2f(130, 35);
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
        }
    }

    // Draw the color palette.
    drawPalette();

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

    int main(int argc, char** argv)
    {
        srand((unsigned int)time(nullptr));

        glutInit(&argc, argv);
        glutInitWindowSize(windowX, windowY);
        glutCreateWindow("Shapes");

        // Add some initial shapes to the drawing.
        shapes.push_back(new Rect(50, 50, 200, 100));
        shapes.push_back(new Square(300, 50, 150));
        shapes.push_back(new Triangle(500, 50, 150, 150));
        shapes.push_back(new Line(100, 300, 500, 400));
        // shapes.push_back(new Circle(200.0f, 150.0f, 50.0f));

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
