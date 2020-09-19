#!pip install paho-mqtt

import paho.mqtt.client as mqtt
import time

def on_connect(client, userdata, flags, rc): # FUNÇÃO DE CALL BACK, ELA IRÁ NOS RETORNAR SE A CONEXÃO COM O BROKER ESTA VIAVEL OU NÃO.
    if rc ==0: # 'rc' = RESULTADO DA CONEXÃO, RETORNA UM VALOR CORRESPONDENTE AO STATUS DA CONEXÃO ATUAL.
        print('connected OK') # 'rc' = 0 (C)
    else:
        print('Bad connection, Returned code =',rc)

client = mqtt.Client("Computador") # CRIA UM NOME PARA O CLIENTE QUE ACESSAR O BROKEN PELO PYTHON. 

client.on_connect=on_connect

cliente.loop_start()

client.connect("192.168.15.3", 1883, 60) # CONECTA AO BROKER.
# '192.168.15.3' = endereço IP do broker remoto.
# '1883' = porta de rede do host.
# '60' = período máximo em segundos.

cliente.publish("topico_teste","Helow Word")
time.sleep(4)

cliente.loop_stop()

cliente.disconnect() # DISCONECTA DO BROKER.
