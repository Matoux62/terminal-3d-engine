#include "type.h"
#include "input.h"
#include "render.h"
#include "debug.h"
#include "model.h"

#include <stdio.h>
#include <conio.h> 
#include <windows.h>

int main(void) {
	Camera camera = {{0.0f, 0.0f, 0.0f}, 0.0f, 0.0f, 0.0f};


    Object test_objects[MAX_OBJECTS];
    test_objects[0] = (Object){.pos = {0.0f, 0.0f, 10.0f},.rotation = {0.0f, 0.0f, 0.0f},.scale = {2.0f, 2.0f, 2.0f},.model = &DetailedCylinder};
    test_objects[1] = (Object){.pos = {-4.0f, -1.0f, 8.0f},.rotation = {0.0f, 45.0f, 0.0f},.scale = {1.5f, 3.0f, 1.5f},.model = &TriangularPrism};
    test_objects[2] = (Object){.pos = {4.0f, 0.0f, 12.0f},.rotation = {0.0f, 0.0f, 0.0f},.scale = {2.0f, 2.5f, 2.0f},.model = &Pyramid};
    test_objects[3] = (Object){.pos = {0.0f, 3.0f, 15.0f},.rotation = {30.0f, 45.0f, 0.0f},.scale = {1.5f, 1.5f, 1.5f},.model = &Octahedron};
    test_objects[4] = (Object){.pos = {-3.0f, 2.0f, 11.0f},.rotation = {0.0f, 90.0f, 0.0f},.scale = {2.0f, 2.0f, 2.0f},.model = &Wedge};
    test_objects[5] = (Object){.pos = {3.0f, -2.0f, 9.0f},.rotation = {0.0f, 0.0f, 0.0f},.scale = {3.0f, 0.5f, 3.0f},.model = &FlatCube};
    Scene scene;
    scene.max_object = MAX_OBJECTS;
    scene.current_nb_object = 6;
    for (int i = 0; i < 6; i++) {
        scene.object_list[i] = &test_objects[i];
    }

    FrameBuffer bufs[2];
    FrameBuffer *front = &bufs[0], *back = &bufs[1];
    memset(front->buffer, ' ', sizeof front->buffer);
    memset(back->buffer, ' ', sizeof back->buffer);

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    raw_on(h);

    while (1)
    {
        if (_kbhit())
        {
            int k = read_key();
            if (k == 27) break;
            input_update(k, &camera);
            
        }

        render(&scene, &camera, back);
        swap_buffer(&front, &back);
        display(front, h);

        Sleep(33);
    }
}