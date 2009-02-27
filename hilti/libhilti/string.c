/* $Id$
 * 
 * Support functions HILTI's string data type.
 * 
 */

#include <string.h>

#include "hilti_intern.h"
#include "utf8proc.h"

__hlt_string_size_t __hlt_string_len(const struct __hlt_string* s, __hlt_exception_t* exception)
{
    int32_t dummy;
    int8_t* p = s->bytes;
    int8_t* e = p + s->len;
    __hlt_string_size_t len = 0; 
    
    while ( p < e ) {
        ssize_t n = utf8proc_iterate((const uint8_t *)p, e - p, &dummy);
        if ( n < 0 ) {
        *exception = __hlt_exception_value_error;
            return 0;
        }
        
        ++len; 
        p += n;
    }
    
    if ( p != e ) {
        *exception = __hlt_exception_value_error;
        return 0;
    }
    
    return len;
}

#include <stdio.h>

const struct __hlt_string* __hlt_string_concat(const struct __hlt_string* s1, const struct __hlt_string* s2, __hlt_exception_t* exception)
{
    __hlt_string_size_t len1 = s1->len; 
    __hlt_string_size_t len2 = s2->len; 

    if ( ! len1 )
        return s2;
    
    if ( ! len2 )
        return s1;
    
    struct __hlt_string *dst = __hlt_gc_malloc_atomic(sizeof(struct __hlt_string) + len1 + len2);
    
    if ( ! dst ) {
        *exception = __hlt_exception_value_error;
        return 0;
    }
    
    dst->len = len1 + len2;
    memcpy(dst->bytes, s1->bytes, len1);
    memcpy(dst->bytes + len1, s2->bytes, len2);

    return dst;
}

const struct __hlt_string* __hlt_string_substr(const struct __hlt_string* s1, __hlt_string_size_t pos, __hlt_string_size_t len, __hlt_exception_t* exception)
{
    return 0;
}

__hlt_string_size_t __hlt_string_find(const struct __hlt_string* s, const struct __hlt_string* pattern, __hlt_exception_t* exception)
{
    return 0;
}

int __hlt_string_cmp(const struct __hlt_string* s1, const struct __hlt_string* s2, __hlt_exception_t* exception)
{
    return 0;
}