//
// Created by bishwajit on ২৩/৬/২১.
//

#ifndef RAY_TRACING_1605003_LIGHT_H
#include "1605003_Objects.h"

struct Light {
    Point light_pos;
    Color color;
    Light(){};
    Light(Point pos, Color col):light_pos(pos),color(col){}

    friend istream& operator>>(istream &is, Light& l);
    friend ostream& operator<<(ostream &os, Light& l);
};


istream &operator>>(istream& is, Light &l){
    is >> l.light_pos >> l.color;
    return is;
}

ostream &operator<<(ostream & os, Light &l){
    os << "light eye " << l.light_pos << endl;
    os << "color " << l.color << endl;
    return os;
}
#define RAY_TRACING_1605003_LIGHT_H

#endif //RAY_TRACING_1605003_LIGHT_H
