#!/bin/python
'''
Created on 26 сент. 2019 г.

@author: wocson
'''
import sys

from aspd_lib.sysut import AServer, logger, keep_fds, prgname, pid
from daemonize import Daemonize as Dm

IPDEF='172.31.83.13'
PORTDEF=7775

def main():
    argc=len(sys.argv)
   
    logger.info("Start "+prgname)
    if argc==1:
        logger.info("usage: "+prgname+" [ip] [port]")
        ip=IPDEF; port=PORTDEF
        logger.info("accept IP: "+ip)
        logger.info("accept Port: "+str(port))
    elif argc==2:
        ip=sys.argv[1]; port=PORTDEF
        logger.info("accept IP: "+ip)
        logger.info("accept Port: "+str(port))
    elif argc==3:
        ip=sys.argv[1]; port=int(sys.argv[2]);
        logger.info("accept IP: "+ip)
        logger.info("accept Port: "+str(port))

    AServer(ip, port, 'ORDER')
    
if __name__=='__main__':
    daemon=Dm(app=prgname, pid=pid, action=main, keep_fds=keep_fds, foreground=False, logger=logger)
    daemon.start()

