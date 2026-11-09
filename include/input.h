#ifndef INPUT_H
#define INPUT_H

#include "type.h"

#include <windows.h>

enum Key { KEY_UP = 256, KEY_DOWN, KEY_RIGHT, KEY_LEFT };

void input_update(int k, Camera *cam);

void raw_off(void);

void raw_on(HANDLE h);

int read_key(void);

#endif