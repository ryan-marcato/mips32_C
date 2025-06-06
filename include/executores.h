#ifndef EXECUTORES_H
#define EXECUTORES_H

#include "instrucoes.h"

void executa_add(Instrucao inst);
void executa_sub(Instrucao inst);
void executa_mult(Instrucao inst);
void executa_and(Instrucao inst);
void executa_or(Instrucao inst);
void executa_sll(Instrucao inst);
void executa_lui(Instrucao inst);
void executa_addi(Instrucao inst);
void executa_slt(Instrucao inst);
void executa_slti(Instrucao inst);
void executa_la(Instrucao inst);
void executa_lw(Instrucao inst);
void executa_sw(Instrucao inst);
void executa_syscall(Instrucao inst);
void executa_li(Instrucao inst);
void executa_pseudo_inst(Instrucao inst);
void executa_j(Instrucao inst);
void executa_instrucoes();

#endif