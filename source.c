#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <stdlib.h>

#define WIDTH   100     
#define HEIGHT   40

typedef double (*TFunc_t)(double);

double funcY(double);
double funcV(double);

int printTab(TFunc_t, double, double, double);
double* TabForArray(double*,TFunc_t, double, double, double);
int buildGraph(TFunc_t, double, double);
int chunkV(double);
double* combSort(double*, int, int);
double* gnomeSort(double*, int, int);
int printArray(double*, int);
double* load_datafile(double*,char*, int*);
double* generate_NewData(double*, char*, double, double, double, TFunc_t, int*);

int main(void)
{
    //function
    setlocale(LC_ALL, "RUS");
    int f_choise, op_choise,obj_choise, repeatF = 1, inputEr;
    double x1 = 0, x2 = 0, step = 0;
    TFunc_t func = NULL;

    //Array
    int a_choise, a_opChoise, a_choiceSort[2],a_saveChoise[2],a_choiceOut, size, cnt;
    double* array = (double*)malloc(sizeof(double));
    char nameIn[100];
    char nameOut[100];

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

                printf("\n\tВаш выбор: ");
                scanf("\t%d", &f_choise);
                printf("\t-------------------------------------------\n");

                func = f_choise == 1 ? funcY : funcV;
            case 2: 
                puts("\tВыберите объёкта с которым хотите работать:");
                puts("\t1) Функция (график, табулирование, значение в точке)");
                puts("\t2) Массив (сортировка, вывести отрицательные элементы)");

                printf("\n\tВаш выбор: ");
                scanf("\t%d", &obj_choise);
                printf("\t-------------------------------------------\n");
            case 3:
                if (obj_choise == 1)
                {
                    puts("\tВыберите операцию:");
                    puts("\t1) Вычислить значение функции в точке");
                    puts("\t2) Протабулировать функцию на интервале");
                    puts("\t3) Построить график функции на интервале");

                    printf("\n\tВаш выбор: ");
                    scanf("\t%d", &op_choise);
                    printf("\t-------------------------------------------\n");
                }
                else
                {
                    printf("\n\tВы хотите:\n\t1) Загрузить массив из файла\n\t2) Создать новый\n");// Имя файл должен указывать пользователь??

                    printf("\n\tВаш выбор: ");
                    scanf("\t%d", &a_choise);
                    printf("\t-------------------------------------------\n");

                    printf("\n\n\tВведите название файла (меньше 50 символов): ");
                    scanf("%s", &nameIn);
                    sprintf(nameOut, "%s.txt", nameIn);
                    printf("\t-------------------------------------------\n");

                    if (a_choise == 1)
                        array = load_datafile(array, nameOut, &size);

                    if (a_choise == 2 || array == NULL)
                    {
                        printf("\tДля генерации данных, далее записанных в файл, будет протабулированна функция.");
                        printf("\n\tВведите диапазон и шаг через для табуляции (начало конец шаг): ");
                        scanf("\t%lf %lf %lf", &x1, &x2, &step);
                        array = generate_NewData(array,nameOut, x1, x2, step, func, &size);
                    }

                    if (array == NULL)
                        return -1;
                }                 
            case 4:
                if (obj_choise == 1) {
                    switch (op_choise)
                    {
                        case 1:
                            printf("\n\tВведите x: ");
                            scanf("\t%lf", &x1);
                            if (func == funcV && cos(x1) == 0)
                                puts("\tФункция не существует в данной точке.");
                            else
                                printf("\n\tРезультат: f(%.3lf) = %.10lf\n", x1, func(x1));
                            break;
                        case 2:
                            printf("\tВведите диапазон и шаг через пробел (начало конец шаг): ");
                            scanf("\t%lf %lf %lf", &x1, &x2, &step);
                            printTab(func, x1, x2, step);
                            break;
                        case 3:
                            printf("\tВведите диапазон через пробел (начало конец): ");
                            scanf("\t%lf %lf", &x1, &x2);
                            buildGraph(func, x1, x2);
                            break;
                    }
                }
                else
                {
                    printf("\n\tЧто вы хотите сделать с массивом:\n\t1) Отсортировать\n\t2) Вывести все отрицательные элементы\n");
                    printf("\n\tВаш выбор: ");
                    scanf("\t%d", &a_opChoise);
                    printf("\t-------------------------------------------\n");

                    if (a_opChoise == 1) {
                        printf("\n\tКаким алгоритмом сортировки вы хотите отсортировать массив:");
                        printf("\n\t1) Сортировка расчёской\n\t2) Гномья сортировка\n");

                        printf("\n\tВаш выбор: ");
                        scanf("\t%d", &a_choiceSort[0]);
                        printf("\t-------------------------------------------\n");

                        printf("\n\tКак вы хотите отсортировать массив:");
                        printf("\n\t1) По убыванию\n\t2) По возрастанию\n");

                        printf("\n\tВаш выбор: ");
                        scanf("\t%d", &a_choiceSort[1]);
                        printf("\t-------------------------------------------\n");

                        printf("\n\tИзначальный массив:\n");
                        printArray(array, size);

                        if (a_choiceSort[0] == 1)
                            combSort(array, size, a_choiceSort[1]);
                        else
                            gnomeSort(array, size, a_choiceSort[1]);

                        printf("\n\n\tОтсортированный массив:\n");
                        printArray(array, size);

                        printf("\n\n\tCохранить отсортированный массив в файл?\n\t1) Да\n\t2) Нет\n");

                        printf("\n\tВаш выбор: ");
                        scanf("\t%d", &a_saveChoise[0]);
                        printf("\t-------------------------------------------\n");

                        if (a_saveChoise[0] == 1)
                        {
                            printf("\n\n\tCохранить в новый файл или добавить в старый файл?\n\t1) Новый\n\t2) Старый\n");

                            printf("\n\tВаш выбор: ");
                            scanf("\t%d", &a_saveChoise[1]);
                            printf("\t-------------------------------------------\n");

                            if (a_saveChoise[1] == 1)
                            {        
                                printf("\n\n\tВведите название нового файла (меньше 50 символов): ");
                                scanf("%s", &nameIn);
                                sprintf(nameOut, "%s.txt", nameIn);

                            }
                            save_toFile(nameOut, array,size,a_saveChoise[1]);
                        }
                    }
                    else
                    {
                        printf("\n\tКак вы хотите вывести элементы:");
                        printf("\n\t1) По убыванию\n\t2) По возрастанию\n");

                        printf("\n\tВаш выбор: ");
                        scanf("\t%d", &a_choiceOut);
                        printf("\t-------------------------------------------\n");

                        printf("\n\tИзначальный массив:\n");
                        printArray(array, size);

                        combSort(array, size, a_choiceOut);

                        cnt = 0;
                        printf("\n\n\tОтсортированные отрицательные элементы:\n\t");
                        for (int i = 0; i < size; i++)
                        {
                            if (array[i] < 0)
                            {
                                cnt++;
                                printf("%.3lf ", array[i]);
                            }
                        }
                        if (cnt == 0)
                            printf("\n\n\tВ массиве нет отрицательных элемментов.");
                    }
                }
                break;
            case 5:
                if (obj_choise == 1) {
                    switch (op_choise)
                    {
                        case 1:
                            if (cos(x1) != 0)
                                printf("\n\tРезультат: f(%.3lf) = %.10lf\n", x1, func(x1));
                            else
                                puts("\tФункция не существует в данной точке.");
                            break;
                        case 2:
                            array = printTab(func, x1, x2, step);
                            free(array);
                            break;
                        case 3:
                            buildGraph(func, x1, x2);
                            break;
                    }
                }
                break;
        }

        if (obj_choise == 1)
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

            printf("\n\tВаш выбор: ");
            scanf("\t%d", &repeatF);

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

            printf("\n\tВаш выбор: ");
            scanf("\t%d", &repeatF);

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
            return x + log(fabs(cos(x)));
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
        if (pFunc == funcV && cos(i) == 0)
            printf("\t| %10.3lf |             Не существует |\n", i);
        else
        {
            y = pFunc(i);
            printf("\t| %10.3lf | %26.3lf |\n", i, y);
        }
    }

    printf("\t-------------------------------------------\n");
    return 0;
}

int buildGraph(TFunc_t f, double xStart, double xEnd)
{
    char screen[HEIGHT][WIDTH];
    double x, y[WIDTH];
    double ymin, ymax;
    double stepX, stepY, curr_x;
    int xZero, yZero, screenCordY,oldScreenCordY;

    if (xStart > xEnd || xStart == xEnd)
    {
        printf("\n\tОшибка входных данных");
        return -1;
    }
    stepX = (xEnd - xStart) / (WIDTH - 1);

    x = xStart;

    for (int i = 0; i < WIDTH; ++i, x += stepX)
    {
        if (f == funcV && x < 0 && cos(x) == 0)
            y[i] = 1;
        else
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

    oldScreenCordY = -1;

    for (int i = 0; i < WIDTH; ++i)
    {
        curr_x = xStart + i * stepX;

        screenCordY = (int)floor((ymax - y[i]) / stepY + 0.5);
        
        // Заполнение массива + обработка ОДЗ
        if (screenCordY >= 0 && screenCordY < HEIGHT)
        {
            if (f == funcV && curr_x < 0 && y[i] == 1)
                screen[screenCordY][i] = ' ';
            else
                screen[screenCordY][i] = '*';
        }

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
                int temp;
                if (a > b)
                {
                    temp = a;
                    a = b;
                    b = temp; 
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
        printf("\t");
        for (int i = 0; i < WIDTH; ++i)
            printf("%c",screen[j][i]);
        printf("\n");
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

//Расчёской (англ. comb)
double* combSort(double* array, int size, int h)
{
    double temp;
    int step = size, cnt = 0;
    if (h == 1)
    {
        while (step > 1 || cnt)
        {
            cnt = 0;
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
            step /= 1.247;
        }
    }
    else
    {
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

// Гномья
double* gnomeSort(double* array, int size, int h)
{
    double temp;
    int i = 1;
    //Убывание
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
    else// Возрастание
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

double* load_datafile(double* array,char* name, int* inSize)
{
    FILE* file;
    double temp,*tempPtr = 0;
    int size = 0;
    char name1[100];
    sprintf(name1,"%s.txt",name);

    if ((file = fopen(name1, "r")) != NULL)
    {
        while (fscanf(file, "%lf", &temp) == 1)
            size++;
        fclose(file);

        if (!size)
        {
            printf("\n\tФайл пуст. Переходим к созданию нового файла\n");
            return NULL;
        }
        else
        {
            tempPtr = (double*)realloc(array,sizeof(double) * size);
            if (tempPtr == NULL)
                return NULL;
            array = tempPtr;

            file = fopen(name1, "r");
            for (int i = 0; i < size; i++)
                fscanf(file, "%lf", &array[i]);

            fclose(file);
        }
    }
    else
    {
        printf("\n\tФайл не существует. Переходим к созданию нового файла\n");
        return NULL;
    }
    *inSize = size;
    return array;
}

double* generate_NewData(double* array, char* name,double start, double end, double step, TFunc_t func, int* inSize)
{
    FILE* file;
    double temp, *tempPtr = NULL;
    int size = 0;
    char name1[100];
    sprintf(name1, "%s.txt", name);

    file = fopen(name1, "w");

    array = TabForArray(array, func, start, end, step);
    size = (int)(end - start) / step + 1;

    for (int i = 0; i < size; i++)
    {
        fprintf(file, "%lf ", array[i]);
    }

    fclose(file);
    printf("\n\tСтолбец значений функции был записан в файл");
    printf("\n\t-------------------------------------------\n");

    *inSize = size;
    return array;
}

int save_toFile(char* name, double* array, int size,int choice)
{
    FILE* file;

    if (choice == 1)
    {
        if ((file = fopen(name, "w")) == NULL)
            return -1;
        for (int i = 0; i < size; i++)
        {
            fprintf(file, "%lf ", array[i]);
        }
        fclose(file);
    }
    else
    {
        if ((file = fopen(name, "a")) == NULL)
            return -1;
        for (int i = 0; i < size; i++)
        {
            fprintf(file, "%lf ", array[i]);
        }
        fclose(file);
    }
    return 0;
}

double* TabForArray(double* array,TFunc_t pFunc, double x1, double x2, double step)
{
    int cnt = 0;
    double* temp = (double*)realloc(array,sizeof(double) * (int)ceil(((x2 - x1) / step))+1);

    if (temp == NULL)
        return NULL;

    array = temp;

    for (double i = x1; i <= x2; i += step)
    {
        if (pFunc == funcV && chunkV(i) == 1 && cos(i) == 0)
        {
            array[cnt] = 1. * (rand() % 1000 - 1000) * rand()/RAND_MAX;
        }
        else 
        {
            array[cnt] = pFunc(i);
            cnt++;
        }
    }
    return array;
}