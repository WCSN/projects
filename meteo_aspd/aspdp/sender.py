#!/bin/python
'''
Created on 26 сент. 2019 г.

@author: wocson
'''

import sys
from aspd_lib.sysut import ASender
    
def main():
    argc=len(sys.argv)
    ip=cmd=''; ip=0
    if argc==1 and argc<3 : 
        print("usage: serder IP PORT cmd\ncmd: mixer, RHVoice-test, aspd, only.")
        return 1
    else:
        ip=sys.argv[1]
        port=int(sys.argv[2])
        cmd=sys.argv[3]
        ASender(ip, port, cmd)
        
    return 0

if __name__=='__main__':
    main()
    
