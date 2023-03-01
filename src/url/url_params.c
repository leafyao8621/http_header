#include <containers/eq.h>
#include <containers/hash.h>

#include "url.h"
#include "../util/errcode.h"

int URLParams_initialize(URLParams *url_params) {
    int ret =
        HashMapStringString_initialize(
            url_params,
            10,
            containers_eq_dstr,
            containers_hash_dstr
        );
    if (ret) {
        return HTTP_UTIL_ERR_INITIALIZE;
    }
    return HTTP_UTIL_ERR_OK;
}
