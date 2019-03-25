#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import random
from ecpy import FiniteField, ExtendedFiniteField
from ecpy import EllipticCurve
from ecpy import weil_pairing
from ecpy import utils 

# Curve 1
# [!] type-1 pairing
#p = 51913085418121682294001018392922882479990360896585434465224709506184017104633180621645036239902771873890716081492327513618482373167152353369121135029626237
#l = (p + 1) / 6
#F = ExtendedFiniteField(p, "x^2+x+1")
#E = EllipticCurve(F, 0, 1)
#P = E(6, (13223256612935805456685486619785988828211352545760979145826773809338317294785343911938620915300032787305564224073179749632175004531990130007962275678599731, 26446513225871610913370973239571977656422705091521958291653547618676634589570687823877241830600065574611128448146359499264350009063980260015924551357199462))

# Curve 2
# [!] type-1 pairing
#p = 50179444633418995760428215518943816084543378339277274339557962861710992916021522142514248292890927025958085436246349332698880745359574857376419559953437557
#l = (p + 1) / 6
#F = ExtendedFiniteField(p, "x^2+x+1")
#E = EllipticCurve(F, 0, 1)
#P = E(3, (14180773112704578861392922920205876836428986366773536643541011717684401801069777797699258667061922178009879315047772033936311133535564812495329881887557081))

# BN(2,256) curve
# [!] type-1 pairing
#p = 16798108731015832284940804142231733909889187121439069848933715426072753864723 
#l = 16798108731015832284940804142231733909759579603404752749028378864165570215949
#F = FiniteField(p)
#E = EllipticCurve(F, 0, 2)
#P = E(-1, 1)

# BN(2,256) curve
# [!] type-2 pairing
#p = 16798108731015832284940804142231733909889187121439069848933715426072753864723
#l = 16798108731015832284940804142231733909759579603404752749028378864165570215949
#F = ExtendedFiniteField(p, "x^2+1")
#E = EllipticCurve(F, 0, 2)
#P = E((2759930593230997547690248631365636073479225314645471320757910281674905877291, 2301614907882718573745244110620256732212332571700737603512907075120331574515), (948076515351688797057606839494504109262247838840660288969725032302618946458, 6663077446927392079224045631425291036692402823802663947112913140121004068507)) 

# BLS12-381 curve
# [!] type-1 pairing
p = 4002409555221667393417789825735904156556882819939007885332058136124031650490837864442687629129015664037894272559787
l = 52435875175126190479447740508185965837690552500527637822603658699938581184513 
F = FiniteField(p)
E = EllipticCurve(F, 0, 4)
P = E(3685416753713387016781088315183077757961620795782546409894578378688607592378376318836054947676345821548104185464507, 1339506544944476473020471379941921221584933875938349620426543736416511423956333506472724655353366534992391756441569)

# BLS12-381 curve
# [!] type-2 pairing
#p = 4002409555221667393417789825735904156556882819939007885332058136124031650490837864442687629129015664037894272559787
#l = 52435875175126190479447740508185965837690552500527637822603658699938581184513 
#F = ExtendedFiniteField(p, "x^2+1")
#E = EllipticCurve(F, 0, 4)
#P = E((352701069587466618187139116011060144890029952792775240219908644239793785735715026873347600343865175952761926303160, 3059144344244213709971259814753781636986470325476647558659373206291635324768958432433509563104347017837885763365758), (1985150602287291935568054521177171638300868978215655730859378665066344726373823718423869104263333984641494340347905, 927553665492332455747201965776037880757740193453592970025027978793976877002675564980949289727957565575433344219582))

Z = weil_pairing(E,P,P,l)

#----------------------------------------------
# return (x,y,d) such that a*x+b*y = d = gcd(a,b)
def exGcd(a, b):
    if (b==0):
        return (1,0,a);
    else:
        (y,x,d) = exGcd(b, a%b)   # which means y*b + x*(a%b) = d, i.e. y*b + x*(a-a/b*b) = d <=> a*x + b*(y-a/b*x) = d
        return (x, y-a/b*x, d)

# return x such that x^n = a mod p
def nth_root(a, n, p):
	if (exGcd(n, p-1)[2] != 1): sys.exit("gcd(n, p-1) != 1")
	#return powermod(a, exGcd(n, p-1)[0], p)
	return a**exGcd(n, p-1)[0]
#----------------------------------------------

def keyGen():
	global p, P
	sk = 2
	while (exGcd(sk, p-1)[2] != 1):
		sk = random.randint(1, p-1)
	pk = sk * P
	return sk, pk

def rekeyGen(sk_a, pk_b):
	global p
	return sk_a * pk_b 

def encrypt(m, pk):
 	global E, l, p, P
	r = random.randint(1, p-1)
	c = r * P, m * weil_pairing(E, P, r * pk, l)
	return c

def decrypt(c, sk):
	global E, l, P
	return c[1] / weil_pairing(E, c[0], sk * P, l)

def reencrypt(c, rk):
	global E, l
	return weil_pairing(E, c[0], rk, l), c[1]

def redecrypt(c, sk):
	global p
	#X = c[1]**sk / c[0]
	return nth_root( c[1]**sk / c[0], sk, p ) 


def main():
	global E, p, l, P, Z
	print "\n [+]", E
	print "\n [+]", P
	print "\n [+] pairing(P,P): ", Z
	sk_a, pk_a = keyGen()
	print "\n [+] Alice secret key sk_a = ", sk_a
	print "\n [+] Alice public key pk_a = ", pk_a
	#m = random.randint(1, p-1) 
	m = random.randint(1, 100)
	c = encrypt(m, pk_a)
	print "\n [+] plaintext = ", m
	print "\n [+] cypthertext for Alice = ", c
	d_m = decrypt(c, sk_a)
	print "\n [+] decrypted text by Alice = ", d_m
	if (m-d_m == 0): print "\n [!] encryption/decryption checked!"
	sk_b, pk_b = keyGen()
	print "\n [+] Bob's secret key sk_b = ", sk_b
	print "\n [+] Bob's public key pk_b = ", pk_b
	rk = rekeyGen(sk_a, pk_b)
	print "\n [+] reencryption key from Alice to Bob rk = ", rk
	cc = reencrypt(c, rk)
	print "\n [+] reencrypted cypthertext for Bob = ", cc
	dd_m = redecrypt(cc, sk_b)
	print "\n [+] decrypted text by Bob = ", dd_m
	#print powermod(m, sk_b, p) - dd_m[0]
	if ( m-dd_m == 0 ): print "\n [!] rencryption/redecryption checked!"
	if (redecrypt(cc, sk_a) - m): print "\n [!] Only Bob can decrypt!" 


if __name__ == "__main__":
  main()
