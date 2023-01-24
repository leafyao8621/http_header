#ifndef HTTP_HEADER_HTTP_HEADER_H_
#define HTTP_HEADER_HTTP_HEADER_H_

#include <containers/darray.h>
#include <containers/hashmap.h>

#include <json/document.h>

DEF_DARRAY(String)

DEF_HASHMAP(String, String)

typedef struct HTTPHeader {
    String method, url, protocol;
    DArrayString url_detail;
    HashMapStringString attributes;
    union {
        String raw;
        HashMapStringString form;
        JSONDocument json;
    } data;
} HTTPHeader;

#endif
