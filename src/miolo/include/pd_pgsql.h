#ifndef PD_PGSQL_H
#define PD_PGSQL_H

#include <vector>
#include "global.h"
int updateVariaveisAmbiente(void);
int updateOutput(unsigned int id, float valor);
int carrega_rna(std::vector<structNeuronio> *rede, unsigned short int id_rede);
#endif // PD_PGSQL_H
