#pragma once
#ifndef __CALCULATE_H__
#define __CALCULATE_H__

#include<stdio.h>
#include<stdlib.h>

typedef enum 
{
	Equal_to,
	More_than,
	Less_than
}BigNum_Compare_Flag;

void Calculate_MultiplyResult(const char* num1, const char* num2, int Max_Digit, int* Result_Multiply);
void Calculate_DivideResult(const char* Num1, const char* Num2, int Max_Digit, int* Result_Divide, int* Remainder);

#endif