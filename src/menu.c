#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/executores.h"
#include "../include/registradores.h"
#include "../include/codificadores.h"
#include "../include/memoria.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

void limpa_tela()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void espera_enter()
{
    printf("\n%sPressione Enter para continuar...%s", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
    getchar();
}

void limpa_instucoes()
{
    memset(mem_intrrucoes, 0, sizeof(Instrucao) * INST_MEM_SIZE);
    endereco_atual_texto = 0;
    pc = 0;
}

int menu()
{

    int op = 0;
    int arq_carregado = 0;
    int eh_primeira_exe = 1;
    char filepath[256] = "";
    int resultado;

    do
    {

        limpa_tela();
        printf("%s========================================\n", ANSI_COLOR_CYAN);
        printf("              MENU PRINCIPAL            \n");
        printf("========================================%s\n", ANSI_COLOR_RESET);
        printf("%s1.%s Carregar arquivo\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
        printf("%s2.%s Executar instruções\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
        printf("%s3.%s Imprimir tabela de registradores\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
        printf("%s4.%s Imprimir binários\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
        printf("%s5.%s Sair\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
        printf("%s========================================\n", ANSI_COLOR_CYAN);

        printf("Escolha uma opção: %s", ANSI_COLOR_MAGENTA);
        scanf("%d", &op);
        printf(ANSI_COLOR_RESET);

        while (getchar() != '\n')
            ;

        switch (op)
        {

        case 1:
        {

            limpa_tela();
            printf("%sDigite o diretório do arquivo: %s", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
            fgets(filepath, sizeof(filepath), stdin);
            size_t len = strlen(filepath);

            if (len > 0 && filepath[len - 1] == '\n')
            {
                filepath[len - 1] = '\0';
            }

            limpa_instucoes();
            ini_registradores();

            printf("%sCarregando arquivo: %s%s\n", ANSI_COLOR_YELLOW, filepath, ANSI_COLOR_RESET);
            resultado = analisar_arquivo(filepath);

            if (resultado == 0)
            {
                arq_carregado = 1;
                eh_primeira_exe = 1;
                printf("%sArquivo carregado \xE2\x9C\x94%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
            }
            else
            {
                arq_carregado = 0;
                printf("%sErro ao carregar o arquivo.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
            }

            espera_enter();

            break;
        }
        case 2:
        {

            if (!arq_carregado)
            {
                printf("%sErro: Carregue um arquivo primeiro.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
                espera_enter();
            }
            else
            {

                limpa_tela();

                if (eh_primeira_exe)
                {
                    printf("%sExecutando instruções...%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
                    executa_instrucoes();
                    printf("%sInstruções executadas.%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                    eh_primeira_exe = 0;
                }
                else
                {
                    limpa_instucoes();
                    ini_registradores();
                    printf("%sRecarregando arquivo: %s%s\n", ANSI_COLOR_YELLOW, filepath, ANSI_COLOR_RESET);
                    resultado = analisar_arquivo(filepath);
                    if (resultado == 0)
                    {
                        printf("%sArquivo recarregado \xE2\x9C\x94%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                        printf("%sExecutando instruções...%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
                        execute_instructions();
                        printf("%sInstruções executadas.%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                    }
                    else
                    {
                        printf("%sErro ao recarregar o arquivo.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
                    }
                }

                espera_enter();
            }

            break;
        }
        case 3:

            if (!arq_carregado)
            {
                printf("%sErro: Carregue um arquivo primeiro.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
                espera_enter();
            }
            else
            {
                limpa_tela();
                print_reg_table();
                espera_enter();
            }

            break;

        case 4:

            if (!arq_carregado)
            {
                printf("%sErro: Carregue um arquivo primeiro.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
                espera_enter();
            }
            else
            {
                limpa_tela();
                codifica_inst_arr(mem_intrrucoes);
                espera_enter();
            }

            break;

        case 5:

            printf("%sSaindo do programa...%s\n", ANSI_COLOR_MAGENTA, ANSI_COLOR_RESET);
            return EXIT_SUCCESS;

        default:

            printf("%sOpção inválida. Tente novamente.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
            espera_enter();
            break;
        }

    } while (op != 5);
}