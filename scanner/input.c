/*	This comment is for
testing ERROR code*/

1234 zxc 1c1c1

void printString () {
	print5f("Too Many 2rr5r Code");
	printf("Com/*COMMENT*/ment");
	puts("55c3/*PUPPET*/333")
}

/* A program to perform Euclid's
	Algorithm to compute gcd. */

int gcd (int u, int v)
{	if (v == 0) return u ;
	else return gcd(v,u-u/v*v);
	/* u-u/v*v == u mod v * /
}

void main(void)
{	int x; int y;
	x = input(); y = input();
	output(gcd(x,y));
}