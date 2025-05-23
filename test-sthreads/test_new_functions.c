#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sthread.h>
#include <sthread_user.h>

// Simula trabalho e chama yield
void* tarefa(void* arg) {
    int id = *(int*)arg;
    printf("[Tarefa %d] Iniciou (tid=%d)\n", id, sthread_self()->tid);

    // Mostra estado atual do escalonador
    if (id == 0) {
        printf("[Tarefa %d] Chamando sthread_dump() no início\n", id);
        sthread_dump();
    }

    // Muda a prioridade com nice se for uma thread com prioridade dinâmica
    if (id == 1) {
        printf("[Tarefa %d] Chamando sthread_nice(5)...\n", id);
        int nova = sthread_nice(5);
        printf("[Tarefa %d] Nova prioridade retornada: %d\n", id, nova);
    }

    for (int i = 0; i < 3; i++) {
        printf("[Tarefa %d] Executando passo %d\n", id, i + 1);
        sthread_yield(); // força o escalonador a alternar
    }

    printf("[Tarefa %d] Terminou\n", id);
    free(arg);
    sthread_exit(NULL);
    return NULL;
}

int main() {
    sthread_init();

    printf("[Main] Criando tarefas com diferentes prioridades\n");

    // Cria 3 threads: uma fixa (prioridade 3) e duas dinâmicas (6 e 10)
    for (int i = 0; i < 3; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        int prioridade = (i == 0) ? 3 : (i == 1) ? 6 : 10;
        sthread_create(tarefa, id, prioridade);
    }

    for (int i = 0; i < 2; i++) {
        printf("[Main] Yield passo %d\n", i + 1);
        sthread_yield();
    }

    printf("[Main] Dump final do estado\n");
    sthread_dump();

    return 0;
}
