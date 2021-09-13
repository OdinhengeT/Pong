#ifndef QSHAPES_H
#define QSHAPES_H

#include <vector>

namespace qShapes {

class qPoint2D {
    public:
        float x, y;
        qPoint2D(float x, float y);
        qPoint2D(const qPoint2D&)=default;
        ~qPoint2D()=default;

        int getIntX();
        int getIntY();

        qPoint2D normalize();

        qPoint2D operator*=(const float& factor);
        qPoint2D operator/=(const float& factor);
        qPoint2D operator+=(const qPoint2D& qP2);
        qPoint2D operator-=(const qPoint2D& qP2);
        friend qPoint2D operator*(const qPoint2D& qP2, const float& factor);
        friend qPoint2D operator*(const float& factor, const qPoint2D& qP2);
        friend qPoint2D operator/(const qPoint2D& qP2, const float& factor);
        friend qPoint2D operator+(const qPoint2D& qP2_1, const qPoint2D& qP2_2);
        friend qPoint2D operator-(const qPoint2D& qP2_1, const qPoint2D& qP2_2);

        float dot(const qPoint2D& qP2) const;
        qPoint2D rotate(const qPoint2D& qP2_rot, const float& angle);
        qPoint2D projectOnto(const qPoint2D& qP2_proj);

        friend float qP2_dot(const qPoint2D& qP2_1, const qPoint2D& qP2_2);
        friend qPoint2D qP2_projectOnto(const qPoint2D& qP2, const qPoint2D& qP2_proj);
        friend qPoint2D qP2_rotate(const qPoint2D& q, const qPoint2D& rot_axis, const float& angle);
};

class qLine2D {
    public:
        qPoint2D vert_1, vert_2; 
        qLine2D(qPoint2D vert_1, qPoint2D vert_2);
        qLine2D(float x1, float y1, float x2, float y2);
        qLine2D(const qLine2D&)=default;
        ~qLine2D()=default;
};

class qTriangle2D {
    public:
        qPoint2D vert_1, vert_2, vert_3;
        qTriangle2D(qPoint2D vert_1, qPoint2D vert_2, qPoint2D vert_3);
        qTriangle2D(const qTriangle2D&)=default;
        ~qTriangle2D()=default;
};

class qRectangle2D {
    public:
        qPoint2D vert_1;
        float width, height;
        qRectangle2D(qPoint2D vert_1, float width, float heigth);
        qRectangle2D(float x0, float y0, float width, float heigth);
        qRectangle2D(const qRectangle2D&)=default;
        ~qRectangle2D()=default;

        void translate(qPoint2D translation_vector);
};


/*
struct qPoint3D {
public:
    float x, y, z;
    Q3vector(float x, float y, float z);
    Q3vector(const Q3vector&)=default;
    ~Q3vector()=default;

    Q3vector operator*=(const float& factor);
    Q3vector operator/=(const float& factor);
    Q3vector operator+=(const Q3vector& q);
    Q3vector operator-=(const Q3vector& q);
    Q3vector normalize();
    float dot(const Q3vector& q);
    Q3vector cross(const Q3vector& q);
    Q3vector project_onto(const Q3vector& pro_axis);
    Q3vector rotate(const Q3vector& rot_axis, const float& angle);

    friend Q3vector operator*(const Q3vector& q, const float& factor);
    friend Q3vector operator*(const float& factor, const Q3vector& q);
    friend Q3vector operator/(const Q3vector& q, const float& factor);
    friend Q3vector operator+(const Q3vector& q1, const Q3vector& q2);
    friend Q3vector operator-(const Q3vector& q1, const Q3vector& q2);
    friend Q3vector Q3_normalize(const Q3vector& q);
    friend float Q3_dot(const Q3vector& q1, const Q3vector& q2);
    friend Q3vector Q3_cross(const Q3vector& q1, const Q3vector& q2);
    friend Q3vector Q3_project_onto(const Q3vector& q, const Q3vector& pro_axis);
    friend Q3vector Q3_rotate(const Q3vector& q, const Q3vector& rot_axis, const float& angle);
    friend std::ostream& operator<<(std::ostream&, const Q3vector&);
};
*/

}

#endif