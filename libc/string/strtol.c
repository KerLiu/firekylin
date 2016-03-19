#include <string.h>
#include <limits.h>

int strtol(const char *nptr, char **endptr, int base)
{
	const char *p = nptr;
	unsigned long ret;
	int ch;
	unsigned long Overflow;
	int sign = 0, flag, LimitRemainder;

	do {
		ch = *p++;
	} while (' ' == ch);

	if (ch == '-') {
		sign = 1;
		ch = *p++;
	} else if (ch == '+')
		ch = *p++;
	if ((base == 0 || base == 16) && ch == '0'
			&& (*p == 'x' || *p == 'X')) {
		ch = p[1];
		p += 2;
		base = 16;
	}
	if (base == 0)
		base = ch == '0' ? 8 : 10;

	Overflow = sign ? -(unsigned long) LONG_MIN : LONG_MAX;
	LimitRemainder = Overflow % (unsigned long) base;
	Overflow /= (unsigned long) base;

	for (ret = 0, flag = 0;; ch = *p++) {
		if (isdigit(ch))
			ch -= '0';
		else if (isalpha(ch))
			ch -= isupper(ch) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (ch >= base)
			break;

		if (flag < 0 || ret > Overflow
				|| (ret == Overflow && ch > LimitRemainder))
			flag = -1;
		else {
			flag = 1;
			ret *= base;
			ret += ch;
		}
	}

	if (flag < 0)
		ret = sign ? LONG_MIN : LONG_MAX;
	else if (sign)
		ret = -ret;

	if (endptr)
		*endptr = (char *) (flag ? (p - 1) : nptr);

	return ret;
}
