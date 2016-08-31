BINARY := lang-x

BINDIR := bin
OBJDIR := obj
SRCDIR := src
DOCDIR := doc

SOURCES := $(shell find src -name **.c)
OBJECTS := $(patsubst src/%.c, obj/%.o, $(SOURCES))

CSTD := -std=gnu11
WARNINGS := -Wall -Wextra -Werror
CFLAGS := -g $(CSTD) $(WARNINGS)

all: $(BINDIR)/$(BINARY)

$(BINDIR)/$(BINARY): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJECTS): $(OBJDIR)%.o: $(SRCDIR)%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rfv bin obj
