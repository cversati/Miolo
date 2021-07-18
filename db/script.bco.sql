
CREATE DATABASE bco;

DROP TABLE transacao;
DROP TABLE conta;
DROP TABLE agencia;
DROP TABLE tp_conta;
DROP TABLE tp_cliente;

CREATE TABLE "agencia" (
	"prefixo"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"nm"	TEXT
);

INSERT INTO agencia (prefixo, nm) VALUES (1, "SPO");
INSERT INTO agencia (prefixo, nm) VALUES (2, "BSB");
INSERT INTO agencia (prefixo, nm) VALUES (3, "RJO");
INSERT INTO agencia (prefixo, nm) VALUES (4, "CTB");

CREATE TABLE "tp_conta" (
	"tp_conta"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"nm"	TEXT NOT NULL
);

INSERT INTO tp_conta (tp_conta, nm) VALUES (1, "PF Comum");
INSERT INTO tp_conta (tp_conta, nm) VALUES (2, "PF Especial");
INSERT INTO tp_conta (tp_conta, nm) VALUES (3, "PJ Comum");
INSERT INTO tp_conta (tp_conta, nm) VALUES (4, "PJ Especial");

CREATE TABLE "tp_cliente" (
	"tp_cliente"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"nm"	TEXT NOT NULL
);

INSERT INTO tp_cliente (tp_cliente, nm) VALUES (1, "PF");
INSERT INTO tp_cliente (tp_cliente, nm) VALUES (2, "PJ");

CREATE TABLE "conta" (
	"prefixo"	INTEGER NOT NULL,
	"conta"	INTEGER NOT NULL,
	"nm"	TEXT NOT NULL,
	"tp_cliente"	INTEGER,
	"tp_conta"	INTEGER,
	"status" INTEGER,
	FOREIGN KEY("prefixo") REFERENCES "agencia"("prefixo"),
	FOREIGN KEY("tp_cliente") REFERENCES "tp_cliente"("tp_cliente")
	FOREIGN KEY("tp_conta") REFERENCES "tp_conta"("tp_conta")
);

INSERT INTO conta (prefixo, conta, nm, tp_cliente, tp_conta, status) VALUES (1, 1, "Maria", 1, 1, 1);
INSERT INTO conta (prefixo, conta, nm, tp_cliente, tp_conta, status) VALUES (1, 2, "Fernanda", 2, 1, 1);
INSERT INTO conta (prefixo, conta, nm, tp_cliente, tp_conta, status) VALUES (2, 1, "Angela", 1, 1, 1);
INSERT INTO conta (prefixo, conta, nm, tp_cliente, tp_conta, status) VALUES (2, 2, "Rafael", 1, 2, 1);
INSERT INTO conta (prefixo, conta, nm, tp_cliente, tp_conta, status) VALUES (2, 3, "Higa", 1, 1, 1);
INSERT INTO conta (prefixo, conta, nm, tp_cliente, tp_conta, status) VALUES (3, 1, "Pedro", 1, 1, 1);
INSERT INTO conta (prefixo, conta, nm, tp_cliente, tp_conta, status) VALUES (3, 2, "Padaria", 2, 1, 0);
INSERT INTO conta (prefixo, conta, nm, tp_cliente, tp_conta, status) VALUES (4, 1, "Farmacia", 2, 2, 1);
INSERT INTO conta (prefixo, conta, nm, tp_cliente, tp_conta, status) VALUES (4, 2, "Barbara", 1, 1, 1);

CREATE TABLE "transacao" (
	"timestamp"	REAL NOT NULL,
	"transacao"	INTEGER NOT NULL,
	"valor"	REAL,
	"deb_cred"	INTEGER,
	"bco_dst"	INTEGER,
	"ag_dst"	INTEGER,
	"cc_dst"	INTEGER,
	"prefixo"	INTEGER,
	"conta"	INTEGER
--	FOREIGN KEY("prefixo") REFERENCES "conta"("prefixo"),
--	FOREIGN KEY("conta") REFERENCES "conta"("conta")
);

INSERT INTO transacao (timestamp, prefixo, conta, transacao, valor, deb_cred, bco_dst, ag_dst, cc_dst) VALUES (1, 1, 1, 2, 100, 2, 0, 0, 0);
INSERT INTO transacao (timestamp, prefixo, conta, transacao, valor, deb_cred, bco_dst, ag_dst, cc_dst) VALUES (2, 1, 1, 3, 10, 1, 1, 1, 20);
INSERT INTO transacao (timestamp, prefixo, conta, transacao, valor, deb_cred, bco_dst, ag_dst, cc_dst) VALUES (2, 1, 2, 2, 10, 1, 1, 1, 20);
INSERT INTO transacao (timestamp, prefixo, conta, transacao, valor, deb_cred, bco_dst, ag_dst, cc_dst) VALUES (3, 1, 1, 5, 10, 1, 1, 1, 20);
INSERT INTO transacao (timestamp, prefixo, conta, transacao, valor, deb_cred, bco_dst, ag_dst, cc_dst) VALUES (4, 1, 1, 3, 10, 1, 1, 1, 20);
INSERT INTO transacao (timestamp, prefixo, conta, transacao, valor, deb_cred, bco_dst, ag_dst, cc_dst) VALUES (5, 1, 1, 3, 10, 1, 1, 1, 20);
INSERT INTO transacao (timestamp, prefixo, conta, transacao, valor, deb_cred, bco_dst, ag_dst, cc_dst) VALUES (6, 1, 1, 3, 10, 1, 1, 1, 20);
INSERT INTO transacao (timestamp, prefixo, conta, transacao, valor, deb_cred, bco_dst, ag_dst, cc_dst) VALUES (7, 1, 1, 3, 10, 1, 1, 1, 20);
INSERT INTO transacao (timestamp, prefixo, conta, transacao, valor, deb_cred, bco_dst, ag_dst, cc_dst) VALUES (7, 1, 2, 3, 10, 1, 1, 1, 20);
INSERT INTO transacao (timestamp, prefixo, conta, transacao, valor, deb_cred, bco_dst, ag_dst, cc_dst) VALUES (8, 1, 1, 3, 10, 1, 1, 1, 20);
INSERT INTO transacao (timestamp, prefixo, conta, transacao, valor, deb_cred, bco_dst, ag_dst, cc_dst) VALUES (8, 1, 2, 3, 10, 1, 1, 1, 20);
INSERT INTO transacao (timestamp, prefixo, conta, transacao, valor, deb_cred, bco_dst, ag_dst, cc_dst) VALUES (9, 1, 1, 3, 10, 1, 1, 1, 20);
INSERT INTO transacao (timestamp, prefixo, conta, transacao, valor, deb_cred, bco_dst, ag_dst, cc_dst) VALUES (10, 1, 1, 3, 10, 1, 1, 1, 20);
INSERT INTO transacao (timestamp, prefixo, conta, transacao, valor, deb_cred, bco_dst, ag_dst, cc_dst) VALUES (11, 1, 1, 3, 10, 1, 1, 1, 20);
