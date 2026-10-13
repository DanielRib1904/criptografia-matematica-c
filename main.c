#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LETRAS 16

int primo(int n) {
    int numero = 2;
    int quantidade = 0;

    while (1) {
        int ehPrimo = 1;

        for (int i = 2; i * i <= numero; i++) {
            if (numero % i == 0) {
                ehPrimo = 0;
                break;
            }
        }

        if (ehPrimo) {
            quantidade++;
            if (quantidade == n)
                return numero;
        }

        numero++;
    }
}

int sequencia(int tipo, int i) {
    if (tipo == 1) {
        return 1 + i * 2;
    }

    if (tipo == 2) {
        int valor = 1;

        for (int j = 0; j < i; j++)
            valor *= 2;

        return valor;
    }

    if (tipo == 3) {
        int a = 1, b = 1, c;

        if (i == 0 || i == 1)
            return 1;

        for (int j = 2; j <= i; j++) {
            c = a + b;
            a = b;
            b = c;
        }

        return b;
    }

    if (tipo == 4) {
        return primo(i + 1);
    }

    return 0;
}

int main(void) {
    char palavra[MAX_LETRAS];
    char original[MAX_LETRAS];
    char codificada[MAX_LETRAS];
    int shift, tipo;

    printf("=== CRIPTOGRAFIA MATEMATICA ===\n");
    printf("Digite uma palavra (ate 15 letras, sem acentos): ");

    if (fgets(palavra, sizeof(palavra), stdin) == NULL)
        return 1;

    palavra[strcspn(palavra, "\n")] = '\0';

    int tamanho = strlen(palavra);

    if (tamanho == 0 || tamanho > 15) {
        printf("Erro: digite de 1 a 15 letras.\n");
        return 1;
    }

    for (int i = 0; i < tamanho; i++) {
        if (!isalpha((unsigned char)palavra[i]) ||
            (unsigned char)palavra[i] > 127) {
            printf("Erro: use somente letras sem acentos.\n");
            return 1;
        }

        palavra[i] = toupper((unsigned char)palavra[i]);
    }

    strcpy(original, palavra);

    printf("Digite o SHIFT (0 a 25): ");

    if (scanf("%d", &shift) != 1 ||
        shift < 0 || shift > 25) {
        printf("SHIFT invalido.\n");
        return 1;
    }

    printf("\nEscolha a sequencia:\n");
    printf("1 - PA (1, 3, 5, 7...)\n");
    printf("2 - PG (1, 2, 4, 8...)\n");
    printf("3 - Fibonacci (1, 1, 2, 3...)\n");
    printf("4 - Numeros primos (2, 3, 5, 7...)\n");
    printf("Opcao: ");

    if (scanf("%d", &tipo) != 1 ||
        tipo < 1 || tipo > 4) {
        printf("Opcao invalida.\n");
        return 1;
    }

    for (int i = 0; i < tamanho; i++) {
        int deslocamento = shift + sequencia(tipo, i);

        codificada[i] =
            'A' + (palavra[i] - 'A' + deslocamento) % 26;
    }

    codificada[tamanho] = '\0';

    printf("\nPalavra original: %s\n", original);
    printf("Palavra criptografada: %s\n", codificada);

    FILE *arquivo = fopen("resultado_criptografia.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao criar arquivo de resultado.\n");
        return 1;
    }

    fprintf(arquivo,
        "Palavra codificada: %s | SHIFT: %d | Tipo: %d | Letras: %d\n",
        codificada, shift, tipo, tamanho);

    fclose(arquivo);

    FILE *log = fopen("log_execucao.txt", "a");

    if (log == NULL) {
        printf("Resultado salvo, mas nao foi possivel criar o log.\n");
        return 1;
    }

    time_t agora = time(NULL);

    fprintf(log,
        "Data: %sPalavra original: %s | Codificada: %s | "
        "SHIFT: %d | Tipo: %d | Letras: %d\n",
        ctime(&agora), original, codificada,
        shift, tipo, tamanho);

    fclose(log);

    printf("\nArquivos gerados com sucesso!\n");
    printf("- resultado_criptografia.txt\n");
    printf("- log_execucao.txt\n");

    return 0;
}
