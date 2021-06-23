//
// Created by bishwajit on ২৩/৬/২১.
//

#ifndef RAY_TRACING_1605003_OBJECT_H
#include "1605003_Point.h"

using namespace std;

struct Object {
    Point reference_point;
    double height, width, length;
    Color color;
    double coEfficients[4];
    int shine;
    Object(){}
    virtual void draw();
    virtual istream& input(istream &is);
    virtual ostream& output(ostream &os);

    friend ostream& operator<<(ostream &os, Object& o);
    friend istream& operator>>(istream &is, Object& o);
    ~Object(){}
};

istream& operator>>(istream &is, Object &o){
    o.input(is);
    return is;
}

ostream &operator<<(ostream &os, Object &o) {
    o.output(os);
    return os;
}

istream &Object::input(istream &is) {
    return is;
}

void Object::draw() {

}

ostream &Object::output(ostream &os) {
    return os;
}


#define RAY_TRACING_1605003_OBJECT_H

#endif //RAY_TRACING_1605003_OBJECT_H
