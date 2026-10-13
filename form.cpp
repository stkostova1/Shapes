
#include "form.h"
#include <GL/glut.h>
#include <cstdlib>
#include <algorithm>
#include <cmath>

// =========================
// Shape
// =========================

Shape::Shape(float x, float y, float xSize, float ySize)
{
    this->x = x;
    this->y = y;

    this->xSize = xSize;
    this->ySize = ySize;

    // Generate a random fill color.
    fillR = rand() % 256;
    fillG = rand() % 256;
    fillB = rand() % 256;

    // Generate a random border color.
    borderR = rand() % 256;
    borderG = rand() % 256;
    borderB = rand() % 256;
}

void Shape::setColor(unsigned char r, unsigned char g, unsigned char b)
{
    fillR = borderR = r;
    fillG = borderG = g;
    fillB = borderB = b;
}

void Shape::updateSize(float xSize, float ySize)
{
    this->xSize = xSize;
    this->ySize = ySize;
}

void Shape::move(float dx, float dy)
{
    x += dx;
    y += dy;
}

// =========================
// Rect
// =========================

Rect::Rect(float x, float y, float xSize, float ySize)
    : Shape(x, y, xSize, ySize)
{
}

void Rect::draw()
{
    // Draw the filled rectangle.
    glColor3ub(fillR, fillG, fillB);

    glBegin(GL_POLYGON);

    glVertex2f(x, y);
    glVertex2f(x, y + ySize);
    glVertex2f(x + xSize, y + ySize);
    glVertex2f(x + xSize, y);

    glEnd();

    // Draw the rectangle border.
    glColor3ub(borderR, borderG, borderB);

    glBegin(GL_LINE_LOOP);

    glVertex2f(x, y);
    glVertex2f(x, y + ySize);
    glVertex2f(x + xSize, y + ySize);
    glVertex2f(x + xSize, y);

    glEnd();
}

bool Rect::contains(float mouseX, float mouseY)
{
    float minX = std::min(x, x + xSize);
    float maxX = std::max(x, x + xSize);
    float minY = std::min(y, y + ySize);
    float maxY = std::max(y, y + ySize);

    return (mouseX >= minX && mouseX <= maxX &&
            mouseY >= minY && mouseY <= maxY);
}

// =========================
// Square
// =========================

Square::Square(float x, float y, float xSize)
    : Shape(x, y, xSize, xSize)
{
}

void Square::draw()
{
    // Draw the filled square.
    glColor3ub(fillR, fillG, fillB);

    glBegin(GL_POLYGON);

    glVertex2f(x, y);
    glVertex2f(x, y + ySize);
    glVertex2f(x + xSize, y + ySize);
    glVertex2f(x + xSize, y);

    glEnd();

    // Draw the square border.
    glColor3ub(borderR, borderG, borderB);

    glBegin(GL_LINE_LOOP);

    glVertex2f(x, y);
    glVertex2f(x, y + ySize);
    glVertex2f(x + xSize, y + ySize);
    glVertex2f(x + xSize, y);

    glEnd();
}

bool Square::contains(float mouseX, float mouseY)
{
    float minX = std::min(x, x + xSize);
    float maxX = std::max(x, x + xSize);
    float minY = std::min(y, y + ySize);
    float maxY = std::max(y, y + ySize);

    return (mouseX >= minX && mouseX <= maxX &&
            mouseY >= minY && mouseY <= maxY);
}

void Square::updateSize(float xSize, float ySize)
{
    // Use the larger movement so both sides remain equal.
    float side = std::max(std::abs(xSize), std::abs(ySize));

    // Preserve the direction in which the square is drawn.
    this->xSize = (xSize < 0) ? -side : side;
    this->ySize = (ySize < 0) ? -side : side;
}

// =========================
// Triangle
// =========================

Triangle::Triangle(float x, float y, float xSize, float ySize)
    : Shape(x, y, xSize, ySize)
{
}

void Triangle::draw()
{
    // Draw the filled triangle.
    glColor3ub(fillR, fillG, fillB);

    glBegin(GL_TRIANGLES);

    // First vertex: bottom-left.
    glVertex2f(x, y);

    // Second vertex: top.
    glVertex2f(x + xSize / 2, y + ySize);

    // Third vertex: bottom-right.
    glVertex2f(x + xSize, y);

    glEnd();

    // Draw the triangle border.
    glColor3ub(borderR, borderG, borderB);

    glBegin(GL_LINE_LOOP);

    glVertex2f(x, y);
    glVertex2f(x + xSize / 2, y + ySize);
    glVertex2f(x + xSize, y);

    glEnd();
}

bool Triangle::contains(float mouseX, float mouseY)
{
    // Use the triangle's bounding rectangle for selection.
    float minX = std::min(x, x + xSize);
    float maxX = std::max(x, x + xSize);
    float minY = std::min(y, y + ySize);
    float maxY = std::max(y, y + ySize);

    return (mouseX >= minX && mouseX <= maxX &&
            mouseY >= minY && mouseY <= maxY);
}

// =========================
// Line
// =========================

Line::Line(float x1, float y1, float x2, float y2)
    : Shape(x1, y1, x2 - x1, y2 - y1)
{
    this->x2 = x2;
    this->y2 = y2;
}

void Line::draw()
{
    // Draw the line using its border color.
    glColor3ub(borderR, borderG, borderB);

    glBegin(GL_LINES);

    glVertex2f(x, y);
    glVertex2f(x + xSize, y + ySize);

    glEnd();
}

bool Line::contains(float mouseX, float mouseY)
{
    // Use the line's bounding rectangle for selection.
    float minX = std::min(x, x + xSize);
    float maxX = std::max(x, x + xSize);
    float minY = std::min(y, y + ySize);
    float maxY = std::max(y, y + ySize);

    return (mouseX >= minX && mouseX <= maxX &&
            mouseY >= minY && mouseY <= maxY);
}
