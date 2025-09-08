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

static int safe_cube(int n)
{
    // Считаем n^3 в расширенном типе и проверяем, что помещается в int
    int64_t n2 = (int64_t)n * n;
    int64_t n3 = n2 * n;
    if (n3 > INT_MAX)
    {
        error("Слишком большое число");
    }
    return (int)n3;
}

static int parse_int(const char *s, int min, int max, const char *name, int *out)
{
    char *end = NULL;
    errno = 0;
    long v = strtol(s, &end, 10);
    if (errno || end == s || *end != '\0' || v < min || v > max)
    {
        error(sprintf("Ошибка: аргумент '%s' вне диапазона [%d;%d] или некорректен: \"%s\".\n", name, min, max, s));
    }
    *out = (int)v;
    return 1;
}

static int read_int(char **argv, int index, const char *name)
{
    int target = 0;
    if (!parse_int(argv[index], 1, INT_MAX, name, &target))
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

int find_mod(int *a, int *b, int length, int target)
{
    int min = 0, max = 999;

    size_t k = 0;
    for (int i = 0; i < length; ++i)
    {
        int r = min + rand() % (max - min + 1);
        a[i] = r;
        if (r % target == 0)
        {
            b[k++] = r;
        }
    }

    return k;
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
                        "  ./main <length> <n> [seed]\n\n"
                        "Meaning:\n"
                        "  length  - размер вектора (>=1)\n"
                        "  n       - целое >=1; ищем элементы, кратные n^3\n"
                        "  seed    - необязательный seed для PRNG\n");

    int length = read_int(argv, 1, "length");
    int n = read_int(argv, 2, "n");

    change_seed();

    int n3 = safe_cube(n);

    int *a = (int *)malloc(sizeof(int) * (size_t)length);
    int *b = (int *)malloc(sizeof(int) * (size_t)length);

    if (!a || !b)
    {
        free(a);
        free(b);

        error("Ошибка: недостаточно памяти.\n");
    }

    int b_length = find_mod(a, b, length, n3);

    print_vec("A (исходный)", a, length);
    print_vec("B (кратные n^3)", b, b_length);

    free(a);
    free(b);

    return 0;
}
