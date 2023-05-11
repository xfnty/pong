#include <system/strid.h>

#include <assert.h>

#include <khash.h>

#include <system/logging.h>

KHASH_INIT(strid_t, strid_t, char*, 1, kh_int64_hash_func, kh_int64_hash_equal)


static khash_t(strid_t)* s_map;

khiter_t strid_get_iter(strid_t sid);

bool strid_init() {
    s_map = kh_init(strid_t);
    return true;
}

void strid_shutdown() {
    char* str;
    kh_foreach_value(s_map, str, free(str));

    kh_clear(strid_t, s_map);
    kh_destroy(strid_t, s_map);
}

strid_t strid_get_strid(const char* str) {
    assert(str != NULL);

    khint_t sid = kh_str_hash_func(str);
    khint_t it = kh_get(strid_t, s_map, sid);
    if (it == kh_end(s_map)) {
        int err;
        it = kh_put(strid_t, s_map, sid, &err);
        assert(err != -1);
        kh_val(s_map, it) = strdup(str);
    }

    return sid;
}

const char* strid_get_str(strid_t sid) {
    khint_t it = kh_get(strid_t, s_map, sid);
    return (it != kh_end(s_map)) ? kh_val(s_map, it) : NULL;
}

bool strid_has_str(const char* str) {
    assert(str != NULL);
    
    khint_t sid = kh_str_hash_func(str);
    return strid_has_strid(sid);
}

bool strid_has_strid(strid_t sid) {
    return kh_get(strid_t, s_map, sid) != kh_end(s_map);
}
