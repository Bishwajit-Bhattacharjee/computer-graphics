#include <GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
#include<time.h>
using namespace std;
#define pi (2*acos(0.0))

double const EPS = 0;
int const NUMBER_OF_CIRCLE = 5;
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
int seg, slices;
double speed = 0.009, tuner = 0.0001;
double maxSpeed = 0.05, minSpeed = 0.0008;
double GRID_LEN = 250;
double R = 100, r = 10;
bool insideBigCircle[NUMBER_OF_CIRCLE];
bool stillTouching[NUMBER_OF_CIRCLE][NUMBER_OF_CIRCLE];
int drawing_ind, tick = 0, pause = 0;


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

double dot(Point &a, Point &b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double norm(Point p){
    return sqrt(dot(p, p));
}

Point normalize(Point p){
    return p / norm(p);
}

Point reflect(Point d, Point n){
    return d - n * 2 * dot(d, n);
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

    bool isFarEnough(const Circle &rhs) const {
        double centerDistance = norm(centre - rhs.centre);
        return radius + rhs.radius + 0.01 >= EPS + centerDistance;
    }
};


Point centrePoint;
Circle BIG;
Circle small[NUMBER_OF_CIRCLE];
Point velocity[NUMBER_OF_CIRCLE];

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
    //glColor3f(0.7,0.0,0.0);
    //generate points
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

void positionCircle(Circle c){
    glPushMatrix();
    glTranslatef(c.centre.x, c.centre.y, c.centre.z);
    drawCircle(c.radius, 50);
    glPopMatrix();
}

void drawScreen(){
    drawRectangle();
    glColor3f(1.0, 0, 0);
    positionCircle(BIG);
    glColor3f(0, 0.7, 0.7);

    for (int i = 0; i < drawing_ind; i++)
        positionCircle(small[i]);
}

void keyboardListener(unsigned char key, int x,int y){
    switch(key){

        case '1':
            drawgrid=1-drawgrid;
            break;

        case 'p':
            pause ^= 1;
            break;

        default:
            break;
    }
}


void specialKeyListener(int key, int x,int y){
    switch(key){
        case GLUT_KEY_DOWN:		//down arrow key
            speed -= tuner;
            speed = max(speed, minSpeed);
            printf("speed %f\n", speed);
            break;
        case GLUT_KEY_UP:		// up arrow key
            speed += tuner;
            speed = min(speed, maxSpeed);
            printf("speed %f\n", speed);
            break;

        case GLUT_KEY_RIGHT:
            cameraAngle += 0.03;
            break;
        case GLUT_KEY_LEFT:
            cameraAngle -= 0.03;
            break;

        case GLUT_KEY_PAGE_UP:
            break;
        case GLUT_KEY_PAGE_DOWN:
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
    gluLookAt(GRID_LEN / 2,GRID_LEN / 2,200,
              GRID_LEN / 2,GRID_LEN / 2,0,
              0,1,0);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects
    drawScreen();
    //drawSS();
    //drawCircle(10, 25);

    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate(){
    if (pause) return;

    if (tick == 0){
        drawing_ind = min(drawing_ind + 1, NUMBER_OF_CIRCLE);
    }
    tick++;
    if (tick == 3000) tick = 0;
    // handle side reflection


    for (int i = 0; i < drawing_ind; i++){
        Circle &cur = small[i];
        if (cur.centre.y <= EPS + cur.radius) {     // y = 0
            velocity[i].y *= -1.0;
        }
        if (cur.centre.y + cur.radius + EPS >= GRID_LEN) { // y = GRID_LEN
             velocity[i].y *= -1.0;
        }
        if (cur.centre.x <= EPS + cur.radius) {     // x = 0
            velocity[i].x *= -1.0;
        }
        if (cur.centre.x + cur.radius + EPS >= GRID_LEN) {  // x = GRID_LEN
            velocity[i].x *= -1.0;
        }
    }

    for (int i = 0; i < drawing_ind; i++){
        if (insideBigCircle[i]) continue;
        insideBigCircle[i] = BIG.isInside(small[i]);
        if (insideBigCircle[i]){ // entered now
            for (int j = 0; j < drawing_ind; j++){
                if (j == i) continue;
                if (small[i].doesTouchFromOutside(small[j])){
                    stillTouching[i][j] = stillTouching[j][i] = true;
                }
            }
        }
    }

    for (int i = 0; i < drawing_ind; i++){
        if (!insideBigCircle[i]) continue;
        if (small[i].doesTouchFromInside(BIG)){
            velocity[i] = reflect(velocity[i],
                                  normalize(BIG.centre - small[i].centre));
        }
    }

    for (int i = 0; i < drawing_ind; i++){
        for (int j = i + 1; j < drawing_ind; j++){
            if (!insideBigCircle[i] || !insideBigCircle[j]) continue;
            bool currentlyTouch = small[i].doesTouchFromOutside(small[j]);
            if (stillTouching[i][j] && currentlyTouch){
                continue;
            }
            stillTouching[i][j] = stillTouching[j][i] = false;

            if (currentlyTouch) {
                swap(velocity[i], velocity[j]);
            }
        }
    }

    for (int i = 0; i < drawing_ind; i++){
        //printf("%d : (%f, %f)\n", i, velocity[i].x, velocity[i].y);
        small[i].centre += velocity[i] * speed;
        //printf("position %d : (%f, %f)\n", i, small[i].centre.x, small[i].centre.y);
    }

    glutPostRedisplay();
}

void init(){
    //codes for initialization
    drawgrid=0;
    drawaxes=0;
    cameraHeight=150.0;
    cameraAngle=1.0;
    angle=0;
    centrePoint = Point(GRID_LEN/2, GRID_LEN/2, 0);
    BIG = Circle(centrePoint, R);

    for (int i = 0; i < NUMBER_OF_CIRCLE; i++){
        small[i] = Circle(Point(r + 0.001, r + 0.001, 0), r);
        velocity[i] = Point(rand(), rand(), 0);
        velocity[i] = normalize(velocity[i]);
        printf("%lf %lf\n", velocity[i].x, velocity[i].y);
    }
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
    srand(time(0));
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(300, 250);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("Task-2");

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