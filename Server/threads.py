import threading 
class ThreadSearch(threading.Thread):

  def __init__(self, logging):
    threading.Thread.__init__(self)
    self.logging = logging

  def run(self):
    print "Hello World, I'm searching!"
    #while(True):
    #  pass


class ThreadCommand(threading.Thread):

  def __init__(self, logging, door):
    threading.Thread.__init__(self)
    self.logging = logging
    self.door = door

  def run(self):
    import btserver
    self.logging.info("Starting Command BT Server ...")
    self.btserver = btserver.BTServer(self.door, self.logging)

