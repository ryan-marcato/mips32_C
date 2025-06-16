#include <stdio.h>

#include "../include/codificadores.h"
#include "../include/memoria.h"
#include "../include/registradores.h"

void print_bin(int num)
{

    for (int i = 31; i >= 0; i--)
    {
        printf("%d", (num >> i) & 1);
    }

    printf("\n");
}

int codifica_inst(Instrucao inst)
{
    int binario = -1;

    switch (inst.type)
    {

    case R:

        binario = (inst.opcode << 26) | (inst.valores.RType.rs << 21) | (inst.valores.RType.rt << 16) |
                  (inst.valores.RType.rd << 11) | (inst.valores.RType.shamt << 6) | inst.valores.RType.funct;

        break;

    case I:

        binario = (inst.opcode << 26) | (inst.valores.IType.rs << 21) |
                  (inst.valores.IType.rt << 16) | inst.valores.IType.imm;

        break;

    case J:

        binario = (inst.opcode << 26) | (inst.valores.JType.address & 0x03FFFFFF);

        break;

    default:

        printf("Nao foi possível codificar a pseudo-instrucao\n");
        break;
    }

    return binario;
}

void codifica_inst_arr(Instrucao *inst_arr)
{

    int inst_bin;

    for (int i = 0; i < endereco_atual_texto; i++)
    {

        inst_bin = codifica_inst(inst_arr[i]);

        if (inst_bin == -1)
        {
            continue;
        }

        print_bin(inst_bin);
    }
}
