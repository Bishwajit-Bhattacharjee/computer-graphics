//
// Created by bishwajit on ১৭/৫/২১.
//

#ifndef RASTER_BASED_GRAPHICS_PIPELINE_HELPER_H
#define RASTER_BASED_GRAPHICS_PIPELINE_HELPER_H
#include "point.h"
#include "matrix.h"
double const PI = 2 * acos(0.0);

double dot(Point &a, Point &b);
double norm(Point p);
Point normalize(Point p);
Point cross(Point a, Point b);
Matrix get_identity(int sz);
Matrix create_translation_matrix(Point p);
Matrix create_scaling_matrix(Point p);
Matrix create_rotation_matrix(double angle, Point axis);
Point rodriguesFormula(Point rotatingVector, Point axis, double angle);
#endif //RASTER_BASED_GRAPHICS_PIPELINE_HELPER_H
