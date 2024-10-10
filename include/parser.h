#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdio.h>
#include <stdint-gcc.h>

typedef struct string_with_size {
    char *string;

    // The read pointer
    char *pread;
    
    // It does not contain the null-terminator
    uint16_t size;
} sstring;

void init_sstring(sstring *str, uint16_t size);

// File handling operations.
FILE *open_file_read(char *filepath);
void close_file(FILE *file);




void get_line_from_file(FILE *file, sstring *line);
void get_next_word(sstring *word, sstring *line, char sep);
void *reset_pread(sstring *str);


uint32_t encode(sstring *line);


#endif