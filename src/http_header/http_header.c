#include "http_header.h"
#include "../util/errcode.h"

int HTTPHeader_initialize(HTTPHeader *header) {
    if (!header) {
        return HTTP_HEADER_ERR_NULL_PTR;
    }
    return HTTP_HEADER_ERR_OK;
}

int HTTPHeader_parse(HTTPHeader *header, char *str) {
    if (!header || !str) {
        return HTTP_HEADER_ERR_NULL_PTR;
    }
    String buf;
    int ret = DArrayChar_initialize(&buf, 100);
    if (ret) {
        return HTTP_HEADER_ERR_PARSE;
    }
    char *iter = str;
    for (; *iter && *iter != ' '; ++iter) {

    }
    return HTTP_HEADER_ERR_OK;
}

// int HTTPHeader_serialize(
//     HTTPHeader *header,
//     String *buf,
//     bool use_url_detail,
//     bool use_params) {
//     if (!header || !buf) {
//         return HTTP_HEADER_ERR_NULL_PTR;
//     }
//     return HTTP_HEADER_ERR_OK;
// }
