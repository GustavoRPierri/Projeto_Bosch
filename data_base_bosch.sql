CREATE DATABASE galpao_bosch;

USE galpao_bosch;

CREATE TABLE GALPAO(
	ID bigint(7) PRIMARY KEY AUTO_INCREMENT,
	PRODUTO varchar(30),
    QUANTIDADE int
    );
    
DESCRIBE GALPAO;

SELECT * FROM GALPAO;

INSERT INTO GALPAO(produto,quantidade) VALUES('MARTELETE','3');

SELECT QUANTIDADE FROM GALPAO WHERE PRODUTO = 'BROCA';