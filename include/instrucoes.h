#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdbool.h>
#include <stdint.h>

typedef enum TipoInstrucao
{
    R,
    I,
    J,
    P,
    UNKNOWN
} TipoInstrucao;

typedef struct Instrucao
{

    uint8_t opcode;

    TipoInstrucao type;

    union
    {
        struct
        {
            uint8_t rs;
            uint8_t rt;
            uint8_t rd;
            uint8_t shamt;
            uint8_t funct;
        } RType;

        struct
        {
            uint8_t rs;
            uint8_t rt;
            uint16_t imm;
        } IType;

        struct
        {
            uint32_t address;
        } JType;

        struct
        {

            uint8_t rt;
            bool is_label;

            union
            {
                uint32_t address;
                uint32_t imm;
            } data;

        } PType;

    } valores;

} Instrucao;

typedef struct InfoInstrucao
{

    char *nome;
    TipoInstrucao type;
    uint8_t opcode;
    uint8_t funct;
    uint8_t op_count;

} InfoInstrucao;

typedef void (*InstructionHandler)(Instrucao);
extern InstructionHandler tabela_jump[];

InfoInstrucao *busca_instrucao(const char *nome);

#endif