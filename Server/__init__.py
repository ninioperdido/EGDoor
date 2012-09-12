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

import threading
class ThreadSearch(threading.Thread):
  def run(self):
    print "Hello World, I'm searching!"

class ThreadCommand(threading.Thread):
  def run(self):
    print "Hello World, I'm expecting a command!"

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

  # Imports
  import acl
  #from bluetooth import *
  # Global configuration

  open_by_default = True
  
  # acl structure
  # try to load acl structure
  try:
    egd_acl = acl.Config.load("Acl")
  except Exception as e:
    print "Except loading previous acl configuration, creating a new one"
    egd_acl = default_acl()

  print "EGD_ACL: %s" % egd_acl
  print "User has access to open the lock: %s" % egd_acl.check_access('user', 'lock', 'open')
  print "User has access to close the lock: %s" % egd_acl.check_access('user', 'lock', 'close')
  print "Manager has access to open the lock: %s" % egd_acl.check_access('manager', 'lock', 'open')
  print "Manager has access to close the lock: %s" % egd_acl.check_access('manager', 'lock', 'close')


  tsearch = ThreadSearch()
  tsearch.start()
  tcommand = ThreadCommand()
  tcommand.start()


