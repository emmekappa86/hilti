/* $Id$
 * 
 * printf() function for libhilti.
 * 
 */

#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "hilti_intern.h"
#include "utf8proc.h"

/* FIXME: This function doesn't print non-ASCII Unicode codepoints as we can't 
 * convert to the locale encoding yet. We just print them in \u syntax. */
static void _print_str(const __hlt_string* s, __hlt_exception* excpt)
{
    char c;
    int32_t cp;
    const int8_t* p = s->bytes;
    const int8_t* e = p + s->len;
    
    while ( p < e ) {
        ssize_t n = utf8proc_iterate((const uint8_t *)p, e - p, &cp);
        
        if ( n < 0 ) {
            *excpt = __hlt_exception_value_error;
            return;
        }
        
        if ( cp < 128 ) {
            c = (char)cp;
            write(1, &c, 1);
        }
        else {
            char buffer[10];
            int len;
            // FIXME: We shouldn't rely on snprintf here but bring our own itoa().
            if ( cp < (1 << 16) )
                len = snprintf(buffer, 10, "\\u%04x", cp);
            else
                len = snprintf(buffer, 10, "\\U%08x", cp);
            
            write(1, buffer, len);
        }
        
        p += n;
    }
}

/*
 * Hilti::print(obj, newline = True)
 * 
 * Prints a textual representation of an object to stdout.
 * 
 * obj: instance of any HILTI type - The object to print. 
 * newline: bool - If true, a newline is added automatically.
 * 
 */
void hilti_print(const __hlt_type_info* type, void* obj, int8_t newline, __hlt_exception* excpt)
{
    if ( type->libhilti_fmt ) {
        __hlt_string *s = (*type->libhilti_fmt)(type, obj, 0, excpt);
        if ( *excpt )
            return;
        
        _print_str(s, excpt);
        if ( *excpt )
            return;
    }
    
    else {
        /* No fmt function, just print the tag. */
        write(1, "<", 1);
        write(1, type->tag, strlen(type->tag));
        write(1, ">", 1);
    }
    
    if ( newline )
        write(1, "\n", 1);
}

