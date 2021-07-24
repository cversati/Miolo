#include "cliente.h"
#include "RNA.h"


cliente::cliente()
{
    //ctor
}

cliente::~cliente()
{
    //dtor
}

float cliente::reacao( void )
{
    return sinapse(rede);
}
