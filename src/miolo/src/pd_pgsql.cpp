#include <iostream>
#include <vector>
#include <libpq-fe.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <libpq-fe.h>
#include <errno.h>

#include "pd_pgsql.h"
#include "global.h"
#include "RNA.h"

using namespace std;

const char *conninfo =
"host=localhost " \
"dbname=rna " \
"user=miolo " \
"password=miolo";

/*
 * Connect, disconnect and error checking example
 *
 * Copyright (C) 2002, Robert Vojta
 *
 * Robert Vojta <robert@v0jta.net>
 *
 * This program is free software, distributed under the terms of
 * the GNU General Public License
 *
 */


/*
 * Function for (re)connection polling ...
 *
 * conn - is returned pointer by PQconnectStart() or current connection info
 * reset - 0 - new connection (use PQconnectPoll)
 *         1 - reseting (use PQresetPoll)
 *
 * return:  0 - (re)connected OK
 *         -1 - failed to connect
 */
int poll_for_connection(PGconn *conn, int reset)
{
	PostgresPollingStatusType status;

	fd_set fd;
	int sock;

	/* check for conn status */
	if (PQstatus(conn) == CONNECTION_BAD)
	{
		fprintf(stderr, "Asynchronous connection failed...\n");
		return -1;
	}

	/* get socket for select() */
	sock = PQsocket(conn);

	/* print correct label */
	if (reset)
		printf("Reconnecting: ");
//	else
//		printf("Connecting: ");

	/* poll (re)connection */
	do
	{
		/* determine which function we should use for Polling */
		if (reset)
			status = PQresetPoll(conn);
		else
			status = PQconnectPoll(conn);

		/* next action based upon status value */
		switch (status)
		{
			case PGRES_POLLING_READING:
			case PGRES_POLLING_WRITING:
				/* we may use select() before polling again and select()
				 * for reading/writing upon status value
				 */
				FD_ZERO(&fd);
				FD_SET(sock, &fd);
				select(sock + 1,
					   status == PGRES_POLLING_READING ? &fd : NULL,
					   status == PGRES_POLLING_WRITING ? &fd : NULL,
					   NULL, NULL);
				break;

			case PGRES_POLLING_ACTIVE:
				/* can call poll function immediately, do nothing */
				break;
		}
		/* loop until we have OK or FAILED status */
	} while (status != PGRES_POLLING_OK && status!= PGRES_POLLING_FAILED);

	/* check if our connection failed */
	if (status == PGRES_POLLING_FAILED)
	{
		printf("failed\n");
		return -1;
	}

	//printf("OK\n");
	return 0;
}

/*
 * Start asynchronous connection and return pointer to the PGconn
 * object if connection is established otherwise return NULL
 */
PGconn* make_asynchronous_connection(void)
{
	PGconn *conn;

	/* try to start connection */
	conn = PQconnectStart(conninfo);

	if (poll_for_connection(conn, 0))
	{
		cerr << "e#037 " << PQerrorMessage( conn ) << endl;
		PQfinish(conn);
		return NULL;
	}

	return conn;
}

int reset_asynchronous_connection(PGconn *conn)
{
	if (!PQresetStart(conn))
	{
		cerr << "e#033 " << PQerrorMessage( conn ) << endl;
		PQfinish(conn);
		return -1;
	}

	if (poll_for_connection(conn, 1))
	{
		PQfinish(conn);
		return -1;
	}

	return 0;
}

/*
AQUI COMECA O MEU CODIGO
*/
/*
Instancia na memoria a RNA
*/
int carrega_rna(std::vector<structNeuronio> *rede, unsigned short int id_rede)
{
    rede->clear();

	PGconn *conexao;
	if ((conexao = make_asynchronous_connection()) == NULL)
	{
        cerr << "e#030 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
	}

    if ( (PQstatus(conexao) == CONNECTION_BAD) )
    {
        cerr << "e#010 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
    }

    char strSQL[256];
    memset (strSQL, '\x0', 256);
    char strIdRNA[3];
    strcpy (strSQL, "SELECT id, id_rna, camada,limite_superior,limiar_superior,valor_referencia,limiar_inferior,limite_inferior,criterio,status,peso,funcao_processamento,funcao_ativacao,valor_recebido FROM neuronio WHERE id_rna=" );
    sprintf(strIdRNA, "%d", id_rede);
    strcat (strSQL, strIdRNA);

    PGresult *retornoSelect = PQexec( conexao, strSQL );

    if ( (!PQresultStatus(retornoSelect )) || (PQresultStatus(retornoSelect ) == PGRES_EMPTY_QUERY) || (PQresultStatus(retornoSelect ) != PGRES_TUPLES_OK) )
    {
        cerr << "e#012 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 1;
    }

    structNeuronio neuro;

    unsigned long int nr_tuplas = PQntuples ( retornoSelect );

    while ( nr_tuplas-- )
    {
        neuro.id = atoi(PQgetvalue( retornoSelect, nr_tuplas, 0));
        neuro.id_rna = atoi(PQgetvalue( retornoSelect, nr_tuplas, 1));
        neuro.camada = (enumCamadaNeuronio) atoi(PQgetvalue(retornoSelect, nr_tuplas, 2));
        neuro.segmentos.limiteSuperior = atof( PQgetvalue(retornoSelect, nr_tuplas, 3));
        neuro.segmentos.limiarSuperior = atof( PQgetvalue(retornoSelect, nr_tuplas, 4));
        neuro.segmentos.valorReferencia = atof (PQgetvalue(retornoSelect, nr_tuplas, 5));
        neuro.segmentos.limiarInferior = atof (PQgetvalue(retornoSelect, nr_tuplas, 6));
        neuro.segmentos.limiteInferior = atof (PQgetvalue(retornoSelect, nr_tuplas, 7));
        neuro.criterio = (enumCriterio) atoi(PQgetvalue(retornoSelect, nr_tuplas, 8));
        neuro.peso = atof (PQgetvalue(retornoSelect, nr_tuplas, 9));
        neuro.status = (enumStatusNeuronio) atoi(PQgetvalue(retornoSelect, nr_tuplas, 10) );
        neuro.funcaoCondensacao =  (enumFuncaoProcessamento) atoi(PQgetvalue(retornoSelect, nr_tuplas, 11) );
        neuro.funcaoAtivacao = (enumFuncaoAtivacao)  atoi(PQgetvalue(retornoSelect, nr_tuplas, 12) );
        neuro.valorRecebido = atof (PQgetvalue(retornoSelect, nr_tuplas, 13));
        rede->push_back(neuro);
    };

//zera os inputs
    int n = rede->size();
    while (n--) for (int k=NR_INPUTS; k;) rede->at(n).inputs[--k].idxNeuronio = 0;

//carrega o vinculo dos inputs: neuro orig e dst.
    memset (strSQL, '\x0', 256);
    strcpy (strSQL, "SELECT id_neuro_dst,id_neuro_orig,valor FROM grafo WHERE id_rna=" );
    strcat (strSQL, strIdRNA);

    retornoSelect = PQexec( conexao, strSQL );

    if ( ( !PQresultStatus(retornoSelect ) ) || ( PQresultStatus(retornoSelect ) == PGRES_EMPTY_QUERY ) || ( PQresultStatus(retornoSelect ) != PGRES_TUPLES_OK ) )
    {
        cerr << "e#023 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 1;
    }

//prepara as matrizes de inputs zerando os registradores
    int idx_neuro_atual=-1;
    int idx_neuro_dst=0;
    int idx_input=0; //contador de inputs de um neuronio na struct inputs

    nr_tuplas = PQntuples ( retornoSelect );

    while ( nr_tuplas-- )
    {
        idx_neuro_dst =  getIdxVectorByIdxNeuronio( atoi(PQgetvalue(retornoSelect, nr_tuplas, 0)), *rede ); //obtem a posicao no vector do neuronio destino com seu id recuperado da tabela de vinculos
        if ( idx_neuro_atual != idx_neuro_dst ) //se for uma posicao valida no vector
        {
            idx_input = 0; //zera o contador
            idx_neuro_atual = idx_neuro_dst; //atualiza o vector de inputs
        }
        rede->at(idx_neuro_atual).inputs[idx_input].idxNeuronio = atoi(PQgetvalue(retornoSelect, nr_tuplas, 1)); //origem
        rede->at(idx_neuro_atual).inputs[idx_input++].valor = atof(PQgetvalue(retornoSelect, nr_tuplas, 2)); //valor
    };

    PQfinish(conexao);
    return 1;
}

/*
Pega o campo da tabela neuronio indicativo da posicao do campo na tabela aquario de todos os neuronios de entrada e atualiza na tabela input_neuro o valor
*/
int updateVariaveisAmbiente(void)
{
//realiza a conexao

	PGconn *conexao;
	if ((conexao = make_asynchronous_connection()) == NULL)
	{
        cerr << "e#031 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
	}

    if ( (PQstatus(conexao) == CONNECTION_BAD) )
    {
        cerr << "e#025 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
    }

// pegas as variaveis na tabela de registro pelo sistemas externos

    PGresult *retornoSelectAmbiente = PQexec( conexao, "SELECT id_neuro, valor FROM fn WHERE id_neuro IN (SELECT id FROM neuronio WHERE neuronio.id_rna=1)" );

    if ( (!PQresultStatus(retornoSelectAmbiente)) || (PQresultStatus(retornoSelectAmbiente)==PGRES_EMPTY_QUERY) || (PQresultStatus(retornoSelectAmbiente)!=PGRES_TUPLES_OK) )
    {
        cerr << "e#015 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 1;
    }
/*
    PGresult *retornoSelectNeuronio = PQexec( conexao, "SELECT id,idx_campo_origem FROM neuronio WHERE camada = 0" );


    if ( (!PQresultStatus(retornoSelectNeuronio)) || (PQresultStatus(retornoSelectNeuronio)==PGRES_EMPTY_QUERY) || (PQresultStatus(retornoSelectNeuronio)!=PGRES_TUPLES_OK) )
    {
        cerr << "e#018 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 1;
    }
*/
// armazena a qtde de neuronios a serem atualizados
    unsigned long int nr_tuplas = PQntuples ( retornoSelectAmbiente );

// cria variavel para registrar o SQL de update do campo valor recebido
    char strSQL[100];
    memset (strSQL, '\x0', 100);
    unsigned long int posicao_campo_TabAquario=0;

    while ( nr_tuplas-- ){

//        posicao_campo_TabAquario = atoi(PQgetvalue( retornoSelectNeuronio, nr_tuplas, 1)); //a posicao da coluna na tabela aquario (ref variavel ambiente)

        strcpy (strSQL, "UPDATE neuronio SET valor_recebido=" );
        strcat (strSQL, PQgetvalue(retornoSelectAmbiente,nr_tuplas, 1)); //valor variavel ambiente
        strcat (strSQL, " WHERE id=");
        strcat (strSQL, PQgetvalue( retornoSelectAmbiente, nr_tuplas, 0 )); //id do neuronio

        if (PQresultStatus(PQexec (conexao, strSQL)) != PGRES_COMMAND_OK)
        {
            cerr << "e#014 " << PQerrorMessage(conexao) << endl;
            PQfinish(conexao);
            return 0;
        }
    };

    PQfinish(conexao);
    return 1;
}

/*
Grava a recomendacao

Parametros:
id da RNA
valor calculado
*/
int updateOutput(unsigned int id, float valor)
{
	PGconn *conexao;
	if ((conexao = make_asynchronous_connection()) == NULL)
	{
        cerr << "e#035 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
	}

    char strSQL[100];
    char strValor[100];
    strcpy (strSQL, "UPDATE rna SET output=" );
    sprintf(strValor, "%f", valor);
    strcat (strSQL, strValor);
    strcat (strSQL, " WHERE id=");
    sprintf(strValor, "%d", id);
    strcat (strSQL, strValor);

    if (PQresultStatus(PQexec (conexao, strSQL)) != PGRES_COMMAND_OK)
    {
        cerr << "e#024\n";
        PQfinish(conexao);
        return 0;
    }
    PQfinish(conexao);
    return 1;
}

