from subprocess import call
from utils import config
import logging

# Class that holds the underlying
# OS configuration when the system
# starts

class systemconfig(object):
  def loader(self):
    data = config.load("systemconfig")

  def __init__(self):
    try:
      data = {}
      systemconfig.loader()

    except Exception as e:
      logging.error("Exception loading previous configuration: %s" % (e))

    self.configure()
    logging.info("EGDoor.systemconfig configuration loaded")

  def configure(self):
    try:
      for key, value in self.__dict__.iteritems():
        try:
          if key in 'OS':
            call([item])
          else:
            key = value
        except Exception as e:
          logging.error("Exception configuring item: %s, %s" % (item, e)) 
    finally:
      data['configured'] = True

  def __str__(self):
    str = "systemconfig object: \n"
    for key, value in self.data.iteritems():
      str += "%s: %s\n" % (key, value)
    return str

  def __setattr__(self, name, value):
    if name != "configured":
      self.configured = False
    self.data[name] = value

  def __del__(self):
    config.save(data, systemconfig)

