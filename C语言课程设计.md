## 前言
大数相乘除是一个更偏向于考验算法能力的项目，而不会涉及C语言当中那些结构体等较难的部分。
我在写这个程序的时候我会切实的把自己想象成用户，思考如何让别人在使用我封装函数的时候更容易上手。最终我实现了大数乘除，调用各自的一个函数就可以返回结果数组，也算是比较方便。有一个小缺陷就是最后还得自己手动释放内存，稍微麻烦，是在灵活多变性和方便性之间选择了灵活多变性（代码也更加健壮）。

/*用指针代替数组传递，一般加上length*/
/*指针的字节大小是固定的，由操作系统决定。数组的字节大小由数组大小和数组指向的类型共同来决定。*/
/*数组作为参数退化为指针，从而丢失数组长度信息*/
/*函数内部栈上创建的临时变量、临时对象，生命周期只在函数运行期间，函数运行结束后，就会释放对应内存空间。*/
/*
1.临时变量的值可以作为返回值；
2.临时变量的指针不能作为返回值；
*/

## 发现问题
* 数组作为函数参数时退化为指针，从而丢失数组长度信息
```c
void test(int arr[]){
    // 这里 sizeof(arr) 计算的是「指针的大小」，而非数组长度
    printf("函数内 arr 大小：%zu\n", sizeof(arr));  // 输出 8（64位系统）
    printf("函数内数组长度：%zu\n", sizeof(arr) / sizeof(arr[0]));  // 结果错误（8/4=2）
}
int main() {
    int arr[5] = { 1,2,3,4,5 };
    printf("main 中 arr 大小：%zu\n", sizeof(arr));  // 输出 20（5*4，int占4字节）
    printf("main 中数组长度：%zu\n", sizeof(arr) / sizeof(arr[0]));  // 输出 5
    test(arr);  // 数组传参，退化为指针
    return 0;
}
```

## 解决问题
## 改进
#### 改进1
* 改进原因：原程序Data1、Data2和Result数组是在main文件里定义，而Result_Calculate在源文件<calculate.cpp>里面。
* 缺点：Data1、Data2和Result数组是关键参数，在项目当中需反复传递，而且作为函数参数传递的是临时变量指针，无法在函数当中充当返回值。
```c
int* Result_Calculate(int *Data1, int *Data2)
//通过传参进入函数，函数执行完就会释放内存
{
	int Result[40];//临时变量
    ...	
	return Result;//返回一个临时的指针常量！
}
```
* 改进措施：用全局变量，在<calculate.cpp>里创建全局变量，并且在<calculate.h>里声明外部可调用(extern)。
* 优点：避免反复创建临时变量，避免传参造成不必要的麻烦；同时外部可调用减少main函数负担，不再需要创建变量，因此使主程序更加简洁；而且可以充当返回值（虽然没有必要返回Result的地址，因为Result数组外部可调用）
```c
//<calculate.h>
extern int Data1[], Data2[], Result[];
```
```c
//<calculate.cpp>
int Data1[DIGHT] = { 0 }, Data2[DIGHT] = { 0 }, Result[2 * DIGHT] = { 0 };
/*定义全局变量，因此不再需要返回临时变量指针，不需要考虑内存释放的问题*/
int* Result_Calculate()
{
	...
	return Result;
}
```

#### 改进2

```c

```
#### 改进3
```c
	for (int j = 1; j <= Result_Divide[0]; j++)//循环超级嵌套
	{
		while (j != 1 && Compare_BigNum(Data2, pTemp))
		{
			...
		}
		while(k < 10)//每次k都被赋初始值
		{
			if (Compare_BigNum(pTemp, Multiply(Data2, k)))//还要考虑等于的情况
			{
				k++;
			}
			else
			{
				k--;
				break;
			}
		}
		pM = Multiply(Data2, k);
		Result_Divide[j] = k;
	}
```
```c
while (true)
{
	...
	if (StateFlag == 1)//计算k值
	{
		...
		StateFlag = 2;
	}
	else if (StateFlag == 2)//减法运算+进位
	{
		...
		if (cmp == 1 || cmp == 0)
		{
			StateFlag = 1;
		}
		else
		{
			StateFlag = 3;
		}
	}
	else if (StateFlag == 3)//进位+补0
	{
		...
		if (cmp == 1 || cmp == 0)
		{
			StateFlag = 1;
		}
		else
		{
			StateFlag = 3;
		}
	}
}
```
```c
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
```
这段代码为什么出错
```c
long long Pow(int a, int b)
{
	if (b == 0 && a != 0)
	{
		return 1;
	}
	int Result = 1;
	int i;
	for (i = 0; i < b; i++)
	{
		Result = Result * a;
	}
	return Result;
}

void Input_Data(long long Data, int* arr)
//输入134566775896552
{
	int Digit, i;
	for (Digit = 0; Data / Pow(10, Digit) != 0; Digit++);
	arr[0] = Digit;

	for (i = 1; i <= Digit; i++)
	{
		arr[i] = Data / Pow(10, Digit - i) % 10;
	}
}
```

为什么把Temp清零中，同时把传进来的data1也清零了。经过地址查询两者占用同一块内存！这让我很是吃惊，于是不得不学起来内存管理
```c
int* Calculate_Subtract(int* data1, int* data2)
{
	Reserve_Array(data1);
	Reserve_Array(data2);
	static int Temp[DIGHT] = { 0 };//静态变量，可以放心做函数返回值
	for (int i = 0; i < DIGHT; i++)
	{
		Temp[i] = 0;
	}
}
```
#### 改进4
原本盲目使用extern，全局变量特别多，而且关键变量全暴露给外部文件，会让程序封装很难看，不能达到一个函数就解决问题的简洁程度，也是十分影响可移植性
```c
extern int Data1[] ,Data2[], Result_Multiply[], Result_Divide[];
```

#### 改进5
全局变量穿梭于各个函数之间，导致函数之间相互影响，当时就闹就了一个笑话——在执行一个函数时，误将指针指向另一个变量，导致两个毫不相关的变量居然共用一个地址！！这个问题也是找了我很久很久才找出来。这就反映了全局变量太多，还贯穿整个文件的弊端。
```c

```

#### 改进6
C 语言中，数组初始化时 [] 里不能放普通变量，但可以放常量表达式（比如宏定义、枚举、字面量）。因此要想改变数组长度必须另外得跑到头文件里手动修改宏定义
```c
#define DIGIT 40//手动修改此宏定义
...
int Data1[DIGIT] = { 0 };
int Data2[DIGIT] = { 0 };//数组初始化
```
所以考虑动态开辟内存，用 calloc 开辟，自动初始化为 0，避免垃圾值影响计算，但麻烦的是需要手动释放内存
```c
void Calculate_MultiplyResult(const char* Num1, const char* Num2, int Max_Digit)
{
	int* Data1 = (int*)calloc(Max_Digit, sizeof(int));
	int* Data2 = (int*)calloc(Max_Digit, sizeof(int));//传入参数Max_Digit
	...
	free(Data1); 
	free(Data2);
}
```
#### 改进7
例如如下函数，0，1，2这样的取值很随意，在每次用到这个函数时都需要对照注释来判断结果
```c
int Compare_BigNum(int* Data1, int* Data2)//返回0相等，1大于，2小于
```
所以考虑在此利用枚举类型充当返回值，改正后：
```c
typedef enum 
{
	Equal_to,//等于
	More_than,//大于
	Less_than//小于
}BigNum_Compare_Flag;
...
//函数定义
BigNum_Compare_Flag Compare_BigNum(int* Data1, int* Data2)
```
#### 改进8
返回值一旦使用全局变量，就会导致一个非常致命的问题，函数在调用时，后一次会覆盖前一次的值。在我的算法里，大数除法会用到另外的大数乘法，而大数乘法外部在调用，二者返回在同一个数组里就会造成混乱。因此，我决定再次使用动态内存管理，函数使用时开辟内存，再在main函数里释放内存，因此给使用者带来了一点点不便。
```c
int* Calculate_MultiplyResult(const char* Num1,const char* Num2,int Max_Digit)
{
	...
	int* Result = (int*)calloc(2 * Max_Digit, sizeof(int));
	...
	return Result;//此时并未释放内存
}

int main()
{
	...
	int* result = NULL;
	result=Calculate_MultiplyResult("16000000001", "7", 40);
	free(result);//在主函数释放
	...
}
```
#### 改进9
```c
int* Calculate_Subtract(int* Data1,int* Data2,int Max_Digit)
{
	Reserve_Array(Data1);
	Reserve_Array(Data2);
	int* Result_Substract = (int*)calloc(Max_Digit + 1, sizeof(int));

	for (int i = 1; i < Max_Digit; i++)
	{
		Result_Substract[i] = Data1[i] - Data2[i] + Result_Substract[i];
		if (Result_Substract[i] < 0)
		{
			Result_Substract[i + 1]--;
			Result_Substract[i] += 10;
		}
	}
	Result_Substract[0] = Get_Length(Result_Substract,Max_Digit);
	Reserve_Array(Data1);
	Reserve_Array(Data2);//不改变其原来值
	Reserve_Array(Result_Substract);//不改变其原来值

	return Result_Substract;
}
```
#### 改进
居然敢直接修改动态内存指针指向的值？这就会引起Temp_Data1指向非动态开辟的区域，最后释放内存也是没有意义的，因此直接导致内存泄漏。
```c
int* Calculate_DivideResult(const char* Num1, const char* Num2, int Max_Digit)
{
	...
	int* Temp_Data1 = (int*)calloc(Max_Digit + 1, sizeof(int));
	...
	Temp_Data1 = Calculate_Subtract(Temp_Data1, Temp_Data2, Max_Digit);
	...
	free(Temp_Data1);
}
```
```c
void Calculate_Subtract(int* Data1, int* Data2, int Max_Digit, int* Result_Substract)
{
	...
}
```
#### 浅谈函数的指针参数的利与弊
其实函数传入指针参数的优势就是灵活，指针作为参数时，程序复制了一遍地址，函数就可以直接修改地址所对应的值（下面有举例）；而对于传入一般参数就是将参数值复制一遍，额外放在一个空间存储着，再进行带入函数运算，所以无论怎么运算，不会改变原值，同时在函数结束存储的临时变量就会销毁
```c
void ptr_demo(int *p)
{
	(*p)++;
}
int main()
{
	int a = 1;
	int* p = &a;
	ptr_demo(p);
	printf("%d  %d",a,*p);//输出2，2
}
```
指针这样的特性其实会造成一定的不便：
首先，我用完指针之后，必须将原指针对应的数还原，像下面那样交换(Reserve_Array)之后一定要还原回来，如果不还原对源程序是毁灭性的！
```c
void Calculate_Subtract(int* Data1,int* Data2,int Max_Digit, int* Result_Substract)
{
	...
	Reserve_Array(Data1);
	Reserve_Array(Data2);
	...
	Reserve_Array(Data1);
	Reserve_Array(Data2);
}
```
其次，这其实在无形之中限制了这些指针变量不能相同，因为一旦相同就在运行此函数造成二者共用一块地址这样搞笑的局面，而这恰和我在运用这个函数时的需求产生冲突！如下所示，一旦这么使用这个函数，那将收获一堆“垃圾计算值”。
```c
Calculate_Subtract(Temp_Substract, Temp_Data2, Max_Digit, Temp_Substract);
```
于是我面临一个选择，改变1就是改变传入函数的参数类型，像这样传入具体的数组位数，但是得手动修改宏定义MAX_DIGITS，而不能像之前那样通过一个函数参数直接输入，于是我把这个方案放后面了
```c
void Calculate_Subtract(int Data1[MAX_DIGITS + 1], int Data1[MAX_DIGITS + 1], int Data1[MAX_DIGITS + 1])
{
	...
}
```
另一个比较丑陋的方法就是，我开辟一块备份区域，在函数内部加入了一个临时指针，而不直接对传入的Data1进行操作，也算是一种对症下药，最后也解决了这个问题。但外部最好还是别调用这么丑陋的函数了，于是我还贴心的加上了static关键字。
```c
static void Calculate_Subtract(int* Data1,int* Data2,int Max_Digit, int* Result_Substract)
{
	int* Data1_Backup = (int*)calloc(Max_Digit, sizeof(int));
	...
	memcpy(Data1_Backup, Data1, Max_Digit * sizeof(int)); // 复制原始数据
	...
}
```
#### 改进10
大数除法严格来说返回值不止一个数组，而包括结果和余数，但函数只能有一个返回值int*那怎么办？
```c
int* Calculate_DivideResult(const char* Num1, const char* Num2, int Max_Digit)
```
其实可以将返回的指针写到函数参数里面，这样同时还避免到main函数末尾还加上free(result)，真是一举两得！！这不就是运用了指针可以修改原值这样的特性吗？所以这样还是有好处的，就像下面这样：
```c
void Calculate_MultiplyResult(const char* num1, const char* num2, int Max_Digit, int* Result_Multiply);
void Calculate_DivideResult(const char* Num1, const char* Num2, int Max_Digit, int* Result_Divide, 
int* Remainder);
```

#### 
## 我的收获
* Debug调试
* 指针替换数组时一定要关注数组长度，因为一旦变成指针将丢失数组长度信息，非常不利程序进行，就相当于这个题目里的数组第一位总显示数组位数（即长度）
* 静态变量有好有坏！！小心static，每次调用函数都会初始化？还是只会在第一次调用函数时初始化？
* 静态函数是什么
* 函数参数的传递方式是值传递：调用函数时，会复制参数的值到函数栈帧中。
对于指针参数：复制的是 “指针变量存储的内存地址”（而非指针指向的内容）；
函数内部修改 “复制的地址”（比如让指针指向新内存），不会影响函数外部原指针变量；但函数内部通过 “复制的地址”，可以修改指针指向的内存空间（这是指针参数实现 “输出” 的核心）。
* 学着使用memcpy,memset处理内存的函数
* 状态机的应用————原本不太了解大数除法进位逻辑，滥用状态机，其实进位需要考虑的逻辑很固定，只需要for/while循环就好了
* 原来可移植性较差，大数比较、大数减法只能在文件内部调用，而无法在main函数里直接调用，修改之后可直接调用
* 优秀的代码设计遵循「最小暴露原则」：只暴露必要的接口，隐藏内部实现细节
* 原来代码依靠全局变量吃遍天，非常影响函数封装，从而影响可移植性。改正后函数通过参数接收输入和输出，不依赖全局变量
* 原来的代码用全局变量存储输入和结果，改进后用局部变量，通过参数传递给函数，计算完成后返回结果，移植时只需调用接口函数即可，无需关心内部实现。
## 参考资料
[学习调试C语言代码](https://www.bilibili.com/video/BV1Vm4y1r7jY?t=3.4&p=87)

[C语言临时变量与临时变量指针作函数的返回值](https://blog.csdn.net/OKVSA/article/details/105208955?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522b829048a025b27acf09e79438b8b5a91%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=b829048a025b27acf09e79438b8b5a91&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-2-105208955-null-null.142^v102^pc_search_result_base4&utm_term=%E8%BF%94%E5%9B%9E%E4%B8%B4%E6%97%B6%E5%8F%98%E9%87%8F%E6%8C%87%E9%92%88&spm=1018.2226.3001.4187)

[另]()