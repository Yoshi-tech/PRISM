#ifndef CAMERA_H
#define CAMERA_H
#include <cmath>
#include <iostream>
#include "vec3.h"
#include "mat4.h"

class camera{
    public:
        Vec3 position;
        Vec3 forward;
        Vec3 up;
        Vec3 target;

        double azimuth;
        double elevation;
        double radius;


        camera() {
            position = Vec3(0, 0, 5);
            target = Vec3(0, 0, 0);
            up = Vec3(0, 1, 0);
            azimuth = 0.0;
            elevation = 0.0;
            radius = 5.0;
        }


        Vec3 updatePosition(){
            double x = x = radius * cos(elevation) * sin(azimuth);
            double y = radius * sin(elevation);
            double z = radius * cos(elevation) * cos(azimuth);

            position = {x,y,z};
            return position;

        }

        mat4 lookAt(Vec3 position, Vec3 target, Vec3 up){
            Vec3 forward = unit_vector(target - position);
            Vec3 right = unit_vector(cross(forward, up));
            up = cross(right, forward);

            mat4 rot = mat4::rotationMat(right, up, forward);
            mat4 tran = mat4::translation(-position.x(), -position.y(), -position.z());

            return rot * tran;
        }


};

#endif
