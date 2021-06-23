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


double dot(Point &a, Point &b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double norm(Point p){
    return sqrt(dot(p, p));
}

Point normalize(Point p){
    return p / norm(p);
}

Point cross(Point a, Point b) {
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

Matrix get_identity(int sz) {
    Matrix ret = Matrix(SZ, SZ);
    for (int r = 0; r < SZ; r++)
        ret.mat[r][r] = 1;
    return ret;
}

Matrix create_scaling_matrix(Point p) {
    Matrix m = get_identity(SZ);
    m.mat[0][0] = p.x;
    m.mat[1][1] = p.y;
    m.mat[2][2] = p.z;
    return m;
}

Matrix create_translation_matrix(Point p) {
    Matrix m = get_identity(SZ);
    m.mat[0][3] = p.x;
    m.mat[1][3] = p.y;
    m.mat[2][3] = p.z;
    return m;
}
/**
 *
 * @param axis -> rotating axis
 * @param rotatingVector -> the vector that needs to rotate
 * @param angle -> in degrees
 * @return
 */
Point rodriguesFormula(Point rotatingVector, Point axis, double angle){
    angle = PI / 180.0 * angle;
    return rotatingVector * cos(angle) +
           cross(axis, rotatingVector) * sin(angle) +
           axis * dot(axis, rotatingVector) * (1 - cos(angle));
}

// angle is in degree
Matrix create_rotation_matrix(double angle, Point axis) {
    Point i = Point(1, 0, 0);
    Point j = Point(0, 1, 0);
    Point k = Point(0, 0, 1);
    axis = normalize(axis);
    Point c1 = rodriguesFormula(i, axis, angle);
    Point c2 = rodriguesFormula(j, axis, angle);
    Point c3 = rodriguesFormula(k, axis, angle);

    Matrix m = get_identity(SZ);
    m.mat[0][0]= c1.x;
    m.mat[1][0]= c1.y;
    m.mat[2][0]= c1.z;

    m.mat[0][1]= c2.x;
    m.mat[1][1]= c2.y;
    m.mat[2][1]= c2.z;

    m.mat[0][2]= c3.x;
    m.mat[1][2]= c3.y;
    m.mat[2][2]= c3.z;

    return m;
}
#endif //RASTER_BASED_GRAPHICS_PIPELINE_HELPER_H
