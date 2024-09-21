#ifndef TEXT_T_H
#define TEXT_T_H

#include <stdio.h>

typedef enum {
    UNSORTED = 0,
    BEG_ASC =   1,
    END_ASC =   2
} Sorting;

typedef struct {
    char* ptr;
    size_t len;
} line_t;

typedef struct {
    char* buffer;
    size_t length;
    line_t* lines;
    size_t line_n;
    Sorting state = UNSORTED;
} text_t;

bool text_ctor(text_t* text, FILE* text_file_orig);
void text_dtor(text_t* text);

size_t count_lines(text_t* text);
size_t get_text_length(FILE* text_p_orig);

void print_buff (text_t* text, FILE* out_f);
void print_text_lines (text_t* text, FILE* out_f);

#endif // TEXT_T_H
