#include "type.h"
#include "debug.h"

#include <stdio.h>

void print_mat4(const char *name, Mat4 M)
{
	printf("%s =\n", name);
	for (int i = 0; i < 4; i++) {
		printf("| ");
		for (int j = 0; j < 4; j++) {
			printf("%8.3f ", M.m[i][j]);
		}
		printf("|\n");
	}
}

void print_vec3(const char *name, Vec3 v) {
    printf("%s = (%.3f, %.3f, %.3f)\n", name, v.x, v.y, v.z);
}

void print_camera(const char *name, Camera cam) {
    printf("%s:\n", name);
    printf("  pos   = (%.3f, %.3f, %.3f)\n", cam.pos.x, cam.pos.y, cam.pos.z);
    printf("  yaw   = %.2f\n", cam.yaw);
    printf("  pitch = %.2f\n", cam.pitch);
    printf("  fov   = %.2f\n", cam.fov);
}

void print_triangle(Triangle t)
{
    printf("(%.3f, %.3f, %.3f) (%.3f, %.3f, %.3f) (%.3f, %.3f, %.3f)\n",
           t.a.x, t.a.y, t.a.z,
           t.b.x, t.b.y, t.b.z,
           t.c.x, t.c.y, t.c.z);
}

void print_triangle_image(TriangleImage t)
{
    printf("(%.3f, %.3f) (%.3f, %.3f) (%.3f, %.3f)\n",
           t.a.x, t.a.y,
           t.b.x, t.b.y,
           t.c.x, t.c.y);
}

void print_triangles_object(const char *name, TrianglesObject obj)
{
    printf("%s: %d triangle(s)\n", name, obj.triangle_count);
    for (int i = 0; i < obj.triangle_count; i++) {
        printf("  [%d] ", i);
        print_triangle(obj.triangles[i]);
    }
}

void print_triangles_object_image(const char *name, TrianglesObjectImage obj)
{
    printf("%s: %d triangle(s)\n", name, obj.triangle_count);
    for (int i = 0; i < obj.triangle_count; i++) {
        printf("  [%d] ", i);
        print_triangle_image(obj.triangles[i]);
    }
}