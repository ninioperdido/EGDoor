from bluetooth import *
import logging 

class BTServer(object):
  def __init__(self):
    
    server_sock=BluetoothSocket(RFCOMM)
    server_sock.bind(("", PORT_ANY))
    server_sock.listen(1)
    
    port = server_sock.getsockname()[1]
    #TODO: Ex the command uuidgen for every machine
    uuid = "4fa05998-47e1-4f0a-9d87-f84c58cc1401"
    advertise_service( server_sock, 
                       "EGDoor BT Lock",
                       service_id = uuid,
                       service_classes = [uuid, SERIAL_PORT_CLASS],
                       profiles = [SERIAL_PORT_PROFILE])
    
    print "Waiting for connection on RFCOMM channel %d" % port
    client_sock, client_info = server_sock.accept()
    print "Accepted connection from ", client_info
    
    try:
      while True:
        data = client_sock.recv(1024)
        if len(data) == 0: break
        print "received [%s]" % data
    except IOError:
      pass
    
    #TODO: Main loop of the server
    client_sock.close()
    server_sock.close()
    print ("Exited connection from BT Server")
    
