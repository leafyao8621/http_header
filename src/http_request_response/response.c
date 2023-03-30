#include <stdio.h>
#include <http_util/errcode.h>
#include <json/errcode.h>

#include "http_request_response.h"

char *http_response_code_lookup[63] = {
    "100 Continue",
    "101 Switching Protocols",
    "102 Processing",
    "103 Early Hints",
    "200 OK",
    "201 Created",
    "202 Accepted",
    "203 Non-Authoritative Information",
    "204 No Content",
    "205 Reset Content",
    "206 Partial Content",
    "207 Multi-Status",
    "208 Already Reported",
    "226 IM Used",
    "300 Multiple Choices",
    "301 Moved Permanently",
    "302 Found",
    "303 See Other",
    "304 Not Modified",
    "305 Use Proxy",
    "306 unused",
    "307 Temporary Redirect",
    "308 Permanent Redirect",
    "400 Bad Request",
    "401 Unauthorized",
    "402 Payment Required",
    "403 Forbidden",
    "404 Not Found",
    "405 Method Not Allowed",
    "406 Not Acceptable",
    "407 Proxy Authentication Required",
    "408 Request Timeout",
    "409 Conflict",
    "410 Gone",
    "411 Length Required",
    "412 Precondition Failed",
    "413 Payload Too Large",
    "414 URI Too Long",
    "415 Unsupported Media Type",
    "416 Range Not Satisfiable",
    "417 Expectation Failed",
    "418 I'm a teapot",
    "421 Misdirected Request",
    "422 Unprocessable Content",
    "423 Locked",
    "424 Failed Dependency",
    "425 Too Early",
    "426 Upgrade Required",
    "428 Precondition Required",
    "429 Too Many Requests",
    "431 Request Header Fields Too Large",
    "451 Unavailable For Legal Reasons",
    "500 Internal Server Error",
    "501 Not Implemented",
    "502 Bad Gateway",
    "503 Service Unavailable",
    "504 Gateway Timeout",
    "505 HTTP Version Not Supported",
    "506 Variant Also Negotiates",
    "507 Insufficient Storage",
    "508 Loop Detected",
    "510 Not Extended",
    "511 Network Authentication Required"
};

int HTTPResponse_initialize(
    HTTPResponse *response,
    HTTPResponseCode response_code,
    HTTPBodyType body_type) {
    if (!response) {
        return HTTP_UTIL_ERR_INITIALIZE;
    }
    int ret = HTTPHeader_initialize(&response->header);
    if (ret) {
        return HTTP_UTIL_ERR_INITIALIZE;
    }
    response->response_code = response_code;
    response->body_type = body_type;
    String key, value;
    ret = DArrayChar_initialize(&key, 20);
    if (ret) {
        return HTTP_UTIL_ERR_INITIALIZE;
    }
    ret = DArrayChar_initialize(&value, 50);
    if (ret) {
        return HTTP_UTIL_ERR_INITIALIZE;
    }
    switch (body_type) {
    case BODY_TYPE_NONE:
        break;
    case BODY_TYPE_TEXT:
        ret = DArrayChar_initialize(&response->body.text, 100);
        break;
    case BODY_TYPE_URL_ENCODED:
        DArrayChar_push_back_batch(&key, "Content-Type", 13);
        DArrayChar_push_back_batch(
            &value,
            "application/x-www-form-urlencoded",
            34
        );
        ret = URLParams_initialize(&response->body.url_encoded);
        break;
    case BODY_TYPE_JSON:
        DArrayChar_push_back_batch(&key, "Content-Type", 13);
        DArrayChar_push_back_batch(&value, "application/json", 17);
        ret = JSONNodePtr_initialize(&response->body.json.root);
        break;
    }
    if (ret) {
        return HTTP_UTIL_ERR_INITIALIZE;
    }
    if (key.size || value.size) {
        String *tgt;
        ret = HashMapStringString_fetch(&response->header, &key, &tgt);
        if (ret) {
            DArrayChar_finalize(&key);
            DArrayChar_finalize(&value);
            return HTTP_UTIL_ERR_INITIALIZE;
        }
        *tgt = value;
    } else {
        DArrayChar_finalize(&key);
        DArrayChar_finalize(&value);
    }
    return HTTP_UTIL_ERR_OK;
}

int HTTPResponse_finalize(HTTPResponse *response) {
    if (!response) {
        return HTTP_UTIL_SERIALIZE;
    }
    HTTPHeader_finalize(&response->header);
    switch (response->body_type) {
    case BODY_TYPE_NONE:
        break;
    case BODY_TYPE_TEXT:
        DArrayChar_finalize(&response->body.text);
        break;
    case BODY_TYPE_URL_ENCODED:
        URLParams_finalize(&response->body.url_encoded);
        break;
    case BODY_TYPE_JSON:
        JSONDocument_finalize(&response->body.json);
        break;
    }
    return 0;
}

int HTTPResponse_serialize(HTTPResponse *response, String *buf) {
    if (!response || !buf) {
        return HTTP_UTIL_SERIALIZE;
    }
    int ret = DArrayChar_push_back_batch(buf, "HTTP/1.1 ", 9);
    if (ret) {
        return HTTP_UTIL_SERIALIZE;
    }
    ret =\
        DArrayChar_push_back_batch(
            buf,
            http_response_code_lookup[response->response_code],
            strlen(http_response_code_lookup[response->response_code])
        );
    if (ret) {
        return HTTP_UTIL_SERIALIZE;
    }
    char chr = '\n';
    ret = DArrayChar_push_back(buf, &chr);
    if (ret) {
        return HTTP_UTIL_SERIALIZE;
    }
    ret = HTTPHeader_serialize(&response->header, buf);
    if (ret) {
        return HTTP_UTIL_SERIALIZE;
    }
    DArrayChar_pop_back(buf);
    if (response->response_code == HTTP_RESPONSE_204) {
        DArrayChar_pop_back(buf);
        DArrayChar_pop_back(buf);
        chr = 0;
        ret = DArrayChar_push_back(buf, &chr);
        if (ret) {
            return HTTP_UTIL_SERIALIZE;
        }
        return HTTP_UTIL_ERR_OK;
    }
    switch (response->body_type) {
    case BODY_TYPE_NONE:
        break;
    case BODY_TYPE_TEXT:
        ret =
            DArrayChar_push_back_batch(
                buf,
                response->body.text.data,
                response->body.text.size
            );
        break;
    case BODY_TYPE_URL_ENCODED:
        ret =
            URLParams_serialize(&response->body.url_encoded, buf);
        break;
    case BODY_TYPE_JSON:
        ret =
            JSONDocument_serialize(&response->body.json, buf, true);
        break;
    }
    if (ret) {
        return HTTP_UTIL_SERIALIZE;
    }
    return HTTP_UTIL_ERR_OK;
}
