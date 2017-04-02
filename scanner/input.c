int SR(int);

int main()
	int it53 = 5;
	char tor = 'a';
	char tor2[5] = "12345";
	printf(SR(5));

int SR(int n)
	int s = 0, i;
	int ary[5];
	for (i = 1; i <= n; i =i+1)
		s = s + i;
	return s;
