from bluetooth import *
import logging 

class BTServer(object):
  def __init__(self, door, logging):
    server_sock=BluetoothSocket(RFCOMM)
    #server_sock.bind(("", PORT_ANY))
    # because we want always port 1, at least 
    # for testing
    server_sock.bind(("", 1))
    server_sock.listen(1)
    self.door = door
    self.logging = logging
    port = server_sock.getsockname()[1]
    #TODO: Ex the command uuidgen for every machine
    uuid = "4fa05998-47e1-4f0a-9d87-f84c58cc1401"
    advertise_service( server_sock, 
                       "EGDoor BT Lock",
                       service_id = uuid,
                       service_classes = [uuid, SERIAL_PORT_CLASS],
                       profiles = [SERIAL_PORT_PROFILE])
    
    self.logging.info ("btserver: Waiting for connection on RFCOMM channel %d" % port)
    client_sock, client_info = server_sock.accept()
    self.logging.info ("btserver: Accepted connection from %s" % client_info[0] + client_info[1])
    # True when we're inside command mode
    # Exit from it with 'E'
    manage_mode = False
    command_mode = False
    try:
     
      while True:

        while True:
          data = client_sock.recv(1024)
          if len(data) == 0: break
          if manage_mode:
            # TODO
            if (data == 'E') :
              self.logging.info("btserver: Exiting manage mode")
              manage_mode = False
      
          elif command_mode:
            if ( data == 'E') : 
              self.logging.info ("btserver: Exiting command mode")
              command_mode = False
            if ( data == 'L') : 
              self.logging.info ("btserver: Linking new device")
            if ( data == 'D') :
              self.logging.info ("btserver: Deleting device")
            if ( data == 'O') :
              self.logging.info ("btserver: Opening lock")
              self.door.open()
            if ( data == 'C') :
              self.logging.info ("btserver: Closing lock")
              self.door.close()
            if ( data == 'M') :
              self.logging.info ("btserver: Entering manage mode ...")
              manage_mode = True
          
          else:
            if ( data == 'O' ): self.door.open()
            if ( data == 'C' ):
              self.logging.info ("btserver: Entering command mode ...")
              command_mode = True
            if ( data == 'E' ):
              self.logging.info ("btserver: Terminating connection ...")
              
          self.logging.info ("btserver: received [%s]" % data)
     
      #TODO: Main loop of the server
      client_sock.close()
      server_sock.close()
      self.logging.info ("btserver: Exited connection from BT Server")

    except IOError:
      pass
      
