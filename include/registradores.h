#ifndef REGISTRADORES_H
#define REGISTRADORES_H

#include <stdint.h>

#define NUM_REG 32

extern uint32_t pc;

typedef struct {
    char nome[5];
    int32_t numero;
    int32_t valor;
} Registrador;

extern Registrador r_array[NUM_REG];

void ini_registradores();
void print_tabela_reg();
int indice_registradores(const char *reg);

#endif