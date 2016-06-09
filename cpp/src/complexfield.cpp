#include <ecpy.h>

ComplexField _cf_instance {};

std::ostream& operator<<(std::ostream& os, const ComplexField& field) {
  os << "Complex Field";
  return os;
}
