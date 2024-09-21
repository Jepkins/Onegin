#ifndef SORTING_H
#define SORTING_H

#include "text_t.h"

typedef int (*comp_t) (const void* ptr1, const void* ptr2);

const size_t QSORT_LIMIT = 64;

void text_sort(text_t* text, comp_t comp);

void my_bsort(void* arr, size_t count, size_t elm_size, comp_t comp);
void my_qsort(void* arr, size_t count, size_t elm_size, comp_t comp);

int comp_beg_ascend (const void* ptr1, const void* ptr2);
int comp_end_ascend (const void* ptr1, const void* ptr2);

void swap_elms (void* a, void* b, size_t elm_size);

#endif // SORTING_H
