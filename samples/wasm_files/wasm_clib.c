#include <sys/types.h>

#include <ctype.h>
#include <limits.h>
#include <time.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

_Thread_local int errno;

void* realloc (void* ptr, size_t size)
{
    void* ret = NULL;

    //printf("%s() is called. size = %zu, ptr = %p:\n%s\n", __FUNCTION__, size, ptr, (char*)ptr);

    if ( size == 0 )
    {
        if (ptr != NULL )
            free(ptr);
        return NULL;
    }
    
    ret = malloc (size);

    if (ret != NULL)
    {
        memset (ret, 0, size);

        if (ptr != NULL )
        {
            memcpy(ret, ptr, size);
            free (ptr);
        }
    }

    //printf("%s(): ret = %p:\n%s\n", __FUNCTION__, ret, (char*)ret);

    return ret;
}

double strtod(const char* str, char** endptr)
{
    double result = 0.0;
    char signedResult = '\0';
    char signedExponent = '\0';
    int decimals = 0;
    bool isExponent = false;
    bool hasExponent = false;
    bool hasResult = false;
    // exponent is logically int but is coded as double so that its eventual
    // overflow detection can be the same as for double result
    double exponent = 0;
    char c;

    printf("%s(): input = %s, ", __FUNCTION__, str);

    for (; '\0' != (c = *str); ++str) {
        if ((c >= '0') && (c <= '9')) {
            int digit = c - '0';
            if (isExponent) {
                exponent = (10 * exponent) + digit;
                hasExponent = true;
            } else if (decimals == 0) {
                result = (10 * result) + digit;
                hasResult = true;
            } else {
                result += (double)digit / decimals;
                decimals *= 10;
            }
            continue;
        }

        if (c == '.') {
            if (!hasResult) break; // don't allow leading '.'
            if (isExponent) break; // don't allow decimal places in exponent
            if (decimals != 0) break; // this is the 2nd time we've found a '.'

            decimals = 10;
            continue;
        }

        if ((c == '-') || (c == '+')) {
            if (isExponent) {
                if (signedExponent || (exponent != 0)) break;
                else signedExponent = c;
            } else {
                if (signedResult || (result != 0)) break;
                else signedResult = c;
            }
            continue;
        }

        if (c == 'E') {
            if (!hasResult) break; // don't allow leading 'E'
            if (isExponent) break;
            else isExponent = true;
            continue;
        }

        break; // unexpected character
    }

    if (isExponent && !hasExponent) {
        while (*str != 'E')
            --str;
    }

    if (!hasResult && signedResult) --str;

    if (endptr) *endptr = (char*)str;

    for (; exponent != 0; --exponent) {
        if (signedExponent == '-') result /= 10;
        else result *= 10;
    }

    if (signedResult == '-' && result != 0) result = -result;

    printf(" output = %f\n", result);

    return result;
}

unsigned long long strtoull(const char *str, char **endptr, int base)
{
	const char *s;
	unsigned long long acc, cutoff;
	int c;
	int neg, any, cutlim;

	/*
	 * See strtoll for comments as to the logic used.
	 */
	if (base < 0 || base == 1 || base > 36) {
		if (endptr != 0)
			*endptr = (char *)str;
		errno = EINVAL;
		return 0;
	}

	s = str;
	do {
		c = (unsigned char) *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else {
		neg = 0;
		if (c == '+')
			c = *s++;
	}
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

	cutoff = ULLONG_MAX / (unsigned long long)base;
	cutlim = ULLONG_MAX % (unsigned long long)base;
	for (acc = 0, any = 0;; c = (unsigned char) *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0)
			continue;
		if (acc > cutoff || (acc == cutoff && c > cutlim)) {
			any = -1;
			acc = ULLONG_MAX;
			errno = ERANGE;
		} else {
			any = 1;
			acc *= (unsigned long long)base;
			acc += c;
		}
	}
	if (neg && any > 0)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : str);

    printf("%s(): input = %s, output = %llu\n", __FUNCTION__, str, acc);
	return (acc);
}