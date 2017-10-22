#include <iostream>
#include "Grafo.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <limits.h>
#include <vector>
#include <string>
#include <ctime>


///INFORMACOES DOS NOS: c;carga=potencia-ativa, p_re=potencia reativa, v=voltagem no no(nao usei pra nada ainda)
///INFORMACOES DAS ARESTAS: r=resistencia, reat=reatancia, fat = fluxo de potencia ativa, freat= fluxo de potencia reativa

using namespace std;

void imprimeVetorArcos(vector<Arco*> solucao);
void testeFLuxos();
void testeConstrutivo();
void testeMovimentoSolucao();
void testeBuscaLocal();
void testePercursoProfundidade();
void testeLeonardo();///funcao com calculo de fluxos, voltagem em barra e perdas nas linhas usando foward e backward

int main()
{
    srand(time(NULL));

    testeLeonardo();
//    testePercursoProfundidade();
//    testeFLuxos();
//    testeConstrutivo();
//    testeMovimentoSolucao();
//    testeBuscaLocal();

    return 0;
}

void testeLeonardo(){
    Grafo *g = new Grafo();

    char nome[] = "entrada_bruno.txt";
    g->leEntrada(nome);

    g->imprime();
    cout << "\n\n\n";

//    g->calcula_fluxos_e_perdas();
    g->foward(0);
    g->backward(0);

    g->imprime();
}

void testePercursoProfundidade(){
    Grafo *g = new Grafo();

    char nome[] = "entrada_bruno.txt";
    g->leEntrada(nome);

    g->percursoProfundidade(g->listaNos);

    g->imprime();
}

void imprimeVetorArcos(vector<Arco*> solucao){
    printf("\n\n------------------Vetor de arcos---------------\n\n");
    for(u_int i=0; i<solucao.size(); i++)
        printf("A%d (%d, %d) [ %d ]: perda = %2.5f\n", solucao.at(i)->getID(),
        solucao.at(i)->getNoOrigem()->getID(),
        solucao.at(i)->getNoDestino()->getID(),
        solucao.at(i)->chave,
        solucao.at(i)->perda_ativ);
    printf("\n\n-----------------------------------------------\n\n");
}

void testeFLuxos(){
    Grafo *g = new Grafo();

    char nome[] = "entrada_bruno.txt";
    g->leEntrada(nome);

    g->AtualizaFLuxos();

    g->imprime();
}

void testeConstrutivo(){
    Grafo *g = new Grafo();

    char nome[] = "entrada.txt";
    g->leEntrada(nome);

    g->imprime();
    g->n_marcados = 0;

    //    g->AtualizaFLuxos();
    g->construtivo();

    g->imprime();

    vector<Arco*> solucao = g->geraVetorArcos();

    imprimeVetorArcos(solucao);
}

void testeMovimentoSolucao(){

    u_int n_mov = 100;

    Grafo *g = new Grafo();

    char nome[] = "entrada.txt";
    g->leEntrada(nome);

    g->n_marcados = 0;

    //    g->AtualizaFLuxos();
    g->construtivo();


    vector<Arco*> solucao = g->geraVetorArcos();

    imprimeVetorArcos(solucao);
    g->calculaPerdaTotal();

    printf("\nPerda total: %2.5f\n", g->perdaTotal);

    printf("\n\nINICIO DOS MOVIMENTOS\n\n");
    for(u_int i=0; i<n_mov; i++){
        movimentoSolucao(solucao);
        g->AtualizaFLuxos();
        g->calculaPerdaTotal();

        if(g->validaSolucao()){
            imprimeVetorArcos(solucao);
            printf("Perda total: %2.5f\n", g->perdaTotal);
        }
        else
            printf("\nsolucao invalida!\n");
    }
}

void testeBuscaLocal(){
    ///numero de iteracoes sem melhora
    u_int n_mov = 100;

    Grafo *g = new Grafo();

    char nome[] = "entrada.txt";
    g->leEntrada(nome);

    g->n_marcados = 0;

    //    g->AtualizaFLuxos();
    g->construtivo();


    vector<Arco*> solucao = g->geraVetorArcos();

    bool chaveamento[solucao.size()];

    imprimeVetorArcos(solucao);
    g->calculaPerdaTotal();
    double perdaMinima = g->perdaTotal;

    u_int itSemMelhora = 0;

    printf("\nPerda total: %2.5f\n", g->perdaTotal);

    printf("\n\nINICIO DOS MOVIMENTOS\n\n");
    while(itSemMelhora<=n_mov){
        movimentoSolucao(solucao);
        g->AtualizaFLuxos();
        g->calculaPerdaTotal();

        if(g->validaSolucao() && g->perdaTotal < perdaMinima){
            printf("Perda total: %2.5f\n", g->perdaTotal);
            perdaMinima = g->perdaTotal;
            itSemMelhora = 0;
        }

        itSemMelhora++;
    }
}
