#include <JR/parse.h>
#include <string>

struct Vec3
{
    float data[3];
};

struct TestStruct
{
    std::string test1;
    Vec3 velocity;
};

#include "reflect_json_test_struct.h"
