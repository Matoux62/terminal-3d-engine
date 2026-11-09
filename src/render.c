#include "type.h"
#include "maths.h"
#include "debug.h"
#include "config.h"

#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

Mat4 ViewMatrix;
Mat4 ProjectMatrix;

TrianglesObject make_object(Object *obj)
/*Premiere etape du rendu, cette fonction prend un objet et renvoi une liste de triangle Vec3 qui represente l'objet place au bon endroit*/
{
    float cx = cosf_deg(obj->rotation.x);
    float cy = cosf_deg(obj->rotation.y);
    float cz = cosf_deg(obj->rotation.z);
    float sx = sinf_deg(obj->rotation.x);
    float sy = sinf_deg(obj->rotation.y);
    float sz = sinf_deg(obj->rotation.z);
    float Sx = obj->scale.x;
    float Sy = obj->scale.y;
    float Sz = obj->scale.z;
    float Px = obj->pos.x;
    float Py = obj->pos.y;
    float Pz = obj->pos.z;

    ProjectMatrix = (Mat4) {
        {
            { Sx * (cy * cz),                 Sy * (sx * sy * cz + cx * sz),  Sz * (sx * sz - cx * sy * cz),  Px },
            { Sx * (-cy * sz),                Sy * (cx * cz - sx * sy * sz),  Sz * (cx * sy * sz + sx * cz),  Py },
            { Sx * (sy),                      Sy * (-sx * cy),                Sz * (cx * cy),                 Pz },
            { 0.0f,                           0.0f,                           0.0f,                           1.0f }
        }
    };

    TrianglesObject res;
    res.triangle_count = obj->model->triangle_count;
    res.triangles = malloc(sizeof(Triangle) * res.triangle_count);

    for(int i = 0; i < obj->model->triangle_count; i++)
    {
        TriangleIndex t = obj->model->triangles[i];

        Vec3 p1 = obj->model->points[t.a];
        Vec3 p2 = obj->model->points[t.b];
        Vec3 p3 = obj->model->points[t.c];

        p1 = mat4_mul_vec3(ProjectMatrix, p1);
        p2 = mat4_mul_vec3(ProjectMatrix, p2);
        p3 = mat4_mul_vec3(ProjectMatrix, p3);

        res.triangles[i] = (Triangle){p1, p2, p3};
    }

    return res;
}

TrianglesObject view_transform(TrianglesObject *triangles_object, Camera *cam)
/*Deuxieme etape du rendu, cette fonction prend un liste de triangles Vec3 et applique a chacun la view matrix, elle renvoi une liste de triangles Vec3 representant lobjet exprime dans le plan de la camera*/
{
    float c1 = cosf_deg(cam->yaw);
	float s1 = sinf_deg(cam->yaw);
	float c2 = cosf_deg(cam->pitch);
	float s2 = sinf_deg(cam->pitch);
	float tx1 = -(c1 * cam->pos.x - s1 * cam->pos.z);
	float ty1 = -cam->pos.y;
	float tz1 = -(s1 * cam->pos.x + c1 * cam->pos.z);
	float tx2 = tx1;
	float ty2 = c2 * ty1 - s2 * tz1;
	float tz2 = s2 * ty1 + c2 * tz1;

	ViewMatrix = (Mat4){
		{
			{  c1,       0,   -s1,      tx2 },
			{ -s2 * s1,  c2,  -s2 * c1, ty2 },
			{  c2 * s1,  s2,   c2 * c1, tz2 },
			{  0,        0,    0,       1   }
		}
	};

    for (int i = 0; i < triangles_object->triangle_count; i++)
        {
            Triangle *t = &triangles_object->triangles[i];

            t->a = mat4_mul_vec3(ViewMatrix, t->a);
            t->b = mat4_mul_vec3(ViewMatrix, t->b);
            t->c = mat4_mul_vec3(ViewMatrix, t->c);
        }

    return *triangles_object;
}

TrianglesObjectImage project(TrianglesObject *triangles_object)
/*Troisieme etape du rendu, cette fonctione prend une liste de triangle Vec3 et project le triangle tous ses point sont visible, elle renvoi donc une liste de triangle Vec2 ou None si aucun triangle n'est visible*/
{
    TrianglesObjectImage res;
    res.triangle_count = 0;
    res.triangles = malloc(sizeof(TriangleImage) * triangles_object->triangle_count);

    float t = sinf_deg(FOV / 2) / cosf_deg(FOV / 2);
    float aspect = (float)WIDTH / (HEIGTH * 2.0);
    float tx = t * aspect;
    float ty = t;

    for (int i = 0; i < triangles_object->triangle_count; i++)
    {
        Triangle triangle = triangles_object->triangles[i];

        if (triangle.a.z < 0.1 || triangle.b.z < 0.1 || triangle.c.z < 0.1)
            continue;

        Vec2 a = {(triangle.a.x / (triangle.a.z * tx) + 1.0f) * 0.5f * WIDTH,
                  (1.0f - triangle.a.y / (triangle.a.z * ty)) * 0.5f * HEIGTH};
        Vec2 b = {(triangle.b.x / (triangle.b.z * tx) + 1.0f) * 0.5f * WIDTH,
                  (1.0f - triangle.b.y / (triangle.b.z * ty)) * 0.5f * HEIGTH};
        Vec2 c = {(triangle.c.x / (triangle.c.z * tx) + 1.0f) * 0.5f * WIDTH,
                  (1.0f - triangle.c.y / (triangle.c.z * ty)) * 0.5f * HEIGTH};

        res.triangles[res.triangle_count] = (TriangleImage){a, b, c};
        res.triangle_count++;
    }

    return res;
}

void rasterization(TrianglesObjectImage *triangle_image, FrameBuffer *buffer)
/*Dernier etape du rendu, cette fonction rempli les pixel a l'ecran s'ils sont contenus dans un triangle, elle ne renvoi rien*/
{
    for (int i = 0; i < triangle_image->triangle_count; i++)
    {
        Vec2 a = triangle_image->triangles[i].a;
        Vec2 b = triangle_image->triangles[i].b;
        Vec2 c = triangle_image->triangles[i].c;

        float area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        if (area == 0.0f)
            continue;

        int min_x = (int)fmaxf(0.0f,         floorf(fminf(a.x, fminf(b.x, c.x))));
        int max_x = (int)fminf(WIDTH - 1,    ceilf (fmaxf(a.x, fmaxf(b.x, c.x))));
        int min_y = (int)fmaxf(0.0f,         floorf(fminf(a.y, fminf(b.y, c.y))));
        int max_y = (int)fminf(HEIGTH - 1,   ceilf (fmaxf(a.y, fmaxf(b.y, c.y))));

        for (int y = min_y; y <= max_y; y++)
        {
            for (int x = min_x; x <= max_x; x++)
            {
                float px = x + 0.5f;
                float py = y + 0.5f;

                float w0 = (b.x - a.x) * (py - a.y) - (b.y - a.y) * (px - a.x);  // arete AB
                float w1 = (c.x - b.x) * (py - b.y) - (c.y - b.y) * (px - b.x);  // arete BC
                float w2 = (a.x - c.x) * (py - c.y) - (a.y - c.y) * (px - c.x);  // arete CA

                if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0))
                    buffer->buffer[y * WIDTH + x] = '#';
            }
        }
    }
}

void swap_buffer(FrameBuffer **A, FrameBuffer **B)
{
    FrameBuffer *tmp = *A;
    *A = *B;
    *B = tmp;
}

void display(FrameBuffer *buffer, HANDLE h)
/*Affiche le framebuffer dans la console en ecrivant directement dans les cellules, sans deplacer le curseur*/
{
    DWORD written;
    COORD origin = {0, 0};
    WriteConsoleOutputCharacterA(h, buffer->buffer, WIDTH * HEIGTH, origin, &written);
}

void render(Scene *scene, Camera *cam, FrameBuffer *buffer)
/*Chef d'orchestre du rendu : vide le buffer, puis pour chaque objet de la scene
  applique model -> view -> projection -> rasterisation*/
{
    memset(buffer->buffer, ' ', WIDTH * HEIGTH);

    for (int i = 0; i < scene->current_nb_object; i++)
    {
        Object *obj = scene->object_list[i];

        TrianglesObject triangles = make_object(obj);            // espace objet -> monde
        view_transform(&triangles, cam);                         // monde -> camera
        TrianglesObjectImage image = project(&triangles);        // camera -> ecran;
        rasterization(&image, buffer);                           // ecran -> pixels

        free(triangles.triangles);
        free(image.triangles);
    }
}
