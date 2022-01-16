#ifndef CANVAS_H
#define CANVAS_H

#ifdef __cplusplus
    extern "C" {
#endif

#include "canvas_buffer.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define canvas_set_pixel_literal(cv, type, pixel, x, y) do {         \
    type storage = pixel;                                            \
    size_t offset = (y * (cv)->width + x) * sizeof(type);            \
    memcpy((cv)->buffer + offset, (uint8_t*)&storage, sizeof(type)); \
} while (0)

#define canvas_fill_literal(cv, type, pixel) do { \
    type storage = pixel;                         \
    canvas_fill(cv, (uint8_t*)&storage);          \
} while (0)

#define canvas_fill_rect_literal(cv, type, pixel, x0, x1, y0, y1) do { \
    type storage = pixel;                                              \
    canvas_fill_rect(cv, (uint8_t*)&storage, x0, x1, y0, y1);          \
} while (0)

#define canvas_draw_rect_literal(cv, type, pixel, x0, x1, y0, y1) do { \
    type storage = pixel;                                              \
    canvas_draw_rect(cv, (uint8_t*)&storage, x0, x1, y0, y1);          \
} while (0)

#define canvas_draw_line_literal(cv, type, pixel, x0, x1, y0, y1) do { \
    type storage = pixel;                                              \
    canvas_draw_line(cv, (uint8_t*)&storage, x0, x1, y0, y1);          \
} while (0)

#define canvas_draw_circle_literal(cv, type, pixel, x, y, r) do { \
    type storage = pixel;                                         \
    canvas_draw_circle(cv, (uint8_t*)&storage, x, y, r);          \
} while (0)

#define canvas_fill_circle_literal(cv, type, pixel, x, y, r) do { \
    type storage = pixel;                                         \
    canvas_fill_circle(cv, (uint8_t*)&storage, x, y, r);          \
} while (0)

#define canvas_fill_triangle_literal(cv, type, pixel, x0, x1, x2, y0, y1, y2) do { \
    type storage = pixel;                                                          \
    canvas_fill_triangle(cv, (uint8_t*)&storage, x0, x1, x2, y0, y1, y2);          \
} while (0)

typedef struct canvas_t {
    size_t width;
    size_t height;
    size_t pixel_size;
    size_t buffer_size;
    size_t alloc_size;
    uint8_t *buffer;
    uint8_t *_temp_buffer;
    bool _swapped;
} canvas_t;

static inline void canvas_swap_buffers(canvas_t *cv)
{
    size_t buffer_size = cv->buffer_size;
    if (cv->_swapped)
    {
        cv->buffer -= buffer_size;
        cv->_temp_buffer += buffer_size;
    }
    else
    {
        cv->buffer += buffer_size;
        cv->_temp_buffer -= buffer_size;
    }
    cv->_swapped = !cv->_swapped;
}

static inline void canvas_sync_buffers(canvas_t *cv)
{
    memcpy(cv->_temp_buffer, cv->buffer, cv->buffer_size);
}

static inline canvas_t canvas_init(
    size_t width,
    size_t height,
    size_t pixel_size
)
{
    size_t buffer_size = width * height * pixel_size;

    return (canvas_t){
        .width = width,
        .height = height,
        .pixel_size = pixel_size,
        .buffer_size = buffer_size,
        .alloc_size = buffer_size * 2,
        ._swapped = false
    };
}

static inline void canvas_set_memory(canvas_t *cv, uint8_t *memory)
{
    cv->buffer = memory;
    cv->_temp_buffer = memory + cv->buffer_size;
}

static inline void canvas_set_pixel(canvas_t* CANVAS_RESTRICT cv, uint8_t* CANVAS_RESTRICT pixel, size_t x, size_t y)
{
    canvas_buffer_set_pixel(
        cv->buffer,
        pixel,
        cv->pixel_size,
        cv->width,
        x,
        y
    );
}

static inline void canvas_draw_rect(
    canvas_t* CANVAS_RESTRICT cv,
    uint8_t* CANVAS_RESTRICT pixel,
    size_t x_left,
    size_t x_right,
    size_t y_top,
    size_t y_bottom
)
{
    canvas_buffer_draw_rect(
        cv->buffer,
        pixel,
        cv->pixel_size,
        cv->width,
        x_left,
        x_right,
        y_top,
        y_bottom
    );
}

static inline void canvas_draw_horizontal_line(
    canvas_t* CANVAS_RESTRICT cv,
    uint8_t* CANVAS_RESTRICT pixel,
    size_t x_left,
    size_t x_right,
    size_t y
)
{
    canvas_buffer_draw_horizontal_line(
        cv->buffer,
        pixel,
        cv->pixel_size,
        cv->width,
        x_left,
        x_right,
        y
    );
}

static inline void canvas_draw_vertical_line(
    canvas_t* CANVAS_RESTRICT cv,
    uint8_t* CANVAS_RESTRICT pixel,
    size_t x,
    size_t y_top,
    size_t y_bottom
)
{
    canvas_buffer_draw_vertical_line(
        cv->buffer,
        pixel,
        cv->pixel_size,
        cv->width,
        x,
        y_top,
        y_bottom
    );
}

static inline void canvas_draw_line(
    canvas_t* CANVAS_RESTRICT cv,
    uint8_t* CANVAS_RESTRICT pixel,
    size_t x_left,
    size_t x_right,
    size_t y_top,
    size_t y_bottom
)
{
    canvas_buffer_draw_line(
        cv->buffer,
        pixel,
        cv->pixel_size,
        cv->width,
        x_left,
        x_right,
        y_top,
        y_bottom
    );
}

static inline void canvas_fill_rect(
    canvas_t* CANVAS_RESTRICT cv,
    uint8_t* CANVAS_RESTRICT pixel,
    size_t x_left,
    size_t x_right,
    size_t y_top,
    size_t y_bottom
)
{
    canvas_buffer_fill_rect(
        cv->buffer,
        pixel,
        cv->pixel_size,
        cv->width,
        x_left,
        x_right,
        y_top,
        y_bottom
    );
}

static inline void canvas_draw_circle(
    canvas_t* CANVAS_RESTRICT cv,
    uint8_t* CANVAS_RESTRICT pixel,
    size_t x_center,
    size_t y_center,
    size_t radius
)
{
    canvas_buffer_draw_circle(
        cv->buffer,
        pixel,
        cv->pixel_size,
        cv->width,
        x_center,
        y_center,
        radius
    );
}

static inline void canvas_fill_triangle(
    canvas_t* CANVAS_RESTRICT cv,
    const uint8_t* CANVAS_RESTRICT pixel,
    size_t x_0,
    size_t x_1,
    size_t x_2,
    size_t y_0,
    size_t y_1,
    size_t y_2
)
{
    canvas_buffer_fill_triangle(
        cv->buffer,
        pixel,
        cv->pixel_size,
        cv->width,
        x_0,
        x_1,
        x_2,
        y_0,
        y_1,
        y_2
    );
}

static inline void canvas_fill_circle(
    canvas_t* CANVAS_RESTRICT cv,
    uint8_t* CANVAS_RESTRICT pixel,
    size_t x_center,
    size_t y_center,
    size_t radius
)
{
    canvas_buffer_fill_circle(
        cv->buffer,
        pixel,
        cv->pixel_size,
        cv->width,
        x_center,
        y_center,
        radius
    );
}

static inline void canvas_place_bitmap(
    canvas_t* CANVAS_RESTRICT cv,
    const uint8_t* CANVAS_RESTRICT bitmap,
    size_t x_left,
    size_t x_right,
    size_t y_top,
    size_t y_bottom
)
{
    canvas_buffer_place_bitmap(
        cv->buffer,
        bitmap,
        cv->pixel_size,
        cv->width,
        x_left,
        x_right,
        y_top,
        y_bottom
    );
}

static inline void canvas_extract_bitmap(
    const canvas_t* CANVAS_RESTRICT cv,
    uint8_t* CANVAS_RESTRICT bitmap,
    size_t x_left,
    size_t x_right,
    size_t y_top,
    size_t y_bottom
)
{
    canvas_buffer_extract_bitmap(
        cv->buffer,
        bitmap,
        cv->pixel_size,
        cv->width,
        x_left,
        x_right,
        y_top,
        y_bottom
    );
}

static inline void canvas_copy_region(
    canvas_t* CANVAS_RESTRICT cv,
    uint8_t* CANVAS_RESTRICT bitmap,
    size_t source_x_left,
    size_t source_x_right,
    size_t source_y_top,
    size_t source_y_bottom,
    size_t dest_x_left,
    size_t dest_y_top
)
{
    canvas_buffer_copy_region(
        cv->buffer,
        bitmap,
        cv->pixel_size,
        cv->width,
        source_x_left,
        source_x_right,
        source_y_top,
        source_y_bottom,
        dest_x_left,
        dest_y_top
    );
}

static inline void canvas_fill(canvas_t* CANVAS_RESTRICT cv, uint8_t* CANVAS_RESTRICT pixel)
{
    canvas_buffer_fill(
        cv->buffer,
        pixel,
        cv->buffer_size,
        cv->pixel_size
    );
}

static inline void canvas_rotate_90_cw(canvas_t* cv)
{
    canvas_buffer_rotate_90_cw(
        cv->_temp_buffer,
        cv->buffer,
        cv->buffer_size,
        cv->pixel_size,
        cv->width,
        cv->height
    );
    canvas_swap_buffers(cv);
}

static inline void canvas_rotate_90_ccw(canvas_t* cv)
{
    canvas_buffer_rotate_90_ccw(
        cv->_temp_buffer,
        cv->buffer,
        cv->buffer_size,
        cv->pixel_size,
        cv->width,
        cv->height
    );
    canvas_swap_buffers(cv);
}

static inline void canvas_rotate_180(canvas_t* cv)
{
    canvas_buffer_rotate_180(
        cv->_temp_buffer,
        cv->buffer,
        cv->buffer_size,
        cv->pixel_size,
        cv->width,
        cv->height
    );
    canvas_swap_buffers(cv);
}

static inline void canvas_flip_up_down(canvas_t* cv)
{
    canvas_buffer_flip_up_down(
        cv->_temp_buffer,
        cv->buffer,
        cv->buffer_size,
        cv->pixel_size,
        cv->width,
        cv->height
    );
    canvas_swap_buffers(cv);
}

static inline void canvas_flip_left_right(canvas_t* cv)
{
    canvas_buffer_flip_left_right(
        cv->_temp_buffer,
        cv->buffer,
        cv->buffer_size,
        cv->pixel_size,
        cv->width,
        cv->height
    );
    canvas_swap_buffers(cv);
}

#ifdef __cplusplus
    }
#endif

#endif
