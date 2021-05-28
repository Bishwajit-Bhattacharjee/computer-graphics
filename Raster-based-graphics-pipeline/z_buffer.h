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
        tr.setColor(rand() % 256, rand() % 256, rand() % 256);
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
    int g = (int)round((Top_Y - y)/dy + EPS);
    return clamp(g, 0, screen_height-1);
}

int x_axis_to_grid(double x){
    int g = (int)round((x - Left_X)/dx + EPS);
    return clamp(g, 0, screen_width - 1);
}

pair<int, int> find_y_range(const Triangle& tr){

    double mn = min(tr.points[0].y, tr.points[1].y, tr.points[2].y);
    double mx = max(tr.points[0].y, tr.points[1].y, tr.points[2].y);

    mx = clamp(mx, y_min, y_max), mn = clamp(mn, y_min, y_max);
    return {y_axis_to_grid(mx),y_axis_to_grid(mn)};
}


Point get_intersection_point(Point &p1, Point &p2, double y){
    Point tmp;
    tmp.y = y;
    tmp.z = p1.z + (y - p1.y)/(p2.y - p1.y) * (p2.z - p1.z);
    tmp.x = p1.x + (y - p1.y)/(p2.y - p1.y) * (p2.x - p1.x);
    return tmp;
}


bool contains_in_line_segment(const Point &p1, const Point &p2, const Point &cut) {
    double mn_x = min(p1.x, p2.x), mx_x = max(p1.x, p2.x);
    double mn_y = min(p1.y, p2.y), mx_y = max(p1.y, p2.y);

    return (cut.x + EPS > mn_x  && cut.x < mx_x + EPS && cut.y + EPS > mn_y  && cut.y < mx_y + EPS);
}

/**
 *
 * @param tr
 * @param y is the coordinate(not pixel) of current y scan line
 * @return intersecting points
 */
vector<Point> find_limiting_points(const Triangle& tr, double y){
    vector<Point> intersections;
    for (int i = 0; i < 3; i++){
        auto p1 = tr.points[i], p2 = tr.points[(i+1)%3];
        Point cut = get_intersection_point(p1, p2, y);
        if (contains_in_line_segment(p1, p2, cut)) {
            intersections.push_back(cut);
        }
    }
    sort(intersections.begin(), intersections.end(),
         [](Point a, Point b)->bool {
             return a.x < b.x;
         });
    return intersections;
}


void z_buffer_algorithm(){
    bitmap_image frame_buffer = bitmap_image(screen_width, screen_height);
    vector<vector<double>> z_buffer(screen_width, vector<double> (screen_height, z_max + EPS));

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

        for (yp = y_scan_top; yp <= y_scan_bottom; yp++, y_cord -= dy){

            vector<Point> limiting_points= find_limiting_points(triangle, y_cord);
            if (limiting_points.empty()) continue;

            Point A = limiting_points.front(), B = limiting_points.back();
            double clip_left_x = clamp(A.x, x_min, x_max), clip_right_x = clamp(B.x, x_min, x_max);
            int x_scan_left = x_axis_to_grid(clip_left_x), x_scan_right = x_axis_to_grid(clip_right_x);

            double cur_z = A.z + (clip_left_x - A.x)/(B.x - A.x) * (B.z - A.z);
            double z_delta = dx * (B.z - A.z)/(B.x - A.x);

            for (int xp = x_scan_left; xp <= x_scan_right; xp++, cur_z += z_delta){

                if (z_buffer[xp][yp] > cur_z  && EPS + cur_z > z_min){
                    cout << "update " << xp << " " << yp << " " << cur_z << endl;

                    z_buffer[xp][yp] = cur_z;
                    frame_buffer.set_pixel(xp, yp,
                                           triangle.color[0],
                                           triangle.color[1],
                                           triangle.color[2]);
                }
            }
        }
    }

    frame_buffer.save_image("myout.bmp");
}



#define RASTER_BASED_GRAPHICS_PIPELINE_Z_BUFFER_H

#endif //RASTER_BASED_GRAPHICS_PIPELINE_Z_BUFFER_H
