//
// Created by bishwajit on ১৭/৫/২১.
//

#ifndef RASTER_BASED_GRAPHICS_PIPELINE_MATRIX_H
#define RASTER_BASED_GRAPHICS_PIPELINE_MATRIX_H
#include "point.h"
#include <bits/stdc++.h>
using namespace std;

const int SZ = 4;

struct Matrix{
    vector<vector<double>> mat;
    int row, col;
    Matrix();
    Matrix(int row, int col);
    Matrix(const Point &p);

    Matrix& operator+=(double tr);
    Matrix& operator*=(double sc);

    Matrix operator*(const Matrix& rhs);
    Point operator*(const Point& p);

    friend ostream& operator<<(ostream& stream, const Matrix& rhs);
};

#endif //RASTER_BASED_GRAPHICS_PIPELINE_MATRIX_H
