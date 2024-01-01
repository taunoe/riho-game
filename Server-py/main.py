# Game Server
# main.py
# Tauno Erik
# Started: 18.12.2023
# Edited:  21.12.2023
# https://bytesnbits.co.uk/multi-thread-coding-on-the-raspberry-pi-pico-in-micropython/

import network
import time
import socket
from machine import Pin, Timer
import _thread

import gc  # garbage collector
gc.collect()

# Pins
LED_1_PIN = Pin(6, Pin.OUT)  # GPIO6
LED_2_PIN = Pin(7, Pin.OUT)
LED_3_PIN = Pin(8, Pin.OUT)
LED_4_PIN = Pin(9, Pin.OUT)
LED_5_PIN = Pin(10, Pin.OUT)

def tick(timer):
    print("L1")
    global LED_1_PIN
    LED_1_PIN.toggle()


def web_page():
    """
    Returns HTML string
    """
    html = """<html><head><meta name="viewport" content="width=device-width, initial-scale=1"></head>
            <body><h1>Game Server</h1></body></html>
         """
    return html

def parse_request(request):
    """
    Parse the HTTP request to extract parameters.
    """
    parts = request.split()
    if len(parts) >= 2:
        method = parts[0]
        path = parts[1]
        return method, path
    return None, None

def find_id(request):
    request = str(request)
    method, path = parse_request(request)
    print("path:")
    print(path)

    params_start = path.find('?')
    print("params_start:")
    print(params_start)
    if params_start == 1:
        query_string = path[params_start + 1:]

        key, value = query_string.split('=')

        if key == "id":
            response_content = f"ID value is {value}"
            if value == '1':
                print("LED_1_PIN")
                LED_1_PIN.value(1)
            elif value == '2':
                print("LED_2_PIN")
                LED_2_PIN.value(1)
            elif value == '3':
                print("LED_3_PIN")
                LED_3_PIN.value(1)
            elif value == '4':
                print("LED_4_PIN")
                LED_4_PIN.value(1)
            elif value == '5':
                print("LED_5_PIN")
                LED_5_PIN.value(1)
            else:
                print("wrong id")
        else:
            response_content = "Bad key"
    else:
        response_content = "404 Not Found"
    print(response_content)
    return 0



def ap_mode(ssid, password):
    """
    Description: Activates Access Point (AP) mode
    Parameters:
        ssid[str]: network name
        password[str]: password
    Returns: Nada
    """
    # creating ap object
    ap = network.WLAN(network.AP_IF)
    ap.config(essid=ssid, password=password)
    ap.active(True)
    # 1 - static IP address
    # 2 - Subnet mask
    # 3 - Gateway IP address, representing the device connecting the local network to the internet.
    # 4 - DNS server
    ap.ifconfig(('192.168.4.1', '255.255.255.0', '192.168.4.1', '8.8.8.8'))

    while ap.active() == False:
        # oota kuni aktiveerub
        pass
    
    print('AP Mode Is Active')
    print('IP Address:: ' + ap.ifconfig()[0])
    
    # creating socket object
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(('', 80))  # ip, port
    s.listen(5)       # kuni 5 seadet

    # Handle incoming connections
    while True:
      conn, addr = s.accept()
      print('Client %s' % str(addr))
      
      request = conn.recv(1024) # max data that can be received at once
      #request = str(request)
      print('Content = %s' % request)

      player_id = find_id(request)
      
      response = web_page()
      conn.send('HTTP/1.1 200 OK\n')
      conn.send('Content-Type: text/html\n')
      conn.send('Connection: close\n\n')
      conn.sendall(response)
      
      conn.close()



# main thread
def core0_thread():
    # Loo võrk
    ap_mode('Game', 'Tauno Erik')  # ssid, pass


def core1_thread():
    pass

########
second_thread = _thread.start_new_thread(core1_thread, ())

core0_thread()

