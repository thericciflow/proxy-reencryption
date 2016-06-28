import ecpy_native

print ecpy_native.add(2**512, 2**256)
print dir(ecpy_native)
P = ecpy_native.ZZPoint(0, 1, 0)
print P
