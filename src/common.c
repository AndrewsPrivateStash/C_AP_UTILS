/*
 *
 *
 */

#include "../include/aputils.h"


const char *UTIL_ERR_PRINT(UTIL_ERR e) {
    switch (e) {
        case 0: return "E_SUCCESS";
        case -1: return "E_EMPTY_OBJ";
        case -2: return "E_EMPTY_ARG";
        case -3: return "E_BAD_ALLOC";
        case -4: return "E_MEMCOPY";
        case -5: return "E_OUTOFBOUNDS";
        case -6: return "E_EMPTY_FUNC";
        case -7: return "E_NOOP";
        case -8: return "E_NODATA";
        case -9: return "E_DOESNT_EXIST";
        case -10: return "E_BAD_TYPE";
        default:
    }
    return "UNDEF";
}