package main

import (
  "fmt"
  "database/sql"
//  "log"
//  "os"i

_  "github.com/mattn/go-sqlite3" // Import go-sqlite3 library

)

type TestItem struct {
	id	string
}

func SQLite3_Configura () {

}

func SQLite3_Conecta () *sql.DB{

  conn, err :=sql.Open("sqlite3", "/home/versati/Projects/Miolo/db/rna.db3")

  if err != nil {
    fmt.Println("Failed to create the handle")
  }

  return conn

}

func BDSQLite3_Desconecta () {

}

func CarregaRede( pIdRNA int32 ) bool{
//checa se em conexao com o banco de dados
conexao:=SQLite3_Conecta ()
//realiza o select
//"SELECT neuronio.id,camada,limite_superior,limiar_superior,valor_referencia,limiar_inferior,limite_inferior,criterio,status,peso,funcao_processamento, funcao_ativacao FROM neuronio,neuro_rna WHERE neuro_rna.id_neuro=neuronio.id AND neuro_rna.id_rna=" );
//ReadItem ( conexao )
//carrega a matriz de structNeuronio (rede) com os registros
//rows, err := conexao.Query ( "SELECT neuronio.id,camada,limite_superior,limiar_superior,valor_referencia,limiar_inferior,limite_inferior,criterio,status,peso,funcao_processamento, funcao_ativacao FROM neuronio,neuro_rna WHERE neuro_rna.id_neuro=neuronio.id AND neuro_rna.id_rna=1" )
rows, err := conexao.Query ( "SELECT id FROM neuronio" )

if err != nil { panic(err) }
defer rows.Close()

var result []TestItem

for rows.Next() {
  tupla := TestItem{}
  err2 := rows.Scan(&tupla.id)
  if err2 != nil { panic(err2) }
  result = append(result, tupla)
  fmt.Println (tupla.id)
}


//zera inputs
//realiza o select
//"SELECT id_neuro_dst,id_neuro_orig,valor FROM input_neuro WHERE id_neuro_dst IN (SELECT id_neuro FROM neuro_rna WHERE id_rna=" );
//carrega o vinculdo dos inputs: neuro orig e dst
//fecha conexao

  return true
}

func UpdateVariaveisAmbiente () {
//checa se em conexao com o banco de dados
//realiza o select da base transacoes
}

func UpdateOutput () {

}

func ReadItem(db *sql.DB) []TestItem {

    fmt.Println ("#2")

	sql_readall := `SELECT id FROM neuronio`

	rows, err := db.Query(sql_readall)
	if err != nil { panic(err) }
	defer rows.Close()

	var result []TestItem
	for rows.Next() {
		item := TestItem{}
		err2 := rows.Scan(&item.id)
		if err2 != nil { panic(err2) }
		result = append(result, item)
    fmt.Println (item.id)
	}
	return result
}

func Teste () {
  ReadItem(SQLite3_Conecta())
}
