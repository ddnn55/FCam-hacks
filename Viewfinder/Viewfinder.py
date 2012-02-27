#!/usr/bin/env python

import web
import threading
import time
import datetime
import ssh
import Queue
import sys

q = Queue.Queue(maxsize=0)

class Expose():

    def GET(self, exposure, gain, focus, white_balance, no_cache_salt):

	    phone = ssh.Connection(host='192.168.2.15', username='user', password='user')
	    command = '/opt/viewfinder/bin/viewfinder %s %s %s %s' % (exposure, gain, focus, white_balance)
	    print command
	    phone.execute('killall -9 viewfinder')
	    phone.execute(command)
	    phone.get('/home/user/frame.jpg', 'static/frame.jpg')
	    phone.close()
	    
	    return file('static/frame.jpg')



urls = (
    '/', 'index',
    '/expose/(.+)/(.+)/(.+)/(.+)/(.+)', 'Expose',
    '/frame/(.+)', 'frame'
)

class index:
    def GET(self):
        return file('static/index.html')

class set_params:
    def GET(self, exposure, gain, focus, white_balance):

        params = {}
        params['exposure']      = exposure
        params['gain']          = gain
        params['focus']         = focus
        params['white_balance'] = white_balance

        print params

        q.put(params)
        print q.qsize()

        return "ok"

class frame:
    def GET(self, unique_no_cache):
        return file('static/frame.jpg')



if __name__ == "__main__":
    app = web.application(urls, globals())
    app.internalerror = web.debugerror
    app.run()
