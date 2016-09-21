#ifndef JR_PARSE_H
#define JR_PARSE_H

#include "json_tokenizer.h"

namespace JR
{
template<typename T>
bool parseData(T &to_type, JT::Tokenizer &tokenizer, JT::Token &token)
{
    fprintf(stderr, "Failed to find parse function.\n");
    return false;
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
    parsed = parseData<T>(ret, tokenizer, token);
    return ret;
}
template<>
bool parseData<std::string>(std::string &to_type, JT::Tokenizer &tokenizer, JT::Token &token)
{
    if (token.value_type != JT::Token::String)
        return false;

    to_type = std::string(token.value.data, token.value.size);
    return true;
}
}
#endif
