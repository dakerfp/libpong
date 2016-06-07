include(FindPkgConfig)
pkg_check_modules(NCURSES ncurses)

if (NCURSES_FOUND)
	add_executable(pong-ncurses
		main.c
	)
	target_link_libraries(pong-ncurses
		ncurses
		libpong
	)
else ()
    message(STATUS "ncurses sample application not being built: ncurses not found")
endif()
