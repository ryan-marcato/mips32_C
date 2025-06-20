#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/validador.h"
#include "../include/memoria.h"
#include "../include/registradores.h"
#include "../include/instrucoes.h"
#include "../include/label.h"

extern uint32_t endereco_atual_texto;
extern uint32_t endereco_atual_dados;
extern uint32_t pc;

bool eh_drtv(const char *token)
{
    const char *drtv[] = {".word", ".byte", ".half", ".float", ".double", ".space", ".ascii", ".asciiz", NULL};

    for (int i = 0; drtv[i] != NULL; i++)
    {
        if (strcasecmp(token, drtv[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

bool eh_campo_texto(const char *token)
{
    return strcasecmp(token, ".data") == 0 || strcasecmp(token, ".data:") == 0;
}

bool eh_campo_dados(const char *token)
{
    return strcasecmp(token, ".text") == 0;
}

bool eh_registrador(const char *token)
{
    return (get_register_index(token) != -1);
}

bool eh_label(const char *token)
{
    if (token == NULL || *token == '\0')
    {
        return false;
    }

    if (!isalpha(*token) && *token != '_')
    {
        return false;
    }

    while (*token)
    {
        if (!isalnum(*token) && *token != '_')
        {
            return false;
        }
        token++;
    }
    return true;
}

bool eh_imdt(const char *token)
{
    if (*token == '-' || *token == '+')
    {
        token++;
    }

    if (*token == '\0')
    {
        return false;
    }

    while (*token)
    {
        if (!isdigit(*token))
        {
            return false;
        }
        token++;
    }
    return true;
}

bool eh_endereco(const char *token)
{
    if (token == NULL)
    {
        return false;
    }

    char *abre_parenteses = strchr(token, '(');
    char *fecha_parentes = strchr(token, ')');

    if (abre_parenteses == NULL || fecha_parentes == NULL)
    {
        return false;
    }

    size_t reg_len = fecha_parentes - abre_parenteses - 1;
    if (reg_len <= 0 || reg_len >= 8)
    {
        return false;
    }

    char reg[8];
    strncpy(reg, abre_parenteses + 1, reg_len);
    reg[reg_len] = '\0';

    if (get_register_index(reg) == -1)
    {
        return false;
    }

    size_t offset_len = abre_parenteses - token;
    if (offset_len <= 0 || offset_len >= 16)
    {
        return false;
    }

    char offset_str[16];
    strncpy(offset_str, token, offset_len);
    offset_str[offset_len] = '\0';
    char *endptr;
    strtol(offset_str, &endptr, 10);
    if (*endptr != '\0')
    {
        return false;
    }

    return true;
}

bool eh_op(const char *token)
{
    if (token == NULL)
    {
        return false;
    }

    if (get_register_index(token) == -1)
    {
        return false;
    }

    return eh_imdt(token);
}

void validar_inst(const char *instrucao, char **operands, int cont_op)
{

    InfoInstrucao *inst = busca_instrucao(instrucao);

    if (!inst)
    {
        printf("Erro: Instrução invalida: %s\n", instrucao);
        exit(EXIT_FAILURE);
    }

    if (!validar_operand(inst, operands, cont_op))
    {
        printf("Operação invalida: %s\n", inst->nome);
        exit(EXIT_FAILURE);
    }

    if (extrair_op(inst, operands, cont_op))
    {
        exit(EXIT_FAILURE);
    }
}

bool validar_operand(const InfoInstrucao *inst, char **operands, int cont_op)
{

    if (inst->op_count != cont_op)
    {
        return false;
    }

    switch (inst->type)
    {

    case R:

        // SLL
        if (strcmp(inst->nome, "sll") == 0 && eh_imdt(operands[2]) == 0)
        {

            for (int i = 0; i < cont_op - 1; i++)
            {
                if (!eh_registrador(operands[i]))
                {
                    printf("Error: Operação %d (%s) não é valida %s\n",
                           i + 1, operands[i], inst->nome);
                    return false;
                }
            }
        }
        else if (strcmp(inst->nome, "syscall") == 0)
        {

            if (cont_op != 0)
            {
                return false;
            }
        }
        else if (strcmp(inst->nome, "mult") == 0)
        {

            if (!eh_registrador(operands[0]) || !eh_registrador(operands[1]))
            {
                return false;
            }
        }
        else
        { // Other R-type instructions
            for (int i = 0; i < cont_op; i++)
            {
                if (!eh_registrador(operands[i]))
                {
                    return false;
                }
            }
        }

        break;

    case I:

        if (strcmp(inst->nome, "lw") == 0 || strcmp(inst->nome, "sw") == 0)
        {

            if (!eh_registrador(operands[0]))
            {
                return false;
            }
            if (!eh_endereco(operands[1]))
            {
                return false;
            }
        }
        else
        {
            if (!eh_registrador(operands[0]))
            {
                return false;
            }

            if (!eh_registrador(operands[1]))
            {
                return false;
            }

            if (!eh_imdt(operands[2]))
            {
                return false;
            }
        }

        break;

    case J:

        if (!eh_label(operands[0]) && !eh_imdt(operands[0]))
        {
            return false;
        }

        break;

    case P:

        if (strcmp(inst->nome, "li") == 0)
        {

            if (!eh_registrador(operands[0]))
            {
                return false;
            }

            if (!eh_imdt(operands[1]))
            {
                return false;
            }

            return true;
        }
        else
        {
            if (!eh_registrador(operands[0]))
            {
                return false;
            }

            if (!eh_label(operands[1]))
            {
                return false;
            }
        }

        break;

    default:
        return false;
    }

    return true;
}

bool extrair_op(const InfoInstrucao *inst, char **operands, int cont_op)
{

    if (!validar_operand(inst, operands, cont_op))
    {
        printf("Instrução invalida\n");
        return false;
    }

    Instrucao inst_nova;
    inst_nova.opcode = inst->opcode;
    inst_nova.type = inst->type;

    switch (inst_nova.type)
    {

    case R:
        inst_nova.valores.RType.funct = inst->funct;
        inst_nova.valores.RType.shamt = 0;

        // sll
        if (inst_nova.valores.RType.funct == 0x00)
        {
            inst_nova.valores.RType.rd = indice_registradores(operands[0]);
            inst_nova.valores.RType.rt = indice_registradores(operands[1]);
            inst_nova.valores.RType.shamt = atoi(operands[2]);
        }
        else if (inst_nova.valores.RType.funct == 0x18)
        { // mult
            inst_nova.valores.RType.rs = indice_registradores(operands[0]);
            inst_nova.valores.RType.rt = indice_registradores(operands[1]);
        }
        else if (inst_nova.valores.RType.funct == 0x0C)
        { // syscall
            inst_nova.valores.RType.rd = 0;
            inst_nova.valores.RType.rs = 0;
            inst_nova.valores.RType.rt = 0;
        }
        else
        { // other cases
            inst_nova.valores.RType.rd = indice_registradores(operands[0]);
            inst_nova.valores.RType.rs = indice_registradores(operands[1]);
            inst_nova.valores.RType.rt = indice_registradores(operands[2]);
        }

        break;

    case I:

        if (inst_nova.opcode == 0x23 || inst_nova.opcode == 0x2B)
        {
            inst_nova.valores.IType.rt = indice_registradores(operands[0]);
            inst_nova.valores.IType.imm = extrair_endereco(operands[1]);
            inst_nova.valores.IType.rs = extrair_registrador(operands[1]);
        }
        else
        {
            inst_nova.valores.IType.rs = indice_registradores(operands[0]);
            inst_nova.valores.IType.rt = indice_registradores(operands[1]);
            inst_nova.valores.IType.imm = atoi(operands[2]);
        }

        break;

    case J:

        if (eh_label(operands[0]))
        {
            inst_nova.valores.JType.address = encontra_label(operands[0]);
        }
        else
        {
            inst_nova.valores.JType.address = atoi(operands[0]);
        }

        break;

    case P:

        inst_nova.valores.PType.rt = indice_registradores(operands[0]);

        if (eh_label(operands[1]))
        {
            inst_nova.valores.PType.is_label = true;
            inst_nova.valores.PType.data.address = encontra_label(operands[1]);
        }
        else
        {
            inst_nova.valores.PType.is_label = false;
            inst_nova.valores.PType.data.imm = atoi(operands[1]);
        }

        break;

    default:
        return false;
    }

    armazenar_intrucao_memoria(endereco_atual_texto, inst_nova);
    endereco_atual_texto++;

    return true;
}

int extrair_registrador(const char *operand)
{
    if (operand == NULL)
    {
        return -1;
    }

    char *abre_paren = strchr(operand, '(');
    char *fecha_paren = strchr(operand, ')');

    if (abre_paren == NULL || fecha_paren == NULL)
    {
        return -1;
    }

    size_t tam_reg = fecha_paren - abre_paren - 1;
    if (tam_reg <= 0 || tam_reg >= 8)
    {
        return -1;
    }

    char reg[8];
    strncpy(reg, abre_paren + 1, tam_reg);
    reg[tam_reg] = '\0';

    return indice_registradores(reg);
}

int extrair_endereco(const char *operand)
{
    if (operand == NULL)
    {
        return -1;
    }

    char *abre_paren = strchr(operand, '(');
    char *fecha_paren = strchr(operand, ')');

    if (abre_paren == NULL || fecha_paren == NULL)
    {
        return -1;
    }

    size_t tam_reg = fecha_paren - abre_paren - 1;
    if (tam_reg <= 0 || tam_reg >= 8)
    {
        return -1;
    }

    char reg[8];
    strncpy(reg, abre_paren + 1, tam_reg);
    reg[tam_reg] = '\0';

    size_t tam_offset = abre_paren - operand;
    if (tam_offset <= 0 || tam_offset >= 16)
    {
        return -1;
    }

    char offset_str[16];
    strncpy(offset_str, operand, tam_offset);
    offset_str[tam_offset] = '\0';

    char *endptr;

    return strtol(offset_str, &endptr, 10);
}

void validar_campo_dados(const char *nome_label, char **args, int cont_agr, Label *label_arr)
{

    if (cont_agr < 2)
    {
        printf("Erro: Numeros de argumentos insuficientes.\n");
        exit(EXIT_FAILURE);
    }

    if (!eh_drtv(args[0]))
    {
        exit(EXIT_FAILURE);
    }

    if (strcasecmp(args[0], ".word") == 0)
    {
        if (endereco_atual_dados % 4 != 0)
        {
            endereco_atual_dados += 4 - (endereco_atual_dados % 4);
        }

        for (int i = 1; i < cont_agr; i++)
        {
            char *endptr;
            long value_long = strtol(args[i], &endptr, 10);

            if (*endptr != '\0')
            {
                exit(EXIT_FAILURE);
            }

            uint32_t valor = (uint32_t)value_long;

            if (!armazenar_palavra_memoria(endereco_atual_dados, valor))
            {
                exit(EXIT_FAILURE);
            }

            if (i == 1 && nome_label != NULL)
            {
                add_label(nome_label, endereco_atual_dados);
            }

            endereco_atual_dados += 4;
        }
    }
    else if (strcasecmp(args[0], ".asciiz") == 0)
    {
        if (nome_label == NULL)
        {
            exit(EXIT_FAILURE);
        }

        char string_value[256] = "";
        for (int i = 1; i < cont_agr; i++)
        {
            strcat(string_value, args[i]);
            if (i < cont_agr - 1)
                strcat(string_value, " ");
        }

        // Store the string in memory
        if (!armazenar_string_memoria(endereco_atual_dados, string_value))
        {
            printf("Error: Failed to store string at address 0x%X\n", endereco_atual_dados);
            exit(EXIT_FAILURE);
        }

        add_label(nome_label, endereco_atual_dados);

        endereco_atual_dados += strlen(string_value) + 1; // +1 for null terminator
    }
    else
    {
        exit(EXIT_FAILURE);
    }
}
