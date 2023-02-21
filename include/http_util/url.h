#ifndef HTTP_UTIL_URL_URL_H_
#define HTTP_UTIL_URL_URL_H_

#include <containers/dstring.h>
#include <containers/darray.h>
#include <containers/hashmap.h>

DEF_DARRAY(String)

typedef DArrayString URLBreakdown;

typedef struct URL {
    String text;
    URLBreakdown breakdown;
} URL;

int URL_initialize(URL *url);
int URL_parse(URL *url, char **iter);
int URL_finalize(URL *url);
int URL_serialize(URL *url, String *buf);

#endif
