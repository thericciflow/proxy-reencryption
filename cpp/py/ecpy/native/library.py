from ctypes import *

lib = cdll.LoadLibrary("libecpy_native.so")

def to_char_ptr(string):
  return c_char_p(string.encode('us-ascii'))
