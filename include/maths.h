#ifndef MATHS_H
#define MATHS_H

#include "type.h"

float cosf_deg(float angle_in_deg);

float sinf_deg(float angle_in_deg);

Vec3 mat4_mul_vec3(Mat4 M, Vec3 v);

#endif