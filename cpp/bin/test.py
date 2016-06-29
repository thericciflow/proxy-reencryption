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
