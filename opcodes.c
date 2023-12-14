#include "monty.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * push - Pushes an element onto the stack.
 * @stack: Pointer to the top of the stack
 * @line_number: Line number in the Monty byte code file
 */
void push(stack_t **stack, unsigned int line_number)
{
    /* Check if there is an argument */
    if (!stack || !line_number)
    {
        fprintf(stderr, "L%u: usage: push integer\n", line_number);
        exit(EXIT_FAILURE);
    }

    /* Use atoi to convert the argument to an integer */
    int value = atoi(arg);  // You need to define 'arg' based on how you're reading the file

    /* Create a new node for the stack */
    stack_t *new_node = malloc(sizeof(stack_t));
    if (new_node == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    /* Initialize the new node */
    new_node->n = value;
    new_node->prev = NULL;
    new_node->next = *stack;

    /* Update the top of the stack */
    if (*stack != NULL)
        (*stack)->prev = new_node;
    *stack = new_node;
}

/**
 * pall - Prints all values on the stack.
 * @stack: Pointer to the top of the stack
 * @line_number: Line number in the Monty byte code file
 */
void pall(stack_t **stack, unsigned int line_number)
{
    /* Print all values on the stack */
    stack_t *current = *stack;
    while (current != NULL)
    {
        printf("%d\n", current->n);
        current = current->next;
    }
}

/**
 * pint - Prints the value at the top of the stack.
 * @stack: Pointer to the top of the stack
 * @line_number: Line number in the Monty byte code file
 */
void pint(stack_t **stack, unsigned int line_number)
{
    /* Check if the stack is empty */
    if (*stack == NULL)
    {
        fprintf(stderr, "L%u: can't pint, stack empty\n", line_number);
        exit(EXIT_FAILURE);
    }

    /* Print the value at the top of the stack */
    printf("%d\n", (*stack)->n);
}

/**
 * pop - Removes the top element from the stack.
 * @stack: Pointer to the top of the stack
 * @line_number: Line number in the Monty byte code file
 */
void pop(stack_t **stack, unsigned int line_number)
{
    /* Check if the stack is empty */
    if (*stack == NULL)
    {
        fprintf(stderr, "L%u: can't pop an empty stack\n", line_number);
        exit(EXIT_FAILURE);
    }

    /* Remove the top element from the stack */
    stack_t *temp = *stack;
    *stack = (*stack)->next;
    if (*stack != NULL)
        (*stack)->prev = NULL;
    free(temp);
}

/**
 * swap - Swaps the top two elements of the stack.
 * @stack: Pointer to the top of the stack
 * @line_number: Line number in the Monty byte code file
 */
void swap(stack_t **stack, unsigned int line_number)
{
    /* Check if the stack has at least two elements */
    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%u: can't swap, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }

    /* Swap the values of the top two elements */
    int temp = (*stack)->n;
    (*stack)->n = (*stack)->next->n;
    (*stack)->next->n = temp;
}

/**
 * add - Adds the top two elements of the stack.
 * @stack: Pointer to the top of the stack
 * @line_number: Line number in the Monty byte code file
 */
void add(stack_t **stack, unsigned int line_number)
{
    /* Check if the stack has at least two elements */
    if (*stack == NULL || (*stack)->next == NULL)
    {
        fprintf(stderr, "L%u: can't add, stack too short\n", line_number);
        exit(EXIT_FAILURE);
    }

    /* Add the top two elements and store the result in the second top element */
    (*stack)->next->n += (*stack)->n;

    /* Pop the top element from the stack */
    pop(stack, line_number);
}

/**
 * nop - Doesn't do anything, it's a no-operation.
 * @stack: Pointer to the top of the stack
 * @line_number: Line number in the Monty byte code file
 */
void nop(stack_t **stack, unsigned int line_number)
{
    /* nop doesn't do anything, it's a no-operation */
    (void)stack;
    (void)line_number;
}
