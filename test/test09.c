void print (int i);
int scan ();
void exit (int i);

int main()
{
	int i,j;
	
	i = 0;
	j = 0;

	while(i>=-10)
	{
		print(i);
		i = i - 1;
		j = 0;
		while(j<=10)
		{
			print(j);
			j = j + 1;
		}
	}

	print(scan());

	exit(0);
}
