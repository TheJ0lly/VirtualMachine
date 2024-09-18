#include "../include/vm_errors.h"

char* error_as_string(Error err) {
    switch (err)
    {
    case ERR_OK:
        return "ERR_OK";
    case ERR_FAILED_INIT_VM:
        return "ERR_FAILED_INIT_VM";
    case ERR_INVALID_REGISTER:
        return "ERR_INVALID_REGISTER";
    case ERR_INVALID_MEM_ACCESS:
        return "ERR_INVALID_MEM_ACCESS";
    case ERR_WRITE_ERROR:
        return "ERR_WRITE_ERROR";
    case ERR_READ_ERROR:
        return "ERR_READ_ERROR";
    default:
        return "ERR_UNKNOWN_ERROR";
    }
}