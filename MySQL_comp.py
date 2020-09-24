#!pip install PyMySQL

import pymysql
import time
import pandas as pd

def atualização():
    conn = pymysql.connect(host='127.0.0.1', user='root', passwd=None, database='galpao_bosch');
    cur = conn.cursor()
    cur.execute('USE galpao_bosch')
    cur.execute('SELECT * FROM  GALPAO')

x = 'MARRETA'
df = pd.read_sql("SELECT QUANTIDADE FROM GALPAO WHERE PRODUTO = '%s'" % x, conn)

x = df.iloc[0,0]

def desconectar():
    cur.close()
    conn.close()