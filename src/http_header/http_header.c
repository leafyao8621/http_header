#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "http_header.h"
#include "../util/errcode.h"

DEF_DARRAY_FUNCTIONS(Char)
DEF_HASHMAP_FUNCTIONS(String, String)

bool eq_str(String *a, String *b) {
    return containers_eq_str(&a->data, &b->data);
}

size_t hash_str(String *a) {
    return containers_hash_str(&a->data);
}

int HTTPHeader_initialize(HTTPHeader *header) {
    if (!header) {
        return HTTP_HEADER_ERR_NULL_PTR;
    }
    int ret = HashMapStringString_initialize(header, 10, eq_str, hash_str);
    if (ret) {
        return HTTP_HEADER_ERR_INITIALIZE;
    }
    return HTTP_HEADER_ERR_OK;
}

int HTTPHeader_parse(HTTPHeader *header, char **iter) {
    if (!header || !iter) {
        return HTTP_HEADER_ERR_NULL_PTR;
    }
    char chr = 0;
    for (; **iter && **iter != '\n' && **iter != '\r'; ++(*iter)) {
        String buf;
        int ret = DArrayChar_initialize(&buf, 100);
        if (ret) {
            return HTTP_HEADER_ERR_PARSE;
        }
        for (; **iter && **iter != ':'; ++(*iter)) {
            ret = DArrayChar_push_back(&buf, *iter);
            if (ret) {
                DArrayChar_finalize(&buf);
                return HTTP_HEADER_ERR_PARSE;
            }
        }
        ret = DArrayChar_push_back(&buf, &chr);
        if (ret) {
            DArrayChar_finalize(&buf);
            return HTTP_HEADER_ERR_PARSE;
        }
        if (!**iter) {
            DArrayChar_finalize(&buf);
            return HTTP_HEADER_ILL_FORMATTED;
        }
        String temp;
        ret = DArrayChar_initialize(&temp, 100);
        if (ret) {
            DArrayChar_finalize(&buf);
            return HTTP_HEADER_ERR_PARSE;
        }
        for (*iter += 2; **iter && **iter != '\n'; ++(*iter)) {
            ret = DArrayChar_push_back(&temp, *iter);
            if (ret) {
                DArrayChar_finalize(&buf);
                DArrayChar_finalize(&temp);
                return HTTP_HEADER_ERR_PARSE;
            }
        }
        ret = DArrayChar_push_back(&temp, &chr);
        if (ret) {
            DArrayChar_finalize(&buf);
            DArrayChar_finalize(&temp);
            return HTTP_HEADER_ERR_PARSE;
        }
        if (!**iter) {
            DArrayChar_finalize(&buf);
            DArrayChar_finalize(&temp);
            return HTTP_HEADER_ILL_FORMATTED;
        }
        String *val;
        ret = HashMapStringString_fetch(header, &buf, &val);
        if (ret) {
            DArrayChar_finalize(&buf);
            DArrayChar_finalize(&temp);
            return HTTP_HEADER_ERR_PARSE;
        }
        *val = temp;
    }
    return HTTP_HEADER_ERR_OK;
}

int HTTPHeader_finalize(HTTPHeader *header) {
    if (!header) {
        return HTTP_HEADER_ERR_NULL_PTR;
    }
    HashMapStringStringNode *iter = header->data;
    for (size_t i = 0; i < header->capacity; ++i, ++iter) {
        if (iter->in_use) {
            DArrayChar_finalize(&iter->key);
            DArrayChar_finalize(&iter->value);
        }
    }
    HashMapStringString_finalize(header);
    return HTTP_HEADER_ERR_OK;
}

int HTTPHeader_serialize(HTTPHeader *header, String *buf) {
    if (!header || !buf) {
        return HTTP_HEADER_ERR_NULL_PTR;
    }
    int ret = 0;
    char chr = '\n';
    HashMapStringStringNode *iter = header->data;
    for (size_t i = 0; i < header->capacity; ++i, ++iter) {
        if (iter->in_use) {
            ret =
                DArrayChar_push_back_batch(
                    buf,
                    iter->key.data,
                    iter->key.size - 1
                );
            if (ret) {
                return HTTP_HEADER_SERIALIZE;
            }
            ret = DArrayChar_push_back_batch(buf, ": ", 2);
            if (ret) {
                return HTTP_HEADER_SERIALIZE;
            }
            ret =
                DArrayChar_push_back_batch(
                    buf,
                    iter->value.data,
                    iter->value.size - 1
                );
            if (ret) {
                return HTTP_HEADER_SERIALIZE;
            }
            ret = DArrayChar_push_back(buf, &chr);
            if (ret) {
                return HTTP_HEADER_SERIALIZE;
            }
        }
    }
    ret = DArrayChar_push_back(buf, &chr);
    if (ret) {
        return HTTP_HEADER_SERIALIZE;
    }
    return HTTP_HEADER_ERR_OK;
}
