#ifndef CANVAS_BUFFER_H
#define CANVAS_BUFFER_H

#ifdef __cplusplus
    #define CANVAS_RESTRICT
    extern "C" {
#else
    #define CANVAS_RESTRICT restrict
#endif

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

static inline void canvas_buffer_set_pixel(
    uint8_t* CANVAS_RESTRICT buffer,
    const uint8_t* CANVAS_RESTRICT pixel,
    size_t pixel_size,
    size_t width,
    size_t x,
    size_t y
)
{
    size_t offset = (y * width + x) * pixel_size;
    memcpy(buffer + offset, pixel, pixel_size);
}

static inline void canvas_buffer_fill(
    uint8_t* CANVAS_RESTRICT buffer,
    const uint8_t* CANVAS_RESTRICT pixel,
    size_t memory_size,
    size_t pixel_size
)
{
    for (size_t i = 0; i < memory_size; i += pixel_size)
    {
        memcpy(buffer + i, pixel, pixel_size);
    }
}

static inline void canvas_buffer_rotate_90_cw(
    uint8_t* CANVAS_RESTRICT destination,
    const uint8_t* CANVAS_RESTRICT source,
    size_t memory_size,
    size_t pixel_size,
    size_t width,
    size_t height
)
{
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            canvas_buffer_set_pixel(
                destination,
                source + (y * width + x) * pixel_size,
                pixel_size,
                width,
                height - y,
                x
            );
        }
    }
}

static inline void canvas_buffer_rotate_90_ccw(
    uint8_t* CANVAS_RESTRICT destination,
    const uint8_t* CANVAS_RESTRICT source,
    size_t memory_size,
    size_t pixel_size,
    size_t width,
    size_t height
)
{
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            canvas_buffer_set_pixel(
                destination,
                source + (y * width + x) * pixel_size,
                pixel_size,
                width,
                y,
                width - x
            );
        }
    }
}

static inline void canvas_buffer_rotate_180(
    uint8_t* CANVAS_RESTRICT destination,
    const uint8_t* CANVAS_RESTRICT source,
    size_t memory_size,
    size_t pixel_size,
    size_t width,
    size_t height
)
{
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            canvas_buffer_set_pixel(
                destination,
                source + (y * width + x) * pixel_size,
                pixel_size,
                width,
                width - x,
                height - y
            );
        }
    }
}

static inline void canvas_buffer_flip_up_down(
    uint8_t* CANVAS_RESTRICT destination,
    const uint8_t* CANVAS_RESTRICT source,
    size_t memory_size,
    size_t pixel_size,
    size_t width,
    size_t height
)
{
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            canvas_buffer_set_pixel(
                destination,
                source + (y * width + x) * pixel_size,
                pixel_size,
                width,
                x,
                height - y
            );
        }
    }
}

static inline void canvas_buffer_flip_left_right(
    uint8_t* CANVAS_RESTRICT destination,
    const uint8_t* CANVAS_RESTRICT source,
    size_t memory_size,
    size_t pixel_size,
    size_t width,
    size_t height
)
{
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            canvas_buffer_set_pixel(
                destination,
                source + (y * width + x) * pixel_size,
                pixel_size,
                width,
                width - x,
                y
            );
        }
    }
}

static inline void canvas_buffer_fill_rect(
    uint8_t* CANVAS_RESTRICT buffer,
    const uint8_t* CANVAS_RESTRICT pixel,
    size_t pixel_size,
    size_t width,
    size_t x_left,
    size_t x_right,
    size_t y_top,
    size_t y_bottom
)
{
    size_t width_rect = x_right - x_left;
    size_t offset_top_left = (y_top * width + x_left) * pixel_size;
    size_t offset_bottom_right = (y_bottom * width + x_right) * pixel_size;
    size_t offset_from_end_of_row_to_start_of_next = (width - width_rect) * pixel_size;
    size_t offset = offset_top_left;
    while (offset < offset_bottom_right)
    {
        for (size_t i = 0; i < width_rect; i++)
        {
            memcpy(buffer + offset, pixel, pixel_size);
            offset += pixel_size;
        }
        offset += offset_from_end_of_row_to_start_of_next;
    }
}

static inline void canvas_buffer_draw_horizontal_line(
    uint8_t* CANVAS_RESTRICT buffer,
    const uint8_t* CANVAS_RESTRICT pixel,
    size_t pixel_size,
    size_t width,
    size_t x_left,
    size_t x_right,
    size_t y
)
{
    for (size_t x = x_left; x < x_right; x++)
    {
        canvas_buffer_set_pixel(buffer, pixel, pixel_size, width, x, y);
    }
}

static inline void canvas_buffer_draw_vertical_line(
    uint8_t* CANVAS_RESTRICT buffer,
    const uint8_t* CANVAS_RESTRICT pixel,
    size_t pixel_size,
    size_t width,
    size_t x,
    size_t y_top,
    size_t y_bottom
)
{
    for (size_t y = y_top; y < y_bottom; y++)
    {
        canvas_buffer_set_pixel(buffer, pixel, pixel_size, width, x, y);
    }
}

static inline void canvas_buffer_draw_line(
    uint8_t* CANVAS_RESTRICT buffer,
    const uint8_t* CANVAS_RESTRICT pixel,
    size_t pixel_size,
    size_t width,
    size_t x_left,
    size_t x_right,
    size_t y_top,
    size_t y_bottom
)
{
    // Bressenham's line drawing algorithm

    // Each coordinate, as integers
    int x_left_i = (int)x_left;
    int x_right_i = (int)x_right;
    int y_top_i = (int)y_top;
    int y_bottom_i = (int)y_bottom;

    // Absolute difference in each direction
    int x_diff = x_right_i - x_left_i;
    int y_diff = y_bottom_i - y_top_i;

    // Absolute difference in each direction
    int x_diff_abs = x_diff > 0 ? x_diff : -x_diff;
    int y_diff_abs = y_diff > 0 ? y_diff : -y_diff;

    // Start and stop points, where x_stop > x_start and y_stop > y_start
    int x_start;
    int x_stop;
    int y_start;
    int y_stop;

    if (y_diff_abs < x_diff_abs)
    {
        if (x_left_i > x_right_i)
        {
            x_start = x_right;
            x_stop = x_left;
            y_start = y_bottom;
            y_stop = y_top;
        }
        else
        {
            x_start = x_left;
            x_stop = x_right;
            y_start = y_top;
            y_stop = y_bottom;
        }
        int y_step = y_diff > 0 ? 1 : -1;
        int error = 2 * y_diff_abs - x_diff;
        int y = y_start;
        for (int x = x_start; x < x_stop; x++)
        {
            canvas_buffer_set_pixel(buffer, pixel, pixel_size, width, x, y);
            if (error > 0)
            {
                y += y_step;
                error += 2 * (y_diff_abs - x_diff);
            }
            else
            {
                error += 2 * y_diff_abs;
            }
        }
    }
    else
    {
        if (y_top_i > y_bottom_i)
        {
            x_start = x_right;
            x_stop = x_left;
            y_start = y_bottom;
            y_stop = y_top;
        }
        else
        {
            x_start = x_left;
            x_stop = x_right;
            y_start = y_top;
            y_stop = y_bottom;
        }
        int x_step = x_diff > 0 ? 1 : -1;
        int error = 2 * x_diff_abs - y_diff;
        int x = y_start;
        for (int y = y_start; y < y_stop; y++)
        {
            canvas_buffer_set_pixel(buffer, pixel, pixel_size, width, x, y);
            if (error > 0)
            {
                x += x_step;
                error += 2 * (x_diff_abs - y_diff);
            }
            else
            {
                error += 2 * x_diff_abs;
            }
        }
    }
}

static inline size_t canvas_buffer_max_of_3(size_t a, size_t b, size_t c)
{
    size_t r = a;
    if (b > r)
    {
        r = b;
    }
    if (c > r)
    {
        r = c;
    }
    return r;
}

static inline size_t canvas_buffer_min_of_3(size_t a, size_t b, size_t c)
{
    size_t r = a;
    if (b < r)
    {
        r = b;
    }
    if (c < r)
    {
        r = c;
    }
    return r;
}

static inline bool canvas_buffer_edge_function(
    int x_a,
    int x_b,
    int x_c,
    int y_a,
    int y_b,
    int y_c
)
{
    return ((x_c - x_a) * (y_b - y_a) - (y_c - y_a) * (x_b - x_a)) < 0;
}

static inline void canvas_buffer_fill_triangle(
    uint8_t* CANVAS_RESTRICT buffer,
    const uint8_t* CANVAS_RESTRICT pixel,
    size_t pixel_size,
    size_t width,
    size_t x_0,
    size_t x_1,
    size_t x_2,
    size_t y_0,
    size_t y_1,
    size_t y_2
)
{
    size_t x_max = canvas_buffer_max_of_3(x_0, x_1, x_2);
    size_t x_min = canvas_buffer_min_of_3(x_0, x_1, x_2);
    size_t y_max = canvas_buffer_max_of_3(y_0, y_1, y_2);
    size_t y_min = canvas_buffer_min_of_3(y_0, y_1, y_2);

    for (size_t y = y_min; y < y_max; y++)
    {
        for (size_t x = x_min; x < x_max; x++)
        {
            if (
                   canvas_buffer_edge_function(x_0, x_1, x, y_0, y_1, y)
                && canvas_buffer_edge_function(x_1, x_2, x, y_1, y_2, y)
                && canvas_buffer_edge_function(x_2, x_0, x, y_2, y_0, y)
            )
            {
                canvas_buffer_set_pixel(buffer, pixel, pixel_size, width, x, y);
            }
        }
    }
}

static inline void canvas_buffer_draw_rect(
    uint8_t* CANVAS_RESTRICT buffer,
    const uint8_t* CANVAS_RESTRICT pixel,
    size_t pixel_size,
    size_t width,
    size_t x_left,
    size_t x_right,
    size_t y_top,
    size_t y_bottom
)
{
    canvas_buffer_draw_horizontal_line(buffer, pixel, pixel_size, width, x_left, x_right-1, y_top);
    canvas_buffer_draw_horizontal_line(buffer, pixel, pixel_size, width, x_left, x_right-1, y_bottom-1);
    canvas_buffer_draw_vertical_line(buffer, pixel, pixel_size, width, x_left, y_top, y_bottom-1);
    canvas_buffer_draw_vertical_line(buffer, pixel, pixel_size, width, x_right-1, y_top, y_bottom-1);
}

static inline void canvas_buffer_draw_octants(
    uint8_t* CANVAS_RESTRICT buffer,
    const uint8_t* CANVAS_RESTRICT pixel,
    size_t pixel_size,
    size_t width,
    size_t x_center,
    size_t y_center,
    size_t x_diff,
    size_t y_diff
)
{
    size_t coordinates[8][2] = {
        { x_center + x_diff, y_center + y_diff },
        { x_center + x_diff, y_center - y_diff },
        { x_center + y_diff, y_center + x_diff },
        { x_center + y_diff, y_center - x_diff },
        { x_center - x_diff, y_center + y_diff },
        { x_center - x_diff, y_center - y_diff },
        { x_center - y_diff, y_center + x_diff },
        { x_center - y_diff, y_center - x_diff },
    };

    for (int i = 0; i < 8; i++)
    {
        canvas_buffer_set_pixel(
            buffer,
            pixel,
            pixel_size,
            width,
            coordinates[i][0],
            coordinates[i][1]
        );
    }
}

static inline void canvas_buffer_fill_octants(
    uint8_t* CANVAS_RESTRICT buffer,
    const uint8_t* CANVAS_RESTRICT pixel,
    size_t pixel_size,
    size_t width,
    size_t x_center,
    size_t y_center,
    size_t x_diff,
    size_t y_diff
)
{
    size_t coordinates[8][2] = {
        { x_center + x_diff, y_center + y_diff },
        { x_center + x_diff, y_center - y_diff },
        { x_center + y_diff, y_center + x_diff },
        { x_center + y_diff, y_center - x_diff },
        { x_center - x_diff, y_center + y_diff },
        { x_center - x_diff, y_center - y_diff },
        { x_center - y_diff, y_center + x_diff },
        { x_center - y_diff, y_center - x_diff },
    };

    for (int i = 0; i < 8; i++)
    {
        size_t x = coordinates[i][0];
        size_t x_max = x > x_center ? x : x_center;
        size_t x_min = x < x_center ? x : x_center;
        for (size_t x_i = x_min; x_i <= x_max; x_i++)
        {
            canvas_buffer_set_pixel(
                buffer,
                pixel,
                pixel_size,
                width,
                x_i,
                coordinates[i][1]
            );
        }
    }
}

static inline void canvas_buffer_draw_circle(
    uint8_t* CANVAS_RESTRICT buffer,
    const uint8_t* CANVAS_RESTRICT pixel,
    size_t pixel_size,
    size_t width,
    size_t x_center,
    size_t y_center,
    size_t radius
)
{
    // Algorithm due to Stefan Gustavson, "An Efficient Circle Drawing Algorithm", 2003-08-20
    size_t x = 0;
    size_t y = radius;
    int d = 5 - 4 * radius;
    int da = 12;
    int db = 20 - 8 * radius;
    while (x < y)
    {
        canvas_buffer_draw_octants(buffer, pixel, pixel_size, width, x_center, y_center, x, y);
        if (d < 0)
        {
            d += da;
            db += 8;
        }
        else
        {
            y--;
            d += db;
            db += 16;
        }
        x++;
        da += 8;
    }

    // The original algorithm doesn't fill the corners; do so here
    size_t radius_div_sqrt2 = radius * 70 / 99;
    canvas_buffer_draw_octants(buffer, pixel, pixel_size, width, x_center, y_center, radius_div_sqrt2, radius_div_sqrt2);
}

static inline void canvas_buffer_fill_circle(
    uint8_t* CANVAS_RESTRICT buffer,
    const uint8_t* CANVAS_RESTRICT pixel,
    size_t pixel_size,
    size_t width,
    size_t x_center,
    size_t y_center,
    size_t radius
)
{
    // Algorithm due to Stefan Gustavson, "An Efficient Circle Drawing Algorithm", 2003-08-20
    size_t x = 0;
    size_t y = radius;
    int d = 5 - 4 * radius;
    int da = 12;
    int db = 20 - 8 * radius;
    while (x < y)
    {
        canvas_buffer_fill_octants(buffer, pixel, pixel_size, width, x_center, y_center, x, y);
        if (d < 0)
        {
            d += da;
            db += 8;
        }
        else
        {
            y--;
            d += db;
            db += 16;
        }
        x++;
        da += 8;
    }

    // The original algorithm doesn't fill the "corners; do so here"
    size_t radius_div_sqrt2 = radius * 70 / 99;
    canvas_buffer_fill_octants(buffer, pixel, pixel_size, width, x_center, y_center, radius_div_sqrt2, radius_div_sqrt2);
}

static inline void canvas_buffer_place_bitmap(
    uint8_t* CANVAS_RESTRICT buffer,
    const uint8_t* CANVAS_RESTRICT bitmap,
    size_t pixel_size,
    size_t width,
    size_t x_left,
    size_t x_right,
    size_t y_top,
    size_t y_bottom
)
{
    size_t width_rect = x_right - x_left;

    size_t offset_top_left = (y_top * width + x_left) * pixel_size;
    size_t offset_bottom_right = (y_bottom * width + x_right) * pixel_size;
    size_t offset_from_end_of_row_to_start_of_next = (width - width_rect) * pixel_size;

    size_t offset = offset_top_left;
    size_t offset_bitmap = 0;
    while (offset < offset_bottom_right)
    {
        for (size_t i = 0; i < width_rect; i++)
        {
            memcpy(buffer + offset, bitmap + offset_bitmap, pixel_size);
            offset += pixel_size;
            offset_bitmap += pixel_size;
        }
        offset += offset_from_end_of_row_to_start_of_next;
    }
}

static inline void canvas_buffer_extract_bitmap(
    const uint8_t* CANVAS_RESTRICT buffer,
    uint8_t* CANVAS_RESTRICT bitmap,
    size_t pixel_size,
    size_t width,
    size_t x_left,
    size_t x_right,
    size_t y_top,
    size_t y_bottom
)
{
    size_t width_rect = x_right - x_left;

    size_t offset_top_left = (y_top * width + x_left) * pixel_size;
    size_t offset_bottom_right = (y_bottom * width + x_right) * pixel_size;
    size_t offset_from_end_of_row_to_start_of_next = (width - width_rect) * pixel_size;

    size_t offset = offset_top_left;
    size_t offset_bitmap = 0;
    while (offset < offset_bottom_right)
    {
        for (size_t i = 0; i < width_rect; i++)
        {
            memcpy(bitmap + offset_bitmap, buffer + offset, pixel_size);
            offset += pixel_size;
            offset_bitmap += pixel_size;
        }
        offset += offset_from_end_of_row_to_start_of_next;
    }
}

static inline void canvas_buffer_copy_region(
    uint8_t* CANVAS_RESTRICT buffer,
    uint8_t* CANVAS_RESTRICT temporary,
    size_t pixel_size,
    size_t width,
    size_t source_x_left,
    size_t source_x_right,
    size_t source_y_top,
    size_t source_y_bottom,
    size_t dest_x_left,
    size_t dest_y_top
)
{
    canvas_buffer_extract_bitmap(
        buffer,
        temporary,
        pixel_size,
        width,
        source_x_left,
        source_x_right,
        source_y_top,
        source_y_bottom
    );
    canvas_buffer_place_bitmap(
        buffer,
        temporary,
        pixel_size,
        width,
        dest_x_left,
        dest_x_left + source_x_right - source_x_left,
        dest_y_top,
        dest_y_top + source_y_bottom - source_y_top
    );
}

#ifdef __cplusplus
    }
#endif

#endif
