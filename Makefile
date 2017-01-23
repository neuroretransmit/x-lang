include conf/directories.mk
include conf/programs/compiler_linker.mk
include conf/programs/debug.mk
include conf/programs/utility.mk

BINARY := $(BINDIR)/x-lang
LIBRARY_BINARY := $(LIBDIR)/libx-lang.so
TESTS_BINARY := $(BINDIR)/x-lang-tests

SOURCES := $(shell find src -name '**.c')
TEST_SOURCES := $(shell find tests -name '**.c')

OBJECTS := $(patsubst src/%.c, obj/%.o, $(SOURCES))
TEST_OBJECTS := $(patsubst tests/%.c, obj/tests/%.o, $(TEST_SOURCES))

all: $(TESTS_BINARY) $(BINARY) docs

$(BINARY): $(LIBRARY_BINARY) $(OBJECTS) 
	$(MKDIR) $(@D)
	$(CXX) $^ -o $@ $(LIBS) $(LDFLAGS)

$(LIBRARY_BINARY): $(filter-out obj/main.o obj/arguments.o, $(OBJECTS))
	$(MKDIR) $(@D)
	$(CXX) `llvm-config --cxxflags --ldflags` -shared $^ -o $@ $(LIBS)

$(TESTS_BINARY): $(LIBRARY_BINARY) $(TEST_OBJECTS)
	$(MKDIR) $(@D)
	$(CC) $^ -o $@

$(OBJECTS): $(OBJDIR)%.o: $(SRCDIR)%.c
	$(MKDIR) $(@D)
	$(CC) -c $< -o $@

$(TEST_OBJECTS): $(TESTOBJDIR)%.o: $(TESTDIR)%.c
	$(MKDIR) $(@D)
	$(CC) -c $< -o $@ 

docs: Doxyfile
	$(info Generating documentation...)
	@doxygen $< &>/dev/null

run-%: $(BINARY)
	$(MKDIR) $(LOGDIR)
	$(VALGRIND) $< $(RESDIR)/$*.x --ast --ir --bitcode bin/worked.bc --asm bin/worked.S -o bin/$* 2>&1 | tee -a $(LOGDIR)/leak-report.txt

run-tests: $(TESTS_BINARY)
	$(MKDIR) $(LOGDIR)
	$(VALGRIND) $< 2>&1 | tee -a $(LOGDIR)/leak-report-tests.txt

clean-logs:
	$(CLEAN) $(LOGDIR)
	
clean: clean-logs
	$(CLEAN) $(BINDIR) $(OBJDIR) $(LIBDIR) $(DOXYGENDIRS)

dist-clean: clean-logs clean

print-%  : ; @echo $* = $($*)
