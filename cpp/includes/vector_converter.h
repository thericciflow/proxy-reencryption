#pragma once

template <class T>
struct vector_to_python {
  static PyObject *convert(std::vector<T> const & s) {
    boost::python::list ret;
    BOOST_FOREACH(const T& i, s) {
      ret.append(boost::python::object(i));
    }
    return boost::python::incref(ret.ptr());
  }
};

template <class T>
struct vector_from_python {
  vector_from_python() {
    boost::python::converter::registry::push_back(
      &convertible,
      &construct,
      boost::python::type_id<vector_from_python<T>>()
    );
  }

  static void *convertible(PyObject *obj) {
    if (!PyList_Check(obj)) {
      return nullptr;
    } else {
      return obj;
    }
  }

  static void construct(PyObject *obj, boost::python::converter::rvalue_from_python_stage1_data *data) {
    uint32_t len = PyList_Size(obj);
    std::vector<T> v{};
    for (uint32_t i = 0; i < len; i++) {
      v.push_back(ZZ(PyList_GetItem(obj, i)));
    }
    void *storage = ((boost::python::converter::rvalue_from_python_storage<std::vector<T>>*) data) -> storage.bytes;
    new (storage) std::vector<T> (v);
    data->convertible = storage;
  }
};

template <class T>
void initializeVectorConverter() {
  using namespace boost::python;

  to_python_converter<std::vector<T>, vector_to_python<T>>();

  vector_from_python<T>();
}

