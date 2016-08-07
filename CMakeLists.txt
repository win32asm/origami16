cmake_minimum_required(VERSION 3.5)
project(icfpc2016)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
        /usr/include/gtk-3.0
        /usr/include/at-spi2-atk/2.0
        /usr/include/at-spi-2.0
        /usr/include/dbus-1.0
        /usr/lib/x86_64-linux-gnu/dbus-1.0/include
        /usr/include/gtk-3.0
        /usr/include/gio-unix-2.0/
        /usr/include/cairo
        /usr/include/pango-1.0
        /usr/include/harfbuzz
        /usr/include/pango-1.0
        /usr/include/atk-1.0
        /usr/include/cairo
        /usr/include/pixman-1
        /usr/include/freetype2
        /usr/include/libpng16
        /usr/include/gdk-pixbuf-2.0
        /usr/include/libpng16
        /usr/include/glib-2.0
        /usr/lib/x86_64-linux-gnu/glib-2.0/include
)

find_library(pthread gtk-3 gdk-3 pangocairo-1.0 pango-1.0 atk-1.0 cairo-gobject cairo gdk_pixbuf-2.0 gio-2.0 gobject-2.0 glib-2.0 gmp gmpxx)

set(SOURCE_FILES main.cpp Skeleton.cpp Skeleton.h)

add_executable(icfpc2016xx ${SOURCE_FILES})
target_link_libraries(icfpc2016xx pthread gtk-3 gdk-3 pangocairo-1.0 pango-1.0 atk-1.0 cairo-gobject cairo gdk_pixbuf-2.0 gio-2.0 gobject-2.0 glib-2.0 gmp gmpxx)

