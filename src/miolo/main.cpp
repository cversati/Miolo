#include <iostream>
#include "RNA.h"
#include "Neuronio.h"
#include "pd_pgsql.h"
#include "cliente.h"

using namespace std;

cliente *cliente1;

int main()
{
    cout << "Miolo" << endl;
    updateVariaveisAmbiente();

    cliente1 = new cliente();
    carrega_rna(&cliente1->rede,1);
    updateOutput(1, cliente1->reacao());

    cout << "Fim" << endl;
    return 1;
}
