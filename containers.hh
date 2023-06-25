#pragma once
#include "types_and_concepts.hh"
#include <cstdint>
#include <cassert>
#include <utility>

template<typename T>
struct vector;

template<typename T> requires DefaultConstructible<T>
class vector<T> {
public:

  vector() : m_data { nullptr }, m_size { 0ul }, m_capacity { 0ul }
  {
  }
  vector(vector const& other)
    : m_data { nullptr },
      m_size { other.m_size },
      m_capacity { other.m_capacity }
  {
    if (other.m_data) {
      m_data = new T[m_capacity] {};
      for (uint64_t i = 0; i < m_size; ++i) {
        m_data[i] = other.m_data[i];
      }
    }
  }

  vector(vector&& other)
    : m_data { other.m_data },
      m_size { other.m_size },
      m_capacity { other.m_capacity }
  {
    other.m_data = nullptr;
  }

  ~vector() {
    if (m_data) {
      delete[] m_data;
    }
  }

  void push_back(T const& t) {
    if (m_data == nullptr) {
      m_capacity = 2;
      allocate_(&m_data, m_capacity);
    }
    if (m_size == m_capacity) {
      reallocate_(&m_data, m_size, m_capacity * 2);
    }

    m_data[m_size++] = t;
  };
  void push_back(T&& t) {
    if (m_data == nullptr) {
      m_capacity = 2;
      allocate_(&m_data, m_capacity);
    }
    if (m_size == m_capacity) {
      reallocate_(&m_data, m_size, m_capacity * 2);
    }

    m_data[m_size++] = std::forward(t);
  }

  uint64_t size() const {
    return m_size; 
  }

  T& operator[] (uint64_t index) {
#ifdef DEBUG
    if (index >= m_size) throw 1;
#endif
    return m_data[index];
  }

  T const& operator[] (uint64_t index) const {
#ifdef DEBUG
    if (index >= m_size) throw 1;
#endif
    return m_data[index];
  }

private:
  void allocate_(T** ptr, unsigned long capacity) {
    *ptr = new T[capacity] {};
  }

  void reallocate(T** ptr, uint64_t size, uint64_t capacity) {
    assert(capacity > size);
    T* tmp = new T[capacity] {};
    for (uint64_t i = 0; i < size; ++i) {
      tmp[i] = (*ptr)[i];
    }
    delete[] *ptr;

    *ptr = tmp;
  }

  T* m_data;
  unsigned long m_size;
  unsigned long m_capacity;
};
