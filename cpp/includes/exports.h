#pragma once
#include "ecpy_native.h"

__EXPORT__ {
  ZZ *ZZ_create(const char*);
  void ZZ_destroy(ZZ*);
  ZZ *ZZ_add(const ZZ*, const ZZ*);
  ZZ *ZZ_neg(const ZZ*);
  ZZ *ZZ_mul(const ZZ*, const ZZ*);
  ZZ *ZZ_div(const ZZ*, const ZZ*);
  ZZ *ZZ_mod(const ZZ*, const ZZ*);
  ZZ *ZZ_modinv(const ZZ*, const ZZ*);
  bool ZZ_to_string(const ZZ *zz, char *p, int maxlen);
  bool ZZ_is_equals(const ZZ*, const ZZ*);
  int ZZ_jacobi(const ZZ*, const ZZ*);
  int ZZ_legendre(const ZZ*, const ZZ*);

  FF *FF_create(const char*, const char*);
  void FF_destroy(FF*);
  FF *FF_add(const FF*, const FF*);
  FF *FF_neg(const FF*);
  FF *FF_mul(const FF*, const FF*);
  FF *FF_div(const FF*, const FF*);
  FF *FF_mod(const FF*, const FF*);
  bool FF_is_equals(const FF*, const FF*);
  bool FF_to_string(const FF*, char*, int);
  bool FF_to_raw_string(const FF*, char*, int);

  EF *EF_create(const char*, const char*, const char*, const char*);
  void EF_destroy(EF*);
  bool EF_to_string(const EF*, char*, int);
  bool EF_to_tuple(const EF*, char*, int);
  bool EF_is_equals(const EF*, const EF*);
  EF *EF_add(const EF*, const EF*);
  EF *EF_neg(const EF*);
  EF *EF_mul(const EF*, const EF*);
  EF *EF_inv(const EF*);

  EC *EC_create(const char*, const char*, const char*);
  void EC_destroy(EC*);
  bool EC_is_equals(const EC*, const EC*);
  bool EC_to_string(const EC*, char*, int);

  void EP_destroy(EP*);
  bool EP_equals(const EP*, const EP*);
  EP *EP_FF_create(const EC*, const char*, const char*, const char*, const char*);
  EP *EP_EF_create(const EC*, const char*, const char*, const char*, const char*, const char*, const char*, const char*, const char*);
  EP *EP_FF_create_with_FF(const EC*, const FF*, const FF*, const FF*);
  EP *EP_EF_create_with_EF(const EC*, const EF*, const EF*, const EF*);
  EP *EP_FF_add(const EP*, const EP*);
  EP *EP_EF_add(const EP*, const EP*);
  EP *EP_mul(const EP*, const ZZ*);
  bool EP_to_string(const EP*, char*, int);
  bool EP_to_tuple(const EP*, char*, int);
  EP *EP_neg(const EP*);
  g_object_t *EP_line_coeff(const EP*, const EP*);
  g_object_t *EP_miller(const EP*, const EP*, const ZZ*);
}
