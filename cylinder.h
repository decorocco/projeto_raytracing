#ifndef CYLINDER_H
#define CYLINDER_H


#include "rtweekend.h"

#include "hittable.h"


class cylinder : public hittable {
    public:
        cylinder() {}

        cylinder(point3 cen, double r, double h, shared_ptr<material> m)
            : center(cen), radius(r), height(h), mat_ptr(m) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center;
        double radius;
        double height;
        shared_ptr<material> mat_ptr;

};


bool cylinder::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {

    auto o = r.origin();
    auto d = r.direction();
    auto f = center;

    auto a = d[0]*d[0] + d[2]*d[2];
    
    auto b = 2*o[0]*d[0] - 2*f[0]*d[0] + 2*o[2]*d[2] - 2*f[2]*d[2];

    auto c = o[0]*o[0] - 2*o[0]*f[0] - f[0]*f[0] + o[2]*o[2] - 2*o[2]*f[2] - f[2]*f[2] - radius*radius;

    auto discriminant = b*b - 4*a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    auto root = (-b - sqrtd) / (2*a);
    auto root2 = (-b + sqrtd) / (2*a);
    auto p = r.at(root);

    bool caso1 = false;
    bool caso2 = false;

    if (root < t_min || t_max < root) {

        p = r.at(root2);

        if (root2 < t_min || t_max < root2)
            return false;
    }

    auto x = o[1] + root * d[1];
    auto z = o[1] + root2 * d[1];

    if((x > f[1] - height/2) && (z > f[1] - height/2) && (x < f[1]+height/2) && (z < f[1]+height/2)){
        p = r.at(root2);
    } 

    else if(!(x > f[1] - height/2) != !(z > f[1] - height/2)){
        p = r.at(((f[1] + height) - o[1]) / d[1]);
        caso1 = true;
    }
    else if(!(x < f[1] + height/2) != !(z < f[1] + height/2)){
        p = r.at((f[1] - o[1]) / d[1]);
        caso2 = true;
    }
    else return false;

    rec.t = root;
    rec.p = r.at(rec.t);

    vec3 norm = vec3(f[0], f[1], rec.p[2]);
    vec3 outward_normal = (rec.p - norm) / radius;

    //if(caso1){ std::cerr << "\n1\n"; }
    //if(caso2){ std::cerr << "\n2\n"; }

    if(caso1){
        vec3 outward_normal = vec3(0, 0, 1);
    }
    if(caso2){
        vec3 outward_normal = vec3(0, 0, -1);
    }

    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}


#endif
