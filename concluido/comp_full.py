#!pip install PyMySQL
#!pip install paho-mqtt
import pymysql
import time
import pandas as pd
import threading
import paho.mqtt.client as mqtt
from sqlalchemy.exc import DatabaseError, InterfaceError
############################## CONEXÃO COM O XAMPP ############################## 

global var
var=False

def atualizacao():
    global conn, cur
    while(var == False):
        conn = pymysql.connect(host='127.0.0.1', user='root', passwd=None, database='galpao_bosch');
        cur = conn.cursor()
        cur.execute('USE galpao_bosch')
        cur.execute('SELECT * FROM  GALPAO')
        print('Banco de dados atualizado!')
        time.sleep(25)
        
def desconectar():
    cur.close()
    conn.close()
    client.loop_stop()
    client.disconnect() # DISCONECTA DO BROKER.
    var = True # STOP LOOP DA FUNÇÃO QUE ESTA DENTRO DA THREAD, QUANDO ELE PARA, A THREAD PARA AUTOMATICAMENTE
    print(th.is_alive()) # VER SE A THREAD AINDA ESTA EM EXECUÇÃO OU NÃO
    
th = threading.Thread(target=atualizacao, name='atualizacao')  # CONFIGURA A THREAD QUE VAMOS UTILIZAR
# TARGET -> FUNÇÃO QUE DESEJAMOS RODAR EM SEGUNDO PLANO
# NAME -> NOME QUE VAMOS DAR A ESSA THREAD
# DEAMON -> PARAR ESSA THREAD NO MOMENTO QUE PARARMOS A EXECUÇÃO DE TODO ESSE SCRIPT PYTHON 

th.start() # INICIAR A THREAD

#var = True # STOP LOOP DA FUNÇÃO QUE ESTA DENTRO DA THREAD, QUANDO ELE PARA, A THREAD PARA AUTOMATICAMENTE
#th.is_alive() # VER SE A THREAD AINDA ESTA EM EXECUÇÃO OU NÃO

############################## CONEXÃO COM O MQTT ############################## 

L = []

def on_connect(client, userdata, flags, rc): # FUNÇÃO DE CALL BACK, ELA IRÁ NOS RETORNAR SE A CONEXÃO COM O BROKER ESTA VIAVEL OU NÃO.
    if rc ==0: # 'rc' = RESULTADO DA CONEXÃO, RETORNA UM VALOR CORRESPONDENTE AO STATUS DA CONEXÃO ATUAL.
        print('connected OK') # 'rc' = 0 (C)
    else:
        print('Bad connection, Returned code =',rc)

def on_message(client, userdata, message):
    msg = str(message.payload.decode("utf-8"))
    L.append(msg)   

client = mqtt.Client("Computador") # CRIA UM NOME PARA O CLIENTE QUE ACESSAR O BROKEN PELO PYTHON. 

client.on_connect=on_connect

client.on_message=on_message

client.connect("192.168.15.3") # CONECTA AO BROKER.
# '192.168.15.3' = endereço IP do broker remoto.

client.loop_start()

client.subscribe("recebe")


############################## PROCURANDO NO BANCO DE DADOS ############################## 
L = []
 
time.sleep(1) # TEMPO QUE DEMORA PARA FAZER A CONEXAO COM O BANCO DE DADOS

while(1):  
    if len(L) >= 1: 
        print(L[0])
        try:
            df = pd.read_sql("SELECT * FROM GALPAO WHERE PRODUTO = '%s'" % L[0], conn)
            prod = df.iloc[0,1]
            qnt = str(df.iloc[0,2])
            data = str(df.iloc[0,3])
            hora = str(df.iloc[0,4])[7:]
            client.publish("envia", prod + ';' + qnt + ';' + data + ';' + hora + ';')
            L = []
        except (DatabaseError, InterfaceError,IndexError):
            client.publish("envia","Nao encontrado;")
            L = []
