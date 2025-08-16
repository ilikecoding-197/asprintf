/*
    asprintf.h - make SURE you have the asprintf function

    define ASPRINTF_IMPL in one of your files using this! this is a single header library!
    you can also define HAVE_ASPRINTF to 1 if you are SURE you want the implementation here

    Copyright 2025 Thomas Shrader

    Permission is hereby granted, free of charge, to any person obtaining a copy of this
    software and associated documentation files (the “Software”), to deal in the Software
    without restriction, including without limitation the rights to use, copy, modify,
    merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be included in all copies
    or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
    OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef ASPRINTF_H
#define ASPRINTF_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// check if we already have it
#ifndef HAVE_ASPRINTF
# if defined(__GLIBC__) || defined(__APPLE__) || defined(__MACH__) || defined(__FreeBSD__)
#  define HAVE_ASPRINTF 1
# endif
#endif

// if we dont have it, recreate it
#if !HAVE_ASPRINTF
int vasprintf(char **restrict strp, const char *restrict fmt, va_list arg);
int asprintf(char **restrict strp, const char *restrict fmt, ...);

#ifdef ASPRINTF_IMPL
int vasprintf(char **restrict strp, const char *restrict fmt, va_list arg) {
    va_list arg_copy;
    va_copy(arg_copy, arg);

    int len = vsnprintf(NULL, 0, fmt, arg_copy);
    va_end(arg_copy);

    if (len < 0) {
        *strp = NULL;
        return -1;
    }

    char *buf = malloc(len + 1);
    if (!buf) {
        *strp = NULL;
        return -1;
    }

    va_copy(arg_copy, arg);
    vsnprintf(buf, len + 1, fmt, arg_copy);
    va_end(arg_copy);

    *strp = buf;
    return len;
}

int asprintf(char **restrict strp, const char *restrict fmt, ...) {
    va_list arg;
    va_start(arg, fmt);

    int len = vasprintf(strp, fmt, arg);

    va_end(arg);
    return len;
}

#endif // ASPRINTF_IMPL
#endif // !HAVE_ASPRINTF
#endif // ASPRINTF_H