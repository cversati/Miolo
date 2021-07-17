package main

import (
  "fmt"
  "strconv"
  "database/sql"
//  "log"
//  "os"i

_  "github.com/mattn/go-sqlite3" // Import go-sqlite3 library

)

func SQLite3_Conecta () *sql.DB{

  conn, err :=sql.Open("sqlite3", "/home/versati/Projects/Miolo/db/rna.db3")

  if err != nil {
    fmt.Println("Failed to create the handle")
  }

  return conn

}

func CarregaRede( pIdRNA int ) bool {
  //checa se em conexao com o banco de dados
  conexao:=SQLite3_Conecta ()

  //realiza o select
  rowsNeuronio, err1 := conexao.Query ( "SELECT neuronio.id,camada,limite_superior,limiar_superior,valor_referencia,limiar_inferior,limite_inferior,criterio,status,peso,funcao_processamento, funcao_ativacao FROM neuronio,neuro_rna WHERE neuro_rna.id_neuro=neuronio.id AND neuro_rna.id_rna=" + strconv.Itoa(pIdRNA) )
  if err1 != nil {
    panic(err1)
    return false
  }
  defer rowsNeuronio.Close()

  //carrega a matriz de structNeuronio (rede) com os registros
  var resultNeuronio []structNeuronio
  tuplaNeuronio := structNeuronio{}

  for rowsNeuronio.Next() {
    err2 := rowsNeuronio.Scan(&tuplaNeuronio.id, &tuplaNeuronio.camada, &tuplaNeuronio.limiteSuperior, &tuplaNeuronio.limiarSuperior, &tuplaNeuronio.valorReferencia, &tuplaNeuronio.limiarInferior, &tuplaNeuronio.limiteInferior, &tuplaNeuronio.criterio, &tuplaNeuronio.status, &tuplaNeuronio.peso, &tuplaNeuronio.funcaoCondensacao, &tuplaNeuronio.funcaoAtivacao)
    if err2 != nil {
      panic(err2)
      return false
    }
    resultNeuronio = append(resultNeuronio, tuplaNeuronio)
    fmt.Println (tuplaNeuronio.id, tuplaNeuronio.limiarInferior, tuplaNeuronio.camada)
  }

  //zera inputs

  //realiza o select
  rowsGrafo, err3 := conexao.Query ( "SELECT id_neuro_dst,id_neuro_orig,valor FROM input_neuro WHERE id_neuro_dst IN (SELECT id_neuro FROM neuro_rna WHERE id_rna=" + strconv.Itoa(pIdRNA) + ")" )
  if err3 != nil {
    panic(err3)
    return false
  }
  defer rowsGrafo.Close()

  //carrega o vinculo dos inputs: neuro orig e dst
  var resultGrafo []structGrafo
  tuplaGrafo := structGrafo{}

  for rowsGrafo.Next() {
    err4 := rowsGrafo.Scan( &tuplaGrafo.id_neuro_dst, &tuplaGrafo.id_neuro_orig, &tuplaGrafo.valor )
    if err4 != nil {
      panic(err4)
      return false
    }

    resultGrafo = append(resultGrafo, tuplaGrafo)
    fmt.Println ( tuplaGrafo.id_neuro_dst, tuplaGrafo.id_neuro_orig, tuplaGrafo.valor)
  }

  //fecha conexao structInput
  conexao.Close()

  return true
}

func UpdateVariaveisAmbiente () {
//checa se em conexao com o banco de dados
//realiza o select da base transacoes
}

func UpdateOutput () {

}
