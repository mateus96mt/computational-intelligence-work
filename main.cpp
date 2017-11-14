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
void testes();
void testeConstrutivoAleatorio();
void testeGenetico();
void testeMutacao();
void testeCruzamento();
void imprimeDifPerdas(Grafo *g, Grafo *h);

int main()
{
    srand(time(NULL));

    testeGenetico();

//    testeConstrutivoAleatorio();

//    testes();

//    testeLeonardo();

//    testePercursoProfundidade();

//    testeFLuxos();

//    testeConstrutivo();

//    testeMovimentoSolucao();

//    testeBuscaLocal();

    return 0;
}

void testeConstrutivoAleatorio(){
    Grafo *g = new Grafo();

    char nome[] = "SISTEMA119s2 - base.m";
    g->leEntrada(nome);
//    g->imprime();

    g->desmarcaNos();
    g->ehArvore();
    bool **vetChaves = g->construtivoAleatorio();


    cout << "\n\n\nfuncao objetivo: " << g->funcaoObjetivo(vetChaves, 1e-8) << endl;

    g->desmarcaNos();
    g->ehArvore();

//    g->imprime();
}

void testes(){
    Grafo *g = new Grafo();

    char nome[] = "sist14barras.m";
    g->leEntrada(nome);

    g->imprime();

    cout << "\n\nsoma das cargas:" << g->cargasPerdasRamoAtiv(g->getListaNos()) << endl;

    g->calcula_fluxos_e_perdas(1e-6);
    g->imprime();


    bool **vetChaves = new bool*[g->numeroNos];
    for(u_int i=0; i<g->numeroNos; i++){
        vetChaves[i] = new bool[1];
        vetChaves[i][0] = true;
    }

    cout << "\nfuncao objetivo:" << g->funcaoObjetivo(vetChaves, 10) << endl;

//    g->foward(0);
//
//    g->imprime();
//
//    g->backward();
//
//    g->imprime();
//
//
//
//
//    g->foward(1);
//
//    g->imprime();
//
//    g->backward();
////
//    g->imprime();
}

//void testeLeonardo(){
//    Grafo *g = new Grafo();
//
//    char nome[] = "entrada_bruno.txt";
//    g->leEntrada(nome);
//
//    g->imprime();
//    cout << "\n\n\n";
//
//    g->calcula_fluxos_e_perdas();
//    g->foward(0);
//    g->backward(0);
//
//    g->imprime();
//}

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

void testeGenetico(){


    Grafo *g = new Grafo();

    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);
//
////    cout << "resultado genetico:" << g->funcaoObjetivo(g->algoritmoGenetico(100), 1e-6);
//
    g->buscaArco(46, 27)->chave=false;
    g->buscaArco(17, 27)->chave=false;
    g->buscaArco(8, 24)->chave=false;
    g->buscaArco(54, 43)->chave=false;
    g->buscaArco(62, 54)->chave=false;
    g->buscaArco(37, 62)->chave=false;
    g->buscaArco(9, 40)->chave=false;
    g->buscaArco(58, 96)->chave=false;
    g->buscaArco(73, 91)->chave=false;
    g->buscaArco(88, 75)->chave=false;
    g->buscaArco(99, 77)->chave=false;
    g->buscaArco(108, 83)->chave=false;
    g->buscaArco(105, 86)->chave=false;
    g->buscaArco(110, 118)->chave=false;
    g->buscaArco(25, 35)->chave=false;

//    g->imprime();

//    g->imprime();

    g->calcula_fluxos_e_perdas(1e-8);
//    g->foward(0);
//    g->backward();

//    g->imprime();

//    cout << "\n\n\nMAIN    nadress: " << g->buscaArco(8, 9) << endl;
//    printf("(8,9) - fluxo: (%f , %f)", g->buscaArco(8, 9)->fluxoP_ativ, g->buscaArco(8, 9)->fluxoP_reativ);

    cout << "\nresultado rede com chaves fechadas:" << g->soma_perdas()[0];


//    g->imprime();


    Grafo *h = new Grafo();
    char nome2[] = "SISTEMA119s2 - base.m";
    h->leEntrada(nome2);
////
    h->calcula_fluxos_e_perdas(1e-8);
//    h->foward(0);
////    h->backward();
//
    cout << "\nresultado entrada arvore base:" << h->soma_perdas()[0];

//    cout << "\n\nadress: " << h->buscaArco(8, 9) << endl;
//    printf("main(8,9) - fluxo: (%f , %f)", h->buscaArco(8, 9)->fluxoP_ativ, h->buscaArco(8, 9)->fluxoP_reativ);


    imprimeDifPerdas(g, h);
//    imprimeDifFluxos(g, h);



    cout << "\n\n\n";
}

void testeMutacao(){
    Grafo *g = new Grafo();

    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);

    bool **vetChaves = g->construtivoAleatorio();
    cout  << "individuo original:\n";
    g->imprimeChaves(vetChaves);


    for(int i=0; i<10; i++){
        cout << "apos mutacao:\n";
        g->mutacao(vetChaves);
        g->imprimeChaves(vetChaves);
    }

}

void testeCruzamento(){
    Grafo *g = new Grafo();

    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);

    bool **pai1 = g->construtivoAleatorio();
    bool **pai2 = g->construtivoAleatorio();

    cout << "pai1:\n";
    g->imprimeChaves(pai1);
    cout << "pai2:\n";
    g->imprimeChaves(pai2);
    cout << "filho:\n";
    g->imprimeChaves(g->cruzamento_metade(pai1, pai2));

}

void imprimeDifPerdas(Grafo *g, Grafo *h){
    No *hNo = h->listaNos;
    for(No *no = g->listaNos; no!=NULL; no = no->proxNo){

        Arco *ha = hNo->listaArcos;
        for(Arco *a=no->listaArcos; a!=NULL; a=a->proxArco){
            while(a!=NULL && a->chave==false)
                a = a->proxArco;
            if(a==NULL)
                break;

            cout << "g->a( " << a->noOrigem->id << " , " << a->noDestino->id << " ) = ( " << a->perda_ativ << " , " << a->perda_reat << " )";
            cout << "    h->a( " << ha->noOrigem->id << " , " << ha->noDestino->id << " ) = ( " << ha->perda_ativ << " , " << ha->perda_reat << " )";
            cout << "    diferenca:( " << a->perda_ativ - ha->perda_ativ << " , " << a->perda_reat - ha->perda_reat << endl;

            ha = ha->proxArco;
        }
        hNo = hNo->proxNo;

    }
}
