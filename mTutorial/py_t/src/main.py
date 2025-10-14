#!/bin/python
'''
Created on Jul 11, 2016

@author: wocson
'''
import sys
from wsys.sysio import *
import time
import os
import math
from math import floor

# import numpy as np

def hello():
    print("Hello,", input('Whats you name?\nName: '), '!')
    print("Begin work!")
    time.sleep(3)

def prints():

    print(sys.api_version)
    print(sys.argv)
    print(sys.base_exec_prefix)
    print(sys.builtin_module_names)
    print(sys.byteorder)
    print(sys.copyright)
    print(sys.exc_info())
    print(sys.getcheckinterval())
    print(sys.getdefaultencoding())
    print(sys.getprofile())
    print(sys.version)
    print(sys.float_info)
    print(sys.int_info)
    print(sys.thread_info)
    print(sys.hexversion)
    print(sys.implementation)

def coloring():
    colortext(GREEN, BGBLUE)

    cprint(" Hello")
    cprint(" world ")

    endl()

    cprint("end")
    cprint(" first")
    cprint(" second")
    print(" thiest")
    print(" forth")

    for i in 'hello world':
        if i == 'a':
            break
    else:
        print('Буквы a в строке нет')

def fibonachi(numout = 10):
    t0 = time.time()
    cprint(t0); endl()
    colortext(LGREEN)

    print ("Числа Фибоначчи:")
    fbn = fbn1 = 0
    fbn2 = 1

    for i in range(0, numout):
        cprint (i); cprint('. '); print (fbn);
        fbn = fbn1 + fbn2
        fbn1 = fbn2
        fbn2 = fbn

    endl()
    colortext(DEF)
    t1 = time.time()
    cprint(t1); endl()

    colortext(LRED)
    cprint (t1 - t0)
    endl()

def pythagoras(a, b):
    value = math.sqrt(a * a + b * b)
    return value

def fssys(fnd = ' ', tc = LRED, tb = BGBLUE):
    try:
        ls = os.listdir(input('Listing directiry: '))
        ls.sort()

        print (os.getegid())
        print (os.geteuid())
        # ls.reverse()

        for name in ls:

            if name.find(fnd) == 0:
                colortext(tc, tb)
                print (name, end = '')
                endl()

            else:
                colortext(LCYAN, BGBLUE)
                print (name, end = '')
                endl()

        colortext(DEF, BGDEF)
        return 0
    except OSError as err:
        print("{0}".format(err))
        return 1
    except:
        print("Unexpected error:", sys.exc_info()[0])
        raise

def lists():

    c = [1, 2, 3, 4, 5, 6, 7, 8, 9]
    x = (1, 2, 3, 4, 5, 6, 7, 8, 9)
    y = ["list.append(x)    Добавляет элемент в конец списка",
		"list.extend(L)    Расширяет список list, добавляя в конец все элементы списка L",
		"list.insert(i, x)    Вставляет на i-ый элемент значение x",
		"list.remove(x)    Удаляет первый элемент в списке, имеющий значение x",
		"list.pop([i])    Удаляет i-ый элемент и возвращает его. Если индекс не указан, удаляется последний элемент",
		"list.index(x, [start [, end]])    Возвращает положение первого элемента от start до end со значением x",
		"list.count(x)    Возвращает количество элементов со значением x",
		"list.sort([key = функция])    Сортирует список на основе функции",
		"list.reverse()    Разворачивает список",
		"list.copy()    Поверхностная копия списка (новое в python 3.3)",
		"list.clear()    Очищает список (новое в python 3.3))"]

    buff01 = []
    buff01l = []
    buff02 = []

    for w in y:
        splitstr = w.split('    ')
        buff01.append(splitstr[0])
        buff01l.append(len(splitstr[0]))
        buff02.append(splitstr[1])


    maxstr = max(buff01l)

    i = 0
    y.clear()
    for w in buff01:
        w = [w, buff02[i]]
        cs = ' ' * (maxstr - buff01l[i] + 1)
        y.append(cs.join(w))
        i = i + 1

    z = ['Январь', 'Февраль', 'Март', 'Апрель', 'Май', 'Июнь', 'Июль', 'Август', 'Сентябрь', 'Октябрь', 'Ноябрь', "Декабрь"]
    s = []
    tt = [c, x, z, y]

    i = 0
    print(tt)
    for to in tt:
        s.append(to)
        for ti in to:
            print (ti)
        i = i + 1
    print ('list s:')
    print (s)



def openfileout(filename):
    try:
        with open(filename) as fn:
            content = fn.readlines()

        for line in content:
            print(line, end = '')

        fn.close()

    except OSError as err:
        print("{0}".format(err))
        return 1
    except:
        print("Unexpected error:", sys.exc_info()[0])
        raise

def logged():
    import logging
    logging.basicConfig(level = logging.DEBUG)

    logging.debug('Debug message')
    logging.info('Program started..')
    logging.error('This is an error')
    logging.critical('This is an critical')
    logging.fatal('This is an fatal')
    logging.warning("Это предупреждение warning")

def split_join():
    string = "ABCDE"
    print (string[::-1])

    original = "Здравствуй, дедушка. Пишу тебе я письмо..."
    words = original.split(" ")
    rebuilt = " + ".join(words)
    rebuilt2 = " | ".join(words)

    print ("Original: " + original)
    print ("Split: " + str(words))
    print ("Rebuilt: " + rebuilt)
    print ("Rebuilt: " + rebuilt2)

def timefnc():
    print ('time.clock ', time.clock())
    print ('time.localtime ', time.localtime())
    print ('time.asctime ', time.asctime())

def binfl():
    with open("/home/wocson/projects/py_t/test/source_bin", "rb") as binary_file:
        # Read the whole file at once
        data = binary_file.read()
        print(data)

        # Seek position and read N bytes
        i = 0
        cmpd = b'\x00'
        binary_file.seek(0)  # Go to beginning

        while True:
            byte = binary_file.read(1)

            if byte == cmpd:
                print('This is binary file...')
                binary_file.close()
                break

            i = i + 1
            binary_file.seek(i)  # Go to next byte


def main():

    # print (os.name, os.uname() )
    # hello()
    # prints()
    # coloring()
    # fibonachi()
    # fssys('home',LYELLOW,BGRED)
    # c = pythagoras(3, 3)
    # print('с^2=a^2+b^2 c=', c, 'при a=', 3, ' b=', 3)

    # lists()

    # openfileout("/home/wocson/daemon")
    # logged()
    # split_join()
    # opt()

    timefnc()
    binfl()

if __name__ == '__main__':
    main()
    print ('Ok. Done')
