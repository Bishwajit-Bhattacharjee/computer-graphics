//
// Created by bishwajit on ১৭/৫/২১.
//

#include "matrix.h"
#include <bits/stdc++.h>
using namespace std;

Matrix::Matrix() {
    this->row = this->col = SZ;
    mat.assign(SZ, vector<double> (SZ, 0));
}
Matrix::Matrix(int row, int col) {
    this->row = row, this->col = col;
    mat.assign(row, vector<double> (col, 0));
}

Matrix &Matrix::operator+=(double tr) {
    for (int r = 0; r < row; r++){
        for (int c = 0; c < col; c++){
            mat[r][c] += tr;
        }
    }
    return *this;
}

Matrix &Matrix::operator*=(double tr) {
    for (int r = 0; r < row; r++){
        for (int c = 0; c < col; c++){
            mat[r][c] *= tr;
        }
    }
    return *this;
}

Matrix Matrix::operator*(const Matrix &rhs) {
    assert(col == rhs.row);
    Matrix tmp(row, rhs.col);

    for (int r = 0; r < tmp.row; r++){
        for (int c = 0; c < tmp.col; c++){
            for (int k = 0; k < col; k++){
                tmp.mat[r][c] += mat[r][k] * rhs.mat[k][c];
            }
        }
    }
    return tmp;
}

Point Matrix::operator*(const Point &p) {
    Matrix ret = (*this) * Matrix(p);
    assert(ret.row == SZ && ret.col == 1);

    ret *= (1.0/ret.mat[3][0]);

    return {ret.mat[0][0], ret.mat[1][0], ret.mat[2][0]};
}


Matrix::Matrix(const Point &p) {
    row = SZ, col = 1;
    mat.assign(SZ, vector<double>(1, 0));

    mat[0][0] = p.x;
    mat[1][0] = p.y;
    mat[2][0] = p.z;
    mat[3][0] = 1;
}

ostream &operator<<(ostream &stream, const Matrix &rhs) {
    stream <<fixed << setprecision(7) << "Mat(" << rhs.row << ", " << rhs.col << ")" << endl;
    for (int r = 0; r < rhs.row; r++){
        stream << "[";
        for (int c = 0; c < rhs.col; c++){
            stream << " " << rhs.mat[r][c];
        }
        stream << " ]";
        if (r + 1 != rhs.row) stream << endl;
    }
    stream << endl;
    return stream;
}
