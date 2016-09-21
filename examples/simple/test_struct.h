#include <JR/parse.h>
#include <string>
#include "vec3.h"

struct TestStruct
{
    std::string test1;
    Vec3 velocity;
};

#include "reflect_json_test_struct.h"
