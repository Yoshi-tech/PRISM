#ifndef MAT4
#define MAT4
#include <cmath>
#include <iostream>
#include "vec3.h"
#include "vec4.h"

class mat4{
    public:
        double mat[4][4];
        mat4() {
            clearMat();
        }

        mat4(double a00, double a01, double a02, double a03,
         double a10, double a11, double a12, double a13,
         double a20, double a21, double a22, double a23,
         double a30, double a31, double a32, double a33);

        double get(int row, int column){
            return mat[row][column];
        }

        void set(int row, int column, double value){
            mat[row][column] = value;
        }

        void clearMat(){
            for(int i =0; i < 4; i++){
                for(int j =0; j < 4; j++){
                    mat[i][j] = 0.0;
                }
            }
        }

        static mat4 identity(){
            mat4 other;
            other.clearMat();

            for(int i =0; i < 4; i++){
                for(int j =0; j < 4; j++){
                    if(i == j){
                        other.set(i,j,1.0);
                    }
                }
            }

            return other;
        }

        mat4 operator*(const mat4& other) const {
            mat4 result;
            result.clearMat();

            for (int i = 0; i < 4; ++i){
                for (int j = 0; j < 4; ++j){
                    for (int k = 0; k < 4; ++k){
                        result.mat[i][j] += mat[i][k] * other.mat[k][j];
                    }
                }
            }

            return result;
        }

        Vec4 operator*(const Vec4& v) const {
            Vec4 result;
            for (int row = 0; row < 4; ++row) {
                double sum = 0;
                for (int col = 0; col < 4; ++col) {
                    sum += mat[row][col] * v[col];
                }
                result[row] = sum;
            }
            return result;
        }


        static mat4 translation(double x, double y, double z){
            mat4 i = identity();
            Vec3 translationVec = {x, y, z};
            i.set(0, 3, x);
            i.set(1, 3, y);
            i.set(2, 3, z);

            return i;

        }

        static mat4 rotationX(double angle){
            mat4 i = identity();
            i.set(1,1,cos(angle));
            i.set(1,2,-sin(angle));
            i.set(2,1,sin(angle));
            i.set(2,2,cos(angle));

            return i;

        }
        
        static mat4 scale(double x, double y, double z){
            mat4 i = identity();
            Vec3 translationVec = {x, y, z};
            i.set(0, 0, x);
            i.set(1, 1, y);
            i.set(2, 2, z);

            return i;

        }

        static mat4 rotationMat(Vec3 right, Vec3 up, Vec3 forward){
            mat4 i = identity();
            i.set(0,0,right.x());
            i.set(0,1,right.y());
            i.set(0,2,right.z());
            i.set(1,0, up.x());
            i.set(1,1, up.y());
            i.set(1,2, up.z());
            i.set(2,0, -forward.x());
            i.set(2,1, -forward.y());
            i.set(2,2, -forward.z());

            return i;
        }

        static mat4 perspective(double fov, double aspect, double near, double far) {
            double radians = fov * M_PI / 180.0;
            double tanHalfFov = tan(radians / 2.0);
            mat4 result;
            result.clearMat();

            result.set(0, 0, 1.0 / (aspect * tanHalfFov));
            result.set(1, 1, 1.0 / tanHalfFov);
            result.set(2, 2, -(far + near) / (far - near));
            result.set(2, 3, -(2.0 * far * near) / (far - near));
            result.set(3, 2, -1.0);

            return result;
        }



};
#endif
