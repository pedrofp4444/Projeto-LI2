#!/bin/bash

# Checks for memory leaks in the program
# Must be run form the root of the repository

cat <<EOF > /tmp/leak_suppression.supp
{
	ignore_ncurses_leaks
	Memcheck:Leak
	match-leak-kinds:all
	...
	obj:*lib*curses*
}

{
	ignore_load_deps_leaks
	Memcheck:Leak
	match-leak-kinds:all
	...
	fun:load_deps
}

EOF

valgrind --leak-check=full --show-leak-kinds=all --leak-resolution=high \
	--suppressions=/tmp/leak_suppression.supp ./build/roguelite

