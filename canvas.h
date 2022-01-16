/** @file      canvas.h
 *  @brief     Canvas
 *
 *  Canvas
 */

#ifndef CANVAS_H
#define CANVAS_H

#ifdef __cplusplus
    #define CANVAS_RESTRICT
    extern "C" {
#else
    #define CANVAS_RESTRICT restrict
#endif

#ifdef DOXYGEN
    #define CANVAS_STATIC_INLINE
    #define CANVAS_FEATURE_TWO_BUFFERS 1
#else
    #define CANVAS_STATIC_INLINE static inline
#endif

#ifndef CANVAS_FEATURE_TWO_BUFFERS
    #define CANVAS_FEATURE_TWO_BUFFERS 1
#endif

#include "vendor/st/fonts.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/**
 * @defgroup BUFFER_API Buffer API
 *
 * @{
 */

CANVAS_STATIC_INLINE void canvas_buffer_set_pixel(
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

CANVAS_STATIC_INLINE void canvas_buffer_fill(
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

CANVAS_STATIC_INLINE void canvas_buffer_rotate_90_cw(
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

CANVAS_STATIC_INLINE void canvas_buffer_rotate_90_ccw(
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

CANVAS_STATIC_INLINE void canvas_buffer_rotate_180(
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

CANVAS_STATIC_INLINE void canvas_buffer_flip_up_down(
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

CANVAS_STATIC_INLINE void canvas_buffer_flip_left_right(
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

CANVAS_STATIC_INLINE void canvas_buffer_fill_rect(
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

CANVAS_STATIC_INLINE void canvas_buffer_draw_horizontal_line(
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

CANVAS_STATIC_INLINE void canvas_buffer_draw_vertical_line(
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

CANVAS_STATIC_INLINE void canvas_buffer_draw_line(
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

CANVAS_STATIC_INLINE size_t canvas_buffer_max_of_3(size_t a, size_t b, size_t c)
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

CANVAS_STATIC_INLINE size_t canvas_buffer_min_of_3(size_t a, size_t b, size_t c)
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

CANVAS_STATIC_INLINE bool canvas_buffer_edge_function(
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

CANVAS_STATIC_INLINE void canvas_buffer_fill_triangle(
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

CANVAS_STATIC_INLINE void canvas_buffer_draw_rect(
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

CANVAS_STATIC_INLINE void canvas_buffer_draw_octants(
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

CANVAS_STATIC_INLINE void canvas_buffer_fill_octants(
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

CANVAS_STATIC_INLINE void canvas_buffer_draw_circle(
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

CANVAS_STATIC_INLINE void canvas_buffer_fill_circle(
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

CANVAS_STATIC_INLINE void canvas_buffer_place_bitmap(
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

CANVAS_STATIC_INLINE void canvas_buffer_extract_bitmap(
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

CANVAS_STATIC_INLINE void canvas_buffer_copy_region(
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

/**
 * @}
 */

/**
 * @defgroup CANVAS_API Canvas API
 *
 * @{
 */

/**
 * Holds information about the canvas.
 *
 * For the canvas to be valid, it must be a return value from @ref canvas_init,
 * and it must have been passed to @ref canvas_set_memory with a pointer to valid memory.
 */
typedef struct canvas_t {
    size_t width;           /**< Width of the canvas, in number of pixels */
    size_t height;          /**< Height of the canvas, in number of pixels */
    size_t pixel_size;      /**< Number of bytes per pixel */
    size_t buffer_size;     /**< Size of the buffer, in bytes. */
    size_t alloc_size;      /**< Number of bytes that must be allocated for the buffer provided in @ref canvas_set_memory */
    uint8_t *buffer;        /**< The buffer that currently holds valid data. */
    #if CANVAS_FEATURE_TWO_BUFFERS
        uint8_t *_temp_buffer;  /**< Internal. A secondary buffer used to hold temporary data during certain actions such as canvas rotations. Exists only if @ref CANVAS_FEATURE_TWO_BUFFERS=1 */
        bool _swapped;          /**< Internal. Whether the primary and secondary buffer have been swapped. Exists only if @ref CANVAS_FEATURE_TWO_BUFFERS=1 */
    #endif
} canvas_t;

#if CANVAS_FEATURE_TWO_BUFFERS
    /**
     * Only intended for internal use.
     *
     * Swap the primary and secondary buffers so that the primary buffer holds
     *
     * @param cv Canvas
     */
    CANVAS_STATIC_INLINE void canvas_swap_buffers(canvas_t *cv)
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

    /**
     * Copy the contents of the primary buffer into the temporary buffer.
     * For testing; should not be required.
     *
     * @param cv Canvas
     * @return nothing
     */
    CANVAS_STATIC_INLINE void canvas_sync_buffers(canvas_t *cv)
    {
        memcpy(cv->_temp_buffer, cv->buffer, cv->buffer_size);
    }
#endif

/**
 * Returns a new canvas where everything has been initialized except the actual memory.
 *
 * @param width       Width of the canvas in pixels.
 * @param height      Height of the canvas in pixels.
 * @param pixel_size  The size of one pixel in memory, in bytes. For example, if your pixels are 3-byte RGB values, pixel_size should be 3.
 *
 * @warning After calling `canvas_t cv = canvas_init(...)`, the application must provide a buffer of size `cv.alloc_size` or larger
 *          by calling `canvas_set_memory(&cv, buffer)`.
 *
 * @note The only supported interactions with the canvas object are the provided API functions, as well as
 *       passing cv.buffer to other functions for display.
 *
 * @return Canvas
 */
CANVAS_STATIC_INLINE canvas_t canvas_init(
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
        #if CANVAS_FEATURE_TWO_BUFFERS
            .alloc_size = buffer_size * 2,
            ._swapped = false
        #else
            .alloc_size = buffer_size
        #endif
    };
}

/**
 * Provide the canvas with a pixel buffer.
 *
 * @param canvas A canvas that was returned from @ref canvas_init
 * @param memory Pointer to a buffer of size `cv.alloc_size` or larger
 *
 * @warning The memory pointed to by `memory` must remain valid for as long as `cv` is in use.
 *          This can be achieved by allocating it statically, globally, or with an allocator such as `malloc`.
 *          If `memory` was allocated on the stack, it will be invalid when the caller returns.
 */
CANVAS_STATIC_INLINE void canvas_set_memory(canvas_t *cv, uint8_t *memory)
{
    cv->buffer = memory;
    #if CANVAS_FEATURE_TWO_BUFFERS
        cv->_temp_buffer = memory + cv->buffer_size;
    #endif
}

/**
 * Set the value of a single pixel.
 *
 * @param canvas Canvas
 * @param pixel  Pixel data for a single pixel, which will be placed into the canvas at the given coordinates.
 * @param x      X-coordinate in the canvas
 * @param y      Y-coordinate in the canvas
 *
 * If the pixel data is representable as an integer literal, consider using @ref canvas_set_pixel_literal to avoid using a dummy variable.
 */
CANVAS_STATIC_INLINE void canvas_set_pixel(canvas_t* CANVAS_RESTRICT cv, const uint8_t* CANVAS_RESTRICT pixel, size_t x, size_t y)
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

/**
 * Draw the edges of a rectangle, 1 pixel wide.
 *
 * @param canvas  Canvas
 * @param pixel   Pixel data for a single pixel, which will be used along the edge of the rectangle
 * @param x_left  X-coordinate of the left side of the rectangle
 * @param x_right X-coordinate of the right side of the rectangle, plus 1.
 * @param y_top   Y-coordinate of the top side of the rectangle
 * @param y_right Y-coordinate of the bottom side of the rectangle, plus 1.
 *
 * If the pixel data is representable as an integer literal, consider using @ref canvas_draw_rect_literal to avoid using a dummy variable.
 */
CANVAS_STATIC_INLINE void canvas_draw_rect(
    canvas_t* CANVAS_RESTRICT cv,
    const uint8_t* CANVAS_RESTRICT pixel,
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

CANVAS_STATIC_INLINE void canvas_draw_horizontal_line(
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

CANVAS_STATIC_INLINE void canvas_draw_vertical_line(
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

CANVAS_STATIC_INLINE void canvas_draw_line(
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

CANVAS_STATIC_INLINE void canvas_fill_rect(
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

CANVAS_STATIC_INLINE void canvas_draw_circle(
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

CANVAS_STATIC_INLINE void canvas_fill_triangle(
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

CANVAS_STATIC_INLINE void canvas_fill_circle(
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

CANVAS_STATIC_INLINE void canvas_place_bitmap(
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

CANVAS_STATIC_INLINE void canvas_extract_bitmap(
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

CANVAS_STATIC_INLINE void canvas_copy_region(
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


/**
 * Fill the entire canvas with a pixel value.
 *
 * @param canvas Canvas
 * @param pixel  Pixel data for a single pixel. The entire canvas will be filled with this pixel
 *
 * If the pixel data is representable as an integer literal, consider using @ref canvas_fill_literal to avoid using a dummy variable.
 */
CANVAS_STATIC_INLINE void canvas_fill(canvas_t* CANVAS_RESTRICT cv, uint8_t* CANVAS_RESTRICT pixel)
{
    canvas_buffer_fill(
        cv->buffer,
        pixel,
        cv->buffer_size,
        cv->pixel_size
    );
}

#if CANVAS_FEATURE_TWO_BUFFERS
    CANVAS_STATIC_INLINE void canvas_rotate_90_cw(canvas_t* cv)
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

    CANVAS_STATIC_INLINE void canvas_rotate_90_ccw(canvas_t* cv)
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

    CANVAS_STATIC_INLINE void canvas_rotate_180(canvas_t* cv)
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

    CANVAS_STATIC_INLINE void canvas_flip_up_down(canvas_t* cv)
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

    CANVAS_STATIC_INLINE void canvas_flip_left_right(canvas_t* cv)
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
#endif

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

/**
 * @}
 */

/**
 * @defgroup LITERAL_MACROS Using literal values as pixels
 *
 * @{
 */

/**
 * Set the value of a single pixel.
 *
 * @param canvas Canvas (type: `canvas_t`)
 * @param type   The type of `pixel` (type: e.g. `uint8_t`, `uint16_t`, or `my_pixel_type`)
 * @param pixel  Pixel data for a single pixel, which will be placed into the canvas at the given coordinates. (type: whatever `type` is)
 * @param x      X-coordinate in the canvas (type: `size_t`)
 * @param y      Y-coordinate in the canvas (type: `size_t`)
 *
 * If the pixel data is already stored in a variable, use @ref canvas_set_pixel.
 */
#define canvas_set_pixel_literal(cv, type, pixel, x, y)              \
do {                                                                 \
    type storage = pixel;                                            \
    size_t offset = (y * (cv)->width + x) * sizeof(type);            \
    memcpy((cv)->buffer + offset, (uint8_t*)&storage, sizeof(type)); \
} while (0)

/**
 * Set the value of the entire canvas.
 *
 * @param canvas Canvas (type: `canvas_t`)
 * @param type   The type of `pixel` (type: e.g. `uint8_t`, `uint16_t`, or `my_pixel_type`)
 * @param pixel  Pixel data for a single pixel. The entire canvas will be filled with this pixel (type: whatever `type` is)
 *
 * If the pixel data is already stored in a variable, use @ref canvas_fill.
 */
#define canvas_fill_literal(cv, type, pixel) \
do {                                         \
    type storage = pixel;                    \
    canvas_fill(cv, (uint8_t*)&storage);     \
} while (0)

#define canvas_fill_rect_literal(cv, type, pixel, x0, x1, y0, y1) \
do {                                                              \
    type storage = pixel;                                         \
    canvas_fill_rect(cv, (uint8_t*)&storage, x0, x1, y0, y1);     \
} while (0)

#define canvas_draw_rect_literal(cv, type, pixel, x0, x1, y0, y1) \
do {                                                              \
    type storage = pixel;                                         \
    canvas_draw_rect(cv, (uint8_t*)&storage, x0, x1, y0, y1);     \
} while (0)

#define canvas_draw_line_literal(cv, type, pixel, x0, x1, y0, y1) \
do {                                                              \
    type storage = pixel;                                         \
    canvas_draw_line(cv, (uint8_t*)&storage, x0, x1, y0, y1);     \
} while (0)

#define canvas_draw_circle_literal(cv, type, pixel, x, y, r) \
do {                                                         \
    type storage = pixel;                                    \
    canvas_draw_circle(cv, (uint8_t*)&storage, x, y, r);     \
} while (0)

#define canvas_fill_circle_literal(cv, type, pixel, x, y, r) \
do {                                                         \
    type storage = pixel;                                    \
    canvas_fill_circle(cv, (uint8_t*)&storage, x, y, r);     \
} while (0)

#define canvas_fill_triangle_literal(cv, type, pixel, x0, x1, x2, y0, y1, y2) \
do {                                                                          \
    type storage = pixel;                                                     \
    canvas_fill_triangle(cv, (uint8_t*)&storage, x0, x1, x2, y0, y1, y2);     \
} while (0)

#define canvas_text_stm_draw_char_literal(cv, type, font, pixel_foreground, pixel_background, character, x_left, y_top)          \
do {                                                                                                                             \
    type storage_foreground = pixel_foreground;                                                                                  \
    type storage_background = pixel_background;                                                                                  \
    canvas_text_stm_draw_char(cv, font, (uint8_t*)&storage_foreground, (uint8_t*)&storage_background, character, x_left, y_top); \
} while (0)

#define canvas_text_stm_draw_string_literal(cv, type, font, pixel_foreground, pixel_background, string, x_left, y_top)          \
do {                                                                                                                            \
    type storage_foreground = pixel_foreground;                                                                                 \
    type storage_background = pixel_background;                                                                                 \
    canvas_text_stm_draw_string(cv, font, (uint8_t*)&storage_foreground, (uint8_t*)&storage_background, string, x_left, y_top); \
} while (0)

/** @} */

#ifdef __cplusplus
    }
#endif

#endif
