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

__all__ = ["systemconfig", "utils", "btserver", "acl", "threads", "rpi"]

import logging
import traceback
import os
import sys

logging.basicConfig(filename=os.path.expanduser('~')+'/.egd.log', level=logging.INFO, format='%(asctime)s %(levelname)s : %(message)s')


if __name__ == '__main__':

  logging.info('EGDoor Server Started')
  try:
    # Imports
    import acl
    import utils
    import systemconfig
    import threads
    import door
    import rpi
    # Global configuration
    open_by_default = True
    
    # acl structure
    # try to load acl structure
    try:
      al = config.load("Acl")
    except Exception as e:
      logging.warning("Except loading previous acl configuration, creating a new one %s" % e)
      al = acl.default_acl()

    sc = systemconfig.systemconfig()
    dr = door.door()
    tc = threads.ThreadCommand(logging, dr)
    tc.setDaemon(True)
    tc.start()
   
    #tsearch = threads.ThreadSearch(logging)
    #tsearch.setDaemon(True)
    #tsearch.start()
 
    
    while(True):
      pass
  except KeyboardInterrupt:
    logging.exception('EGDoor Server Finished due to:')
    sys.exit(0)

  finally:
    config.save(al)

if __name__ == "__main__":
  main()

