#ifndef SORTING_H
#define SORTING_H

#include "text_t.h"

typedef int (*comp_t) (const void* e1, const void* e2);

int comp_beg_ascend (const void* e1, const void* e2);
void to_alpha_right (const line_t* line, size_t* cnt);
int comp_end_ascend (const void* e1, const void* e2);
void to_alpha_left (const line_t* line, size_t* cnt);

void my_bsort(void* arr, size_t count, size_t elm_size, comp_t comp);
void my_qsort(void* arr, size_t count, size_t elm_size, comp_t comp);

void text_sort(text_t* text, comp_t comp);

#endif // SORTING_H
