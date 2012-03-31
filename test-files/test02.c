int f(int i, int j[10]) 
{
	return i;
} 

int main() 
{
	int i; 
	int j[10];
	i = 0;
	i = j;
	i = 1 + f(i, j); //Arrays will be passed by reference (as a pointer) not by value (as a copy)
}
