#include "maths.h"
#include "type.h"

#include <math.h>

float cosf_deg(float angle_in_deg)
{
    return cosf(angle_in_deg  * (float)M_PI / 180.0f);
}

float sinf_deg(float angle_in_deg)
{
    return sinf(angle_in_deg  * (float)M_PI / 180.0f);
}

Vec3 mat4_mul_vec3(Mat4 M, Vec3 v)
{
    Vec3 res;
    res.x = M.m[0][0]*v.x + M.m[0][1]*v.y + M.m[0][2]*v.z + M.m[0][3];
    res.y = M.m[1][0]*v.x + M.m[1][1]*v.y + M.m[1][2]*v.z + M.m[1][3];
    res.z = M.m[2][0]*v.x + M.m[2][1]*v.y + M.m[2][2]*v.z + M.m[2][3];
    return res;
}
