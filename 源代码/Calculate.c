#include<string.h>
#include"Calculate.h"

static int Get_Length(int* arr, int MaxDigit)
{
	int length_temp = MaxDigit;
	while (*(arr + length_temp) == 0)
	{
		length_temp--;
	}
	return length_temp;
}

void Input_Data(const char* num,int* arr)
{
	int i = 0;
	int j = 0;
	while (*(num + i) <= '0' || *(num + i) > '9')
	{
		i++;
	}
	while (true)
	{		
		if (*(num + i) == '\0')
		{
			break;
		}
		else if (*(num + i) >= '0' && *(num + i) <= '9')
		{
			arr[j + 1] = *(num + i) - 48;
			i++;
			j++;
		}
		else
		{
			i++;
			continue;
		}
	}
	arr[0] = j;
}

static void Swap(int* a, int* b)
{
	int Result= *a;
	*a = *b;
	*b = Result;
}

static void Reserve_Array(int* arr)
{
	char i = 1;
	for (i = 1; i <= *arr / 2; i++)
	{
		Swap(arr + i, arr + *arr - i + 1);
	}
}

void Calculate_MultiplyResult(const char* Num1, const char* Num2, int Max_Digit, int* Result_Multiply)
{
	int* Data1 = (int*)calloc(Max_Digit + 1, sizeof(int));
	int* Data2 = (int*)calloc(Max_Digit + 1, sizeof(int));

	if (Data1 == NULL || Data2 == NULL)
	{
		printf("内存分配失败！请检查原因\n");
		if (Data1 != NULL) free(Data1);
		if (Data2 != NULL) free(Data2);
		return;
	}
	Input_Data(Num1, Data1);
	Input_Data(Num2, Data2);
	//将字符串转化为数组Data1,Data2
	Reserve_Array(Data1);
	Reserve_Array(Data2);
	for (int i = 0; i <= Data1[0]; i++)
	{
		for (int j = 0; j <= Data2[0]; j++)
		{
			Result_Multiply[i + j + 1] = Data1[i + 1] * Data2[j + 1] + Result_Multiply[i + j + 1];
		}
	}
	for (int i = 1; i < Data1[0] + Data2[0]; i++)
	{
		Result_Multiply[i + 1] = Result_Multiply[i + 1] + Result_Multiply[i] / 10;
	}
	Result_Multiply[0] = Get_Length(Result_Multiply, Max_Digit);//获取数组长度信息

	for (int i = 1; i < Result_Multiply[0]; i++)
	{
		Result_Multiply[i] %= 10;
	}
	Reserve_Array(Result_Multiply);

	free(Data1);
	free(Data2);
}

static BigNum_Compare_Flag Compare_BigNum(int* Data1, int* Data2)
{
	int i = 1;
	if (Data1[0] > Data2[0])
		return More_than;
	else if (Data1[0] == Data2[0])
	{
		while (true)
		{
			if (Data1[i] > Data2[i])
			{
				return More_than;
			}
			else if (Data1[i] == Data2[i])
			{
				if (i == Data1[0])
					return Equal_to;//判断相等
				i++;
				continue;
			}
			else
			{
				return Less_than;
			}
			i++;
		}
	}
	else
		return Less_than;
}

//像这样把传指针进来，对其指向值进行修改，是会改变原数值的，所以用完之后注意还原
void Calculate_Subtract(int* Data1, int* Data2, int Max_Digit, int* Result_Substract)
{
	int* Data1_Backup = (int*)calloc(Max_Digit + 1, sizeof(int));
	if (Data1_Backup == NULL) {
		printf("Data1 备份内存分配失败！\n");
		exit(1);
	}
	memcpy(Data1_Backup, Data1, (Max_Digit + 1) * sizeof(int)); // 复制原始数据
	Reserve_Array(Data1_Backup);
	Reserve_Array(Data2);

	// 清空结果数组（确保初始值为 0，若外部已 calloc 可省略，保险起见加上）
	memset(Result_Substract, 0, (Max_Digit + 1) * sizeof(int));

	// 逐位减法（处理借位）
	for (int i = 1; i < Max_Digit; i++)
	{
		Result_Substract[i] = Data1_Backup[i] - Data2[i] + Result_Substract[i]; // 去掉冗余的 + Result_Substract[i]
		if (Result_Substract[i] < 0)
		{
			//if (i < Max_Digit - 1)
			//{
				Result_Substract[i + 1]--;
				Result_Substract[i] += 10;
			//}
		}
	}
	Result_Substract[0] = Get_Length(Result_Substract, Max_Digit);// 获取结果的有效长度

	Reserve_Array(Data2);
	Reserve_Array(Result_Substract);
	free(Data1_Backup);	// 恢复原数组
}

void Calculate_DivideResult(const char* Num1, const char* Num2, int Max_Digit, int* Result_Divide, int* Remainder)
{
	int* Data1 = (int*)calloc(Max_Digit + 1, sizeof(int));
	int* Data2 = (int*)calloc(Max_Digit + 1, sizeof(int));
	int* Temp_Data2 = (int*)calloc(Max_Digit + 1, sizeof(int));

	if (Data1 == NULL || Data2 == NULL || Temp_Data2 == NULL)
	{
		printf("内存分配失败！请检查原因\n");
		if (Data1 != NULL) free(Data1);
		if (Data2 != NULL) free(Data2);
		if (Temp_Data2 != NULL) free(Temp_Data2);
		return;
	}
	Input_Data(Num1, Data1);
	Input_Data(Num2, Data2);

	if (Compare_BigNum(Data1, Data2) == Less_than)
	{
		Result_Divide[0] = 1;
		Result_Divide[1] = 0;
		memcpy(Remainder, Data1, (Max_Digit + 1) * sizeof(int));
	}
	else
	{
		memcpy(Remainder, Data1, (Max_Digit + 1) * sizeof(int));
		memcpy(Temp_Data2, Data2, (Max_Digit + 1) * sizeof(int));

		Result_Divide[0] = Data1[0] - Data2[0] + 1;//默认值
		Temp_Data2[0] = Data1[0];//将二者位数齐平

		if (Compare_BigNum(Data1, Temp_Data2) == Less_than)
		{
			Result_Divide[0]--;
			Temp_Data2[0]--;
		}//确定好Result_Divide的位数

		for (int i = 0; i < Result_Divide[0]; i++)
		{
			while (Compare_BigNum(Remainder, Temp_Data2) == More_than ||
				Compare_BigNum(Remainder, Temp_Data2) == Equal_to)
			{
				Calculate_Subtract(Remainder, Temp_Data2, Max_Digit, Remainder);
				//最后Temp_Data1减剩下的就是余数
				Result_Divide[i + 1]++;
			}
			Temp_Data2[0]--;
		}
	}
	if (Remainder[0] == -1)
	{
		Remainder[0] = 1;
		Remainder[1] = 0;
	}
	free(Data1);
	free(Data2);
	free(Temp_Data2);
}