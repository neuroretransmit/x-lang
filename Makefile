BINDIR := bin
OBJDIR := obj
SRCDIR := src
DOCDIR := doc
RESDIR := res
TESTDIR := tests
TESTOBJDIR := obj/tests

BINARY := $(BINDIR)/x-lang
LIBRARY := $(BINDIR)/libx-lang.so
TESTS_BINARY := $(BINDIR)/x-lang-tests

SOURCES := $(shell find src -name **.c)
OBJECTS := $(patsubst src/%.c, obj/%.o, $(SOURCES))

TEST_SOURCES := $(shell find tests -name **.c)
TEST_OBJECTS := $(patsubst tests/%.c, obj/tests/%.o, $(TEST_SOURCES))

CC := gcc
CSTD := -std=gnu11
WARNINGS := -Wall -Wextra -Werror -fPIC
CFLAGS := -g $(CSTD) $(WARNINGS)

all: $(BINARY) $(TESTS_BINARY) $(LIBRARY)

$(BINARY): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@

$(LIBRARY): $(filter-out obj/main.o, $(OBJECTS))
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -shared -o $@ $^

$(TESTS_BINARY): $(TEST_OBJECTS) $(LIBRARY)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJECTS): $(OBJDIR)%.o: $(SRCDIR)%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJECTS): $(TESTOBJDIR)%.o: $(TESTDIR)%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -Isrc -c $< -o $@

run-%: $(BINARY)
	valgrind --leak-check=full $< $(RESDIR)/$*.x 2>&1 | tee -a LEAK_REPORT.txt

run-tests: $(TESTS_BINARY)
	valgrind --leak-check=full $< 2>&1 | tee -a LEAK_REPORT_TESTS.txt

clean:
	rm -rfv bin obj LEAK_REPORT.txt
