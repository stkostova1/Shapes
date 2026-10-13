#ifndef FORM_H
#define FORM_H



class Shape
{
protected:
    float x, y;
    float xSize, ySize;

    // Fill color
    unsigned char fillR, fillG, fillB;

    // Border color
    unsigned char borderR, borderG, borderB;

public:
    Shape(float x, float y, float xSize, float ySize);

void move(float dx, float dy);
    void setColor(unsigned char r,unsigned char g,unsigned char b);
    virtual void draw() = 0;
    virtual bool contains(float mouseX, float mouseY) = 0;
    virtual void updateSize(float xSize, float ySize);

    virtual ~Shape() = default;
};


// =========================
// Rect
// =========================

class Rect : public Shape
{
public:
    Rect(float x, float y, float xSize, float ySize);

    void draw() override;
    bool contains(float mouseX, float mouseY) override;
};


// =========================
// Square
// =========================

class Square : public Shape
{
public:
    Square(float x, float y, float xSize);

    void draw() override;
    bool contains(float mouseX, float mouseY) override;
    void updateSize(float xSize, float ySize) override;
};



// =========================
// Triangle
// =========================

class Triangle : public Shape
{
public:
    Triangle(float x, float y, float xSize, float ySize);

    void draw() override;
    bool contains(float mouseX, float mouseY) override;
};


// =========================
// Line
// =========================

class Line : public Shape
{
private:
    float x2, y2;

public:
    Line(float x1, float y1, float x2, float y2);

    void draw() override;
    bool contains(float mouseX, float mouseY) override;
};


#endif
