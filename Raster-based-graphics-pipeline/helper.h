//
// Created by bishwajit on ১৭/৫/২১.
//

#ifndef RASTER_BASED_GRAPHICS_PIPELINE_HELPER_H
#define RASTER_BASED_GRAPHICS_PIPELINE_HELPER_H
#include "point.h"
#include "matrix.h"


double dot(Point &a, Point &b);
double norm(Point p);
Point normalize(Point p);
Point cross(Point a, Point b);
Matrix get_identity(int sz);

#endif //RASTER_BASED_GRAPHICS_PIPELINE_HELPER_H
