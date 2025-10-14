'''
Created on 17 окт. 2019 г.

@author: wocson
'''
import os
from setuptools import setup

# Utility function to read the README file.
# Used for the long_description.  It's nice, because now 1) we have a top level
# README file and 2) it's easier to type in the README file than to put a raw
# string in below ...
def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()

setup(
    name="aspd exchanger",
    version="1.007",
    author="Vladimir Noskov",
    author_email="wocson@gmail.com",
    description=("aspd exchanger"),
    license="My",
    keywords="utils tools application",
    url="",
    packages=['aspd exchanger', 'work'],
    long_description=read('README'),
    classifiers=[
        "Development Status :: 1",
        "Topic :: Utilities",
        "License :: OSI Approved :: BSD License",
    ],
)
