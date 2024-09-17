#include <ctype.h>
#include <cmath>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "text_t.h"
#include "sorting.h"

void qsort_r(void* arr, size_t elm_size, comp_t comp, size_t low, size_t high);
size_t qsort_partition (void* arr, size_t elm_size, comp_t comp, size_t low, size_t high);
void swap_elms (void* a, void* b, size_t elm_size);

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

void qsort_r(void* arr, size_t elm_size, comp_t comp, size_t low, size_t high)
{
    if (low < high)
    {
        if (high - low < 32)
        {
            my_bsort((char*)arr + elm_size*low, high - low + 1, elm_size, comp);
            return;
        }
        size_t pi = qsort_partition(arr, elm_size, comp, low, high);
        qsort_r(arr, elm_size, comp, low, pi - 1);
        qsort_r(arr, elm_size, comp, pi + 1, high);
    }
}

size_t qsort_partition (void* arr, size_t elm_size, comp_t comp, size_t low, size_t high)
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

int comp_beg_ascend (const void* e1, const void* e2)
{
    const line_t* s1 = (const line_t*)e1;
    const line_t* s2 = (const line_t*)e2;

    size_t minlen = std::min(s1->len, s2->len);

    size_t cnt1 = 0, cnt2 = 0;
    int ch1 = 0, ch2 = 0;
    while (cnt1 < minlen && cnt2 < minlen)
    {
        to_alpha_right (s1, &cnt1);
        to_alpha_right (s2, &cnt2);

        ch1 = toupper(s1->ptr[cnt1]);
        ch2 = toupper(s2->ptr[cnt2]);

        if (ch1 != ch2)
            return (ch2 - ch1);

        cnt1++;
        cnt2++;
    }
    return (int)(minlen - s1->len);
}

int comp_end_ascend (const void* e1, const void* e2)
{
    const line_t* s1 = (const line_t*)e1;
    const line_t* s2 = (const line_t*)e2;

    size_t cnt1 = s1->len - 1, cnt2 = s2->len - 1;
    int ch1 = 0, ch2 = 0;

    while (cnt1 != (size_t)-1 && cnt2 != (size_t)-1)
    {
        to_alpha_left(s1, &cnt1);
        to_alpha_left(s2, &cnt2);

        if (cnt1 == (size_t)-1 || cnt2 == (size_t)-1)
            break;

        ch1 = toupper(s1->ptr[cnt1]);
        ch2 = toupper(s2->ptr[cnt2]);

        if (ch1 != ch2)
        {
            return (ch2 - ch1);
        }

        cnt1--;
        cnt2--;
    }
    return (int)(cnt2 - cnt1);
}

void to_alpha_right (const line_t* line, size_t* cnt)
{
    while (*cnt < line->len)
    {
        if (isalpha(line->ptr[*cnt]))
            return;

        (*cnt)++;
    }
    return;
}

void to_alpha_left (const line_t* line, size_t* cnt)
{
    while (*cnt != (size_t)-1)
    {
        if (isalpha(line->ptr[*cnt]))
            return;

        (*cnt)--;
    }
    return;
}



