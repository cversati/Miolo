package main

import (
  "fmt"
  "database/sql"
//  "log"
//  "os"

_  "github.com/mattn/go-sqlite3" // Import go-sqlite3 library

)

type TestItem struct {
	id	string
}

func SQLite3_Configura () {

}

func SQLite3_Conecta () *sql.DB{

  conn, err :=sql.Open("sqlite3", "/home/versati/Projects/Miolo/db/rna.db")

  if err != nil {
    fmt.Println("Failed to create the handle")
  }

  return conn

}

func BDSQLite3_Desconecta () {

}

func CarregaRede() {

}

func UpdateVariaveisAmbiente () {

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
