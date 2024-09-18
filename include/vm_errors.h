#ifndef ERRORS_H_
#define ERRORS_H_

typedef enum {
    ERR_OK,

    ERR_FAILED_INIT_VM,

    ERR_INVALID_REGISTER,

    ERR_INVALID_MEM_ACCESS,

    ERR_UNKNOWN_OP,

    ERR_WRITE_ERROR,
    ERR_READ_ERROR,
} Error;

char* error_as_string(Error err);

#endif