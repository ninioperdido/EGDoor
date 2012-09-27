import commands
import logging
from utils import config

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
    for item in dir(self):
      if not item.startswith("__"):
        try:
            if item.startswith('OS'):
              if debug:
                logging.info("EGDoor.systemconfig, executing command: %s" % getattr(self,item))
              output = (commands.getstatusoutput(getattr(self,item)))
              if output[0] != 0:
                logging.error("EGDoor.systemconfig, error executing last command.")
        except Exception as e:
          logging.error("EGDoor.systemconfig Exception configuring item: %s, %s" % (item, e)) 
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

