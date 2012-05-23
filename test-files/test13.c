void print (int i);
int scan ();
void exit (int i);

int global-array[10];

int func1(param-array1[10])
{
	int i;i=0;
	print(1337);
	while(i<10)
	{
		print(param-array1[i]);
		i=i+1;
	}
}

int func2(param-array2[20])
{
	int i;i=0;
	print(1337);
	while(i<20)
	{
		print(main-array[i]);
		i=i+1;
	}
	i=0;
	print(1337);
	while(i<10)
	{
		print(global-array[i]);
		i=i+1;
	}
}

int main()
{
	int main-array[20];
	int i;i=0;
	while(i<20)
	{
		main-array[i] = i;
		i=i+1;
	}
	i=0;
	while(i<10)
	{
		global-array[i] = i*10;
		i=i+1;
	}
	func2(main-array);

	func1(global-array);
}
