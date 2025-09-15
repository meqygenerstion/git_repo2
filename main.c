#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>
#include <errno.h>

void error(char *text)
{
    fprintf(stderr, "%s", text);
    exit(1);
}

int safe_cube(int n)
{
    int n2 = n * n;
    int n3 = n2 * n;

    if (n3 > INT_MAX)
    {
        error("Слишком большое число");
    }

    return (int)n3;
}

int parse_int(const char *s, const char *name, int *out)
{
    char *end = NULL;
    long v = strtol(s, &end, 10);

    if (end == s || *end != '\0' || v < 1 || v > INT_MAX)
    {
        char *buffer[100];
        sprintf("Ошибка: аргумент '%s' вне диапазона [%d;%d] или некорректен: \"%s\".\n", name, 1, INT_MAX, s);

        error((char *)buffer);
    }
    *out = (int)v;
    return 1;
}

int read_int(char **argv, int index, const char *name)
{
    int target = 0;
    if (!parse_int(argv[index], name, &target))
    {
        error("Не получилось считать число");
    }

    return target;
}

void check_args(int argc, int needed, char *text)
{
    if (argc < needed)
    {
        error(text);
    }
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
        free(a);
        error("Ошибка: Недостаточно памяти");
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
    check_args(argc, 3, "Usage:\n"
                        "  ./main <length> <n>\n\n"
                        "Meaning:\n"
                        "  length  - размер вектора (>=1)\n"
                        "  n       - целое >=1; ищем элементы, кратные n^3\n");

    int length = read_int(argv, 1, "length");
    int n = read_int(argv, 2, "n");

    change_seed();

    int n3 = safe_cube(n);

    int *a = create_vec(length);
    int *b = create_vec(length);

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
    free(b);

    return 0;
}
