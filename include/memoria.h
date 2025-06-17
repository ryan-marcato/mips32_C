#ifndef MEMORIA_H
#define MEMORIA_H

#include <stdint.h>
#include <stdbool.h>

#include "instrucoes.h"

#define DATA_MEM_SIZE 0x200000 // 2 MB
#define INST_MEM_SIZE 0x100000 // 1 MB

extern unsigned char memoria_dados[];
extern Instrucao mem_intrrucoes[];

extern uint32_t endereco_atual_texto;
extern uint32_t endereco_atual_dados;
extern uint32_t pc;

bool carregar_palavra_memoria(uint32_t endereco, int *valor);

bool armazenar_palavra_memoria(uint32_t endereco, int valor);

bool armazenar_string_memoria(uint32_t endereco, const char *string);

bool armazenar_intrucao_memoria(uint32_t endereco, Instrucao instrucao);

void inicializar_memoria();

#endif