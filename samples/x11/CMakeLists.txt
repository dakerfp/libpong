include(FindPkgConfig)
pkg_check_modules(X11 x11)

if (X11_FOUND)
    add_executable(pong-x11
        main.c
    )
    target_link_libraries(pong-x11
        X11
        pong
    )
else ()
    message(STATUS "X11 sample application not being built: x11 not found")
endif()
