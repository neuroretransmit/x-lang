#pragma once

#include "ast.h"
#include "../util/collections/list.h"

List* _ast;

void init_parser(char* fname);
void destroy_parser();
List* parse();

