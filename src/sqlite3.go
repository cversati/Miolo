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
  rowsNeuronio, err1 := conexao.Query ( "SELECT id,camada,limite_superior,limiar_superior,valor_referencia,limiar_inferior,limite_inferior,criterio,status,peso,funcao_processamento, funcao_ativacao FROM neuronio WHERE id_rna=" + strconv.Itoa(pIdRNA) )
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
  rowsGrafo, err3 := conexao.Query ( "SELECT id_neuro_dst,id_neuro_orig,valor FROM grafo WHERE id_rna=" + strconv.Itoa(pIdRNA) )
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

func UpdateVariaveis (pIdRNA int) bool {
//checa  conexao com o banco de dados
  conexao:=SQLite3_Conecta ()

//realiza o select da base de variaveis
  rowsFN, err5 := conexao.Query ( "SELECT id_neuro, valor FROM fn WHERE id_neuro IN (SELECT id FROM neuronio WHERE neuronio.id_rna="+strconv.Itoa(pIdRNA)+")" )
  if err5 != nil {
    panic(err5)
    return false
  }
  defer rowsFN.Close()
  fmt.Println ("-FN-----------------")
//carrega o vinculo dos inputs: neuro orig e dst
  var resultFN []structFN
  tuplaFN := structFN{}

  for rowsFN.Next() {

    err6 := rowsFN.Scan( &tuplaFN.id_neuro, &tuplaFN.valor )
    if err6 != nil {
      panic(err6)
      return false
    }
    fmt.Println (tuplaFN.id_neuro, tuplaFN.valor)
    defer rowsFN.Close()

    resultFN = append(resultFN, tuplaFN)
    sql := "UPDATE neuronio SET valor_recebido=" + strconv.FormatFloat(tuplaFN.valor, 'f', 3, 64) + " WHERE id=" + strconv.Itoa ( int ( tuplaFN.id_neuro ) )
    fmt.Println ( sql)
    stmt, err := conexao.Prepare (sql)
    if err != nil {
      panic(err.Error())
    }
    res, err := stmt.Exec()
    res.RowsAffected()

//    stmt.Commit()
/*
    sql := "UPDATE neuronio SET valor_recebido=" + strconv.FormatFloat(tuplaFN.valor, 'f', 3, 64) + " WHERE id=" + strconv.Itoa ( int ( tuplaFN.id_neuro ) )
    fmt.Println ( sql)
    stmt, err := conexao.Prepare ("UPDATE neuronio SET valor_recebido=? WHERE id=?")
    if err != nil {
      panic(err.Error())
    }

    res, err := stmt.Exec (strconv.FormatFloat(tuplaFN.valor, 'f', 3, 64), strconv.Itoa ( int ( tuplaFN.id_neuro ) ) )
    if err != nil {
      panic(err.Error())
      fmt.Println(res)
    }
*/
  }
/*
  // Atualizando a data
   tx, err := mydb.Begin()
   checkErr(err)
   //...
   cmd := "UPDATE userinfo SET created = ? WHERE uid = ?"
   updateDate, err := tx.Prepare(cmd)
   checkErr(err)
*/
//fecha conexao
//realiza o select
rowsNeuronio, err1 := conexao.Query ( "SELECT id,camada,limite_superior,limiar_superior,valor_referencia,limiar_inferior,limite_inferior,criterio,status,peso,funcao_processamento, funcao_ativacao,valor_recebido FROM neuronio WHERE id_rna=" + strconv.Itoa(pIdRNA) )
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
  fmt.Println (tuplaNeuronio.id, tuplaNeuronio.valorRecebido, tuplaNeuronio.camada)
}



  conexao.Close()

  return true

}
