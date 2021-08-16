CREATE DATABASE rna;

CREATE TABLE fn (
	id_neuro INTEGER NOT NULL,
	valor	NUMERIC(9,3) DEFAULT 0
);

INSERT INTO fn VALUES (1,2);
INSERT INTO fn VALUES (2,3);

CREATE TABLE rna (
	output NUMERIC(9,3)	NOT null,
	id SERIAL PRIMARY key,
	id_cliente INTEGER NOT NULL
);

INSERT INTO rna VALUES (0,1,1);
INSERT INTO rna VALUES (0,2,2);
INSERT INTO rna VALUES (0,3,3);

CREATE TABLE neuronio (
	id	serial PRIMARY key,
	id_rna	INTEGER,
	nm VARCHAR(100),
	camada	INTEGER default 0,
	limite_superior	numeric(9,3),
	limite_inferior	numeric(9,3),
	limiar_superior	numeric(9,3),
	limiar_inferior	numeric(9,3),
	valor_referencia numeric(9,3),
	valor_recebido numeric(9,3),
	criterio INTEGER,
	status INTEGER default 1,
	peso numeric(9,3),
	funcao_processamento INTEGER,
	funcao_ativacao	INTEGER,
	FOREIGN KEY(id_rna) REFERENCES rna(id) ON DELETE CASCADE
);

INSERT INTO neuronio VALUES (1,1,'a',0,10,2,7,4,3,0,1,1,1,1,1);
INSERT INTO neuronio VALUES (2,1,'a',0,10,2,7,4,3,0,1,1,1,1,1);
INSERT INTO neuronio VALUES (3,1,'a',1,10,2,7,4,3,0,1,1,1,1,1);
INSERT INTO neuronio VALUES (4,1,'a',0,10,2,7,4,3,0,1,1,1,1,1);
INSERT INTO neuronio VALUES (5,1,'a',9,10,2,7,4,3,0,1,1,1,1,1);

CREATE TABLE grafo (
	id_rna INTEGER,
	id_neuro_orig INTEGER,
	id_neuro_dst INTEGER,
	valor numeric(9,3),
	FOREIGN KEY(id_rna) REFERENCES rna(id),
	FOREIGN KEY(id_neuro_orig) REFERENCES neuronio(id),
	FOREIGN KEY(id_neuro_dst) REFERENCES neuronio(id)
);

INSERT INTO grafo VALUES (1,1,3,0);
INSERT INTO grafo VALUES (1,2,3,0);
INSERT INTO grafo VALUES (1,3,5,0);
INSERT INTO grafo VALUES (1,4,5,0);
