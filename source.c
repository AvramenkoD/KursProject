#include <stdio.h>
#include <locale.h>
#include <math.h>

//Работа с массивом(пункт меню) загрузить данные из файла, создать новые(из табулирования)
//отсортировать массив(убывание / возастание), алгоритм(расчёска, гномья)
//найти все отрицательные элементы и вывести остортированные

#define WIDTH   100     
#define HEIGHT   50     

typedef double (*TFunc_t)(double);

double funcY(double);
double funcV(double);

int printTab(TFunc_t pFunc, double x1, double x2, double step);
int buildGraph(TFunc_t f, double xStart, double xEnd);
int chunkV(double x);


int main(void)
{
    setlocale(LC_ALL, "RUS");
    int choice, opChoice, repeatF = 1, inputEr;
    double x1, x2, step;
    TFunc_t func[2] = { funcY, funcV };

    puts("-----------------------------------------");
    puts("| Курсовой проект                       |");
    puts("| Разработка программы анализа функций  |");
    puts("|                                       |");
    puts("| Разработал: Д.А. Авраменко            |");
    puts("| Группа: бИПТ-251                      |");
    puts("| Руководитель О.В. Курипта             |");
    puts("------------------------------------------------------");
    puts("|             Функции                                |");
    puts("| 1) Y(x) = e^(cos(x)) * cos(sin(x))                 |");
    puts("|                                                    |");
    puts("|           при x > 0.75         -> x + 1            |");
    puts("| 2) V(x) = при 0 <= x <= 0.75   -> 1 - x^5          |");
    puts("|           при x < 0            -> x + ln(|cos(x)|) |");
    puts("------------------------------------------------------");

    do
    {
        switch (repeatF)
        {
            case 1:
                do
                {
                    printf("\nВыберите функцию\n\nВаш выбор: ");
                    scanf("%d", &choice);

                    inputEr = 0;
                    if (choice < 1 || choice > 2)
                    {
                        printf("\n------------------------------------------------------\n");
                        puts("Ошибка: выбран некорректный номер функции.");
                        inputEr = 1;
                    }
                    printf("\n------------------------------------------------------\n\n");
                    choice--;
                } while (inputEr);
            case 2:
                do
                {
                    puts("Выберите операцию:");
                    puts("1) Вычислить значение функции в точке");
                    puts("2) Протабулировать функцию на интервале");
                    puts("3) Построить график функции на интервале");
                    printf("\nВаш выбор: ");
                    scanf("%d", &opChoice);
                    printf("\n------------------------------------------------------\n");

                    inputEr = 0;
                    if (opChoice < 1 || opChoice > 3)
                    {
                        printf("Ошибка: выбран некорректный номер действия.\n");
                        puts("------------------------------------------------------\n");
                        inputEr = 1;
                    }
                } while (inputEr);
            case 3:
                switch (opChoice)
                {
                    case 1:
                        printf("\nВведите x: ");
                        scanf("%lf", &x1);
                        if(func[choice](x1) != NAN)
                            printf("\nРезультат: f(%.3lf) = %.10lf\n", x1, func[choice](x1));
                        else
                            puts("Функция не существует в данной точке.");
                        puts("------------------------------------------------------");
                        break;
                    case 2:
                        printf("Введите диапазон и шаг через пробел (x1 x2 h): ");
                        scanf("%lf %lf %lf", &x1, &x2, &step);
                        printTab(func[choice], x1, x2, step);
                        break;
                    case 3:
                        printf("Введите диапазон через пробел (x1 x2): ");
                        scanf("%lf %lf", &x1, &x2);
                        buildGraph(func[choice], x1, x2);
                        break;
                }
                break;
            case 4:
                switch (opChoice)
                {
                    case 1:
                        printf("Результат: f(%.3lf) = %.10lf\n", x1, func[choice](x1));
                        puts("------------------------------------------------------");
                        break;
                    case 2:
                        printTab(func[choice], x1, x2, step);
                        break;
                    case 3:
                        buildGraph(func[choice], x1, x2);
                        break;
                }
                break;
        }

        repeatF = 0;
        puts("\nВыберите действие:");
        puts("1) Вернуться к выбору функции");
        puts("2) Вернуться к выбору операции с сохранением выбранной ранее функции");
        puts("3) Повторить последнюю операцию с новыми данными");
        puts("4) Повторить последнюю операцию со старыми данными");
        puts("0) Выйти из программы");
        printf("\nВаш выбор: ");
        scanf("%d", &repeatF);
        puts("------------------------------------------------------");
        printf("\n\n\n\n\n\n\n");

    } while (repeatF);
    return 0;
}

// Функция Y(x) = e^(cos(x)) * cos(sin(x))
double funcY(double x, int g)
{
    return exp(cos(x)) * cos(sin(x));
}

/*------------------------------------------------------------
 * Кусочно-заданная функция V(x):
 *  x > 0.75        -> x + 1
 *  0 <= x <= 0.75  -> 1 - x^5
 *  x < 0           -> x + ln(|cos(x)|)
 *------------------------------------------------------------*/
double funcV(double x, int goal)
{
    if (x > 0.75)
    {
        return x + 1;
    } else if (x >= 0)
    {
        return 1 - pow(x, 5);
    } else
    {
        if (cos(x) != 0)
        {
            return x + log(fabs(cos(x)));
        } else if (cos(x) == 0)
        {
            return NAN;
        }
    }
}


int printTab(TFunc_t pFunc, double x1, double x2, double step)
{
    double y;

    printf("\n-------------------------------------------\n");
    printf("|     x      |           f(x)             |\n");
    printf("-------------------------------------------\n");

    for (double i = x1; i <= x2; i += step)
    {
        y = pFunc(i);
        printf("| %10.3lf | %26.10lf |\n", i, y);
    }

    printf("-------------------------------------------\n");
    return 0;
}


int buildGraph(TFunc_t f, double xStart, double xEnd)
{
    char screen[HEIGHT][WIDTH];
    double x, y[WIDTH];
    double ymin = 1e9, ymax = -1e9;
    double stepX, stepY;
    int xZero, yZero, screenCordY;

    if (xStart > xEnd || xStart == xEnd)
    {
        puts("Ошибка входных данных");
        return -1;
    }
    stepX = (xEnd - xStart) / (WIDTH - 1);

    x = xStart;

    for (int i = 0; i < WIDTH; ++i, x += stepX)
    {
        y[i] = f(x);
        if (!i)
        {
            ymin = y[i];
            ymax = y[i];
        }
        if (y[i] < ymin)
            ymin = y[i];
        if (y[i] > ymax)
            ymax = y[i];
    }

    stepY = (ymax - ymin) / (HEIGHT - 1);
    yZero = (int)floor(ymax / stepY + 0.5);
    xZero = (int)floor((0.0 - xStart) / stepX + 0.5);


    for (int j = 0; j < HEIGHT; ++j)
        for (int i = 0; i < WIDTH; ++i)
            screen[j][i] = (j == yZero ? '-' : (i == xZero ? '|' : ' '));

    int oldScreenCordY = -1;

    for (int i = 0; i < WIDTH; ++i)
    {
        double curr_x = xStart + i * stepX;

        screenCordY = (int)floor((ymax - y[i]) / stepY + 0.5);

        if (screenCordY >= 0 && screenCordY < HEIGHT)
            screen[screenCordY][i] = '*';

        if (i > 0)
        {
            int doConnect = 1;

            if (f == funcV)
            {
                double prevX = xStart + (i - 1) * stepX;
                if (chunkV(prevX) != chunkV(curr_x))
                    doConnect = 0;
            }

            if (doConnect)
            {
                int a = oldScreenCordY;
                int b = screenCordY;
                if (a > b)
                {
                    int t = a;
                    a = b;
                    b = t; 
                }

                for (int k = a; k <= b; ++k)
                    if (k >= 0 && k < HEIGHT)
                        screen[k][i] = '*';
            }
        }

        oldScreenCordY = screenCordY;
    }

    for (int j = 0; j < HEIGHT; ++j)
    {
        for (int i = 0; i < WIDTH; ++i)
            putchar(screen[j][i]);
        putchar('\n');
    }

    return 0;
}

int chunkV(double x)
{
    if (x > 0.75)
        return 3;
    else if (x >= 0)
        return 2;
    else
        return 1;
}


