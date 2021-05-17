//
// Created by bishwajit on ১৭/৫/২১.
//
#include "point.h"
#include "helper.h"

#include <bits/stdc++.h>
using namespace std;

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

