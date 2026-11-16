#ifndef CONSOLE_H
#define CONSOLE_H

#include "type.h"

#include <windows.h>

extern COORD console_size;

enum Key { KEY_UP = 256, KEY_DOWN, KEY_RIGHT, KEY_LEFT };

void input_update(int k, Camera *cam);

int read_key(void);

void restore_console(void);

void init_console(void);

int update_console(void);

#endif