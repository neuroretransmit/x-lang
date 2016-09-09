#pragma once

#include "../util/collections/list.h"

List* _ast;
List* root_construct;

void init_parser(char* fname);
void destroy_parser();
void parse();

