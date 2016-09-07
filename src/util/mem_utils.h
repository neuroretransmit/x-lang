#pragma once

#include <stdlib.h>

static inline void destroy(void* ptr)
{
	free(ptr);
	ptr = NULL;
}
