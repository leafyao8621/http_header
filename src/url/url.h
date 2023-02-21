#ifndef HTTP_UTIL_URL_URL_H_
#define HTTP_UTIL_URL_URL_H_

#include <containers/dstring.h>
#include <containers/darray.h>
#include <containers/hashmap.h>

DEF_DARRAY(String)

typedef DArrayString URLBreakdown;

int URLBreakdown_initialize(URLBreakdown *breakdown);
int URLBreakdown_parse(URLBreakdown *breakdown, char **iter);
int URLBreakdown_finalize(URLBreakdown *breakdown);

#endif
