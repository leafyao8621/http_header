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
    HashMapStringString params;
    HashMapStringString attrs;
} HTTPHeader;

int HTTPHeader_initialize(HTTPHeader *header);
int HTTPHeader_parse(HTTPHeader *header, char *str);
// int HTTPHeader_serialize(
//     HTTPHeader *header,
//     String *buf,
//     bool use_url_detail,
//     bool use_params);

#endif
