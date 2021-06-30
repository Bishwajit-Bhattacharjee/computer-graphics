//
// Created by bishwajit on ২৩/৬/২১.
//

#ifndef RAY_TRACING_1605003_FLOOR_H
#define RAY_TRACING_1605003_FLOOR_H

#include "1605003_Objects.h"
#include <GL/glut.h>

void drawSquare(double a, int col)
{
    glColor3f(col,col,col);
    glBegin(GL_QUADS);{
        glVertex3f( 0, 0,0);
        glVertex3f( 0,a,0);
        glVertex3f(a,a,0);
        glVertex3f(a, 0,0);
    }glEnd();
}

struct Floor: Object{
    Floor(int floor_width, int tile_width){
        reference_point = Point(-1.0*floor_width/2, -1.0*floor_width/2, 0);
        width = tile_width;
        length = floor_width;
        coEfficients[0] = 0.1;
        coEfficients[1] = 0.4;
        coEfficients[2] = 0.1;
        coEfficients[3] = 0.5;
        shine = 1;
    }

    void draw() override{
        int limit = (int)(length/width + 1e-9);
        for (int i = 0; i < limit; i++){
            for (int j = 0; j < limit; j++){
                Point cur = reference_point;
                cur.x += i * width;
                cur.y += j * width;
                glPushMatrix();
                glTranslatef(cur.x, cur.y, 0);
                drawSquare(width, (i+j)&1);
                glPopMatrix();
            }
        }
    }

    Color getColorAt(Point &p) override {
        int x = max(0, int(EPS + (p.x - reference_point.x)/ width));
        int y = max(0, int(EPS + (p.y - reference_point.y)/ width));
        bool parity = (x + y) & 1;
        return Point(parity, parity, parity);
    }

    Point getNormal(Point &p) override {
        return Point (0, 0, 1);
    }

    double nearestTouch(Ray &r) override {
        double d = 0;
        Point n = Point(0,0,1);
        double denom = dot(n, r.dir);
        if (fabs(denom) < EPS) {
            return -1.0; // ray is parallel to the plane
        }
        double t = -(d + dot(n, r.start)) / denom;
        if (t < 0) return -1.0;
        Point intersectingPoint = r.start + t * r.dir;
        assert(fabs(intersectingPoint.z) < EPS);

        bool isXinside = reference_point.x < intersectingPoint.x + EPS && intersectingPoint.x < reference_point.x + length + EPS;
        bool isYinside = reference_point.y < intersectingPoint.y + EPS && intersectingPoint.y < reference_point.y + length + EPS;

        if (isXinside && isYinside) {
            return t;
        }
        return -1.0;
    }

    istream &input(istream &is) override {
        return is;
    }

    ostream &output(ostream &os) override {
        os << "Floor" << endl;
        os << "Ref Point " << reference_point << endl;
        os << "floor width: " << length << " tile width " << width << endl;
        return os;
    }
};


#endif //RAY_TRACING_1605003_FLOOR_H
