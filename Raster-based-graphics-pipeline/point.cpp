//
// Created by bishwajit on ১৭/৫/২১.
//
#include "point.h"

#include <bits/stdc++.h>
using namespace std;

Point::Point(){}
Point::Point(double x, double y, double z):x(x), y(y), z(z){}

Point& Point::operator+=(const Point &p) {
    x += p.x;
    y += p.y;
    z += p.z;
    return *this;
}
Point& Point::operator-=(const Point &p) {
    x -= p.x;
    y -= p.y;
    z -= p.z;
    return *this;
}
Point& Point::operator*=(const double s) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
}
Point& Point::operator/=(const double s) {
    x /= s;
    y /= s;
    z /= s;
    return *this;
}
Point Point::operator+(const Point& p) const {
    return Point(*this) += p;
}

Point Point::operator-(const Point& p) const {
    return Point(*this) -= p;
}

Point Point::operator*(const double t) const {
    return Point(*this) *= t;
}

Point Point::operator/(const double t) const {
    return Point(*this) /= t;
}

ostream& operator<<(ostream &os, const Point &p) {
    return os << fixed << setprecision(7) << p.x << " " << p.y <<" " << p.z;
}

Point operator*(double t, Point &p) {
    return p * t;
}

istream &operator>>(istream &is,Point &p) {
    double x, y, z;
    if (is >> x >> y >> z){
        p.x = x, p.y = y, p.z = z;
    }
    return is;
}

