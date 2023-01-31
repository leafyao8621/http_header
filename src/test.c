#include <stdio.h>

#include <http_header/http_header.h>
#include <http_header/errcode.h>
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
read_file("data/"#fn".txt", &file_buf);\
HTTPHeader_initialize(&header);\
iter = file_buf.data;\
ret = HTTPHeader_parse(&header, &iter);\
printf("%d %s\n", ret, http_header_errcode_lookup[ret]);\
if (!ret) {\
    ret = HTTPHeader_serialize(&header, &out_buf);\
    printf("%d %s\n", ret, http_header_errcode_lookup[ret]);\
    char chr = 0;\
    DArrayChar_push_back(&out_buf, &chr);\
    puts(out_buf.data);\
    DArrayChar_clear(&out_buf);\
}\
HTTPHeader_finalize(&header);

int main(void) {
    HTTPHeader header;
    String file_buf, out_buf;
    char *iter;
    int ret = 0;
    DArrayChar_initialize(&file_buf, 1000);
    DArrayChar_initialize(&out_buf, 1000);

    TEST(test1)
    TEST(test2)

    DArrayChar_finalize(&file_buf);
    DArrayChar_finalize(&out_buf);
    return 0;
}
