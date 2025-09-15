#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>
#include <errno.h>

int safe_cube(int n, int *out)
{
    long long n2 = (long long)n * n;
    long long n3 = n2 * n;

    if (n3 > INT_MAX)
    {
        return 0; // ошибка
    }

    *out = (int)n3;
    return 1; // успех
}

int parse_int(const char *s, int *out)
{
    char *end = NULL;
    long v = strtol(s, &end, 10);

    if (end == s || *end != '\0' || v < 1 || v > INT_MAX)
    {
        return 0;
    }
    *out = (int)v;
    return 1;
}

void change_seed()
{
    unsigned int seed = (unsigned)time(NULL);
    srand(seed);
}

int *create_vec(int length)
{
    int *a = (int *)malloc(sizeof(int) * length);

    if (!a)
    {
        return NULL;
    }

    return a;
}

void fill_random_vec(int *a, int length)
{
    int min = 0, max = 999;

    for (int i = 0; i < length; ++i)
    {
        int r = 0;
        while (r == 0)
        {
            r = min + rand() % (max - min + 1);
        }
        a[i] = r;
    }
}

int find_mod(int *a, int length, int target)
{
    for (int i = 0; i < length; ++i)
    {
        if (a[i] % target == 0)
        {
            return a[i];
        }
    }
    return -1;
}

void print_vec(char *title, int *a, int length)
{
    printf("%s:\n[", title);

    for (int i = 0; i < length; i++)
    {
        printf("%d%s", a[i], (i + 1 == length) ? "" : ", ");
    }

    printf("]\n");
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage:\n"
                        "  ./main <length> <n>\n\n"
                        "Meaning:\n"
                        "  length  - размер вектора (>=1)\n"
                        "  n       - целое >=1; ищем элементы, кратные n^3\n");
        return 1;
    }

    int length = 0, n = 0;

    if (!parse_int(argv[1], &length))
    {

        fprintf(stderr, "Ошибка: аргумент 'length' вне диапазона [%d;%d].\n",
                1, INT_MAX);

        return 1;
    }

    if (!parse_int(argv[2], &n))
    {

        fprintf(stderr, "Ошибка: аргумент 'n' вне диапазона [%d;%d].\n",
                1, INT_MAX);

        return 1;
    }

    int n3 = 0;

    if (!safe_cube(n, &n3))
    {
        fprintf(stderr, "Слишком большое число\n");
        return 1;
    }

    change_seed();

    int *a = create_vec(length);
    if (!a)
    {
        fprintf(stderr, "Ошибка: Недостаточно памяти\n");
        return 1;
    }

    fill_random_vec(a, length);

    int finded = find_mod(a, length, n3);

    printf("length = %d, n^3 = %d\n\n", length, n3);

    print_vec("A (исходный вектор)", a, length);

    if (finded == -1)
    {
        printf("Не найдено кратное число\n");
    }
    else
    {
        printf("Первое кратное число: %d\n", finded);
    }

    free(a);

    return 0;
}
