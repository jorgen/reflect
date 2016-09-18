#include "test_struct.h"
#include <stdio.h>

const char *json = "{ \"test1\" : \"stringVal\" }";

int main()
{
    bool parsed;
    TestStruct test = JR::parseData<TestStruct>(json, sizeof json, parsed);

    if (parsed)
        fprintf(stderr, "Parsed with value %s\n", test.test1.c_str());
    else
        fprintf(stderr, "Failed to parse\n");

    return 0;
}
