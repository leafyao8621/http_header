#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "url.h"
#include "../util/errcode.h"

int URLParams_initialize(URLParams *url_params) {
    int ret =
        HashMapStringString_initialize(
            url_params,
            10,
            containers_eq_dstr,
            containers_hash_dstr
        );
    if (ret) {
        return HTTP_UTIL_ERR_INITIALIZE;
    }
    return HTTP_UTIL_ERR_OK;
}

int URLParams_finalize(URLParams *url_params) {
    if (!url_params) {
        return HTTP_UTIL_ERR_NULL_PTR;
    }
    HashMapStringStringNode *iter = url_params->data;
    for (size_t i = 0; i < url_params->capacity; ++i, ++iter) {
        if (iter->in_use) {
            DArrayChar_finalize(&iter->key);
            DArrayChar_finalize(&iter->value);
        }
    }
    HashMapStringString_finalize(url_params);
    return HTTP_UTIL_ERR_OK;
}

int URLParams_parse(URLParams *url_params, char **iter) {
    if (!url_params || !iter) {
        return HTTP_UTIL_ERR_NULL_PTR;
    }
    int ret = 0;
    bool key_mode = true;
    char chr = 0;
    String key, value, *tgt;
    for (; **iter && **iter != ' ' && **iter != '\n'; ++(*iter)) {
        switch (**iter) {
        case '?':
            ret = DArrayChar_initialize(&key, 100);
            if (ret) {
                return HTTP_UTIL_ERR_PARSE;
            }
            break;
        case '=':
            if (!key_mode) {
                puts("=");
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&value);
                return HTTP_UTIL_ILL_FORMATTED;
            }
            key_mode = false;
            ret = DArrayChar_push_back(&key, &chr);
            if (ret) {
                DArrayChar_finalize(&key);
                return HTTP_UTIL_ERR_PARSE;
            }
            ret = DArrayChar_initialize(&value, 100);
            if (ret) {
                DArrayChar_finalize(&key);
                return HTTP_UTIL_ERR_PARSE;
            }
            break;
        case '&':
            if (key_mode) {
                DArrayChar_finalize(&key);
                return HTTP_UTIL_ILL_FORMATTED;
            }
            key_mode = true;
            ret = DArrayChar_push_back(&value, &chr);
            if (ret) {
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&value);
                return HTTP_UTIL_ERR_PARSE;
            }
            ret = HashMapStringString_fetch(url_params, &key, &tgt);
            if (ret) {
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&value);
                return HTTP_UTIL_ERR_PARSE;
            }
            *tgt = value;
            ret = DArrayChar_initialize(&key, 100);
            if (ret) {
                return HTTP_UTIL_ERR_PARSE;
            }
            break;
        default:
            ret = DArrayChar_push_back(key_mode ? &key : &value, *iter);
            if (ret) {
                DArrayChar_finalize(&key);
                if (!key_mode) {
                    DArrayChar_finalize(&value);
                }
                return HTTP_UTIL_ERR_PARSE;
            }
            break;
        }
    }
    ret = DArrayChar_push_back(&value, &chr);
    if (ret) {
        DArrayChar_finalize(&key);
        DArrayChar_finalize(&value);
        return HTTP_UTIL_ERR_PARSE;
    }
    ret = HashMapStringString_fetch(url_params, &key, &tgt);
    if (ret) {
        DArrayChar_finalize(&key);
        DArrayChar_finalize(&value);
        return HTTP_UTIL_ERR_PARSE;
    }
    *tgt = value;
    return HTTP_UTIL_ERR_OK;
}
