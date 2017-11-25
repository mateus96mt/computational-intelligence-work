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
void teste();
void testeConstrutivoKtruskal();
void buscaLocal();
void tentaAcharMelhor();

int main()
{
    srand(time(NULL));

//    const int nrolls=10000;  // number of experiments
//  const int nstars=95;     // maximum number of stars to distribute
//  const int nintervals=10; // number of intervals
//
//  std::default_random_engine generator;
//  std::uniform_real_distribution<double> distribution(0.0,1.0);
//
//  for (int i = 0; i < 10; i++)
//  {
//      double number = distribution(generator);
//    cout << number << endl;
//  }


//    tentaAcharMelhor();


    testeGenetico();


//    buscaLocal();

//    testeConstrutivoKtruskal();

//    teste();

//    testeCruzamento();

//    testeMutacao();

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

void tentaAcharMelhor(){
    Grafo *g = new Grafo();
    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);

    Solucao melhor = g->procuraMelhorSolucao(10000000);




    Grafo *h = new Grafo();
    h->leEntrada(nome);

    printf("\n\nfinal: %f", 100*1000*h->funcaoObjetivo(melhor, erro_fObjetivo) );
}

void buscaLocal(){
    Grafo *g = new Grafo();
    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);

    for(int i=0; i<100000; i++){

        u_int id1 = rand() % g->nosEntrada.size();
        u_int id2 = rand() % g->nosEntrada.size();
        u_int id3 = rand() % g->nosEntrada.size();

//        cout << "id1: " << id1 << "   id2: " << id2 << "   id3: " << id3 << endl;

        Solucao solucao = g->buscaLocal(g->construtivoAleatorio(), id1, id2, id3);
        if(g->verificaSolucaoValida(solucao)==false)
            cout << "solucao invalida!" << endl;

//        g->imprimeSolucao(solucao);
    }

//    g->imprime();
//
//    for(u_int i=118; i<=132; i++){
//        Arco *a = g->buscaArcoID(i);
//        a->chave=false;
//    }
//
//    g->calcula_fluxos_e_perdas(1e-8);
//    cout << "\ntensao minima:" << g->tensaoMinima();
//    cout << "\nperdaTotal: " << 100*1000*g->soma_perdas()[0] << endl;
//
//    g->zeraFluxosPerdas();
//    g->calcula_fluxos_e_perdas(1e-8);
//    cout << "\ntensao minima:" << g->tensaoMinima();
//    cout << "\nperdaTotal: " << 100*1000*g->soma_perdas()[0] << endl;
//
//    g->zeraFluxosPerdas();
//    g->calcula_fluxos_e_perdas(1e-8);
//    cout << "\ntensao minima:" << g->tensaoMinima();
//    cout << "\nperdaTotal: " << 100*1000*g->soma_perdas()[0] << endl;
//
//    g->zeraFluxosPerdas();
//    g->calcula_fluxos_e_perdas(1e-8);
//    cout << "\ntensao minima:" << g->tensaoMinima();
//    cout << "\nperdaTotal: " << 100*1000*g->soma_perdas()[0] << endl;
//
//    g->zeraFluxosPerdas();
//    g->calcula_fluxos_e_perdas(1e-8);
//    cout << "\ntensao minima:" << g->tensaoMinima();
//    cout << "\nperdaTotal: " << 100*1000*g->soma_perdas()[0] << endl;
//
//
//
//
//
//
//    Solucao solucao;
//    solucao.vetChaves = new bool*[g->nosEntrada.size()];
//    for(int i=0; i<g->nosEntrada.size(); i++){
//        solucao.vetChaves[i] = new bool[g->nosEntrada.at(i)->volta.size()];
//        for(int j=0; j<g->nosEntrada.at(i)->volta.size(); j++)
//            solucao.vetChaves[i][j] = g->nosEntrada.at(i)->volta.at(j)->chave;
//    }
//    g->zeraFluxosPerdas();
//    g->calcula_fluxos_e_perdas(1e-8);
//    g->abreFechaChavesGrafo(solucao);
//    cout << "\n\n\n\ntensao minima:" << g->tensaoMinima();
//    cout << "\nperdaTotal: " << 100*1000*g->soma_perdas()[0] << endl;
//
////    g->zeraFluxosPerdas();
//    double objetivo = g->funcaoObjetivo(solucao, erro_fObjetivo);
//    cout << "\n\n\n\ntensao minima:" << g->tensaoMinima();
//    cout << "\nperdaTotal: " << 100*1000*objetivo << endl;
//
//
//    Grafo *h = new Grafo();
//    h->leEntrada(nome);
//
//    objetivo = h->funcaoObjetivo(solucao, erro_fObjetivo);
//    cout << "\n\n\n\ntensao minima:" << h->tensaoMinima();
//    cout << "\nperdaTotal: " << 100*1000*objetivo << endl;

//
//
//
//
//
//
//    Solucao solucao = g->construtivoAleatorio();
//
//    cout << "perda total: " << 100*1000*g->funcaoObjetivo(solucao, erro_fObjetivo) << endl;
//
////    g->zeraFluxosPerdas();
////    g->calcula_fluxos_e_perdas(erro_fObjetivo);
//    cout << "perda total: " << 100*1000*g->funcaoObjetivo(solucao, erro_fObjetivo) << endl;
//
////    g->zeraFluxosPerdas();
////    g->calcula_fluxos_e_perdas(erro_fObjetivo);
//    cout << "perda total: " << 100*1000*g->funcaoObjetivo(solucao, erro_fObjetivo) << endl;
//
//
//    Grafo *h = new Grafo();
//    h->leEntrada(nome);
//    h->abreFechaChavesGrafo(solucao);
//    h->calcula_fluxos_e_perdas(erro_fObjetivo);
//    cout << "perda total: " << 100*1000*h->soma_perdas()[0] << endl;
//
//    Grafo *l = new Grafo();
//    l->leEntrada(nome);
//    l->abreFechaChavesGrafo(solucao);
//    l->calcula_fluxos_e_perdas(erro_fObjetivo);
//    cout << "perda total: " << 100*1000*l->soma_perdas()[0] << endl;

//    cout << "fobj:" << 100*1000*solucao.valorObjetivo << endl;
//    cout << "fobj:" << 100*1000*g->funcaoObjetivo(solucao, erro_fObjetivo) << endl;
//    cout << "fobj:" << 100*1000*g->funcaoObjetivo(solucao, erro_fObjetivo) << endl;
//    cout << "fobj:" << 100*1000*g->funcaoObjetivo(solucao, erro_fObjetivo) << endl;

//    Solucao solucao = g->construtivoAleatorio();
//
//    cout << "fobj:" << 100*1000*solucao.valorObjetivo << endl;
//
//    Solucao melhor = g->buscaLocal(solucao, 0, 1, 2);
//
//    cout << "fobj:" << 100*1000*melhor.valorObjetivo << endl;

}

void testeConstrutivoKtruskal(){
    Grafo *g = new Grafo();

    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);

    Solucao solucao;

    for(u_int i=0; i<4; i++){
        solucao = g->CONSTRUTIVO(i);

        cout << "\nfuncao objetivo: " << 100*1000*solucao.valorObjetivo;
        cout << "\neh solucao? " << g->verificaSolucaoValida(solucao);
    }
}

void testeGenetico(){
    int num_exec = 10;

    ofstream saida;
    saida.open("saida.txt");

    Grafo *g = new Grafo();

    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);

    Solucao solucao;

    saida << "(100it, popAle, cruz_metade, taxa_mut 5)\nx=c(";
    for(int i=0; i<num_exec; i++){
        solucao = g->algoritmoGenetico(100, 0, 0, 5);

        if(g->verificaSolucaoValida(solucao)==true)
            saida << 100*1000*solucao.valorObjetivo;
        else
            saida << "solucao invalida";
        if(i<10-1)
            saida << ",";

    }
    saida << "\n\n";


    cout << "\n\n" << endl;

    saida << "(100it, popBuscaLocal, cruz_metade, taxa_mut 5)\nx=c(";
    for(int i=0; i<num_exec; i++){
        solucao = g->algoritmoGenetico(100, 1, 0, 5);

        if(g->verificaSolucaoValida(solucao)==true)
            saida << 100*1000*solucao.valorObjetivo;
        else
            saida << "solucao invalida";
        if(i<10-1)
            saida << ",";

    }
    saida << ")\n\n";


    saida << "(100it, popBuscaLocal, cruz_aleatorio, taxa_mut 5)\nx=c(";
    for(int i=0; i<num_exec; i++){
        solucao = g->algoritmoGenetico(100, 1, 1, 5);

        if(g->verificaSolucaoValida(solucao)==true)
            saida << 100*1000*solucao.valorObjetivo;
        else
            saida << "solucao invalida";
        if(i<10-1)
            saida << ",";

    }
    saida << ")\n\n";


    saida << "(1000it, popBuscaLocal, cruz_aleatorio, taxa_mut 5)\nx=c(";
    for(int i=0; i<num_exec; i++){
        solucao = g->algoritmoGenetico(100, 1, 1, 5);

        if(g->verificaSolucaoValida(solucao)==true)
            saida << 100*1000*solucao.valorObjetivo;
        else
            saida << "solucao invalida";
        if(i<10-1)
            saida << ",";

    }
    saida << ")\n\n";


    saida << "alternado(1000it, popBuscaLocal, 5ciclos, taxa_mut 5)\nx=c(";
    for(int i=0; i<num_exec; i++){
        solucao = g->algoritmoGeneticoAdaptativo(50, 1, 5, 5);

        if(g->verificaSolucaoValida(solucao)==true)
            saida << 100*1000*solucao.valorObjetivo;
        else
            saida << "solucao invalida";
        if(i<10-1)
            saida << ",";

    }
    saida << ")\n\n";

//
//
//
//    saida << "(100it, popAle, cruz_metade, taxa_mut 50)\nx=c(";
//    for(int i=0; i<num_exec; i++){
//        solucao = g->algoritmoGenetico(100, 0, 0, 50);
//
//        if(g->verificaSolucaoValida(solucao)==true)
//            saida << 100*1000*solucao.valorObjetivo;
//        else
//            saida << "solucao invalida";
//        if(i<10-1)
//            saida << ",";
//
//    }
//    saida << ")\n\n";
//
//
//
//    saida << "(100it, popAle, cruz_metade, taxa_mut 100)\nx=c(";
//    for(int i=0; i<num_exec; i++){
//        solucao = g->algoritmoGenetico(100, 0, 0, 100);
//
//        if(g->verificaSolucaoValida(solucao)==true)
//            saida << 100*1000*solucao.valorObjetivo;
//        else
//            saida << "solucao invalida";
//        if(i<10-1)
//            saida << ",";
//
//    }
//    saida << ")\n\n";
//
//
//
//    saida << "(100it, popAle, cruz_suave, taxa_mut 5)\nx=c(";
//    for(int i=0; i<num_exec; i++){
//        solucao = g->algoritmoGenetico(100, 0, 3, 5);
//
//        if(g->verificaSolucaoValida(solucao)==true)
//            saida << 100*1000*solucao.valorObjetivo;
//        else
//            saida << "solucao invalida";
//        if(i<10-1)
//            saida << ",";
//
//    }
//    saida << ")\n\n";
//
//
//
//    saida << "(100it, popAle, cruz_suave, taxa_mut 50)\nx=c(";
//    for(int i=0; i<num_exec; i++){
//        solucao = g->algoritmoGenetico(100, 0, 3, 50);
//
//        if(g->verificaSolucaoValida(solucao)==true)
//            saida << 100*1000*solucao.valorObjetivo;
//        else
//            saida << "solucao invalida";
//        if(i<10-1)
//            saida << ",";
//
//    }
//    saida << ")\n\n";
//
//
//    saida << "(100it, popAle, cruz_suave, taxa_mut 100)\nx=c(";
//    for(int i=0; i<num_exec; i++){
//        solucao = g->algoritmoGenetico(100, 0, 3, 100);
//
//        if(g->verificaSolucaoValida(solucao)==true)
//            saida << 100*1000*solucao.valorObjetivo;
//        else
//            saida << "solucao invalida";
//        if(i<10-1)
//            saida << ",";
//
//    }
//    saida << ")\n\n";
//
//
//    saida << "Adpt(50it, popAle, ciclo2, taxa_mut 5)\nx=c(";
//    for(int i=0; i<num_exec; i++){
//        solucao = g->algoritmoGeneticoAdaptativo(50, 0, 2, 5);
//
//        if(g->verificaSolucaoValida(solucao)==true)
//            saida << 100*1000*solucao.valorObjetivo;
//        else
//            saida << "solucao invalida";
//        if(i<10-1)
//            saida << ",";
//
//    }
//    saida << ")\n\n";
//
//
//    saida << "Adpt(50it, popAle, ciclo2, taxa_mut 50)\nx=c(";
//    for(int i=0; i<num_exec; i++){
//        solucao = g->algoritmoGeneticoAdaptativo(50, 0, 2, 50);
//
//        if(g->verificaSolucaoValida(solucao)==true)
//            saida << 100*1000*solucao.valorObjetivo;
//        else
//            saida << "solucao invalida";
//        if(i<10-1)
//            saida << ",";
//
//    }
//    saida << ")\n\n";
//
//
//    saida << "Adpt(50it, popAle, ciclo2, taxa_mut 100)\nx=c(";
//    for(int i=0; i<num_exec; i++){
//        solucao = g->algoritmoGeneticoAdaptativo(50, 0, 2, 100);
//
//        if(g->verificaSolucaoValida(solucao)==true)
//            saida << 100*1000*solucao.valorObjetivo;
//        else
//            saida << "solucao invalida";
//        if(i<10-1)
//            saida << ",";
//
//    }
//    saida << ")\n\n";
//


    saida.close();
}

void testeConstrutivoAleatorio(){
    Grafo *g = new Grafo();

    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);


    Solucao solucao;
    solucao.vetChaves = new bool*[g->nosEntrada.size()];
    for(u_int i=0; i<g->nosEntrada.size(); i++)
        solucao.vetChaves[i] = new bool[g->nosEntrada.at(i)->volta.size()];
    for(u_int i=0; i<g->nosEntrada.size(); i++){
        for(u_int j=0; j<g->nosEntrada.at(i)->volta.size(); j++){
            solucao.vetChaves[i][j] = true;
        }
    }

    cout << "\nentrada e solucao valida:" << g->verificaSolucaoValida(solucao) << endl;

    solucao = g->construtivoAleatorio();


    cout << "\n\n\nfuncao objetivo: " << solucao.valorObjetivo << endl;

    g->desmarcaNos();

    cout << "\nsolucao e valida:" << g->verificaSolucaoValida(solucao) << endl;

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


    Solucao solucao;
    solucao.vetChaves = new bool*[g->numeroNos];
    for(u_int i=0; i<g->numeroNos; i++){
        solucao.vetChaves[i] = new bool[1];
        solucao.vetChaves[i][0] = true;
    }

    cout << "\nfuncao objetivo:" << g->funcaoObjetivo(solucao, 1) << endl;

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

    char nome[] = "sist14barras.m";
    g->leEntrada(nome);

    g->calcula_fluxos_e_perdas(1e-6);

/*
    g->foward(0);
    g->backward();
    g->imprime();

    g->foward(1);*/
//    g->backward();


    g->imprime();

//    cout << "\nperdas: " << g->soma_perdas()[0] << endl;
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

void teste(){

    Grafo *g = new Grafo();

    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);

    for(u_int i=118; i<=132; i++){
        Arco *a = g->buscaArcoID(i);
        a->chave=false;
    }


//    g->buscaArcoID(24-1)->chave=false;
//    g->buscaArcoID(27-1)->chave=false;
//    g->buscaArcoID(35-1)->chave=false;
//    g->buscaArcoID(40-1)->chave=false;
//    g->buscaArcoID(43-1)->chave=false;
//    g->buscaArcoID(52-1)->chave=false;
//    g->buscaArcoID(59-1)->chave=false;
//    g->buscaArcoID(72-1)->chave=false;
//    g->buscaArcoID(75-1)->chave=false;
//    g->buscaArcoID(96-1)->chave=false;
//    g->buscaArcoID(99-1)->chave=false;
//    g->buscaArcoID(110-1)->chave=false;
//    g->buscaArcoID(123-1)->chave=false;
//    g->buscaArcoID(130-1)->chave=false;
//    g->buscaArcoID(131-1)->chave=false;


//    g->buscaArcoID(24-1)->chave=false;
//    g->buscaArcoID(26-1)->chave=false;
//    g->buscaArcoID(35-1)->chave=false;
//    g->buscaArcoID(40-1)->chave=false;
//    g->buscaArcoID(43-1)->chave=false;
//    g->buscaArcoID(51-1)->chave=false;
//    g->buscaArcoID(61-1)->chave=false;
//    g->buscaArcoID(72-1)->chave=false;
//    g->buscaArcoID(75-1)->chave=false;
//    g->buscaArcoID(96-1)->chave=false;
//    g->buscaArcoID(98-1)->chave=false;
//    g->buscaArcoID(110-1)->chave=false;
//    g->buscaArcoID(122-1)->chave=false;
//    g->buscaArcoID(130-1)->chave=false;
//    g->buscaArcoID(131-1)->chave=false;


    g->calcula_fluxos_e_perdas(1e-8);
    cout << "\ntensao minima:" << g->tensaoMinima();
    cout << "\nperdaTotal: " << 100*1000*g->soma_perdas()[0] << endl;


    Solucao solucao;
    solucao.vetChaves = new bool*[g->nosEntrada.size()];
    for(u_int i=0; i<g->nosEntrada.size(); i++)
        solucao.vetChaves[i] = new bool[g->nosEntrada.at(i)->volta.size()];

    for(u_int i=0; i<g->nosEntrada.size(); i++){
        for(u_int j=0; j<g->nosEntrada.at(i)->volta.size(); j++){
            solucao.vetChaves[i][j] = g->nosEntrada.at(i)->volta.at(j)->chave;
        }
    }
    cout << "\n\n Eh solucao: " << g->verificaSolucaoValida(solucao);

/*
    Grafo *g = new Grafo();

    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);

    for(u_int i=118; i<=132; i++){
        Arco *a = g->buscaArcoID(i);
        a->chave=false;
//        printf("\n[%d]a( %d , %d )", a->chave, a->noOrigem->id, a->noDestino->id);
    }

    g->calcula_fluxos_e_perdas(1e-8);

//    u_int n_arcos = 0;
//    for(No *no=g->listaNos; no!=NULL; no=no->proxNo){
//        for(Arco *a=no->listaArcos; a!=NULL; a=a->proxArco){
//            if(a->chave==true)
//                n_arcos++;
//        }
//    }

    cout << "\n\nresultado rede com chaves fechadas(pu):" << g->soma_perdas()[0];

    cout << "\ntensao minima(pu): " << g->tensaoMinima() << endl;

//    g->imprime();

//    cout << "\nresultado rede com chaves fechadas(MW):" << ((n_arcos*(g->pb*g->pb)/(g->fator*g->vb*g->vb))*g->soma_perdas()[0]) << endl;
//    cout << "\nresultado rede com chaves fechadas(KW):" << 1000*((n_arcos*(g->pb*g->pb)/(g->fator*g->vb*g->vb))*g->soma_perdas()[0]) << endl;
//    g->imprime();







    Grafo *h = new Grafo();
    char nome2[] = "SISTEMA119s2 - base.m";
    h->leEntrada(nome2);

    h->calcula_fluxos_e_perdas(1e-8);

    cout << "\n\nresultado entrada arvore base:" << h->soma_perdas()[0];
    cout << "\ntensao minima(pu): " << h->tensaoMinima() << endl;


//    cout << "\n\nadress: " << h->buscaArco(8, 9) << endl;
//    printf("main(8,9) - fluxo: (%f , %f)", h->buscaArco(8, 9)->fluxoP_ativ, h->buscaArco(8, 9)->fluxoP_reativ);
*/
    cout << "\n\n\n";
}

void testeMutacao(){
    Grafo *g = new Grafo();

    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);

    Solucao solucao = g->construtivoAleatorio();
    cout  << "individuo original:\n";
    g->imprimeChaves(solucao);


    for(int i=0; i<10; i++){
        cout << "apos mutacao:\n";
        g->mutacao(solucao, 50);
//        g->imprimeChaves(solucao);
        cout << "objetivo: " << solucao.valorObjetivo << endl;
    }

}

void testeCruzamento(){
    Grafo *g = new Grafo();

    char nome[] = "SISTEMA119s2.m";
    g->leEntrada(nome);

    Solucao pai1 = g->construtivoAleatorio();
    Solucao pai2 = g->construtivoAleatorio();

    cout << "pai1:\n";
    g->imprimeChaves(pai1);
    cout << "pai2:\n";
    g->imprimeChaves(pai2);
    cout << "filho:\n";
    Solucao filho = g->cruzamento_metade(pai1, pai2);
    g->imprimeChaves(filho);

    cout << "\npai1: " << pai1.valorObjetivo;
    cout << "\npai2: " << pai2.valorObjetivo;
    cout << "\nfilho: " <<filho.valorObjetivo;



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
