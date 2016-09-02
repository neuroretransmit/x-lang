#include "file_utils.h"

#include <stdbool.h>
#include <unistd.h>

bool file_exists(const char* fname)
{
	return (access(fname, F_OK) != -1) ? true : false;
}

