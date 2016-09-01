BINARY := x-lang

BINDIR := bin
OBJDIR := obj
SRCDIR := src
DOCDIR := doc
RESDIR := res

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

run-%: $(BINDIR)/$(BINARY)
	valgrind --leak-check=full $(BINDIR)/$(BINARY) $(RESDIR)/$*.x 2>&1 | tee -a LEAK_REPORT.txt

clean:
	rm -rfv bin obj LEAK_REPORT.txt
