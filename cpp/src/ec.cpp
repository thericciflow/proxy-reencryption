#include "ecpy_native.h"

bool EC_ZZPoint_Is_Infinity(ZZPoint P) {
  return P.x == 0 && P.y == 1 && P.z == 0;
}

bool EC_Equals_Mod(ZZPoint P, ZZPoint Q, ZZ a, ZZ b, ZZ modulo) {
  return (P.x * Q.y) % modulo == (P.y * Q.x) % modulo;
}

EC::EC(ZZ _a, ZZ _b)
  : a(_a), b(_b) {}

std::string EC::to_string() const {
  std::stringstream ss;
  ss << "y^2 = x^3";
  if (a != 0) {
    ss << " + ";
    if (a == -1) {
      ss << "-";
    }
    if (a != 1 || a != -1) {
      ss << a;
    }
    ss << "x";
  }
  if (b != 0) {
    ss << " + " << b;
  }
  return ss.str();
}

EC_Mod::EC_Mod(ZZ _a, ZZ _b, ZZ _modulo)
  : EC(_a, _b), modulo(_modulo) {
}

ZZPoint EC_Mod::add(ZZPoint P, ZZPoint Q) const {
  if (EC_ZZPoint_Is_Infinity(P)) {
    return Q;
  } else if (EC_ZZPoint_Is_Infinity(Q)) {
    return P;
  } else if (P.x == Q.x && (P.y + Q.y) % modulo == 0) {
    return ZZPoint(0, 1, 0);
  }
  if (EC_Equals_Mod(P, Q, a, b, modulo)) {
    auto u = (3 * P.x * P.x + a * P.z * P.z) % modulo;
    auto v = (P.y * P.z) % modulo;
    auto yv = (P.y * v) % modulo;
    auto yv4 = (4 * yv) % modulo;
    auto w = (u * u - 2 * P.x * yv4) % modulo;
    auto Rx = (2 * v * w) % modulo;
    auto Ry = ((u * ((P.x * yv4 - w) % modulo)) - 8 * yv * yv) % modulo;
    auto Rz = (8 * v * v * v) % modulo;
    return ZZPoint(Rx, Ry, Rz);
  } else {
    auto u = (Q.y * P.z - P.y * Q.z) % modulo;
    auto v = (Q.x * P.z - P.x * Q.z) % modulo;
    auto v2 = (v * v) % modulo;
    auto v3 = (v2 * v) % modulo;
    auto w = (u * u * P.z * P.z - v3 - 2 * v2 * P.x * Q.z) % modulo;
    auto Rx = (v * w) % modulo;
    auto Ry = (u * (v2 * P.x * Q.z - w) - v3 * P.y * Q.z) % modulo;
    auto Rz = (v3 * P.z * Q.z) % modulo;
    return ZZPoint(Rx, Ry, Rz);
  }
}

ZZPoint EC_Mod::sub(ZZPoint P, ZZPoint _Q) const {
  ZZPoint Q {_Q};
  Q.y = -1 * Q.y;
  return EC_Mod::add(P, Q);
}

ZZPoint EC_Mod::scalarMult(ZZPoint P, ZZ n) const {
  auto m = n.to_mpz_class();
  if (m == 0) {
    return ZZPoint(0, 1, 0);
  } else if (m == 1) {
    return P;
  } else if (m == 2) {
    return add(P, P);
  }
  ZZPoint Q(0, 0);
  while (m != 0) {
    if ((m & 1) == 1) {
      Q = add(P, Q);
    }
    P = add(P, P);
    m >>= 1;
  }
  return Q;
}

ZZPoint EC_Mod::normalize(ZZPoint P) const {
  auto inv = static_cast<ZZ>(modinv(P.z, modulo));
  return ZZPoint((P.x * inv) % modulo, (P.y * inv) % modulo, ZZ(1));
}

std::string EC_Mod::to_string() const {
  std::stringstream ss;
  ss << EC::to_string();
  ss << " on Ring Z/" << modulo << "Z";
  return ss.str();
}
