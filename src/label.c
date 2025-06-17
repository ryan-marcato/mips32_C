#include <string.h>
#include <stdio.h>

#include "../include/label.h"

Label label_arr[MAX_LABELS];
uint32_t label_atual = 0;

// Add a label na lista
void add_label(const char *nome, uint32_t endereco)
{
    if (label_atual >= MAX_LABELS)
    {
        fprintf(stderr, "Maximo de labels.\n");
        return;
    }

    label_arr[label_atual].endereco = endereco;
    strncpy(label_arr[label_atual].nome, nome, MAX_LABEL_TAM - 1);
    label_arr[label_atual].nome[MAX_LABEL_TAM - 1] = '\0';

    label_atual++;
}

void print_label_lista()
{
    for (int i = 0; i < label_atual; i++)
    {
        printf("%s, 0x%X\n", label_arr[i].nome, label_arr[i].endereco);
    }
}

// Procura a label pelo o nome e retorna o endereco
uint32_t encontra_label(const char *nome)
{
    for (int i = 0; i < label_atual; i++)
    {
        if (strcmp(nome, label_arr[i].nome) == 0)
        {
            return label_arr[i].endereco;
        }
    }

    return -1;
}