#ifndef JR_PARSE_H
#define JR_PARSE_H

#include "json_tokenizer.h"

namespace JR
{
template<typename T>
void parseData(T &to_type, const char *json_data, size_t size, bool &parsed)
{
    fprintf(stderr, "Failed to find parse function.\n");
}
    
template<typename T>
T parseData(const char *json_data, size_t size, bool &parsed)
{
    T ret;
    parseData(ret, json_data, size, parsed);
    return ret;
}
}
#endif
