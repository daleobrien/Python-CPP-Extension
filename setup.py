# for OSX , ...
#
# export CC=clang
# export LDSHARED='clang -bundle -undefined dynamic_lookup -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk -g'

from distutils.core import setup, Extension

module = Extension('helloWorld', sources = ['helloWorld.cpp'])

setup(name = 'helloWorld',
      version = '1.0',
      description = 'C++ module test',
      ext_modules = [module])
