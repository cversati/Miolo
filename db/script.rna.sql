
CREATE DATABASE rna;

DROP TABLE neuronio;
DROP TABLE rna;
DROP TABLE neuro_rna;
DROP TABLE input_neuro;
DROP TABLE fn;

CREATE TABLE "fn" (
	"id_neuro"	INTEGER NOT NULL,
	"valor"	REAL NOT NULL
);

CREATE TABLE "rna" (
	"output"	REAL,
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT
);

CREATE TABLE "input_neuro" (
	"valor"	REAL,
	"id_neuro_dst"	INTEGER,
	"id_neuro_orig"	INTEGER,
	FOREIGN KEY("id_neuro_orig") REFERENCES "neuronio"("id"),
	FOREIGN KEY("id_neuro_dst") REFERENCES "neuronio"("id")
);

CREATE TABLE "neuronio" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"nm"	TEXT,
	"camada"	INTEGER,
	"limite_superior"	REAL,
	"limite_inferior"	REAL,
	"limiar_superior"	REAL,
	"limiar_inferior"	REAL,
	"valor_referencia"	REAL,
	"criterio"	INTEGER,
	"status"	INTEGER,
	"peso"	REAL,
	"funcao_processamento"	INTEGER,
	"funcao_ativacao"	INTEGER
);

CREATE TABLE "neuro_rna" (
	"id_rna"	INTEGER NOT NULL,
	"id_neuro"	INTEGER NOT NULL,
	FOREIGN KEY("id_neuro") REFERENCES "neuronio"("id"),
	FOREIGN KEY("id_rna") REFERENCES "rna"("id")
);
