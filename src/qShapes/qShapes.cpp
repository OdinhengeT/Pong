#include <cmath>

#include "qShapes.hpp"

float fastRSqrt(float number) {
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

using namespace qShapes;

qPoint2D::qPoint2D(float x, float y): x{x}, y{y} {
}

int qPoint2D::getIntX() {
    return (int) this->x;
}

int qPoint2D::getIntY() {
    return (int) this->y;
}

qPoint2D qPoint2D::normalize() {
    *this *= fastRSqrt(this->x * this->x + this->y * this->y);
    return *this;
}

qPoint2D qPoint2D::operator*=(const float& factor) {
    this->x *= factor;
    this->y *= factor;
    return *this;
}

qPoint2D qPoint2D::operator/=(const float& factor) {
    this->x /= factor;
    this->y /= factor;
    return *this;
}

qPoint2D qPoint2D::operator+=(const qPoint2D& qP2) {
    this->x += qP2.x;
    this->y += qP2.y;
    return *this;
}

qPoint2D qPoint2D::operator-=(const qPoint2D& qP2) {
    this->x -= qP2.x;
    this->y -= qP2.y;
    return *this;
}

qPoint2D operator*(const qPoint2D& qP2, const float& factor) {
    return qPoint2D(qP2.x * factor, qP2.y * factor);
}

qPoint2D operator*(const float& factor, const qPoint2D& qP2) {
    return qPoint2D(qP2.x * factor, qP2.y * factor);
}

qPoint2D operator/(const qPoint2D& qP2, const float& factor) {
    return qPoint2D(qP2.x / factor, qP2.y / factor);
}

qPoint2D operator+(const qPoint2D& qP2_1, const qPoint2D& qP2_2) {
    return qPoint2D(qP2_1.x + qP2_2.x, qP2_1.y + qP2_2.y);
}

qPoint2D operator-(const qPoint2D& qP2_1, const qPoint2D& qP2_2) {
    return qPoint2D(qP2_1.x - qP2_2.x, qP2_1.y - qP2_2.y);
}

float qPoint2D::dot(const qPoint2D& qP2) const {
    return this->x * qP2.x + this->y * qP2.y;
}

qPoint2D qPoint2D::rotate(const qPoint2D& qP2_rot, const float& angle) {
    this->x = qP2_rot.x + (this->x - qP2_rot.x) * cosf(angle) - (this->y - qP2_rot.y) * sinf(angle);
    this->y = qP2_rot.y + (this->x - qP2_rot.x) * sinf(angle) + (this->y - qP2_rot.y) * cosf(angle);
    return *this;
}


qPoint2D qPoint2D::projectOnto(const qPoint2D& qP2_proj) {    
    float factor = this->dot(qP2_proj) / qP2_proj.dot(qP2_proj);

    this->x = qP2_proj.x * factor;
    this->y = qP2_proj.y * factor;
    return *this;
}

float qP2_dot(const qPoint2D& qP2_1, const qPoint2D& qP2_2) {
    return qP2_1.x * qP2_2.x + qP2_1.y * qP2_2.y;
}

qPoint2D qP2_projectOnto(const qPoint2D& qP2, const qPoint2D& qP2_proj) {
    float factor = qP2.dot(qP2_proj) / qP2_proj.dot(qP2_proj);
    return qPoint2D(factor * qP2_proj.x, factor * qP2_proj.y);
}

qPoint2D qP2_rotate(const qPoint2D& qP2, const qPoint2D& qP2_rot, const float& angle) {
    float x = qP2_rot.x + (qP2.x - qP2_rot.x) * cosf(angle) - (qP2.y - qP2_rot.y) * sinf(angle);
    float y = qP2_rot.y + (qP2.x - qP2_rot.x) * sinf(angle) + (qP2.y - qP2_rot.y) * cosf(angle);
    return qPoint2D(x, y);
}

qLine2D::qLine2D(qPoint2D vert_1, qPoint2D vert_2): vert_1{vert_1}, vert_2{vert_2} {
}

qLine2D::qLine2D(float x1, float y1, float x2, float y2): vert_1{x1, y1}, vert_2{x2, y2} {
}

qTriangle2D::qTriangle2D(qPoint2D vert_1, qPoint2D vert_2, qPoint2D vert_3):
    vert_1{vert_1}, vert_2{vert_2}, vert_3{vert_3} {
}

qRectangle2D::qRectangle2D(qPoint2D vert_1, float width, float height): 
    vert_1{vert_1}, width{width}, height{height} {
}
        
qRectangle2D::qRectangle2D(float x0, float y0, float width, float height):
    vert_1{x0, y0}, width{width}, height{height} {
}

void qRectangle2D::translate(qPoint2D translation_vector) {
    this->vert_1 += translation_vector;
}