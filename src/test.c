#include <stdio.h>

#include <http_util/http_header.h>
#include <http_util/errcode.h>
#include <http_util/url.h>
#include <containers/eq.h>
#include <containers/hash.h>



void read_file(char *fn, String *buf) {
    char in_buf[1000];
    FILE *fin = fopen(fn, "rb");
    size_t sz = 0;
    for (; (sz = fread(in_buf, 1, 1000, fin)) == 1000;) {
        DArrayChar_push_back_batch(buf, in_buf, sz);
    }
    DArrayChar_push_back_batch(buf, in_buf, sz);
    char zero = 0;
    DArrayChar_push_back(buf, &zero);
    fclose(fin);
}

#define TEST(fn)\
has_param = false;\
read_file("data/"#fn".txt", &file_buf);\
puts(#fn);\
iter = file_buf.data;\
for (; *iter != 0 && *iter != '/'; ++iter);\
URL_initialize(&url);\
ret = URL_parse(&url, &iter);\
printf("%d %s\n", ret, http_util_errcode_lookup[ret]);\
if (!ret) {\
    ret = URL_serialize(&url, &out_buf);\
    printf("%d %s\n", ret, http_util_errcode_lookup[ret]);\
    char chr = 0;\
    DArrayChar_push_back(&out_buf, &chr);\
    puts(out_buf.data);\
    DArrayChar_clear(&out_buf);\
}\
if (*iter == '?') {\
    has_param = true;\
    URLParams_initialize(&params);\
    ret = URLParams_parse(&params, &iter);\
    printf("%d %s\n", ret, http_util_errcode_lookup[ret]);\
    if (!ret) {\
        ret = HTTPHeader_serialize(&params, &out_buf);\
        printf("%d %s\n", ret, http_util_errcode_lookup[ret]);\
        char chr = 0;\
        DArrayChar_push_back(&out_buf, &chr);\
        puts(out_buf.data);\
        DArrayChar_clear(&out_buf);\
    }\
}\
for (; *iter != 0 && *iter != '\n'; ++iter);\
++iter;\
HTTPHeader_initialize(&header);\
ret = HTTPHeader_parse(&header, &iter);\
printf("%d %s\n", ret, http_util_errcode_lookup[ret]);\
if (!ret) {\
    ret = HTTPHeader_serialize(&header, &out_buf);\
    printf("%d %s\n", ret, http_util_errcode_lookup[ret]);\
    char chr = 0;\
    DArrayChar_push_back(&out_buf, &chr);\
    puts(out_buf.data);\
    DArrayChar_clear(&out_buf);\
}\
HTTPHeader_finalize(&header);\
URL_finalize(&url);\
if (has_param) {\
    URLParams_finalize(&params);\
}\
DArrayChar_clear(&file_buf);


int main(void) {
    URL url;
    HTTPHeader header;
    URLParams params;
    String file_buf, out_buf;
    bool has_param;
    char *iter;
    int ret = 0;
    DArrayChar_initialize(&file_buf, 1000);
    DArrayChar_initialize(&out_buf, 1000);

    TEST(test1)
    TEST(test2)
    TEST(test3)

    DArrayChar_finalize(&file_buf);
    DArrayChar_finalize(&out_buf);
    return 0;
}
