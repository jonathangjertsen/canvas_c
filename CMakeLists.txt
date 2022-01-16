add_library(canvas INTERFACE)
target_include_directories(canvas INTERFACE .)

add_library(canvas_text
    canvas_text/stm_fonts/font8.c
    canvas_text/stm_fonts/font12.c
    canvas_text/stm_fonts/font16.c
    canvas_text/stm_fonts/font20.c
    canvas_text/stm_fonts/font24.c
)
target_include_directories(canvas_text PUBLIC canvas_text)
