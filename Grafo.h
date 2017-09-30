#ifndef DIGRAFO_H_INCLUDED
#define DIGRAFO_H_INCLUDED
#include "No.h"
#include "Arco.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;
typedef u_int u_int;
struct Dijkstra;

class Grafo
{
public:
    No *listaNos;
    u_int grau;///maior grau de vértice do grafo
    u_int numeroNos;///n
    u_int numeroArcos;///m
    u_int contAux;/// contador auxiliar
    bool flagDir;///flag indicando se o grafo é direcionado ou não
    int auxEhNoArticulacao(No* no);
    void auxBuscaProfundidade(No *no, No* noArv, Grafo* Arv);
//    void incrementaContador(No* n);
    void atualizaDist(No* u, map<u_int, No*> Q, map<u_int, double> distancias);
    Grafo();
    No *getListaNos(){return this->listaNos;};
    u_int getGrau(){     return grau;    };
    u_int getNumeroNos(){    return numeroNos;   };
    u_int getNumeroArcos(){    return numeroArcos;   };
    bool getFlagDir(){  return flagDir; };
    void setFlagDir(bool flag){ flagDir=flag;   };
    u_int getContAux(){  return contAux; };
    void setContAux(u_int i){ contAux=i;   };

    void leDataArquivo(char nome[]);

    No* buscaNo(u_int id);
    No* insereNo(u_int id);
    void removeNo(u_int id);

    void imprime();

    void atualizaGrau();
    void atualizaGrausEntradaSaidaDosNos();
    void desmarcaNos();
    void leArquivo(char nome[]);

    void insereArco(No* noOrigem, No* noDestino, u_int id, bool atualizarGrau = true);
    void insereArcoID(u_int idOrigem, u_int idDestino, u_int id);

    void removeArco(u_int idOrigem, u_int idDestino);
    void removeArco(No* noOrigem, No* noDestino, bool atualizarGrau = true);
    void removeArcos(No *no, bool atualizarGrau);
    void removeArcosLigadasAoNo(No *no, bool atualizarGrau);

    bool ehGrafoCompleto();
    bool ehGrafoKRegular(u_int k);
    bool mesmaComponenteFortementeConexa(u_int id1, u_int id2);
    bool mesmaComponenteFortementeConexa(No *i1, No *i2);

    Grafo *subGrafoInduzido(u_int E[], u_int tam);

    u_int* sequenciaGrau();

    bool saoAdjacentes(u_int id1, u_int id2);
    bool saoAdjacentes(No *no1, No *no2);

    u_int numeroNosComponenteFortementeConexa(No *no);
    bool ehNoArticulacao(u_int id);
    bool ehNoArticulacao(No* no);

    bool ehArcoPonte(u_int id);
    bool ehArcoPonte(Arco* arco);

    bool ehFortementeConexo();
    u_int rubustezVertice(u_int *ids);
    u_int rubustezAresta();

    void percursoProfundidade(No *inicio);
    Grafo* buscaProfundidade(No *no);
    Grafo* BuscaEmLargura(No *no);

    vector<No*> vizinhancaAberta(u_int id, bool fechada = false);
    vector<No*> vizinhancaFechada(u_int id, bool fechada = true);
    bool ehGrafoKRegular();

    Grafo* clone();

    vector<No*> ordenacaoTopologicaDAG();

    int numeroComponentesConexas();

    Arco* buscaArco(u_int id1, u_int id2);
    Arco* buscaArco(No* no1, No* no2);

    double** algoritmoFloyd();
    double consultaMenorCaminhoEntreDoisNos(u_int i, u_int j);

    vector<Arco*> algorimoPrim();

    /**
    Retorna o subgrafo (ou floresta de subgrafos se o grafo nao e não-conexo)
    contendo as arestas de fluxo minimo que ligam todos os nos do grafo
    que formam a arvore/floresta
    */
    vector<Arco*> Kruskal();




    ///funcoes trabalho IC:
    u_int n_marcados;/// contagem percurso em profundidade
    No *insereNoCarga(u_int id, double carga, double potencia_reativa);
    void insereArcoResistencia(u_int idOrig, u_int idDest, u_int ID, double resistencia, double fluxo);
    void leEntrada();
    ~Grafo();
};

#endif // DIGRAFO_H_INCLUDED
