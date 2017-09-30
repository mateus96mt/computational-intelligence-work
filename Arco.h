#ifndef ARCO_H_INCLUDED
#define ARCO_H_INCLUDED
#include <stdio.h>
#include <iostream>
#include "No.h"

typedef unsigned int u_int;
class Arco
{
public:
    u_int id;
    Arco *proxArco;
    No *noOrigem, *noDestino;


    bool chave;
    double fluxo;
    ///rextricoes de fluxo
    double lim_inf, lim_sup;
    ///resistencia do arco
    double resistencia;

    Arco(u_int id){
        this->id=id;
        this->proxArco=0;
        this->noDestino=0;
        this->fluxo=0;
    };

    double getfluxo(){   return fluxo;    };
    void setfluxo(double fluxo){  this->fluxo = fluxo;};

    u_int getID(){   return this->id;    };

    Arco *getProxArco(){    return this->proxArco;  };
    void setProxArco(Arco *arc){    this->proxArco=arc; };

    No *getNoDestino(){ return this->noDestino; };
    void setNoDestino(No *no){  this->noDestino=no; };

    No *getNoOrigem(){ return this->noOrigem; };
    void setNoOrigem(No *no){  this->noOrigem=no; };

    void imprime(){
        printf(" --|A%2d: r: %2.5f : f %2.1f:|--> (%d, %d) ", id, resistencia, fluxo, noOrigem->getID(), noDestino->getID());
    }


    ~Arco(){};
};

#endif // ARCO_H_INCLUDED
