import ecpy_native

print ecpy_native.add(2**512, 2**256)
print dir(ecpy_native)
P = ecpy_native.ZZPoint(1, 1)
print P
Q = ecpy_native.ZZPoint(0, 2)
a = 3
b = 4
p = 7
R = ecpy_native.EC_Add_Mod(P, Q, a, b, p)
print R
print ecpy_native.EC_Normalize_Mod(R, p)
