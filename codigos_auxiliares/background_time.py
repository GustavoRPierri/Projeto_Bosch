import threading
import time

global var

var=False

def funcao():
    global x
    while(var == False):
        x = 2
        time.sleep(15)
        print('time out')

th = threading.Thread(target=funcao, name='funcao')  # CONFIGURA A THREAD QUE VAMOS UTILIZAR
# TARGET -> FUNÇÃO QUE DESEJAMOS RODAR EM SEGUNDO PLANO
# NAME -> NOME QUE VAMOS DAR A ESSA THREAD
# DEAMON -> PARAR ESSA THREAD NO MOMENTO QUE PARARMOS A EXECUÇÃO DE TODO ESSE SCRIPT PYTHON 

th.start() # INICIAR A THREAD

for i in range(0,1000000):
    print('time')
    time.sleep(2)
 
var = True # STOP LOOP DA FUNÇÃO QUE ESTA DENTRO DA THREAD, QUANDO ELE PARA, A THREAD PARA AUTOMATICAMENTE

th.is_alive() # VER SE A THREAD AINDA ESTA EM EXECUÇÃO OU NÃO

