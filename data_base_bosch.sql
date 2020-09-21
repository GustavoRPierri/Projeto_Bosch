CREATE DATABASE galpao_bosch;

use galpao_bosch;

CREATE TABLE GALPAO(
	ID bigint(7) PRIMARY KEY AUTO_INCREMENT,
	PRODUTO varchar(30),
    QUANTIDADE int
    );
    
DESCRIBE galpao;

SELECT * FROM galpao;

DROP DATABASE galpão_bosch;