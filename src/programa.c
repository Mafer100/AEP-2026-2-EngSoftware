#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACOES 100


typedef struct {
    int codigo;
    char escola[100];
    char tema[100];
    char data_prevista[15]; // Formato DD/MM/AAAA
    char publico_alvo[100];
    char responsavel[100];
    int qtd_prevista;
    int qtd_efetiva;
    int situacao; // 1: Planejada, 2: Realizada, 3: Cancelada
} Acao;

/* Protótipos das Funções */
void cadastrarAcao(Acao lista[], int *total);
void listarAcoes(const Acao lista[], int total);
void pesquisarAcao(const Acao lista[], int total);
void atualizarSituacao(Acao lista[], int total);
void gerarResumoGeral(const Acao lista[], int total);
void exibirMenu(void);
void limparBuffer(void);
void removerQuebraLinha(char *str);

int main(void) {
    Acao listaAcoes[MAX_ACOES];
    int totalAcoes = 0;
    int opcao;

    do {
        exibirMenu();
        printf("Digite a opcao desejada: ");
        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Entrada invalida! Digite apenas numeros.\n");
            limparBuffer();
            opcao = -1;
            continue;
        }
        limparBuffer();
        
        system("cls");

        switch (opcao) {
            case 1:
                cadastrarAcao(listaAcoes, &totalAcoes);
                break;
            case 2:
                listarAcoes(listaAcoes, totalAcoes);
                break;
            case 3:
                pesquisarAcao(listaAcoes, totalAcoes);
                break;
            case 4:
                atualizarSituacao(listaAcoes, totalAcoes);
                break;
            case 5:
                gerarResumoGeral(listaAcoes, totalAcoes);
                break;
            case 0:
                printf("\nEncerrando o Sistema PSE em Acao. Ate logo!\n");
                break;
            default:
                printf("\n[ERRO] Opcao invalida! Escolha um valor entre 0 e 5.\n");
                break;
        }
        printf("\n------------------------------------------------------------\n");
        system("pause");
        system("cls");
    } while (opcao != 0);

    return 0;
}

/* Exibe o menu principal interativo no terminal */
void exibirMenu(void) {
    printf("\n============================================================\n");
    printf("         SISTEMA PSE EM ACAO - GESTAO COLETIVA              \n");
    printf("============================================================\n");
    printf("  1. Cadastrar nova acao\n");
    printf("  2. Listar todas as acoes\n");
    printf("  3. Pesquisar acao por codigo, escola ou tema\n");
    printf("  4. Atualizar situacao de uma acao\n");
    printf("  5. Gerar resumo geral estatistico\n");
    printf("  0. Sair\n");
    printf("============================================================\n");
}

/* 1. Cadastrar Ação Coletiva (RF-01) */
void cadastrarAcao(Acao lista[], int *total) {
    if (*total >= MAX_ACOES) {
        printf("\n[ERRO] Limite maximo de cadastros atingido (%d acoes)!\n", MAX_ACOES);
        return;
    }

    Acao nova;
    printf("\n--- CADASTRO DE NOVA ACAO DO PSE ---\n");

    /* Validação do Código (Unicidade e valor positivo) */
    while (1) {
        printf("Codigo identificador da acao: ");
        if (scanf("%d", &nova.codigo) != 1) {
            printf("[ERRO] O codigo deve ser um numero inteiro valido!\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (nova.codigo <= 0) {
            printf("[ERRO] O codigo deve ser maior que zero!\n");
            continue;
        }

        int duplicado = 0;
        int i;
        for (i = 0; i < *total; i++) {
            if (lista[i].codigo == nova.codigo) {
                duplicado = 1;
                break;
            }
        }

        if (duplicado) {
            printf("[ERRO] Ja existe uma acao cadastrada com o codigo %d! Digite outro codigo.\n", nova.codigo);
        } else {
            break;
        }
    }

    /* Leitura de campos textuais obrigatórios (não vazios) */
    do {
        printf("Nome da Escola: ");
        fgets(nova.escola, sizeof(nova.escola), stdin);
        removerQuebraLinha(nova.escola);
        if (strlen(nova.escola) == 0) {
            printf("[ERRO] O nome da escola e obrigatorio!\n");
        }
    } while (strlen(nova.escola) == 0);

    do {
        printf("Tema da acao (ex: Saude Bucal, Vacinacao, Alimentacao Saudavel): ");
        fgets(nova.tema, sizeof(nova.tema), stdin);
        removerQuebraLinha(nova.tema);
        if (strlen(nova.tema) == 0) {
            printf("[ERRO] O tema da acao e obrigatorio!\n");
        }
    } while (strlen(nova.tema) == 0);

    do {
        printf("Data prevista (DD/MM/AAAA): ");
        fgets(nova.data_prevista, sizeof(nova.data_prevista), stdin);
        removerQuebraLinha(nova.data_prevista);
        if (strlen(nova.data_prevista) == 0) {
            printf("[ERRO] A data prevista e obrigatoria!\n");
        }
    } while (strlen(nova.data_prevista) == 0);

    do {
        printf("Publico-alvo (ex: Ensino Fundamental I, Anos Finais, Docentes): ");
        fgets(nova.publico_alvo, sizeof(nova.publico_alvo), stdin);
        removerQuebraLinha(nova.publico_alvo);
        if (strlen(nova.publico_alvo) == 0) {
            printf("[ERRO] O publico-alvo e obrigatorio!\n");
        }
    } while (strlen(nova.publico_alvo) == 0);

    do {
        printf("Responsavel pela acao (Profissional/Equipe): ");
        fgets(nova.responsavel, sizeof(nova.responsavel), stdin);
        removerQuebraLinha(nova.responsavel);
        if (strlen(nova.responsavel) == 0) {
            printf("[ERRO] O responsavel e obrigatorio!\n");
        }
    } while (strlen(nova.responsavel) == 0);

    /* Leitura e validação da quantidade prevista (>= 0) */
    while (1) {
        printf("Quantidade prevista de participantes: ");
        if (scanf("%d", &nova.qtd_prevista) != 1) {
            printf("[ERRO] Valor invalido! Digite um numero inteiro.\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (nova.qtd_prevista < 0) {
            printf("[ERRO] A quantidade prevista nao pode ser negativa!\n");
        } else {
            break;
        }
    }

    nova.qtd_efetiva = 0;
    nova.situacao = 1; // Inicializada automaticamente como 1: Planejada

    lista[*total] = nova;
    (*total)++;

    printf("\n[SUCESSO] Acao coletiva cadastrada com sucesso (Status: Planejada)!\n");
}

/* 2. Listar Ações Cadastradas (RF-02) */
void listarAcoes(const Acao lista[], int total) {
    if (total == 0) {
        printf("\nNenhuma acao cadastrada no sistema ate o momento.\n");
        return;
    }

    printf("\n========================================================================================================\n");
    printf("                                  LISTAGEM DE ACOES DO PSE (%d registradas)                              \n", total);
    printf("========================================================================================================\n");
    printf("%-6s | %-22s | %-20s | %-10s | %-10s | %-8s | %-8s\n",
           "COD", "ESCOLA", "TEMA", "DATA", "SITUACAO", "PREVISTO", "EFETIVO");
    printf("--------------------------------------------------------------------------------------------------------\n");
        int i;
    for (i = 0; i < total; i++) {
        const char *statusStr;
        switch (lista[i].situacao) {
            case 1:  statusStr = "Planejada"; break;
            case 2:  statusStr = "Realizada"; break;
            case 3:  statusStr = "Cancelada"; break;
            default: statusStr = "Indefinida"; break;
        }

        printf("%-6d | %-22.22s | %-20.20s | %-10.10s | %-10s | %-8d | %-8d\n",
               lista[i].codigo, lista[i].escola, lista[i].tema,
               lista[i].data_prevista, statusStr, lista[i].qtd_prevista, lista[i].qtd_efetiva);
    }
    printf("========================================================================================================\n");
}

/* 3. Pesquisar Ação por Código, Escola ou Tema (RF-03) */
void pesquisarAcao(const Acao lista[], int total) {
    if (total == 0) {
        printf("\nNenhuma acao cadastrada disponivel para pesquisa.\n");
        return;
    }

    int tipoBusca;
    printf("\n--- PESQUISAR ACAO ---\n");
    printf("1. Buscar por Codigo\n");
    printf("2. Buscar por Nome da Escola\n");
    printf("3. Buscar por Tema da Acao\n");
    printf("Escolha o tipo de pesquisa: ");
    if (scanf("%d", &tipoBusca) != 1) {
        printf("[ERRO] Entrada invalida!\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    int encontrados = 0;

    if (tipoBusca == 1) {
        int codBusca;
        printf("Digite o codigo da acao: ");
        if (scanf("%d", &codBusca) != 1) {
            printf("[ERRO] Codigo invalido!\n");
            limparBuffer();
            return;
        }
        limparBuffer();
        int i;
        for (i = 0; i < total; i++) {
            if (lista[i].codigo == codBusca) {
                const char *statusStr = (lista[i].situacao == 1) ? "Planejada" :
                                        (lista[i].situacao == 2) ? "Realizada" : "Cancelada";
                printf("\n>>> REGISTRO LOCALIZADO <<<\n");
                printf("Codigo: %d\n", lista[i].codigo);
                printf("Escola: %s\n", lista[i].escola);
                printf("Tema: %s\n", lista[i].tema);
                printf("Data Prevista: %s\n", lista[i].data_prevista);
                printf("Publico-Alvo: %s\n", lista[i].publico_alvo);
                printf("Responsavel: %s\n", lista[i].responsavel);
                printf("Situacao: %s\n", statusStr);
                printf("Participantes Previstos: %d\n", lista[i].qtd_prevista);
                printf("Participantes Efetivos: %d\n", lista[i].qtd_efetiva);
                encontrados++;
                break;
            }
        }
    } else if (tipoBusca == 2 || tipoBusca == 3) {
        char termo[100];
        printf("Digite o termo de busca: ");
        fgets(termo, sizeof(termo), stdin);
        removerQuebraLinha(termo);
        int i;
        for (i = 0; i < total; i++) {
            const char *alvo = (tipoBusca == 2) ? lista[i].escola : lista[i].tema;
            if (strstr(alvo, termo) != NULL) {
                const char *statusStr = (lista[i].situacao == 1) ? "Planejada" :
                                        (lista[i].situacao == 2) ? "Realizada" : "Cancelada";
                printf("\n------------------------------------------------------------\n");
                printf("Codigo: %d | Escola: %s | Tema: %s\n", lista[i].codigo, lista[i].escola, lista[i].tema);
                printf("Data: %s | Responsavel: %s | Situacao: %s\n", lista[i].data_prevista, lista[i].responsavel, statusStr);
                printf("Previstos: %d | Efetivos: %d\n", lista[i].qtd_prevista, lista[i].qtd_efetiva);
                encontrados++;
            }
        }
    } else {
        printf("[ERRO] Opcao de pesquisa invalida!\n");
        return;
    }

    if (encontrados == 0) {
        printf("\nNenhum registro localizado correspondente aos criterios informados.\n");
    } else {
        printf("\nTotal de registros encontrados: %d\n", encontrados);
    }
}

/* 4. Atualizar Situação e Participantes Efetivos (RF-04) */
void atualizarSituacao(Acao lista[], int total) {
    if (total == 0) {
        printf("\nNenhuma acao cadastrada para atualizacao.\n");
        return;
    }

    int codBusca;
    printf("\n--- ATUALIZAR SITUACAO DA ACAO ---\n");
    printf("Digite o codigo da acao a ser atualizada: ");
    if (scanf("%d", &codBusca) != 1) {
        printf("[ERRO] Codigo invalido!\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    int indice = -1;
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].codigo == codBusca) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("\n[ERRO] Nenhuma acao localizada com o codigo %d!\n", codBusca);
        return;
    }

    const char *statusAtual = (lista[indice].situacao == 1) ? "Planejada" :
                             (lista[indice].situacao == 2) ? "Realizada" : "Cancelada";
    printf("\nAcao encontrada: %s (Tema: %s)\n", lista[indice].escola, lista[indice].tema);
    printf("Situacao atual: %s\n", statusAtual);

    int novaSituacao;
    while (1) {
        printf("\nEscolha a nova situacao:\n");
        printf("  1. Planejada\n");
        printf("  2. Realizada\n");
        printf("  3. Cancelada\n");
        printf("Opcao: ");
        if (scanf("%d", &novaSituacao) != 1) {
            printf("[ERRO] Digite um numero valido (1, 2 ou 3)!\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (novaSituacao >= 1 && novaSituacao <= 3) {
            break;
        }
        printf("[ERRO] Opcao invalida! Escolha 1, 2 ou 3.\n");
    }

    lista[indice].situacao = novaSituacao;

    /* Se a ação foi realizada, registrar o público efetivamente atendido */
    if (novaSituacao == 2) {
        while (1) {
            printf("Digite a quantidade EFETIVA de participantes atendidos: ");
            if (scanf("%d", &lista[indice].qtd_efetiva) != 1) {
                printf("[ERRO] Digite um numero valido!\n");
                limparBuffer();
                continue;
            }
            limparBuffer();

            if (lista[indice].qtd_efetiva < 0) {
                printf("[ERRO] A quantidade nao pode ser negativa!\n");
            } else {
                break;
            }
        }
    } else if (novaSituacao == 3) {
        lista[indice].qtd_efetiva = 0;
    }

    printf("\n[SUCESSO] Situacao da acao %d atualizada com sucesso!\n", codBusca);
}

/* 5. Gerar Resumo Geral Estatístico (RF-05) */
void gerarResumoGeral(const Acao lista[], int total) {
    if (total == 0) {
        printf("\nNenhum dado cadastrado para consolidacao estatistica.\n");
        return;
    }

    int planejadas = 0, realizadas = 0, canceladas = 0;
    int totPrevisto = 0, totEfetivo = 0;
    
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].situacao == 1) planejadas++;
        else if (lista[i].situacao == 2) realizadas++;
        else if (lista[i].situacao == 3) canceladas++;

        totPrevisto += lista[i].qtd_prevista;
        totEfetivo += lista[i].qtd_efetiva;
    }

    float percentual = 0.0f;
    if (totPrevisto > 0) {
        percentual = ((float)totEfetivo / (float)totPrevisto) * 100.0f;
    }

    printf("\n============================================================\n");
    printf("         RESUMO GERAL ESTATISTICO DO SISTEMA PSE            \n");
    printf("============================================================\n");
    printf("Total de Acoes Coletivas Cadastradas: %d\n", total);
    printf("  • Acoes Planejadas : %2d (%5.1f%%)\n", planejadas, (total > 0 ? (float)planejadas * 100.0f / total : 0.0f));
    printf("  • Acoes Realizadas : %2d (%5.1f%%)\n", realizadas, (total > 0 ? (float)realizadas * 100.0f / total : 0.0f));
    printf("  • Acoes Canceladas : %2d (%5.1f%%)\n", canceladas, (total > 0 ? (float)canceladas * 100.0f / total : 0.0f));
    printf("------------------------------------------------------------\n");
    printf("Participantes Previstos : %d\n", totPrevisto);
    printf("Participantes Atendidos : %d\n", totEfetivo);
    printf("Taxa Geral de Atingimento : %.2f%%\n", percentual);
    printf("============================================================\n");
}

/* Funções utilitárias */
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void removerQuebraLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}