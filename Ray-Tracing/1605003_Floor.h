//
// Created by bishwajit on ২৩/৬/২১.
//

#ifndef RAY_TRACING_1605003_FLOOR_H
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

//struct Floor: Object{
//    Floor(int floor_width, int tile_width){
//        reference_point = Point(-1.0*floor_width/2, -1.0*floor_width/2, 0);
//        width = tile_width;
//        length = floor_width;
//        shine = 1;
//    }
//    void draw() override{
//        int limit = (int)(length/width + 1e-9);
//        for (int i = 0; i < limit; i++){
//            for (int j = 0; j < limit; j++){
//                Point cur = reference_point;
//                cur.x += i * width;
//                cur.y += j * width;
//                glPushMatrix();
//                glTranslatef(cur.x, cur.y, 0);
//                drawSquare(width, (i+j)%2);
//                glPopMatrix();
//            }
//        }
//    }
//};

#define RAY_TRACING_1605003_FLOOR_H

#endif //RAY_TRACING_1605003_FLOOR_H
