BINDIR := bin
OBJDIR := obj
SRCDIR := src
DOCDIR := doc
LIBDIR := lib
RESDIR := res
TESTDIR := tests
TESTOBJDIR := $(OBJDIR)/tests

BINARY := $(BINDIR)/x-lang
LIBRARY_BINARY := $(LIBDIR)/libx-lang.so
TESTS_BINARY := $(BINDIR)/x-lang-tests

SOURCES := $(shell find src -name **.c)
OBJECTS := $(patsubst src/%.c, obj/%.o, $(SOURCES))

TEST_SOURCES := $(shell find tests -name **.c)
TEST_OBJECTS := $(patsubst tests/%.c, obj/tests/%.o, $(TEST_SOURCES))

INCLUDES := -Isrc

CC := gcc
CSTD := -std=gnu11
WARNINGS := -Wall -Wextra -Werror -fPIC
CFLAGS := -g $(CSTD) $(WARNINGS) $(INCLUDES)


all: $(LIBRARY_BINARY) $(TESTS_BINARY) $(BINARY)

$(BINARY): $(LIBRARY_BINARY) $(OBJECTS) 
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

$(LIBRARY_BINARY): $(filter-out obj/main.o, $(OBJECTS))
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -shared $^ -o $@

$(TESTS_BINARY): $(LIBRARY_BINARY) $(TEST_OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJECTS): $(OBJDIR)%.o: $(SRCDIR)%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJECTS): $(TESTOBJDIR)%.o: $(TESTDIR)%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ 

run-%: $(BINARY)
	valgrind --leak-check=full --show-leak-kinds=all $< $(RESDIR)/$*.x 2>&1 | tee -a LEAK_REPORT.txt

run-tests: $(TESTS_BINARY)
	valgrind --leak-check=full $< 2>&1 | tee -a LEAK_REPORT_TESTS.txt

clean:
	rm -rfv bin obj lib LEAK_REPORT.txt LEAK_REPORT_TESTS.txt
