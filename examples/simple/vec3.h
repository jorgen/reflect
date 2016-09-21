#include <JR/json_tokenizer.h>

struct Vec3
{
    float data[3];
};

template<>
bool JR::parseData<Vec3>(Vec3 &to_type, JT::Tokenizer &tokenizer, JT::Token &token)
{
    if (token.value_type != JT::Token::ArrayStart)
        return false;

    JT::Error error;
    for (int i = 0; i < 3; i++) {
        error = tokenizer.nextToken(token);
        if (error != JT::Error::NoError || token.value_type != JT::Token::Number)
            return false;

        to_type.data[i] = strtod(token.value.data, 0);
    }
    error = tokenizer.nextToken(token);
    if (token.value_type != JT::Token::ArrayEnd)
        return false;

    return true;
}
