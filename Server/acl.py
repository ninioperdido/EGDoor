"""Role based security"""
# Many thanks to Dave Thompson
# www.limscoder.com/2009/11/role-based-security-with-python.html

class AccessError(Exception):
    pass

from datetime import datetime, time, date
from utils import config

class TimedAccess(object):
    """A TimedAccess is an object that defined when a Role can access to a Resource."""
    def __init__(self, name='', since=None, to=None):
      
      self.name = name
                       #Monday                                 
      self._weekdays = {0: {time(0, 0): True, time(23,59): True},
                        1: {time(0, 0): True, time(23,59): True},
                        2: {time(0, 0): True, time(23,59): True},
                        3: {time(0, 0): True, time(23,59): True},
                        4: {time(0, 0): True, time(23,59): True},
                        5: {time(0, 0): True, time(23,59): True},
                        6: {time(0, 0): True, time(23,59): True},}
                       #Sunday
      now = date.today()
      max = date.max
      if since == None:
        self._since = now
      else:
        assert type(since) == type(date.today())
        self._since = since

      if to == None:
        self._to = max
      else:
        assert type(to) == type(date.today())
        self._to = to
      
      print self
    
    def set_day(self, dow=7, hoursperday={time(0, 0): True, time(23,59): True}):
      assert 0 <= dow <= 7
      for time, access in hoursperday.iteritems():
        assert access == True or access == False

      if dow == 7:
        for weekday in iter(self._weekdays):
          self._weekdays[weekday] = hoursperday

      else:
        self._weekdays[dow] = hoursperday

    def has_access(self):
      haccess = False
      now = date.today()
      if now >= self._since and now <= self._to:
        dt = now.timetuple()
        dow = self._weekdays[getattr(dt,'tm_wday')]
        now = datetime.now().time()
        for time, access in dow.iteritems():
            if access and time <= now:
              haccess = True
            if not access and time >= now:
              haccess = False
      return haccess
  
    def __str__(self):
      rpr = self.name + ': '
      for weekday, hoursperday in self._weekdays.iteritems():
        rpr += "%s:%s " % (weekday, hoursperday)
      rpr += "Since:%s " % (self._since)
      rpr += "To:%s " % (self._to)
      return rpr


class Resource(object):
    """An Resource is an object that can be accessed by a Role."""
    def __init__(self, name=''):
        self.name = name
        self._privileges = {}
        self._timedaccess = {}
 
    def set_privilege(self, privilege, allowed=True):
        self._privileges[privilege] = allowed

    def set_timedaccess(self, privilege, timedaccess):
        self._timedaccess[privilege] = timedaccess
 
    def has_access(self, privilege):
        if privilege in self._privileges \
           and privilege in self._timedaccess:
          return self._privileges[privilege] \
                 and self._timedaccess[privilege].has_access()
        if privilege in self._privileges:
            return self._privileges[privilege]
        return False
     
    def __str__(self):
        rpr = self.name + ': '
        for privilege, access in self._privileges.iteritems():
            rpr += "%s:%s " % (privilege, access)
        for privilege, timedaccess in self._timedaccess.iteritems():
            rpr += "%s:%s " % (privilege, timedacess)
        return rpr
 
class Role(object):
    def __init__(self, name=''):
        """An Acl role has access to resources with specific privileges."""
        self.name = name
        self._parents = {}
        self._resources = {}
 
    def set_parent(self, parent):
        self._parents[parent.name] = parent
 
    def set_resource(self, resource):
        self._resources[resource.name] = resource
 
    def has_access(self, attr_name, privilege):
        if attr_name in self._resources:
            if self._resources[attr_name].has_access(privilege):
                return True
 
        for parent in self._parents.values():
            if parent.has_access(attr_name, privilege):
                return True
 
        return False
     
    def __str__(self):
        rpr = self.name + ":\n"
        rpr += "parents:\n"
        for parent in self._parents.keys():
            rpr += "\t%s\n" % parent
        rpr += "resources:\n"
        for resource in self._resources.values():
            rpr += "\t%s\n" % resource
        return rpr


class Acl(object):
    """Manages roles and resources."""
    def __init__(self):
        self._acl = {}

    def set_role(self, role):
        self._acl[role.name] = role
         
    def check_access(self, role_name, resource, privilege):
        """Check whether a role has access to a resource or not."""

        if not role_name in self._acl:
            raise AccessError('Role does not exist.')
        return self._acl[role_name].has_access(resource, privilege)


    def _create_role(self, role_name, roles_to_create):
        """Recursively create parent roles and then create child role."""
         
        if role_name in self._acl:
            role = self._acl[role_name]
        else:
            role = Role()
            role.name = role_name
             
        for parent_name in roles_to_create[role_name]:
            if parent_name in self._acl:
                parent = self._acl[parent_name]
            else:
                parent = self._create_role(parent_name, roles_to_create)
                self.set_role(parent)
            role.set_parent(parent)
        return role
     
    def __str__(self):
        rpr = ''
        for role in self._acl.values():
            rpr += '----------\n'
            rpr += str(role)
        return rpr

    def Role(self, name):
      self._acl[name] = Role(name)
      return self._acl[name]

    def Resource(self, name):
      return Resource(name)

