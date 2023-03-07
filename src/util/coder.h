#ifndef HTTP_UTIL_UTIL_CODER_H_
#define HTTP_UTIL_UTIL_CODER_H_

#include "containers.h"

int url_decode(char *in, String *out);
int url_encode(char *in, String *out);

#endif
