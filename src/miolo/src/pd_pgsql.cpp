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
		fprintf(stderr, "e#002 Asynchronous connection failed...\n");
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
		printf("e#002 failed\n");
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
		cerr << "e#003 " << PQerrorMessage( conn ) << endl;
		PQfinish(conn);
		return NULL;
	}

	return conn;
}

int reset_asynchronous_connection(PGconn *conn)
{
	if (!PQresetStart(conn))
	{
		cerr << "e#004 " << PQerrorMessage( conn ) << endl;
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
    if (RNA_LOG) cout << endl << "l#001 " << "iniciando do carregamento da rna " << id_rede << endl;
    rede->clear();

	PGconn *conexao;
	if ((conexao = make_asynchronous_connection()) == NULL)
	{
        cerr << "e#005 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
	}
    if ( (PQstatus(conexao) == CONNECTION_BAD) )
    {
        cerr << "e#006 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
    }

    char strSQL[256];
    memset (strSQL, '\x0', 256);
    char strIdRNA[3];
    strcpy (strSQL, "SELECT id,id_rna,camada,limite_superior,limiar_superior,valor_referencia,limiar_inferior,limite_inferior,criterio,status,peso,funcao_processamento,funcao_ativacao,valor_recebido FROM neuronio WHERE id_rna=" );
    sprintf(strIdRNA, "%d", id_rede);
    strcat (strSQL, strIdRNA);
// recupera da base a rna
    if (RNA_LOG) cout << "s#001 " << strSQL << endl;
    PGresult *retornoSelect = PQexec( conexao, strSQL );

    if ( (!PQresultStatus(retornoSelect )) || (PQresultStatus(retornoSelect ) == PGRES_EMPTY_QUERY) || (PQresultStatus(retornoSelect ) != PGRES_TUPLES_OK) )
    {
        cerr << "e#007 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
    }
// armazena numa estrutura em memoria a rna
    if (RNA_LOG) cout << "l#002 " << "instanciando a rna " << id_rede << " em memoria" << endl;

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
    if (RNA_LOG) cout << "l#004 " << n << " neuronios impostados na matriz RNA" << endl;
    while (n--) for (int k=NR_INPUTS; k;) rede->at(n).inputs[--k].id_neuro_orig = -1;

// carrega o vinculo dos inputs: neuro orig e dst.
    if (RNA_LOG) cout << "l#005 " << "iniciando carregamento do grafo da rna" << endl;
    memset (strSQL, '\x0', 256);
    strcpy (strSQL, "SELECT id_neuro_dst,id_neuro_orig,valor FROM grafo WHERE id_rna=" );
    strcat (strSQL, strIdRNA);
    strcat (strSQL, " ORDER BY id_neuro_orig DESC");

    if (RNA_LOG) cout << "s#002 " << strSQL << endl;
    retornoSelect = PQexec( conexao, strSQL );

    if ( ( !PQresultStatus(retornoSelect ) ) || ( PQresultStatus(retornoSelect ) == PGRES_EMPTY_QUERY ) || ( PQresultStatus(retornoSelect ) != PGRES_TUPLES_OK ) )
    {
        cerr << "e#008 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
    }

// prepara as matrizes de inputs
    nr_tuplas = PQntuples ( retornoSelect );
    if (RNA_LOG) cout << "l#006 " << "grafo formado por " << nr_tuplas << " vertices" << endl;
    unsigned short int idx_neuro = 0; //posicao do neuronio na struct array
    unsigned short int k_input = 0; //contador de inputs
    unsigned short int id_neuro_anterior = 0; //variavel para checar se mudou o neuronio e zerar o k_input

    while (nr_tuplas--)
    {
// obtem a posicao do neuronio no array struct
        idx_neuro = getIdxVectorByIdxNeuronio(atoi(PQgetvalue(retornoSelect, nr_tuplas, 1)), *rede);

// checa se é o mesmo neuronio de origem. Se for incrementa o contador de inputs, caso contrario, zera a variavel
        if ( id_neuro_anterior == rede->at(idx_neuro).id )
        {
            if ( k_input >= NR_INPUTS )
            {
                cerr << "e#014 " << " estorou do contador de NR_INPUTS" << endl;
                return 0;
            }
            else
            {
                k_input++;
            }
        }
        else
        {
            id_neuro_anterior = rede->at(idx_neuro).id;
            k_input = 0;
        }

// grava o id do neuronio de origem da info (se camada de entrada coloca 0 no campo neuronio de origem
        if ( rede->at(idx_neuro).camada == Entrada )
            rede->at(idx_neuro).inputs[k_input].id_neuro_orig = 0;
        else
            rede->at(idx_neuro).inputs[k_input].id_neuro_orig = atoi(PQgetvalue(retornoSelect, nr_tuplas, 1)); //id orig

// grava o valor e destino
        rede->at(idx_neuro).inputs[k_input].id_neuro_dst = atoi(PQgetvalue(retornoSelect, nr_tuplas, 0)); //id dst
        rede->at(idx_neuro).inputs[k_input].valor = atof(PQgetvalue(retornoSelect, nr_tuplas, 2)); //valor

        if (RNA_LOG) cout << "l#007 " << "vertice " << rede->at(idx_neuro).id << " -> " << atoi(PQgetvalue(retornoSelect, nr_tuplas, 0)) << " : " << rede->at(idx_neuro).inputs[k_input].valor << endl;
    }

    PQfinish(conexao);
    return 1;
}

/*
Pega o campo da tabela neuronio indicativo da posicao do campo na tabela aquario de todos os neuronios de entrada e atualiza na tabela input_neuro o valor
*/
int updateVariaveisAmbiente(void)
{
    if (RNA_LOG) cout << endl << "l#008 capturando as variaveis de ambiente" << endl;
//realiza a conexao

	PGconn *conexao;
	if ((conexao = make_asynchronous_connection()) == NULL)
	{
        cerr << "e#009 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
	}

    if ( (PQstatus(conexao) == CONNECTION_BAD) )
    {
        cerr << "e#010 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
    }

// pegas as variaveis na tabela de registro pelo sistemas externos

    PGresult *retornoSelectAmbiente = PQexec( conexao, "SELECT id_neuro, valor FROM fn WHERE id_neuro IN (SELECT id FROM neuronio WHERE neuronio.id_rna=1)" );

    if ( (!PQresultStatus(retornoSelectAmbiente)) || (PQresultStatus(retornoSelectAmbiente)==PGRES_EMPTY_QUERY) || (PQresultStatus(retornoSelectAmbiente)!=PGRES_TUPLES_OK) )
    {
        cerr << "e#011 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 1;
    }

// armazena a qtde de neuronios a serem atualizados
    unsigned long int nr_tuplas = PQntuples ( retornoSelectAmbiente );

// cria variavel para registrar o SQL de update do campo valor recebido
    char strSQL[100];
    memset (strSQL, '\x0', 100);

    while ( nr_tuplas-- ){
        //persiste na tabela de grafos os valores das variaveis de ambiente
        strcpy (strSQL, "UPDATE grafo SET valor=" );
        strcat (strSQL, PQgetvalue(retornoSelectAmbiente,nr_tuplas, 1)); //valor variavel ambiente
        strcat (strSQL, " WHERE id_neuro_orig=");
        strcat (strSQL, PQgetvalue( retornoSelectAmbiente, nr_tuplas, 0 )); //id do neuronio
        if (RNA_LOG) cout << "s#008 " << strSQL << endl;

        if (PQresultStatus(PQexec (conexao, strSQL)) != PGRES_COMMAND_OK)
        {
            cerr << "e#012 " << PQerrorMessage(conexao) << endl;
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


/*
calibragem
*/

int calibrarTodos(unsigned short int idCliente)
{
    if (RNA_LOG) cout << "l#019 " << "calibrando..." << endl;

//realiza a conexao

	PGconn *conexao;
	if ((conexao = make_asynchronous_connection()) == NULL)
	{
        cerr << "e#009 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
	}

    if ( (PQstatus(conexao) == CONNECTION_BAD) )
    {
        cerr << "e#010 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
    }

    PGresult *retornoSelect;
    char strSQL[200];
    memset (strSQL, '\x0', 100);
    char strIdCliente[3];
    sprintf(strIdCliente, "%d", idCliente);

    /*
    item 001 - valor medio dos TEDs (id transacao = 2)
    Neuronio 001
    */

    strcpy (strSQL, "SELECT AVG(valor) FROM transacoes WHERE id_transacao=2 AND id_cliente=" );
    strcat (strSQL, strIdCliente); //valor variavel ambiente
    if (RNA_LOG) cout << "s#006 " << strSQL << endl;
    retornoSelect = PQexec( conexao, strSQL );

    if ( (!PQresultStatus(retornoSelect)) || (PQresultStatus(retornoSelect)==PGRES_EMPTY_QUERY) || (PQresultStatus(retornoSelect)!=PGRES_TUPLES_OK) )
    {
        cerr << "s#004 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 1;
    }

    gravaNaTabelaFN ( 1, ( PQgetisnull(retornoSelect,0,0) ? "-1" : PQgetvalue(retornoSelect,0, 0 ) ) );

    /*
    item 002 - valor da ultima transacao
    Neuronio 002
    */
    strcpy (strSQL, "select valor from transacoes where id_cliente=");
    strcat (strSQL, strIdCliente); //valor variavel ambiente
    strcat (strSQL, " order by ts desc limit 1" );
    if (RNA_LOG) cout << "s#007 " << strSQL << endl;
    retornoSelect = PQexec( conexao, strSQL );

    if ( (!PQresultStatus(retornoSelect)) || (PQresultStatus(retornoSelect)==PGRES_EMPTY_QUERY) || (PQresultStatus(retornoSelect)!=PGRES_TUPLES_OK) )
    {
        cerr << "e#016 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 1;
    }
    gravaNaTabelaFN ( 2, ( PQgetisnull(retornoSelect,0,0) ? "-1" : PQgetvalue(retornoSelect,0, 0 ) ) );

    /*
    item 003 - qtde de TED já realizados para o ultimo CPF que transacionou com o cliente nos ultimos 180 dias
    Neuronio 003
    */
    if (RNA_LOG) cout << "l#022 " << "gravando na tabela FN a qtde de TED para o ultimo CPF que transacionou nos ultimos 180 dias" << endl;
    strcpy (strSQL, "update fn set valor = (select count(id_dst) from transacoes where id_dst=(select id_dst from transacoes order by ts desc limit 1)) where id_neuro=3");
    if (RNA_LOG) cout << "s#010 " << strSQL << endl;

    if (PQresultStatus(PQexec (conexao, strSQL)) != PGRES_COMMAND_OK)
    {
        cerr << "e#020 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
    }

    PQfinish(conexao);
    return 1;
}

int gravaNaTabelaFN ( unsigned short int pIdNeuro, const char* pStrValor)
{
    if (RNA_LOG) cout << "l#020 " << "gravando na tabela FN valor ";

//realiza a conexao

	PGconn *conexao;
	if ((conexao = make_asynchronous_connection()) == NULL)
	{
        cerr << "e#018 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
	}

    if ( (PQstatus(conexao) == CONNECTION_BAD) )
    {
        cerr << "e#019 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
    }

    char strSQL[100];
    memset (strSQL, '\x0', 100);
    char strIdNeuro[3];
    sprintf(strIdNeuro, "%d", pIdNeuro);
    if (RNA_LOG) cout << pStrValor << " no id neuro "  << pIdNeuro << endl;

     //persiste na tabela
    strcpy (strSQL, "UPDATE fn SET valor=" );
    strcat (strSQL, pStrValor );
    strcat (strSQL, " WHERE id_neuro=" );
    strcat (strSQL, strIdNeuro );

    if (RNA_LOG) cout << "s#009 " << strSQL << endl;
    if (PQresultStatus(PQexec (conexao, strSQL)) != PGRES_COMMAND_OK)
    {
        cerr << "e#020 " << PQerrorMessage(conexao) << endl;
        PQfinish(conexao);
        return 0;
    }

    PQfinish(conexao);
    return 1;
}
