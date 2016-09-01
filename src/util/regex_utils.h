#pragma once

#include <stdbool.h>
#include <regex.h>

int compile_regex(regex_t* r, const char* regex_text);
bool match_regex(regex_t* r, const char* to_match);

