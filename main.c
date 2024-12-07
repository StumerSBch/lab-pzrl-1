#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void print_error(const char *message) 
{
    printf("Ошибка: %s\n", message);
    exit(EXIT_FAILURE);
}

int is_operator(char op) 
{
    return op == '+' || op == '-' || op == '*' || op == '%';
}

int calculate(int left, char operator, int right) 
{
    switch (operator) 
    {
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '%': 
            if (right == 0)
            {
                print_error("Деление на ноль в операции %");
            }
            return left % right;
        default:
            print_error("Некорректный оператор");
            return 0; // Не достигнет сюда, но добавлено для завершённости
    }
}

// Проверка: является ли строка числом
int is_number(const char *str) 
{
    if (*str == '-' || *str == '+') str++; // Пропускаем знак
    while (*str) 
    {
        if (!isdigit(*str))
        {
            return 0;
        } 
        str++;
    }
    return 1;
}

int main(int argc, char *argv[]) 
{
    if (argc < 5) 
    {
        print_error("Недостаточно аргументов. Требуется минимум 5 аргументов.");
    }

    int *results = NULL;
    int results_count = 0;
    int key = 0;
    char *result_string = NULL;
    int key_found = 0;

    for (int i = 1; i < argc; i++) 
    {
        if (strcmp(argv[i], "-k") == 0) 
        {
            // Проверка, что ключ задан
            if (i + 1 >= argc) 
            {
                print_error("Флаг -k указан без ключа дешифрования.");
            }
            // Проверка, что ключ - число
            if (!is_number(argv[i + 1])) 
            {
                print_error("Флаг -k указан с некорректным значением ключа.");
            }
            key = atoi(argv[i + 1]);
            key_found = 1;
            i++; // Пропускаем ключ
        } 
        else if (is_number(argv[i])) 
        {
            // Проверяем корректность выражения из 3 аргументов
            if (i + 2 >= argc) 
            {
                print_error("Ожидалось выражение из 3-х аргументов (операнд, оператор, операнд).");
            }
            int left_operand = atoi(argv[i]);
            char operator = argv[i + 1][0];
            int right_operand = atoi(argv[i + 2]);

            if (!is_operator(operator)) 
            {
                print_error("Некорректный оператор.");
            }

            int result = calculate(left_operand, operator, right_operand);

            // Динамическое выделение памяти для хранения результатов
            results = realloc(results, (results_count + 1) * sizeof(int));
            if (!results) 
            {
                print_error("Ошибка выделения памяти.");
            }
            results[results_count++] = result;

            i += 2; // Пропускаем операнд, оператор и второй операнд
        } 

        else 
        {
            print_error("Некорректный аргумент.");
        }
    }

    // Проверка, что ключ дешифрования был найден
    if (!key_found) 
    {
        print_error("Флаг -k должен быть указан.");
    }

    if (results_count == 0) 
    {
        print_error("Не удалось вычислить ни одного выражения.");
    }

    // Дешифрование и построение результирующей строки
    result_string = malloc(results_count + 1);
    if (!result_string) 
    {
        print_error("Ошибка выделения памяти для результирующей строки.");
    }

    for (int i = 0; i < results_count; i++) 
    {
        results[i] -= key; // Дешифрование результата
        printf("Ответ №%d: %d\n", i + 1, results[i]);
        // Проверка, что результат входит в диапазон ASCII
        if (results[i] >= 0 && results[i] <= 127) 
        {
            result_string[i] = (char)results[i];
        } 
        else 
        {
            result_string[i] = '?'; // Замена некорректного символа
        }
    }
    result_string[results_count] = '\0';

    // Вывод результирующей строки
    printf("Результирующая строка: %s\n", result_string);

    // Очистка памяти
    free(results);
    free(result_string);

    return 0;
}
