#ifndef VALIDADOR_H
#define VALIDADOR_H

#include "instrucoes.h"
#include "label.h"

bool validar_operand(const InfoInstrucao *inst, char **operands, int cont_op);
void validar_inst(const char *instrucao, char **operands, int cont_op);
void validar_campo_dados(const char *nome_label, char **args, int cont_agr, Label *label_arr);

bool extrair_op(const InfoInstrucao *inst, char **operands, int cont_op);
int extrair_registrador(const char *operand);
int extrair_endereco(const char *operand);

bool eh_label(const char *token);
bool eh_op(const char *token);
bool eh_imdt(const char *token);
bool eh_endereco(const char *token);
bool eh_campo_dados(const char *token);
bool eh_campo_texto(const char *token);
bool eh_drtv(const char *token);
bool eh_registrador(const char *token);

#endif