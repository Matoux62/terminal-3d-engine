#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "type.h"

int fb_resize(FrameBuffer *fb, int width, int height);

void fb_free(FrameBuffer *fb);

#endif