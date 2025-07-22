#ifndef VEC3

#define VEC3

#include <cmath>
#include <iostream>

using std::sqrt;

class Vec3{

    public:
        // declaring an array for representing a vector in x,y,z
        double vec[3];

        Vec3() : vec{0,0,0} {}
        Vec3(double v0, double v1, double v2) : vec{v0, v1, v2} {}

        // creating functions for x, y and z

        double x() const {return vec[0];}
        double y() const {return vec[1];}
        double z() const {return vec[2];}

        Vec3 operator-() const {
            return Vec3(-vec[0], -vec[1], -vec[2]);
        }


        double& operator[](int i) { return vec[i]; }
        const double& operator[](int i) const { return vec[i]; }

        // adding vectors together 
   
         Vec3& operator+=(const Vec3 &v) {
             vec[0] += v.vec[0];
             vec[1] += v.vec[1];
             vec[2] += v.vec[2];
             return *this;
        }

        Vec3& operator*=(double i) {
            vec[0] *= i;
            vec[1] *= i;
            vec[2] *= i;
            return *this;
        }
        double length() const {
            return sqrt(pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2));
        }

        Vec3& operator/=(double i) {
            vec[0] = vec[0] / i;
            vec[1] = vec[1] / i;
            vec[2] = vec[2] / i;
            return *this;
        }

      
        Vec3 getVec() const {
            for (int i = 0; i < 3; ++i) {
                if (abs(vec[i]) == INFINITY) {
                    std::cerr << "Vector component is infinity!" << std::endl;
                    // Returning a default vector in case of infinity to avoid undefined behavior
                    return Vec3(0.0, 0.0, 0.0);
                 }
            }
            return *this;
        }       

};



        inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
            return Vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
        }

        inline Vec3 operator*(double t, const Vec3 &v) {
            return Vec3(t * v[0], t * v[1], t * v[2]);
        }

        inline Vec3 operator*(const Vec3 &v, double t) {
            return Vec3(t * v[0], t * v[1], t * v[2]);
        }

        inline Vec3 operator/(const Vec3& v, double t) {
            return Vec3(v[0] / t, v[1] / t, v[2] / t);
        }



        inline Vec3 operator+(const Vec3&g, const Vec3&x){
            return Vec3(g.vec[0] + x.vec[0], g.vec[1] + x.vec[1], g.vec[2] + x.vec[2]);
         }

        inline Vec3 operator-(const Vec3&g, const Vec3&x){
            return Vec3(g.vec[0] - x.vec[0], g.vec[1] - x.vec[1], g.vec[2] - x.vec[2]);
         }
         
        inline Vec3 operator-(const Vec3&g, double x){
            return Vec3(g.vec[0] - x, g.vec[1] - x, g.vec[2] - x);
         }



        Vec3 unit_vector(Vec3 v){
            double length = v.length();
            if (length != 0.0) {
                return v * (1/length);
            } else {
                std::cerr << "Cannot calculate unit vector for a zero-length vector." << std::endl;
                return Vec3(0.0, 0.0, 0.0);  // Returning a default vector to avoid division by zero
            }
        }

        // scalar product
 
        inline double scalarP (const Vec3 &g, const Vec3 &h){

            return h.vec[0]*g.vec[0] + h.vec[1]*g.vec[1] + h.vec[2]*g.vec[2];

        }

         

        // angle betwen two vectors

       /* double angle(const Vec3 &g) {


            double magnitude = g.length() * g.length();

            if (magnitude != 0.0){

                return acos(scalarP(g)/magnitude);
            }

            return 0.0;

        }*/

        // dot product

        /*inline double dotP(const Vec3 &g) {

            double magnitude = length() * length();

            return cos(angle(g)) * magnitude;
        }*/
        inline double dot(const Vec3 &u, const Vec3 &v) {
            return u.vec[0] * v.vec[0]
                + u.vec[1] * v.vec[1]
                + u.vec[2] * v.vec[2];
        }



        // cross product

        /*inline double crossP(const Vec3 &g) {

            double magnitude = length() * length();

            return sin(angle(g)) * magnitude;
        }*/

        // vector product

        Vec3 vecP(const Vec3 &h, const Vec3 &g){

                   return Vec3(h.vec[1] * g.vec[2] - h.vec[2]*g.vec[1],
                    h.vec[2] * g.vec[0] - h.vec[0]*g.vec[2],
                    h.vec[0] * g.vec[1] - h.vec[1]*g.vec[0]);


        }


#endif
