#include <GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
#include<time.h>
using namespace std;
#define pi (2*acos(0.0))

double const EPS = 1e-9;
int const NUMBER_OF_CIRCLE = 5;
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
int const GRID_LEN = 500;
double keySensitivity = 10, angleSensitivity = 3;

// constants about shapes
const int slice = 50, stack = 50;
double const BigRadius = 50;

struct Point{
    double x{},y{},z{};
    Point(){}
    Point(double _x, double _y, double _z):x(_x), y(_y), z(_z){}

    Point& operator+=(const Point &p) {
        x += p.x;
        y += p.y;
        z += p.z;
        return *this;
    }
    Point& operator-=(const Point &p) {
        x -= p.x;
        y -= p.y;
        z -= p.z;
        return *this;
    }
    Point& operator*=(const double s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    Point& operator/=(const double s) {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }
    Point operator+(const Point& p) const {
        return Point(*this) += p;
    }

    Point operator-(const Point& p) const {
        return Point(*this) -= p;
    }

    Point operator*(const double t) const {
        return Point(*this) *= t;
    }

    Point operator/(const double t) const {
        return Point(*this) /= t;
    }

};

Point u, l, r, pos;


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
    return Point(a.y * b.z - a.z * b.y,
                   a.z * b.x - a.x * b.z,
                   a.x * b.y - a.y * b.x);
}

/**
 *
 * @param axis -> rotating axis
 * @param rotatingVector -> the vector that needs to rotate
 * @param angle -> in degrees
 * @return
 */
Point rotateAroundAnAxis(Point axis, Point rotatingVector, double angle){
    angle = pi / 180.0 * angle;
    return rotatingVector * cos(angle) + cross(axis, rotatingVector) * sin(angle);
}

struct Circle{
    Point centre;
    double radius;
    Circle(){}
    Circle(Point c, double r):centre(c), radius(r){}

    bool isInside(const Circle &rhs) const {
        double centerDistance = norm(centre - rhs.centre);
        return fabs(radius - rhs.radius) + EPS > centerDistance;
    }

    bool doesTouchFromInside(const Circle &rhs) const {      // rhs is the bigger circle
        double centerDistance = norm(centre - rhs.centre);
        return  centerDistance + radius + EPS > rhs.radius;
    }

    bool doesTouchFromOutside(const Circle &rhs) const {
        double centerDistance = norm(centre - rhs.centre);
        return radius + rhs.radius >= centerDistance;
    }

};

void drawAxes() {
    if (drawaxes == 1) {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f(100, 0, 0);
            glVertex3f(-100, 0, 0);

            glVertex3f(0, -100, 0);
            glVertex3f(0, 100, 0);

            glVertex3f(0, 0, 100);
            glVertex3f(0, 0, -100);
        }
        glEnd();
    }
}

void drawLine(Point a, Point b){
    glBegin(GL_LINES);{
        glVertex3f( a.x,a.y,a.z);
        glVertex3f(b.x, b.y, b.z);
    }glEnd();
}

void drawCircle(double radius,int segments)
{
    int i;
    Point points[100];
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawRectangle(){
    glColor3f(0, 1.0, 0);
    drawLine(Point(0, 0, 0), Point(0, GRID_LEN, 0));
    drawLine(Point(0, 0, 0), Point(GRID_LEN, 0, 0));
    drawLine(Point(GRID_LEN, 0, 0), Point(GRID_LEN, GRID_LEN, 0));
    drawLine(Point(0, GRID_LEN, 0), Point(GRID_LEN, GRID_LEN, 0));

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
            points[i][j].x=r*cos(((double)j/(double)slices)*pi - pi/2);
            points[i][j].y=h;
            points[i][j].z=r*sin(((double)j/(double)slices)*pi - pi/2);
        }
    }
    //draw quads using generated points
    for(i=0;i<stacks;i++)
    {
        for(j=0;j<slices;j++)
        {
            glBegin(GL_QUADS);{
                //upper hemisphere
                glColor3f(j&1 , j&1, j&1);
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

void positionBigSemiSphereLeft(){
    glPushMatrix();
    glRotatef(-180, 0, 0, 1);
    glRotatef(-180, 1, 0, 0);
    drawSphere(BigRadius, slice,  stack);
    glPopMatrix();
}

void positionBigSemiSphereRight(){
    glPushMatrix();
    drawSphere(BigRadius, slice, stack);
    glPopMatrix();
}

void drawScreen(){
    drawAxes();
    //drawRectangle();
    glColor3f(1.0, 0, 0);
    glRotatef(90, 0, 0, 1);
    positionBigSemiSphereLeft();
    positionBigSemiSphereRight();
}

void keyboardListener(unsigned char key, int x,int y){
    switch(key){
        case '1':
            l = rotateAroundAnAxis(u, l, angleSensitivity);
            r = rotateAroundAnAxis(u, r, angleSensitivity);
            break;
        case '2':
            l = rotateAroundAnAxis(u, l, -angleSensitivity);
            r = rotateAroundAnAxis(u, r, -angleSensitivity);
            break;
        case '3':
            l = rotateAroundAnAxis(r, l, angleSensitivity);
            u = rotateAroundAnAxis(r, u, angleSensitivity);
            break;
        case '4':
            l = rotateAroundAnAxis(r, l, -angleSensitivity);
            u = rotateAroundAnAxis(r, u, -angleSensitivity);
            break;
        case '5':
            r = rotateAroundAnAxis(l, r, angleSensitivity);
            u = rotateAroundAnAxis(l, u, angleSensitivity);
            break;
        case '6':
            r = rotateAroundAnAxis(l, r, -angleSensitivity);
            u = rotateAroundAnAxis(l, u, -angleSensitivity);
            break;
        default:
            break;
    }
}


void specialKeyListener(int key, int x,int y){
    switch(key){
        case GLUT_KEY_DOWN:		//down arrow key
            pos -= l * keySensitivity;
            break;
        case GLUT_KEY_UP:		// up arrow key
            pos += l * keySensitivity;
            break;
        case GLUT_KEY_RIGHT:
            pos += r * keySensitivity;
            break;
        case GLUT_KEY_LEFT:
            pos -= r * keySensitivity;
            break;
        case GLUT_KEY_PAGE_UP:
            pos += u * keySensitivity;
            break;
        case GLUT_KEY_PAGE_DOWN:
            pos -= u * keySensitivity;
            break;
        case GLUT_KEY_INSERT:
            break;
        case GLUT_KEY_HOME:
            break;
        case GLUT_KEY_END:
            break;

        default:
            break;
    }
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
    switch(button){
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
                drawaxes=1-drawaxes;
            }
            break;

        case GLUT_RIGHT_BUTTON:
            //........
            break;

        case GLUT_MIDDLE_BUTTON:
            //........
            break;

        default:
            break;
    }
}


void display(){

    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?

    //gluLookAt(100,100,100,	0,0,0,	0,0,1);
    //gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
    Point lookAt = pos + l;

    gluLookAt(pos.x, pos.y, pos.z,
              lookAt.x, lookAt.y, lookAt.z,
              u.x, u.y, u.z);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    drawScreen();

    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate(){
   glutPostRedisplay();
}

void init(){
    //codes for initialization
    drawgrid=0;
    drawaxes=1;
    pos = Point(0, 0, 200);
    u = Point(0, 1, 0);
    l = Point(0, 0, -1);
    r = Point(1, 0, 0);

    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

int main(int argc, char **argv){
    //srand(time(0));
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(300, 250);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("Task-1");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occurring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}