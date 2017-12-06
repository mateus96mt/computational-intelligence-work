#include <iostream>
#include "Grafo.h"
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <climits>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include <random>


///INFORMACOES DOS NOS: c;carga=potencia-ativa, p_re=potencia reativa, v=voltagem no no(nao usei pra nada ainda)
///INFORMACOES DAS ARESTAS: r=resistencia, reat=reatancia, fat = fluxo de potencia ativa, freat= fluxo de potencia reativa

using namespace std;



void testeFLuxos();

void testeConstrutivoAleatorio();

void testeGenetico();

void testeMutacao();

void testeCruzamento();

void testeEntrada();

void testeBuscaLocal();

void tentaAcharMelhor();

void testeMemoria();

void testeConstrutivo1();

int main()
{
    srand(time(NULL));

//    tentaAcharMelhor();

//    testeEntrada();

//    testeConstrutivo1();

    testeMemoria();

//    testeGenetico();

//    testeCruzamento();

//    testeMutacao();

//    testeConstrutivoAleatorio();

//    testes();

//    testePercursoProfundidade();

//    testeFLuxos();

//    testeConstrutivo();

//    testeMovimentoSolucao();

//    testeBuscaLocal();

    return 0;

}

void testeConstrutivo1(){
    Grafo *g = new Grafo();
//    char nome[] = "entradas/sist33barras_Yang.m";
    char nome[] = "entradas/sist33barras_Yang-modificado.m";
//    char nome[] = "entradas/SISTEMA119s2.m";

    g->leEntrada(nome);

    cout << "\n\n\n";

    Solucao *solucao1 = g->construtivo1(0);

    g->imprimeSolucao(solucao1);

    cout << "\n\neh solucao? " << g->verificaSolucaoValida(solucao1) << endl;
    cout << "tensao minima:" << g->tensaoMinima() << endl;
    cout << "funcao objetivo: " << 100*1000*solucao1->valorObjetivo << endl;


    Solucao *solucao2 = g->construtivo1(1);

    g->imprimeSolucao(solucao2);

    cout << "\n\neh solucao? " << g->verificaSolucaoValida(solucao2) << endl;
    cout << "tensao minima:" << g->tensaoMinima() << endl;
    cout << "funcao objetivo: " << 100*1000*solucao2->valorObjetivo << endl;

}

void testeMemoria(){
    Grafo *g = new Grafo();

    char nome[] = "entradas/sist33barras_Yang.m";

    g->leEntrada(nome);

    cout << "\n\n\n";

//    ///construtivo aleatorio:
//    Solucao *solucao;
//    while(true){
//        solucao = g->construtivoAleatorio();
//
//        g->desalocaSolucao(solucao);
//    }

    ///populacao aleatoria
//    vector<Solucao*> vet;
//    while(true){
//        vet = g->populacaoInicial(100);
//
//        for(int i=0; i<100; i++)
//            g->desalocaSolucao(vet[i]);
//    }

    ///busca local
//    Solucao *solucao, *const_;
//    while(true){
//        const_ = g->construtivoAleatorio();
//        solucao = g->buscaLocal(const_, 0, 1, 2);
//
//        g->desalocaSolucao(const_);
//        g->desalocaSolucao(solucao);
//
//    }

    ///populacao busca local
//    vector<Solucao*> vet;
//    while(true){
//        vet = g->populacaoInicialBuscaLocal(100);
//        for(int i=0; i<100; i++){
//            g->desalocaSolucao(vet[0]);
//            vet.erase(vet.begin());
//        }
//    }


    ///cruzamento
//    Solucao *pai1, *pai2, *filho;
//
//    while(true){
//        pai1 = g->construtivoAleatorio();
//        pai2 = g->construtivoAleatorio();
//
//        filho = g->cruzamentoAleatorio(pai1, pai2);
//
//        g->desalocaSolucao(pai1);
//        g->desalocaSolucao(pai2);
//        g->desalocaSolucao(filho);
//    }

    ///mutacao
//    Solucao *s1;
//    while(true){
//        s1 = g->construtivoAleatorio();
//        g->mutacao(s1, 100);
//
//        g->desalocaSolucao(s1);
//
//    }

    ///proxima geracao
//    vector<Solucao*> pop_anterior = g->populacaoInicial(100);
//
//    while(true){
//        g->proximaGeracao(pop_anterior, 5);
//    }

    Solucao *s;
    while(true){
        s = g->algoritmoGenetico(1000, 5, 100);
        g->desalocaSolucao(s);
    }

}

void tentaAcharMelhor(){
    Grafo *g = new Grafo();
    char nome[] = "entradas/SISTEMA119s2.m";
    g->leEntrada(nome);

    Solucao *melhor = g->procuraMelhorSolucao(10000000);




    Grafo *h = new Grafo();
    h->leEntrada(nome);

    printf("\n\nfinal: %f", 100*1000*h->funcaoObjetivo(melhor, erro_fObjetivo) );
}

void testeBuscaLocal(){
    Grafo *g = new Grafo();
    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);

    for(int i=0; i<100000; i++){

        u_int id1 = rand() % g->nosEntrada.size();
        u_int id2 = rand() % g->nosEntrada.size();
        u_int id3 = rand() % g->nosEntrada.size();

//        cout << "id1: " << id1 << "   id2: " << id2 << "   id3: " << id3 << endl;

        Solucao *solucao = g->buscaLocal(g->construtivoAleatorio(), id1, id2, id3);
        if(g->verificaSolucaoValida(solucao)==false)
            cout << "solucao invalida!" << endl;

//        g->imprimeSolucao(solucao);
    }
}

void testeGenetico(){
    int num_exec = 10;

    ofstream saida;
    saida.open("saida.txt");

    Grafo *g = new Grafo();

    //    char nome[] = "entradas/SISTEMA119s2.m";
    char nome[] = "entradas/sist33barras_Yang.m";
//    char nome[] = "entradas/SISTEMA83_TAIWAN.m";

    g->leEntrada(nome);

    Solucao *solucao;

    for(int taxa_mutacao=5; taxa_mutacao<=100; taxa_mutacao+=5){

        saida << "alternado(1000it, popBuscaLocal, 5ciclos, taxa_mut " << taxa_mutacao << ")\nx=c(";
        for(int i=0; i<num_exec; i++){

            printf("EXECUCAO %d", i+1);
            solucao = g->algoritmoGenetico(100, 5, 100);

            if(g->verificaSolucaoValida(solucao)==true){
                saida << 100*1000*solucao->valorObjetivo;
                cout << "\nsolucao genetico: " << 100*1000*solucao->valorObjetivo << "\n\n";
            }
            else
                saida << "solucao invalida!";

            if(i<num_exec-1)
                saida << ",";

            g->desalocaSolucao(solucao);

        }
        saida << ")\n\n";
        printf("rodou %d vezes o genetico com taxa de mutacao %d\n\n\n", num_exec, taxa_mutacao);

    }


    saida.close();
}

void testeConstrutivoAleatorio(){
    Grafo *g = new Grafo();

    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);


    Solucao *solucao = new Solucao;
    solucao->vetChaves = new bool*[g->nosEntrada.size()];
    for(u_int i=0; i<g->nosEntrada.size(); i++)
        solucao->vetChaves[i] = new bool[g->nosEntrada.at(i)->volta.size()];
    for(u_int i=0; i<g->nosEntrada.size(); i++){
        for(u_int j=0; j<g->nosEntrada.at(i)->volta.size(); j++){
            solucao->vetChaves[i][j] = true;
        }
    }

    cout << "\nentrada e solucao valida:" << g->verificaSolucaoValida(solucao) << endl;

    solucao = g->construtivoAleatorio();


    cout << "\n\n\nfuncao objetivo: " << solucao->valorObjetivo << endl;

    g->desmarcaNos();

    cout << "\nsolucao e valida:" << g->verificaSolucaoValida(solucao) << endl;

//    g->imprime();
}

void testeEntrada(){

    Grafo *g = new Grafo();

//    char nome[] = "entradas/SISTEMA119s2.m";
//    char nome[] = "entradas/sist33barras_Yang.m";
    char nome[] = "entradas/sist33barras_Yang-modificado.m";


    g->leEntrada(nome);

    for(u_int i=g->numeroNos; i<=g->numeroArcos; i++){
//        cout << "i: " << i << endl;
        Arco *a = g->buscaArcoID(i);
        a->chave=false;
    }

    g->calcula_fluxos_e_perdas(1e-8);
    cout << "\ntensao minima:" << g->tensaoMinima();
    cout << "\nperdaTotal: " << 100*1000*g->soma_perdas()[0] << endl;


    Solucao *solucao = new Solucao;
    solucao->vetChaves = new bool*[g->nosEntrada.size()];
    for(u_int i=0; i<g->nosEntrada.size(); i++)
        solucao->vetChaves[i] = new bool[g->nosEntrada.at(i)->volta.size()];

    for(u_int i=0; i<g->nosEntrada.size(); i++){
        for(u_int j=0; j<g->nosEntrada.at(i)->volta.size(); j++){
            solucao->vetChaves[i][j] = g->nosEntrada.at(i)->volta.at(j)->chave;
        }
    }
    cout << "\n\n Eh solucao: " << g->verificaSolucaoValida(solucao);
    cout << "\n\n\n";
}

void testeMutacao(){
    Grafo *g = new Grafo();

    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);

    Solucao *solucao = g->construtivoAleatorio();
    cout  << "individuo original:\n";
    g->imprimeChaves(solucao);


    for(int i=0; i<10; i++){
        cout << "apos mutacao:\n";
        g->mutacao(solucao, 50);
//        g->imprimeChaves(solucao);
        cout << "objetivo: " << solucao->valorObjetivo << endl;
    }

}

void testeCruzamento(){
    Grafo *g = new Grafo();

    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);

    Solucao *pai1 = g->construtivoAleatorio();
    Solucao *pai2 = g->construtivoAleatorio();

    cout << "pai1:\n";
    g->imprimeChaves(pai1);
    cout << "pai2:\n";
    g->imprimeChaves(pai2);
    cout << "filho:\n";
    Solucao *filho = g->cruzamentoAleatorio(pai1, pai2);
    g->imprimeChaves(filho);

    cout << "\npai1: " << pai1->valorObjetivo;
    cout << "\npai2: " << pai2->valorObjetivo;
    cout << "\nfilho: " <<filho->valorObjetivo;
}
