### Tools ###
export VALGRIND := valgrind
export CC := cc

### Commands ###
export CLEAN := rm -rfv
export MKDIR := @mkdir -p

### Compiler Options ###
export CSTD := -std=gnu11
export CXXSTD := -std=gnu++14
export WARNINGS := -Wall -Wextra -Werror -fPIC
export INCLUDES := -Isrc
export LIBS := `llvm-config --libs core analysis` -lcurses -ldl -lz
export LDFLAGS := `llvm-config --ldflags`
export CFLAGS := $(CSTD) -ggdb3 $(WARNINGS) $(INCLUDES) -DDEBUG
export CXXFLAGS := $(CXXSTD) -ggdb3 $(WARNINGS) $(INCLUDES) -DDEBUG -rdynamic -pthread
export CC += $(CFLAGS)
export CXX += $(CXXFLAGS)

### Tool Options ###
export VALGRIND_OPTIONS := --leak-check=full
export VALGRIND += $(VALGRIND_OPTIONS)
