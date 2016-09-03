#include <stdio.h>

#include "util/collections/fifo_tests.h"
#include "util/collections/list_tests.h"
#include "util/collections/hashmap_tests.h"

int main()
{
	fifo_tests();
	list_tests();
	hashmap_tests();
}
