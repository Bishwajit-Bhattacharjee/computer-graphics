//
// Created by bishwajit on ২৩/৬/২১.
//

#ifndef RAY_TRACING_1605003_OBJECT_H
#define RAY_TRACING_1605003_OBJECT_H

#include "1605003_Point.h"
#include "1605003_Ray.h"
#include "1605003_Light.h"

using namespace std;

double const EPS = 1e-7;
double const OFFSET = 1e-7;

struct Object;

extern vector<Object*> objects;
extern vector<Light*> lights;
extern int level_of_recursion;
extern bool isShadowOn;
extern bool isRecursionLevelOn;


struct Object {
    Point reference_point;
    double height, width, length;
    Color color;
    double coEfficients[4];
    int shine;
    Object(){}
    double intersect(Ray &r, Color &c, int level);
    void updateDiffuseAndSpecularComponent(Ray &r, Color &c, Point &intersectingPoint, Color &intersectingPointColor, Point &normal, Light* light);
    bool isPointInShadow(Ray &rayFromLight, Point &intersecingPoint);

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

double Object::intersect(Ray &rayFromEye, Color &c, int depth) {
    double tMin = nearestTouch(rayFromEye);
    if (depth == 0) return tMin;
    Point intersectingPoint = rayFromEye.start + rayFromEye.dir*tMin;
    Color intersectingPointColor = getColorAt(intersectingPoint);

    c = intersectingPointColor * coEfficients[0]; // ambient component
    
    Point normal = getNormal(intersectingPoint);

    for (Light* light : lights){
        Ray rayFromLight = Ray(light->light_pos, intersectingPoint - light->light_pos);

        if (!isShadowOn || !isPointInShadow(rayFromLight, intersectingPoint)) {
            updateDiffuseAndSpecularComponent(rayFromEye, c, intersectingPoint, intersectingPointColor, normal, light);
        }
    }

    if (!isRecursionLevelOn) {
        return tMin;
    }

    if (depth >= level_of_recursion) return tMin;

    Point reflectedDir = rayFromEye.dir - 2 * dot(rayFromEye.dir, normal) * normal;
    Ray reflectedRay = Ray(intersectingPoint + reflectedDir*OFFSET, reflectedDir);

    
    double nearestDist = 1e9;
    Object* nearestObject = nullptr;
    Color dummy;
    for (auto object : objects){
        double t = object->intersect(reflectedRay, dummy, 0);
        if (t < 0) continue;
        if (t < nearestDist) {
            nearestDist = t;
            nearestObject = object;
        }
    }

    if (nearestObject) {
        Color reflectedColor;
        assert(fabs(norm(reflectedColor)) < EPS);
        nearestObject->intersect(reflectedRay, reflectedColor, depth+1);
        c = c + reflectedColor * coEfficients[3];
    }

    return tMin;
}

bool Object::isPointInShadow(Ray &rayFromLight, Point &intersectingPoint){

    Color dummy;
    double objectDistance = norm(intersectingPoint - rayFromLight.start);

    double tMin = 1e9;
    for (Object* object: objects){
        double curVal = object->intersect(rayFromLight, dummy, 0);
        if (curVal < 0) continue;
        if (curVal < tMin) tMin = curVal;
    }

    assert(tMin < 1e8);
    return objectDistance > tMin + EPS;
}

void Object::updateDiffuseAndSpecularComponent(Ray &rayFromEye, Color &c, Point &intersectingPoint, Color &intersectingPointColor, Point &normal, Light* light){
    Ray rayFromLight = Ray(light->light_pos, intersectingPoint - light->light_pos);
    // diffuse component
    double lambertValue = max(0.0, 
        dot(normal, normalize(light->light_pos - intersectingPoint)));

    Point reflected = rayFromLight.dir - 2 * dot(normal, rayFromLight.dir) * normal;
    reflected *= -1;

    double phongValue = max(0.0, dot(reflected, rayFromEye.dir));

    c = c + light->color * coEfficients[1] * lambertValue * intersectingPointColor;
    c = c + light->color * coEfficients[2] * pow(phongValue, shine);

    assert(c.x <= 1.0 && c.y <= 1.0 && c.z <= 1.0);
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
