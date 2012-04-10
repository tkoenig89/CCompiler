void print (int i);
int scan ();
void exit (int i);

int main(){

int bla1[10];
int i;
i=0;

//bla1[0]=1337;
//bla1[1]=1;
//bla1[2]=2;
//bla1[3]=3;
//bla1[4]=4;
//bla1[5]=5;

while(i<10)
{
	bla1[i]=i;
	i=i+1;

	print(bla1[i]);
	//print(i);
}

}
