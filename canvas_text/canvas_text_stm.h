#ifndef CANVAS_TEXT_H
#define CANVAS_TEXT_H

#ifdef __cplusplus
    extern "C" {
#endif

#include "canvas.h"
#include "stm_fonts/fonts.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define canvas_text_stm_draw_char_literal(cv, type, font, pixel_foreground, pixel_background, character, x_left, y_top) do {     \
    type storage_foreground = pixel_foreground;                                                                                  \
    type storage_background = pixel_background;                                                                                  \
    canvas_text_stm_draw_char(cv, font, (uint8_t*)&storage_foreground, (uint8_t*)&storage_background, character, x_left, y_top); \
} while (0)

#define canvas_text_stm_draw_string_literal(cv, type, font, pixel_foreground, pixel_background, string, x_left, y_top) do {     \
    type storage_foreground = pixel_foreground;                                                                                 \
    type storage_background = pixel_background;                                                                                 \
    canvas_text_stm_draw_string(cv, font, (uint8_t*)&storage_foreground, (uint8_t*)&storage_background, string, x_left, y_top); \
} while (0)

static inline void canvas_text_stm_draw_char(
    canvas_t *cv,
    sFONT *font,
    uint8_t *pixel_foreground,
    uint8_t *pixel_background,
    char character,
    size_t x_left,
    size_t y_top
)
{
    // Gleaned from the format and STM's drivers
    size_t font_table_index = (character - ' ') * font->Height * ((font->Width + 7) / 8);
    for (size_t dy = 0; dy < font->Height; dy++)
    {
        size_t y = y_top + dy;
        for (size_t dx = 0; dx < font->Width; dx++)
        {
            size_t x = x_left + dx;
            uint8_t *pixel = (font->table[font_table_index] & (0x80 >> (dx & 7))) ? pixel_foreground : pixel_background;
            canvas_set_pixel(cv, pixel, x, y);
            if ((dx & 7) == 7)
            {
                font_table_index++;
            }
        }
        if (font->Width & 7)
        {
            font_table_index++;
        }
    }
}

static inline void canvas_text_stm_draw_string(
    canvas_t *cv,
    sFONT *font,
    uint8_t *pixel_foreground,
    uint8_t *pixel_background,
    char *string,
    size_t x_left,
    size_t y_top
)
{
    size_t len = strlen(string);
    size_t x = x_left;
    size_t y = y_top;
    for (size_t i = 0; i < len; i++)
    {
        canvas_text_stm_draw_char(
            cv,
            font,
            pixel_foreground,
            pixel_background,
            string[i],
            x,
            y
        );

        x += font->Width;
        if ((x + font->Width) > cv->width - 5)
        {
            y += font->Height;
            x = x_left;
        }
    }
}

#ifdef __cplusplus
    }
#endif

#endif
