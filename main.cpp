
#include <GL/glut.h>
#include "form.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// =========================
// Global variables
// =========================

int windowX = 800;
int windowY = 600;
int toolsH = 100;

// Variables used to create a shape with two clicks.
bool firstClick = false;
int startX = 0;
int startY = 0;
int type = 0;

// Currently selected shape type: -1 means random.
int activeShape = -1;

// Variables used to select and move an existing shape.
int selectedShape = -1;
int lastX = 0;
int lastY = 0;

// Currently selected drawing color.
bool activeColor = false;
unsigned char activeR = 255;
unsigned char activeG = 0;
unsigned char activeB = 0;

// Pointer to the shape currently being created.
Shape* drawingShape = nullptr;

// All shapes in the drawing.
vector<Shape*> shapes;

// Available colors, shared by the keyboard and palette.
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

// Convert the mouse Y coordinate to drawing-area coordinates.
int getWorldY(int mouseY)
{
    return windowY - 1 - mouseY - toolsH;
}

// =========================
// Shape management
// =========================

// Delete a shape from the vector and free its memory.
void deleteShape(int i)
{
    // Cancel the current drawing if this is the shape being created.
    if (shapes[i] == drawingShape)
    {
        drawingShape = nullptr;
        firstClick = false;
    }

    delete shapes[i];
    shapes.erase(shapes.begin() + i);

    glutPostRedisplay();
}

// Create a shape based on its type and initial dimensions.
void createRandomShape(int type, int x, int y,
                       int xSize, int ySize)
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
        newShape = new Line(
            x, y, x + xSize, y + ySize
        );
        break;

    case 3:
        newShape = new Square(x, y, xSize);
        break;
    }

    if (newShape == nullptr)
        return;

    // Apply the selected color, if one has been chosen.
    if (activeColor)
    {
        newShape->setColor(activeR, activeG, activeB);
    }

    shapes.push_back(newShape);

    glutPostRedisplay();
}

// Return the index of the topmost shape at a given position.
// Return -1 if no shape is found.
int findShapeAt(int x, int y)
{
    // Search backwards because later shapes are drawn on top.
    for (int i = (int)shapes.size() - 1; i >= 0; i--)
    {
        if (shapes[i]->contains(x, y))
        {
            return i;
        }
    }

    return -1;
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
    for (Shape* shape : shapes)
    {
        shape->draw();
    }

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
// Mouse clicks
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
        int index = findShapeAt(mouseX, worldY);

        if (index != -1)
        {
            deleteShape(index);
            return;
        }
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // If a shape is not currently being drawn, check whether
        // the click selects an existing shape to move.
        if (!firstClick)
        {
            int index = findShapeAt(mouseX, worldY);

            if (index != -1)
            {
                selectedShape = index;
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
    shapes.push_back(new Line(200, 400, 600, 500));

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
