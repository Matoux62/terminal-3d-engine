#include "type.h"

#include <stdlib.h>
#include <string.h>

int fb_resize(FrameBuffer *fb, int width, int height)
{
    size_t n = (size_t)width * height;
    char *p = realloc(fb->buffer, n);
    if (!p) return 0;

    fb->buffer = p;
    fb->width  = width;
    fb->height = height;
    memset(fb->buffer, ' ', n);
    return 1;
}

void fb_free(FrameBuffer *fb)
{
    free(fb->buffer);
    fb->buffer = NULL;
    fb->width = fb->height = 0;
}