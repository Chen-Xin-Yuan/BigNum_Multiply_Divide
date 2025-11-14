#include<stdio.h>

void Print_Mutiply_Result(int* Result_Mutiply)
{
	int i;
	printf("积共%d位数，为", Result_Mutiply[0]);
	for (i = 1; i <= Result_Mutiply[0]; i++)
	{
		printf("%d", Result_Mutiply[i]);
	}
	printf("\n");
}

void Print_Divide_Result(int* Result_Divide, int* Remainder)
{
	int i;
	printf("商共%d位数，为", Result_Divide[0]);
	for (i = 1; i <= Result_Divide[0]; i++)
	{
		printf("%d", Result_Divide[i]);
	}
	printf("%c", ',');
	//printf("\n");

	printf("余数共%d位数，为", Remainder[0]);
	for (i = 1; i <= Remainder[0]; i++)
	{
		printf("%d", Remainder[i]);
	}
	printf("\n");
}
