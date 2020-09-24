import threading
import time

def trigger():
    time.sleep(15)
    print('time out')
    thread.run()
    # do something else here.

thread = threading.Thread(target=trigger)
thread.daemon = False
thread.start()

for i in range(0,1000000):
    print('time')
    time.sleep(2)
