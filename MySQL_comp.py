#!pip install PyMySQL

import pymysql

conn = pymysql.connect(host='127.0.0.1', user='root', passwd=None, database='galpao_bosch');

cur = conn.cursor()

cur.execute('SELECT * FROM  galpao')

row = cur.fetchall()
    
cur.execute("INSERT INTO galpao(produto,quantidade) VALUES('BROCA','8')")

conn.commit()

cur.close()
conn.close()
