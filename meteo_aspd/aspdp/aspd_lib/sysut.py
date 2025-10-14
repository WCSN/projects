'''
Created on 21 окт. 2019 г.

@author: wocson
'''
import logging
import socket, sys, os

from aspd_lib.sysio import shellcmd

prgname=os.path.basename(sys.argv[0])
pid='/var/run/'+prgname+'.pid'
#pid='./'+prgname+'.pid'
    
logger=logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)
formatter=logging.Formatter('%(asctime)s %(name)s %(levelname)s: %(message)s')
logger.propagate=False
fh=logging.FileHandler("/var/log/aspd/"+prgname+".log", "a")
#fh=logging.FileHandler("./"+prgname+".log", "w")
fh.setLevel(logging.DEBUG)
fh.setFormatter(formatter)
logger.addHandler(fh)
keep_fds=[fh.stream.fileno()]

def parse(conn, typepipe):

    data=b""
    while not b"\r\n" in data:
        tmp=conn.recv(1024)
        if not tmp: 
            break
        else:
            data+=tmp
    
    if not data: return
  
    dtdecode=data.decode("utf-8")
    commands=dtdecode.split(';')
    cmds=''
    
    for cmd in commands:
        if cmd in ('bye', 'close', 'quit', 'exit'):
            return 'close'
        elif not cmd in ('', ' ', '\r\n'):
            cmds+=cmd.strip()+";"

    for fnd in ('mixer', 'RHVoice', 'aspd'):
        if cmds.find(fnd)!=-1:
            ret=shellcmd(cmds, '/dev/null', 'w', typepipe)
            logger.debug(cmd)
            logger.debug(ret)
            break
            
    return None

def AServer(Ip, Port, typepipe='ORDER'):
    
    sock=socket.socket()
    sock.bind((Ip, Port))
    sock.listen(5)
    
    try:
        while True: 
            conn, addr=sock.accept()
            try:
                if parse(conn, typepipe)=='close':
                    break;
            except:
                logger.error("Socket Error")
            finally:
                logger.info("Connction Close")
                conn.close()
                
    finally: 
        sock.close()
        logger.info("Done")

def ASender(ip, port, cmd):
    sock=socket.socket()
    sock.connect((ip, port))
    sock.settimeout(5)
    sock.send(bytes(cmd, 'utf-8'))
    sock.close()
