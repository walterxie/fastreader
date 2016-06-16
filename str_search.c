#include <string.h>

// strnstr is a BSD extension
// Creative Commons. http://stackoverflow.com/questions/23999797/implementing-strnstr
char *strnstr(const char *haystack, const char *needle, size_t len)
{
        int i;
        size_t needle_len;

        /* segfault here if needle is not NULL terminated */
        if (0 == (needle_len = strlen(needle)))
                return (char *)haystack;

        for (i=0; i<=(int)(len-needle_len); i++)
        {
                if ((haystack[0] == needle[0]) &&
                        (0 == strncmp(haystack, needle, needle_len)))
                        return (char *)haystack;

                haystack++;
        }
        return NULL;
}


// Find the first occurrence of a needle byte sequence inside a haystack,
// otherwise return NULL.
// memmem is a GNU extension
char *mem_mem(char *haystack_begin, const char *haystack_end, const char *needle, size_t needle_len) {
    char *p = haystack_begin;
    haystack_end -= needle_len - 1;
    while (p < haystack_end) {
        p = (char*)memchr(p, needle[0], haystack_end - p);
        if (!p)
            return NULL;
        if (memcmp(p, needle, needle_len) == 0)
            return p;
        p++;
    }
    return NULL;
}
