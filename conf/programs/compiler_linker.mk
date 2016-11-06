CSTD := -std=gnu11
CXXSTD := -std=gnu++14
WARNINGS := -Wall -Wextra -Werror -fPIC
INCLUDES := -Isrc
LIBS := `llvm-config --libs core analysis` -lcurses -ldl -lz
LDFLAGS := `llvm-config --ldflags`
CFLAGS := $(CSTD) -ggdb3 $(WARNINGS) $(INCLUDES) -DDEBUG
CXXFLAGS := $(CXXSTD) -ggdb3 $(WARNINGS) $(INCLUDES) -DDEBUG -rdynamic -pthread
CC += $(CFLAGS)
CXX += $(CXXFLAGS)
