#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <stdlib.h>

//Работа с массивом(пункт меню) загрузить данные из файла, создать новые(из табулирования)
//отсортировать массив(убывание / возастание), алгоритм(расчёска, гномья)
//найти все отрицательные элементы и вывести остортированные

#define WIDTH   100     
#define HEIGHT   50
#define SFILENAME "ARRAY.txt"

typedef double (*TFunc_t)(double);

double funcY(double);
double funcV(double);

double* printTab(TFunc_t, double, double, double);
int buildGraph(TFunc_t, double, double);
int chunkV(double);
int workMas(TFunc_t,int);
double* combSort(double*, int, int);
double* gnomeSort(double*, int, int);
int printArray(double*, int);
int checkInputEr(int, int);


int main(void)
{
    setlocale(LC_ALL, "RUS");
    int choice, opChoice,objChoice, repeatF = 1, inputEr;
    double x1, x2, step,*array;
    TFunc_t func[2] = { funcY, funcV };

    puts("\t-----------------------------------------");
    puts("\t| Курсовой проект                       |");
    puts("\t| Разработка программы анализа функций  |");
    puts("\t|                                       |");
    puts("\t| Разработал: Д.А. Авраменко            |");
    puts("\t| Группа: бИПТ-251                      |");
    puts("\t| Руководитель О.В. Курипта             |");
    puts("  ------------------------------------------------------");
    puts("  |             Функции                                |");
    puts("  | 1) Y(x) = e^(cos(x)) * cos(sin(x))                 |");
    puts("  |                                                    |");
    puts("  |           при x > 0.75         -> x + 1            |");
    puts("  | 2) V(x) = при 0 <= x <= 0.75   -> 1 - x^5          |");
    puts("  |           при x < 0            -> x + ln(|cos(x)|) |");
    puts("  ------------------------------------------------------");

    do
    {
        switch (repeatF)
        {
            case 1:
                printf("\nВыберите функцию\n");
                choice = checkInputEr(1, 2) - 1;
            case 2: 
                puts("Выберите объёкта с которым хотите работать:");
                puts("1) Функция (график, табулирование, значение в точке)");
                puts("2) Массив (сортировка, вывести отрицательные элементы)");
                objChoice = checkInputEr(1, 2);
            case 3:
                if (objChoice == 1)
                {
                    puts("Выберите операцию:");
                    puts("1) Вычислить значение функции в точке");
                    puts("2) Протабулировать функцию на интервале");
                    puts("3) Построить график функции на интервале");
                    opChoice = checkInputEr(1, 3);
                }
                else
                    workMas(func[choice],repeatF);
            case 4:
                if (objChoice == 1) {
                    switch (opChoice)
                    {
                    case 1:
                        printf("\nВведите x: ");
                        scanf("%lf", &x1);
                        if (func[choice](x1) != NAN)
                            printf("\nРезультат: f(%.3lf) = %.10lf\n", x1, func[choice](x1));
                        else
                            puts("Функция не существует в данной точке.");
                        break;
                    case 2:
                        printf("Введите диапазон и шаг через пробел (начало конец шаг): ");
                        scanf("%lf %lf %lf", &x1, &x2, &step);
                        array = printTab(func[choice], x1, x2, step);
                        free(array);
                        break;
                    case 3:
                        printf("Введите диапазон через пробел (началр конец): ");
                        scanf("%lf %lf", &x1, &x2);
                        buildGraph(func[choice], x1, x2);
                        break;
                    }
                }
                break;
            case 5:
                if (objChoice == 1) {
                    switch (opChoice)
                    {
                        case 1:
                            printf("Результат: f(%.3lf) = %.10lf\n", x1, func[choice](x1));
                            break;
                        case 2:
                            array = printTab(func[choice], x1, x2, step);
                            free(array);
                            break;
                        case 3:
                            buildGraph(func[choice], x1, x2);
                            break;
                    }
                }
                break;
        }

        if (objChoice == 1)
        {
            printf("\n\n------------------------------------------------------\n");
            repeatF = 0;

            puts("Выберите действие:");
            puts("1) Вернуться к выбору функции");
            puts("2) Вернуться к выбору объекта с которым будет проводиться работа");
            puts("3) Вернуться к выбору операции");
            puts("4) Повторить последнюю операцию с новыми данными");
            puts("5) Повторить последнюю операцию со старыми данными");
            puts("0) Выйти из программы");
            repeatF = checkInputEr(1, 5);

            puts("------------------------------------------------------");
            printf("\n\n\n\n\n\n\n");
        }

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
    {
        return x + 1;
    }
    else if (x >= 0)
    {
        return 1 - pow(x, 5);
    }
    else
    {
        if (cos(x) != 0)
        {
            return x + log(fabs(cos(x)));
        }
        else
        {
            return NAN;
        }
    }
}

double* printTab(TFunc_t pFunc, double x1, double x2, double step)
{
    int cnt = 0;
    double y;
    double* array = malloc(sizeof(double) * ((x2 - x1)/ step) + 1);

    printf("\n-------------------------------------------\n");
    printf("|     x      |           f(x)             |\n");
    printf("-------------------------------------------\n");

    for (double i = x1; i <= x2; i += step)
    {
        y = pFunc(i);
        printf("| %10.3lf | %26.3lf |\n", i, y);

        array[cnt] = y;
        cnt++;
    }

    printf("-------------------------------------------\n");
    return array;
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
                double oldX = xStart + (i - 1) * stepX;
                if (chunkV(oldX) != chunkV(curr_x))
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

//Работа с массивом(пункт меню) загрузить данные из файла, создать новые(из табулирования)
//отсортировать массив(убывание / возастание), алгоритм(расчёска, гномья)
//найти все отрицательные элементы и вывести остортированные.
int workMas(TFunc_t func, int repeatF)
{
    FILE* file;
    int inputErL, choise1 = 0, choise2 = 0, size = 0, cnt;
    double* array = 0, start, end, step,temp;
    do
    {
        if (repeatF <= 3)
        {
            printf("\nВы хотите:\n1) Загрузить массив из файла\n2) Создать новый\n");
            choise1 = checkInputEr(1, 2);

            switch (choise1)
            {
            case 1:
                if ((file = fopen(SFILENAME, "r")) != NULL)
                {
                    while (fscanf(file, "%lf", &temp) == 1)
                        size++;
                    fclose(file);
                    if (!size)
                    {
                        printf("\nФайл пуст. Переходим к созданию нового файла\n");
                    }
                    else
                    {
                        array = malloc(sizeof(double) * size);
                        if (array == NULL)
                            return -1;

                        file = fopen(SFILENAME, "r");
                        for (int i = 0; i < size; i++)
                            fscanf(file, "%lf", &array[i]);
                        fclose(file);
                        break;
                    }
                }
                else
                    printf("\nФайл не существует. Переходим к созданию нового файла\n");
            case 2:
                file = fopen(SFILENAME, "w");
                printf("Для генерации данных, далее записанных в файл, будет протабулированна функция.");
                printf("\nВведите диапазон и шаг через для табуляции (начало конец шаг): ");
                scanf("%lf %lf %lf", &start, &end, &step);
                array = printTab(func, start, end, step);
                size = (int)(end - start) / step + 1;

                for (int i = 0; i < size; i++)
                {
                    fprintf(file, "%lf ", array[i]);
                }
                fclose(file);
                puts("Столбец значений функции был записан в файл");
                break;
            }
        }

        printf("\nЧто вы хотите сделать с массивом:\n1) Отсортировать\n2) Вывести все отрицательные элементы\n");
        choise2 = checkInputEr(1, 2);

        if (choise2 == 1) {
            printf("\nКаким алгоритмом сортировки вы хотите отсортировать массив:");
            printf("\n1) Сортировка расчёской\n2) Гномья сортировка\n");
            choise1 = checkInputEr(1, 2);

            printf("\nКак вы хотите отсортировать массив:");
            printf("\n1) По убыванию\n2) По возрастанию\n");
            choise2 = checkInputEr(1, 2);

            printf("\nИзначальный массив:\n");
            printArray(array, size);

            if (choise1 == 1)
                combSort(array, size, choise2);
            else if (choise1 == 2)
                gnomeSort(array, size, choise2);

            printf("\nОтсортированный массив:\n");
            printArray(array, size);

            printf("\n\nCохранить отсортированный массив в файл?\n1) Да\n2) Нет\n");
            choise1 = checkInputEr(1, 2);

            if (choise1 == 1)
            {
                file = fopen(SFILENAME, "w");
                for (int i = 0; i < size; i++)
                {
                    fprintf(file, "%lf ", array[i]);
                }
                fclose(file);
            }
        }
        else if (choise2 == 2)
        {
            printf("\nКак вы хотите вывести элементы:");
            printf("\n1) По убыванию\n2) По возрастанию\n");
            choise1 = checkInputEr(1, 2);

            printf("\nИзначальный массив:\n");
            printArray(array, size);

            combSort(array, size, choise1);

            cnt = 0;
            for (int i = 0; i < size; i++)
            {
                if (array[i] < 0)
                {
                    cnt++;
                }
            }
            if (cnt)
            {
                printf("\n\nОтсортированные отрицательные элементы:\n");
                for (int i = 0; i < size; i++)
                {
                    if (array[i] < 0)
                    {
                        printf("%lf ", array[i]);
                    }
                }
            }
            else
                printf("\n\nВ массиве нет отрицательных элемментов.");
        }

        printf("\n\n------------------------------------------------------\n");

        puts("Выберите действие:");
        puts("1) Вернуться к выбору функции");
        puts("2) Вернуться к выбору объекта с которым будет проводиться работа");
        puts("3) Вернуться к выбору загрузки данных в массив");
        puts("4) Вернуться к выбору операции над массивом");
        puts("0) Выйти из программы");
        repeatF = checkInputEr(1, 4);

        puts("------------------------------------------------------");
        printf("\n\n\n\n\n\n\n");
    } while (repeatF > 2);

    free(array);
    return repeatF;
}

int checkInputEr(int min, int max)
{
    int input = 0;
    int inputErL;

    do
    {
        printf("\nВаш выбор: ");
        scanf("%d", &input);

        inputErL = 0;
        if (input < min || input > max)
        {
            printf("\n------------------------------------------------------\n\n");
            puts("Ошибка: выбран некорректный номер.");
            inputErL = 1;
        }
        printf("\n------------------------------------------------------\n\n");
    } while (inputErL);

    return input;
}

double* combSort(double* array, int size, int h)
{
    double temp;
    int step = size, cnt = 0;
    switch (h)
    {
        case 1:
            while (step > 1 || cnt)
            {
                cnt = 0;
                step /= 1.247;
                step = step < 1 ? 1 : step;

                for (int i = 0; i + step < size; i++)
                {
                    if (array[i] < array[i + step])
                    {
                        temp = array[i];
                        array[i] = array[i + step];
                        array[i + step] = temp;
                        cnt++;
                    }
                }
            }
        case 2:
            while (step > 1 || cnt)
            {
                cnt = 0;
                step /= 1.247;
                step = step < 1 ? 1 : step;

                for (int i = 0; i + step < size; i++)
                {
                    if (array[i] > array[i + step])
                    {
                        temp = array[i];
                        array[i] = array[i + step];
                        array[i + step] = temp;
                        cnt++;
                    }
                }
            }
    }
    return array;
}

double* gnomeSort(double* array, int size, int h)
{
    double temp;
    int i = 1;

    if (h == 1) {
        while (i < size)
        {
            if (i == 0 || array[i - 1] >= array[i])
            {
                i++;
            }
            else
            {
                temp = array[i];
                array[i] = array[i - 1];
                array[i - 1] = temp;
                i--;
            }
        }
    }
    else if (h == 2)
    {
        while (i < size)
        {
            if (i == 0 || array[i - 1] <= array[i])
            {
                i++;
            }
            else
            {
                temp = array[i];
                array[i] = array[i - 1];
                array[i - 1] = temp;
                i--;
            }
        }
    }
    return array;
}

int printArray(double* array, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%.3lf ", array[i]);
    }

    return 0;
}

