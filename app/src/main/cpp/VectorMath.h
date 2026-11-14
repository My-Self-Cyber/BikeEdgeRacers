#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#include <cmath>
#include <cstring>

struct Vec3 {
    float x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& o) const { return Vec3(x + o.x, y + o.y, z + o.z); }
    Vec3 operator-(const Vec3& o) const { return Vec3(x - o.x, y - o.y, z - o.z); }
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
    float length() const { return std::sqrt(x * x + y * y + z * z); }
};

struct Mat4 {
    float m[16];

    Mat4() {
        std::memset(m, 0, sizeof(m));
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }

    static Mat4 perspective(float fovRad, float aspect, float nearZ, float farZ) {
        Mat4 res;
        std::memset(res.m, 0, sizeof(res.m));
        float tanHalfFov = std::tan(fovRad / 2.0f);
        res.m[0] = 1.0f / (aspect * tanHalfFov);
        res.m[5] = 1.0f / tanHalfFov;
        res.m[10] = -(farZ + nearZ) / (farZ - nearZ);
        res.m[11] = -1.0f;
        res.m[14] = -(2.0f * farZ * nearZ) / (farZ - nearZ);
        return res;
    }

    static Mat4 lookAt(const Vec3& eye, const Vec3& target, const Vec3& up) {
        Vec3 f = target - eye;
        float fl = f.length();
        if (fl > 0.0001f) f = f * (1.0f / fl);

        // Cross product up x f
        Vec3 s(up.y * f.z - up.z * f.y, up.z * f.x - up.x * f.z, up.x * f.y - up.y * f.x);
        float sl = s.length();
        if (sl > 0.0001f) s = s * (1.0f / sl);

        // Cross product f x s
        Vec3 u(f.y * s.z - f.z * s.y, f.z * s.x - f.x * s.z, f.x * s.y - f.y * s.x);

        Mat4 res;
        res.m[0] = s.x; res.m[4] = s.y; res.m[8]  = s.z; res.m[12] = -(s.x * eye.x + s.y * eye.y + s.z * eye.z);
        res.m[1] = u.x; res.m[5] = u.y; res.m[9]  = u.z; res.m[13] = -(u.x * eye.x + u.y * eye.y + u.z * eye.z);
        res.m[2] = -f.x;res.m[6] = -f.y;res.m[10] = -f.z;res.m[14] = (f.x * eye.x + f.y * eye.y + f.z * eye.z);
        res.m[3] = 0.0f;res.m[7] = 0.0f;res.m[11] = 0.0f;res.m[15] = 1.0f;
        return res;
    }
};

#endif // VECTOR_MATH_H
