/*
 *	Alunos: Gabriel Bodenmüller e Luiza Volpato Barbosa
 *  Matéria: Sistemas em Tempo Real
 *  Professor: Felipe Viel
 *  Titulo: Concorrência e multithread em STR
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>

int injecao = 0, temperatura = 0, ABS = 0, airbag = 0, cinto = 0, luz = 0, vidro = 0, trava = 0, esq = 0;
clock_t tempo;
pthread_mutex_t em = PTHREAD_MUTEX_INITIALIZER;

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

/***
 *	Codigo de thread leitura de teclado
 ***/
void thr_le_teclado(void)
{
    char tecla;
    while (1)
    {
        if (kbhit())
        {
            // Stores the pressed key in ch
            tecla = getchar();
            // Terminates the loop
            // when escape is pressed
            if ((int)tecla == 118) // leitura do acionamento do vidro
            {                      // v minusculo
                pthread_mutex_lock(&em);
                vidro = 1;
                pthread_mutex_unlock(&em);
            }
            else if ((int)tecla == 105) // leitura do acionamento da injeção eletronica
            {                           // i minusculo
                pthread_mutex_lock(&em);
                injecao = 1;
                pthread_mutex_unlock(&em);
            }
            else if ((int)tecla == 116) // leitura do acionamento da temperatura do motor
            {                           // t minusculo
                pthread_mutex_lock(&em);
                temperatura = 1;
                pthread_mutex_unlock(&em);
            }
            else if ((int)tecla == 97) // leitura do acionamento do ABS
            {                          // a minusculo
                pthread_mutex_lock(&em);
                ABS = 1;
                pthread_mutex_unlock(&em);
            }
            else if ((int)tecla == 98) // leitura do acionamento do Airbag
            {                          // b minusculo
                pthread_mutex_lock(&em);
                airbag = 1;
                pthread_mutex_unlock(&em);
            }
            else if ((int)tecla == 99) // leitura do acionamento do cinto de segurança
            {                          // c minusculo
                pthread_mutex_lock(&em);
                cinto = 1;
                pthread_mutex_unlock(&em);
            }
            else if ((int)tecla == 108) // leitura do acionamento da luz
            {                           // l minusculo
                pthread_mutex_lock(&em);
                luz = 1;
                pthread_mutex_unlock(&em);
            }
            else if ((int)tecla == 114) // leitura do acionamento das travas da porta
            {                           // r minusculo
                pthread_mutex_lock(&em);
                trava = 1;
                pthread_mutex_unlock(&em);
            }
            else if ((int)tecla == 27) // leitura para sair do sistema
            {                          // esq
                pthread_mutex_lock(&em);
                esq = 1;
                pthread_mutex_unlock(&em);
            }
        }
    }
}

void *thr_vidro(void) // Codigo de thread que verifica se a tecla lida equivale ao acionamento
{
    while (1)
    {
        pthread_mutex_lock(&em);
        if (vidro == 1)
        {
            tempo = clock();
            vidro = 0;
            tempo = clock() - tempo;
            double time_taken = ((double)tempo) / CLOCKS_PER_SEC; // in seconds
            if (time_taken > 1)
            {
                printf("Tempo de execução %f", time_taken);
                printf(" segundos");
                printf("\nVidro não acionou antes do deadline\n");
            }
            else
            {
                printf("Tempo de execução %f", time_taken);
                printf(" segundos");
                printf("\nVidro acionado com sucesso\n");
            }
        }
        pthread_mutex_unlock(&em);
    }
}

void *thr_injecao(void)
{
    while (1)
    {
        pthread_mutex_lock(&em);
        if (injecao == 1)
        {
            tempo = clock();
            injecao = 0;
            tempo = clock() - tempo;
            double time_taken = ((double)tempo) / CLOCKS_PER_SEC; // in seconds
            time_taken = time_taken * 1000000;
            if (time_taken > 500)
            {
                printf("Tempo de execução %f", time_taken);
                printf(" microsegundos");
                printf("\nInjeção Eletronica não acionou antes do deadline\n");
            }
            else
            {
                printf("Tempo de execução %f", time_taken);
                printf(" microsegundos");
                printf("\nInjeção Eletronica acionado com sucesso\n");
            }
        }
        pthread_mutex_unlock(&em);
    }
}

void *thr_temperatura(void)
{
    while (1)
    {
        pthread_mutex_lock(&em);
        if (temperatura == 1)
        {
            tempo = clock();
            temperatura = 0;
            tempo = clock() - tempo;
            double time_taken = ((double)tempo) / CLOCKS_PER_SEC; // in seconds
            time_taken = time_taken * 1000;
            if (time_taken > 20)
            {
                printf("Tempo de execução %f", time_taken);
                printf(" milisegundos");
                printf("\nTemperatura do Motor não acionou antes do deadline\n");
            }
            else
            {
                printf("Tempo de execução %f", time_taken);
                printf(" milisegundos");
                printf("\nTemperatura do Motor acionado com sucesso\n");
            }
        }
        pthread_mutex_unlock(&em);
    }
}

void *thr_abs(void)
{
    while (1)
    {
        pthread_mutex_lock(&em);
        if (ABS == 1)
        {
            tempo = clock();
            ABS = 0;
            tempo = clock() - tempo;
            double time_taken = ((double)tempo) / CLOCKS_PER_SEC; // in seconds
            time_taken = time_taken * 1000;
            if (time_taken > 100)
            {
                printf("Tempo de execução %f", time_taken);
                printf(" milisegundos");
                printf("\nABS não acionou antes do deadline\n");
            }
            else
            {
                printf("Tempo de execução %f", time_taken);
                printf(" milisegundos");
                printf("\nABS acionado com sucesso\n");
            }
        }
        pthread_mutex_unlock(&em);
    }
}

void *thr_airbag(void)
{
    while (1)
    {
        pthread_mutex_lock(&em);
        if (airbag == 1)
        {
            tempo = clock();
            airbag = 0;
            tempo = clock() - tempo;
            double time_taken = ((double)tempo) / CLOCKS_PER_SEC; // in seconds
            time_taken = time_taken * 1000;
            if (time_taken > 100)
            {
                printf("Tempo de execução %f", time_taken);
                printf(" milisegundos");
                printf("\nAirbag não acionou antes do deadline\n");
            }
            else
            {
                printf("Tempo de execução %f", time_taken);
                printf(" milisegundos");
                printf("\nAirbag acionado com sucesso\n");
            }
        }
        pthread_mutex_unlock(&em);
    }
}

void *thr_cinto(void)
{
    while (1)
    {
        pthread_mutex_lock(&em);
        if (cinto == 1)
        {
            tempo = clock();
            cinto = 0;
            tempo = clock() - tempo;
            double time_taken = ((double)tempo) / CLOCKS_PER_SEC; // in seconds
            if (time_taken > 1)
            {
                printf("Tempo de execução %f", time_taken);
                printf(" segundos");
                printf("\nCinto de Segurança não acionou antes do deadline\n");
            }
            else
            {
                printf("Tempo de execução %f", time_taken);
                printf(" segundos");
                printf("\nCinto de Segurança acionado com sucesso\n");
            }
        }
        pthread_mutex_unlock(&em);
    }
}

void *thr_luz(void)
{
    while (1)
    {
        pthread_mutex_lock(&em);
        if (luz == 1)
        {
            tempo = clock();
            luz = 0;
            tempo = clock() - tempo;
            double time_taken = ((double)tempo) / CLOCKS_PER_SEC; // in seconds
            if (time_taken > 1)
            {
                printf("Tempo de execução %f", time_taken);
                printf(" segundos");
                printf("\nLuz não acionou antes do deadline\n");
            }
            else
            {
                printf("Tempo de execução %f", time_taken);
                printf(" segundos");
                printf("\nLuz acionado com sucesso\n");
            }
        }
        pthread_mutex_unlock(&em);
    }
}

void *thr_trava(void)
{
    while (1)
    {
        pthread_mutex_lock(&em);
        if (trava == 1)
        {
            tempo = clock();
            trava = 0;
            tempo = clock() - tempo;
            double time_taken = ((double)tempo) / CLOCKS_PER_SEC; // in seconds
            if (time_taken > 1)
            {
                printf("Tempo de execução %f", time_taken);
                printf(" segundos");
                printf("\nTrava da porta não acionou antes do deadline\n");
            }
            else
            {
                printf("Tempo de execução %f", time_taken);
                printf(" segundos");
                printf("\nTrava da porta acionado com sucesso\n");
            }
        }
        pthread_mutex_unlock(&em);
    }
}

void *thr_esq(void)
{
    while (1)
    {
        pthread_mutex_lock(&em);
        if (esq == 1)
        {
            printf("Desligando o sistema...\n");
            esq = 0;
            break;
        }
        pthread_mutex_unlock(&em);
    }
}

int main(int argc, char *argv[])
{

    // criação da estrutura de dados referente as threads usadas
    pthread_t t_le_teclado, t_vidro, t_injecao, t_temperatura, t_abs, t_airbag, t_cinto, t_luz, t_trava, t_esq;
    // criação das threads
    pthread_create(&t_le_teclado, NULL, (void *)thr_le_teclado, NULL);
    pthread_create(&t_vidro, NULL, (void *)thr_vidro, NULL);
    pthread_create(&t_injecao, NULL, (void *)thr_injecao, NULL);
    pthread_create(&t_temperatura, NULL, (void *)thr_temperatura, NULL);
    pthread_create(&t_abs, NULL, (void *)thr_abs, NULL);
    pthread_create(&t_airbag, NULL, (void *)thr_airbag, NULL);
    pthread_create(&t_cinto, NULL, (void *)thr_cinto, NULL);
    pthread_create(&t_luz, NULL, (void *)thr_luz, NULL);
    pthread_create(&t_trava, NULL, (void *)thr_trava, NULL);
    pthread_create(&t_esq, NULL, (void *)thr_esq, NULL);

    // espera as threads para finalizar o programa
    pthread_join(t_le_teclado, NULL);
    pthread_join(t_vidro, NULL);
    pthread_join(t_injecao, NULL);
    pthread_join(t_temperatura, NULL);
    pthread_join(t_abs, NULL);
    pthread_join(t_airbag, NULL);
    pthread_join(t_cinto, NULL);
    pthread_join(t_luz, NULL);
    pthread_join(t_trava, NULL);
    pthread_join(t_trava, NULL);

    return 0;
}