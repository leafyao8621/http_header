#ifndef HTTP_HEADER_HTTP_HEADER_H_
#define HTTP_HEADER_HTTP_HEADER_H_

#include <http_util/containers.h>

typedef HashMapStringString HTTPHeader;

int HTTPHeader_initialize(HTTPHeader *header);
int HTTPHeader_parse(HTTPHeader *header, char **iter);
int HTTPHeader_finalize(HTTPHeader *header);
int HTTPHeader_serialize(HTTPHeader *header, String *buf);

#endif
