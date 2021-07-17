package main

import (
//  "fmt"
)

type enumStatusNeuronio int
const (
  Inativo = iota //0
  Ativo
)

type enumCamadaNeuronio int
const (
  Entrada = iota // 0
  Intermediaria1
  Final = 9
)

type enumFuncaoAtivacao int
const (
  rnaFA_um = iota  //td ou nada
  rnaFA_sempre //qlqr caso
  rnaFA_entre //qlqr menos 1 e 0
  rnaFA_zero //somente se 0
)

type enumFuncaoProcessamento int
const (
  rnaFP_MediaSimples = 1
  rnaFP_MediaPonderada = 2
  rnaFP_Somatoria = 3
  rnaFP_Conjucao = 4
  rnaFP_Disjucao = 5
  rnaFP_Condicional = 6
  rnaFP_Bicodicional = 7
  rnaFP_XOR = 8
  rnaFP_NOR = 9
)


/*Pontuacao:
0.rnaPontua_ALS   PontuaAcimaLimiteSuperior (Sobejo superior)
1.rnaPontua_ELSTS Entre LimiteSuperior e Threshold superior (entre sobejo e limiar)
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
type enumCriterio int
const (
    rnaPontua_ALS enumCriterio = iota // 0
    rnaPontua_ELSTS
    rnaPontua_ELSVR
    rnaPontua_VR
    rnaPontua_EVRTI
    rnaPontua_ETILI
    rnaPontua_ALI
    rnaPontua_ALSLI
    rnaPontua_ETSTI
    rnaPontua_ELSLI
    rnaPontua_FTSTI
    rnaPontua_NVR
    rnaPontua_ELITS
)


type segmentosCurva struct {
    limiteInferior float64
    limiarInferior float64
    valorReferencia float64
    limiarSuperior float64
    limiteSuperior float64
}

type structInput struct {
    valor float64
    idxNeuronio int32 //id do neuronio de origem da informacao, se zero  = inativo
}

type structGrafo struct {
    id_neuro_dst int32
    id_neuro_orig int32
    valor float64
}

type structNeuronio struct {
  id int32
  camada enumCamadaNeuronio
  criterio enumCriterio //area de sucesso na curva
  limiteInferior float64
  limiarInferior float64
  valorReferencia float64
  limiarSuperior float64
  limiteSuperior float64
  inputs [2]structInput
  funcaoCondensacao enumFuncaoProcessamento //Usada na entrada do neuronio intermediario e final
  funcaoAtivacao enumFuncaoAtivacao //a pensar: funcao de ativacao: o valor so passa para a proxima camada se chegar ate um valor
  peso float64
  output float64
  status enumStatusNeuronio
}


/*
1-acima do limite superior
2-no limite superior
3-no limiar superior
4-no valor padrao
5-no limiar inferior
6-no limite inferior
7-abaixo do limite inferior
*/


func GetPosicaoLinear ( ponto float64, segmentos segmentosCurva ) int {

  if ( ponto > segmentos.limiteSuperior) {
     return 1
  }

  if ( ( ponto <= segmentos.limiteSuperior ) && ( ponto > segmentos.limiarSuperior) ) {
     return 2
  }

  if ( ( ponto <= segmentos.limiarSuperior ) && ( ponto > segmentos.valorReferencia ) ) {
    return 3
  }

  if ( ponto == segmentos.valorReferencia ) {
    return 4
  }

  if ( ( ponto >= segmentos.limiarInferior ) && ( ponto < segmentos.valorReferencia ) ) {
    return 5
  }

  if ( ( ponto < segmentos.limiarInferior ) && ( ponto >= segmentos.limiteInferior ) ) {
    return 6
  }

  if ( ponto < segmentos.limiteInferior ) {
    return 7
  }

  return 0

 }


 //    lim sup    threshold      vr      threshold          lim inf
 //  ----o------------o----------o------------o--------------o-------
 //   1       2           3      4      5           6            7
func isAtendeCriterio ( pPosicaoLinear int, pCriterio enumCriterio ) int {
      atende := 0
      switch (pPosicaoLinear) {
      case 1:
          if ( ( pCriterio == rnaPontua_ALS ) || ( pCriterio == rnaPontua_ALSLI ) || ( pCriterio == rnaPontua_FTSTI ) || ( pCriterio == rnaPontua_NVR ) ) {
            atende = 1
          }
      case 2:
          if ( ( pCriterio == rnaPontua_ELSTS ) || ( pCriterio == rnaPontua_ALSLI ) || ( pCriterio == rnaPontua_ELSLI ) || ( pCriterio == rnaPontua_FTSTI ) || ( pCriterio == rnaPontua_NVR ) ) {
            atende = 1
          }
      case 3:
          if ( ( pCriterio == rnaPontua_ELSVR ) || ( pCriterio == rnaPontua_ALSLI ) || ( pCriterio == rnaPontua_ETSTI ) || ( pCriterio == rnaPontua_ELSLI ) || ( pCriterio == rnaPontua_NVR ) || ( pCriterio == rnaPontua_ELITS )) {
            atende = 1
          }
      case 4:
          if ( ( pCriterio == rnaPontua_VR ) || ( pCriterio == rnaPontua_ALSLI ) || ( pCriterio == rnaPontua_ETSTI ) || ( pCriterio == rnaPontua_ELSLI ) || ( pCriterio == rnaPontua_ELITS ) ) {
            atende = 1
          }
      case 5:
          if ( ( pCriterio == rnaPontua_EVRTI ) || ( pCriterio == rnaPontua_ALSLI ) || ( pCriterio == rnaPontua_ETSTI ) || ( pCriterio == rnaPontua_ELSLI ) || ( pCriterio == rnaPontua_NVR ) || ( pCriterio == rnaPontua_ELITS )) {
            atende = 1
          }
      case 6:
          if ( ( pCriterio == rnaPontua_ETILI ) || ( pCriterio == rnaPontua_ALSLI ) || ( pCriterio == rnaPontua_FTSTI ) || ( pCriterio == rnaPontua_NVR ) || ( pCriterio == rnaPontua_ELITS ) ) {
            atende = 1
          }
      case 7:
          if ( ( pCriterio == rnaPontua_ALI ) || ( pCriterio == rnaPontua_FTSTI ) || ( pCriterio == rnaPontua_NVR ) ) {
            atende = 1
          }
      }

      return atende
  }

/*
  //Devolve a posicao do neuronio na matriz baseado no seu id. Se nao encontrar devolve -1
  long int getIdxVectorByIdxNeuronio ( unsigned long int idx, std::vector<structNeuronio> rede )
  {
      int n = rede.size();
      while (n) if ( rede.at(--n).id == idx ) return n;
      return -1;
  }
*/
