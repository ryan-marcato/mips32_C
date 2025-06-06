#ifndef LABEL_H
#define LABEL_H

#include <stdint.h>

#define MAX_LABELS 500
#define MAX_LABEL_TAM 100

typedef struct {
    char nome[MAX_LABEL_TAM];
    uint32_t endereco; 
} Label;

extern Label label_arr[];
extern uint32_t label_atual;

void add_label(const char *nome, uint32_t endereco);
void print_label_lista();
uint32_t encontra_end_label(const char *nome);

#endif