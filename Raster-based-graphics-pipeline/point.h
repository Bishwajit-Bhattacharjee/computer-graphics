//
// Created by bishwajit on ১৭/৫/২১.
//

#ifndef RASTER_BASED_GRAPHICS_PIPELINE_POINT_H
#define RASTER_BASED_GRAPHICS_PIPELINE_POINT_H

#include <bits/stdc++.h>

using namespace std;

struct Point {
    double x, y, z;
    Point(double x, double y, double z);
    Point();

    Point& operator+=(const Point &p);
    Point& operator-=(const Point &p);
    Point& operator*=(double s);
    Point& operator/=(double s);

    Point operator+(const Point& p)const;
    Point operator-(const Point& p)const;
    Point operator*(double t)const;
    Point operator/(double t)const;

    friend Point operator*(double t, Point& p);
    friend ostream& operator<<(ostream &os, const Point& p);
};

#endif //RASTER_BASED_GRAPHICS_PIPELINE_POINT_H
