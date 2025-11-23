#include <stdio.h>
#include <locale.h>
#include <math.h>

#define WIDTH   100     
#define HEIGHT   50     


typedef double (*TFunс_t)(double);

double funcY(double x);
double funcV(double x);

int printTab(TFunс_t pFunc, double x1, double x2, double step);
int buildGraph(TFunс_t f, double xStart, double xEnd);
int chunkV(double x);

int main(void)
{
    setlocale(LC_ALL, "RUS");

    TFunс_t masFunc[2] = { funcY, funcV };
    int choice, opChoice, repeatF = 0, inputEr = 0;
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

            inputEr = 0;
            if (choice < 0 || choice > 1)
            {
                puts("Ошибка: выбран некорректный номер функции.");
                inputEr = 1;
            }
        } while (inputEr);

        do
        {
            puts("\nВыберите действие:");
            puts("1) Вычислить значение функции");
            puts("2) Табулировать функцию");
            puts("3) Построить график функции");
            printf("\nВаш выбор: ");
            scanf("%d", &opChoice);

            inputEr = 0;
            if (opChoice < 1 || opChoice > 3)
            {
                puts("Ошибка: выбран некорректный номер действия.");
                inputEr = 1;
            }
        } while (inputEr);

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
                printf("Введите диапазон через пробел (x1 x2): ");
                scanf("%lf %lf", &x1, &x2);
                buildGraph(masFunc[choice], x1, x2);
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

// Функция Y(x) = e^(cos(x)) * cos(sin(x))
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
    if (x > 0.75)
        return x + 1;
    else if (x >= 0)
        return 1 - pow(x, 5);
    else
    {
        if (cos(x) != 0)
            return x + log(fabs(cos(x)));
        else
            return NAN;
    }
}


// Функция вывода таблицы значений
int printTab(TFunс_t pFunc, double x1, double x2, double step)
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

int buildGraph(TFunс_t f, double xStart, double xEnd)
{
    char screen[HEIGHT][WIDTH];
    double x, y[WIDTH];
    double ymin = 1e9, ymax = -1e9;
    double hx, hy;
    int i, j, xz, yz;

    hx = (xEnd - xStart) / (WIDTH - 1);

    /* вычисление значений функции и поиск min/max */
    for (i = 0, x = xStart; i < WIDTH; ++i, x += hx) {
        y[i] = f(x);
        if (isfinite(y[i])) {
            if (y[i] < ymin) ymin = y[i];
            if (y[i] > ymax) ymax = y[i];
        }
    }

    hy = (ymax - ymin) / (HEIGHT - 1);
    yz = (int)floor(ymax / hy + 0.5);
    xz = (int)floor((0.0 - xStart) / hx + 0.5);

    /* построение осей и фона */
    for (j = 0; j < HEIGHT; ++j)
        for (i = 0; i < WIDTH; ++i)
            screen[j][i] = (j == yz ? '-' : (i == xz ? '|' : ' '));

    /* нанесение точек функции с обработкой разрывов */
    int prev_j = -1;
    int prev_valid = 0;

    for (i = 0; i < WIDTH; ++i) {
        double curr_x = xStart + i * hx;

        j = (int)floor((ymax - y[i]) / hy + 0.5);

        /* рисуем точку */
        if (j >= 0 && j < HEIGHT)
            screen[j][i] = '*';

        /* соединяем с предыдущей точкой только если обе валидны */
        if (i > 0 && prev_valid) {
            int do_connect = 1;

            /* костыль для funcV: не соединяем точки разных кусков */
            if (f == funcV) {
                double prev_x = xStart + (i - 1) * hx;
                if (chunkV(prev_x) != chunkV(curr_x))
                    do_connect = 0;
            }

            if (do_connect) {
                int a = prev_j;
                int b = j;
                if (a > b) { int t = a; a = b; b = t; }

                for (int k = a; k <= b; ++k)
                    if (k >= 0 && k < HEIGHT)
                        screen[k][i] = '*';
            }
        }

        prev_j = j;
        prev_valid = 1;
    }

    /* вывод на экран */
    for (j = 0; j < HEIGHT; ++j) {
        for (i = 0; i < WIDTH; ++i)
            putchar(screen[j][i]);
        putchar('\n');
    }

    return 0;
}
/* функция для определения куска funcV */
int chunkV(double x) {
    if (x > 0.75) return 3;
    else if (x >= 0) return 2;
    else return 1;
}
