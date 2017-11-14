#include "Grafo.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>
#include <math.h>
#include <map>
#include <set>
#define INFINITO HUGE_VAL

using namespace std;

Grafo::Grafo(){
    listaNos = NULL;
    grau = 0;
    numeroArcos = 0;
    numeroNos = 0;
    flagDir = true;
    perdaTotal = 0.0;
    p_ativ_total = 0.0;
    p_reat_total = 0.0;
    pb =vb =zb =0.0;
}

///Função que insere nó no inicio(cabeça) do grafo
No *Grafo::insereNo(u_int id){
    No *no=new No(id);
    no->setProxNo(listaNos);
    listaNos=no;  ///nao deveria ser um setListaDeNos(this->listaNos)???
    this->numeroNos++;///atualiza numero de vertices(nos)
    return no;
}

///potencia ativa=carga
No *Grafo::insereNoCargaVoltagem(u_int id, double carga, double potencia_reativa, double voltagem){
    No *no=new No(id);
    no->carga = carga;
    ///potencia ativa=carga
    no->potencia_ativa = carga;
    no->potencia_reativa = potencia_reativa;
    no->voltagem = voltagem;
    no->setProxNo(listaNos);
    listaNos=no;  ///nao deveria ser um setListaDeNos(this->listaNos)???
    this->numeroNos++;///atualiza numero de vertices(nos)
    this->p_ativ_total+=carga;
    this->p_reat_total+=potencia_reativa;
    return no;
}

No *Grafo::buscaNo(u_int id){
    No *no = listaNos;
    while(no!=NULL){
        if(no->getID() == id)
            return no;
        no = no->getProxNo();
    }
    return 0;
}

void Grafo::insereArco(No* noOrigem, No* noDestino, u_int id, bool atualizarGrau){
    noOrigem->insereArco(noDestino, id);
    this->numeroArcos++;
    if (atualizarGrau)
        this->atualizaGrau();
}

/**
Função que insere arco na estrutura:

faz a busca do no entrada(idOrigem) do arco no grafo [arco:(idOrigem----->praOnde)]
cria o arco que sera inserido como arco desse no encontrado na busca
define para onde(saida) no arco com o no idDestino
arco inserido com sucesso
**/
void Grafo::insereArcoID(u_int idOrigem, u_int idDestino, u_int id){
    No *noOrigem = buscaNo(idOrigem);
    No *noDestino = buscaNo(idDestino);
    this->insereArco(noOrigem, noDestino, id, true);
}

void Grafo::insereArcoDados(u_int idOrigem, u_int idDestino, u_int id, double resistencia, double reatancia, double fluxoP_ativ, double fluxoP_reativ, bool chave){
    No *noOrigem = buscaNo(idOrigem);
    No *noDestino = buscaNo(idDestino);

    Arco *novaArco = new Arco(id);
    novaArco->setNoDestino(noDestino);
    novaArco->setNoOrigem(noOrigem);
    novaArco->setProxArco(noOrigem->listaArcos);

    novaArco->resistencia = resistencia;
    novaArco->reatancia = reatancia;
    novaArco->fluxoP_ativ = fluxoP_ativ;
    novaArco->fluxoP_reativ = fluxoP_reativ;

    if(chave)
        novaArco->chave = true;

    noOrigem->setListaArcos(novaArco);
    noDestino->volta.push_back(novaArco);
    noDestino->grauEntrada++;

    this->numeroArcos++;


    if(chave){
        noOrigem->grau++;
        noOrigem->grauSaida++;
        this->atualizaGrau();
    }
}

/** desmarcar os nos do grafo */
void Grafo::desmarcaNos(){
    for(No *i = listaNos; i != NULL; i = i->getProxNo())
        i->setMarcado(false);
    n_marcados = 0;
}

bool Grafo::mesmaComponenteFortementeConexa(u_int id1, u_int id2){
    No *i1 = buscaNo(id1), *i2 = buscaNo(id2);
    if(i1!=NULL && i2!=NULL)
        return mesmaComponenteFortementeConexa(i1, i2);
    else
        return false;
}

bool Grafo::mesmaComponenteFortementeConexa(No *i1, No *i2){
    ///Se existe caminho de i1 pra i2
    this->desmarcaNos();
    percursoProfundidade(i1);
    bool result1=i2->getMarcado();

    ///Se existe caminho de i2 pra i1
    this->desmarcaNos();
    percursoProfundidade(i2);
    bool result2=i1->getMarcado();

    this->desmarcaNos();
    return result1 && result2;
}

/**
Percorre grafo a partir de um nó e sai marcando todo mundo da mesma componente conexa
*/
//void Grafo::percursoProfundidade(No *no,  void(Grafo::*funcao)(No*)){
void Grafo::percursoProfundidade(No *no){
    if(no == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        if(no->getMarcado() == false){
//            cout<<"marcando: "<<no->getID()<<endl;
            no->setMarcado(true);
            n_marcados++;
            this->contAux++;
//            if (funcao != NULL)
//                (this->*funcao)(no);
            for(Arco *a=no->getListaArcos(); a!=NULL; a=a->getProxArco()){

                ///nao descer por arcos com chave aberta
                while(a!=NULL && a->chave == false){
                    //printf("A%d\n", a->getID());
                    a = a->getProxArco();
                }

                if(a==NULL)
                    break;
                ///---------------------------------

                cout << "( " << a->getNoOrigem()->getID() << " , " << a->getNoDestino()->getID() << " )" << endl;

                percursoProfundidade(a->getNoDestino());
            }
        }
    }
}

void Grafo::AtualizaFLuxos(){
    desmarcaNos();
    AuxAtualizaFLuxos(this->getListaNos());
}

void Grafo::AuxAtualizaFLuxos(No *no){
    //printf("\n\nno:%d\n\n", no->getID());
    if(no == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        if(no->getMarcado() == false){
//            cout<<"marcando: "<<no->getID()<<endl;
            no->setMarcado(true);
            n_marcados++;
            this->contAux++;
//            if (funcao != NULL)
//                (this->*funcao)(no);
            for(Arco *a=no->getListaArcos(); a!=NULL; a=a->getProxArco()){

                ///nao descer por arcos com chave aberta
                while(a!=NULL && a->chave == false){
                    //printf("A%d\n", a->getID());
                    a->fluxoP_ativ = a->fluxoP_reativ = a->perda_ativ = 0.0;
                    a = a->getProxArco();
                }

                if(a==NULL)
                    break;
                ///---------------------------------

                ///chamada recursiva
                AuxAtualizaFLuxos(a->getNoDestino());

                calculaPerda(a);
            }
        }
    }
}

void calculaPerda(Arco *a){
    //printf("A:%d\n", a->getID());
    No *no_dest = a->getNoDestino();
    a->fluxoP_ativ = 0.0;
    a->fluxoP_reativ = 0.0;

    if(no_dest->getGrau()>0){
        for(Arco *aNext=no_dest->getListaArcos(); aNext!=NULL; aNext=aNext->getProxArco()){
            ///o calculo do V nao entendi
            double Vi = 100.0;

            double Pi_ = aNext->fluxoP_ativ + no_dest->carga;
            double Qi_ = aNext->fluxoP_reativ + no_dest->potencia_reativa;

            if(aNext->chave == false){
                a->fluxoP_ativ = no_dest->carga;
                a->fluxoP_reativ = no_dest->potencia_reativa;
                a->perda_ativ = a->resistencia*pow(a->fluxoP_ativ, 2);
            }else{
                a->fluxoP_ativ += aNext->fluxoP_ativ + (aNext->resistencia*( pow(Pi_, 2) + pow(Qi_, 2) )/pow(Vi,2)) + no_dest->carga;
                a->fluxoP_reativ += aNext->fluxoP_reativ + (aNext->reatancia*( pow(Pi_, 2) + pow(Qi_, 2) )/pow(Vi,2)) + no_dest->potencia_reativa;
            }
        }
        a->perda_ativ = a->resistencia*pow(a->fluxoP_ativ, 2);
    }
    else{
        a->fluxoP_ativ = no_dest->carga;
        a->fluxoP_reativ = no_dest->potencia_reativa;
        a->perda_ativ = a->resistencia*pow(a->fluxoP_ativ, 2);
    }
}

bool menorPerda(Arco *a1, Arco *a2){calculaPerda(a1); calculaPerda(a2);return a1->perda_ativ < a2->perda_ativ;};

void Grafo::construtivo(){
    printf("\n\n-----------------CONSTRUTIVO---------------\n\n");
    desmarcaNos();
    AuxConstrutivo(this->listaNos);
    printf("\n\n----------------------------------------------\n\n");
}

void Grafo::AuxConstrutivo(No *no){
    //printf("\n\nno:%d\n\n", no->getID());
    if(no == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        if(no->getMarcado() == false){
//            cout<<"marcando: "<<no->getID()<<endl;
            n_marcados++;
            this->contAux++;
//            if (funcao != NULL)
//                (this->*funcao)(no);
            for(Arco *a=no->getListaArcos(); a!=NULL; a=a->getProxArco()){

                    ///nao descer por arcos com chave aberta
                    while(a!=NULL && a->chave == false){
                        //printf("A%d\n", a->getID());
                        a = a->getProxArco();
                    }

                    if(a==NULL)
                        break;
                    ///---------------------------------


                    ///chamada recursiva
                    AuxConstrutivo(a->getNoDestino());

                    ///calcula a perda_ativ no arco 'a' da volta recursiva
                    calculaPerda(a);


                    No *no_dest = a->getNoDestino();
                    ///ordenar por menor perda_ativ os arcos que incidem no no
                    sort(no_dest->volta.begin(), no_dest->volta.end(), menorPerda);
                    ///uso o arco que teve menor perda_ativ
                    no_dest->volta.at(0)->chave = true;
                    ///demais arcos nao usa
                    for(u_int i=1; i<no_dest->volta.size(); i++){

                        no_dest->volta.at(i)->chave = false;

                        no_dest->volta.at(i)->fluxoP_ativ = 0.0;
                        no_dest->volta.at(i)->fluxoP_reativ = 0.0;
                        no_dest->volta.at(i)->perda_ativ = 0.0;

                        no_dest->volta.at(i)->noOrigem->grau--;

                        ///print dos arcos que foram abertos pela escolha do guloso
                        printf("\nabriu A:%d (%d, %d)\n", no_dest->volta.at(i)->getID(),
                        no_dest->volta.at(i)->getNoOrigem()->getID(),
                        no_dest->volta.at(i)->getNoDestino()->getID());
                    }
            }
            no->setMarcado(true);
        }
    }
}

//void Grafo::foward(u_int it){
//    cout << "\tFOWARD" << endl;
//    this->listaNos->voltagem = 1.0;///voltagem controlada na estacao
//    Auxfoward(this->listaNos, this->listaNos->listaArcos, it);
//}
//
//void Grafo::Auxfoward(No *no, Arco *ak, u_int it){
//    if(no == NULL)
//        cout<<"\n No NULL \n"<<endl;
//    else{
//        for(Arco *a=no->getListaArcos(); a!=NULL; a=a->getProxArco()){
//
//            ///nao descer por arcos com chave aberta
//            while(a!=NULL && a->chave == false){
//                //printf("A%d\n", a->getID());
//                a = a->getProxArco();
//            }
//
//            if(a==NULL)
//                break;
//
//            ///----foward----
//
//            ///chute inicial para o fluxo nas primeiras
//            if(no==listaNos){
//                a->fluxoP_ativ = this->p_ativ_total;
//                a->fluxoP_reativ = this->p_reat_total;
//                if(it>0){///temos perdas ja calculadas
//                    double *sum_perdas = soma_perdas();
//                    a->fluxoP_ativ += sum_perdas[0];
//                    a->fluxoP_reativ += sum_perdas[1];
//                }
//            }else{
//
//                double perda_ativ = 0.0;
//                double perda_reat = 0.0;
//                if(it>0){
//                    perda_ativ = ak->resistencia*(pow(ak->fluxoP_ativ, 2) + pow(ak->fluxoP_reativ, 2)) / pow(no->voltagem, 2);
//                    perda_reat = ak->reatancia*(pow(ak->fluxoP_ativ, 2) + pow(ak->fluxoP_reativ, 2)) / pow(no->voltagem, 2);
//                }
//
//                a->fluxoP_ativ   = ak->fluxoP_ativ - perda_ativ - no->carga;
//                a->fluxoP_reativ   = ak->fluxoP_reativ - perda_reat - no->potencia_reativa;
//            }
//
//            ///--------------
//
//
//            Auxfoward(a->getNoDestino(), a, it);
//        }
//    }
//}

double *Grafo::soma_perdas(){
    double *perda = new double[2]; perda[0] = perda[1] = 0.0;
    for(No *no=listaNos; no!=NULL; no = no->getProxNo()){
        for(Arco *a = no->listaArcos; a!=NULL; a = a->getProxArco()){
            perda[0] += a->perda_ativ;
            perda[1] += a->perda_reat;
        }
    }
    return perda;
}


//void Grafo::backward(u_int it){
//    cout << "\tBACKWARD" << endl;
//    this->listaNos->voltagem = 1.0;///voltagem controlada na estacao
//    Auxbackward(this->listaNos, this->listaNos->listaArcos, it);
//}
//
//void Grafo::Auxbackward(No *no, Arco *ak, u_int it){
//    if(no == NULL)
//        cout<<"\n No NULL \n"<<endl;
//    else{
//        for(Arco *a=no->getListaArcos(); a!=NULL; a=a->getProxArco()){
//
//            ///nao descer por arcos com chave aberta
//            while(a!=NULL && a->chave == false){
//                //printf("A%d\n", a->getID());
//                a = a->getProxArco();
//            }
//
//            if(a==NULL)
//                break;
//
//            ///----backward----
//
//            No *noDest = a->getNoDestino();
//            No *noOrig = a->getNoOrigem();
//
//            noDest->voltagem = pow(noOrig->voltagem, 2)
//            - 2*(a->resistencia*a->fluxoP_ativ + a->reatancia*a->fluxoP_reativ) +
//            (pow(a->resistencia, 2) + pow(a->reatancia, 2))*
//            (pow(a->fluxoP_ativ, 2) + pow(a->fluxoP_reativ, 2))/pow(noOrig->voltagem, 2);
//
//            a->perda_ativ = a->resistencia*(pow(a->fluxoP_ativ, 2) + pow(a->fluxoP_reativ, 2))
//            /pow(noOrig->voltagem, 2);
//            a->perda_reat = a->reatancia*(pow(a->fluxoP_ativ, 2) + pow(a->fluxoP_reativ, 2))
//            /pow(noOrig->voltagem, 2);
//
////            cout << "\nperda ativa: " << a->resistencia*(pow(a->fluxoP_ativ, 2) + pow(a->fluxoP_reativ, 2))
////            /pow(noOrig->voltagem, 2);
//
//            ///--------------
//
//            Auxbackward(a->getNoDestino(), a, it);
//        }
//    }
//}
//
void Grafo::calcula_fluxos_e_perdas(double tol){

    double *perdas_total, erro = 1.0;

    for(u_int it=0; erro>tol; it++){

        perdas_total = this->soma_perdas();

        ///calcula fluxos de potencia nas linhas
        foward(it);

        ///calcula voltagem nas barras e perdas nas linhas
        backward();

//        cout << "\n\nperda total da rede: (" << this->soma_perdas()[0] << " , " << this->soma_perdas()[1] << ")" << endl;
//        cout << "DIFERENCA ENTRE PERDA ATUAL E ANTERIOR CALCULADA: ( " << this->soma_perdas()[0] - perdas_total[0]
//        << " , " << this->soma_perdas()[1] - perdas_total[1] << " )\n";

        erro = this->soma_perdas()[0] - perdas_total[0];

//        cout << "\nperda total da rede:" << this->soma_perdas() << endl;
    }
}

/**
 * Auxiliar pata buscaProfundidade
 */
void Grafo::auxBuscaProfundidade(No* noGrafo, No* noArv, Grafo* Arv){
    noGrafo->setMarcado(true);

    /// percorre adjacencias do (noGrafo)
    for(Arco *a = noGrafo->getListaArcos(); a != NULL; a = a->getProxArco()){
        No* proxNo = a->getNoDestino();
        if (!proxNo->getMarcado()){
            ///Insere proximo (no) na arvore
            No* novoNoArv = Arv->insereNo(proxNo->getID());
            Arv->insereArco(noArv, novoNoArv, a->getID());
            novoNoArv->setNivel(noArv->getNivel()+1);

            auxBuscaProfundidade(proxNo, novoNoArv, Arv);
        }
    }
}

/**
 * Constroi a arvore (Arv) a partir da busca em profundidade em (noGrafo)
 */
Grafo* Grafo::buscaProfundidade(No *no){
    if(no == NULL)
        return NULL;

    this->desmarcaNos();
    Grafo* Arv = new Grafo();

    ///raiz da arvore
    No* raiz = Arv->insereNo(no->getID());
    raiz->setNivel(0);
    auxBuscaProfundidade(no, raiz, Arv);

    return Arv;
}

/**
 * Constroi a arvore (Arv) a partir da busca em largura em (noGrafo)
 */
Grafo* Grafo::BuscaEmLargura(No *noGrafo){
    this->desmarcaNos();
    queue<No*> fila;

    Grafo* Arv = new Grafo();
    ///raiz da arvore (Arv)
    No* noArv = Arv->insereNo(noGrafo->getID());
    noArv->setNivel(0);

    noGrafo->setMarcado(true);
    fila.push(noGrafo);

    ///enquanto a fila nao esta vazia
    while(!fila.empty()){
        ///vertice que esta no inicio da fila
        noArv = Arv->buscaNo(fila.front()->getID());

        /// percorre adjacencias do (noGrafo)
        for(Arco *a = fila.front()->getListaArcos(); a != NULL; a = a->getProxArco()){//enquanto a lista de adjacencia do vertice1 nao acaba
            No* proxNo = a->getNoDestino();
            if(!proxNo->getMarcado()){
                ///Marca vertice e o poe na fila
                proxNo->setMarcado(true);
                fila.push(proxNo);

                ///Insere proximo (no) na arvore
                No* novoNo = Arv->insereNo(proxNo->getID());
                Arv->insereArco(noArv, novoNo, a->getID());
                novoNo->setNivel(noArv->getNivel()+1);
            }
        }
        ///retira vertice da fila
        fila.pop();
    }

    return Arv;
}

Grafo *Grafo::subGrafoInduzido(u_int E[], u_int tam){
    Grafo *induzido=new Grafo();
    for(u_int i=0; i<tam; i++)
        induzido->insereNo(E[i]);
    No *no;
    for(u_int i=0; i<tam; i++){
        no=this->buscaNo(E[i]);
        ///procura Arcos do grafo que ligam os vertices do grafo induzido
        for(Arco *a=no->getListaArcos(); a!=NULL; a=a->getProxArco()){
            for(u_int j=0; j<tam; j++){
//                if(a->getNoDestino() == this->buscaNo(E[j]))
                if(a->getNoDestino()->getID() == E[j])
                    induzido->insereArcoID(no->getID(), a->getNoDestino()->getID(), a->getID());
            }
        }
    }
    return induzido;
}

bool Grafo::ehGrafoKRegular(u_int k){
    for(No *i=listaNos; i!=NULL; i=i->getProxNo()){
        if(i->getGrau()!=k)
            return false;
    }
    return true;
}

///Funcao Iago
bool Grafo::ehGrafoKRegular(){
    No* inicio = this->listaNos;
    return ehGrafoKRegular(inicio->getGrau());
}

bool Grafo::ehGrafoCompleto(){
    return ehGrafoKRegular(this->numeroNos-1);
}

void Grafo::removeArco(No* noOrigem, No* noDestino, bool atualizarGrau){
    if(noOrigem!=NULL && noDestino != NULL && noOrigem->getListaArcos()!=NULL){
        Arco* arcoRemover = NULL;
        ///se primeiro arco sera removido
        if(noOrigem->getListaArcos()->getNoDestino() == noDestino){
            arcoRemover = noOrigem->getListaArcos();
            noOrigem->setListaArcos(arcoRemover->getProxArco());
        }else{
            Arco *anterior= noOrigem->getListaArcos();
            while(anterior->getProxArco() != NULL &&
                  anterior->getProxArco()->getNoDestino() != noDestino)
                anterior=anterior->getProxArco();

            /// arco existe no no
            if(anterior->getProxArco()!=NULL){
                Arco *sucessor = anterior->getProxArco()->getProxArco();
                arcoRemover = anterior->getProxArco();
                anterior->setProxArco(sucessor);
            }
        }
        if(arcoRemover != NULL){
            delete arcoRemover;
            this->numeroArcos--;
            noOrigem->setGrau(noOrigem->getGrau() - 1);

            if(atualizarGrau)
                this->atualizaGrau();
            }
    }
}

void Grafo::removeArco(u_int idOrigem, u_int idDestino){
    this->removeArco(buscaNo(idOrigem), buscaNo(idDestino));
}

void Grafo::removeArcosLigadasAoNo(No *no, bool atualizaGrau = true){
    No *aux=this->listaNos;
    while(aux != NULL){
        this->removeArco(aux, no, false);
        aux=aux->getProxNo();
    }
    if(atualizaGrau)
        this->atualizaGrau();
}

void Grafo::removeArcos(No *no, bool atualizarGrau = true){
    this->numeroArcos -= no->getGrau();
    no->removeArcos();
    if(atualizarGrau)
        this->atualizaGrau();
}

void Grafo::atualizaGrau(){
    this->grau=0;
    for(No *i=listaNos; i!=NULL; i=i->getProxNo()){
        if(i->getGrau() > grau)
            grau=i->getGrau();
    }
}

///Faz a atualização dos graus de Entrada e Saida do digrafo para deteccao de nós fonte e sinks
void Grafo::atualizaGrausEntradaSaidaDosNos(){
    ///Zerar todos os graus de entrada e de saida
    for(No *i=listaNos; i!=NULL; i=i->getProxNo()){
        i->setGrauEntrada(0);
        i->setGrauSaida(0);
    }

    ///Percorrer todos os nos e suas listas de adjacencia incrementando os graus de entrada e de saida
    for(No *i=listaNos; i!=NULL; i=i->getProxNo()){
        for(Arco *j=i->getListaArcos(); j!=NULL; j=j->getProxArco()){
            i->setGrauSaida(i->getGrauSaida()+1);
            j->getNoDestino()->setGrauEntrada(j->getNoDestino()->getGrauEntrada()+1);
        }
    }
}

void Grafo::removeNo(u_int id){
    No *noRemover=NULL;

    ///se a listaNos eh o no a ser removido
    if(listaNos->getID()==id){
        noRemover = listaNos;
        listaNos = listaNos->getProxNo();
    }
    else{
        No *anterior= listaNos;
        while(anterior->getProxNo()!=NULL &&
              anterior->getProxNo()->getID()!=id)
            anterior=anterior->getProxNo();

        /// no existe no grafo
        if(anterior->getProxNo() != NULL){
            No *sucessor = anterior->getProxNo()->getProxNo();
            noRemover = anterior->getProxNo();
            anterior->setProxNo(sucessor);
        }
    }
    if(noRemover!=NULL){
//        cout <<"\nremovendo no com id:" << noRemover->getID()<<endl;
        this->removeArcos(noRemover, false);
        this->removeArcosLigadasAoNo(noRemover, false);
        this->atualizaGrau();
        delete noRemover;
        this->numeroNos--;
    }
}

bool compareReverse(u_int a, u_int b){
    return a >= b;
}

/** retorna a sequencia de inteiros dos graus do no */
u_int* Grafo::sequenciaGrau(){
    u_int* seq = new u_int [this->numeroNos];
    u_int cont = 0;
    No* noAux = listaNos;
    while(noAux != NULL){
        seq[cont] = noAux->getGrau();
        noAux = noAux->getProxNo();
        cont++;
    }

    sort(seq, seq+cont, compareReverse);
    return seq;
}

void Grafo::imprime(){
    printf("\n\n-----------------PRINT DO GRAFO-------pot_ativ_total = %2.5f-----pot_reat_total = %2.5f--------\n\n", p_ativ_total, p_reat_total);
    cout<<"Grau do Grafo: "<<this->grau<<"\tnumero de nos: "<<this->numeroNos
    <<"\tnumero de arcos: "<<this->numeroArcos<<
    " \t\t obs: r=resistencia, c=carga=potencia_ativa, p_re=potencia_reativa, f=fluxo(inicialmente nao sei calcular fluxo, coloquei -1.0)\n\n";
    No *no=listaNos;
    while(no!=NULL){
        printf("\n");
        no->imprime();
        no=no->getProxNo();
    }
    printf("\n\n----------------------------------------------\n\n");
}

void Grafo::leArquivo(char nome[]){
    u_int i,j,n_nos;
    ifstream arq;
    arq.open(nome);
    if(arq.good()==false)
        cout<<"arquivo nao encontrado"<<endl;
    arq>>n_nos;
    for(u_int i=1;i<=n_nos;i++)
        insereNo(i);
    while(arq.good()){
        arq>>i>>j;
        insereArcoID(i,j, numeroArcos+1);
    }
}

bool Grafo::saoAdjacentes(u_int id1, u_int id2){
    No *no1=buscaNo(id1);
    No *no2=buscaNo(id2);
    return saoAdjacentes(no1, no2);
}

bool Grafo::saoAdjacentes(No *no1, No *no2){
    ///ser ou não digrafo não interfere na resposta
    return no1->ehAdjacente(no2)||no2->ehAdjacente(no1);
}

/**
FUNÇÃO AUXILIAR PARA FAZER A FUNÇÃO DO DUARDO FUNCIONAR.
Recebe um nó e retorna o numero de nos da componente conexa que o nó esta presente.
*/
u_int Grafo::numeroNosComponenteFortementeConexa(No *no1){
    u_int n_nos=0;
    for(No *no2=this->listaNos; no2!=NULL; no2=no2->getProxNo()){
        if(this->mesmaComponenteFortementeConexa(no1, no2))// && this->mesmaComponenteFortementeConexa(no2, no1))
            n_nos++;
    }
    return n_nos;
}

int Grafo::auxEhNoArticulacao(No *no){
    ///Percurso em profundidade
    if(no->getMarcado()==false){
            no->setMarcado(true);
            for(Arco *a=no->getListaArcos(); a!=NULL; a=a->getProxArco())
                return 1+auxEhNoArticulacao(a->getNoDestino());
    }
    return 0;
}
//void Grafo::incrementaContador(No* n){
//    this->contAux++;
//}

/**
ESSA FUNÇÃO DO JEITO QUE ESTAVA ANTES NÃO FUNCIONAVA!!!!!
Marca o nó, faz a busca em profundidade e ve se o numero de nos marcados e menor do que o numero de nos da componente conexa,
se for verdade o no e de articulacao.
*/
bool Grafo::ehNoArticulacao(No *no){
    if(no!=NULL){
        u_int cont=0;
        this->desmarcaNos();
        no->setMarcado(true);
    //    cout<<"busca em progundidade começando em:"<<no->getListaArcos()->getNoDestino()->getID()<<endl;
        if(no->getListaArcos()!=NULL){
            this->percursoProfundidade(no->getListaArcos()->getNoDestino());
            for(No *i=this->getListaNos(); i!=NULL; i=i->getProxNo()){
                if(i->getMarcado()==true)
                    cont++;
            }
        //    cout<<"cont de nos marcados:"<<cont<<endl;
        //    cout<<"numero de nos de "<< no->getID()<<" e:"<<NosComponenteConexa(no)<<endl;
            bool result = (cont<this->numeroNosComponenteFortementeConexa(no));
            this->desmarcaNos();
            return result;
        }
        else
            return false;
    }
    else return false;

    ///COMO O MELAO TINHA FEITO2
//    ///Marca o no como visitado e faz busca em profundidade
//    ///no seu primeiro adjacente, contando os nos visitados.
//    ///Se o numero de nos for menor que n-1, significa que
//    ///aquele no marcado no inicio eh de articulacao.
//
//    this->desmarcaNos();
//    no->setMarcado(true);
//    Arco *a=no->getListaArcos();
//    if(a==NULL) return false;
//    No *noAux=a->getNoDestino();
//    u_int cont=this->auxEhNoArticulacao(noAux);
//    ///tem que verificar no final se todos os nós da mesma componente conexa estao marcados no final.
//    if(cont<this->NosComponenteConexa(no)) { cout<<"cont:"<<cont<<endl; return true;};
//    return false;
//    this->desmarcaNos();///MELÃO ESQUECEU DE DESMARCAR OS NÓS DEPOIS
}

bool Grafo::ehNoArticulacao(u_int id){
    No* no=buscaNo(id);
    if(no != NULL)
        return this->ehNoArticulacao(buscaNo(id));
    return false;
}

/** PENSANDO...
bool Grafo::ehArcoPonte(Arco* arco){
    this->desmarcaNos();
    ///Verificar se componente fortemente conexa tem ordem = 2
    arco->getNoDestino()->setMarcado(true);
    this->setContAux(0);
    this->percursoProfundidade(arco->getNoOrigem());
    bool 2_conexo = this->contAux == 0;


    return ehNoArticulacao(arco->getNoDestino());
}
*/

/**
Se o grafo é fortemente conexo cada par de vertice (a,b) estao na mesma conponente conexa.
*/
bool Grafo::ehFortementeConexo(){
    for(No *inicio1=listaNos; inicio1!=NULL; inicio1=inicio1->getProxNo()){
        for(No *inicio2=listaNos; inicio2!=NULL; inicio2=inicio2->getProxNo()){
            if(!this->mesmaComponenteFortementeConexa(inicio1, inicio2))
                return false;
        }
    }
    return true;
}

/**
Função para retornar a rubustez de um grafo baseado nas vertices.
A rubustez baseada em vertices indica o numero maximo de vertices que podem ser removidos do grafo mantendo a conexividade.
Se o no a ser removido nao for no de articulação entao o numero de componentes conexas apos a remoção do nó se mantem.
Assim qualquer no que nao seja de articulação pode ser removido e o grafo ainda se mantem conexo, a rubustez e entao
o numero maximo de nos menos o numero de nos de articulacao.
Coloquei pra receber os ids dos nos de articulacao pra conferir na função '' do main se esta funcionando.

IMPORTANTE!!!!
No caso a função retorna o numero de nos que voce pode retirar considerando todas as componentes conexas.
Se uma componente conexa pode ter 5 nos removidos e a outra pode ter 6 a função retorna 11.
recebe um vetor de ids pra salvar os ids daqueles nos que nao podem ser removidos e sao de articulacao
*/
u_int Grafo::rubustezVertice(u_int *ids){
    u_int rubustez=this->numeroNos, i=0;
    ids=new u_int[this->numeroNos];
    for(No *inicio=this->listaNos; inicio!=NULL; inicio=inicio->getProxNo()){
        cout<< ( (float)( 1001 - inicio->getID() )/this->numeroNos ) * 100 <<"%"<<endl;
        if(this->ehNoArticulacao(inicio)){
            cout<<"achou articulacao"<<endl;
            rubustez--;
            ids[i]=inicio->getID();///armazenar id dos nos de articulação
            i++;
        }
    }

    return rubustez;
}

vector<No*>  Grafo::vizinhancaAberta(u_int id, bool fechada){
    return vizinhancaFechada(id, fechada);
}

vector<No*> Grafo::vizinhancaFechada(u_int id, bool fechada){
    No* no=buscaNo(id);
    vector<No*> nos;
    if(fechada)
        nos.push_back(no);
    if(no != NULL)
        for(Arco *a=no->getListaArcos(); a!=NULL; a=a->getProxArco())
            nos.push_back(a->getNoDestino());
    return nos;
}

///Cria um novo Grafo, com os mesmos nós e arestas (por id) do grafo atual
Grafo* Grafo::clone(){
    int idArvore=1;
    Grafo* G=new Grafo();
    for(No *i=listaNos; i!=NULL; i=i->getProxNo()){
        G->insereNo(i->getID())->setIdArvore(idArvore);///id auxiliar para algoritmo de kruskal
        idArvore++;
    }
    for(No *i=listaNos; i!=NULL; i=i->getProxNo()){
        for(Arco *j=i->getListaArcos(); j!=NULL; j=j->getProxArco()){
            G->insereArcoID(i->getID(), j->getNoDestino()->getID(), j->getID());
        }
    }

    ///Precisa retirar algumas atribuicoes redundantes?
    G->setFlagDir(this->flagDir);
    G->grau=this->grau;
    G->numeroNos=this->getNumeroNos();
    G->numeroArcos=this->getNumeroArcos();
    return G;
}

/**
 * Detecta todos os nós fonte e adiciona aos candidatos.
 * A cada iteracao, pega um candidato, coloca na solucao,
 * remove ele do grafo e verifica novos candidatos.
 */
vector<No*> Grafo::ordenacaoTopologicaDAG(){
    vector<No*> solucao;
    vector<u_int> candidatos;
    Grafo* G=this->clone();
    for(u_int k=0;k<this->getNumeroNos();k++){
        G->atualizaGrausEntradaSaidaDosNos();
        for(No *i=G->listaNos; i!=NULL; i=i->getProxNo()){
            if(i->getGrauEntrada()==0){
                candidatos.push_back(i->getID());
                G->removeNo(i->getID());
            }
        }
        solucao.push_back(this->buscaNo(candidatos[0]));//Vou colocar na solucao ponteiros do grafo original(this)
        candidatos.erase(candidatos.begin());
    }
    cout<<"Ordenacao topologica do DAG por ID:"<<endl;
    for(u_int m=0;m<this->getNumeroNos();m++){
        cout<<solucao[m]->getID()<<endl;
    }
    delete G;
    return solucao;
}

/**
 * Percorre todos os nós do grafo e, para cada nó, incrementa o contador e
 * realiza a busca em profundidade caso ele não esteja marcado. A busca em profundidade marca
 * os nós pertencentes a uma mesma componente conexa, logo o contator não vai ser incrementado ao passar por um nó já percorrido.
*/
int Grafo::numeroComponentesConexas(){
    this->desmarcaNos();
    int num=0;
    for(No *i=this->getListaNos(); i!=NULL; i=i->getProxNo()){
        if(i->getMarcado()==false){
            num++;
            percursoProfundidade(i);
        }
    }
    return num;
}

Arco* Grafo::buscaArco(u_int id1, u_int id2){
    No *no1=buscaNo(id1);
    No *no2=buscaNo(id2);
    return buscaArco(no1, no2);
}

Arco* Grafo::buscaArco(No* no1, No* no2){
    Arco *aux = no1->getListaArcos();
    while(aux!=NULL){
        if(no2 == aux->getNoDestino())
            return aux;
        aux = aux->getProxArco();
    }
    return NULL;
}

double Grafo::consultaMenorCaminhoEntreDoisNos(u_int i, u_int j){
    double **menorCaminho = algoritmoFloyd();
    return menorCaminho[i][j];
}

double** Grafo::algoritmoFloyd(){
    double infinito = INFINITO;
    Arco *aux;
    int n = this->getNumeroNos();
    double **mat = new double*[n];
    for(int i = 0; i < n; i++){
        mat[i] = new double[n];
        for(int j = 0; j < n; j++){
            if(i == j) ///Assumindo que nao vai existir selfie loop
                mat[i][j] = 0;
            else{
                aux = buscaArco(i, j);
                if(aux != NULL)
                    mat[i][j] = aux->getfluxoP_ativ();
                else
                    mat[i][j] = infinito;
            }
        }
    }
    for(int k = 0; k < n; k++){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(mat[i][j] > mat[i][k] + mat[k][j])
                    mat[i][j] = mat[i][k] + mat[k][j];
            }
        }
    }
    return mat;
}

/***
funcionando parciamente para grafos direcioandos. Internet diz que o algoritmo so serve para grafos nao direcionado. Verificar.
Verificar complexidade do algoritmo.
***/

///operador para ordenar as arestas por fluxo (Usado no algoritmo de Kruskal)
bool menorfluxo(Arco *a1, Arco *a2){return ( a1->getfluxoP_ativ() < a2->getfluxoP_ativ() );};

vector<Arco*> Grafo::Kruskal(){
    ///todos os arcos para ordenacao e 'solucao' que e a solucao (os arcos que forma as arvore/floresta)
    vector<Arco*> arcos, solucao;
    u_int idArvore=1;

    ///joga todos os arcos no vector para depois ordenar
    for(No *i=this->getListaNos(); i!=NULL; i=i->getProxNo()){
        ///cada no esta inicialmente em uma arvore separada
        i->setIdArvore(idArvore);
        for(Arco *a=i->getListaArcos(); a!=NULL; a=a->getProxArco())
                arcos.push_back(a);
        idArvore++;
    }

    sort(arcos.begin(), arcos.end(), menorfluxo);

    ///nos origem e destino para cada arco
    No *orig, *dest;
    u_int id_orig, id_dest;

    for(u_int pos=0; pos <arcos.size(); pos+=2){
        orig=arcos.at(pos)->getNoOrigem();
        dest=arcos.at(pos)->getNoDestino();

        id_orig=orig->getIdArvore();
        id_dest=dest->getIdArvore();

        ///se arco conecta nos de componentes conexas diferentes ele esta na solucao
        if(id_orig!=id_dest){
            solucao.push_back(arcos.at(pos));
            ///coloca ids iguais para nos em mesma componente conexa
            for(No *i=this->getListaNos(); i!=NULL; i=i->getProxNo()){
                if(i->getIdArvore()==id_orig)
                    i->setIdArvore(id_dest);
            }
        }
    }

    return solucao;
}


void Grafo::leEntrada(char nome[])
{
    double fator_MW = 1e-3;


    ifstream entrada;
    entrada.open(nome);
    double carga, resistencia, reatancia, potencia_reativa, voltagem;

    string aux;

    int n_col_nos=12, n_col_arestas=12;

    do{
        entrada >> aux;
    }while(aux != "Vb");
    entrada >> aux;///le o sinal de igual
    double VB, PB, ZB;
    entrada >> VB;
    entrada >> aux;entrada >> aux;
    entrada >> PB;
    entrada >> aux;entrada >> aux;
    entrada >> ZB;
    this->pb = PB; this->vb = VB; this->zb = ZB;
    cout << "\nVB: " << this->vb << "\tPB: " << this->pb << "\tZB: " << this->zb << endl;



    entrada.close();
    entrada.open(nome);

    u_int idNo, idOrig, idDest;

    ///potencia ativa = carga

    do{
        entrada >> aux;
    }while(aux != "num_nos");///pula lixo ate chegar na parte de informacoes do no

    u_int num_nos;
    entrada >> num_nos;
    for(u_int i=1 ; i<=num_nos*n_col_nos; i++){
        if(i % n_col_nos == 1){
            entrada >> idNo;
            //cout << "idNo: " << idNo;
        }
        else if(i % n_col_nos == 4){
            entrada >> voltagem;
            //cout << "V: " << voltagem;
        }
        else if(i % n_col_nos == 10){
            entrada >> carga;
            carga /= PB;
            carga *=fator_MW;
            //cout << "      pot at=carga: " << carga;
        }
        else if(i % n_col_nos == 11){
            entrada >> potencia_reativa;
            potencia_reativa /=PB;
            potencia_reativa *= fator_MW;
            //cout << "      pot reat: " << potencia_reativa << endl;
            insereNoCargaVoltagem(idNo, carga, potencia_reativa, voltagem);
        }
        else
            entrada >> aux;
    }

    do{
        entrada >> aux;
    }while(aux != "num_arestas");///pula mais lixo ate chegar na parte de informacoes da aresta

    u_int num_arestas;
    entrada >> num_arestas;

    for(u_int i=1 ; i<=num_arestas*n_col_arestas; i++){
        if(i % n_col_arestas == 1){
            entrada >> idOrig;
            //cout << "idOrig: " << idOrig;
        }
        else if(i % n_col_arestas == 2){
            entrada >> idDest;
            //cout << "      idDest: " << idDest;
        }
        else if(i % n_col_arestas == 3){
            entrada >> resistencia;
            resistencia /= ZB;
            //cout << "      r: " << resistencia << "      i%13=" << this->numeroArcos <<endl;
        }
        else if(i % n_col_arestas == 4){
            entrada >> reatancia;
            reatancia /= ZB;
            //cout << "      reat: " << reatancia;
            insereArcoDados(idOrig, idDest, this->numeroArcos + 1, resistencia, reatancia, 0.0, 0.0, true);
        }
        else
            entrada >> aux;
    }

    cout << "\n" << nome << " lida!\n\n\n" << endl;

    for(No *no=listaNos; no!=NULL; no=no->proxNo){
        if(no->grauEntrada>1)
            this->nosEntrada.push_back(no);
    }

    cout << "vetore de nos de entrada construidos!\n\n\n" << endl;
}

/*Gera um vetor de arcos da solucao (valor das chaves de acordo com o uso ou nao dela)*/
vector<Arco*> Grafo::geraVetorArcos(){
    vector<Arco*> arcos;
    for(No *i = listaNos; i!=NULL; i = i->getProxNo()){
        for(Arco *a = i->getListaArcos(); a!=NULL; a = a->getProxArco()){
            arcos.push_back(a);
        }
    }
    return arcos;
}

void movimentoSolucao(vector<Arco*> solucao){
    u_int id1, id0;

    ///escolhe dois ids de dois arcos, um aberto e outro fechado
    ///os arcos sao trocados para se gerar a vizinhanca

    id1 = rand() % solucao.size();
    while(solucao.at(id1)->chave!=1)
        id1 = rand() % solucao.size();


    id0 = rand() % solucao.size();
    while(solucao.at(id0)->chave!=0)
        id0 = rand() % solucao.size();

    bool aux = solucao.at(id1)->chave;
    solucao.at(id1)->chave = solucao.at(id0)->chave;
    solucao.at(id0)->chave = aux;

}

bool Grafo::validaSolucao(){
    desmarcaNos();
    percursoProfundidade(this->listaNos);
    if(this->n_marcados != this->numeroNos)
        return false;
    else
        return true;
}

double Grafo::calculaPerdaTotal(){
    this->perdaTotal = 0.0;
    for(No *i = this->listaNos; i!=NULL; i = i->getProxNo()){
        for(Arco *a = i->getListaArcos(); a!=NULL; a = a->getProxArco()){
            if(a->chave == true)
                perdaTotal += a->perda_ativ;
        }
    }
    return perdaTotal;
}

///agora vai!--------------tem que ir

double Grafo::cargasPerdasRamoAtiv(No *no){
    double soma=no->carga;
    auxcargasPerdasRamoAtiv(no, soma);
    return soma;
}

void Grafo::auxcargasPerdasRamoAtiv(No *no, double &soma){
    for(Arco *a=no->listaArcos; a!=NULL; a=a->proxArco){

        ///nao descer por arcos com chave aberta
        while(a!=NULL && a->chave == false){
            //printf("A%d\n", a->getID());
//            printf("\nzerou [%d]a( %d , %d )", a->chave, a->noOrigem->id, a->noDestino->id);
            a->fluxoP_ativ = a->fluxoP_reativ = a->perda_ativ = a->perda_reat = 0.0;

            a = a->getProxArco();
        }
        if(a==NULL)
            break;

//        cout << "(" << a->noOrigem->id << "," << a->noDestino->id<< ")\t" << a->noDestino->id << endl;
        soma+=a->noDestino->carga + a->perda_ativ;
        auxcargasPerdasRamoAtiv(a->noDestino, soma);
    }
}

double Grafo::cargasPerdasRamoReAtiv(No *no){
    double soma=no->potencia_reativa;
    auxcargasPerdasRamoReAtiv(no, soma);
    return soma;
}

void Grafo::auxcargasPerdasRamoReAtiv(No *no, double &soma){
    for(Arco *a=no->listaArcos; a!=NULL; a=a->proxArco){

        ///nao descer por arcos com chave aberta
        while(a!=NULL && a->chave == false){
            //printf("A%d\n", a->getID());
//            printf("\nzerou [%d]a( %d , %d )", a->chave, a->noOrigem->id, a->noDestino->id);
            a->fluxoP_ativ = a->fluxoP_reativ = a->perda_ativ = a->perda_reat = 0.0;

            a = a->getProxArco();
        }
        if(a==NULL)
            break;

//        cout << "(" << a->noOrigem->id << "," << a->noDestino->id<< ")\t" << a->noDestino->id << endl;
        soma+=a->noDestino->potencia_reativa + a->perda_reat;
        auxcargasPerdasRamoReAtiv(a->noDestino, soma);
    }
}







///FOWARD
void Grafo::foward(u_int it){
//    cout << "\n\n\n\n\tFOWARD!!!\n" << endl;
    this->listaNos->voltagem = 1.0;///voltagem controlada na estacao
    Auxfoward(this->listaNos, this->listaNos->listaArcos, it);
}

void Grafo::Auxfoward(No *no, Arco *ak, u_int it){
    if(no == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        for(Arco *a=no->getListaArcos(); a!=NULL; a=a->getProxArco()){

            ///nao descer por arcos com chave aberta
            while(a!=NULL && a->chave == false){
                //printf("A%d\n", a->getID());
//                printf("\nzerou [%d]a( %d , %d )", a->chave, a->noOrigem->id, a->noDestino->id);
                a->fluxoP_ativ = a->fluxoP_reativ = a->perda_ativ = a->perda_reat = 0.0;

                a = a->getProxArco();
            }
            if(a==NULL)
                break;

            ///----foward----

            ///chute inicial para o fluxo nas arests que partem do no terminal
            if(no==listaNos){
                a->fluxoP_ativ = cargasPerdasRamoAtiv(a->noDestino);
                a->fluxoP_reativ = cargasPerdasRamoReAtiv(a->noDestino);

            }else{

                double perda_ativ = 0.0;
                double perda_reat = 0.0;
                if(it>0){
                    perda_ativ = ak->resistencia*(pow(ak->fluxoP_ativ, 2) + pow(ak->fluxoP_reativ, 2)) / pow(no->voltagem, 2);
                    perda_reat = ak->reatancia*(pow(ak->fluxoP_ativ, 2) + pow(ak->fluxoP_reativ, 2)) / pow(no->voltagem, 2);
                }

                a->fluxoP_ativ   = ak->fluxoP_ativ - perda_ativ - no->carga;
                a->fluxoP_reativ   = ak->fluxoP_reativ - perda_reat - no->potencia_reativa;

                ///bifurcacao
                if(a->noOrigem->grauSaida>1){
                    double somaAtiv=0.0, somaReAtiv=0.0;
                    for(Arco *aux=no->getListaArcos(); aux!=NULL; aux=aux->proxArco){
                        if(aux!=a && aux->chave==true){
                            somaAtiv+=aux->perda_ativ + cargasPerdasRamoAtiv(aux->noDestino);
                            somaReAtiv+=aux->perda_reat + cargasPerdasRamoReAtiv(aux->noDestino);
//                            cout << "a(" << a->noOrigem->id << "," << a->noDestino->id << ")";
//                            cout << "\t a->fat: " << a->fluxoP_ativ;
//                            cout << "\taux(" << aux->noOrigem->id << "," << aux->noDestino->id << ")";
//                            cout << "\t aux->somaAtiv: " << somaAtiv << endl;
                        }
                    }

                    a->fluxoP_ativ -=somaAtiv;
                    a->fluxoP_reativ -=somaReAtiv;
                }
            }

            ///--------------


            Auxfoward(a->getNoDestino(), a, it);
        }
    }
}




void Grafo::backward(){
//    cout << "\n\n\n\n\tBACKWARD!!!" << endl;
    this->listaNos->voltagem = 1.0;///voltagem controlada na estacao
    Auxbackward(this->listaNos, this->listaNos->listaArcos);
}

void Grafo::Auxbackward(No *no, Arco *ak){
    if(no == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        for(Arco *a=no->getListaArcos(); a!=NULL; a=a->getProxArco()){

            ///nao descer por arcos com chave aberta
            while(a!=NULL && a->chave == false){
                //printf("A%d\n", a->getID());
//                printf("\nzerou [%d]a( %d , %d )", a->chave, a->noOrigem->id, a->noDestino->id);
                a->fluxoP_ativ = a->fluxoP_reativ = a->perda_ativ = a->perda_reat = 0.0;

                a = a->getProxArco();
            }
            if(a==NULL)
                break;

            ///----backward----

            No *noDest = a->getNoDestino();
            No *noOrig = a->getNoOrigem();

            noDest->voltagem = pow(noOrig->voltagem, 2)
            - 2*(a->resistencia*a->fluxoP_ativ + a->reatancia*a->fluxoP_reativ) +
            (pow(a->resistencia, 2) + pow(a->reatancia, 2))*
            (pow(a->fluxoP_ativ, 2) + pow(a->fluxoP_reativ, 2))/pow(noOrig->voltagem, 2);

            noDest->voltagem = sqrt(noDest->voltagem);

            a->perda_ativ = a->resistencia*(pow(a->fluxoP_ativ, 2) + pow(a->fluxoP_reativ, 2))
            /pow(noOrig->voltagem, 2);
            a->perda_reat = a->reatancia*(pow(a->fluxoP_ativ, 2) + pow(a->fluxoP_reativ, 2))
            /pow(noOrig->voltagem, 2);

//            cout << "\nperda ativa: " << a->resistencia*(pow(a->fluxoP_ativ, 2) + pow(a->fluxoP_reativ, 2))
//            /pow(noOrig->voltagem, 2);

//            cout << "no(k): " << noOrig->id;
//            cout << "\tno(m): " << noDest->id << endl;


            ///--------------

            Auxbackward(a->getNoDestino(), a);
        }
    }
}

void Grafo::abreFechaChavesGrafo(bool **vetChaves){
//    cout << "abrindo e fechando arcos...";
    ///para cada no com grau de entrada maior que 1
    for(u_int i=0; i<nosEntrada.size(); i++)
    {
//        cout << "No i:" << i << endl;
        ///para cada arco que chega no no
        for(u_int j=0; j<nosEntrada.at(i)->grauEntrada; j++){
            nosEntrada.at(i)->volta.at(j)->chave = vetChaves[i][j];
        }
    }
//    cout << "feito!" << endl;
}

double Grafo::funcaoObjetivo(bool **vetChaves, double tol){
    this->abreFechaChavesGrafo(vetChaves);
    this->calcula_fluxos_e_perdas(tol);
    return calculaPerdaTotal();
}

///construtivo que gera uma arvore aleatoria
bool **Grafo::construtivoAleatorio(){
    bool **vetChaves;
    if(this->nosEntrada.size() > 0){
        vetChaves = new bool*[this->nosEntrada.size()];
        for(u_int i=0; i<this->nosEntrada.size(); i++){
            vetChaves[i] = new bool[nosEntrada.at(i)->volta.size()];
        }
    }

    ///para cada no de entrada
    for(u_int i=0; i<this->nosEntrada.size(); i++){

        u_int id = rand() % nosEntrada.at(i)->volta.size();

        ///para cada arco que incide no no
        for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++){
            if(j==id)
                vetChaves[i][j] = true;
            else
                vetChaves[i][j] = false;
        }
    }

    return vetChaves;
}

void Grafo::ehArvore(){
    cout << "\nverificando se grafo com as chaves no estado atual e uma arvore...\n";
    auxEhArvore(listaNos);
    cout << "\n\nfim de verificacao, se nenhuma menssagem de ciclo apareceu quer dizer que e arvore\n\n";
}

void Grafo::auxEhArvore(No *no){
    if(no == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        if(no->getMarcado() == false){
//            cout<<"marcando: "<<no->getID()<<endl;
            no->setMarcado(true);
            n_marcados++;
            this->contAux++;
//            if (funcao != NULL)
//                (this->*funcao)(no);
            for(Arco *a=no->getListaArcos(); a!=NULL; a=a->getProxArco()){

                ///nao descer por arcos com chave aberta
                while(a!=NULL && a->chave == false){
                    //printf("A%d\n", a->getID());
                    a = a->getProxArco();
                }

                if(a==NULL)
                    break;

                auxEhArvore(a->getNoDestino());
            }
        }
        else
            cout << "\nCICLO ENCONTRADO!";
    }
}

void Grafo::imprimeChaves(bool **chaves){
    for(u_int i=0; i<nosEntrada.size(); i++){
        printf("no(%d) - [ ", nosEntrada.at(i)->id);
        for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++)
            cout << chaves[i][j] << ", ";
        cout << "]\n";
    }
    cout << "\n\n" << endl;
}

void Grafo::mutacao(bool **vetChaves){
//    cout << "aplicando mutacao...";
    u_int tam=0;

    for(u_int i=0; i<nosEntrada.size(); i++)
        tam+= nosEntrada.at(i)->volta.size();

    u_int id = rand() % tam;

    u_int idNo=0, pos=0;
    for(idNo=0; pos<=id; idNo++)
        pos+= nosEntrada.at(idNo)->volta.size();

    idNo--;
//    cout << "id nosEntrada:" << idNo << "\tid: " << id <<"\t tam: " << tam <<endl;
//    cout << "\nidNo: " << nosEntrada.at(idNo)->id;
    u_int idArco = rand() % nosEntrada.at(idNo)->volta.size();
//    cout << "\tidArco: " << idArco << endl;


    for(u_int j=0; j<nosEntrada.at(idNo)->volta.size(); j++){
        if(j==idArco)
            vetChaves[idNo][j] = true;
        else
            vetChaves[idNo][j] = false;
    }
//    cout << "feito!" << endl;
}

/**mantem metade dos genes do pai1 e metade do pai2
gene = cadeia de bits de um no de entrada(grau de entrada > 1)
**/
bool **Grafo::cruzamento_metade(bool **pai1, bool **pai2){
    bool **filho = new bool*[nosEntrada.size()];
    for(u_int i=0; i<nosEntrada.size(); i++)
        filho[i] = new bool[nosEntrada.at(i)->volta.size()];

    for(u_int i=0; i<nosEntrada.size(); i++){
        if(i%2==0){
            for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++)
                filho[i][j] = pai1[i][j];
        }
        else{
            for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++)
                filho[i][j] = pai2[i][j];
        }
    }
    return filho;
}

vector<bool**> Grafo::populacaoInicial(u_int num_individuos){
    cout << "gerando populacao inicial...";
    vector<bool**> populacao;
    for(u_int i=0; i<num_individuos; i++)
        populacao.push_back(construtivoAleatorio());///gera uma populacao inicial inteiramente aleatoria

    cout << "feito!\n\n" << endl;
    return populacao;
}

void Grafo::proximaGeracao(vector<bool**>& populacao){

    vector<bool**> proxGeracao;

    cout << "gerando proxima geracao..";

    u_int tamPopulacao = populacao.size()-1;

    ///cruzamento 2 a 2 - i com i+1
    for(u_int i=0; i<tamPopulacao; i++){
        bool **filho = cruzamento_metade(populacao.at(i), populacao.at(i+1));

        if(rand() % 2 == 1)
            mutacao(filho);

//        populacao.push_back(filho);
        proxGeracao.push_back(filho);
    }
    ///cruzamento 2 a 2 geracao atual com nova
    for(u_int i=0; i<tamPopulacao; i++){
        bool **filho = cruzamento_metade(populacao.at(i), proxGeracao.at(i));

        if(rand() % 2 == 1)
            mutacao(filho);

//        populacao.push_back(filho);
        proxGeracao.push_back(filho);
    }

    populacao = proxGeracao;
    cout << "feito!(tam=" << populacao.size() << ")";

}

void Grafo::sobrevivencia(vector<bool**>& populacao){
    cout << "selecionando individuos..";

    int dif = populacao.size() - (int)1.1*numeroNos;
    if(populacao.size()>(int)1.1*numeroNos)
        populacao.erase(populacao.begin(), populacao.begin() + dif);

    cout << "feito!(tam=" << populacao.size() << ")";
}

bool **Grafo::melhorIndividuoPopulacao(vector<bool**> populacao){
    bool **melhor = populacao.at(0);

    for(u_int i=1; i<populacao.size(); i++){
        if( funcaoObjetivo(populacao.at(i), 1e-6) < funcaoObjetivo(melhor, 1e-6) )
            melhor = populacao.at(i);
    }
    return melhor;
}

bool **Grafo::algoritmoGenetico(u_int itSemMelhora){

    cout << "\n\n----------ALGORITMO GENETICO----------\n\n";

    ///cria populacao inicial
    vector<bool**> populacao = populacaoInicial(this->numeroNos);


    bool **melhorIndividuo = populacao.at(rand()%populacao.size());

    cout << "melhor individuo populacao inicial: " << funcaoObjetivo(melhorIndividuoPopulacao(populacao), 1e-6) << endl;

    bool **melhorIndividuoAtual;

    double atual, melhor = funcaoObjetivo(melhorIndividuo, 1e-6);

    u_int it = 0;
    while(it <=itSemMelhora){

        proximaGeracao(populacao);
        sobrevivencia(populacao);

        melhorIndividuoAtual = melhorIndividuoPopulacao(populacao);


        atual = funcaoObjetivo(melhorIndividuoAtual, 1e-6);

        cout << "  atual: " << atual << "  melhor:" << melhor;

        ///se o melhor individuo nao sobreviveu
        if(melhorIndividuo==NULL)
            melhorIndividuo = melhorIndividuoAtual;
        else if(atual < melhor){
            melhorIndividuo = melhorIndividuoAtual;
            it = 0;
            cout << "         " <<melhor << " --> " << atual;
            melhor = funcaoObjetivo(melhorIndividuo, 1e-6);
        }else{
            cout << "    it sem melhora: " << it <<endl;
        }

        cout << "\n";
        it++;
    }

    return melhorIndividuo;
}

///agora vai!--------------tem que ir


///FALTA DESTRUTOR!
Grafo::~Grafo(){}
