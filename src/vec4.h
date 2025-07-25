#ifndef VEC4
#define VEC4

#include <cmath>
#include <iostream>

class Vec4 {
public:
    double vec[4];

    Vec4() : vec{0,0,0,0} {}
    Vec4(double v0, double v1, double v2, double v3) : vec{v0, v1, v2, v3} {}

    double x() const { return vec[0]; }
    double y() const { return vec[1]; }
    double z() const { return vec[2]; }
    double w() const { return vec[3]; }

    Vec4 operator-() const {
        return Vec4(-vec[0], -vec[1], -vec[2], -vec[3]);
    }

    double& operator[](int i) { return vec[i]; }
    const double& operator[](int i) const { return vec[i]; }

    Vec4& operator+=(const Vec4 &v) {
        for (int i = 0; i < 4; ++i) vec[i] += v.vec[i];
        return *this;
    }

    Vec4& operator*=(double t) {
        for (int i = 0; i < 4; ++i) vec[i] *= t;
        return *this;
    }

    Vec4& operator/=(double t) {
        for (int i = 0; i < 4; ++i) vec[i] /= t;
        return *this;
    }

    double length() const {
        double sum = 0.0;
        for (int i = 0; i < 4; ++i) sum += vec[i] * vec[i];
        return std::sqrt(sum);
    }

    Vec4 getVec() const {
        for (int i = 0; i < 4; ++i) {
            if (std::abs(vec[i]) == INFINITY) {
                std::cerr << "Vector component is infinity!" << std::endl;
                return Vec4(0.0, 0.0, 0.0, 0.0);
            }
        }
        return *this;
    }
};

// Non-member operators

inline Vec4 operator*(const Vec4 &u, const Vec4 &v) {
    return Vec4(u[0] * v[0], u[1] * v[1], u[2] * v[2], u[3] * v[3]);
}

inline Vec4 operator*(double t, const Vec4 &v) {
    return Vec4(t * v[0], t * v[1], t * v[2], t * v[3]);
}

inline Vec4 operator*(const Vec4 &v, double t) {
    return t * v;
}

inline Vec4 operator/(const Vec4& v, double t) {
    return Vec4(v[0] / t, v[1] / t, v[2] / t, v[3] / t);
}

inline Vec4 operator+(const Vec4& a, const Vec4& b) {
    return Vec4(a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]);
}

inline Vec4 operator-(const Vec4& a, const Vec4& b) {
    return Vec4(a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]);
}

inline Vec4 operator-(const Vec4& v, double x) {
    return Vec4(v[0] - x, v[1] - x, v[2] - x, v[3] - x);
}

// Unit vector
inline Vec4 unit_vector(Vec4 v) {
    double len = v.length();
    if (len != 0.0) {
        return v / len;
    } else {
        std::cerr << "Cannot calculate unit vector for zero-length vector." << std::endl;
        return Vec4(0, 0, 0, 0);
    }
}

// Dot product (all 4 components)
inline double dot(const Vec4 &u, const Vec4 &v) {
    double result = 0.0;
    for (int i = 0; i < 4; ++i) {
        result += u.vec[i] * v.vec[i];
    }
    return result;
}

#endif
