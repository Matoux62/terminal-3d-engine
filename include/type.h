#ifndef TYPE_H
#define TYPE_H

#include "config.h"

typedef struct {int x, y;} Resolution;
typedef struct {float x, y;} Vec2;
typedef struct {float x, y, z;} Vec3;
typedef struct {float x, y, z, w;} Vec4;
typedef struct {float m[4][4];} Mat4;
typedef struct {int a, b, c;} TriangleIndex;
typedef struct {Vec3 a, b, c;} Triangle;
typedef struct {Vec2 a, b, c;} TriangleImage;
typedef struct {Vec3 pos; float yaw; float pitch; float fov;} Camera;
typedef struct{
    Vec3		*points;
	int 		point_count;
	TriangleIndex	*triangles;
	int			triangle_count;} Model;
typedef struct {Vec3 pos; Vec3 rotation; Vec3 scale; const Model* model;} Object;
typedef struct {int max_object; int current_nb_object; Object *object_list[MAX_OBJECTS];} Scene;
typedef struct {int height; int width;} TermSize;
typedef struct {int triangle_count; Triangle *triangles;} TrianglesObject;
typedef struct {int triangle_count; TriangleImage *triangles;} TrianglesObjectImage;
typedef struct {char buffer[HEIGTH*WIDTH];} FrameBuffer;

#endif