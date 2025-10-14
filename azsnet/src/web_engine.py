#!/usr/bin/python
# -*- coding: utf-8 -*-
'''
Created on Dec 9, 2011

@author: wocson
'''
from wcpack.wctools import Message
import cherrypy

class SimpleApp:
    @cherrypy.expose
    def say( self ):
        return 'hello'

if __name__ == '__main__':

    global_conf = {'global':{'server.socket_host':'localhost', 'server.socket_port':8080, }}
    cherrypy.config.update( global_conf )
    app = SimpleApp()
    cherrypy.tree.mount( app, '/' )
    cherrypy.quickstart()
    cherrypy.engine.start()
