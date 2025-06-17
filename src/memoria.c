#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/memoria.h"

// Inicializa vetor da memoria
unsigned char memoria_dados[DATA_MEM_SIZE];
Instrucao mem_intrrucoes[INST_MEM_SIZE];

uint32_t endereco_atual_texto = 0;
uint32_t endereco_atual_dados = 0;

void inicializar_memoria()
{
    memset(memoria_dados, 0, sizeof(memoria_dados));
    memset(mem_intrrucoes, 0, sizeof(mem_intrrucoes));
}

bool carregar_palavra_memoria(uint32_t endereco, int *valor)
{

    if (endereco >= DATA_MEM_SIZE - 3)
    {
        fprintf(stderr, "Violação de acesso a memória no endereço: 0x%X\n", endereco);
        return false;
    }

    if (endereco % 4 != 0)
    {
        fprintf(stderr, "Acesso a memória desalinhado no endereço: 0x%X\n", endereco);
        return false;
    }

    *valor = (memoria_dados[endereco] << 24) |
             (memoria_dados[endereco + 1] << 16) |
             (memoria_dados[endereco + 2] << 8) |
             (memoria_dados[endereco + 3]);

    return true;
}

bool armazenar_palavra_memoria(uint32_t endereco, int valor)
{

    if (endereco >= DATA_MEM_SIZE - 3)
    {
        fprintf(stderr, "Violação de acesso a memória no endereço: 0x%X\n", endereco);
        return false;
    }
    if (endereco % 4 != 0)
    {
        fprintf(stderr, "Acesso a memória desalinhado no endereço: 0x%X\n", endereco);
        return false;
    }

    memoria_dados[endereco] = (valor >> 24) & 0xFF;
    memoria_dados[endereco + 1] = (valor >> 16) & 0xFF;
    memoria_dados[endereco + 2] = (valor >> 8) & 0xFF;
    memoria_dados[endereco + 3] = valor & 0xFF;

    return true;
}

bool armazenar_string_memoria(uint32_t endereco, const char *string)
{

    size_t len = strlen(string) + 1;

    if (endereco + len > DATA_MEM_SIZE)
    {
        fprintf(stderr, "Violação de acesso a memória: A string ultrapassa os limites da memória\n");
        return false;
    }

    memcpy(&memoria_dados[endereco], string, len); // Copia string para a memoria

    return true;
}

bool armazenar_intrucao_memoria(uint32_t endereco, Instrucao instrucao)
{

    if (endereco > INST_MEM_SIZE)
    {
        fprintf(stderr, "Memória de instrução insuficiente\n");
        return false;
    }

    mem_intrrucoes[endereco] = instrucao;

    return true;
}