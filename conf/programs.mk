### Tools ###
export VALGRIND := valgrind
export CC := cc

### Commands ###
export CLEAN := rm -rfv
export MKDIR := @mkdir -p

### Compiler Options ###
export CSTD := -std=gnu11
export WARNINGS := -Wall -Wextra -Werror -fPIC
export INCLUDES := -Isrc
export CFLAGS := -g $(CSTD) $(WARNINGS) $(INCLUDES)
export CC += $(CFLAGS)

### Tool Options ###
export VALGRIND_OPTIONS := --leak-check=full
export VALGRIND += $(VALGRIND_OPTIONS)
