add_library(canvas INTERFACE)
target_include_directories(canvas INTERFACE .)

add_library(canvas_st_fonts
    vendor/st/font8.c
    vendor/st/font12.c
    vendor/st/font16.c
    vendor/st/font20.c
    vendor/st/font24.c
)
