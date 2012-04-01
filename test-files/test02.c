int f(int i, int j[10]) 
{
	j[5] = 55;
	j[7] = 77;
	i = j[5] * j[7];
	i = 2;
	return i;
} 

int main() 
{
	int i; 
	int j[10];
	i = 2;
	j[2] = 33;
	i = j[2] * i;
	i = 1 + f(i, j); //Arrays will be passed by reference (as a pointer) not by value (as a copy)
}
