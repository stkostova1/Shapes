
#include "input.h"
#include "form.h"
#include "shapeManager.h"
#include <GL/glut.h>
#include <cstdlib>



//Convert the mouse Y coordinate to drawing-area coordinates.
int getWorldY(int mouseY)
{
    return windowY - 1 - mouseY - toolsH;
}

// =========================
// Mouse click
// =========================
void mymouse(int button, int state, int mouseX, int mouseY)
{
    // Stop moving a shape when the left button is released.
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        selectedShape = -1;
        return;
    }

    // Handle clicks inside the toolbox separately.
    if (state == GLUT_DOWN && mouseY >= windowY - toolsH)
    {
        if (button == GLUT_LEFT_BUTTON)
        {
            // Convert the mouse Y coordinate to toolbox coordinates.
            int toolboxY = windowY - 1 - mouseY;

            int boxSize = 35;
            int gap = 8;
            int paletteX = 180;
            int paletteY = 32;

            // Check whether a palette color was clicked.
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
        }

        // Do not create, select, or delete shapes in the toolbox.
        return;
    }

    // Convert the mouse position to drawing-area coordinates.
    int worldY = getWorldY(mouseY);

    // Delete a shape with the right mouse button.
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {

        int ind=-1;

        for (int i = shapes.size() - 1; i >= 0; i--)
        {
            if (shapes[i]->contains(mouseX,worldY))
            {
                ind=i;
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
            type = rand() % 4;

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
        // Second click: set the final size of the shape.
        else
        {
            int xSize = mouseX - startX;
            int ySize = worldY - startY;

            if (drawingShape != nullptr)
                drawingShape->updateSize(xSize, ySize);

            firstClick = false;
            drawingShape = nullptr;

            glutPostRedisplay();
        }
    }
}

// =========================
// Mouse movement
// =========================
void mousemove(int x, int y)
{
    // Ignore mouse movement inside the toolbox.
    if (y >= windowY - toolsH)
        return;

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
        worldY - startY
    );

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
        for (Shape* shape : shapes)
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
    }

    glutPostRedisplay();
}
