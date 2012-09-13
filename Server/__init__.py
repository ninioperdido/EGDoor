#!/usr/bin/python

#  Copyright (C) 2012 Gerardo Puerta <gerardo@gnuardo.com>

## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.

## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.

## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

def main():
  try:
    # Imports
    import sys
    import acl
    import logging
    # Global configuration
    logging.basicConfig(filename='./.EGDoor.log', level=logging.INFO, format='%(asctime)s %(levelname)s : %(message)s')
    logging.info('Started')
    open_by_default = True
    
    # acl structure
    # try to load acl structure
    try:
      egd_acl = acl.Config.load("Acl")
    except Exception as e:
      logging.warning("Except loading previous acl configuration, creating a new one %s" % e)
      egd_acl = default_acl()

    tcommand = ThreadCommand(logging)
    tcommand.setDaemon(True)
    tcommand.start()
   
    tsearch = ThreadSearch(logging)
    tsearch.setDaemon(True)
    tsearch.start()
 
    
    while(True):
      pass
  except KeyboardInterrupt:
    logging.exception('Finished due to:')
    sys.exit(0)

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
  
  def __init__(self, logging):
    threading.Thread.__init__(self)
    self.logging = logging

  def run(self):
    import btserver
    self.logging.info("Starting Command BT Server ...")
    bts = btserver.BTServer()

import acl
def default_acl():
  defaultacl = acl.Acl()
  # roles
  user = acl.Role("user")
  defaultacl.set_role(user)
  
  manager = acl.Role("manager")
  defaultacl.set_role(manager)

  admin = acl.Role("admin")
  defaultacl.set_role(admin)

  # resources
  lock = acl.Resource("lock")
  lock.set_privilege("open")
  lock.set_privilege("close")

  admin.set_parent(manager)
  manager.set_parent(user)

  user.set_resource(lock)
  return defaultacl

if __name__ == "__main__":
  main()

