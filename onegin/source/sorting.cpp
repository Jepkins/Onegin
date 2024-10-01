#include <ctype.h>
#include <cmath>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "text_t.h"
#include "sorting.h"

static void qsort_r(void* arr, size_t elm_size, comp_t comp, size_t low, size_t high);
static size_t qsort_partition (void* arr, size_t elm_size, comp_t comp, size_t low, size_t high);

void text_sort(text_t* text, comp_t comp)
{
    my_qsort(text->lines, text->line_n, sizeof(*text->lines), comp);
}

void my_bsort(void* arr, size_t count, size_t elm_size, comp_t comp)
{
    for (size_t i = 0; i < count - 1; i++)
    {
        for (size_t j = i + 1; j < count; j++)
        {
            if (comp((char*)arr + i * elm_size, (char*)arr + j * elm_size) < 0)
                swap_elms((char*)arr + i * elm_size, (char*)arr + j * elm_size, elm_size);
        }
    }
}

void my_qsort(void* arr, size_t count, size_t elm_size, comp_t comp)
{
    qsort_r((char*)arr - elm_size, elm_size, comp, 1, count);
}

static void qsort_r(void* arr, size_t elm_size, comp_t comp, size_t low, size_t high)
{
    if (low < high)
    {
        if (high - low < QSORT_LIMIT)
        {
            my_bsort((char*)arr + elm_size*low, high - low + 1, elm_size, comp);
            return;
        }
        size_t pi = qsort_partition(arr, elm_size, comp, low, high);
        qsort_r(arr, elm_size, comp, low, pi - 1);
        qsort_r(arr, elm_size, comp, pi + 1, high);
    }
}

static size_t qsort_partition (void* arr, size_t elm_size, comp_t comp, size_t low, size_t high)
{
    size_t p = low;
    size_t i = low;
    size_t j = high;

    while (i < j)
    {
        while (comp( (char*)arr + i*elm_size, (char*)arr + p*elm_size) >= 0 && i < high)
        {
            i++;
        }
        while (comp( (char*)arr + j*elm_size, (char*)arr + p*elm_size) < 0 && j > low)
        {
            j--;
        }
        if (i < j)
        {
            swap_elms((char*)arr + i*elm_size, (char*)arr + j*elm_size, elm_size);
        }
    }
    swap_elms((char*)arr + low*elm_size, (char*)arr + j*elm_size, elm_size);

    return j;
}

void swap_elms (void* a, void* b, size_t elm_size)
{
    uint64_t temp = 0;

    size_t count = elm_size >> 3;

    char* a_ = (char*)a;
    char* b_ = (char*)b;

    for (size_t i = 0; i < count; i++)
    {
        memcpy(&temp, a_, sizeof(uint64_t));
        memcpy(a_, b_, sizeof(uint64_t));
        memcpy(b_, &temp, sizeof(uint64_t));

        a_ += sizeof(uint64_t);
        b_ += sizeof(uint64_t);
    }

    if (elm_size & 0x04)
    {
        memcpy(&temp, a_, sizeof(uint32_t));
        memcpy(a_, b_, sizeof(uint32_t));
        memcpy(b_, &temp, sizeof(uint32_t));

        a_ += sizeof(uint32_t);
        b_ += sizeof(uint32_t);
    }

    if (elm_size & 0x02)
    {
        memcpy(&temp, a_, sizeof(uint16_t));
        memcpy(a_, b_, sizeof(uint16_t));
        memcpy(b_, &temp, sizeof(uint16_t));

        a_ += sizeof(uint32_t);
        b_ += sizeof(uint32_t);
    }

    if (elm_size & 0x01)
    {
        memcpy(&temp, a_, sizeof(uint8_t));
        memcpy(a_, b_, sizeof(uint8_t));
        memcpy(b_, &temp, sizeof(uint8_t));
    }
}
