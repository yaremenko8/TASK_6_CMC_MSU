#include<stdio.h>
#include <math.h>
#include <string.h>

//точность вычислений
#define PRCSN1 0.0001 //integral
#define PRCSN2 0.0001 //root


extern float f1(float); //exp(x) + 2
extern float f2(float); //-1/x
extern float f3(float); //(-2/3)(x + 1)
extern float f1d(float); //exp(x) = f1'
extern float f2d(float); //1/x^2 = f2'
extern float f3d(float); //(-2/3) = f3'

float rectSum(float(*f)(float), float a, float b, int n) //приближение разбиением на n прямоугольников
{
	float step = (b - a) / n;
	float sum = 0;
	for (int i = 0; i < n; i++) 
		sum += f(a + step * (i + 0.5)) * step;
	return sum;
}

float integral(float (*f)(float), float a, float b, float eps)
{
	int n = (1 / eps);
	float prev = rectSum(f, a, b, n);
	float current;
	while (fabsf(prev - (current = rectSum(f, a, b, n *= 2))) >= eps)  //правило Рунге
		prev = current;
	return current;
}

int iter; //счетчик иттераций

#define F(x) (f(x) - g(x))
#define Fd(x) (fd(x) - gd(x))
float root(float(*f)(float), float(*g)(float), float a, float b, float eps, float(*fd)(float), float(*gd)(float))
{
	int crit1 = F(a) > 0;
	int crit2 = F((a + b) / 2) > ((F(a) + F(b)) / 2);
	if (((crit1) && (!crit2)) || ((!crit1) && (crit2))) //проверка знака F'(x)F''(x)
	{
		float temp = a;
		a = b;
		b = temp;
	}
	iter = 0;
	while (fabsf(a - b) >= eps) //иттерации хорд и касательных
	{
		iter++;
		a -= (F(a) * (a - b)) / (F(a) - F(b));
		b -= F(b) / Fd(b);
	}
		return (a + b) / 2;
}
#undef F
#undef Fd

inline void debugIntegral(void);
inline void debugRoot(void);

//флаги опций
#define OPT_AREA 0b00000001
#define OPT_INTR 0b00000010
#define OPT_ITER 0b00000100

int main(int argc, char *argv[])
{
	char options = 0;
	if (argc <= 1)
	{
		printf("No output options specified. Use '-help' for options info.\n");
		return 0;
	}
	else
		for (int i = 1; i < argc; i++) //цикл опознающий опции переданные через аргументы
		{
			if (!strcmp(argv[i], "-help"))
			{
				printf("-help :  Prints options info. (cancels other options)\n-a    :  Outputs shape area.\n");
				printf("-in   :  Outputs roots.\n-it   :  Outputs iterations taken to calculate roots.\n");
				printf("-di   :  Runs debug mode for the 'integral' function. (cancels other options)\n");
				printf("-di   :  Runs debug mode for the 'root' function. (cancels other options)\n");
				return 0;
			}
			else if (!strcmp(argv[i], "-a"))
				options |= OPT_AREA;
			else if (!strcmp(argv[i], "-in"))
				options |= OPT_INTR;
			else if (!strcmp(argv[i], "-it"))
				options |= OPT_ITER;
			else if (!strcmp(argv[i], "-di"))
			{
				debugIntegral();
				return 0;
			}
			else if (!strcmp(argv[i], "-dr"))
			{
				debugRoot();
				return 0;
			}
			else
			{
				printf("Invalid option '%s'!\n", argv[i]);
				return 0;
			}
		}

	//вычисление пересечений и вывод кол-ва иттераций
	float a = root(f1, f3, -5, -1, PRCSN1, f1d, f3d);
	if(options & OPT_ITER)
		printf("\nexp(x) + 2 = (-2/3) * (1 + x) : %d iterations taken.\n", iter);
	float b = root(f3, f2, -5, -1, PRCSN1, f3d, f2d);
	if (options & OPT_ITER)
		printf("-1/x = (-2/3) * (1 + x)  (1)  : %d iterations taken.\n", iter);
	float d = root(f3, f2, 0.5, 1, PRCSN1, f3d, f2d);
	if (options & OPT_ITER)
		printf("-1/x = (-2/3) * (1 + x)  (2)  : %d iterations taken.\n", iter);
	float c = root(f1, f2, -1, -0.1, PRCSN1, f1d, f2d);
	if (options & OPT_ITER)
		printf("exp(x) + 2 = -1/x             : %d iterations taken.\n", iter);

	if (options & OPT_INTR) //вывод пересечений
	{
		printf("\nexp(x) + 2 = (-2/3) * (1 + x) : x = %f\n", a);
		printf("-1/x = (-2/3) * (1 + x)  (1)  : x = %f\n", b);
		printf("-1/x = (-2/3) * (1 + x)  (2)  : x = %f\n", d);
		printf("exp(x) + 2 = -1/x             : x = %f\n", c);
	}

	if(options & OPT_AREA) //площадь фигуры как разность интегральных площадей
	       printf("\nShape area equals to %f\n", integral(f1, a, c, PRCSN2) -
		                                         integral(f3, a, b, PRCSN2) -
		                                         integral(f2, b, c, PRCSN2));
	return 0;
}

inline void debugIntegral() //тестирование 'integral'
{
	float(*f)(float);
	float a, b, eps;
	int fid;
	scanf("%d%f%f%f", &fid, &a, &b, &eps);
	switch (fid)
	{
	case 1:
		f = f1;
		break;
	case 2:
		f = f2;
		break;
	case 3:
		f = f3;
		break;
	default:
		printf("Invalid parameters!\n");
		return;
	}
	printf("%f\n", integral(f, a, b, eps));
}

inline void debugRoot() //тестирование 'root'
{
	float(*f)(float);
	float(*g)(float);
	float(*fd)(float);
	float(*gd)(float);
	float a, b, eps;
	int fid;
	scanf("%d", &fid);
	switch (fid)
	{
	case 1:
		f = f1;
		fd = f1d;
		break;
	case 2:
		f = f2;
		fd = f2d;
		break;
	case 3:
		f = f3;
		fd = f3d;
		break;
	default:
		printf("Invalid parameters!\n");
		return;
	}
	scanf("%d", &fid);
	switch (fid) //производная соотв. функции задается автоматически
	{
	case 1:
		g = f1;
		gd = f1d;
		break;
	case 2:
		g = f2;
		gd = f2d;
		break;
	case 3:
		g = f3;
		gd = f3d;
		break;
	default:
		printf("Invalid parameters!\n");
		return;
	}
	if (g == f) //так нельзя
	{
		printf("Invalid parameters!\n");
		return;
	}
	scanf("%f%f%f", &a, &b, &eps);
	float temp = root(f, g, a, b, eps, fd, gd);
	printf("%f (value)  %d (iterations)\n", temp, iter);
}