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


    double fluxoP_ativ, fluxoP_reativ;
    ///rextricoes de fluxoP_ativ
    double lim_inf, lim_sup;
    ///resistencia do arco
    double resistencia, reatancia;
    double perda_ativ, perda_reat;

    Arco(u_int id){
        this->id=id;
        this->proxArco=0;
        this->noDestino=0;
        this->fluxoP_ativ=0.0;
        this->fluxoP_reativ=0.0;
        this->perda_ativ = this->perda_reat = 0.0;
    };

    double getfluxoP_ativ(){   return fluxoP_ativ;    };
    void setfluxoP_ativ(double fluxoP_ativ){  this->fluxoP_ativ = fluxoP_ativ;};

    u_int getID(){   return this->id;    };

    Arco *getProxArco(){    return this->proxArco;  };
    void setProxArco(Arco *arc){    this->proxArco=arc; };

    No *getNoDestino(){ return this->noDestino; };
    void setNoDestino(No *no){  this->noDestino=no; };

    No *getNoOrigem(){ return this->noOrigem; };
    void setNoOrigem(No *no){  this->noOrigem=no; };

    void imprime(){
        printf(" --|A%2d[%d]: r: %2.7f : reat: %2.7f : fat %2.7f : freat %2.7f: perda_at %2.7f :|--> (%d, %d) ", id, chave, resistencia, reatancia, fluxoP_ativ, fluxoP_reativ, perda_ativ, noOrigem->getID(), noDestino->getID());
    }


    ~Arco(){};
};

#endif // ARCO_H_INCLUDED
