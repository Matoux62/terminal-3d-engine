#ifndef DEBUG_H
#define DEBUG_H

#include <type.h>

void print_mat4(const char *name, Mat4 M);
void print_vec3(const char *name, Vec3 v);
void print_camera(const char *name, Camera cam);
void print_triangles_object(const char *name, TrianglesObject obj);
void print_triangle(Triangle t);
void print_triangle_image(TriangleImage t);
void print_triangles_object_image(const char *name, TrianglesObjectImage obj);

#endif