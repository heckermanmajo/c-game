//
// Created by mo on 05.05.26.
//

#ifndef CG_TEXT_BUTTON_H
#define CG_TEXT_BUTTON_H

#include <raylib.h>

int text_button(
    const char* text,
    const int x,
    const int y,
    const int font_size,
    const Color color
)
{
    const int width = MeasureText(text, font_size);
    const Rectangle rect = (Rectangle){
        .x = x,
        .y = y,
        .width = (float) width,
        .height = (float) font_size
    };
    Color red = color;
    const int is_hovered = CheckCollisionPointRec(GetMousePosition(), rect);
    if (is_hovered)
    {
        red.g = 200;
        red.b = 200;
    }
    DrawText(text, (int) rect.x, (int) rect.y, font_size, red);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && is_hovered)
    {
        return 1;
    }
    return 0;
}

#endif //CG_TEXT_BUTTON_H
