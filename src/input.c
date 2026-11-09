#include "type.h"
#include "input.h"
#include "config.h"
#include "debug.h"
#include "maths.h"

#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>

static DWORD original_mode;
static CONSOLE_SCREEN_BUFFER_INFO original_csbi;

void input_update(int k, Camera *cam)
{
	switch(k)
	{
		case KEY_UP:
			cam->pitch += TURN_SPEED;
			if (cam->pitch > PITCH_MAX) cam->pitch = PITCH_MAX;
        	break;
		case KEY_DOWN:
			cam->pitch -= TURN_SPEED;
			if (cam->pitch < -PITCH_MAX) cam->pitch = -PITCH_MAX;
			break;
		case KEY_LEFT:
			cam->yaw -= TURN_SPEED;
			if (cam->yaw < 0.0f) cam->yaw += 360.0f;
			break;
		case KEY_RIGHT:
			cam->yaw += TURN_SPEED;
			if (cam->yaw >= 360.0f) cam->yaw -= 360.0f;
            break;
		case 'z':
            cam->pos.z += cosf_deg(cam->yaw) * DISTANCE_UNIT;
            cam->pos.x += sinf_deg(cam->yaw) * DISTANCE_UNIT;
            break;
		case 'q':
            cam->pos.x -= cosf_deg(cam->yaw) * DISTANCE_UNIT;
            cam->pos.z += sinf_deg(cam->yaw) * DISTANCE_UNIT;
            break;
		case 's':
            cam->pos.z -= cosf_deg(cam->yaw) * DISTANCE_UNIT;
            cam->pos.x -= sinf_deg(cam->yaw) * DISTANCE_UNIT;
            break;
		case 'd':
            cam->pos.x += cosf_deg(cam->yaw) * DISTANCE_UNIT;
            cam->pos.z -= sinf_deg(cam->yaw) * DISTANCE_UNIT;
            break;
	}
}

void raw_off(void)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    fputs("\x1b[0m\x1b[?25h\x1b[2J\x1b[3J\x1b[H", stdout);
    fflush(stdout);
    SetConsoleMode(h, original_mode);

    SHORT w = original_csbi.srWindow.Right  - original_csbi.srWindow.Left + 1;
    SHORT hh = original_csbi.srWindow.Bottom - original_csbi.srWindow.Top  + 1;
    SMALL_RECT shrink = {0, 0, (w < WIDTH ? w : WIDTH) - 1,
                               (hh < HEIGTH ? hh : HEIGTH) - 1};
    SetConsoleWindowInfo(h, TRUE, &shrink);
    SetConsoleScreenBufferSize(h, original_csbi.dwSize);
    SetConsoleWindowInfo(h, TRUE, &original_csbi.srWindow);
}

void raw_on(HANDLE h)
{
    GetConsoleMode(h, &original_mode);
    atexit(raw_off);
    SetConsoleMode(h, original_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    GetConsoleScreenBufferInfo(h, &original_csbi);
    SHORT w = original_csbi.srWindow.Right  - original_csbi.srWindow.Left + 1;
    SHORT hh = original_csbi.srWindow.Bottom - original_csbi.srWindow.Top  + 1;
    SMALL_RECT shrink = {0, 0, (w < WIDTH ? w : WIDTH) - 1,
                               (hh < HEIGTH ? hh : HEIGTH) - 1};
    SetConsoleWindowInfo(h, TRUE, &shrink);
    COORD size = {WIDTH, HEIGTH};
    SetConsoleScreenBufferSize(h, size);
    SMALL_RECT win = {0, 0, WIDTH - 1, HEIGTH - 1};
    SetConsoleWindowInfo(h, TRUE, &win);

    fputs("\x1b[2J\x1b[3J\x1b[H\x1b[?25l", stdout);
    fflush(stdout);
}


int read_key(void)
{
    int c = _getch();

    if (c == 0 || c == 224) {
        switch (_getch()) {
            case 72: return KEY_UP;
            case 80: return KEY_DOWN;
            case 77: return KEY_RIGHT;
            case 75: return KEY_LEFT;
        }
        return 0;
    }
    return c;
}
