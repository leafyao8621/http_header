#include <stdio.h>

#include "coder.h"
#include "errcode.h"

int url_decode(char *in, String *out) {
    uint8_t chr = 0;
    int ret = 0;
    for (char *iter = in; *iter; ++iter) {
        if (*iter == '%') {
            chr = 0;
            for (int i = 0; i < 2; ++i) {
                ++iter;
                chr <<= 4;
                if (*iter >= '0' && *iter <= '9') {
                    chr += *iter - '0';
                } else if (*iter >= 'A' && *iter <= 'F') {
                    chr += *iter - 'A' + 10;
                } else if (*iter >= 'a' && *iter <= 'f') {
                    chr += *iter - 'a' + 10;
                } else {
                    return HTTP_UTIL_ILL_FORMATTED;
                }
            }
            ret = DArrayChar_push_back(out, (char*)&chr);
            if (ret) {
                return HTTP_UTIL_ERR_PARSE;
            }
        } else {
            ret = DArrayChar_push_back(out, iter);
            if (ret) {
                return HTTP_UTIL_ERR_PARSE;
            }
        }
    }
    chr = 0;
    ret = DArrayChar_push_back(out, (char*)&chr);
    if (ret) {
        return HTTP_UTIL_ERR_PARSE;
    }
    return 0;
}

int url_encode(char *in, String *out) {
    char chr = 0;
    int ret = 0;
    char buf[4];
    for (char *iter = in; *iter; ++iter) {
        if (
            (*iter >= 'A' && *iter <= 'Z') ||
            (*iter >= 'a' && *iter <= 'z') ||
            (*iter >= '0' && *iter <= '9')) {
            ret = DArrayChar_push_back(out, iter);
            if (ret) {
                return HTTP_UTIL_SERIALIZE;
            }
        } else {
            switch (*iter) {
            case '-':
            case '_':
            case '.':
            case '~':
                ret = DArrayChar_push_back(out, iter);
                if (ret) {
                    return HTTP_UTIL_SERIALIZE;
                }
                break;
            default:
                snprintf(buf, 4, "%%%02X", *iter);
                ret = DArrayChar_push_back_batch(out, buf, 3);
                break;
            }
        }
    }
    chr = 0;
    ret = DArrayChar_push_back(out, &chr);
    if (ret) {
        return HTTP_UTIL_SERIALIZE;
    }
    return HTTP_UTIL_ERR_OK;
}
