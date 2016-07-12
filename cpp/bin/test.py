import ecpy_native

print dir(ecpy_native)
P = ecpy_native.ZZPoint(1, 1)
print P
Q = ecpy_native.ZZPoint(0, 2)
a = 3
b = 4
p = 7
E = ecpy_native.EC_Mod(a, b, p)
print E
R = E.add(P, Q)
print R
print E.normalize(R)

p = 37139
q = 11087
n = p * q
a = 3

r = ecpy_native.modular_square_root(a, n)
print r
print (r[0] ** 2 % n), (r[1] ** 2 % n)

r = ecpy_native.modular_square_root(9, p)
print r
print (r[0] ** 2 % p), (r[1] ** 2 % p)

p = 3472313333
a = 9

r = ecpy_native.modular_square_root(a, p)
print r
print (r[0] ** 2 % p), (r[1] ** 2 % p)
