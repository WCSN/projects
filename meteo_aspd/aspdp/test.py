#!/bin/python
'''
Created on 17 нояб. 2019 г.

@author: wocson
'''

import os, locale, sys
import chardet

def StrUTF8(bstr): #bytes
    locale.setlocale(locale.LC_ALL, "ru_RU.UTF-8")
    if len(bstr)==0: return str(bstr).replace("b'", '').replace("'", '')
    enc=chardet.detect(bstr).get('encoding')
    if enc!=None:
        if enc.lower()=='ascii':
            cnvstr=str(bstr)
        elif enc.lower()=='utf-8':
            cnvstr=bstr.decode('utf-8')
        elif enc.lower()=='windows-1253':
            cnvstr=bstr.decode('cp866')
        elif enc.lower()!='utf-8':
            cnvstr=bstr.decode('utf-8')
        else:
            cnvstr=str(bstr)

    return cnvstr.replace("b'", '').replace("'", '')

def psecho(prompt):
    sys.stdout.write(prompt)
    return StrUTF8(bytes(input(), 'utf-8')).strip()

def main():
    
    
    print ("-h - help, 1 - 1, 2 -2")
    
    
    asdf=psecho("Отправить сообщение?(y/n/редактировать(e)):")
    
    print(asdf)

if __name__=='__main__':
    main()
    pass
