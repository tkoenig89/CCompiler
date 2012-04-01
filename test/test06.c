int fack(int n)
{
	int a,b,c,d,e,f;
	if (n==0||n==1) 
	{
		return 1;
	}
	else 
	{
		return n*fack(n-1);
	}
}

void print (int i);
int scan ();
void exit (int i);

int main()
{
	print(fack(5));

	print(scan());

	exit(0);
}
