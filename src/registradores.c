#include <string.h>
#include <stdio.h>

#include "../include/registradores.h"

Registrador r_array[NUM_REG];
uint32_t pc = 0;

const char *registradores_validos[] = {
    "$zero", "$at", "$v0", "$v1",
    "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3",
    "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3",
    "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9", "$k0", "$k1",
    "$gp", "$sp", "$fp", "$ra",
    NULL // Sentinel
};

void ini_registradores()
{
    for (int i = 0; registradores_validos[i] != NULL; i++)
    {
        strncpy(r_array[i].nome, registradores_validos[i], 5);
        r_array[i].numero = i;
        r_array[i].valor = 0;
    }
}

void print_tabela_reg()
{
    printf("Registrador ||| Valor\n");

    for (int i = 0; i < NUM_REG; i++)
    {
        printf("| %-9s | %-10d |\n", r_array[i].nome, r_array[i].valor);
    }
}

int indice_registradores(const char *reg)
{
    for (int i = 0; registradores_validos[i] != NULL; i++)
    {
        if (strcmp(reg, registradores_validos[i]) == 0)
        {
            return i;
        }
    }

    return -1;
}