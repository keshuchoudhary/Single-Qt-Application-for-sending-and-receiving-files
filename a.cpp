#include <stdio.h>
#include <unistd.h>
	//int a[(int)3e9];
int main()
{
	//int a[(int)1e8];
	unsigned long int n = 1.999*1024*1024*1024;
	int *a = new int[n];
	//sleep(100);
	for(unsigned long i=0;i<n;i++)
		a[i] = 2;
	printf("\nHi %d size %lu ", a[n-1], n);
}
