#include "../include/parser.h"
#include <malloc.h>
#include <errno.h>
#include <string.h>

void init_sstring(sstring *str, uint16_t size) {
    str->string = malloc(size);
    str->size = size;
    str->pread = str->string;

    // Set the null terminator
    str->string[size-1] = 0;
}

FILE *open_file_read(char *filepath) {
    return fopen(filepath, "r");
}

void close_file(FILE *file) {
    fclose(file);
}

void get_line_from_file(FILE *file, sstring *line) {
    uint16_t index = 0;
    char c = 0;

    do {
        fread(&c, 1, 1, file);
        index++;

    } while (c != '\n' && c != EOF);

    // We reset the file pointer to index bytes backwards.
    file->_IO_read_ptr = file->_IO_read_ptr - index;

    // We decrement the index because the last char accounted for is either '\n' or EOF

    init_sstring(line, index--);

    // We add the characters in the buffer
    for (int i = 0; i < index; i++) {
        fread(&c, 1, 1, file);
        line->string[i] = c;
    }

    // To get rid of the trailing '\n'
    fread(&c, 1, 1, file);
}

void get_next_word(sstring *word, sstring *line, char sep) {
    uint64_t index = 0;
    
    // We compute the starting offset
    uint16_t offset = (intptr_t)line->pread - (intptr_t)line->string;

    do {
        line->pread++;
        index++;
    } while(*line->pread != sep && line->pread - line->string < line->size && index < line->size);

    init_sstring(word, index);

    for (int i = 0; i < index; i++) {
        word->string[i] = line->string[i + offset];
    }

    // We increment once more, because we are at the ' '
    line->pread++;
}

uint32_t encode(sstring *line) {
    return 0;
}