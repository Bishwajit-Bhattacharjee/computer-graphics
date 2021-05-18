//
// Created by bishwajit on ১৭/৫/২১.
//

#include "point.h"
#include "matrix.h"
#include "helper.h"

#include <bits/stdc++.h>
using namespace std;

Point eye, look, up;
double fovy, aspectRatio, near, far;
Matrix cur = get_identity(SZ);

void read_a_point(ifstream &in, Point &p){
    in >> p.x >> p.y >> p.z;
}

void process_a_triangle(istream &in, ostream &out) {
    Point a, b, c;
    in >> a >> b >> c;
    a = cur * a;
    b = cur * b;
    c = cur * c;
    out << a << "\n" << b << "\n" << c << "\n";
    out << "\n";
}

void stage1(){
    stack<Matrix> st;
    ifstream in;
    in.open("resources/1/scene.txt");

    ofstream out;
    out.open("out1/stage1.txt");

    // read four lines
    in >> eye >> look >> up;
    in >> fovy >> aspectRatio >> near >> far;

    string cmd;
    while (in >> cmd){
        if (cmd == "triangle"){
            process_a_triangle(in, out);
        }
        else if (cmd == "translate"){
            Point tr;
            in >> tr;
            Matrix translate = create_translation_matrix(tr);
            cur = cur * translate;
        }
        else if (cmd == "scale"){
            Point sc;
            in >> sc;
            Matrix scale = create_scaling_matrix(sc);
            cur = cur * scale;
        }
        else if (cmd == "rotate"){
            double angle; // in degree
            Point axis;
            in >> angle >> axis;
            Matrix rotate = create_rotation_matrix(angle, axis);
            cur = cur * rotate;
        }
        else if (cmd == "push"){
            st.push(cur);
        }
        else if (cmd == "pop"){
            cur = st.top();
            st.pop();
        }
        else if (cmd == "end"){
            break;
        }
        else {
            cout << "Unknown Operation" << endl;
            break;
        }
    }
    in.close();
    out.close();
}


Matrix create_view_transformation_matrix(){
    Point l = look - eye;
    l = normalize(l);
    Point r = cross(l, up);
    r = normalize(r);
    Point u = cross(r, l);

    Matrix T = get_identity(SZ);
    T.mat[0][3] = -eye.x;
    T.mat[1][3] = -eye.y;
    T.mat[2][3] = -eye.z;


    Matrix R = get_identity(SZ);

    R.mat[0][0] = r.x;
    R.mat[1][0] = r.y;
    R.mat[2][0] = r.z;

    R.mat[0][1] = u.x;
    R.mat[1][1] = u.y;
    R.mat[2][1] = u.z;

    R.mat[0][2] = -l.x;
    R.mat[1][2] = -l.y;
    R.mat[2][2] = -l.z;
    return R * T;
}

void stage2(){
    Matrix V = create_view_transformation_matrix();
    ifstream in;
    ofstream out;

    in.open("out1/stage1.txt");
    out.open("out1/stage2.txt");

    Point a, b, c;

    while (in >> a >> b >> c){
        out << V * a << "\n";
        out << V * b << "\n";
        out << V * c << "\n";
        out << "\n";
    }
    in.close();
    out.close();
}

int main(){
    stage1();
    stage2();
    cout << "process ended" << endl;
}
