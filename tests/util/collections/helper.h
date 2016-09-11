#pragma once

#define MATCH (0)

typedef struct {
	int x, y, z;
} TestObj;

TestObj* mock_test_object();

void dummy_destructor(void* fake_data);