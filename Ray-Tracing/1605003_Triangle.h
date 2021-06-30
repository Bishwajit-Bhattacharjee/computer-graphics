//
// Created by bishwajit on ৩০/৬/২১.
//

#ifndef RAY_TRACING_1605003_TRIANGLE_H
#define RAY_TRACING_1605003_TRIANGLE_H
#include "1605003_Objects.h"

struct Triangle:Object {
    Point a, b, c;
    Triangle() = default;

    void draw() override {
        glPushMatrix();
        glColor3f(color.x, color.y, color.z);
        glBegin(GL_TRIANGLES);{
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }glEnd();
        glPopMatrix();
    }

    Color getColorAt(Point &p) override {
        return this->color;
    }

    Point getNormal(Point &p) override {
        return normalize(cross(b-a, c-a));
    }

    double nearestTouch(Ray &r) override {
        return Object::nearestTouch(r);
    }

    istream &input(istream &is) override {
        is >> a >> b >> c;
        is >> color;
        for (double & coEfficient : coEfficients) is >> coEfficient;
        is >> shine;
        return is;
    }

    ostream &output(ostream &os) override {
        os << "Printing Triangle " << endl;
        os << a << b << c;
        os << color;
        for (double & coEfficient : coEfficients) os << coEfficient;
        os << shine;
        os << endl;
        return os;
    }

};

#endif //RAY_TRACING_1605003_TRIANGLE_H
