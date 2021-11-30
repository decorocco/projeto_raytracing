#ifndef RING_H
#define RING_H

#include "rtweekend.h"

#include "hittable.h"


class ring : public hittable {
    public:
        ring() {}

        ring(point3 cen, double r, double h, shared_ptr<material> m)
            : center(cen), radius(r), height(h), mat_ptr(m) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center;
        double radius;
        double height;
        shared_ptr<material> mat_ptr;

};


bool ring::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {

    auto o = r.origin();
    auto d = r.direction();
    auto f = center;

    auto a = d[0]*d[0] + d[2]*d[2];
    auto b = 2*o[0]*d[0] - 2*f[0]*d[0] + 2*o[2]*d[2] - 2*f[2]*d[2];
    auto c_maior = o[0]*o[0] - 2*o[0]*f[0] - f[0]*f[0] + o[2]*o[2] - 2*o[2]*f[2] - f[2]*f[2] - radius*radius;
    auto c_menor = o[0]*o[0] - 2*o[0]*f[0] - f[0]*f[0] + o[2]*o[2] - 2*o[2]*f[2] - f[2]*f[2] - (radius/2)*(radius/2);

    auto discriminant_maior = b*b - 4*a*c_maior;
    if (discriminant_maior < 0) return false;
    auto sqrtd_maior = sqrt(discriminant_maior);

    auto discriminant_menor = b*b - 4*a*c_menor;
    if (discriminant_menor < 0) return false;
    auto sqrtd_menor = sqrt(discriminant_menor);

    auto root1_maior = (-b - sqrtd_maior) / (2*a);
    auto root2_maior = (-b + sqrtd_maior) / (2*a);

    auto root1_menor = (-b - sqrtd_menor) / (2*a);
    auto root2_menor = (-b + sqrtd_menor) / (2*a);

    auto p_maior = r.at(root1_maior);
    auto p_menor = r.at(root1_menor);

    bool caso1 = false;
    bool caso2 = false;

    if (root1_maior < t_min || t_max < root1_maior) {

        p_maior = r.at(root2_maior);

        if (root2_maior < t_min || t_max < root2_maior)
            return false;
    }
    if (root1_menor < t_min || t_max < root1_menor) {

        p_menor = r.at(root2_menor);

        if (root2_menor < t_min || t_max < root2_menor)
            return false;
    }

    auto x_maior = o[1] + root1_maior * d[1];
    auto z_maior = o[1] + root2_maior * d[1];
    auto x_menor = o[1] + root1_menor * d[1];
    auto z_menor = o[1] + root2_menor * d[1];

    if((x_maior > f[1] - height/2) && (z_maior > f[1] - height/2) && (x_maior < f[1]+height/2) && (z_maior < f[1]+height/2)){
        p_maior = r.at(root2_maior);
    } 
    if((x_menor > f[1] - height/2) && (z_menor > f[1] - height/2) && (x_menor < f[1]+height/2) && (z_menor < f[1]+height/2)){
        p_menor = r.at(root2_menor);
    } 

    else if(!(x_maior > f[1] - height/2) != !(z_maior > f[1] - height/2)){
        p_maior = r.at(((f[1] + height) - o[1]) / d[1]);
        caso1 = true;
    }
    else if(!(x_menor > f[1] - height/2) != !(z_menor > f[1] - height/2)){
        p_menor = r.at(((f[1] + height) - o[1]) / d[1]);
        caso1 = true;
    }

    else if(!(x_maior < f[1] + height/2) != !(z_maior < f[1] + height/2)){
        p_maior = r.at((f[1] - o[1]) / d[1]);
        caso2 = true;
    }
    else if(!(x_menor < f[1] + height/2) != !(z_menor < f[1] + height/2)){
        p_menor = r.at((f[1] - o[1]) / d[1]);
        caso2 = true;
    }

    rec.t = root1_maior;
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
