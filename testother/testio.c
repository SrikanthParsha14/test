#include <stdio.h>
#include <ctype.h>

int main()
{
	int c;
	while (( c = getchar()) != EOF)
		if (isascii(c) &&
			(isprint(c) || c == '\n' || c == '\t' || c == ' '))
			putchar(c);
		else
			printf("\\%03o", c);

	return 0;
}
