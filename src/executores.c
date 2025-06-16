#include <stdio.h>
#include <stdlib.h>

#include "../include/executores.h"
#include "../include/registradores.h"
#include "../include/memoria.h"
#include "../include/codificadores.h"

void executa_add(Instrucao inst)
{

    r_array[inst.valores.RType.rd].valor = r_array[inst.valores.RType.rs].valor + r_array[inst.valores.RType.rt].valor;
}

void executa_sub(Instrucao inst)
{

    r_array[inst.valores.RType.rd].valor = r_array[inst.valores.RType.rs].valor - r_array[inst.valores.RType.rt].valor;
}

void executa_mult(Instrucao inst)
{

    r_array[inst.valores.RType.rd].valor = r_array[inst.valores.RType.rs].valor * r_array[inst.valores.RType.rt].valor;
}

void executa_and(Instrucao inst)
{

    r_array[inst.valores.RType.rd].valor = r_array[inst.valores.RType.rs].valor & r_array[inst.valores.RType.rt].valor;
}

void executa_or(Instrucao inst)
{

    r_array[inst.valores.RType.rd].valor = r_array[inst.valores.RType.rs].valor | r_array[inst.valores.RType.rt].valor;
}

void executa_sll(Instrucao inst)
{

    r_array[inst.valores.RType.rd].valor = r_array[inst.valores.RType.rt].valor << inst.valores.RType.shamt;
}

void executa_slt(Instrucao inst)
{

    r_array[inst.valores.RType.rd].valor = (r_array[inst.valores.RType.rs].valor < r_array[inst.valores.RType.rt].valor) ? 1 : 0;
}

void executa_addi(Instrucao inst)
{

    r_array[inst.valores.IType.rt].valor = r_array[inst.valores.IType.rs].valor + inst.valores.IType.imm;
}

void executa_slti(Instrucao inst)
{

    r_array[inst.valores.IType.rt].valor = r_array[inst.valores.IType.rs].valor < inst.valores.IType.imm ? 1 : 0;
}

void executa_lui(Instrucao inst)
{

    r_array[inst.valores.IType.rt].valor = (int32_t)inst.valores.IType.imm << 16;
}

void executa_lw(Instrucao inst)
{

    uint32_t endereco = r_array[inst.valores.IType.rs].valor + inst.valores.IType.imm;
    int valor_carregado;
    if (carregar_palavra_memoria(endereco, &valor_carregado))
    {
        r_array[inst.valores.IType.rt].valor = valor_carregado;
    }
    else
    {
        printf("Erro: Nao foi possível carregar a palavra da memoria no endereço %u\n", endereco);
        exit(EXIT_FAILURE);
    }
}

void executa_sw(Instrucao inst)
{

    uint32_t endereco = r_array[inst.valores.IType.rs].valor + inst.valores.IType.imm;
    armazenar_palavra_memoria(endereco, r_array[inst.valores.IType.rt].valor);
}

void executa_pseudo_inst(Instrucao inst)
{

    if (inst.valores.PType.is_label)
    {
        r_array[inst.valores.PType.rt].valor = inst.valores.PType.data.address;
    }
    else
    {
        r_array[inst.valores.PType.rt].valor = inst.valores.PType.data.imm;
    }
}

void executa_syscall(Instrucao inst)
{

    int syscall_code = r_array[2].valor;

    switch (syscall_code)
    {
    case 1:
        printf("%d\n", r_array[4].valor);
        break;
    case 4:
        printf("%s\n", (char *)&memoria_dados[r_array[4].valor]);
        break;
    case 10:
        printf("Saindo do  programa..\n");
        break;
    default:
        printf("Erro: Syscall nao suportada %d\n", syscall_code);
        exit(EXIT_FAILURE);
        break;
    }
}

void executa_j(Instrucao inst)
{

    pc = inst.valores.JType.address;
}

void executa_instrucoes()
{

    while (pc < endereco_atual_texto)
    {

        Instrucao instucao_atual = mem_intrrucoes[pc];

        switch (instucao_atual.type)
        {

        case R:

            tabela_jump[instucao_atual.valores.RType.funct](instucao_atual);
            pc++;
            break;

        case I:

            tabela_jump[instucao_atual.opcode](instucao_atual);
            pc++;
            break;

        case J:

            executa_j(instucao_atual);
            break;

        case P:

            executa_pseudo_inst(instucao_atual);
            pc++;
            break;

        default:

            pc++;
            break;
        }
    }
}