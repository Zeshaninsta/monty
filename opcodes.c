#include "monty.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * f_push - Pushes an element onto the stack.
 * @stack: Pointer to the top of the stack
 * @line_number: Line number in the Monty byte code file
 */
void f_push(stack_t **stack, unsigned int line_number)
{
    if (!bus.arg)
    {
        fprintf(stderr, "L%u: usage: push integer\n", line_number);
        exit(EXIT_FAILURE);
    }

    int value = atoi(bus.arg);
    stack_t *new_node = malloc(sizeof(stack_t));

    if (new_node == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    new_node->n = value;
    new_node->prev = NULL;
    new_node->next = *stack;

    if (*stack != NULL)
    {
        (*stack)->prev = new_node;
    }

    *stack = new_node;
}

/**
 * f_pall - Prints all values on the stack.
 * @stack: Pointer to the top of the stack
 * @line_number: Line number in the Monty byte code file
 */
void f_pall(stack_t **stack, unsigned int line_number)
{
    stack_t *current = *stack;

    while (current != NULL)
    {
        printf("%d\n", current->n);
        current = current->next;
    }

    (void)line_number; // Unused parameter
}

// Implement other opcode functions (pint, pop, swap, add, nop) here...

/**
 * f_pint - Prints the value at the top of the stack.
 * @stack: Pointer to the top of the stack
 * @line_number: Line number in the Monty byte code file
 */
void f_pint(stack_t **stack, unsigned int line_number)
{
    if (*stack == NULL)
    {
        fprintf(stderr, "L%u: can't pint, stack empty\n", line_number);
        exit(EXIT_FAILURE);
    }

    printf("%d\n", (*stack)->n);
}

/**
 * f_pop - Removes the top element from the stack.
 * @stack: Pointer to the top of the stack
 * @line_number: Line number in the Monty byte code file
 */
void f_pop(stack_t **stack, unsigned int line_number)
{
    if (*stack == NULL)
    {
        fprintf(stderr, "L%u: can't pop an empty stack\n", line_number);
        exit(EXIT_FAILURE);
    }

    stack_t *temp = *stack;
    *stack = (*stack)->next;

    if (*stack != NULL)
    {
        (*stack)->prev = NULL;
    }

    free(temp);
}

/**
 * f_swap - Swaps the top two elements of the stack.
 * @stack: Pointer to the top of the stack
 * @line_number: Line number in the Monty byte code file
 */
void f_swap(stack_t **stack, unsigned int line_number)
{
    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%u: can't swap, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }

    int temp = (*stack)->n;
    (*stack)->n = (*stack)->next->n;
    (*stack)->next->n = temp;
}

/**
 * f_add - Adds the top two elements of the stack.
 * @stack: Pointer to the top of the stack
 * @line_number: Line number in the Monty byte code file
 */
void f_add(stack_t **stack, unsigned int line_number)
{
    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%u: can't add, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }

    (*stack)->next->n += (*stack)->n;
    f_pop(stack, line_number);
}

/**
 * f_nop - Doesn't do anything, it's a no-operation.
 * @stack: Pointer to the top of the stack
 * @line_number: Line number in the Monty byte code file
 */
void f_nop(stack_t **stack, unsigned int line_number)
{
    (void)stack; // Unused parameter
    (void)line_number; // Unused parameter
    /* nop doesn't do anything, it's a no-operation */
}

