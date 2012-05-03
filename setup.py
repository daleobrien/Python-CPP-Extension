
# for OSX , ...
#
# export CC=clang
#

from distutils.core import setup, Extension

module = Extension('helloWorld', sources = ['helloWorld.cpp'])

setup(name = 'helloWorld',
      version = '1.0',
      description = 'C++ module test',
      ext_modules = [module])
