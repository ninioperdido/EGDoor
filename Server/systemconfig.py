import subprocess, shlex, logging, time
from utils import config
from collections import OrderedDict

# Class that holds the underlying
# OS configuration when the system
# starts

debug = True

class systemconfig(object):
  def loader(self):
    temp  = config.load("systemconfig")
    for k, v in temp.iteritems():
      self.__setattr__(k,v)

  def __init__(self):
    try:
      systemconfig.loader(self)

    except Exception as e:
      logging.error("EGDoor.systemconfig Exception loading previous configuration: %s" % (e))

    self.configure()
    logging.info("EGDoor.systemconfig configuration loaded")

  def configure(self):
    d = {}

    for item in dir(self):
      if not item.startswith("__"):
        command = getattr(self,item)
        try:
            if item.startswith('OS'): # We're xcuting OS customization
              if debug:
                logging.info("EGDoor.systemconfig, executing command: %s" % command)
              output = subprocess.check_call(command, shell=True)
              if output != 0:
                logging.error("EGDoor.systemconfig, error executing last command: %s" % command)
            elif item.startswith('DN'): # We're launching a daemon, but in order please ...
              d[item] = command
        except Exception as e:
          logging.error("EGDoor.systemconfig Exception configuring item: %s, %s" % (item, e)) 
    
    od = OrderedDict(sorted(d.items(), key=lambda t: t[0]))
    for item,command in od.iteritems():
      try:
        if debug:
          logging.info("EGDoor.systemconfig, launching daemon: %s" % command)
        name = "PR_" + item
        command = shlex.split(command)
        setattr(self, name, subprocess.Popen(command))
        time.sleep(1) # Time for the BT daemon to start
      except Exception as e:
        logging.error("EGDoor.systemconfig Exception launching daemon item: %s, %s" % (item, e)) 
     
    self.configured = True

  def __str__(self):
    import operator
    str = "systemconfig object: \n"
    sorted_dict = sorted(self.__dict__.iteritems(), key=operator.itemgetter(1))
    for item in sorted_dict:
      str += "%s: %s\n" % (item[0], item[1])
    return str

  def __setattr__(self, name, value):
    if name != "configured":
      self.__dict__['configured'] = False
    self.__dict__[name] = value

  def __delattr__(self, name):
    if name != "configured":
      self.__dict__['configured'] = False
      del self.__dict__[name]

  def __del__(self):
    config.save(self.__dict__, 'systemconfig')
    d = {}
    for item in dir(self):
      if not item.startswith("__"):
        if item.startswith("PR_"):
          d[item] = getattr(self,item)
    od = OrderedDict(sorted(d.items(), key=lambda t: t[0]))
    items = od.items() # Because we terminate in reverse order
    items.reverse()
    rod = OrderedDict(items)
    for item, value in rod.iteritems():
      try:
        output = subprocess.check_call("sudo kill " + str(value.pid), shell=True)
        time.sleep(1)
        if output != 0:
          logging.error("EGDoor.systemconfig, error executing last command: %s" % command)
      except Exception as e:
        logging.error("EGDoor.systemconfig Exception terminating daemon item: %s, %s" % (item, e))

