#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_REGISTERS 32
#define MEMORY_SIZE 1024

// Estrutura para instruções no formato R
struct InstrucaoR {
    unsigned int opcode;
    unsigned int rd;
    unsigned int rs;
    unsigned int rt;
    unsigned int shiftAmount;
    unsigned int funct;
};

// Estrutura para instruções no formato I
struct InstrucaoI {
    unsigned int opcode;
    unsigned int rt;
    unsigned int rs;
    int immediate;
};

// Estrutura para instruções no formato J
struct InstrucaoJ {
    unsigned int opcode;
    unsigned int jumpAddress;
};

// Estrutura para representar o estado do processador
struct EstadoProcessador {
    int registradores[NUM_REGISTERS];
    int memoria[MEMORY_SIZE];
    unsigned int pc; // Program Counter
};

// Função para determinar o formato da instrução
int determinarFormato(unsigned int opcode) {
    printf("Opcode Formato: %u\n", opcode);
    if (opcode == 0) {
        return 1; // Formato R
    } else if (opcode == 0b100011 || opcode == 0b101011) {
        return 2; // Formato I
    } else if (opcode == 0b000010 || opcode == 0b000011) {
        return 3; // Formato J
    } else {
        return 0; // Formato desconhecido
    }
}

// Função para converter um número inteiro para uma string binária
unsigned int binaryStringToInt(const char* s) {
    if (s == NULL) return 0; /* nenhum string de entrada */
    size_t len = strlen(s);
    unsigned int result = 0;
    for (size_t i = 0; i < len; ++i) {
        char ch = s[i];
        result = (result << 1) + (ch - '0');
    }
    return result;
}

// Função para converter um opcode para binário usando a tabela ASCII
unsigned int opcodeToBinary(const char *instrucao, int *formato) {

    const char *instructionsR[] = {
        "add", "addu", "sub", "subu", "and", "or", "xor", "nor", "slt", "sltu",  // R-Format
    };

    const char *instructionsI[] = {
        "addi", "addiu", "andi", "ori", "xori", "slti", "sltiu",               // I-Format
        "lui", "lb", "lh", "lwl", "lw", "lbu", "lhu", "lwr", "li",                   // I-Format (load)
        "sb", "sh", "swl", "sw", "swr",                                      // I-Format (store)
        "beq", "bne", "blez", "bgtz", "bltz", "bgez", "bltzal", "bgezal",    // I-Format (branch)
    };

    const char *instructionsJ[] = {
        "j", "jal"                                                            // J-Format
    };

    const char *opcodes[] = {
        "000000", "000000", "000000", "000000", "000000", "000000", "000000", "000000", "000000", "000000",  // Funct determina a operação para R-Format
        "001000", "001001", "001100", "001101", "001110", "001010", "001011",                            // Opcodes para I-Format
        "001111", "100000", "100001", "100010", "100011", "100100", "100101", "100110", "001001",                  // Opcodes para I-Format (load)
        "101000", "101001", "101010", "101011", "101110",                                             // Opcodes para I-Format (store)
        "000100", "000101", "000110", "000111", "000001", "000001", "000001", "000001",                // Opcodes para I-Format (branch)
        "000010", "000011"                                                                               // Opcodes para J-Format
    };

    int i;
    for (i = 0; i < sizeof(instructionsI) / sizeof(instructionsI[0]); ++i) {
        if (strcmp(instrucao, instructionsI[i]) == 0) {
            *formato = 2; // Formato I
            break;
        }
    }

    if (i == sizeof(instructionsI) / sizeof(instructionsI[0])) {
        for (i = 0; i < sizeof(instructionsR) / sizeof(instructionsR[0]); ++i) {
            if (strcmp(instrucao, instructionsR[i]) == 0) {
                *formato = 1; // Formato R
                break;
            }
        }

        if (i == sizeof(instructionsR) / sizeof(instructionsR[0])) {
            for (i = 0; i < sizeof(instructionsJ) / sizeof(instructionsJ[0]); ++i) {
                if (strcmp(instrucao, instructionsJ[i]) == 0) {
                    *formato = 3; // Formato J
                    break;
                }
            }

            if (i == sizeof(instructionsJ) / sizeof(instructionsJ[0])) {
                printf("Instrucao desconhecida: %s\n", instrucao);
                *formato = 0; // Formato desconhecido
                return 0; // Retorna 0 em caso de formato desconhecido
            }
        }
    }

    // Impressões adicionadas para depuração
    printf("Instrucao: %s\n", instrucao);
    printf("Formato: %d\n", *formato);
    printf("i: %d\n", i);

    // Encontrou a instrução, agora use o índice para obter o opcode
    const char *opcode = opcodes[i];

    // Converte o opcode para um número inteiro
    unsigned int opcodeInt = strtol(opcode, NULL, 2);

    // Retorna o opcode encontrado
    return opcodeInt;
}

// Função para converter uma instrução MIPS para seu equivalente binário
unsigned int converteParaBinario(const char *instrucao) {
    char inst[7], rd[6], rs[6], rt[6], immediate[17], jumpAddress[27];

    printf("Instrucao: %s\n", instrucao);

    // Verifica o formato da instrução (R, I, J)
    sscanf(instrucao, "%6s", inst);

    printf("Formato: %s\n", inst);

    int formato;
    unsigned int opcode;
    opcodeToBinary(inst, &formato);
  
    printf("Opcode: %u\n", opcode);

    unsigned int rdInt, rsInt, rtInt, immediateInt, jumpAddressInt;

    // Monta a instrução binária de acordo com o formato
    unsigned int instrucaoBinaria = 0;

    //int formatoInt  = determinarFormato(opcodeBinary);

    printf("Formato: %d\n", formato);

    switch (determinarFormato(formato)) {
        case 1: {
            // Formato R
            sscanf(instrucao, "%s $%[^,], $%[^,], $%[^\n]", formato, rd, rs, rt);

            printf("formato: %s\n", formato);
            printf("rd: %s\n", rd);
            printf("rs: %s\n", rs);
            printf("rt: %s\n", rt);

            rdInt = atoi(rd + 1);
            rsInt = atoi(rs + 1);
            rtInt = atoi(rt + 1);
            printf("rdInt: %d\n", rdInt);
            printf("rsInt: %d\n", rsInt);
            printf("rtInt: %d\n", rtInt);
            instrucaoBinaria = (opcode << 26) | (rsInt << 21) | (rtInt << 16) | (rdInt << 11);

            break;
        }
        case 2: {
            // Formato I
            sscanf(instrucao, "%6s $%[^,],$%[^(](%[^\n)])", formato, rt, immediate, rs);
            rtInt = strtol(rt + 1, NULL, 10);
            rsInt = strtol(rs + 1, NULL, 10);
            immediateInt = strtol(immediate, NULL, 10);
            instrucaoBinaria = (opcode << 26) | (rsInt << 21) | (rtInt << 16) | (immediateInt & 0xFFFF);
            break;
        }
        case 3: {
            // Formato J
            sscanf(instrucao, "%6s %u", formato, jumpAddress);
            jumpAddressInt = strtol(jumpAddress, NULL, 10);
            instrucaoBinaria = (opcode << 26) | (jumpAddressInt & 0x3FFFFFF);
            break;
        }
        default:
            printf("Formato desconhecido\n");
            break;
    }
    
    return instrucaoBinaria;
}

// Funções para execução de instruções nos diferentes formatos
void executaInstrucaoR(struct EstadoProcessador *estado, struct InstrucaoR *instrucao) {
    unsigned int resultado;

    switch (instrucao->funct) {
        case 0b100000:
            // add
            resultado = estado->registradores[instrucao->rs] + estado->registradores[instrucao->rt];
            break;
        case 0b100001:
            // addu
            resultado = estado->registradores[instrucao->rs] + estado->registradores[instrucao->rt];
            break;
        case 0b100100:
            // and
            resultado = estado->registradores[instrucao->rs] & estado->registradores[instrucao->rt];
            break;
        case 0b011010:
            // div
            estado->registradores[32] = estado->registradores[instrucao->rs] / estado->registradores[instrucao->rt];  // LO register
            estado->registradores[33] = estado->registradores[instrucao->rs] % estado->registradores[instrucao->rt];  // HI register
            break;
        case 0b011011:
            // divu
            estado->registradores[32] = (unsigned int)estado->registradores[instrucao->rs] / (unsigned int)estado->registradores[instrucao->rt];  // LO register
            estado->registradores[33] = (unsigned int)estado->registradores[instrucao->rs] % (unsigned int)estado->registradores[instrucao->rt];  // HI register
            break;
        case 0b001001:
            // jalr
            estado->registradores[instrucao->rd] = estado->pc + 4;
            estado->pc = estado->registradores[instrucao->rs];
            break;
        case 0b001000:
            // jr
            estado->pc = estado->registradores[instrucao->rs];
            break;
        case 0b010000:
            // mfhi
            estado->registradores[instrucao->rd] = estado->registradores[33];  // HI register
            break;
        case 0b010010:
            // mflo
            estado->registradores[instrucao->rd] = estado->registradores[32];  // LO register
            break;
        case 0b011000:
            // mult
            long long mult_result = (long long)estado->registradores[instrucao->rs] * estado->registradores[instrucao->rt];
            estado->registradores[32] = (int)(mult_result & 0xFFFFFFFF);  // LO register
            estado->registradores[33] = (int)(mult_result >> 32);  // HI register
            break;
        case 0b011001:
            // multu
            unsigned long long multu_result = (unsigned long long)(unsigned int)estado->registradores[instrucao->rs] * (unsigned int)estado->registradores[instrucao->rt];
            estado->registradores[32] = (int)(multu_result & 0xFFFFFFFF);  // LO register
            estado->registradores[33] = (int)(multu_result >> 32);  // HI register
            break;
        case 0b100111:
            // nor
            estado->registradores[instrucao->rd] = ~(estado->registradores[instrucao->rs] | estado->registradores[instrucao->rt]);
            break;
        case 0b100101:
            // or
            resultado = estado->registradores[instrucao->rs] | estado->registradores[instrucao->rt];
            break;
        case 0b000000:
            // sll
            resultado = estado->registradores[instrucao->rt] << instrucao->shiftAmount;
            break;
        case 0b000100:
            // sllv
            resultado = estado->registradores[instrucao->rt] << estado->registradores[instrucao->rs];
            break;
        case 0b101010:
            // slt
            resultado = (estado->registradores[instrucao->rs] < estado->registradores[instrucao->rt]);
            break;
        case 0b101011:
            // sltu
            resultado = ((unsigned int)estado->registradores[instrucao->rs] < (unsigned int)estado->registradores[instrucao->rt]);
            break;
        case 0b000011:
            // sra
            resultado = estado->registradores[instrucao->rt] >> instrucao->shiftAmount;
            break;
        case 0b000111:
            // srav
            resultado = estado->registradores[instrucao->rt] >> estado->registradores[instrucao->rs];
            break;
        case 0b000010:
            // srl
            resultado = (unsigned int)estado->registradores[instrucao->rt] >> instrucao->shiftAmount;
            break;
        case 0b000110:
            // srlv
            resultado = (unsigned int)estado->registradores[instrucao->rt] >> estado->registradores[instrucao->rs];
            break;
        case 0b100010:
            // sub
            resultado = estado->registradores[instrucao->rs] - estado->registradores[instrucao->rt];
            break;
        case 0b100011:
            // subu
            resultado = estado->registradores[instrucao->rs] - estado->registradores[instrucao->rt];
            break;
        case 0b100110:
            // xor
            resultado = estado->registradores[instrucao->rs] ^ estado->registradores[instrucao->rt];
            break;
        
        default:
            printf("Funct desconhecido: %u\n", instrucao->funct);
            return;
    }

    if (instrucao->funct != 0b100101 && instrucao->funct != 0b000000 && instrucao->funct != 0b000110) {
        estado->registradores[instrucao->rd] = resultado;
    }
}

void executaInstrucaoI(struct EstadoProcessador *estado, struct InstrucaoI *instrucao) {
    int endereco = estado->registradores[instrucao->rs] + instrucao->immediate;

    switch (instrucao->opcode) {
        case 0b100000:
            // lb
            estado->registradores[instrucao->rt] = (signed char)estado->memoria[endereco];
            break;
        case 0b100100:
            // lbu
            estado->registradores[instrucao->rt] = (unsigned char)estado->memoria[endereco];
            break;
        case 0b100001:
            // lh
            estado->registradores[instrucao->rt] = (short)estado->memoria[endereco];
            break;
        case 0b100101:
            // lhu
            estado->registradores[instrucao->rt] = (unsigned short)estado->memoria[endereco];
            break;
        case 0b001111:
            // lui
            estado->registradores[instrucao->rt] = instrucao->immediate << 16;
            break;
        case 0b100011:
            // lw
            estado->registradores[instrucao->rt] = estado->memoria[endereco];
            break;
        case 0b110001:
            // lwc1
            printf("Implemente a instrução lwc1\n");
            break;
        case 0b110101:
            // ldc1
            printf("Implemente a instrução ldc1\n");
            break;
        case 0b101000:
            // sb
            estado->memoria[endereco] = estado->registradores[instrucao->rt];
            break;
        case 0b101001:
            // sh
            estado->memoria[endereco] = estado->registradores[instrucao->rt];
            break;
        case 0b101011:
            // sw
            estado->memoria[endereco] = estado->registradores[instrucao->rt];
            break;
        case 0b111001:
            // swc1
            printf("Implemente a instrução swc1\n");
            break;
        case 0b111101:
            // sdc1
            printf("Implemente a instrução sdc1\n");
            break;
        case 0b001101:
            // ori
            estado->registradores[instrucao->rt] = estado->registradores[instrucao->rs] | instrucao->immediate;
            break;
        case 0b001100:
            // andi
            estado->registradores[instrucao->rt] = estado->registradores[instrucao->rs] & instrucao->immediate;
            break;
        case 0b001110:
            // xori
            estado->registradores[instrucao->rt] = estado->registradores[instrucao->rs] ^ instrucao->immediate;
            break;
        case 0b001001:
            // li
            estado->registradores[instrucao->rt] = instrucao->immediate;
            break;

        default:
            printf("Opcode desconhecido: %u\n", instrucao->opcode);
            return;
    }
}

void executaInstrucaoJ(struct EstadoProcessador *estado, struct InstrucaoJ *instrucao) {
    estado->pc = instrucao->jumpAddress;
}

// Função para atualizar o PC
void atualizaPC(struct EstadoProcessador *estado) {
    estado->pc += 4; // Incremento típico para a próxima instrução
}

// Função para converter uma instrução MIPS para sua estrutura correspondente
void converteParaEstrutura(const char *instrucao, int formato, struct EstadoProcessador *estado, void *instrucaoEstrutura) {
    if (formato == 1) {
        // Formato R
        struct InstrucaoR *instrucaoR = (struct InstrucaoR *)instrucaoEstrutura;
        sscanf(instrucao, "%s $%d, $%d, $%d", instrucaoR->opcode, &instrucaoR->rd, &instrucaoR->rs, &instrucaoR->rt);
    } else if (formato == 2) {
        // Formato I
        struct InstrucaoI *instrucaoI = (struct InstrucaoI *)instrucaoEstrutura;
        sscanf(instrucao, "%s $%d, $%d, %d", instrucaoI->opcode, &instrucaoI->rt, &instrucaoI->rs, &instrucaoI->immediate);
    } else if (formato == 3) {
        // Formato J
        struct InstrucaoJ *instrucaoJ = (struct InstrucaoJ *)instrucaoEstrutura;
        sscanf(instrucao, "%s %u", instrucaoJ->opcode, &instrucaoJ->jumpAddress);
    }
}

// Função para imprimir o estado atual do processador
void imprimeEstado(struct EstadoProcessador *estado) {
    printf("\nEstado Atual do Processador:\n");

    // Imprime registradores
    printf("Registradores:\n");
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("$%d: %d\t", i, estado->registradores[i]);
        if ((i + 1) % 4 == 0) {
            printf("\n");
        }
    }

    // Imprime PC
    printf("PC: %d\n", estado->pc);
}

// Função para executar todas as etapas do processador
void executaProcessador(const char *instrucao, struct EstadoProcessador *estado) {
    // Converte a instrução para binário
    unsigned int instrucaoBinaria = converteParaBinario(instrucao);

    // Determinar o formato com base no opcode
    int formato = determinarFormato((instrucaoBinaria >> 26) & 0x3F);

    // Estrutura para armazenar a instrução
    union Instrucao {
        struct InstrucaoR r;
        struct InstrucaoI i;
        struct InstrucaoJ j;
    } instrucaoEstrutura;

    // Converte a instrução para sua estrutura correspondente
    converteParaEstrutura(instrucao, formato, estado, &instrucaoEstrutura);

    // Executa as etapas do processador
    switch (formato) {
        case 1:
            executaInstrucaoR(estado, &instrucaoEstrutura.r);
            break;
        case 2:
            executaInstrucaoI(estado, &instrucaoEstrutura.i);
            break;
        case 3:
            executaInstrucaoJ(estado, &instrucaoEstrutura.j);
            break;
        default:
            printf("Formato desconhecido\n");
            break;
    }

    // Imprime o estado atual do processador após a execução
    imprimeEstado(estado);

    // Atualizar o PC para a próxima instrução
    atualizaPC(estado);
}


int main() {
    struct EstadoProcessador estado;
    memset(&estado, 0, sizeof(struct EstadoProcessador)); // Inicializa o estado com zeros

    while (1) {
        char instrucao[50];
        printf("Digite uma instrucao MIPS (ou 'exit' para encerrar): ");
        fgets(instrucao, sizeof(instrucao), stdin);

        printf("Instrucao antes: %s\n", instrucao);

        // Remove o caractere de nova linha da string
        instrucao[strcspn(instrucao, "\n")] = '\0';

        printf("Instrucao depois: %s\n", instrucao);

        // Verifica se o usuário deseja encerrar o programa
        if (strcmp(instrucao, "exit") == 0) {
            break;
        }

        // Converte a instrução para binário
        unsigned int instrucaoBinaria = converteParaBinario(instrucao);

        // Imprime o resultado
        printf("Instrucao Binaria: %08X\n", instrucaoBinaria);

        // Determinar o formato com base no opcode
        int formato = determinarFormato((instrucaoBinaria >> 26) & 0x3F);

        switch (formato) {
            case 1: {
                // Formato R
                struct InstrucaoR instrucaoR;
                instrucaoR.opcode = (instrucaoBinaria >> 26) & 0x3F;
                instrucaoR.rd = (instrucaoBinaria >> 11) & 0x1F;
                instrucaoR.rs = (instrucaoBinaria >> 21) & 0x1F;
                instrucaoR.rt = (instrucaoBinaria >> 16) & 0x1F;
                instrucaoR.shiftAmount = (instrucaoBinaria >> 6) & 0x1F;
                instrucaoR.funct = instrucaoBinaria & 0x3F;
                executaInstrucaoR(&estado, &instrucaoR);
                break;
            }
            case 2: {
                // Formato I
                struct InstrucaoI instrucaoI;
                instrucaoI.opcode = (instrucaoBinaria >> 26) & 0x3F;
                instrucaoI.rt = (instrucaoBinaria >> 16) & 0x1F;
                instrucaoI.rs = (instrucaoBinaria >> 21) & 0x1F;
                instrucaoI.immediate = (short)(instrucaoBinaria & 0xFFFF); // Converte para signed short
                executaInstrucaoI(&estado, &instrucaoI);
                break;
            }
            case 3: {
                // Formato J
                struct InstrucaoJ instrucaoJ;
                instrucaoJ.opcode = (instrucaoBinaria >> 26) & 0x3F;
                instrucaoJ.jumpAddress = instrucaoBinaria & 0x3FFFFFF;
                executaInstrucaoJ(&estado, &instrucaoJ);
                break;
            }
            default:
                printf("Formato desconhecido\n");
                break;
        }

        // Executa todas as etapas do processador
        executaProcessador(instrucao, &estado);
    }

    return 0;
}
