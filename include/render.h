#ifndef RENDER_H
#define RENDER_H

#include "type.h"
#include "config.h"

void make_view_matrix(Camera *cam, Mat4 *ViewMatrix);

TrianglesObject make_object(Object *obj, Mat4 *ProjectMatrix);

TrianglesObjectImage project_object(TrianglesObject *triangles_object);

void render(Scene *scene, Camera *cam, FrameBuffer *buffer);

void swap_buffer(FrameBuffer **A, FrameBuffer **B);

void display(FrameBuffer *buffer, HANDLE h);


#endif