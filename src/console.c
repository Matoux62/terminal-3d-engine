#include "type.h"
#include "console.h"
#include "config.h"
#include "debug.h"
#include "maths.h"

#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>

static DWORD original_mode;
static HANDLE hStdin, hStdout, hStderr;
COORD console_size = {0};

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

static COORD compute_visible_size(void)
{
    COORD size = {0};
    HWND hwnd = GetConsoleWindow();
    CONSOLE_FONT_INFOEX cfi = { .cbSize = sizeof(cfi) };
    RECT rc;

    if (hwnd && GetClientRect(hwnd, &rc)
        && GetCurrentConsoleFontEx(hStdout, FALSE, &cfi)
        && cfi.dwFontSize.X > 0 && cfi.dwFontSize.Y > 0)
    {
        size.X = (SHORT)((rc.right - rc.left) / cfi.dwFontSize.X);
        size.Y = (SHORT)((rc.bottom - rc.top) / cfi.dwFontSize.Y);
    }

    COORD largest = GetLargestConsoleWindowSize(hStdout);
    if (largest.X > 0 && size.X > largest.X) size.X = largest.X;
    if (largest.Y > 0 && size.Y > largest.Y) size.Y = largest.Y;

    if (size.X <= 0 || size.Y <= 0) {
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(hStdout, &info);
        size.X = info.srWindow.Right  - info.srWindow.Left + 1;
        size.Y = info.srWindow.Bottom - info.srWindow.Top  + 1;
    }
    return size;
}

static void apply_buffer_size(COORD size)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hStdout, &info);

    SHORT cw = info.srWindow.Right  - info.srWindow.Left + 1;
    SHORT ch = info.srWindow.Bottom - info.srWindow.Top  + 1;
    SMALL_RECT shrink = { 0, 0,
        (SHORT)((cw < size.X ? cw : size.X) - 1),
        (SHORT)((ch < size.Y ? ch : size.Y) - 1) };
    SetConsoleWindowInfo(hStdout, TRUE, &shrink);
    SetConsoleScreenBufferSize(hStdout, size);
    SMALL_RECT full = { 0, 0, (SHORT)(size.X - 1), (SHORT)(size.Y - 1) };
    SetConsoleWindowInfo(hStdout, TRUE, &full);
}

int update_console(void)
{
    COORD size = compute_visible_size();

    if (size.X == console_size.X && size.Y == console_size.Y)
        return 0;

    apply_buffer_size(size);
    console_size = size;

    fputs("\x1b[2J\x1b[H", stdout);
    fflush(stdout);
    return 1;
}

void restore_console(void)
{
    fputs("\x1b[0m\x1b[?25h\x1b[2J\x1b[3J\x1b[H", stdout);
    fflush(stdout);
    SetConsoleMode(hStdout, original_mode);
}

void init_console(void)
{
    atexit(restore_console);

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hStderr = GetStdHandle(STD_ERROR_HANDLE);
    
    GetConsoleMode(hStdout, &original_mode);
    SetConsoleMode(hStdout, original_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    fputs("\x1b[2J\x1b[3J\x1b[H\x1b[?25l", stdout);
    fflush(stdout);
}

void display(FrameBuffer *buffer)
/*Affiche le framebuffer dans la console en ecrivant directement dans les cellules, sans deplacer le curseur*/
{
    DWORD written;
    COORD origin = {0, 0};
    WriteConsoleOutputCharacterA(hStdout, buffer->buffer, buffer->width * buffer->height, origin, &written);
}