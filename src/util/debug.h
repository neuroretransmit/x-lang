#pragma once

#include <errno.h>

#include <util/log.h>

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }
