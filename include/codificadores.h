#ifndef CODIFICADORES_H
#define CODIFICADORES_H

#include "instrucoes.h"

int codifica_inst(Instrucao inst);

void codifica_inst_arr(Instrucao *inst_arr);

void print_bin(int num);

#endif