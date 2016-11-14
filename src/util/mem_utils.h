#pragma once

#include <string.h>
#include <stdlib.h>

static inline void destroy(void* ptr)
{
	free(ptr);
	ptr = NULL;
}

static inline void* memdup(void* ptr, size_t size)
{
	void* _tmp = malloc(size);
	memcpy(_tmp, ptr, size);
	return _tmp;
}
