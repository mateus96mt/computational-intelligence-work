#include <iostream>
#include "Grafo.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <limits.h>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

int main()
{
    Grafo *g = new Grafo();
    g->leEntrada();

    g->imprime();
    g->n_marcados = 0;


    printf("\npartindo do no fonte:\n");
    ///fazendo o percurso em profundidade vemos no final se conseguimos marcar todos os nos(33 no caso)
    g->percursoProfundidade(g->getListaNos());


    ///assim o no 33, que e o primeiro da lista de adjacencia, e o no fonte
    cout << "num nos marcados:" << g->n_marcados << endl;
    g->desmarcaNos();




    ///so pra ver que com outro no qualquer vc nao consegue marcar todos os nos, so o no 33 é o no fonte
    printf("\npartindo do no 29:\n");
    g->percursoProfundidade(g->buscaNo(29));


    cout << "num nos marcados:" << g->n_marcados << endl;
    g->desmarcaNos();

    return 0;
}
