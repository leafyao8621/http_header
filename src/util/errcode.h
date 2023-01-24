#ifndef HTTP_HEADER_UTIL_ERRCODE_H_
#define HTTP_HEADER_UTIL_ERRCODE_H_

#define HTTP_HEADER_ERR_OK 0
#define HTTP_HEADER_ERR_NULL_PTR 1
#define HTTP_HEADER_ERR_PARSE 2

extern const char *http_header_errcode_lookup[3];

#endif
