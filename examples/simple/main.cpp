#include "test_struct.h"
#include <stdio.h>

const char json[] = "{ \"test1\" : \"stringVal\", \"velocity\" : [ 3.3, 5.5, 8.3] }";

int main()
{
    bool parsed;
    TestStruct test = JR::parseData<TestStruct>(json, sizeof json, parsed);

    if (parsed) {
        fprintf(stderr, "Parsed with value %s\n", test.test1.c_str());
        for (int i = 0; i < 3; i++) {
            fprintf(stderr, "\t vec3[%d] = %f\n", i, test.velocity.data[i]);
        }
    } else {
        fprintf(stderr, "Failed to parse\n");
    }

    return 0;
}
