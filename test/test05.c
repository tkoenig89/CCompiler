//Fibunacci Numbers:
int fib(int n)
{
    if (n == 0 || n == 1)
    {
      return n;
    }
    else
    {
      return fib(n - 1) + fib(n - 2);
    }
}

void print (int i);
int scan ();
void exit (int i);

int main()
{
	print(fib(4));

	print(scan());

	exit(0);
}
