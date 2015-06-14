
import helloWorld


assert helloWorld.__doc__ == 'Module Documentation'

d = dir(helloWorld)

assert "printHelloWorld" in d
assert "dict_to_dict" in d

r = helloWorld.printHelloWorld()
assert r == set(['hello', 'world'])

s = helloWorld.dict_to_dict({'One': 1, "Two": 2})
assert s[1] == 'One'
assert s[2] == 'Two'


s = helloWorld.Simple()

d = dir(s)
assert "i" in d
assert "j" in d
assert "k" in d
assert "l" in d
assert "t" in d
assert "klass" not in d

s.i = 13
assert s.i == 13
s.j = 444
assert s.i == 13
assert s.j == 444

# a list and tuple
assert s.l == [3, 2, 1]
assert s.t == (3, 2, 1)
assert sorted(s.t) == [1, 2, 3]
assert sorted(s.l) == [1, 2, 3]


x = helloWorld.Simple(i=12)
assert x.i == 12
assert x.j == 0
assert x.k == 0

x = helloWorld.Simple(i=345)
assert x.i == 345
assert x.j == 0
assert x.k == 0

x = helloWorld.Simple(i="123", j=45)
assert x.i == 123
assert x.j == 45
assert x.k == 0

print 'All tests pasted.'
#print 'init with string ... "112.0"'
#x = helloWorld.Simple(i="112.0")
#print "x.i = ", x.i

#print 'init with string ... "sss"'
#x = helloWorld.Simple(i="sss")
#print "x.i = ", x.i


#print 'init with string ... "s32"'
#x = helloWorld.Simple(i="s32")
#print "x.i = ", x.i

