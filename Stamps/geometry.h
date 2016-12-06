#ifndef GEOMETRY_H
#define GEOMETRY_H


#include <vector>

/**
 * Represent a point on the plane.
 */
class Position;
class Vector;
class Rectangle;

/**
 * Represent a conatiner of the rectangles.
 */
class Rectangles;

class Position
{
private:
    int x_cord, y_cord;

public:
    Position(const int&, const int&);
    Position(int&&, int&&);
    Position &operator= (const Position&);
    bool operator== (const Position&) const;
    int x() const;
    int& x();
    int y() const;
    int& y();
    /**
     * Return the point reflected across y=x axis
     */
    Position reflection() const;
    Position& operator+= (const Vector&);
    Position& operator+= (Vector&&);

    /**
     * Return an immutable Point (0, 0)
     */
    static const Position& origin();
};

class Vector
{
private:
    Position vec_position;

public:
    Vector(const int&, const int&);
    Vector(int&&, int&&);
    Vector& operator= (const Vector&);
    bool operator== (const Vector&) const;
    int x() const;
    int y() const;

    /**
     * Return the vector reflected across y=x axis
     */
    Vector reflection() const;
    Vector& operator+= (const Vector&);
};

class Rectangle
{
private:
    Position bottom_left;
    Vector diagonal_vector;

public:
    /**
     * @param width of the rectangle (non-negative)
     * @param height of the rectangle (non-negative)
     */
    Rectangle(int, int);

    /**
     * @param width of the rectangle
     * @param height of the rectangle
     * @param position of the left bottom vertex
     */
    Rectangle(int, int, const Position&);
    Rectangle(int, int, Position&&);
    Rectangle& operator= (const Rectangle&);
    bool operator== (const Rectangle&) const;
    int width() const;
    int height() const;
    Position pos() const;

    /**
     * Return the rectangle reflected across y=x axis.
     */
    Rectangle reflection() const;

    /**
     * Return the rectangle moved by a given vector.
     */
    Rectangle& operator+= (const Vector&);
    Rectangle& operator+= (Vector&&);
    int area() const;

    /**
     * Perform a split of the rectangle along the horizontal axis.
     * @param place - level of the split. Place >= 0 and place <= rectangle height.
     * @return pair of the rectangles after the split
     */
    std::pair<Rectangle, Rectangle> split_horizontally(int) const;

    /**
     * Perform a split of the rectangle along the vertical axis.
     * @param place - place of the split. Place >= 0 and place <= rectangle width.
     * @return pair of the rectangles after the split.
     */
    std::pair<Rectangle, Rectangle> split_vertically(int) const;
};

class Rectangles
{
private:
    std::vector<Rectangle> vector_of_rectangles;
    std::vector<Rectangle> split_rectangles(unsigned int, int, int);

public:
    /**
     * Container of the rectangles initialized with the list.
     */
    Rectangles(const std::initializer_list<Rectangle>&);

    /**
     * Empty rectangles container.
     */
    Rectangles();
    Rectangles& operator= (const Rectangles&);
    const Rectangle& operator[](unsigned int) const;
    Rectangle& operator[](unsigned int);

    /**
     * @return number of the rectangles in the container
     */
    unsigned int size() const;
    bool operator== (const Rectangles&) const;
    Rectangles& operator+= (const Vector&);
    Rectangles& operator+= (Vector&&);

    /**
     * Perform the vertical split.
     * @param idx - number of the splitted rectangle
     * @param place - place of the split
     * Replace the rectangle with the pair after the split in its position in the contatiner.
     */
    void split_vertically(unsigned int, int);

    /**
     * Perform the horizontal split.
     * @param idx - number of the splitted rectangle
     * @param place - level of the split
     * Replace the rectangle with the pair after the split in its position in the contatiner.
     */
    void split_horizontally(unsigned int, int);
};

Position operator+ (const Position&, const Vector&);
Position&& operator+ (Position&&, const Vector&);
Position&& operator+ (const Vector&, Position&&);

Position operator+ (const Vector&, const Position&);

Vector operator+ (const Vector&, const Vector&);
Vector&& operator+ (Vector&&, const Vector&);
Vector&& operator+ (const Vector&, Vector&&);
Vector&& operator+ (Vector&&, Vector&&);

Rectangle operator+ (const Rectangle&, const Vector&);
Rectangle&& operator+ (Rectangle&&, const Vector&);
Rectangle operator+ (const Vector&, const Rectangle&);
Rectangle&& operator+ (const Vector&, Rectangle&&);

Rectangles operator+ (const Rectangles&, const Vector&);
Rectangles&& operator+ (Rectangles&&, const Vector&);
Rectangles operator+ (const Vector&, const Rectangles&);
Rectangles&& operator+ (const Vector&, Rectangles&&);

/**
 * Merge two rectangles into one.
 * Assume that the first lies below the second, so that they are
 * adjacent.
 * @return merged rectangle
 */
Rectangle merge_horizontally(const Rectangle&, const Rectangle&);

/**
 * Merge two rectangles into one.
 * Assume that the first lies before the second, so that they are
 * adjacent.
 * @return merged rectangle
 */
Rectangle merge_vertically(const Rectangle&, const Rectangle&);

#endif /* GEOMETRY_H */
