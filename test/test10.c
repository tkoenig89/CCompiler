void print (int i);
int scan ();
void exit (int i);

int main()
{
	int i,j;
	
	i = 0;
	j = 0;

	do
	{
		print(i);
		i = i - 1;
		j = 0;
		do
		{
			print(j);
			j = j + 1;
		}while(j<=10);
	}while(i>=-10);

	print(scan());

	exit(0);
}
