    #ifndef JR_PARSE_H
#define JR_PARSE_H

#include "json_tokenizer.h"

namespace JR
{
template<typename T>
void parseData(T &to_type, JT::Tokenizer &tokenizer, JT::Token &token, bool &parsed)
{
    parsed = false;
    fprintf(stderr, "Failed to find parse function.\n");
}

template<typename T>
T parseData(const char *json_data, size_t size, bool &parsed)
{
    T ret;
    parsed = false;
    JT::Tokenizer tokenizer;
    tokenizer.addData(json_data, size);
    JT::Token token;
    JT::Error error = tokenizer.nextToken(token);
    if (error != JT::Error::NoError)
        return ret;
    parseData(ret, tokenizer, token, parsed);
    return ret;
}
}
#endif
