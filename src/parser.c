#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "../include/parser.h"
#include "../include/validador.h"
#include "../include/label.h"
#include "../include/memoria.h"

#define MAX_DIR 128
#define MAX_INPUT_SIZE 256
#define MAX_OPERANDS 4

void limpar_linha(char *linha)
{
    size_t tam = strlen(linha);

    while (tam > 0 && (linha[tam - 1] == '\n' || linha[tam - 1] == '\r'))
    {
        linha[tam - 1] = '\0';
        tam--;
    }

    char *comentario = strchr(linha, '#');
    if (comentario)
        *comentario = '\0';
    while (isspace(*linha))
        linha++;
}

void tokenizar_linha(char *linha, char *modo_atual, bool *eh_primeira_leitura, int *linha_inst)
{

    const char delimitador[] = " \t,";
    char *token = strtok(linha, delimitador);

    if (token == NULL)
        return;

    if (modo_atual[0] == '\0')
    {
        strcpy(modo_atual, ".text");
    }

    char *nome_label = NULL;

    // Detecta label
    size_t tam_token = strlen(token);

    if (token[tam_token - 1] == ':')
    {
        token[tam_token - 1] = '\0'; // Remove ':'
        nome_label = token;

        if (*eh_primeira_leitura)
        {
            if (strcasecmp(modo_atual, ".text") == 0)
            {
                add_label(nome_label, (*linha_inst));
            }
            else if (strcasecmp(modo_atual, ".data") == 0)
            {
                add_label(nome_label, endereco_atual_dados);
            }
        }

        token = strtok(NULL, delimitador);
        if (token == NULL)
            return;
    }

    if (eh_campo_dados(token))
    {
        strcpy(modo_atual, ".data");
        return;
    }
    else if (eh_campo_texto(token))
    {
        strcpy(modo_atual, ".text");
        return;
    }

    if (strcasecmp(modo_atual, ".data") == 0)
    {
        char *args[MAX_DIR];
        int arg_count = 0;

        args[arg_count++] = token;

        while ((token = strtok(NULL, delimitador)) != NULL)
        {
            args[arg_count++] = token;
        }

        if (arg_count > 0 && !(*eh_primeira_leitura))
        {
            validar_campo_dados(nome_label, args, arg_count, label_arr);
        }
    }
    else if (strcasecmp(modo_atual, ".text") == 0)
    {
        char *instrucao = token;
        char *operands[MAX_OPERANDS];
        int operand_count = 0;

        while ((token = strtok(NULL, delimitador)) != NULL)
        {
            operands[operand_count++] = token;
            if (operand_count >= MAX_OPERANDS)
                break;
        }

        if (!(*eh_primeira_leitura))
        {
            validar_inst(instrucao, operands, operand_count);
        }

        (*linha_inst)++;
    }
}

int analisar_arquivo(char *nome_arquivo)
{

    FILE *fp;
    char linha[MAX_INPUT_SIZE];
    char current_mode[16] = "";
    bool primeira_leitura = true;
    int inst_line = 0;

    if ((fp = fopen(nome_arquivo, "r")) == NULL)
    {
        return 1;
    }

    inicializar_memoria();

    // Identificando as labels
    while (fgets(linha, sizeof(linha), fp))
    {
        clean_line(linha);
        tokenizar_linha(linha, current_mode, &primeira_leitura, &inst_line);
    }

    // Resentando
    rewind(fp);
    primeira_leitura = false;
    inst_line = 0;

    while (fgets(linha, sizeof(linha), fp))
    {
        limpar_linha((linha));
        tokenizar_linha(linha, current_mode, &primeira_leitura, &inst_line);
    }

    fclose(fp);

    return 0;
}
