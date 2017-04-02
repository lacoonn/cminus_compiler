#include <stdio.h>

int S1(int);

int main()
{

	printf("%d\n", S1(5));
}

int S1(int n)
{
	int s = 0, i;
	for (i = 1; i <= n; i++) {
		s = s + i;
	}
	return s;
}