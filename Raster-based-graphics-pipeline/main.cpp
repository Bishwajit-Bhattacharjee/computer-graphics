//
// Created by bishwajit on ১৭/৫/২১.
//

#include "point.h"
#include "matrix.h"
#include "helper.h"

#include <bits/stdc++.h>
using namespace std;

int main(){
    Point p = Point(1, 3, 1);
    cout << normalize(2 * p) << endl;
    Matrix mat = get_identity(4);
    cout << mat * p << endl;

    cout << mat * get_identity(4) << endl;
}