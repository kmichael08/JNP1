#include <cassert>
#include "geometry.h"

Position::Position(const int &x, const int &y) : x_cord(x), y_cord(y) {}

Position::Position(int &&x, int &&y) : x_cord(std::move(x)), y_cord(std::move(y)) {}

Position& Position::operator= (const Position& pos) {
    if (&pos != this) {
        this->x_cord = pos.x();
        this->y_cord = pos.y();
    }
    return *this;
}

bool Position::operator== (Position const & pos) const {
    return this->x() == pos.x() && this->y() == pos.y();
}

int Position::x() const {
    return this->x_cord;
}

int &Position::x() {
    return this->x_cord;
}

int Position::y() const {
    return this->y_cord;
}

int &Position::y() {
    return this->y_cord;
}

Position Position::reflection() const {
    return Position(this->y(), this->x());
}

Position& Position::operator+= (Vector const & vec) {
    this->x_cord += vec.x();
    this->y_cord += vec.y();
    return *this;
}

Position &Position::operator+=(Vector && vec) {
    this->x_cord += std::move(vec.x());
    this->y_cord += std::move(vec.y());
    return *this;
}

const Position& Position::origin() {
    static const Position origin_pos = Position(0, 0);
    return origin_pos;
}

Vector::Vector(const int &x, const int &y) : vec_position(x, y) {}

Vector::Vector(int &&x, int &&y) : vec_position(std::move(x), std::move(y)) {}

Vector& Vector::operator= (const Vector& vec) {
    if (&vec != this) {
        this->vec_position = Position(vec.x(), vec.y());
    }
    return *this;
}

bool Vector::operator== (const Vector& vec) const {
    return this->vec_position == vec.vec_position;
}

int Vector::x() const {
    return this->vec_position.x();
}

int Vector::y() const {
    return this->vec_position.y();
}

Vector Vector::reflection() const {
    Position do_reflection = this->vec_position.reflection();
    return Vector(do_reflection.x(), do_reflection.y());
}

Vector& Vector::operator+= (const Vector& vec) {
    this->vec_position += vec;
    return *this;
}

Rectangle::Rectangle(int x, int y) : bottom_left(0, 0), diagonal_vector(x, y) {  assert(x >= 0 && y >= 0); }

Rectangle::Rectangle(int x, int y, Position const & pos) : bottom_left(std::move(pos.x()), std::move(pos.y())), diagonal_vector(x, y) { assert(x >= 0 && y >= 0); }

Rectangle::Rectangle(int x, int y, Position && pos) : bottom_left(std::move(pos.x()), std::move(pos.y())), diagonal_vector(x, y) { assert (x >=0 && y >= 0); }

Rectangle& Rectangle::operator= (const Rectangle& rec) {
    if (&rec != this) {
        this->bottom_left = Position(rec.pos());
        this->diagonal_vector = Vector(rec.width(), rec.height());
    }
    return *this;
}

bool Rectangle::operator== (Rectangle const & rec) const {
    return this->bottom_left == rec.bottom_left && this->diagonal_vector == rec.diagonal_vector;
}

int Rectangle::width() const {
    return this->diagonal_vector.x();
}

int Rectangle::height() const {
    return this->diagonal_vector.y();
}

Position Rectangle::pos() const {
    return this->bottom_left;
}

Rectangle Rectangle::reflection() const {
    Position ref_position = this->bottom_left.reflection();
    Vector ref_vector = this->diagonal_vector.reflection();
    return Rectangle(ref_vector.x(), ref_vector.y(), ref_position);
}

Rectangle& Rectangle::operator+= (const Vector& vec) {
    this->bottom_left += vec;
    return *this;
}

Rectangle& Rectangle::operator+= (Vector && vec) {
    this->bottom_left += std::move(vec);
    return *this;
}

int Rectangle::area() const {
    return this->width() * this->height();
}

std::pair<Rectangle, Rectangle> Rectangle::split_vertically(int place) const {
    assert(width() >= place);
    assert(place >= 0);

    Vector rec1_vec = Vector(place, this->diagonal_vector.y());
    Rectangle rec1(rec1_vec.x(), rec1_vec.y(), this->bottom_left);

    Position rec2_bottom_left = Position(this->bottom_left.x() + place, this->bottom_left.y());
    Vector rec2_vec = Vector(this->diagonal_vector.x() - place, this->diagonal_vector.y());
    Rectangle rec2(rec2_vec.x(), rec2_vec.y(), rec2_bottom_left);
    return std::make_pair(rec1, rec2);
}

std::pair<Rectangle, Rectangle> Rectangle::split_horizontally(int place) const {
    auto rectangle_pair = this->reflection().split_vertically(place);
    return std::make_pair(rectangle_pair.first.reflection(), rectangle_pair.second.reflection());
}

Rectangles::Rectangles(const std::initializer_list<Rectangle>& list_of_rectangles) {
    this->vector_of_rectangles = std::vector<Rectangle>(list_of_rectangles);
}

Rectangles::Rectangles() {
    this->vector_of_rectangles = std::vector<Rectangle>();
}

Rectangles& Rectangles::operator= (const Rectangles& recs) {
    if (&recs != this) {
        this->vector_of_rectangles.clear();
        for (unsigned int i = 0; i < recs.size(); ++i)
            this->vector_of_rectangles.push_back(recs[i]);
    }
    return *this;
}

const Rectangle& Rectangles::operator[](unsigned int i) const {
    return this->vector_of_rectangles[i];
}

Rectangle& Rectangles::operator[](unsigned int i) {
    assert(i >= 0);
    assert(i < vector_of_rectangles.size());

    return this->vector_of_rectangles[i];
}

unsigned int Rectangles::size() const {
    return (unsigned int) this->vector_of_rectangles.size();
}

bool Rectangles::operator== (const Rectangles& recs) const {
    if (this->vector_of_rectangles.size() != recs.vector_of_rectangles.size())
        return false;
    for (unsigned int i = 0; i < this->vector_of_rectangles.size(); ++i) {
        if (!(this->vector_of_rectangles[i] == recs.vector_of_rectangles[i]))
            return false;
    }
    return true;
}

Rectangles& Rectangles::operator+= (const Vector& vec) {
    for (unsigned int i = 0; i < this->vector_of_rectangles.size(); ++i)
        this->vector_of_rectangles[i] += vec;
    return *this;
}

Rectangles& Rectangles::operator+= (Vector && vec) {
    for (unsigned int i = 0; i < this->vector_of_rectangles.size(); ++i)
        this->vector_of_rectangles[i] += std::move(vec);
    return *this;
}

std::vector<Rectangle> Rectangles::split_rectangles(unsigned int idx, int place, int how) {
    assert (idx >= 0 && idx < vector_of_rectangles.size());

    std::vector<Rectangle> result = std::vector<Rectangle>();
    for (unsigned int i = 0; i < vector_of_rectangles.size(); ++i) {
        if (i != idx)
            result.push_back(vector_of_rectangles[i]);
        else {
            std::pair<Rectangle, Rectangle> new_rectangles = vector_of_rectangles[i].split_horizontally(place);
            if (!how)
                new_rectangles = vector_of_rectangles[i].split_vertically(place);
            result.push_back(new_rectangles.first);
            result.push_back(new_rectangles.second);
        }
    }
    return result;
}

void Rectangles::split_vertically(unsigned int idx, int place) {
    std::vector<Rectangle> new_vector_of_rectangles = split_rectangles(idx, place, 0);
    this->vector_of_rectangles = new_vector_of_rectangles;
}

void Rectangles::split_horizontally(unsigned int idx, int place) {
    std::vector<Rectangle> new_vector_of_rectangles = split_rectangles(idx, place, 1);
    this->vector_of_rectangles = new_vector_of_rectangles;
}

Position operator+ (const Position& pos, const Vector& vec) {
    return Position(pos.x()+vec.x(), pos.y()+vec.y());
}

Position&& operator +(Position&& pos, const Vector& vec) {
    return std::move(pos += vec);
}

Position operator+ (const Vector& vec, const Position& pos) {
    return pos + vec;
}

Position&& operator+ (const Vector& vec, Position&& pos) {
    return std::move(pos += vec);
}

Vector operator+ (const Vector& vec1, const Vector& vec2) {
    return Vector(vec1.x()+vec2.x(), vec1.y()+vec2.y());
}

Vector&& operator+ (Vector&& vec1, const Vector& vec2) {
    return std::move(vec1 += vec2);
}

Vector&& operator+ (const Vector& vec1, Vector&& vec2) {
    return std::move(vec2 += vec1);
}

Vector&& operator+ (Vector&& vec1, Vector&& vec2) {
    return std::move(vec1 += vec2);
}

Rectangle operator+ (const Rectangle& rec, const Vector& vec) {
    return Rectangle(rec.width(), rec.height(), rec.pos()+vec);
}

Rectangle&& operator+ (Rectangle&& rec, const Vector& vec) {
    return std::move(rec += vec);
}

Rectangle operator+ (const Vector& vec, const Rectangle& rec) {
    return rec + vec;
}

Rectangle&& operator+ (const Vector& vec, Rectangle&& rec) {
    return std::move(rec += vec);
}

Rectangles operator+ (const Rectangles& recs, const Vector& vec) {
    Rectangles result_recs = recs;
    for (unsigned int i = 0; i < recs.size(); ++i)
        result_recs[i] += vec;
    return result_recs;
}

Rectangles&& operator+ (Rectangles&& recs, const Vector& vec) {
    return std::move(recs += vec);
}

Rectangles operator+ (const Vector& vec, const Rectangles& recs) {
    return recs + vec;
}

Rectangles&& operator+ (const Vector& vec, Rectangles&& recs) {
    return std::move(recs += vec);
}

Rectangle merge_horizontally(const Rectangle& rec1, const Rectangle& rec2) {
    assert(rec2.pos().y() == rec1.pos().y() + rec1.height());
    assert(rec1.width() == rec2.width());
    assert(rec1.pos().x() == rec2.pos().x());

    return Rectangle(rec1.width(), rec1.height()+rec2.height(), rec1.pos());
}

Rectangle merge_vertically(const Rectangle& rec1, const Rectangle& rec2) {
    Rectangle refl_result = merge_horizontally(rec1.reflection(), rec2.reflection());
    return refl_result.reflection();
}
