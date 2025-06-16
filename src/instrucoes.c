#include <string.h>

#include "../include/instrucoes.h"
#include "../include/executores.h"

InfoInstrucao tabela[] = {
    {"add", R, 0x00, 0x20, 3},
    {"sub", R, 0x00, 0x22, 3},
    {"and", R, 0x00, 0x24, 3},
    {"or", R, 0x00, 0x25, 3},
    {"sll", R, 0x00, 0x00, 3},
    {"slt", R, 0x00, 0x2A, 3},
    {"mult", R, 0x00, 0x18, 2},

    {"addi", I, 0x08, 0x00, 3},
    {"slti", I, 0x0A, 0x00, 3},
    {"lui", I, 0x0F, 0x00, 2},
    {"lw", I, 0x23, 0x00, 2},
    {"sw", I, 0x2B, 0x00, 2},

    {"j", J, 0x02, 0x00, 1},
    {"li", P, 0x00, 0x00, 2},
    {"la", P, 0x00, 0x00, 2},
    {"syscall", R, 0x00, 0x0C, 0},
    {NULL, UNKNOWN, 0x00, 0x00, 0}};

InstrucaoHandler tabela_jump[64] = {

    [0x20] = executa_add,  // R-type ADD (funct 0x20)
    [0x22] = executa_sub,  // R-type SUB (funct 0x22)
    [0x24] = executa_and,  // R-type AND (funct 0x24)
    [0x25] = executa_or,   // R-type OR (funct 0x25)
    [0x00] = executa_sll,  // R-type SLL (funct 0x00)
    [0x2A] = executa_slt,  // R-type SLT (funct 0x2A)
    [0x18] = executa_mult, // R-type MULT (funct 0x18)

    [0x08] = executa_addi, // I-type ADDI (opcode 0x08)
    [0x0A] = executa_slti, // I-type SLTI (opcode 0x0A)
    [0x0F] = executa_lui,  // I-type LUI (opcode 0x0F)
    [0x23] = executa_lw,   // I-type LW (opcode 0x23)
    [0x2B] = executa_sw,   // I-type SW (opcode 0x2B)

    [0x02] = executa_j, // J-type J (opcode 0x02)
    [0x0C] = executa_syscall,

};

InfoInstrucao *busca_instrucao(const char *nome)
{

    for (int i = 0; tabela[i].nome != NULL; i++)
    {
        if (strcmp(nome, tabela[i].nome) == 0)
        {
            return &tabela[i];
        }
    }

    return NULL;
}