void print (int i);
int scan ();
void exit (int i);

//Fibunacci Numbers:
int fib(int n)
{
	int a,b;
	if (n == 0 || n == 1)
	{
		if( n > 1 || n < 0)
			print(scan());
		return n;
		print(scan());
	}
	else
	{
		a = fib(n - 1);
		b = fib(n - 2);
		return a + b;
		print(scan());
		//return fib(n - 1) + fib(n - 2);
	}
}

int main()
{
	//print(fib(4));
	print(fib(16));

	print(scan());

	exit(0);
}
