//
// Created by bishwajit on ২২/৫/২১.
//

#ifndef RASTER_BASED_GRAPHICS_PIPELINE_Z_BUFFER_H
#include "point.h"
#include "matrix.h"
#include "bitmap_image.hpp"
#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;


int screen_width, screen_height;
double z_min, z_max;
double y_min, y_max;
double x_min, x_max;

double dx, dy, Left_X, Top_Y;

struct Triangle {
    Point points[3];
    int color[3];

    void setColor(int r, int g, int b){
        color[0] = r, color[1] = g, color[2] = b;
    }

    friend istream& operator>>(istream &is, Triangle& tr);
    friend ostream& operator<<(ostream &os, const Triangle& tr);
};

istream &operator>>(istream &is,Triangle &tr) {
    Point x, y, z;
    if (is >> x >> y >> z){
        tr.points[0] = x, tr.points[1] = y, tr.points[2] = z;
    }
    return is;
}

ostream& operator<<(ostream &os, const Triangle &tr) {
    os << fixed << setprecision(7) << tr.points[0] << "\n" << tr.points[1] << "\n" << tr.points[2] << "\n";
    return os << "color : (" << tr.color[0] << " " << tr.color[1] << " " << tr.color[2] << " )" << endl;
}


vector<Triangle> triangles;

void read_config(string filename){
    ifstream in;
    in.open(filename);
    if (!in.is_open()) {
        cout << "File " << filename << " not found!" << endl;
        exit(1);
    }
    in >> screen_width >> screen_height;
    in >> x_min;
    x_max = -x_min;
    in >> y_min;
    y_max = -y_min;
    in >> z_min >> z_max;
}

void read_triangle(string filename){
    ifstream in;
    in.open(filename);
    if (!in.is_open()) {
        cout << "File " << filename << " not found!" << endl;
        exit(1);
    }

    Triangle tr;
    while (in >> tr){
        tr.setColor(rand() % 266, rand() % 266, rand() % 266);
        triangles.push_back(tr);
    }
}

double max(double x, double y, double z){
    return max(x, max(y, z));
}
double min(double x, double y, double z){
    return min(x, min(y, z));
}

int y_axis_to_grid(double y){
    return (int)round((Top_Y - y)/dy + EPS);
}

int x_axis_to_grid(double x){
    return (int)round((x - Left_X)/dx + EPS);
}

pair<int, int> find_y_range(const Triangle& tr){

    double mn = min(tr.points[0].y, tr.points[1].y, tr.points[2].y);
    double mx = max(tr.points[0].y, tr.points[1].y, tr.points[2].y);

    mx = clamp(mx, y_min, y_max), mn = clamp(mn, y_min, y_max);
    return {clamp(y_axis_to_grid(mx), 0, screen_height-1), clamp(y_axis_to_grid(mn),0, screen_height-1)};
}

/**
 *
 * @param tr
 * @param y is the coordinate(not pixel) of current y scan line
 * @return {x_left, x_right}
 */
pair<int,int> find_x_range(const Triangle& tr, double y){

}


void z_buffer_algorithm(){
    bitmap_image frame_buffer = bitmap_image(screen_width, screen_height);
    vector<vector<double>> z_buffer(screen_width, vector<double> (screen_height, z_max));

    for (int x = 0; x < screen_width; x++){
        for (int y = 0; y < screen_height; y++){
            frame_buffer.set_pixel(x, y, 0, 0, 0);  // background BLACK
        }
    }
    dx = (x_max - x_min)/ screen_width;
    dy = (y_max - y_min)/ screen_height;

    Top_Y = y_max - dy/2;
    Left_X = x_min + dx/2;


    for (Triangle triangle: triangles){
        auto [y_scan_top, y_scan_bottom] = find_y_range(triangle);
        int yp;
        double y_cord = Top_Y - dy * y_scan_top;
        for (yp = y_scan_top; yp >= y_scan_bottom; yp--, y_cord -= dy){
            auto [x_scan_left, x_scan_right] = find_x_range(triangle, y_cord);

        }
    }

    frame_buffer.save_image("myout.bmp");
}



#define RASTER_BASED_GRAPHICS_PIPELINE_Z_BUFFER_H

#endif //RASTER_BASED_GRAPHICS_PIPELINE_Z_BUFFER_H
