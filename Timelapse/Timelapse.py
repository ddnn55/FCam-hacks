#!/usr/bin/env python

EXPOSURE_POWER_MIN = 5 # .0000002 second
EXPOSURE_POWER_MAX = 21 # 2 seconds
GAIN = 1.0
WHITE_BALANCE = 4100
DIOPTER = 0.0

import time
import datetime
import ssh
import sys
import os

def unix_time():
   return int(time.mktime(datetime.datetime.now().timetuple()))

def expose(n, exposure, gain, focus, white_balance):

   destination_dir = 'Images/' + str(exposure) + '/' + str(white_balance)
   if not os.path.exists(destination_dir):
      os.makedirs(destination_dir)

   phone = ssh.Connection(host='192.168.2.15', username='user', password='user')
   command = '/opt/viewfinder/bin/viewfinder %s %s %s %s' % (exposure, gain, focus, white_balance)
   print command
   phone.execute('/usr/bin/killall -9 viewfinder')
   phone.execute(command)
   phone.get('/home/user/frame.jpg', destination_dir + '/' + str(unix_time()) + '-' + str(n) + '.jpg')
   phone.close()
	    

if __name__ == "__main__":

   interval = sys.argv[1]
   print "Taking bracketed exposures every " + str(interval) + " seconds"


   n = 0
   while True:
      for exposure_power in range(EXPOSURE_POWER_MIN, EXPOSURE_POWER_MAX+1):
         expose(n, pow(2, exposure_power), GAIN, DIOPTER, WHITE_BALANCE)
      n = n + 1
      time.sleep(float(interval))
