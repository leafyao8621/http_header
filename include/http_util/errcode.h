#ifndef HTTP_UTIL_UTIL_ERRCODE_H_
#define HTTP_UTIL_UTIL_ERRCODE_H_

#define HTTP_UTIL_ERR_OK 0
#define HTTP_UTIL_ERR_NULL_PTR 1
#define HTTP_UTIL_ERR_INITIALIZE 2
#define HTTP_UTIL_ERR_PARSE 3
#define HTTP_UTIL_ILL_FORMATTED 4
#define HTTP_UTIL_SERIALIZE 5

extern const char *http_util_errcode_lookup[6];

#endif
