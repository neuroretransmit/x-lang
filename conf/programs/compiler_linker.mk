CSTD := -std=gnu11
CXXSTD := -std=gnu++14
WARNINGS := -Wall -Wextra -Werror -fPIC
INCLUDES := -Isrc
LIBS := `llvm-config --libs core executionengine mcjit analysis native bitwriter --system-libs` 
LDFLAGS := `llvm-config --ldflags`
CFLAGS := $(CSTD) -ggdb3 $(WARNINGS) $(INCLUDES) -DDEBUG -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS
CXXFLAGS := $(CXXSTD) -ggdb3 $(WARNINGS) $(INCLUDES) -DDEBUG -rdynamic -pthread
CC += $(CFLAGS)
CXX += $(CXXFLAGS)
