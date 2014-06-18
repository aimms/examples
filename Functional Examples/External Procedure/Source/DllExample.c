#include <windows.h>
#define DLL_EXPORT(type) __declspec(dllexport) type WINAPI

DLL_EXPORT(void) ComputeAverage(double *a, int card_i, int card_j, double *average)
{
	int i, j;
	double sum_a = 0.0;

#define __A(i,j)	a[i*card_j + j]

	for ( i = 0; i < card_i; i++ ) 
		for ( j = 0; j < card_j; j++ )
			sum_a += __A(i,j);

	*average = sum_a / (card_i*card_j);
}
