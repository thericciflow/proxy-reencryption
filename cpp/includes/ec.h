#pragma once
#include "ecpy_native.h"

struct EC {
  ZZ a, b;
  EC(ZZ, ZZ);
  virtual ZZPoint add(ZZPoint, ZZPoint) const = 0;
  virtual ZZPoint sub(ZZPoint, ZZPoint) const = 0;
  virtual ZZPoint scalarMult(ZZPoint, ZZ) const = 0;
  virtual ZZPoint normalize(ZZPoint) const = 0;
  virtual std::string to_string() const;
};

struct EC_Mod : public EC {
  ZZ a, b, modulo;
  EC_Mod(ZZ, ZZ, ZZ);
  ZZPoint add(ZZPoint, ZZPoint) const override;
  ZZPoint sub(ZZPoint, ZZPoint) const override;
  ZZPoint scalarMult(ZZPoint, ZZ) const override;
  ZZPoint normalize(ZZPoint) const override;
  std::string to_string() const override;
};
