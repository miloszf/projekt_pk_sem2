#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "settings.h"

void draw(struct Point* size);
void draw_rect(struct Point* size, const struct RectCharSet* char_set);

#endif