#pragma once

#include <exception>
#include <iostream>

namespace bmstu {
template <typename T>
class basic_string;

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
using u8string = basic_string<char8_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;

template <typename T>
class basic_string {
public:
  basic_string() : ptr_(new T[1]{0}), size_(0) {}

  basic_string(size_t size) : ptr_(new T[size + 1]), size_(size) {
    for (size_t i = 0; i < size; ++i) {
      ptr_[i] = T(' ');
    }
    ptr_[size] = T(0);
  }

  basic_string(std::initializer_list<T> il)
      : ptr_(new T[il.size() + 1]), size_(il.size()) {
    T* p = ptr_;
    for (const auto& c : il) {
      *p++ = c;
    }
    ptr_[size_] = T(0);
  }

  basic_string(const T* c_str) : ptr_(nullptr), size_(0) {
    if (c_str) {
      size_ = strlen_(c_str);
      ptr_ = new T[size_ + 1];
      T* p = ptr_;
      while (*c_str) {
        *p++ = *c_str++;
      }
      ptr_[size_] = T(0);
    } else {
      ptr_ = new T[1]{0};
    }
  }

  basic_string(const basic_string& other) : size_(other.size_) {
    ptr_ = new T[size_ + 1];
    T* p = ptr_;
    T* q = other.ptr_;
    while (size_--) {
      *p++ = *q++;
    }
    *p = T(0);
    size_ = other.size_;
  }

  basic_string(basic_string&& dying) noexcept : ptr_(nullptr), size_(0) {
    std::swap(ptr_, dying.ptr_);
    std::swap(size_, dying.size_);
  }

  ~basic_string() { delete[] ptr_; }

  const T* c_str() const { return ptr_; }

  size_t size() const { return size_; }

  basic_string& operator=(basic_string&& other) noexcept {
    if (this != &other) {
      clear_();
      std::swap(ptr_, other.ptr_);
      std::swap(size_, other.size_);
    }
    return *this;
  }

  basic_string& operator=(const T* c_str) {
    if (ptr_ != c_str) {
      clear_();
      if (c_str) {
        size_ = strlen_(c_str);
        ptr_ = new T[size_ + 1];
        for (size_t i = 0; i < size_; ++i) {
          ptr_[i] = c_str[i];
        }
        ptr_[size_] = T(0);
      } else {
        ptr_ = new T[1]{0};
      }
    }
    return *this;
  }

  basic_string& operator=(const basic_string& other) {
    if (this != &other) {
      basic_string temp(other);
      std::swap(ptr_, temp.ptr_);
      std::swap(size_, temp.size_);
    }
    return *this;
  }

  friend basic_string<T> operator+(const basic_string<T>& left,
                                   const basic_string<T>& right) {
    basic_string<T> result;
    result.size_ = left.size_ + right.size_;
    delete[] result.ptr_;
    result.ptr_ = new T[result.size_ + 1];

    // Копируем левую строку
    for (size_t i = 0; i < left.size_; ++i) {
      result.ptr_[i] = left.ptr_[i];
    }

    // Копируем правую строку
    for (size_t i = 0; i < right.size_; ++i) {
      result.ptr_[left.size_ + i] = right.ptr_[i];
    }

    result.ptr_[result.size_] = T(0);
    return result;
  }

  friend std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os,
                                           const basic_string& obj) {
    os << obj.c_str();
    return os;
  }

  friend std::basic_istream<T>& operator>>(std::basic_istream<T>& is,
                                           basic_string& obj) {
    is >> std::noskipws;
    T symbol = 1;
    for (size_t i = 0; symbol != 0; ++i) {
      symbol = 0;
      is >> symbol;
      if (symbol != 0) {
        obj += symbol;
      }
    }
    return is;
  }

  basic_string& operator+=(const basic_string& other) {
    if (other.size_ > 0) {
      size_t new_size = size_ + other.size_;
      T* new_ptr = new T[new_size + 1];

      // Копируем текущую строку
      for (size_t i = 0; i < size_; ++i) {
        new_ptr[i] = ptr_[i];
      }

      // Добавляем другую строку
      for (size_t i = 0; i < other.size_; ++i) {
        new_ptr[size_ + i] = other.ptr_[i];
      }

      new_ptr[new_size] = T(0);
      delete[] ptr_;
      ptr_ = new_ptr;
      size_ = new_size;
    }
    return *this;
  }

  basic_string& operator+=(T symbol) {
    if (symbol != T(0)) {
      T* new_ptr = new T[size_ + 2];

      // Копируем текущую строку
      for (size_t i = 0; i < size_; ++i) {
        new_ptr[i] = ptr_[i];
      }

      new_ptr[size_] = symbol;
      new_ptr[size_ + 1] = T(0);
      delete[] ptr_;
      ptr_ = new_ptr;
      ++size_;
    }
    return *this;
  }

  T& operator[](size_t index) noexcept { return ptr_[index]; }

  const T& operator[](size_t index) const noexcept { return ptr_[index]; }

  T& at(size_t index) {
    if (index >= size_) {
      throw std::out_of_range("Index out of range");
    }
    return ptr_[index];
  }

private:
  void clear_() {
    delete[] ptr_;
    ptr_ = new T[1]{0};
    size_ = 0;
  }

  static size_t strlen_(const T* str) {
    if (!str) {
      return 0;
    }

    size_t len = 0;
    while (str[len] != T(0)) {
      ++len;
    }
    return len;
  }

  T* ptr_;
  size_t size_;
};
}  // namespace bmstu2