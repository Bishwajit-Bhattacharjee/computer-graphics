//
// Created by bishwajit on ২৩/৬/২১.
//

#ifndef RAY_TRACING_1605003_OBJECT_H
#define RAY_TRACING_1605003_OBJECT_H

#include "1605003_Point.h"
#include "1605003_Ray.h"
#include "1605003_Light.h"

using namespace std;

struct Object;
extern vector<Object*> objects;
extern vector<Light*> lights;


struct Object {
    Point reference_point;
    double height, width, length;
    Color color;
    double coEfficients[4];
    int shine;
    Object(){}
    double intersect(Ray &r, Color &c, int level);

    virtual void draw();
    virtual Color getColorAt(Point &p);
    virtual Point getNormal(Point &p);
    virtual double nearestTouch(Ray &r);
    virtual istream& input(istream &is);
    virtual ostream& output(ostream &os);

    friend ostream& operator<<(ostream &os, Object& o);
    friend istream& operator>>(istream &is, Object& o);
    ~Object(){}
};

double Object::intersect(Ray &r, Color &c, int depth) {
    double tMin = nearestTouch(r);
    if (depth == 0) return tMin;
    Point intersectingPoint = r.start + r.dir*tMin;
    Color intersectingPointColor = getColorAt(intersectingPoint);
    c = intersectingPointColor * coEfficients[0];
    Point normal = getNormal(intersectingPoint);

    for (Light* light : lights){
        Ray ray_l = Ray(light->light_pos, intersectingPoint - light->light_pos);
        // diffuse component
        double lambertValue = max(0.0, 
            dot(normal, normalize(light->light_pos - intersectingPoint)));

        Point reflected = ray_l.dir - 2 * dot(normal, ray_l.dir) * normal;
        reflected *= -1;

        double phongValue = max(0.0, dot(reflected, r.dir));

        c += light->color * coEfficients[1] * lambertValue * intersectingPointColor;
        c += light->color * coEfficients[2] * pow(phongValue, shine) * intersectingPointColor;
    }

    return tMin;
}
istream& operator>>(istream &is, Object &o){
    o.input(is);
    return is;
}

ostream &operator<<(ostream &os, Object &o) {
    o.output(os);
    return os;
}

istream &Object::input(istream &is) {
    return is;
}

void Object::draw() {

}

ostream &Object::output(ostream &os) {
    return os;
}

Point Object::getNormal(Point &p) {
    return Point();
}

double Object::nearestTouch(Ray &r) {
    return 0;
}

Color Object::getColorAt(Point &p) {
    return Color();
}



#endif //RAY_TRACING_1605003_OBJECT_H
