#!/bin/python
import os, sys, time, signal      

XORG_DELAY = 0

if len(sys.argv) == 1 or (len(sys.argv)==2 and sys.argv[1].startswith('-x:')):
    print("Usage: lnX [-x:id] app [args]")
    sys.exit(2)
if sys.argv[1].startswith('-x:'):
    xid = int(sys.argv[1].split(':')[1])
    argv = sys.argv[2:]
else:
    xid = 1
    argv = sys.argv[1:]

pid = os.fork()
if pid == 0:
    # Xorg child
    print (" * Staring X11 on :%d",xid)
    sys.stdin.close()
    sys.stdout.close()
    sys.stderr.close()
    os.execvp('Xorg', ['/usr/bin/Xorg -listen tcp', ':%d', xid])
else:
    print (" * Waiting for X11 to finish starting (%d seconds)", XORG_DELAY)
    time.sleep(XORG_DELAY)
    cmd = 'env DISPLAY=":' + str(xid) + '" ' + ' '.join(argv)
    print (" * Starting %s",cmd)
    os.system(cmd)
    print (" * Application done, terminating spawned X11")
    os.kill(pid, signal.SIGTERM)
    os.wait()
