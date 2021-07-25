#include <iostream>
#include "RNA.h"
#include "global.h"

using namespace std;

/*
1-acima do limite superior
2-no limite superior
3-no limiar superior
4-no valor padrao
5-no limiar inferior
6-no limite inferior
7-abaixo do limite inferior
*/
int getPosicaoLinear ( float ponto, segmentosCurva segmentos )
 {
	 if ( ponto > segmentos.limiteSuperior) return 1;
     if ( ( ponto <= segmentos.limiteSuperior ) && ( ponto > segmentos.limiarSuperior) ) return 2;
     if ( ( ponto <= segmentos.limiarSuperior ) && ( ponto > segmentos.valorReferencia ) ) return 3;
     if ( ponto == segmentos.valorReferencia ) return 4;
     if ( ( ponto >= segmentos.limiarInferior ) && ( ponto < segmentos.valorReferencia ) ) return 5;
     if ( ( ponto < segmentos.limiarInferior ) && ( ponto >= segmentos.limiteInferior ) ) return 6;
     if ( ponto < segmentos.limiteInferior ) return 7;
     return 0;
 }
/*--------------------------------------------------------------------
Retorna 1 se o valor esta numa posicao na curva que atende o criterio

    lim sup    threshold      vr      threshold          lim inf
  ----o------------o----------o------------o--------------o-------
   1       2           3      4      5           6            7
---------------------------------------------------------------------*/
 int isAtendeCriterio ( int pPosicaoLinear, enumCriterio pCriterio )
 {
     int atende=0;
     switch (pPosicaoLinear)
     {
         case 1:
             if ( ( pCriterio == rnaPontua_ALS ) || ( pCriterio == rnaPontua_ALSLI ) || ( pCriterio == rnaPontua_FTSTI ) || ( pCriterio == rnaPontua_NVR ) ) atende=1;
             break;
         case 2:
             if ( ( pCriterio == rnaPontua_ELSTS ) || ( pCriterio == rnaPontua_ALSLI ) || ( pCriterio == rnaPontua_ELSLI ) || ( pCriterio == rnaPontua_FTSTI ) || ( pCriterio == rnaPontua_NVR ) ) atende=1;
             break;
         case 3:
             if ( ( pCriterio == rnaPontua_ELSVR ) || ( pCriterio == rnaPontua_ALSLI ) || ( pCriterio == rnaPontua_ETSTI ) || ( pCriterio == rnaPontua_ELSLI ) || ( pCriterio == rnaPontua_NVR ) || ( pCriterio == rnaPontua_ELITS )) atende=1;
             break;
         case 4:
             if ( ( pCriterio == rnaPontua_VR ) || ( pCriterio == rnaPontua_ALSLI ) || ( pCriterio == rnaPontua_ETSTI ) || ( pCriterio == rnaPontua_ELSLI ) || ( pCriterio == rnaPontua_ELITS ) ) atende=1;
             break;
         case 5:
             if ( ( pCriterio == rnaPontua_EVRTI ) || ( pCriterio == rnaPontua_ALSLI ) || ( pCriterio == rnaPontua_ETSTI ) || ( pCriterio == rnaPontua_ELSLI ) || ( pCriterio == rnaPontua_NVR ) || ( pCriterio == rnaPontua_ELITS )) atende=1;
             break;
         case 6:
             if ( ( pCriterio == rnaPontua_ETILI ) || ( pCriterio == rnaPontua_ALSLI ) || ( pCriterio == rnaPontua_FTSTI ) || ( pCriterio == rnaPontua_NVR ) || ( pCriterio == rnaPontua_ELITS ) ) atende=1;
             break;
         case 7:
             if ( ( pCriterio == rnaPontua_ALI ) || ( pCriterio == rnaPontua_FTSTI ) || ( pCriterio == rnaPontua_NVR ) ) atende=1;
             break;
     }
     return atende;
 }

//Devolve a posicao do neuronio na matriz baseado no seu id. Se nao encontrar devolve -1
long int getIdxVectorByIdxNeuronio ( unsigned long int idx, std::vector<structNeuronio> rede )
{
    int n = rede.size();
    while (n) if ( rede.at(--n).id == idx ) return n;
    return -1;
}

void processaCamadaIntermediaria(std::vector<structNeuronio> *rede, enumCamadaNeuronio camada)
{
    unsigned short int k = NR_INPUTS;
    float ttl_outputs = 0;
    unsigned short int qtde_outputs = 0;
    unsigned short int idx_vector=0;
    int n = rede->size();
    int proposicao1 = 0;
    int proposicao2 = 0;
    while (n--)
    {
        if ( ( rede->at(n).status == Ativo ) && ( rede->at(n).camada == camada ) ) //pega o neuronio daquela camada intermediaria
        {
            if (RNA_LOG) cout << "l#012 " << "processa a camada #" << camada << endl;
            ttl_outputs = 0;
            qtde_outputs = 0;
            k = NR_INPUTS;
            if (RNA_LOG) cout << "neuronio intermediario " << rede->at(n).id << endl;
            switch ( rede->at(n).funcaoCondensacao )
            {
            case rnaFP_MediaSimples:
               while (k--)
               {
                    if (rede->at(n).inputs[k].id_neuro_orig > 0 )
                    {
                        idx_vector=getIdxVectorByIdxNeuronio(rede->at(n).inputs[k].id_neuro_dst,*rede);
                        if (RNA_LOG) cout << "l#013 " << "output do neuronio " << rede->at(n).inputs[k].id_neuro_orig << " : "  << rede->at(idx_vector).output << endl;
                        ttl_outputs += rede->at(idx_vector).output;
                        qtde_outputs++;
                    }
                }
                ttl_outputs = ( (qtde_outputs) ? ttl_outputs/qtde_outputs : 0 ); //recurso para nao dar divisao por zero
                rede->at(n).output = ( (ttl_outputs>1) ? 1 : ttl_outputs ); //recurso para nao dar divisao por zero
                if (RNA_LOG) cout << "l#014 " << "saida do intermediario : " << rede->at(n).output << endl;
                break;
            case rnaFP_MediaPonderada:
                while (k--)
                {
                    if (rede->at(n).inputs[k].id_neuro_orig > 0 )
                    {
                        idx_vector=getIdxVectorByIdxNeuronio(rede->at(n).inputs[k].id_neuro_dst,*rede);
                        if (RNA_LOG) cout << "l#015 " << "output do neuronio " << rede->at(n).inputs[k].id_neuro_orig << " : "  << rede->at(idx_vector).output << endl;
                        ttl_outputs += rede->at(idx_vector).output * rede->at(idx_vector).peso;
                        qtde_outputs += rede->at(idx_vector).peso;
                    }
                }
                ttl_outputs = ( (qtde_outputs) ? ttl_outputs/qtde_outputs : 0 ); //recurso para nao dar divisao por zero
                rede->at(n).output = ( (ttl_outputs>1) ? 1 : ttl_outputs ); //recurso para nao dar divisao por zero
                if (RNA_LOG) cout << "saida do intermediario : " << rede->at(n).output << endl;
                break;
            case rnaFP_Somatoria:
                while (k--)
                {
                    if (rede->at(n).inputs[k].id_neuro_orig > 0 )
                    {
                        idx_vector=getIdxVectorByIdxNeuronio(rede->at(n).inputs[k].id_neuro_dst,*rede);
                        if (RNA_LOG) cout << "l#016 " << "output do neuronio " << rede->at(n).inputs[k].id_neuro_orig << " : "  << rede->at(idx_vector).output << endl;
                        ttl_outputs += rede->at(idx_vector).output;
                    }
                }
                rede->at(n).output = ( (ttl_outputs>1) ? 1 : ttl_outputs ); //recurso para nao dar divisao por zero
                if (RNA_LOG) cout << "l#017 " << "saida do intermediario : " << rede->at(n).output << endl;
                break;
            case rnaFP_Conjucao:
                proposicao1 = rede->at(getIdxVectorByIdxNeuronio(rede->at(n).inputs[0].id_neuro_dst,*rede)).output;
                proposicao2 = rede->at(getIdxVectorByIdxNeuronio(rede->at(n).inputs[1].id_neuro_dst,*rede)).output;
                if (RNA_LOG) cout << "output do neuronio " << proposicao1 << " : "  << proposicao2 << endl;
                if ( ( proposicao1 ) && ( proposicao2 ) ) rede->at(n).output = 1;
                if ( ( proposicao1 ) && ( !proposicao2 ) ) rede->at(n).output = 1;
                if ( ( !proposicao1 ) && ( proposicao2 ) ) rede->at(n).output = 1;
                if ( ( !proposicao1 ) && ( !proposicao2 ) ) rede->at(n).output = 0;
                break;
            case rnaFP_Disjucao:
                proposicao1 = rede->at(getIdxVectorByIdxNeuronio(rede->at(n).inputs[0].id_neuro_dst,*rede)).output;
                proposicao2 = rede->at(getIdxVectorByIdxNeuronio(rede->at(n).inputs[1].id_neuro_dst,*rede)).output;
                if (RNA_LOG) cout << "output do neuronio " << proposicao1 << " : "  << proposicao2 << endl;
                if ( ( proposicao1 ) && ( proposicao2 ) ) rede->at(n).output = 1;
                if ( ( proposicao1 ) && ( !proposicao2 ) ) rede->at(n).output = 0;
                if ( ( !proposicao1 ) && ( proposicao2 ) ) rede->at(n).output = 0;
                if ( ( !proposicao1 ) && ( !proposicao2 ) ) rede->at(n).output = 0;
                break;
            case rnaFP_Condicional:
                proposicao1 = rede->at(getIdxVectorByIdxNeuronio(rede->at(n).inputs[0].id_neuro_dst,*rede)).output;
                proposicao2 = rede->at(getIdxVectorByIdxNeuronio(rede->at(n).inputs[1].id_neuro_dst,*rede)).output;
                if (RNA_LOG) cout << "output do neuronio " << proposicao1 << " : "  << proposicao2 << endl;
                if ( ( proposicao1 ) && ( proposicao2 ) ) rede->at(n).output = 1;
                if ( ( proposicao1 ) && ( !proposicao2 ) ) rede->at(n).output = 0;
                if ( ( !proposicao1 ) && ( proposicao2 ) ) rede->at(n).output = 1;
                if ( ( !proposicao1 ) && ( !proposicao2 ) ) rede->at(n).output = 1;
                break;
            case rnaFP_Bicodicional:
                proposicao1 = rede->at(getIdxVectorByIdxNeuronio(rede->at(n).inputs[0].id_neuro_dst,*rede)).output;
                proposicao2 = rede->at(getIdxVectorByIdxNeuronio(rede->at(n).inputs[1].id_neuro_dst,*rede)).output;
                if (RNA_LOG) cout << "output do neuronio " << proposicao1 << " : "  << proposicao2 << endl;
                if ( ( proposicao1 ) && ( proposicao2 ) ) rede->at(n).output = 1;
                if ( ( proposicao1 ) && ( !proposicao2 ) ) rede->at(n).output = 0;
                if ( ( !proposicao1 ) && ( proposicao2 ) ) rede->at(n).output = 0;
                if ( ( !proposicao1 ) && ( !proposicao2 ) ) rede->at(n).output = 1;
                break;
            case rnaFP_XOR:
                proposicao1 = rede->at(getIdxVectorByIdxNeuronio(rede->at(n).inputs[0].id_neuro_dst,*rede)).output;
                proposicao2 = rede->at(getIdxVectorByIdxNeuronio(rede->at(n).inputs[1].id_neuro_dst,*rede)).output;
                if (RNA_LOG) cout << "output do neuronio " << proposicao1 << " : "  << proposicao2 << endl;
                if ( ( proposicao1 ) && ( proposicao2 ) ) rede->at(n).output = 0;
                if ( ( proposicao1 ) && ( !proposicao2 ) ) rede->at(n).output = 1;
                if ( ( !proposicao1 ) && ( proposicao2 ) ) rede->at(n).output = 1;
                if ( ( !proposicao1 ) && ( !proposicao2 ) ) rede->at(n).output = 0;
                break;
            case rnaFP_NOR:
                proposicao1 = rede->at(getIdxVectorByIdxNeuronio(rede->at(n).inputs[0].id_neuro_dst,*rede)).output;
                proposicao2 = rede->at(getIdxVectorByIdxNeuronio(rede->at(n).inputs[1].id_neuro_dst,*rede)).output;
                if (RNA_LOG) cout << "output do neuronio " << proposicao1 << " : "  << proposicao2 << endl;
                if ( ( proposicao1 ) && ( proposicao2 ) ) rede->at(n).output = 0;
                if ( ( proposicao1 ) && ( !proposicao2 ) ) rede->at(n).output = 0;
                if ( ( !proposicao1 ) && ( proposicao2 ) ) rede->at(n).output = 0;
                if ( ( !proposicao1 ) && ( !proposicao2 ) ) rede->at(n).output = 1;
                break;
            }
        }
    }
}

float sinapse(std::vector<structNeuronio> rede)
{
    if (RNA_LOG) cout << endl << "l#009 " << "iniciando sinapse" << endl;

    unsigned short int k = NR_INPUTS;
    float ttl_outputs = 0;
    unsigned short int qtde_outputs = 0;

    //calcula output dos neuronios de entrada

    unsigned int n = rede.size();

    if (!n)
    {
        cerr << "e#013 rede vazia" << endl;
        return 0;
    }

    if (RNA_LOG) cout << "l#010 " << "rede " << rede.at(0).id_rna << " formada por " << n << " neuronios em processamento" << endl ;

    if (RNA_LOG) cout << "l#011 " << "processando camada #0 (entrada)" << endl;

    while (n--)
    {
        if ( ( rede.at(n).status == Ativo ) && (rede.at(n).camada == Entrada ) )
        {
            rede.at(n).output = (isAtendeCriterio( getPosicaoLinear(rede.at(n).inputs[0].valor, rede.at(n).segmentos), rede.at(n).criterio ) ? rede.at(n).peso : 0 );
            if (RNA_LOG) cout << "l#003 " << "neuronio " << rede.at(n).id << " com input " << rede.at(n).inputs[0].valor;
            if (RNA_LOG) cout << " na posicao linear " << getPosicaoLinear(rede.at(n).inputs[0].valor, rede.at(n).segmentos);
            if (RNA_LOG) cout << " e peso " << rede.at(n).peso;
            if (RNA_LOG) cout << " e " << (isAtendeCriterio( getPosicaoLinear(rede.at(n).inputs[0].valor, rede.at(n).segmentos), rede.at(n).criterio ) ? "" : "nao" ) << " atende o criterio";
            if (RNA_LOG) cout << " resultado passado para a proxima camada: " << rede.at(n).output << endl;

        }
    }

 //processa intermediarios
    for (int i=1; i<9; i++) processaCamadaIntermediaria(&rede, (enumCamadaNeuronio) i);

 //calcular saida
    if (RNA_LOG) cout << "l#018 " << "calcula saida" << endl;

    n = rede.size();
    while (n--)
    {
        if ( ( rede.at(n).status == Ativo ) && (rede.at(n).camada == Final ) )
        {
        k = NR_INPUTS;
        ttl_outputs = 0;
        qtde_outputs = 0;
        if (RNA_LOG) cout << "neuronio final " <<  rede.at(n).id << endl;
        while (k--)
            {
                if (rede.at(n).inputs[k].id_neuro_dst )
                {
                    if (RNA_LOG) cout << "output do neuronio " << rede.at(n).inputs[k].id_neuro_dst << " : " << rede.at(getIdxVectorByIdxNeuronio(rede.at(n).inputs[k].id_neuro_dst,rede)).output << endl;
                    ttl_outputs += rede.at(getIdxVectorByIdxNeuronio(rede.at(n).inputs[k].id_neuro_dst,rede)).output;
                    qtde_outputs++;
                }
            }
        rede.at(n).output = ( (qtde_outputs) ? ttl_outputs/qtde_outputs : 0 ); //recurso para nao dar divisao por zero
        ttl_outputs = rede.at(n).output; //TODO estou reaproveitando a variavel, pq nao gostei dessa final
        cout << endl << "Final: " << ttl_outputs << endl;
        }
    }

    return ttl_outputs;
}
