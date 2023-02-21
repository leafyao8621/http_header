#include "url.h"
#include "../util/errcode.h"

DEF_DARRAY_FUNCTIONS(String)

int URLBreakdown_initialize(URLBreakdown *breakdown) {
    if (!breakdown) {
        return HTTP_UTIL_ERR_NULL_PTR;
    }
    int ret = DArrayString_initialize(breakdown, 10);
    if (ret) {
        return HTTP_UTIL_ERR_INITIALIZE;
    }
    return HTTP_UTIL_ERR_OK;
}

int URLBreakdown_parse(URLBreakdown *breakdown, char **iter) {
    String buf;
    int ret = DArrayChar_initialize(&buf, 100);
    if (ret) {
        return HTTP_UTIL_ERR_PARSE;
    }
    for (++(*iter); **iter && **iter != ' ' && **iter != '?'; ++(*iter)) {
        if (**iter == '/') {
            ret = DArrayString_push_back(breakdown, &buf);
            DArrayChar_finalize(&buf);
            if (ret) {
                return HTTP_UTIL_ERR_PARSE;
            }
            ret = DArrayChar_initialize(&buf, 100);
            if (ret) {
                return HTTP_UTIL_ERR_PARSE;
            }
        } else {
            ret = DArrayChar_push_back(&buf, *iter);
            if (ret) {
                DArrayChar_finalize(&buf);
                return HTTP_UTIL_ERR_PARSE;
            }
        }
    }
    if (!**iter) {
        DArrayChar_finalize(&buf);
        return HTTP_UTIL_ILL_FORMATTED;
    }
    if (buf.size) {
        ret = DArrayString_push_back(breakdown, &buf);
        if (ret) {
            DArrayChar_finalize(&buf);
            return HTTP_UTIL_ERR_PARSE;
        }
    }
    DArrayChar_finalize(&buf);
    return HTTP_UTIL_ERR_OK;
}
