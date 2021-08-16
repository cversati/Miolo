--SELECT fn.valor FROM fn, neuronio WHERE id=id_neuro AND id IN (SELECT id_neuro FROM neuro_rna WHERE id_rna=1)

---
CREATE DATABASE rna;

DROP TABLE fn;
DROP TABLE rna;
DROP TABLE neuronio;
DROP TABLE grafo;

CREATE TABLE "fn" (
	"id_neuro"	INTEGER NOT NULL,
	"valor"	REAL NOT NULL
);

INSERT INTO fn VALUES (1,2);
INSERT INTO fn VALUES (2,3);

CREATE TABLE "rna" (
	"output"	REAL NOT NULL,
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	"id_cliente"	INTEGER NOT NULL
);

INSERT  INTO rna VALUES (0,1,1);
INSERT  INTO rna VALUES (0,2,2);
INSERT  INTO rna VALUES (0,3,3);

CREATE TABLE "neuronio" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"id_rna"	INTEGER,
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
	"funcao_ativacao"	INTEGER,
	"valor_recebido"	REAL
);

INSERT INTO neuronio VALUES (1,1,"a",1,10,2,7,4,3,1,1,1,1,1,0);
INSERT INTO neuronio VALUES (2,1,"a",1,10,2,7,4,3,1,1,1,1,1,0);
INSERT INTO neuronio VALUES (3,1,"a",9,10,2,7,4,3,1,1,1,1,1,0);


CREATE TABLE "grafo" (
	"valor"	REAL,
	"id_rna"	INTEGER,
	"id_neuro_dst"	INTEGER,
	"id_neuro_orig"	INTEGER,
	FOREIGN KEY("id_rna") REFERENCES "rna"("id"),
	FOREIGN KEY("id_neuro_orig") REFERENCES "neuronio"("id"),
	FOREIGN KEY("id_neuro_dst") REFERENCES "neuronio"("id")
);

INSERT INTO grafo VALUES (0,1,1,3);
INSERT INTO grafo VALUES (0,1,2,3);

------------------===========================-----------------------

DROP TABLE fn;
DROP TABLE neuro_rna;
DROP TABLE rna;
DROP TABLE input_neuro;
DROP TABLE neuronio;

CREATE TABLE "fn" (
	"id_neuro"	INTEGER NOT NULL,
	"valor"	REAL NOT NULL
);

INSERT INTO fn VALUES (1,2);
INSERT INTO fn VALUES (2,3);

CREATE TABLE "rna" (
	"output"	REAL NOT NULL,
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	"id_cliente"	INTEGER NOT NULL
);

INSERT  INTO rna VALUES (0,1,1);
INSERT  INTO rna VALUES (0,2,1);
INSERT  INTO rna VALUES (0,3,2);

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

INSERT INTO neuronio VALUES (1,"a",0,10,2,7,4,3,1,1,1,1,1);
INSERT INTO neuronio VALUES (2,"a",0,10,2,7,4,3,1,1,1,1,1);
INSERT INTO neuronio VALUES (3,"a",9,10,2,7,4,3,1,1,1,1,1);

CREATE TABLE "input_neuro" (
	"valor"	REAL,
	"id_neuro_dst"	INTEGER,
	"id_neuro_orig"	INTEGER,
	FOREIGN KEY("id_neuro_orig") REFERENCES "neuronio"("id"),
	FOREIGN KEY("id_neuro_dst") REFERENCES "neuronio"("id")
);

INSERT INTO input_neuro VALUES (0,1,3);
INSERT INTO input_neuro VALUES (0,2,3);

CREATE TABLE "neuro_rna" (
	"id_rna"	INTEGER NOT NULL,
	"id_neuro"	INTEGER NOT NULL,
	FOREIGN KEY("id_neuro") REFERENCES "neuronio"("id"),
	FOREIGN KEY("id_rna") REFERENCES "rna"("id")
);

INSERT INTO neuro_rna VALUES (1,1);
INSERT INTO neuro_rna VALUES (1,2);
