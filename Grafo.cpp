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

void Grafo::calcula_fluxos_e_perdas(double tol){

    double *perdas_total_anterior, *perdas_total_atual, erro = 1.0;


    for(u_int it=0; erro>tol; it++){

        perdas_total_anterior = this->soma_perdas();

        ///calcula fluxos de potencia nas linhas
        foward(it);

        ///calcula voltagem nas barras e perdas nas linhas
        backward();

        perdas_total_atual = this->soma_perdas();
        erro = perdas_total_atual[0] - perdas_total_anterior[0];

//        cout << "\n\nperda total da rede: (" << perdas_total_atual[0] << " , " << perdas_total_atual[1] << ")" << endl;
//        cout << "DIFERENCA ENTRE PERDA ATUAL E ANTERIOR CALCULADA: ( " << perdas_total_atual[0] - perdas_total_anterior[0]
//        << " , " << perdas_total_atual[1] - perdas_total_anterior[1] << " )\n";

        delete perdas_total_anterior;
        delete perdas_total_atual;

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

Arco *Grafo::buscaArcoID(u_int id){
    for(No *no=listaNos; no!=NULL; no=no->proxNo){
        for(Arco *a=no->listaArcos; a!=NULL; a=a->proxArco){
            if(a->id == id)
                return a;
        }
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

bool menorResistencia(Arco *a1, Arco *a2){return ( a1->resistencia < a2->resistencia );};

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

    sort(arcos.begin(), arcos.end(), menorResistencia);

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

    int n_col_nos=13, n_col_arestas=12;

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
    this->pb = PB; this->vb = VB; this->zb = ZB; this->fator = fator_MW;
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
            cout << "idNo: " << idNo;
        }
        else if(i % n_col_nos == 4){
            entrada >> voltagem;
            cout << "      V: " << voltagem;
        }
        else if(i % n_col_nos == 10){
            entrada >> carga;
            carga /= PB;
            carga *=fator_MW;
            cout << "      pot at=carga: " << carga;
        }
        else if(i % n_col_nos == 11){
            entrada >> potencia_reativa;
            potencia_reativa /=PB;
            potencia_reativa *= fator_MW;
            cout << "      pot reat: " << potencia_reativa << endl;
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
            cout << "idOrig: " << idOrig;
        }
        else if(i % n_col_arestas == 2){
            entrada >> idDest;
            cout << "      idDest: " << idDest;
        }
        else if(i % n_col_arestas == 3){
            entrada >> resistencia;
            resistencia /= ZB;
            cout << "      r: " << resistencia;
        }
        else if(i % n_col_arestas == 4){
            entrada >> reatancia;
            reatancia /= ZB;
            cout << "      reat: " << reatancia << endl;;
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
//    printf("\nsoma fluxos e perdas Ativa:  no(%d)->carga = %.7f", no->id, no->carga);
    double soma =no->carga;
    auxcargasPerdasRamoAtiv(no, soma);
//    printf("\nsoma: %.7f", soma);
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

//        printf("\naux( %d, %d )   aux->perda: %.7f   no(%d)->carga:  %.7f",
//        a->noOrigem->id, a->noDestino->id, a->perda_ativ, a->noDestino->id, a->noDestino->carga);

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
//    cout << "\n\n\n\n\tFOWARD!!!  it: " << it << endl;
    this->listaNos->voltagem = 1.0;///voltagem controlada na estacao
    Auxfoward(this->listaNos, this->listaNos->listaArcos, it);
}

void Grafo::Auxfoward(No *no, Arco *ak, u_int it){
    if(no == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        for(Arco *a=no->getListaArcos(); a!=NULL; a=a->getProxArco()){

            double perda_ativ = 0.0;
            double perda_reat = 0.0;

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

//            printf("\namn( %d , %d )    akm( %d , %d)   Vk(%d)",
//            a->noOrigem->id, a->noDestino->id, ak->noOrigem->id, ak->noDestino->id, no->id);


            ///chute inicial para o fluxo nas arests que partem do no terminal
            if(no==listaNos){
                double carcasPerdasAtivRamo = cargasPerdasRamoAtiv(a->noDestino);
                double carcasPerdasReativRamo = cargasPerdasRamoReAtiv(a->noDestino);

//                cout << " eh:" << carcasPerdasAtivRamo << " + " << a->perda_ativ << " = " << carcasPerdasAtivRamo + a->perda_ativ << endl;

                a->fluxoP_ativ = carcasPerdasAtivRamo + a->perda_ativ;
                a->fluxoP_reativ = carcasPerdasReativRamo + a->perda_reat;

            }else{


                if(it>0){
                    perda_ativ = ak->resistencia*(pow(ak->fluxoP_ativ, 2) + pow(ak->fluxoP_reativ, 2)) / pow(ak->noOrigem->voltagem, 2);
                    perda_reat = ak->reatancia*(pow(ak->fluxoP_ativ, 2) + pow(ak->fluxoP_reativ, 2)) / pow(ak->noOrigem->voltagem, 2);
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
    Auxbackward(this->listaNos);
}

void Grafo::Auxbackward(No *no){
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

//            printf("\nakm( %d , %d )    Vm(%d)    Vk(%d)",
//            a->noOrigem->id, a->noDestino->id, noDest->id, noOrig->id);

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

            Auxbackward(a->getNoDestino());
        }
    }
}

void Grafo::abreFechaChavesGrafo(Solucao *solucao){
//    cout << "abrindo e fechando arcos...";
    ///para cada no com grau de entrada maior que 1
    for(u_int i=0; i<nosEntrada.size(); i++)
    {
//        cout << "No i:" << i << endl;
        ///para cada arco que chega no no
        for(u_int j=0; j<nosEntrada.at(i)->grauEntrada; j++){
            nosEntrada.at(i)->volta.at(j)->chave = solucao->vetChaves[i][j];
        }
    }
//    cout << "feito!" << endl;
}

double Grafo::funcaoObjetivo(Solucao *solucao, double tol){
    this->abreFechaChavesGrafo(solucao);
    this->zeraFluxosPerdas();
    this->calcula_fluxos_e_perdas(tol);
    return calculaPerdaTotal();
}

///construtivo que gera uma arvore aleatoria
Solucao *Grafo::construtivoAleatorio(){
    Solucao *solucao = new Solucao;
    if(this->nosEntrada.size() > 0){
        solucao->vetChaves = new bool*[this->nosEntrada.size()];
        for(u_int i=0; i<this->nosEntrada.size(); i++){
            solucao->vetChaves[i] = new bool[nosEntrada.at(i)->volta.size()];
        }
    }

    ///para cada no de entrada
    for(u_int i=0; i<this->nosEntrada.size(); i++){

        u_int id = rand() % nosEntrada.at(i)->volta.size();

        ///para cada arco que incide no no
        for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++){
            if(j==id)
                solucao->vetChaves[i][j] = true;
            else
                solucao->vetChaves[i][j] = false;
        }
    }

    solucao->valorObjetivo = funcaoObjetivo(solucao, erro_fObjetivo);
    return solucao;
}

bool Grafo::verificaSolucaoValida(Solucao *solucao){
    abreFechaChavesGrafo(solucao);

    desmarcaNos();

    bool ciclo = false;
    u_int marcados = 0;

    auxVerificaSolucaoValida(listaNos, marcados, ciclo);

    if(ciclo==true)
        cout << "\nFECHOU CICLO!!" << endl;
    if(marcados<numeroNos)
        cout << "\nARVORE NAO COBRE TODOS OS NOS!!";

    if(ciclo==false && marcados==numeroNos)
        return true;
    else
        return false;
}

void Grafo::auxVerificaSolucaoValida(No *no, u_int &marcados, bool &ciclo){
    if(no == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        if(no->getMarcado() == false){
//            cout<<"marcando: "<<no->getID()<<endl;
            no->setMarcado(true);
            marcados++;
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

                auxVerificaSolucaoValida(a->getNoDestino(), marcados, ciclo);
            }
        }
        else
            ciclo = true;
    }
}

void Grafo::imprimeChaves(Solucao *solucao){
    for(u_int i=0; i<nosEntrada.size(); i++){
        printf("no(%d) - [ ", nosEntrada.at(i)->id);
        for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++)
            cout << solucao->vetChaves[i][j] << ", ";
        cout << "]\n";
    }
    cout << "\n\n" << endl;
}

void Grafo::mutacao(Solucao *solucao, int taxa){
    int mut = rand() % 100;

    if(mut>=0 && mut<taxa){
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
                solucao->vetChaves[idNo][j] = true;
            else
                solucao->vetChaves[idNo][j] = false;
        }

        solucao->valorObjetivo = funcaoObjetivo(solucao, erro_fObjetivo);

    //    cout << "objetivo:" << solucao->valorObjetivo << endl;
    //    cout << "feito!" << endl;
    }

}

Solucao *Grafo::cruzamentoAleatorio(Solucao *pai1, Solucao *pai2){
    Solucao *filho = new Solucao;
    filho->vetChaves = new bool*[nosEntrada.size()];

    for(u_int i=0; i<nosEntrada.size(); i++)
        filho->vetChaves[i] = new bool[nosEntrada.at(i)->volta.size()];


    for(u_int i=0; i<nosEntrada.size(); i++){
        if((rand() % 2) ==0){
            for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++){
//                cout << pai1->vetChaves[i][j];
                filho->vetChaves[i][j] = pai1->vetChaves[i][j];
            }
        }
        else{
            for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++){
//                cout << pai2->vetChaves[i][j];
                filho->vetChaves[i][j] = pai2->vetChaves[i][j];
            }
        }
    }
    filho->valorObjetivo = funcaoObjetivo(filho, erro_fObjetivo);
    return filho;
}

Solucao *Grafo::cruzamentoCorte(Solucao *pai1, Solucao *pai2){
    Solucao *filho = new Solucao;
    filho->vetChaves = new bool*[nosEntrada.size()];

    for(u_int i=0; i<nosEntrada.size(); i++)
        filho->vetChaves[i] = new bool[nosEntrada.at(i)->volta.size()];

    u_int corte = rand() % nosEntrada.size();
    u_int j = (rand() % 2);

    for(u_int i=0; i<corte; i++){
        if(j==0){
            for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++)
                filho->vetChaves[i][j] = pai1->vetChaves[i][j];
        }
        else{
            for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++)
                filho->vetChaves[i][j] = pai2->vetChaves[i][j];
        }
    }
    for(u_int i=corte; i<nosEntrada.size(); i++){
        if(j==1){
            for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++)
                filho->vetChaves[i][j] = pai1->vetChaves[i][j];
        }
        else{
            for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++)
                filho->vetChaves[i][j] = pai2->vetChaves[i][j];
        }
    }

    filho->valorObjetivo = funcaoObjetivo(filho, erro_fObjetivo);
    return filho;
}

Solucao *Grafo::cruzamentoSuave(Solucao *pai1, Solucao *pai2){


    Solucao *filho = new Solucao;
    filho->vetChaves = new bool*[nosEntrada.size()];

    for(u_int i=0; i<nosEntrada.size(); i++)
        filho->vetChaves[i] = new bool[nosEntrada.at(i)->volta.size()];



    Solucao *melhor, *pior;
    if(pai1->valorObjetivo < pai2->valorObjetivo){
        melhor = pai1;
        pior = pai2;
    }
    else{
        melhor = pai2;
        pior = pai1;
    }




    u_int num_genes_melhor = (u_int)((por_gene_cruzSuave/100.0)*this->nosEntrada.size());
//    cout << "um_genes:" << num_genes_melhor << "   total genes: " << this->nosEntrada.size() << endl;

    for(u_int i=0; i<num_genes_melhor; i++){
        for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++)
                filho->vetChaves[i][j] = melhor->vetChaves[i][j];
    }
    for(u_int i=num_genes_melhor; i<nosEntrada.size(); i++){
        for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++)
            filho->vetChaves[i][j] = pior->vetChaves[i][j];
    }

    filho->valorObjetivo = funcaoObjetivo(filho, erro_fObjetivo);

    return filho;
}

vector<Solucao*> Grafo::populacaoInicialAleatoria(u_int num_individuos){
//    cout << "gerando populacao inicial...";
    vector<Solucao*> populacao;
    for(u_int i=0; i<num_individuos; i++)
        populacao.push_back(construtivoAleatorio());///gera uma populacao inicial inteiramente aleatoria

//    cout << "feito!\n\n" << endl;
    return populacao;
}

vector<Solucao*> Grafo::populacaoInicialBuscaLocal(u_int num_individuos){
    Solucao *const_aleat, *busc_loc;
//    cout << "gerando populacao inicial...";
    vector<Solucao*> populacao;
    for(u_int i=0; i<num_individuos; i++){
        u_int id1 = rand() % nosEntrada.size();
        u_int id2 = rand() % nosEntrada.size();
        u_int id3 = rand() % nosEntrada.size();

        const_aleat = construtivoAleatorio();

        busc_loc = buscaLocal(const_aleat, id1, id2, id3);

        populacao.push_back(busc_loc);///gera uma populacao inicial inteiramente aleatoria

        this->desalocaSolucao(const_aleat);
    }
//    cout << "feito!\n\n" << endl;
    return populacao;
}

vector<Solucao*> Grafo::populacaoInicialCompleta(u_int num_individuos){
    Solucao *const_aleat, *busc_loc;
//    cout << "gerando populacao inicial...";
    vector<Solucao*> populacao;

    ///metade dos individuos gerados por busca local
    for(u_int i=0; i<(u_int)num_individuos/2; i++){
        u_int id1 = rand() % nosEntrada.size();
        u_int id2 = rand() % nosEntrada.size();
        u_int id3 = rand() % nosEntrada.size();

        const_aleat = construtivoAleatorio();

        busc_loc = buscaLocal( const_aleat, id1, id2, id3 );

        populacao.push_back( busc_loc );///gera uma populacao inicial inteiramente aleatoria

        this->desalocaSolucao( const_aleat );

    }

    ///metade dos individuos gerados aleatoriamente
    for(u_int i=(int)num_individuos/2; i<num_individuos-4; i++){
        populacao.push_back( construtivoAleatorio() );
    }

    ///2 individuos gerados pelo construtivo e construtivo inverso
    populacao.push_back( construtivo1(0) );
    populacao.push_back( construtivo1(1) );

    u_int id1, id2, id3;

    ///2 individuos gerados por busca local no construtivo e construtivo inverso

    id1 = rand() % nosEntrada.size();
    id2 = rand() % nosEntrada.size();
    id3 = rand() % nosEntrada.size();
    const_aleat = construtivo1(0);
    populacao.push_back( buscaLocal( const_aleat, id1, id2, id3 ) );
    desalocaSolucao(const_aleat);

    id1 = rand() % nosEntrada.size();
    id2 = rand() % nosEntrada.size();
    id3 = rand() % nosEntrada.size();
    const_aleat = construtivo1(1);
    populacao.push_back( buscaLocal( const_aleat, id1, id2, id3 ) );
    desalocaSolucao(const_aleat);

//    cout << "feito!\n\n" << endl;
    return populacao;
}



bool Grafo::verificaIgualdadeSolucao(Solucao *solucao1, Solucao *solucao2){
    for(u_int i=0; i<nosEntrada.size(); i++){
        for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++){
            if(solucao1->vetChaves[i][j] != solucao2->vetChaves[i][j])
                return false;
        }
    }
    return true;
}

bool Grafo::contido(vector<Solucao*> populacao, Solucao *solucao){
    for(u_int i=0; i<populacao.size(); i++){
        if( verificaIgualdadeSolucao(populacao.at(i), solucao) == true )
            return true;
    }
    return false;
}

///ordena Solucoes de cordo com a funcao objetivo (ordem decrescente, maiores no inicios, menores no fim)
bool melhorObjetivo(Solucao *solucao1, Solucao *solucao2){return solucao1->valorObjetivo > solucao2->valorObjetivo;};

void Grafo::proximaGeracao(vector<Solucao*> &populacao, int taxa_mutacao){

//    cout << "\nmelhor antes: " << melhorIndividuoPopulacao(populacao).valorObjetivo;
//    cout << "\ntamanho populacao antes: " << populacao.size();

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0,1.0);

    int pop_size = populacao.size();

    sort(populacao.begin(), populacao.end(), melhorObjetivo);

    u_int num_piores =(int)((fracPiores)*pop_size);
    u_int num_melhores = (int)((fracMelhores)*pop_size);

    u_int idPai1, idPai2;

    double somaObjetivo = 0;

    for(u_int i=0; i<populacao.size(); i++)
        somaObjetivo+= 1.0 - populacao.at(i)->valorObjetivo;

    ///cruzamentos..
    int num_cruz = pop_size - num_melhores - num_piores;
//    cout << "\ncruzamento: " << num_cruz;
    double corte1;
    double corte2;

    double soma;
    for(int j=0; j<num_cruz; j++){
        corte1 = distribution(generator) * somaObjetivo;
        corte2 = distribution(generator) * somaObjetivo;

        soma = 0;
        for(idPai1 = 0; soma<corte1; idPai1++){
            soma+= 1.0 - populacao.at(idPai1)->valorObjetivo;
        }
        soma = 0;
        for(idPai2 = 0; soma<corte2; idPai2++){
            soma+= 1.0 - populacao.at(idPai2)->valorObjetivo;
        }

        ///Solucao filho = cruzamento_metade(populacao.at(idPai1-1), populacao.at(idPai2-1));
        Solucao *filho;

        int tipoCruz = rand() % 3;

        if(tipoCruz==0)
            filho = cruzamentoAleatorio(populacao.at(idPai1-1), populacao.at(idPai2-1));
        if(tipoCruz==1)
            filho = cruzamentoCorte(populacao.at(idPai1-1), populacao.at(idPai2-1));
        if(tipoCruz==2)
            filho = cruzamentoSuave(populacao.at(idPai1-1), populacao.at(idPai2-1));


        mutacao(filho, taxa_mutacao);

//        cout << "    filho objetivo: " << filho.valorObjetivo;
        populacao.push_back(filho);

//        delete filho;
    }

    for(u_int i=num_piores; i<pop_size-num_melhores; i++){
        this->desalocaSolucao(populacao.at(num_piores));
        populacao.erase(populacao.begin() + num_piores);
    }

}

Solucao *Grafo::melhorIndividuoPopulacao(vector<Solucao*> populacao){
    Solucao *melhor = populacao.at(0);

    for(u_int i=1; i<populacao.size(); i++){
        if( populacao.at(i)->valorObjetivo < melhor->valorObjetivo )
            melhor = populacao.at(i);
    }
    return melhor;
}


Solucao *Grafo::algoritmoGenetico(u_int itSemMelhora, int taxa_mutacao, int taxa_cruzamento, int tipoPopInicial){

//    cout << "\n\n----------ALGORITMO GENETICO----------\n\n";

    vector<Solucao*> populacao;

    if(tipoPopInicial==0)
        populacao = populacaoInicialAleatoria(tam_populacao);

    if(tipoPopInicial==1)
        populacao = populacaoInicialBuscaLocal(tam_populacao);

    if(tipoPopInicial==2)
        populacao = populacaoInicialCompleta(tam_populacao);

    Solucao *melhorIndividuo;

//    cout << "melhor individuo populacao inicial: " << melhorIndividuoPopulacao(populacao).valorObjetivo << endl;

    u_int it = 0;
    while(it <=itSemMelhora){

        proximaGeracao(populacao, taxa_mutacao);

        for(u_int i=0; i<populacao.size(); i++)
            cout << populacao.at(i)->valorObjetivo << endl;

        cout << "\n\n";

        melhorIndividuo = melhorIndividuoPopulacao(populacao);

        it++;

//        cout << "funcao objetivo: " << 100*1000*melhorIndividuo->valorObjetivo << endl;
    }

//    cout << "\nrodou genetico...";

    return melhorIndividuo;
}

double Grafo::tensaoMinima(){
    double tensao_mim = 1.0;
    for(No *no=listaNos; no!=NULL; no=no->proxNo){
        if(no->voltagem < tensao_mim)
            tensao_mim = no->voltagem;
    }
    return tensao_mim;
}

void Grafo::igualaChaves(Solucao *solucao, Solucao *melhor, u_int id_no){
    for(u_int i=0; i<this->nosEntrada.at(id_no)->volta.size(); i++)
        melhor->vetChaves[id_no][i] = solucao->vetChaves[id_no][i];
}

void Grafo::abreChave(Solucao *solucao, u_int id_no, u_int id_arco){
    for(u_int i=0; i<this->nosEntrada.at(id_no)->volta.size(); i++){
        if(i==id_arco)
            solucao->vetChaves[id_no][i] = true;
        else
            solucao->vetChaves[id_no][i] = false;
    }
}

Solucao *Grafo::buscaLocal(Solucao *solucao, u_int id1, u_int id2, u_int id3){

//    printf("\nsolucao vobj: %f    ", 100*1000*solucao.valorObjetivo);

//    solucao.valorObjetivo = funcaoObjetivo(solucao, erro_fObjetivo);
//    printf("\nsolucao vobj: %f    ", 100*1000*solucao.valorObjetivo);


    ///faz uma copia da solucao ------------------------------------------
    Solucao *melhor = new Solucao;
    melhor->vetChaves = new bool*[this->nosEntrada.size()];
    for(u_int i=0; i<this->nosEntrada.size(); i++){
        melhor->vetChaves[i] = new bool[this->nosEntrada.at(i)->volta.size()];
        for(u_int j=0; j<this->nosEntrada.at(i)->volta.size(); j++)
            melhor->vetChaves[i][j] = solucao->vetChaves[i][j];
    }
    melhor->valorObjetivo = solucao->valorObjetivo;
    ///faz uma copia da solucao ------------------------------------------

//    melhor.valorObjetivo = funcaoObjetivo(melhor, erro_fObjetivo);

    for(u_int i=0; i<this->nosEntrada.at(id1)->volta.size(); i++){
        abreChave(solucao, id1, i);

        for(u_int j=0; j<this->nosEntrada.at(id2)->volta.size(); j++){
            abreChave(solucao, id2, j);

            for(u_int k=0; k<this->nosEntrada.at(id3)->volta.size(); k++){
                abreChave(solucao, id3, k);

                solucao->valorObjetivo = funcaoObjetivo(solucao, erro_fObjetivo);

//                cout << "vobj: " << 100*1000*solucao.valorObjetivo << endl;

                if(solucao->valorObjetivo < melhor->valorObjetivo){
                    igualaChaves(solucao, melhor, id1);
                    igualaChaves(solucao, melhor, id2);
                    igualaChaves(solucao, melhor, id3);
                    melhor->valorObjetivo = solucao->valorObjetivo;
                }
            }
        }
    }

//    melhor.valorObjetivo = funcaoObjetivo(melhor, erro_fObjetivo);
//    printf("melhor vobj: %f\n", 100*1000*melhor.valorObjetivo);

    return melhor;
}

void Grafo::zeraFluxosPerdas(){
    for(No *no = listaNos; no!=NULL; no=no->proxNo){
        no->voltagem = 1.0;
        for(Arco *a = no->listaArcos; a!=NULL; a=a->proxArco){
            a->fluxoP_ativ = 0.0;
            a->fluxoP_reativ = 0.0;
            a->perda_ativ = 0.0;
            a->perda_reat = 0.0;
        }
    }
}

Solucao *Grafo::procuraMelhorSolucao(u_int it){

    Solucao *aleatorio = construtivoAleatorio();

    Solucao *solucao = new Solucao;
    solucao->vetChaves = new bool*[nosEntrada.size()];
    for(u_int i=0; i<nosEntrada.size(); i++){
        solucao->vetChaves[i] = new bool[nosEntrada.at(i)->volta.size()];
        for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++)
            solucao->vetChaves[i][j] = aleatorio->vetChaves[i][j];
    }
    solucao->valorObjetivo = aleatorio->valorObjetivo;

    for(u_int i=0; i<it; i++){
        u_int id1 = rand() % nosEntrada.size();
        u_int id2 = rand() % nosEntrada.size();
        u_int id3 = rand() % nosEntrada.size();

        Solucao *aleatorio = buscaLocal(construtivoAleatorio(), id1, id2, id3);


        if(aleatorio->valorObjetivo<solucao->valorObjetivo){
            for(u_int k=0; k<nosEntrada.size(); k++){
                for(u_int u=0; u<nosEntrada.at(k)->volta.size(); u++)
                    solucao->vetChaves[k][u] = aleatorio->vetChaves[k][u];
            }
            solucao->valorObjetivo = aleatorio->valorObjetivo;

            printf("%f\n", 100*1000*solucao->valorObjetivo);
        }
    }
    return solucao;
}

void Grafo::imprimeSolucao(Solucao *solucao){
    for(u_int i=0; i<nosEntrada.size(); i++){
        for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++){
            cout << solucao->vetChaves[i][j] << "  ";
        }
        cout << endl;
    }
}

void Grafo::construtivo1_flux_min(No *no){
    if(no == NULL)
        cout<<"\n No NULL \n"<<endl;
    else{
        for(Arco *a=no->getListaArcos(); a!=NULL; a=a->getProxArco()){

            construtivo1_flux_min(a->getNoDestino());

            double soma_fa_min = 0.0, soma_fr_min = 0.0;
            for(Arco *aux = a->noDestino->listaArcos; aux!=NULL; aux = aux->proxArco){
                soma_fa_min += aux->f_pativ_min;
                soma_fr_min += aux->f_preat_min;
            }

            a->f_pativ_min = a->noDestino->potencia_ativa + soma_fa_min;
            a->f_preat_min = a->noDestino->potencia_reativa + soma_fr_min;

            a->perda_ativ_min = a->resistencia*(pow(a->f_pativ_min, 2) + pow(a->f_preat_min, 2))/pow(a->noDestino->voltagem, 2);
            a->perda_reat_min = a->reatancia*(pow(a->f_pativ_min, 2) + pow(a->f_preat_min, 2))/pow(a->noDestino->voltagem, 2);

        }
    }
}

Solucao *Grafo::construtivo1(bool invertido){
    construtivo1_flux_min(this->listaNos);

    Solucao *solucao = new Solucao;
    solucao->vetChaves = new bool*[this->nosEntrada.size()];

    ///para cada no com grau maior que 1
    for(u_int i=0; i<this->nosEntrada.size(); i++){

        solucao->vetChaves[i] = new bool[this->nosEntrada.at(i)->volta.size()];

        u_int id = 0;
        double perda;
        if(invertido)
            perda = 0.0;
        else
            perda = 1.0;

        ///para cada arco
        for(u_int j=0; j<this->nosEntrada.at(i)->volta.size(); j++){
            if(invertido){
                if(this->nosEntrada.at(i)->volta.at(j)->perda_ativ_min >= perda){
                    perda = this->nosEntrada.at(i)->volta.at(j)->perda_ativ_min;
                    id = j;
                }
            }
            else{
                if(this->nosEntrada.at(i)->volta.at(j)->perda_ativ_min < perda){
                    perda = this->nosEntrada.at(i)->volta.at(j)->perda_ativ_min;
                    id = j;
                }
            }

        }
        for(u_int j=0; j<this->nosEntrada.at(i)->volta.size(); j++){
            if(j==id)
                solucao->vetChaves[i][j] = true;
            else
                solucao->vetChaves[i][j] = false;
        }
    }

    solucao->valorObjetivo = this->funcaoObjetivo(solucao, erro_fObjetivo);

    return solucao;
}

void Grafo::desalocaSolucao(Solucao *solucao){
//    cout << solucao;
    for(u_int i=0; i<this->nosEntrada.size(); i++){
        delete[] solucao->vetChaves[i];
    }

    delete[] solucao->vetChaves;

    delete solucao;
}

Solucao *Grafo::copiaSolucao(Solucao *solucao){
    Solucao *copia = new Solucao;

    copia->vetChaves = new bool*[this->nosEntrada.size()];

    for(u_int i=0; i<nosEntrada.size(); i++){

        copia->vetChaves[i] = new bool[nosEntrada.at(i)->volta.size()];

        for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++){
            copia->vetChaves[i][j] = solucao->vetChaves[i][j];
        }
    }

    copia->valorObjetivo = solucao->valorObjetivo;

    return copia;
}

void Grafo::salvaChavesAbertas(Solucao *s, char *nomeArqSaida){

    ofstream saida;
    saida.open(nomeArqSaida);

    if(verificaSolucaoValida(s)){
        for(u_int i=0; i<nosEntrada.size(); i++){
            for(u_int j=0; j<nosEntrada.at(i)->volta.size(); j++){
                if(nosEntrada.at(i)->volta.at(j)->chave == false)
                    saida << "( " << nosEntrada.at(i)->volta.at(j)->noOrigem->id <<
                            " , " << nosEntrada.at(i)->volta.at(j)->noDestino->id << ") ["
                                  << nosEntrada.at(i)->volta.at(j)->chave << "]\n";
            }
        }
        saida << "tensao minima:  " << tensaoMinima();
        saida << "\nvalor objetivo:  " << 100*1000*s->valorObjetivo << "\n";
    }
    else
        saida << "solucao invalida!";

    saida.close();
}

///agora vai!--------------tem que ir

///FALTA DESTRUTOR!
Grafo::~Grafo(){}
