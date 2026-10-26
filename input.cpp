
#include "input.h"
#include "form.h"
#include "shapeManager.h"
#include <GL/glut.h>
#include <cstdlib>
#include <iostream>

// Convert the mouse Y coordinate to drawing-area coordinates.
int getWorldY(int mouseY)
{
    return windowY - 1 - mouseY;
}

// =========================
// Mouse click
// =========================
void mymouse(int button, int state, int mouseX, int mouseY)
{



    int worldY = getWorldY(mouseY);


    // Stop moving a shape when the left button is released.
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {


        // Second click: set the final size of the shape.
        int xSize = mouseX - startX;
        int ySize = worldY - startY;

        if (drawingShape != nullptr)
        {
            drawingShape->updateSize(xSize, ySize);
        }

        firstClick = false;
        drawingShape = nullptr;

        glutPostRedisplay();

        selectedShape = -1;
        return;
    }

    // Handle clicks inside the toolbox separately.
    if (state == GLUT_DOWN && mouseY < toolsH)
    {
        if (button == GLUT_LEFT_BUTTON)
        {

            // Convert mouse Y to toolbox coordinates.
            int toolboxY = toolsH - 1 - mouseY;

            int boxSize = 35;
            int gap = 8;
            int paletteX = 180;
            int paletteY = 50;

            // Check whether a color was clicked.
            for (int i = 0; i < 10; i++)
            {
                int x = paletteX + i * (boxSize + gap);

                if (mouseX >= x && mouseX <= x + boxSize &&
                    toolboxY >= paletteY &&
                    toolboxY <= paletteY + boxSize)
                {
                    selectColor(i);
                    return;
                }
            }

            int boxSize2 = 35;
            int gap2 = 8;
            int paletteX2 = 180;
            int paletteY2 = 10;

            // Check whether a shape was clicked.
            for (int i = 0; i < 6; i++)
            {
                int x = paletteX2 + i * (boxSize2 + gap2);

                if (mouseX >= x && mouseX <= x + boxSize2 &&
                    toolboxY >= paletteY2 &&
                    toolboxY <= paletteY2 + boxSize2)
                {
                    activeShape = i;
                    glutPostRedisplay();
                    return;
                }
            }

    }
 // Do not create, select, or delete shapes in the toolbox.
        return;


        }




      int boxSize3 = 35;
    int gap3 = 8;
    int paletteX3 = 10;
    int paletteY3 = 50;
int funcX=-(windowX-mouseX-functionW);



    for (int i = 0; i < 4; i++)
    {


    int y = paletteY3 + i * (boxSize3 + gap3);


       if (funcX >= paletteX3 && funcX <= paletteX3 + boxSize3 &&
        worldY >= y && worldY <= y + boxSize3)
    {
       switch(i){
       case 0:mode=0;
       break;
        case 1:mode=1;
       break;
        case 2:mode=2;
       break;
       case 3:mode=3;

       }

        return;
    }

    }

    // Delete a shape with the right mouse button.
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {

        int ind = -1;

        for (int i = shapes.size() - 1; i >= 0; i--)
        {
            if (shapes[i]->contains(mouseX, worldY))
            {
                ind = i;
                break;
            }
        }

        if (ind != -1)
        {
            deleteShape(ind);
            return;
        }
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // If a shape is not currently being drawn, check whether
        // the click selects an existing shape to move.
        if (!firstClick)
        {
            int ind = findShapeAt(mouseX, worldY);

            if (ind != -1)
            {
                selectedShape = ind;
                lastX = mouseX;
                lastY = worldY;
                return;
            }
        }

        // Use the selected shape type, or choose a random one.
        if (activeShape != -1)
            type = activeShape;
        else
            type = rand() % 5;



        // First click: create the shape with zero size.
        if (!firstClick)
        {
            int index = findShapeAt(mouseX, worldY);

            if (index != -1)
            {
                return;
            }

            startX = mouseX;
            startY = worldY;

            firstClick = true;

            createRandomShape(type, startX, startY, 0, 0);
            drawingShape = shapes.back();
        }
    }
}

// =========================
// Mouse movement
// =========================
void mousemove(int x, int y)
{

    // Ignore mouse movement inside the toolbox.

        if(y < toolsH)
        {
            return;
        }

    int worldX = x;
    int worldY = getWorldY(y);

    // Move an existing shape while the mouse button is held.
    if (selectedShape != -1)
    {
        int dx = worldX - lastX;
        int dy = worldY - lastY;

        shapes[selectedShape]->move(dx, dy);

        lastX = worldX;
        lastY = worldY;

        glutPostRedisplay();
        return;
    }

    // Do nothing if a shape is not currently being created.
    if (!firstClick || drawingShape == nullptr)
        return;

    // Update the size of the shape while it is being drawn.
    drawingShape->updateSize(
        worldX - startX,
        worldY - startY);

    glutPostRedisplay();
}

// =========================
// Keyboard
// =========================
void mykey(unsigned char key, int mouseX, int mouseY)
{
    // Quit the program.
    if (key == 'Q' || key == 'q')
    {
        for (Shape *shape : shapes)
            delete shape;

        shapes.clear();
        exit(0);
    }

    // Delete a randomly selected shape.
    if (key == 'D' || key == 'd')
    {
        if (!shapes.empty())
        {
            int randomIndex = rand() % shapes.size();
            deleteShape(randomIndex);
        }

        return;
    }

    // Select a color using the number keys 0-9.
    if (key >= '0' && key <= '9')
    {
        selectColor(key - '0');
    }

    // Select a shape using R, T, L, or S.
    switch (key)
    {
    case 'R':
    case 'r':
        activeShape = 0;
        break;

    case 'T':
    case 't':
        activeShape = 1;
        break;

    case 'L':
    case 'l':
        activeShape = 2;
        break;

    case 'S':
    case 's':
        activeShape = 3;
        break;

    case 'C':
    case 'c':
        activeShape = 4;
        break;

    case 'E':
    case 'e':
        activeShape = 5;
        break;
    }

    glutPostRedisplay();
}
