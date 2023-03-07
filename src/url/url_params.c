#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "url.h"
#include "../util/errcode.h"
#include "../util/coder.h"

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
    String key, value, *tgt, key_decoded, value_decoded;
    ret = DArrayChar_initialize(&key, 100);
    if (ret) {
        return HTTP_UTIL_ERR_PARSE;
    }
    ret = DArrayChar_initialize(&value, 100);
    if (ret) {
        DArrayChar_finalize(&key);
        return HTTP_UTIL_ERR_PARSE;
    }
    for (; **iter && **iter != ' ' && **iter != '\n'; ++(*iter)) {
        switch (**iter) {
        case '?':
            break;
        case '=':
            if (!key_mode) {
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&value);
                return HTTP_UTIL_ILL_FORMATTED;
            }
            key_mode = false;
            ret = DArrayChar_push_back(&key, &chr);
            if (ret) {
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&value);
                return HTTP_UTIL_ERR_PARSE;
            }
            ret = DArrayChar_initialize(&key_decoded, 100);
            if (ret) {
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&value);
                return HTTP_UTIL_ERR_PARSE;
            }
            ret = url_decode(key.data, &key_decoded);
            if (ret) {
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&value);
                return ret;
            }
            break;
        case '&':
            if (key_mode) {
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&value);
                return HTTP_UTIL_ILL_FORMATTED;
            }
            key_mode = true;
            ret = DArrayChar_push_back(&value, &chr);
            if (ret) {
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&value);
                DArrayChar_finalize(&key_decoded);
                return HTTP_UTIL_ERR_PARSE;
            }
            ret = DArrayChar_initialize(&value_decoded, 100);
            if (ret) {
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&value);
                DArrayChar_finalize(&key_decoded);
                return HTTP_UTIL_ERR_PARSE;
            }
            ret = url_decode(value.data, &value_decoded);
            if (ret) {
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&value);
                DArrayChar_finalize(&key_decoded);
                DArrayChar_finalize(&value_decoded);
                return ret;
            }
            ret = HashMapStringString_fetch(url_params, &key_decoded, &tgt);
            if (ret) {
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&value);
                DArrayChar_finalize(&key_decoded);
                DArrayChar_finalize(&value_decoded);
                return HTTP_UTIL_ERR_PARSE;
            }
            *tgt = value_decoded;
            DArrayChar_clear(&key);
            DArrayChar_clear(&value);
            break;
        default:
            ret = DArrayChar_push_back(key_mode ? &key : &value, *iter);
            if (ret) {
                if (!key_mode) {
                    DArrayChar_finalize(&key_decoded);
                }
                return HTTP_UTIL_ERR_PARSE;
            }
            break;
        }
    }
    ret = DArrayChar_push_back(&value, &chr);
    if (ret) {
        DArrayChar_finalize(&key_decoded);
        DArrayChar_finalize(&value_decoded);
        return HTTP_UTIL_ERR_PARSE;
    }
    ret = DArrayChar_initialize(&value_decoded, 100);
    if (ret) {
        DArrayChar_finalize(&key);
        DArrayChar_finalize(&value);
        DArrayChar_finalize(&key_decoded);
        return HTTP_UTIL_ERR_PARSE;
    }
    ret = url_decode(value.data, &value_decoded);
    if (ret) {
        DArrayChar_finalize(&key);
        DArrayChar_finalize(&value);
        DArrayChar_finalize(&key_decoded);
        return ret;
    }
    ret = HashMapStringString_fetch(url_params, &key_decoded, &tgt);
    if (ret) {
        DArrayChar_finalize(&key);
        DArrayChar_finalize(&value);
        DArrayChar_finalize(&key_decoded);
        DArrayChar_finalize(&value_decoded);
        return HTTP_UTIL_ERR_PARSE;
    }
    *tgt = value_decoded;
    DArrayChar_finalize(&key);
    DArrayChar_finalize(&value);
    return HTTP_UTIL_ERR_OK;
}

int URLParams_serialize(URLParams *url_params, String *buf) {
    if (!url_params || !buf) {
        return HTTP_UTIL_ERR_NULL_PTR;
    }
    char chr = 0;
    HashMapStringStringNode *iter = url_params->data;
    int ret = 0;
    for (size_t i = 0; i < url_params->capacity; ++i, ++iter) {
        if (iter->in_use) {
            ret = url_encode(iter->key.data, buf);
            if (ret) {
                return ret;
            }
            DArrayChar_pop_back(buf);
            chr = '=';
            ret = DArrayChar_push_back(buf, &chr);
            if (ret) {
                return HTTP_UTIL_SERIALIZE;
            }
            ret = url_encode(iter->value.data, buf);
            if (ret) {
                return ret;
            }
            DArrayChar_pop_back(buf);
            chr = '&';
            ret = DArrayChar_push_back(buf, &chr);
            if (ret) {
                return HTTP_UTIL_SERIALIZE;
            }
        }
    }
    DArrayChar_pop_back(buf);
    chr = 0;
    ret = DArrayChar_push_back(buf, &chr);
    if (ret) {
        return HTTP_UTIL_SERIALIZE;
    }
    return HTTP_UTIL_ERR_OK;
}
