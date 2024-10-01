#include <math.h>
#include "comparators.h"

static void to_alpha_right (const line_t* line, size_t* cnt);
static void to_alpha_left (const line_t* line, size_t* cnt);

int comp_beg_ascend (const void* ptr1, const void* ptr2)
{
    const line_t* l1 = (const line_t*)ptr1;
    const line_t* l2 = (const line_t*)ptr2;

    size_t minlen = std::min(l1->len, l2->len);

    size_t cnt1 = 0, cnt2 = 0;
    uint32_t cd1 = 0, cd2 = 0;
    while (cnt1 < minlen && cnt2 < minlen)
    {
        to_alpha_right (l1, &cnt1);
        to_alpha_right (l2, &cnt2);

        cd1 = utf8_toupper(l1->ptr[cnt1].code);
        cd2 = utf8_toupper(l2->ptr[cnt2].code);

        if (cd1 != cd2)
            return (int)(cd2 - cd1);

        cnt1++;
        cnt2++;
    }
    return (int)(l2->len - l1->len);
}

int comp_end_ascend (const void* ptr1, const void* ptr2)
{
    const line_t* l1 = (const line_t*)ptr1;
    const line_t* l2 = (const line_t*)ptr2;

    size_t cnt1 = l1->len - 1, cnt2 = l2->len - 1;
    uint32_t cd1 = 0, cd2 = 0;

    while (cnt1 != (size_t)-1 && cnt2 != (size_t)-1)
    {
        to_alpha_left(l1, &cnt1);
        to_alpha_left(l2, &cnt2);

        if (cnt1 == (size_t)-1 || cnt2 == (size_t)-1)
            break;

        cd1 = utf8_toupper(l1->ptr[cnt1].code);
        cd2 = utf8_toupper(l2->ptr[cnt2].code);

        if (cd1 != cd2)
        {
            return (int)(cd2 - cd1);
        }

        cnt1--;
        cnt2--;
    }
    return (int)(cnt2 - cnt1);
}

static void to_alpha_right (const line_t* line, size_t* cnt)
{
    while (*cnt < line->len)
    {
        if (utf8_isalphabetic(line->ptr[*cnt].code))
            return;

        (*cnt)++;
    }
    return;
}

static void to_alpha_left (const line_t* line, size_t* cnt)
{
    while (*cnt != (size_t)-1)
    {
        if (utf8_isalphabetic(line->ptr[*cnt].code))
            return;

        (*cnt)--;
    }
    return;
}
