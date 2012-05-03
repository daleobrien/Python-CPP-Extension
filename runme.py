
import helloWorld


print helloWorld.__doc__
print

print dir(helloWorld)
# should return: set('hello')
print
print helloWorld.printHelloWorld()

print
print helloWorld.dict_to_dict({'One': 1, "Two": 2})

print

s = helloWorld.Simple()
print s
#print helloWorld.Simple.__doc__

print dir(s)
s.k = 13
print s.k

print ""
x = helloWorld.Simple(i=12)
print 'x'
print x.i

print 'init with string ..."123"'
x = helloWorld.Simple(i="123")
print x.i

print 'init with string ... "sss"'
helloWorld.Simple(i="sss")
print 'xxx'

