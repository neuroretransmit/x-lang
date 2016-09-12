#include "helper.h"

#include <stdlib.h>


TestObj* mock_test_object()
{
	TestObj* obj = malloc(sizeof(TestObj));
	obj->x = rand(),
	obj->y = rand(),
	obj->z = rand();
	
	return obj;
}

void dummy_destructor(__attribute__((__unused__)) void* fake_data)
{
	return;
}