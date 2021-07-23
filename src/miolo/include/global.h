#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

//ref neuronio
const unsigned short int NR_INPUTS = 3;
enum enumStatusNeuronio { Ativo = 1, Inativo = 0};
enum enumCamadaNeuronio { Entrada = 0, Intermediaria1 = 1, Final = 9};
enum enumFuncaoAtivacao {rnaFA_um, rnaFA_sempre, rnaFA_entre, rnaFA_zero};
//                     td ou nada / qlqr caso  /qlqr menos 1 e 0 / somente se 0
enum enumFuncaoProcessamento {rnaFP_MediaSimples = 1, rnaFP_MediaPonderada = 2, rnaFP_Somatoria = 3, rnaFP_Conjucao = 4, rnaFP_Disjucao = 5, rnaFP_Condicional = 6, rnaFP_Bicodicional = 7, rnaFP_XOR = 8, rnaFP_NOR = 9 };
//                                                                                                  111/101/011/000     111/100/010/000     111/100/011/001         111/100/010/001        110/101/011/000 110/100/010/001
/*Pontuacao:
0.rnaPontua_ALS   PontuaAcimaLimiteSuperior
1.rnaPontua_ELSTS Entre LimiteSuperior e Threshold superior
2.rnaPontua_ELSVR Entre Threshold Superior e Valor Referencia
3.rnaPontua_VR    No valor de referencia
4.rnaPontua_EVRTI Entre Threshold Inferior e Valor Referencia
5.rnaPontua_ETILI  Entre LimiteInferior e Threshold inferior
6.rnaPontua_ALI   PontuaAbaixoLimiteInferior
7.rnaPontua_ALSLI Pontua acima do limites superior até o limite inferior
8.rnaPontua_ETSTI  Pontua entre o Threshold superior e o inferior
9.rnaPontua_ELSLI  Pontua entre o limite superior e o inferior
10.rnaPontua_FTSTI Pontua fora dos Threshold superior e o inferior
11.rnaPontua_NVR    Pontua quando nao eh o valor VR
12.rnaPontua_ELITS Pontua entre limite inferior e Threshold superior (util em curva logaritmica)*/
enum enumCriterio {rnaPontua_ALS,rnaPontua_ELSTS,rnaPontua_ELSVR,rnaPontua_VR,rnaPontua_EVRTI,rnaPontua_ETILI,rnaPontua_ALI,rnaPontua_ALSLI,rnaPontua_ETSTI,rnaPontua_ELSLI,rnaPontua_FTSTI,rnaPontua_NVR,rnaPontua_ELITS};
struct segmentosCurva
{
    float limiteInferior;
    float limiarInferior;
    float valorReferencia;
    float limiarSuperior;
    float limiteSuperior;
};
struct structInput
{
    float valor;
    short int idxNeuronio; //id do neuronio de origem da informacao, se zero  = inativo
};
struct structNeuronio
{
    unsigned long int id;
    unsigned long int id_rna;
    enumCamadaNeuronio camada;
    enumCriterio criterio; //area de sucesso na curva
    segmentosCurva segmentos; //valores da curva
    float valorRecebido;
    structInput inputs[NR_INPUTS];
    enumFuncaoProcessamento funcaoCondensacao; //Usada na entrada do neuronio intermediario e final
    enumFuncaoAtivacao funcaoAtivacao;//a pensar: funcao de ativacao: o valor so passa para a proxima camada se chegar ate um valor
    float peso;
    float output;
    enumStatusNeuronio status;
};
#endif // GLOBAL_H_INCLUDED
