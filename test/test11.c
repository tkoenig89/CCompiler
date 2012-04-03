void print (int i);
int scan ();
void exit (int i);

int main()
{
	int i,j,k;
	j=2;
	k=1;
	-i=k=j;		//What does real c evaluate this to ?
	i=k=j;		//What does real c evaluate this to ?
	1=i;		//Real c code throws an error
	print(0) = 2;	//What does real c evaluate this to ?
	
	//if(0){}else{print(100);}
	
	print(i);
	print(k);
	print(j);
}
