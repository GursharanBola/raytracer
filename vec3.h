#ifndef VEC3_H
#define VEC3_H

// Some standard libraries
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

class vec3 {
  public:
    double vec[3];

    vec3() : vec{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : vec{e0, e1, e2} {}
    double x() const { return vec[0]; }
    double y() const { return vec[1]; }
    double z() const { return vec[2]; }

    // Lines 19 - 36 are not original code found online
    double operator[](int i) const { return vec[i]; }
    double &operator[](int i) { return vec[i]; }

    vec3 &operator+=(const vec3 &v) {
        vec[0] += v.vec[0];
        vec[1] += v.vec[1];
        vec[2] += v.vec[2];
        return *this;
    }

    vec3 &operator*=(double t) {
        vec[0] *= t;
        vec[1] *= t;
        vec[2] *= t;
        return *this;
    }

    vec3 &operator/=(double t) { return *this *= 1 / t; }

    bool is_equal(const vec3 v1, const vec3 v2) {
        return (v1.vec[0] == v2.vec[0]) && (v1.vec[1] == v2.vec[1]) &&
               (v1.vec[2] == v2.vec[2]);
    }

    double elem_sum(const vec3 v) { return v.vec[0] + v.vec[1] + v.vec[2]; }

    vec3 cross(const vec3 &v1, const vec3 &v2) {
        double i = v1.vec[1] * v2.vec[2] - v1.vec[2] * v2.vec[1];
        double j = v1.vec[2] * v2.vec[0] - v1.vec[0] * v2.vec[2];
        double k = v1.vec[0] * v2.vec[1] - v1.vec[1] * v2.vec[0];
        return vec3(i, j, k);
    }

    // previously implimented version no inline functionality
    // double dot(const vec3 &v1) {
    //     vec3 v = elem_mul(*this, v1);
    //     return elem_sum(v);
    // }

    double length_squared() const {
        return this->vec[0] * this->vec[0] + this->vec[1] * this->vec[1] +
               this->vec[2] * this->vec[2];
    }
    double length() const { return std::sqrt(this->length_squared()); }

    // rotation based on x coming out of the page, y being vertical and z
    // following the right hand rule.
    vec3 rotate_y(double angle) const {
        double c = std::cos(angle);
        double s = std::sin(angle);
        return vec3{vec[0] * c + vec[2] * s, vec[1], -vec[0] * s + vec[2] * c};
    }

    vec3 rotate_x(double angle) const {
        double c = std::cos(angle);
        double s = std::sin(angle);
        return vec3{vec[0], vec[1] * c - vec[2] * s, vec[1] * s + vec[2] * c};
    }
};

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.vec[0] - v.vec[0], u.vec[1] - v.vec[1], u.vec[2] - v.vec[2]);
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.vec[0] + v.vec[0], u.vec[1] + v.vec[1], u.vec[2] + v.vec[2]);
}

inline vec3 operator-(const vec3 &v) {
    return vec3(-v.vec[0], -v.vec[1], -v.vec[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
    return vec3(v.vec[0] * t, v.vec[1] * t, v.vec[2] * t);
}

inline vec3 operator*(double t, const vec3 &v) { return v * t; }

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.vec[0] * v.vec[0], u.vec[1] * v.vec[1], u.vec[2] * v.vec[2]);
}
inline double dot(const vec3 &v1, const vec3 &v2) {
    return v1.vec[0] * v2.vec[0] + v1.vec[1] * v2.vec[1] +
           v1.vec[2] * v2.vec[2];
}

inline vec3 operator/(const vec3 &v, double t) {
    return vec3(v.vec[0] / t, v.vec[1] / t, v.vec[2] / t);
}
inline double distance(const vec3 v1, const vec3 v2) {
    return (v2 - v1).length();
}

inline vec3 unit_vector(const vec3 &v) { return v / v.length(); }

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

inline vec3 random_vec3(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max),
                random_double(min, max));
}
inline vec3 elem_mul(vec3 v1, vec3 v2) {
    return vec3(v1.vec[0] * v2.vec[0], v1.vec[1] * v2.vec[1],
                v1.vec[2] * v2.vec[2]);
}

#endif
