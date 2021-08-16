#ifndef RNA_H
#define RNA_H
#include <vector>
#include "global.h"
long int getIdxVectorByIdxNeuronio ( unsigned long int idx, std::vector<structNeuronio> rede );
float sinapse(std::vector<structNeuronio> rede);
float formataSaidaNeuronio(enumFuncaoAtivacao, float);
char* getStringPosicaoLinear ( int );
#endif // RNA_H
