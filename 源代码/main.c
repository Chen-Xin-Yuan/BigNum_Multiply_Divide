#include"Calculate.h"
#include"PrintData.h"

#define Max_Digit 40

int main()
{
	//Ê¾Àý´úÂë
	int result_divide[Max_Digit + 1] = { 0 };
	int result_mutiply[2 * Max_Digit + 1] = { 0 };
	int remainder[Max_Digit + 1] = { 0 };

	Calculate_MultiplyResult("00000001600^&%&*0000001", "7", Max_Digit, result_mutiply);
	Print_Mutiply_Result(result_mutiply);

	Calculate_DivideResult("00006232%^*4362541", "^&*45*&(22", Max_Digit, result_divide, remainder);
	Print_Divide_Result(result_divide, remainder);

	return 0;
}
