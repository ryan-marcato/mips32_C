#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>

void limpar_linha(char *linha);
int analisar_arquivo(char *nome_arquivo);
void tokenizar_linha(char *linha, char *modo_atual, bool *eh_primeira_leitura, int *linha_inst);

#endif
