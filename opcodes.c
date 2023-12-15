#include "monty.h"

bus_t bus;

/**
 * _realloc - reallocates a memory block using malloc and free
 * @ptr: pointer to the memory previously allocated with malloc
 * @old_size: size in bytes of the allocated space for ptr
 * @new_size: new size in bytes to be allocated
 * Return: pointer to the reallocated memory block
 */
char *_realloc(char *ptr, unsigned int old_size, unsigned int new_size)
{
    char *new_ptr;

    if (new_size == old_size)
        return (ptr);

    if (ptr == NULL)
    {
        new_ptr = malloc(new_size);
        if (new_ptr == NULL)
        {
            fprintf(stderr, "Error: malloc failed\n");
            exit(EXIT_FAILURE);
        }
        return (new_ptr);
    }

    if (new_size == 0 && ptr != NULL)
    {
        free(ptr);
        return (NULL);
    }

    new_ptr = malloc(new_size);
    if (new_ptr == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    memcpy(new_ptr, ptr, old_size);
    free(ptr);

    return (new_ptr);
}

/**
 * getstdin - reads lines from standard input
 * @lineptr: pointer to the buffer containing the read line
 * @file: file descriptor to read from
 * Return: number of characters read, -1 on failure
 */
size_t getstdin(char **lineptr, int file)
{
    ssize_t nread;
    size_t len;
    char *buffer = *lineptr;

    if (buffer == NULL)
    {
        buffer = malloc(sizeof(char) * 1024);
        if (buffer == NULL)
        {
            fprintf(stderr, "Error: malloc failed\n");
            exit(EXIT_FAILURE);
        }
        *lineptr = buffer;
    }

    nread = read(file, buffer, 1024);
    if (nread == -1)
    {
        fprintf(stderr, "Error: read failed\n");
        exit(EXIT_FAILURE);
    }

    len = strlen(buffer);
    if (buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';

    return (nread);
}

/**
 * clean_line - cleans up a line by removing leading/trailing whitespace
 * @content: pointer to the line content
 * Return: pointer to the cleaned line
 */
char *clean_line(char *content)
{
    char *cleaned = content;

    while (*cleaned == ' ')
        cleaned++;

    if (*cleaned == '\0')
        return (cleaned);

    char *end = cleaned + strlen(cleaned) - 1;
    while (*end == ' ' && end > cleaned)
        *(end--) = '\0';

    return (cleaned);
}

/**
 * f_push - pushes an element to the stack
 * @head: double pointer to the head of the stack
 * @number: value to be pushed
 */
void f_push(stack_t **head, unsigned int number)
{
    addnode(head, number);
}

/**
 * f_pall - prints all the values on the stack
 * @head: double pointer to the head of the stack
 * @number: line number (unused)
 */
void f_pall(stack_t **head, unsigned int number)
{
    stack_t *temp = *head;

    while (temp != NULL)
    {
        printf("%d\n", temp->n);
        temp = temp->next;
    }
}

/**
 * f_pint - prints the value at the top of the stack
 * @head: double pointer to the head of the stack
 * @number: line number (unused)
 */
void f_pint(stack_t **head, unsigned int number)
{
    if (*head == NULL)
    {
        fprintf(stderr, "L%d: can't pint, stack empty\n", number);
        fclose(bus.file);
        free(bus.content);
        free_stack(*head);
        exit(EXIT_FAILURE);
    }

    printf("%d\n", (*head)->n);
}

/**
 * execute - executes the instruction given by the opcode
 * @content: pointer to the instruction content
 * @head: double pointer to the head of the stack
 * @counter: line number
 * @file: pointer to the monty file
 * Return: 0 on success, -1 on failure
 */
int execute(char *content, stack_t **head, unsigned int counter, FILE *file)
{
    char *opcode, *arg;
    int i;
    instruction_t instructions[] = {
        {"push", f_push},
        {"pall", f_pall},
        {"pint", f_pint},
        /* Add more instructions here */
        {NULL, NULL}
    };

    opcode = strtok(content, " \t");
    if (opcode == NULL || opcode[0] == '#')
        return (0);

    arg = strtok(NULL, " \t");
    bus.arg = arg;
    bus.file = file;
    bus.content = content;

    for (i = 0; instructions[i].opcode != NULL; i++)
    {
        if (strcmp(opcode, instructions[i].opcode) == 0)
        {
            instructions[i].f(head, counter);
            return (0);
        }
    }

    fprintf(stderr, "L%d: unknown instruction %s\n", counter, opcode);
    fclose(bus.file);
    free(bus.content);
    free_stack(*head);
    exit(EXIT_FAILURE);
}

/**
 * free_stack - frees a stack
 * @head: pointer to the head of the stack
 */
void free_stack(stack_t *head)
{
    stack_t *temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/**
 * f_pop - removes the top element of the stack
 * @head: double pointer to the head of the stack
 * @counter: line number
 */
void f_pop(stack_t **head, unsigned int counter)
{
    stack_t *temp;

    if (*head == NULL)
    {
        fprintf(stderr, "L%d: can't pop an empty stack\n", counter);
        fclose(bus.file);
        free(bus.content);
        free_stack(*head);
        exit(EXIT_FAILURE);
    }

    temp = *head;
    *head = (*head)->next;
    if (*head != NULL)
        (*head)->prev = NULL;
    free(temp);
}

/**
 * f_swap - swaps the top two elements of the stack
 * @head: double pointer to the head of the stack
 * @counter: line number
 */
void f_swap(stack_t **head, unsigned int counter)
{
    int temp;

    if (*head == NULL || (*head)->next == NULL)
    {
        fprintf(stderr, "L%d: can't swap, stack too short\n", counter);
        fclose(bus.file);
        free(bus.content);
        free_stack(*head);
        exit(EXIT_FAILURE);
    }

    temp = (*head)->n;
    (*head)->n = (*head)->next->n;
    (*head)->next->n = temp;
}

/**
 * f_add - adds the top two elements of the stack
 * @head: double pointer to the head of the stack
 * @counter: line number
 */
void f_add(stack_t **head, unsigned int counter)
{
    if (*head == NULL || (*head)->next == NULL)
    {
        fprintf(stderr, "L%d: can't add, stack too short\n", counter);
        fclose(bus.file);
        free(bus.content);
        free_stack(*head);
        exit(EXIT_FAILURE);
    }

    (*head)->next->n += (*head)->n;
    f_pop(head, counter);
}

/**
 * f_nop - does nothing
 * @head: double pointer to the head of the stack
 * @counter: line number
 */
void f_nop(stack_t **head, unsigned int counter)
{
    (void)head;
    (void)counter;
}

/**
 * f_sub - subtracts the top element from the second top element of the stack
 * @head: double pointer to the head of the stack
 * @counter: line number
 */
void f_sub(stack_t **head, unsigned int counter)
{
    if (*head == NULL || (*head)->next == NULL)
    {
        fprintf(stderr, "L%d: can't sub, stack too short\n", counter);
        fclose(bus.file);
        free(bus.content);
        free_stack(*head);
        exit(EXIT_FAILURE);
    }

    (*head)->next->n -= (*head)->n;
    f_pop(head, counter);
}

/**
 * f_div - divides the second top element by the top element of the stack
 * @head: double pointer to the head of the stack
 * @counter: line number
 */
void f_div(stack_t **head, unsigned int counter)
{
    if (*head == NULL || (*head)->next == NULL)
    {
        fprintf(stderr, "L%d: can't div, stack too short\n", counter);
        fclose(bus.file);
        free(bus.content);
        free_stack(*head);
        exit(EXIT_FAILURE);
    }

    if ((*head)->n == 0)
    {
        fprintf(stderr, "L%d: division by zero\n", counter);
        fclose(bus.file);
        free(bus.content);
        free_stack(*head);
        exit(EXIT_FAILURE);
    }

    (*head)->next->n /= (*head)->n;
    f_pop(head, counter);
}

/**
 * f_mul - multiplies the second top element with the top element of the stack
 * @head: double pointer to the head of the stack
 * @counter: line number
 */
void f_mul(stack_t **head, unsigned int counter)
{
    if (*head == NULL || (*head)->next == NULL)
    {
        fprintf(stderr, "L%d: can't mul, stack too short\n", counter);
        fclose(bus.file);
        free(bus.content);
        free_stack(*head);
        exit(EXIT_FAILURE);
    }

    (*head)->next->n *= (*head)->n;
    f_pop(head, counter);
}

/**
 * f_mod - computes the modulus of the second top element with the top element
 * @head: double pointer to the head of the stack
 * @counter: line number
 */
void f_mod(stack_t **head, unsigned int counter)
{
    if (*head == NULL || (*head)->next == NULL)
    {
        fprintf(stderr, "L%d: can't mod, stack too short\n", counter);
        fclose(bus.file);
        free(bus.content);
        free_stack(*head);
        exit(EXIT_FAILURE);
    }

    if ((*head)->n == 0)
    {
        fprintf(stderr, "L%d: division by zero\n", counter);
        fclose(bus.file);
        free(bus.content);
        free_stack(*head);
        exit(EXIT_FAILURE);
    }

    (*head)->next->n %= (*head)->n;
    f_pop(head, counter);
}

/**
 * f_pchar - prints the character at the top of the stack
 * @head: double pointer to the head of the stack
 * @counter: line number
 */
void f_pchar(stack_t **head, unsigned int counter)
{
    if (*head == NULL)
    {
        fprintf(stderr, "L%d: can't pchar, stack empty\n", counter);
        fclose(bus.file);
        free(bus.content);
        free_stack(*head);
        exit(EXIT_FAILURE);
    }

    if ((*head)->n < 0 || (*head)->n > 127)
    {
        fprintf(stderr, "L%d: can't pchar, value out of range\n", counter);
        fclose(bus.file);
        free(bus.content);
        free_stack(*head);
        exit(EXIT_FAILURE);
    }

    printf("%c\n", (*head)->n);
}

/**
 * f_pstr - prints the string starting at the top of the stack
 * @head: double pointer to the head of the stack
 * @counter: line number
 */
void f_pstr(stack_t **head, unsigned int counter)
{
    stack_t *temp = *head;

    (void)counter;

    while (temp != NULL && temp->n != 0 && (temp->n > 0 && temp->n <= 127))
    {
        printf("%c", temp->n);
        temp = temp->next;
    }

    printf("\n");
}

