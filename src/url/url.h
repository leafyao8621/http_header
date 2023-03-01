#ifndef HTTP_UTIL_URL_URL_H_
#define HTTP_UTIL_URL_URL_H_

#include <http_util/containers.h>

typedef DArrayString URLBreakdown;

typedef struct URL {
    String text;
    URLBreakdown breakdown;
} URL;


typedef HashMapStringString URLParams;

int URL_initialize(URL *url);
int URL_parse(URL *url, char **iter);
int URL_finalize(URL *url);
int URL_serialize(URL *url, String *buf);

int URLParams_initialize(URLParams *url_params);
int URLParams_parse(URLParams *url_params, char **iter);
int URLParams_finalize(URLParams *url_params);
int URLParams_serialize(URLParams *url_params, String *buf);

#endif
