#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <stdlib.h>

#define WIDTH   100     
#define HEIGHT   40
#define SFILENAME "ARRAY.txt"

typedef double (*TFunc_t)(double);

double funcY(double);
double funcV(double);

int printTab(TFunc_t, double, double, double);
double* printTabForArray(TFunc_t, double, double, double);
int buildGraph(TFunc_t, double, double);
int chunkV(double);
double* combSort(double*, int, int);
double* gnomeSort(double*, int, int);
int printArray(double*, int);
int checkInputEr(int, int);
double* load_datafile(int, int*, double, double, double, TFunc_t);


int main(void)
{
    //function
    setlocale(LC_ALL, "RUS");
    int choice, opChoice,objChoice, repeatF = 1, inputEr;
    double x1 = 0, x2 = 0, step = 0;
    TFunc_t func[2] = { funcY, funcV };

    //Array
    int a_choice, a_opChoise, a_choiceSort[2],a_saveChoise[2],a_choiceOut, size, cnt;
    double* array = 0;
    char name[100];

    puts("\t\t-----------------------------------------");
    puts("\t\t| Курсовой проект                       |");
    puts("\t\t| Разработка программы анализа функций  |");
    puts("\t\t|                                       |");
    puts("\t\t| Разработал: Д.А. Авраменко            |");
    puts("\t\t| Группа: бИПТ-251                      |");
    puts("\t\t| Руководитель О.В. Курипта             |");
    puts("\t  ------------------------------------------------------");
    puts("\t  |             Функции                                |");
    puts("\t  | 1) Y(x) = e^(cos(x)) * cos(sin(x))                 |");
    puts("\t  |                                                    |");
    puts("\t  |           при x > 0.75         -> x + 1            |");
    puts("\t  | 2) V(x) = при 0 <= x <= 0.75   -> 1 - x^5          |");
    puts("\t  |           при x < 0            -> x + ln(|cos(x)|) |");
    puts("\t  ------------------------------------------------------");

    do
    {
        switch (repeatF)
        {
            case 1:
                printf("\n\tВыберите функцию\n");
                choice = checkInputEr(1, 2) - 1;
            case 2: 
                puts("\tВыберите объёкта с которым хотите работать:");
                puts("\t1) Функция (график, табулирование, значение в точке)");
                puts("\t2) Массив (сортировка, вывести отрицательные элементы)");
                objChoice = checkInputEr(1, 2);
            case 3:
                if (objChoice == 1)
                {
                    puts("\tВыберите операцию:");
                    puts("\t1) Вычислить значение функции в точке");
                    puts("\t2) Протабулировать функцию на интервале");
                    puts("\t3) Построить график функции на интервале");
                    opChoice = checkInputEr(1, 3);
                }
                else
                {
                    printf("\n\tВы хотите:\n\t1) Загрузить массив из файла\n\t2) Создать новый\n");// Имя файл должен указывать пользователь??
                    a_choice = checkInputEr(1, 2);

                    if (a_choice == 2 || (a_choice == 1 && (array = load_datafile(a_choice, &size, x1, x2, step, func[choice])) == NULL))
                    {
                        printf("\tДля генерации данных, далее записанных в файл, будет протабулированна функция.");
                        printf("\n\tВведите диапазон и шаг через для табуляции (начало конец шаг): ");
                        scanf("\t%lf %lf %lf", &x1, &x2, &step);
                        array = load_datafile(2, &size, x1, x2, step, func[choice]);
                    }
                    if (array == NULL)
                        return -1;
                }                 
            case 4:
                if (objChoice == 1) {
                    switch (opChoice)
                    {
                        case 1:
                            printf("\n\tВведите x: ");
                            scanf("\t%lf", &x1);
                            if (func[choice](x1) != NAN)
                                printf("\n\tРезультат: f(%.3lf) = %.10lf\n", x1, func[choice](x1));
                            else
                                puts("\tФункция не существует в данной точке.");
                            break;
                        case 2:
                            printf("\tВведите диапазон и шаг через пробел (начало конец шаг): ");
                            scanf("\t%lf %lf %lf", &x1, &x2, &step);
                            printTab(func[choice], x1, x2, step);
                            break;
                        case 3:
                            printf("\tВведите диапазон через пробел (началр конец): ");
                            scanf("\t%lf %lf", &x1, &x2);
                            buildGraph(func[choice], x1, x2);
                            break;
                    }
                }
                else
                {
                    printf("\n\tЧто вы хотите сделать с массивом:\n\t1) Отсортировать\n\t2) Вывести все отрицательные элементы\n");
                    a_opChoise = checkInputEr(1, 2);

                    if (a_opChoise == 1) {
                        printf("\n\tКаким алгоритмом сортировки вы хотите отсортировать массив:");
                        printf("\n\t1) Сортировка расчёской\n\t2) Гномья сортировка\n");
                        a_choiceSort[0] = checkInputEr(1, 2);

                        printf("\n\tКак вы хотите отсортировать массив:");
                        printf("\n\t1) По убыванию\n\t2) По возрастанию\n");
                        a_choiceSort[1] = checkInputEr(1, 2);

                        printf("\n\tИзначальный массив:\n");
                        printArray(array, size);

                        if (a_choiceSort[0] == 1)
                            combSort(array, size, a_choiceSort[1]);
                        else if (a_choiceSort[0] == 2)
                            gnomeSort(array, size, a_choiceSort[1]);

                        printf("\n\tОтсортированный массив:\n");
                        printArray(array, size);

                        printf("\n\n\tCохранить отсортированный массив в файл?\n\t1) Да\n\t2) Нет\n");
                        a_saveChoise[0] = checkInputEr(1, 2);

                        if (a_saveChoise == 1)
                        {
                            printf("\n\n\tCохранить в новый файл или добавить в старый файл?\n\t1) Новый\n\t2) Старый\n");
                            a_saveChoise[1] = checkInputEr(1, 2);

                            if (a_saveChoise[1] == 1)
                            {        
                                printf("\n\n\tВведите название нового файла: ");
                                scanf("%s", &name);
                            }
                            save_toFile(name, array,size,a_saveChoise[1]);
                        }
                    }
                    else if (a_opChoise == 2)
                    {
                        printf("\n\tКак вы хотите вывести элементы:");
                        printf("\n\t1) По убыванию\n\t2) По возрастанию\n");
                        a_choiceOut = checkInputEr(1, 2);

                        printf("\n\tИзначальный массив:\n");
                        printArray(array, size);

                        combSort(array, size, a_choiceOut);

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
                            printf("\n\n\tОтсортированные отрицательные элементы:\n\t");
                            for (int i = 0; i < size; i++)
                            {
                                if (array[i] < 0)
                                {
                                    printf("%.3lf ", array[i]);
                                }
                            }
                        }
                        else
                            printf("\n\n\tВ массиве нет отрицательных элемментов.");
                    }
                }
                break;
            case 5:
                if (objChoice == 1) {
                    switch (opChoice)
                    {
                        case 1:
                            printf("\tРезультат: f(%.3lf) = %.10lf\n", x1, func[choice](x1));
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
            printf("\n\n\t------------------------------------------------------\n");
            repeatF = 0;

            puts("\tВыберите действие:");
            puts("\t1) Вернуться к выбору функции");
            puts("\t2) Вернуться к выбору объекта с которым будет проводиться работа");
            puts("\t3) Вернуться к выбору операции");
            puts("\t4) Повторить последнюю операцию с новыми данными");
            puts("\t5) Повторить последнюю операцию со старыми данными");
            puts("\t0) Выйти из программы");
            repeatF = checkInputEr(1, 5);

            puts("\t------------------------------------------------------");
            printf("\n\n\n\n\n\n\n");
        }
        else
        {
            printf("\n\n\t------------------------------------------------------\n");

            puts("\tВыберите действие:");
            puts("\t1) Вернуться к выбору функции");
            puts("\t2) Вернуться к выбору объекта с которым будет проводиться работа");
            puts("\t3) Вернуться к выбору загрузки данных в массив");
            puts("\t4) Вернуться к выбору операции над массивом");
            puts("\t0) Выйти из программы");
            repeatF = checkInputEr(1, 4);

            puts("\t------------------------------------------------------");
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

int printTab(TFunc_t pFunc, double x1, double x2, double step)
{
    double y;

    printf("\n\t-------------------------------------------\n");
    printf("\t|     x      |           f(x)             |\n");
    printf("\t-------------------------------------------\n");

    for (double i = x1; i <= x2; i += step)
    {
        y = pFunc(i);
        printf("\t| %10.3lf | %26.3lf |\n", i, y);
    }

    printf("\t-------------------------------------------\n");
    return 0;
}

int buildGraph(TFunc_t f, double xStart, double xEnd)
{
    char screen[HEIGHT][WIDTH];
    double x, y[WIDTH];
    double ymin, ymax;
    double stepX, stepY;
    int xZero, yZero, screenCordY;

    if (xStart > xEnd || xStart == xEnd)
    {
        printf("\n\tОшибка входных данных");
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
        putchar('\t');
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

//functions for array

int checkInputEr(int min, int max)
{
    int input = 0;
    int inputErL;

    do
    {
        printf("\n\tВаш выбор: ");
        scanf("\t%d", &input);

        inputErL = 0;
        if (input < min || input > max)
        {
            printf("\n\t------------------------------------------------------\n\n");
            puts("\tОшибка: выбран некорректный номер.");
            inputErL = 1;
        }
        printf("\n\t------------------------------------------------------\n\n");
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
    printf("\t");
    for (int i = 0; i < size; i++)
    {
        printf("%.3lf ", array[i]);
    }

    return 0;
}

double* load_datafile(int choice, int* inSize, double start, double end, double step, TFunc_t func)
{
    FILE* file;
    double temp,*array = 0;
    int size = 0;

    switch (choice)
    {
        case 1:
            if ((file = fopen(SFILENAME, "r")) != NULL)
            {
                while (fscanf(file, "%lf", &temp) == 1)
                    size++;
                fclose(file);

                if (!size)
                {
                    printf("\n\tФайл пуст. Переходим к созданию нового файла\n");
                    return -2;
                }
                else
                {
                    array = malloc(sizeof(double) * size);
                    if (array == NULL)
                        return NULL;

                    file = fopen(SFILENAME, "r");
                    for (int i = 0; i < size; i++)
                        fscanf(file, "%lf", &array[i]);

                    fclose(file);
                    break;
                }
            }
            else
            {
                printf("\n\tФайл не существует. Переходим к созданию нового файла\n");
                return NULL;
            }
            break;
         case 2:
            file = fopen(SFILENAME, "w");

            array = printTabForArray(func, start, end, step);
            size = (int)(end - start) / step + 1;

            for (int i = 0; i < size; i++)
            {
                fprintf(file, "%lf ", array[i]);
            }

            fclose(file);
            printf("\n\tСтолбец значений функции был записан в файл");
            printf("\n\t-------------------------------------------\n");
            break;
    }
    *inSize = size;
    return array;
}

int save_toFile(char* name, double* array, int size,int choice)
{
    FILE* file;

    if (choice == 1)
    {
        if (file = fopen(SFILENAME, "w") == NULL)
            return -1;
        for (int i = 0; i < size; i++)
        {
            fprintf(file, "%lf ", array[i]);
        }
        fclose(file);
    }
    else
    {
        if (file = fopen(SFILENAME, "a") == NULL)
            return -1;
        for (int i = 0; i < size; i++)
        {
            fprintf(file, "%lf ", array[i]);
        }
        fclose(file);
    }
    return 0;
}

double* printTabForArray(TFunc_t pFunc, double x1, double x2, double step)
{
    int cnt = 0;
    double y;
    double* array = (double*)malloc(sizeof(double) * (int)ceil(((x2 - x1) / step))+1);
    if (array == NULL)
        return NULL;

    printf("\n\t-------------------------------------------\n");
    printf("\t|     x      |           f(x)             |\n");
    printf("\t-------------------------------------------\n");

    for (double i = x1; i <= x2; i += step)
    {
        y = pFunc(i);
        printf("\t| %10.3lf | %26.3lf |\n", i, y);

        array[cnt] = y;
        cnt++;
    }

    printf("\t-------------------------------------------\n");
    return array;
}