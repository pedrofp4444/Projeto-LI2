# START COPYRIGHT NOTICE

#    Copyright 2023 Hélder Gomes, Humberto Gomes, Mariana Rocha, Pedro Pereira
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.

# END COPYRIGHT NOTICE

# START CONFIGURATION

CC              := gcc
CFLAGS          := -std=c99 -Wall -Wextra -Werror -pedantic
LIBS            := -lm -lcurses
DEBUG_CFLAGS    := -g
RELEASE_CFLAGS  := -O2

OBJDIR          := obj
BUILDDIR        := build
EXE_NAME        := roguelite
DOCSDIR         := docs

define Doxyfile

	INPUT                  = include src README.md
	RECURSIVE              = YES
	EXTRACT_ALL            = YES
	FILE_PATTERNS          = *.h *.c

	PROJECT_NAME           = $(EXE_NAME)
	USE_MDFILE_AS_MAINPAGE = README.md

	OUTPUT_DIRECTORY       = $(DOCSDIR)
	GENERATE_HTML          = YES
	GENERATE_LATEX         = NO

endef

# END CONFIGURATION

# START MAKEFILE RULES

export Doxyfile

SHELL := bash -O globstar # Dependency on bash for recursive wildcards

SOURCES = $(shell ls src/**/*.c)
HEADERS = $(shell ls include/**/*.h)
OBJECTS = $(patsubst src/%.c, $(OBJDIR)/%.o, $(SOURCES))

ifeq ($(DEBUG), 1)
	CFLAGS += ${DEBUG_CFLAGS}
else
	CFLAGS += ${RELEASE_CFLAGS}
endif

default: $(BUILDDIR)/$(EXE_NAME)

$(OBJDIR)/%.o: src/%.c $(HEADERS) $(OBJDIRS)
	@mkdir -p $(shell dirname $@)
	${CC} -c -o $@ $< ${CFLAGS} ${DEFINES} -Iinclude

$(BUILDDIR)/$(EXE_NAME): $(OBJECTS)
	@mkdir -p $(BUILDDIR)
	$(CC) ${LIBS} -o $@ $^

$(DOCSDIR): $(SOURCES) $(HEADERS) README.md
	echo "$$Doxyfile" | doxygen -

clean:
	@rm -r $(OBJDIR)   > /dev/null 2>&1 ||:
	@rm -r $(BUILDDIR) > /dev/null 2>&1 ||:
	@rm -r $(DOCSDIR)  > /dev/null 2>&1 ||:

# END MAKEFILE RULES

