#include "shapeManager.h"
#include <GL/glut.h>

// Create a shape and add it to the existing shapes vector.
void createRandomShape(
    int type, int x, int y, int xSize, int ySize)
{
    Shape* newShape = nullptr;

    switch (type)
    {
    case 0:
        newShape = new Rect(x, y, xSize, ySize);
        break;

    case 1:
        newShape = new Triangle(x, y, xSize, ySize);
        break;

    case 2:
        newShape = new Line(x, y, x + xSize, y + ySize);
        break;

    case 3:
        newShape = new Square(x, y, xSize);
        break;

    case 4:
        newShape = new Circle(x, y, xSize);
        break;

    case 5:
        newShape = new EquilateralTriangle(x, y, xSize);
        break;

    default:
        return;
    }

    if (activeColor)
    {
        newShape->setColor(activeR, activeG, activeB);
    }

    shapes.push_back(newShape);
    glutPostRedisplay();
}

// Draw backmost shapes first, so shapes[0] is drawn last and appears on top.
void drawAll()
{
    for (int i = shapes.size() - 1; i >= 0; i--)
    {
        shapes[i]->draw();
    }
}

// Find the frontmost shape under the mouse.
int findShapeAt(float mouseX, float mouseY)
{
    for (int i = 0; i < shapes.size(); ++i)
    {
        if (shapes[i]->contains(mouseX, mouseY))
        {
            return i;
        }
    }

    return -1;
}

// Delete a shape by index.
void deleteShape(int index)
{
    if (index < 0 || index >= shapes.size())
    {
        return;
    }

    if (shapes[index] == drawingShape)
    {
        drawingShape = nullptr;
        firstClick = false;
    }

    delete shapes[index];
    shapes.erase(shapes.begin() + index);

    glutPostRedisplay();
}
