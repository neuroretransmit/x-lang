#include <stdio.h>

#include "util/collections/fifo_tests.h"
#include "util/collections/list_tests.h"
#include "util/collections/hashmap_tests.h"

static void collections_tests()
{
	fifo_tests();
	list_tests();
	hashmap_tests();
}

static void util_tests()
{
	collections_tests();
}

int main()
{
	util_tests();
}
