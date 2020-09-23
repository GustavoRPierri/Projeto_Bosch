#!pip install paho-mqtt
import paho.mqtt.client as mqtt

L = []

def on_connect(client, userdata, flags, rc): # FUNÇÃO DE CALL BACK, ELA IRÁ NOS RETORNAR SE A CONEXÃO COM O BROKER ESTA VIAVEL OU NÃO.
    if rc ==0: # 'rc' = RESULTADO DA CONEXÃO, RETORNA UM VALOR CORRESPONDENTE AO STATUS DA CONEXÃO ATUAL.
        print('connected OK') # 'rc' = 0 (C)
    else:
        print('Bad connection, Returned code =',rc)

def on_message(client, userdata, message):
    msg = str(message.payload.decode("utf-8"))
    L.append(msg)   
    return msg

########################################################

client = mqtt.Client("Computador") # CRIA UM NOME PARA O CLIENTE QUE ACESSAR O BROKEN PELO PYTHON. 

client.on_connect=on_connect

client.on_message=on_message

client.connect("192.168.15.3") # CONECTA AO BROKER.
# '192.168.15.3' = endereço IP do broker remoto.
# '1883' = porta de rede do host.
# '60' = período máximo em segundos.

client.loop_start()

client.subscribe("topico_teste")
client.publish("topico_teste","Helow Word")
print(L)
L = []

#########################################################

client.loop_stop()

client.disconnect() # DISCONECTA DO BROKER.

############################################################################################