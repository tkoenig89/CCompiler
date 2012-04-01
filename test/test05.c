//Fibunacci Numbers:
int fib(int n)
{
	int a,b;
	if (n == 0 || n == 1)
	{
		return n;
	}
	else
	{
		a = fib(n - 1);
		b = fib(n - 2);
		return a + b;
		//return fib(n - 1) + fib(n - 2);
	}
}

void print (int i);
int scan ();
void exit (int i);

int main()
{
	//print(fib(4));
	print(fib(16));

	print(scan());

	exit(0);
}
