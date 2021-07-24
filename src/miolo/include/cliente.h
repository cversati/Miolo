#ifndef CLIENTE_H
#define CLIENTE_H

#include <vector>
#include "global.h"

class cliente
{
    public:
        cliente();
        virtual ~cliente();
        float reacao ( void );
        std::vector<structNeuronio> rede;

    protected:

    private:
        unsigned long int id;
        const char* nome;

};

#endif // CLIENTE_H
