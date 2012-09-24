import cPickle as pickle
import os
class config(object):
  """ class to dump and load object configuration """
  conf_path = os.path.expanduser("~/")
  conf_ext =  ".egd"

  @staticmethod
  def save(obj):
    pickle.dump(obj, open(config.conf_path+"."+str(type(obj).__name__)+config.conf_ext, "wb"))

  @staticmethod
  def load(tobj):
    return pickle.load(open(config.conf_path+"."+tobj+config.conf_ext, "rb"))
