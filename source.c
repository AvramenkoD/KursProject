#include <stdio.h>
#include <locale.h>
#include <math.h>

#define WIDTH   100     
#define HEIGHT   50     
#define SIZE1    10     


typedef double (*TFunс)(double);

double funcY(double x);
double funcV(double x);

int printTab(TFunс pFunc, double x1, double x2, double step);
int buildGraph(TFunс f, double xStart, double xEnd, double step);

int main(void)
{
    setlocale(LC_ALL, "RUS");

    TFunс masFunc[2] = { funcY, funcV };
    int choice, opChoice,repeatF = 0;
    double x1, x2, step;

    puts("----------------------------------------");
    puts("| Курсовой проект                      |");
    puts("| Разработка программы анализа функций |");
    puts("|                                      |");
    puts("| Разработал: Д.А. Авраменко           |");
    puts("| Группа: бИПТ-251                     |");
    puts("| Руководитель О.В. Курипта            |");
    puts("----------------------------------------");

    system("pause");

    do
    {
        puts("\nВыберите функцию:");
        puts("0) Y(x) = e^(cos(x)) * cos(sin(x))");
        puts("");
        puts("1) V(x) =");
        puts("   | x > 0.75           -> x + 1");
        puts("   | 0 <= x <= 0.75     -> 1 - x^5");
        puts("   | x < 0              -> x + ln(|cos(x)|)");
        printf("\nВаш выбор: ");
        scanf("%d", &choice);

        if (choice < 0 || choice > 1)
        {
            puts("Ошибка: выбран некорректный номер функции.");
            return 1;
        }

        puts("\nВыберите действие:");
        puts("1) Вычислить значение функции");
        puts("2) Табулировать функцию");
        puts("3) Построить график функции");
        printf("\nВаш выбор: ");
        scanf("%d", &opChoice);

        switch (opChoice)
        {
        case 1:
            printf("\nВведите x: ");
            scanf("%lf", &x1);
            printf("Результат: f(%.3lf) = %.10lf\n", x1, masFunc[choice](x1));
            break;

        case 2:
            printf("Введите диапазон и шаг через пробел (x1 x2 h): ");
            scanf("%lf %lf %lf", &x1, &x2, &step);
            printTab(masFunc[choice], x1, x2, step);
            break;

        case 3:
            printf("Введите диапазон и шаг через пробел (x1 x2 h): ");
            scanf("%lf %lf %lf", &x1, &x2, &step);
            buildGraph(masFunc[choice], x1, x2, step);
            break;
        default:
            puts("Ошибка: выбран некорректный номер действия.");
            break;
        }

        repeatF = 0;
        printf("\n\nЖелаете повторить (0 - нет, иное число - да): ");
        scanf("%d", &repeatF);
        printf("\n\n\n");

    } while (repeatF);
    return 0;
}

/*------------------------------------------------------------
 * Функция Y(x) = e^(cos(x)) * cos(sin(x))
 * Непрерывная, определена при всех x ∈ R
 *------------------------------------------------------------*/
double funcY(double x)
{
    return exp(cos(x)) * cos(sin(x));
}

/*------------------------------------------------------------
 * Кусочно-заданная функция V(x):
 *  x > 0.75        -> x + 1
 *  0 <= x <= 0.75  -> 1 - x^5
 *  x < 0           -> x + ln(|cos(x)|)
 *------------------------------------------------------------*/
double funcV(double x)
{
    const double EPS = 1e-6;

    if (x > 0.75)
        return x + 1;
    else if (x >= 0)
        return 1 - pow(x, 5);
    else {
        /* проверка области допустимых значений */
        if (fabs(cos(x)) < EPS) {
            fprintf(stderr, "Ошибка: недопустимое значение x (cos(x) ≈ 0)\n");
            return NAN;
        }
        return x + log(fabs(cos(x)));
    }
}

/*------------------------------------------------------------
 * Функция вывода таблицы значений
 *------------------------------------------------------------*/
int printTab(TFunс pFunc, double x1, double x2, double step)
{
    double y;

    printf("-------------------------------------------\n");
    printf("|     x      |           f(x)             |\n");
    printf("-------------------------------------------\n");

    for (double i = x1; i <= x2; i += step) {
        y = pFunc(i);
        printf("| %9.3lf | %23.10lf |\n", i, y);
    }

    printf("-------------------------------------------\n");
    return 0;
}

/*------------------------------------------------------------
 * Функция построения графика функции в консоли
 *------------------------------------------------------------*/
int buildGraph(TFunс f, double xStart, double xEnd, double step)
{
    char screen[HEIGHT][WIDTH];
    double x, y[WIDTH];
    double ymin = 0, ymax = 0;
    double hx, hy;
    int i, j, xz, yz;

    hx = (xEnd - xStart) / (WIDTH - 1);

    /* вычисление значений функции и поиск минимума/максимума */
    for (i = 0, x = xStart; i < WIDTH; ++i, x += hx) {
        y[i] = f(x);
        if (y[i] < ymin) ymin = y[i];
        if (y[i] > ymax) ymax = y[i];
    }

    hy = (ymax - ymin) / (HEIGHT - 1);
    yz = (int)floor(ymax / hy + 0.5);
    xz = (int)floor((0.0 - xStart) / hx + 0.5);

    /* построение осей и фона */
    for (j = 0; j < HEIGHT; ++j)
        for (i = 0; i < WIDTH; ++i)
            screen[j][i] = (j == yz) ? '-' : (i == xz ? '|' : ' ');

    /* нанесение точек функции */
    for (i = 0; i < WIDTH; ++i) {
        j = (int)floor((ymax - y[i]) / hy + 0.5);
        if (j >= 0 && j < HEIGHT)
            screen[j][i] = '*';
    }

    /* вывод на экран */
    for (j = 0; j < HEIGHT; ++j) {
        for (i = 0; i < WIDTH; ++i)
            putchar(screen[j][i]);
        putchar('\n');
    }

    return 0;
}
