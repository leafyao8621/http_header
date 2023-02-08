#ifndef HTTP_UTIL_URL_URL_H_
#define HTTP_UTIL_URL_URL_H_

#include <containers/dstring.h>
#include <containers/darray.h>
#include <containers/hashmap.h>

DEF_HASHMAP(String, String)

int http_util_path_breakdown(char **iter);

#endif
