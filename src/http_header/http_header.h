#ifndef HTTP_HEADER_HTTP_HEADER_H_
#define HTTP_HEADER_HTTP_HEADER_H_

#include <containers/darray.h>
#include <containers/hashmap.h>

typedef char Char;
DEF_DARRAY(Char)
typedef DArrayChar String;
DEF_HASHMAP(String, String)

typedef HashMapStringString HTTPHeader;

int HTTPHeader_initialize(HTTPHeader *header);
int HTTPHeader_parse(HTTPHeader *header, char **iter);
int HTTPHeader_finalize(HTTPHeader *header);
int HTTPHeader_serialize(HTTPHeader *header, String *buf);

#endif
