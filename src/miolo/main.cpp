/*
contador numeradores de logs (l=log informativo s=sql e=msg erros
l 19
s 8
e 17
esses sao os numeros da ultima msg (usar o proximo nr)
*/

#include <iostream>
#include "RNA.h"
#include "Neuronio.h"
#include "pd_pgsql.h"
#include "cliente.h"

using namespace std;

cliente *cliente1;

int main()
{
    cout << "-------------- Miolo --------------" << endl << endl;

    calibrarTodos(1);

    updateVariaveisAmbiente();

    cliente1 = new cliente();
    carrega_rna(&cliente1->rede,1);
    updateOutput(1, cliente1->reacao());

    cout << "--------------- fim ---------------" << endl;
     return 1;
}
