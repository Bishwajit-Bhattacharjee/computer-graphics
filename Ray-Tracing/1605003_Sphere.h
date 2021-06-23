//
// Created by bishwajit on ২৩/৬/২১.
//

#ifndef RAY_TRACING_1605003_SPHERE_H
#include "1605003_Objects.h"
#include "1605003_Point.h"
#include <GL/glut.h>

const int slices = 50, stacks = 50;


struct Sphere:Object {
    Sphere(){}
    Sphere(Point c, double radius){
        reference_point = c;
        length = radius;
    }
    void draw() override{
        glPushMatrix();
        glTranslatef(reference_point.x, reference_point.y, reference_point.z);
        drawSphere(length, slices, stacks);
        glPopMatrix();
    }
    void drawSphere(double radius,int slices,int stacks)
    {
        Point points[100][100];
        int i,j;
        double h,r;
        //generate points
        for(i=0;i<=stacks;i++)
        {
            h=radius*sin(((double)i/(double)stacks)*(pi/2));
            r=radius*cos(((double)i/(double)stacks)*(pi/2));
            for(j=0;j<=slices;j++)
            {
                points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                points[i][j].y=h;
                points[i][j].z=r*sin(((double)j/(double)slices)*2*pi);
            }
        }
        //draw quads using generated points
//        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        glColor3f(color.x, color.y, color.z);
        for(i=0;i<stacks;i++)
        {
            for(j=0;j<slices;j++)
            {
                glBegin(GL_QUADS);{
                    //upper hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x,-points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,-points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,-points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,-points[i+1][j].y,points[i+1][j].z);
                }glEnd();
            }
        }
    }
    istream& input(istream &is){
        is >> reference_point >> length >> color;
        for (int i = 0; i < 4; i++){
            is >> coEfficients[i];
        }
        is >> shine;
        return is;
    }

    ostream& output(ostream& os){
        os << reference_point << endl;
        os << "radius " << length << endl;
        os << "color " << color << endl;
        os << "coeffs " ;
        for (int i = 0; i < 4; i++){
            os << coEfficients[i] << " " ;
        }
        os << endl;
        os << "Shine " << shine << endl;
        return os;
    }
};

#define RAY_TRACING_1605003_SPHERE_H

#endif //RAY_TRACING_1605003_SPHERE_H
