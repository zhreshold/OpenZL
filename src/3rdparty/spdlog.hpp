/*#include "spdlog.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_SPDLOG_H_
#define _AUTOMATIC_GUARD_SPDLOG_SPDLOG_H_

/*#include "tweakme.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_TWEAKME_H_
#define _AUTOMATIC_GUARD_SPDLOG_TWEAKME_H_

#endif /* END _AUTOMATIC_GUARD_SPDLOG_TWEAKME_H_ */
/*#include "common.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_COMMON_H_
#define _AUTOMATIC_GUARD_SPDLOG_COMMON_H_

#include <string>
#include <initializer_list>
#include <chrono>
#include <memory>
#ifndef _MSC_VER
#define SPDLOG_NOEXCEPT noexcept
#else
#define SPDLOG_NOEXCEPT throw()
#endif
namespace spdlog
{
class formatter;
namespace sinks
{
class sink;
}
using log_clock = std::chrono::system_clock;
using sink_ptr = std::shared_ptr < sinks::sink > ;
using sinks_init_list = std::initializer_list < sink_ptr > ;
using formatter_ptr = std::shared_ptr<spdlog::formatter>;
namespace level
{
typedef enum
{
    trace    = 0,
    debug    = 1,
    info     = 2,
    notice   = 3,
    warn     = 4,
    err      = 5,
    critical = 6,
    alert    = 7,
    emerg    = 8,
    off      = 9
} level_enum;
static const char* level_names[] { "trace", "debug", "info", "notice", "warning", "error", "critical", "alert", "emerg", "off"};
static const char* short_level_names[] { "T", "D", "I", "N", "W", "E", "C", "A", "M", "O"};
inline const char* to_str(spdlog::level::level_enum l)
{
    return level_names[l];
}
inline const char* to_short_str(spdlog::level::level_enum l)
{
    return short_level_names[l];
}
}
enum class async_overflow_policy
{
    block_retry,
    discard_log_msg
};
class spdlog_ex : public std::exception
{
public:
    spdlog_ex(const std::string& msg) :_msg(msg) {}
    const char* what() const SPDLOG_NOEXCEPT override
    {
        return _msg.c_str();
    }
private:
    std::string _msg;
};
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_COMMON_H_ */
/*#include "logger.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_LOGGER_H_
#define _AUTOMATIC_GUARD_SPDLOG_LOGGER_H_

#include<vector>
#include<memory>
/*#include "sinks/base_sink.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_SINKS_BASE_SINK_H_
#define _AUTOMATIC_GUARD_SPDLOG_SINKS_BASE_SINK_H_

#include<string>
#include<mutex>
#include<atomic>
/*#include "./sink.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_SINKS_SINK_H_
#define _AUTOMATIC_GUARD_SPDLOG_SINKS_SINK_H_

/*#include "../details/log_msg.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_DETAILS_LOG_MSG_H_
#define _AUTOMATIC_GUARD_SPDLOG_DETAILS_LOG_MSG_H_

#include <thread>
/*#include "../common.h" skipped */
/*#include "./format.h"*/
#ifndef FMT_FORMAT_H_
#define FMT_FORMAT_H_
#define FMT_HEADER_ONLY
#include <stdint.h>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <string>
#include <sstream>
#if _SECURE_SCL
# include <iterator>
#endif
#ifdef _MSC_VER
# include <intrin.h>
namespace fmt {
namespace internal {
# pragma intrinsic(_BitScanReverse)
inline uint32_t clz(uint32_t x) {
  unsigned long r = 0;
  _BitScanReverse(&r, x);
  return 31 - r;
}
# define FMT_BUILTIN_CLZ(n) fmt::internal::clz(n)
# ifdef _WIN64
#  pragma intrinsic(_BitScanReverse64)
# endif
inline uint32_t clzll(uint64_t x) {
  unsigned long r = 0;
# ifdef _WIN64
  _BitScanReverse64(&r, x);
# else
  if (_BitScanReverse(&r, static_cast<uint32_t>(x >> 32)))
    return 63 - (r + 32);
  _BitScanReverse(&r, static_cast<uint32_t>(x));
# endif
  return 63 - r;
}
# define FMT_BUILTIN_CLZLL(n) fmt::internal::clzll(n)
}
}
#endif
#ifdef __GNUC__
# define FMT_GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
# define FMT_GCC_EXTENSION __extension__
# if FMT_GCC_VERSION >= 406
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wlong-long"
#  pragma GCC diagnostic ignored "-Wshadow"
# endif
# if __cplusplus >= 201103L || defined __GXX_EXPERIMENTAL_CXX0X__
#  define FMT_HAS_GXX_CXX11 1
# endif
#else
# define FMT_GCC_EXTENSION
#endif
#ifdef __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation"
#endif
#ifdef __GNUC_LIBSTD__
# define FMT_GNUC_LIBSTD_VERSION (__GNUC_LIBSTD__ * 100 + __GNUC_LIBSTD_MINOR__)
#endif
#ifdef __has_feature
# define FMT_HAS_FEATURE(x) __has_feature(x)
#else
# define FMT_HAS_FEATURE(x) 0
#endif
#ifdef __has_builtin
# define FMT_HAS_BUILTIN(x) __has_builtin(x)
#else
# define FMT_HAS_BUILTIN(x) 0
#endif
#ifdef __has_cpp_attribute
# define FMT_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
# define FMT_HAS_CPP_ATTRIBUTE(x) 0
#endif
#ifndef FMT_USE_VARIADIC_TEMPLATES
# define FMT_USE_VARIADIC_TEMPLATES \
   (FMT_HAS_FEATURE(cxx_variadic_templates) || \
       (FMT_GCC_VERSION >= 404 && FMT_HAS_GXX_CXX11) || _MSC_VER >= 1800)
#endif
#ifndef FMT_USE_RVALUE_REFERENCES
# if defined(FMT_GNUC_LIBSTD_VERSION) && FMT_GNUC_LIBSTD_VERSION <= 402
#  define FMT_USE_RVALUE_REFERENCES 0
# else
#  define FMT_USE_RVALUE_REFERENCES \
    (FMT_HAS_FEATURE(cxx_rvalue_references) || \
        (FMT_GCC_VERSION >= 403 && FMT_HAS_GXX_CXX11) || _MSC_VER >= 1600)
# endif
#endif
#if FMT_USE_RVALUE_REFERENCES
# include <utility>
#endif
#if FMT_USE_NOEXCEPT || FMT_HAS_FEATURE(cxx_noexcept) || \
  (FMT_GCC_VERSION >= 408 && FMT_HAS_GXX_CXX11)
# define FMT_NOEXCEPT noexcept
#else
# define FMT_NOEXCEPT throw()
#endif
#if FMT_USE_DELETED_FUNCTIONS || FMT_HAS_FEATURE(cxx_deleted_functions) || \
  (FMT_GCC_VERSION >= 404 && FMT_HAS_GXX_CXX11) || _MSC_VER >= 1800
# define FMT_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete; \
    TypeName& operator=(const TypeName&) = delete
#else
# define FMT_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&); \
    TypeName& operator=(const TypeName&)
#endif
namespace fmt {
FMT_GCC_EXTENSION typedef long long LongLong;
FMT_GCC_EXTENSION typedef unsigned long long ULongLong;
#if FMT_USE_RVALUE_REFERENCES
using std::move;
#endif
template <typename Char>
class BasicWriter;
typedef BasicWriter<char> Writer;
typedef BasicWriter<wchar_t> WWriter;
template <typename Char>
class BasicFormatter;
template <typename Char, typename T>
void format(BasicFormatter<Char> &f, const Char *&format_str, const T &value);
template <typename Char>
class BasicStringRef {
 private:
  const Char *data_;
  std::size_t size_;
 public:
  BasicStringRef(const Char *s, std::size_t size) : data_(s), size_(size) {}
  BasicStringRef(const Char *s)
    : data_(s), size_(std::char_traits<Char>::length(s)) {}
  BasicStringRef(const std::basic_string<Char> &s)
  : data_(s.c_str()), size_(s.size()) {}
  operator std::basic_string<Char>() const {
    return std::basic_string<Char>(data_, size());
  }
  const Char *c_str() const { return data_; }
  std::size_t size() const { return size_; }
  friend bool operator==(BasicStringRef lhs, BasicStringRef rhs) {
    return lhs.data_ == rhs.data_;
  }
  friend bool operator!=(BasicStringRef lhs, BasicStringRef rhs) {
    return lhs.data_ != rhs.data_;
  }
};
typedef BasicStringRef<char> StringRef;
typedef BasicStringRef<wchar_t> WStringRef;
class FormatError : public std::runtime_error {
 public:
  explicit FormatError(StringRef message)
  : std::runtime_error(message.c_str()) {}
};
namespace internal {
enum { INLINE_BUFFER_SIZE = 500 };
#if _SECURE_SCL
template <typename T>
inline stdext::checked_array_iterator<T*> make_ptr(T *ptr, std::size_t size) {
  return stdext::checked_array_iterator<T*>(ptr, size);
}
#else
template <typename T>
inline T *make_ptr(T *ptr, std::size_t) { return ptr; }
#endif
}
template <typename T>
class Buffer {
 private:
  FMT_DISALLOW_COPY_AND_ASSIGN(Buffer);
 protected:
  T *ptr_;
  std::size_t size_;
  std::size_t capacity_;
  Buffer(T *ptr = 0, std::size_t capacity = 0)
    : ptr_(ptr), size_(0), capacity_(capacity) {}
  virtual void grow(std::size_t size) = 0;
 public:
  virtual ~Buffer() {}
  std::size_t size() const { return size_; }
  std::size_t capacity() const { return capacity_; }
  void resize(std::size_t new_size) {
    if (new_size > capacity_)
      grow(new_size);
    size_ = new_size;
  }
  void reserve(std::size_t capacity) {
    if (capacity > capacity_)
      grow(capacity);
  }
  void clear() FMT_NOEXCEPT { size_ = 0; }
  void push_back(const T &value) {
    if (size_ == capacity_)
      grow(size_ + 1);
    ptr_[size_++] = value;
  }
  void append(const T *begin, const T *end);
  T &operator[](std::size_t index) { return ptr_[index]; }
  const T &operator[](std::size_t index) const { return ptr_[index]; }
};
template <typename T>
void Buffer<T>::append(const T *begin, const T *end) {
  std::ptrdiff_t num_elements = end - begin;
  if (size_ + num_elements > capacity_)
    grow(size_ + num_elements);
  std::copy(begin, end, internal::make_ptr(ptr_, capacity_) + size_);
  size_ += num_elements;
}
namespace internal {
template <typename T, std::size_t SIZE, typename Allocator = std::allocator<T> >
class MemoryBuffer : private Allocator, public Buffer<T> {
 private:
  T data_[SIZE];
  void free() {
    if (this->ptr_ != data_) this->deallocate(this->ptr_, this->capacity_);
  }
 protected:
  void grow(std::size_t size);
 public:
  explicit MemoryBuffer(const Allocator &alloc = Allocator())
      : Allocator(alloc), Buffer<T>(data_, SIZE) {}
  ~MemoryBuffer() { free(); }
#if FMT_USE_RVALUE_REFERENCES
 private:
  void move(MemoryBuffer &other) {
    Allocator &this_alloc = *this, &other_alloc = other;
    this_alloc = std::move(other_alloc);
    this->size_ = other.size_;
    this->capacity_ = other.capacity_;
    if (other.ptr_ == other.data_) {
      this->ptr_ = data_;
      std::copy(other.data_,
                other.data_ + this->size_, make_ptr(data_, this->capacity_));
    } else {
      this->ptr_ = other.ptr_;
      other.ptr_ = other.data_;
    }
  }
 public:
  MemoryBuffer(MemoryBuffer &&other) {
    move(other);
  }
  MemoryBuffer &operator=(MemoryBuffer &&other) {
    assert(this != &other);
    free();
    move(other);
    return *this;
  }
#endif
  Allocator get_allocator() const { return *this; }
};
template <typename T, std::size_t SIZE, typename Allocator>
void MemoryBuffer<T, SIZE, Allocator>::grow(std::size_t size) {
  std::size_t new_capacity =
      (std::max)(size, this->capacity_ + this->capacity_ / 2);
  T *new_ptr = this->allocate(new_capacity);
  std::copy(this->ptr_,
            this->ptr_ + this->size_, make_ptr(new_ptr, new_capacity));
  std::size_t old_capacity = this->capacity_;
  T *old_ptr = this->ptr_;
  this->capacity_ = new_capacity;
  this->ptr_ = new_ptr;
  if (old_ptr != data_)
    this->deallocate(old_ptr, old_capacity);
}
template <typename Char>
class FixedBuffer : public fmt::Buffer<Char> {
 public:
  FixedBuffer(Char *array, std::size_t size) : fmt::Buffer<Char>(array, size) {}
 protected:
  void grow(std::size_t size);
};
#ifndef _MSC_VER
inline int getsign(double x) {
# ifdef signbit
  return signbit(x);
# else
  return std::signbit(x);
# endif
}
# ifdef isinf
inline int isinfinity(double x) { return isinf(x); }
inline int isinfinity(long double x) { return isinf(x); }
# else
inline int isinfinity(double x) { return std::isinf(x); }
inline int isinfinity(long double x) { return std::isinf(x); }
# endif
#else
inline int getsign(double value) {
  if (value < 0) return 1;
  if (value == value) return 0;
  int dec = 0, sign = 0;
  char buffer[2];
  _ecvt_s(buffer, sizeof(buffer), value, 0, &dec, &sign);
  return sign;
}
inline int isinfinity(double x) { return !_finite(x); }
inline int isinfinity(long double x) { return !_finite(static_cast<double>(x)); }
#endif
template <typename Char>
class BasicCharTraits {
 public:
#if _SECURE_SCL
  typedef stdext::checked_array_iterator<Char*> CharPtr;
#else
  typedef Char *CharPtr;
#endif
};
template <typename Char>
class CharTraits;
template <>
class CharTraits<char> : public BasicCharTraits<char> {
 private:
  static char convert(wchar_t);
public:
  static char convert(char value) { return value; }
  template <typename T>
  static int format_float(char *buffer, std::size_t size,
      const char *format, unsigned width, int precision, T value);
};
template <>
class CharTraits<wchar_t> : public BasicCharTraits<wchar_t> {
 public:
  static wchar_t convert(char value) { return value; }
  static wchar_t convert(wchar_t value) { return value; }
  template <typename T>
  static int format_float(wchar_t *buffer, std::size_t size,
      const wchar_t *format, unsigned width, int precision, T value);
};
template <bool IsSigned>
struct SignChecker {
  template <typename T>
  static bool is_negative(T value) { return value < 0; }
};
template <>
struct SignChecker<false> {
  template <typename T>
  static bool is_negative(T) { return false; }
};
template <typename T>
inline bool is_negative(T value) {
  return SignChecker<std::numeric_limits<T>::is_signed>::is_negative(value);
}
template <bool FitsIn32Bits>
struct TypeSelector { typedef uint32_t Type; };
template <>
struct TypeSelector<false> { typedef uint64_t Type; };
template <typename T>
struct IntTraits {
  typedef typename
    TypeSelector<std::numeric_limits<T>::digits <= 32>::Type MainType;
};
template <typename T>
struct MakeUnsigned { typedef T Type; };
#define FMT_SPECIALIZE_MAKE_UNSIGNED(T, U) \
  template <> \
  struct MakeUnsigned<T> { typedef U Type; }
FMT_SPECIALIZE_MAKE_UNSIGNED(char, unsigned char);
FMT_SPECIALIZE_MAKE_UNSIGNED(signed char, unsigned char);
FMT_SPECIALIZE_MAKE_UNSIGNED(short, unsigned short);
FMT_SPECIALIZE_MAKE_UNSIGNED(int, unsigned);
FMT_SPECIALIZE_MAKE_UNSIGNED(long, unsigned long);
FMT_SPECIALIZE_MAKE_UNSIGNED(LongLong, ULongLong);
void report_unknown_type(char code, const char *type);
template <typename T = void>
struct BasicData {
  static const uint32_t POWERS_OF_10_32[];
  static const uint64_t POWERS_OF_10_64[];
  static const char DIGITS[];
};
typedef BasicData<> Data;
#if FMT_GCC_VERSION >= 400 || FMT_HAS_BUILTIN(__builtin_clz)
# define FMT_BUILTIN_CLZ(n) __builtin_clz(n)
#endif
#if FMT_GCC_VERSION >= 400 || FMT_HAS_BUILTIN(__builtin_clzll)
# define FMT_BUILTIN_CLZLL(n) __builtin_clzll(n)
#endif
#ifdef FMT_BUILTIN_CLZLL
inline unsigned count_digits(uint64_t n) {
  unsigned t = (64 - FMT_BUILTIN_CLZLL(n | 1)) * 1233 >> 12;
  return t - (n < Data::POWERS_OF_10_64[t]) + 1;
}
#else
inline unsigned count_digits(uint64_t n) {
  unsigned count = 1;
  for (;;) {
    if (n < 10) return count;
    if (n < 100) return count + 1;
    if (n < 1000) return count + 2;
    if (n < 10000) return count + 3;
    n /= 10000u;
    count += 4;
  }
}
#endif
#ifdef FMT_BUILTIN_CLZ
inline unsigned count_digits(uint32_t n) {
  uint32_t t = (32 - FMT_BUILTIN_CLZ(n | 1)) * 1233 >> 12;
  return t - (n < Data::POWERS_OF_10_32[t]) + 1;
}
#endif
template <typename UInt, typename Char>
inline void format_decimal(Char *buffer, UInt value, unsigned num_digits) {
  --num_digits;
  while (value >= 100) {
    unsigned index = (value % 100) * 2;
    value /= 100;
    buffer[num_digits] = Data::DIGITS[index + 1];
    buffer[num_digits - 1] = Data::DIGITS[index];
    num_digits -= 2;
  }
  if (value < 10) {
    *buffer = static_cast<char>('0' + value);
    return;
  }
  unsigned index = static_cast<unsigned>(value * 2);
  buffer[1] = Data::DIGITS[index + 1];
  buffer[0] = Data::DIGITS[index];
}
#ifdef _WIN32
class UTF8ToUTF16 {
 private:
  MemoryBuffer<wchar_t, INLINE_BUFFER_SIZE> buffer_;
 public:
  explicit UTF8ToUTF16(StringRef s);
  operator WStringRef() const { return WStringRef(&buffer_[0], size()); }
  size_t size() const { return buffer_.size() - 1; }
  const wchar_t *c_str() const { return &buffer_[0]; }
  std::wstring str() const { return std::wstring(&buffer_[0], size()); }
};
class UTF16ToUTF8 {
 private:
  MemoryBuffer<char, INLINE_BUFFER_SIZE> buffer_;
 public:
  UTF16ToUTF8() {}
  explicit UTF16ToUTF8(WStringRef s);
  operator StringRef() const { return StringRef(&buffer_[0], size()); }
  size_t size() const { return buffer_.size() - 1; }
  const char *c_str() const { return &buffer_[0]; }
  std::string str() const { return std::string(&buffer_[0], size()); }
  int convert(WStringRef s);
};
#endif
void format_system_error(fmt::Writer &out, int error_code,
                         fmt::StringRef message) FMT_NOEXCEPT;
#ifdef _WIN32
void format_windows_error(fmt::Writer &out, int error_code,
                          fmt::StringRef message) FMT_NOEXCEPT;
#endif
template <unsigned N>
struct NonZero {
  enum { VALUE = N > 0 ? N : 1 };
};
struct Value {
  template <typename Char>
  struct StringValue {
    const Char *value;
    std::size_t size;
  };
  typedef void (*FormatFunc)(
      void *formatter, const void *arg, void *format_str_ptr);
  struct CustomValue {
    const void *value;
    FormatFunc format;
  };
  union {
    int int_value;
    unsigned uint_value;
    LongLong long_long_value;
    ULongLong ulong_long_value;
    double double_value;
    long double long_double_value;
    const void *pointer;
    StringValue<char> string;
    StringValue<signed char> sstring;
    StringValue<unsigned char> ustring;
    StringValue<wchar_t> wstring;
    CustomValue custom;
  };
  enum Type {
    NONE,
    INT, UINT, LONG_LONG, ULONG_LONG, CHAR, LAST_INTEGER_TYPE = CHAR,
    DOUBLE, LONG_DOUBLE, LAST_NUMERIC_TYPE = LONG_DOUBLE,
    CSTRING, STRING, WSTRING, POINTER, CUSTOM
  };
};
struct Arg : Value {
  Type type;
};
template <typename T = void>
struct None {};
template <typename T, typename Char>
struct WCharHelper {
  typedef None<T> Supported;
  typedef T Unsupported;
};
template <typename T>
struct WCharHelper<T, wchar_t> {
  typedef T Supported;
  typedef None<T> Unsupported;
};
template <typename T>
class IsConvertibleToInt {
 private:
  typedef char yes[1];
  typedef char no[2];
  static const T &get();
  static yes &check(fmt::ULongLong);
  static no &check(...);
 public:
  enum { value = (sizeof(check(get())) == sizeof(yes)) };
};
#define FMT_CONVERTIBLE_TO_INT(Type) \
  template <> \
  class IsConvertibleToInt<Type> { \
   public: \
    enum { value = 1 }; \
  }
FMT_CONVERTIBLE_TO_INT(float);
FMT_CONVERTIBLE_TO_INT(double);
FMT_CONVERTIBLE_TO_INT(long double);
template<bool B, class T = void>
struct EnableIf {};
template<class T>
struct EnableIf<true, T> { typedef T type; };
template<bool B, class T, class F>
struct Conditional { typedef T type; };
template<class T, class F>
struct Conditional<false, T, F> { typedef F type; };
inline bool check(bool value) { return value; }
template <typename Char>
class MakeValue : public Arg {
 private:
  template <typename T>
  MakeValue(const T *value);
  template <typename T>
  MakeValue(T *value);
  MakeValue(typename WCharHelper<wchar_t, Char>::Unsupported);
  MakeValue(typename WCharHelper<wchar_t *, Char>::Unsupported);
  MakeValue(typename WCharHelper<const wchar_t *, Char>::Unsupported);
  MakeValue(typename WCharHelper<const std::wstring &, Char>::Unsupported);
  MakeValue(typename WCharHelper<WStringRef, Char>::Unsupported);
  void set_string(StringRef str) {
    string.value = str.c_str();
    string.size = str.size();
  }
  void set_string(WStringRef str) {
    wstring.value = str.c_str();
    wstring.size = str.size();
  }
  template <typename T>
  static void format_custom_arg(
      void *formatter, const void *arg, void *format_str_ptr) {
    format(*static_cast<BasicFormatter<Char>*>(formatter),
           *static_cast<const Char**>(format_str_ptr),
           *static_cast<const T*>(arg));
  }
 public:
  MakeValue() {}
#define FMT_MAKE_VALUE(Type, field, TYPE) \
  MakeValue(Type value) { field = value; } \
  static uint64_t type(Type) { return Arg::TYPE; }
  FMT_MAKE_VALUE(bool, int_value, INT)
  FMT_MAKE_VALUE(short, int_value, INT)
  FMT_MAKE_VALUE(unsigned short, uint_value, UINT)
  FMT_MAKE_VALUE(int, int_value, INT)
  FMT_MAKE_VALUE(unsigned, uint_value, UINT)
  MakeValue(long value) {
    if (check(sizeof(long) == sizeof(int)))
      int_value = static_cast<int>(value);
    else
      long_long_value = value;
  }
  static uint64_t type(long) {
    return sizeof(long) == sizeof(int) ? Arg::INT : Arg::LONG_LONG;
  }
  MakeValue(unsigned long value) {
    if (check(sizeof(unsigned long) == sizeof(unsigned)))
      uint_value = static_cast<unsigned>(value);
    else
      ulong_long_value = value;
  }
  static uint64_t type(unsigned long) {
    return sizeof(unsigned long) == sizeof(unsigned) ?
          Arg::UINT : Arg::ULONG_LONG;
  }
  FMT_MAKE_VALUE(LongLong, long_long_value, LONG_LONG)
  FMT_MAKE_VALUE(ULongLong, ulong_long_value, ULONG_LONG)
  FMT_MAKE_VALUE(float, double_value, DOUBLE)
  FMT_MAKE_VALUE(double, double_value, DOUBLE)
  FMT_MAKE_VALUE(long double, long_double_value, LONG_DOUBLE)
  FMT_MAKE_VALUE(signed char, int_value, CHAR)
  FMT_MAKE_VALUE(unsigned char, int_value, CHAR)
  FMT_MAKE_VALUE(char, int_value, CHAR)
  MakeValue(typename WCharHelper<wchar_t, Char>::Supported value) {
    int_value = value;
  }
  static uint64_t type(wchar_t) { return Arg::CHAR; }
#define FMT_MAKE_STR_VALUE(Type, TYPE) \
  MakeValue(Type value) { set_string(value); } \
  static uint64_t type(Type) { return Arg::TYPE; }
  FMT_MAKE_VALUE(char *, string.value, CSTRING)
  FMT_MAKE_VALUE(const char *, string.value, CSTRING)
  FMT_MAKE_VALUE(const signed char *, sstring.value, CSTRING)
  FMT_MAKE_VALUE(const unsigned char *, ustring.value, CSTRING)
  FMT_MAKE_STR_VALUE(const std::string &, STRING)
  FMT_MAKE_STR_VALUE(StringRef, STRING)
#define FMT_MAKE_WSTR_VALUE(Type, TYPE) \
  MakeValue(typename WCharHelper<Type, Char>::Supported value) { \
    set_string(value); \
  } \
  static uint64_t type(Type) { return Arg::TYPE; }
  FMT_MAKE_WSTR_VALUE(wchar_t *, WSTRING)
  FMT_MAKE_WSTR_VALUE(const wchar_t *, WSTRING)
  FMT_MAKE_WSTR_VALUE(const std::wstring &, WSTRING)
  FMT_MAKE_WSTR_VALUE(WStringRef, WSTRING)
  FMT_MAKE_VALUE(void *, pointer, POINTER)
  FMT_MAKE_VALUE(const void *, pointer, POINTER)
  template <typename T>
  MakeValue(const T &value,
            typename EnableIf<!IsConvertibleToInt<T>::value, int>::type = 0) {
    custom.value = &value;
    custom.format = &format_custom_arg<T>;
  }
  template <typename T>
  MakeValue(const T &value,
            typename EnableIf<IsConvertibleToInt<T>::value, int>::type = 0) {
    int_value = value;
  }
  template <typename T>
  static uint64_t type(const T &) {
    return IsConvertibleToInt<T>::value ? Arg::INT : Arg::CUSTOM;
  }
};
#define FMT_DISPATCH(call) static_cast<Impl*>(this)->call
template <typename Impl, typename Result>
class ArgVisitor {
 public:
  void report_unhandled_arg() {}
  Result visit_unhandled_arg() {
    FMT_DISPATCH(report_unhandled_arg());
    return Result();
  }
  Result visit_int(int value) {
    return FMT_DISPATCH(visit_any_int(value));
  }
  Result visit_long_long(LongLong value) {
    return FMT_DISPATCH(visit_any_int(value));
  }
  Result visit_uint(unsigned value) {
    return FMT_DISPATCH(visit_any_int(value));
  }
  Result visit_ulong_long(ULongLong value) {
    return FMT_DISPATCH(visit_any_int(value));
  }
  Result visit_char(int value) {
    return FMT_DISPATCH(visit_any_int(value));
  }
  template <typename T>
  Result visit_any_int(T) {
    return FMT_DISPATCH(visit_unhandled_arg());
  }
  Result visit_double(double value) {
    return FMT_DISPATCH(visit_any_double(value));
  }
  Result visit_long_double(long double value) {
    return FMT_DISPATCH(visit_any_double(value));
  }
  template <typename T>
  Result visit_any_double(T) {
    return FMT_DISPATCH(visit_unhandled_arg());
  }
  Result visit_string(Arg::StringValue<char>) {
    return FMT_DISPATCH(visit_unhandled_arg());
  }
  Result visit_wstring(Arg::StringValue<wchar_t>) {
    return FMT_DISPATCH(visit_unhandled_arg());
  }
  Result visit_pointer(const void *) {
    return FMT_DISPATCH(visit_unhandled_arg());
  }
  Result visit_custom(Arg::CustomValue) {
    return FMT_DISPATCH(visit_unhandled_arg());
  }
  Result visit(const Arg &arg) {
    switch (arg.type) {
    default:
      assert(false);
      return Result();
    case Arg::INT:
      return FMT_DISPATCH(visit_int(arg.int_value));
    case Arg::UINT:
      return FMT_DISPATCH(visit_uint(arg.uint_value));
    case Arg::LONG_LONG:
      return FMT_DISPATCH(visit_long_long(arg.long_long_value));
    case Arg::ULONG_LONG:
      return FMT_DISPATCH(visit_ulong_long(arg.ulong_long_value));
    case Arg::DOUBLE:
      return FMT_DISPATCH(visit_double(arg.double_value));
    case Arg::LONG_DOUBLE:
      return FMT_DISPATCH(visit_long_double(arg.long_double_value));
    case Arg::CHAR:
      return FMT_DISPATCH(visit_char(arg.int_value));
    case Arg::CSTRING: {
      Arg::StringValue<char> str = arg.string;
      str.size = 0;
      return FMT_DISPATCH(visit_string(str));
    }
    case Arg::STRING:
      return FMT_DISPATCH(visit_string(arg.string));
    case Arg::WSTRING:
      return FMT_DISPATCH(visit_wstring(arg.wstring));
    case Arg::POINTER:
      return FMT_DISPATCH(visit_pointer(arg.pointer));
    case Arg::CUSTOM:
      return FMT_DISPATCH(visit_custom(arg.custom));
    }
  }
};
class RuntimeError : public std::runtime_error {
 protected:
  RuntimeError() : std::runtime_error("") {}
};
template <typename Char>
class ArgFormatter;
}
class ArgList {
 private:
  uint64_t types_;
  union {
    const internal::Value *values_;
    const internal::Arg *args_;
  };
  internal::Arg::Type type(unsigned index) const {
    unsigned shift = index * 4;
    uint64_t mask = 0xf;
    return static_cast<internal::Arg::Type>(
          (types_ & (mask << shift)) >> shift);
  }
 public:
  enum { MAX_PACKED_ARGS = 16 };
  ArgList() : types_(0) {}
  ArgList(ULongLong types, const internal::Value *values)
  : types_(types), values_(values) {}
  ArgList(ULongLong types, const internal::Arg *args)
  : types_(types), args_(args) {}
  internal::Arg operator[](unsigned index) const {
    using internal::Arg;
    Arg arg;
    bool use_values = type(MAX_PACKED_ARGS - 1) == Arg::NONE;
    if (index < MAX_PACKED_ARGS) {
      Arg::Type arg_type = type(index);
      internal::Value &val = arg;
      if (arg_type != Arg::NONE)
        val = use_values ? values_[index] : args_[index];
      arg.type = arg_type;
      return arg;
    }
    if (use_values) {
      arg.type = Arg::NONE;
      return arg;
    }
    for (unsigned i = MAX_PACKED_ARGS; i <= index; ++i) {
      if (args_[i].type == Arg::NONE)
        return args_[i];
    }
    return args_[index];
  }
};
struct FormatSpec;
namespace internal {
template <std::size_t NUM_ARGS>
struct SelectValueType {
  typedef typename Conditional<
    (NUM_ARGS < ArgList::MAX_PACKED_ARGS), Value, Arg>::type Type;
};
class FormatterBase {
 private:
  ArgList args_;
  int next_arg_index_;
  Arg do_get_arg(unsigned arg_index, const char *&error);
 protected:
  void set_args(const ArgList &args) {
    args_ = args;
    next_arg_index_ = 0;
  }
  Arg next_arg(const char *&error);
  Arg get_arg(unsigned arg_index, const char *&error);
  template <typename Char>
  void write(BasicWriter<Char> &w, const Char *start, const Char *end) {
    if (start != end)
      w << BasicStringRef<Char>(start, end - start);
  }
};
template <typename Char>
class PrintfFormatter : private FormatterBase {
 private:
  void parse_flags(FormatSpec &spec, const Char *&s);
  Arg get_arg(const Char *s,
      unsigned arg_index = (std::numeric_limits<unsigned>::max)());
  unsigned parse_header(const Char *&s, FormatSpec &spec);
 public:
  void format(BasicWriter<Char> &writer,
    BasicStringRef<Char> format_str, const ArgList &args);
};
}
template <typename Char>
class BasicFormatter : private internal::FormatterBase {
 private:
  BasicWriter<Char> &writer_;
  const Char *start_;
  FMT_DISALLOW_COPY_AND_ASSIGN(BasicFormatter);
  internal::Arg parse_arg_index(const Char *&s);
 public:
  explicit BasicFormatter(BasicWriter<Char> &w) : writer_(w) {}
  BasicWriter<Char> &writer() { return writer_; }
  void format(BasicStringRef<Char> format_str, const ArgList &args);
  const Char *format(const Char *&format_str, const internal::Arg &arg);
};
enum Alignment {
  ALIGN_DEFAULT, ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER, ALIGN_NUMERIC
};
enum {
  SIGN_FLAG = 1, PLUS_FLAG = 2, MINUS_FLAG = 4, HASH_FLAG = 8,
  CHAR_FLAG = 0x10
};
struct EmptySpec {};
template <char TYPE>
struct TypeSpec : EmptySpec {
  Alignment align() const { return ALIGN_DEFAULT; }
  unsigned width() const { return 0; }
  int precision() const { return -1; }
  bool flag(unsigned) const { return false; }
  char type() const { return TYPE; }
  char fill() const { return ' '; }
};
struct WidthSpec {
  unsigned width_;
  wchar_t fill_;
  WidthSpec(unsigned width, wchar_t fill) : width_(width), fill_(fill) {}
  unsigned width() const { return width_; }
  wchar_t fill() const { return fill_; }
};
struct AlignSpec : WidthSpec {
  Alignment align_;
  AlignSpec(unsigned width, wchar_t fill, Alignment align = ALIGN_DEFAULT)
  : WidthSpec(width, fill), align_(align) {}
  Alignment align() const { return align_; }
  int precision() const { return -1; }
};
template <char TYPE>
struct AlignTypeSpec : AlignSpec {
  AlignTypeSpec(unsigned width, wchar_t fill) : AlignSpec(width, fill) {}
  bool flag(unsigned) const { return false; }
  char type() const { return TYPE; }
};
struct FormatSpec : AlignSpec {
  unsigned flags_;
  int precision_;
  char type_;
  FormatSpec(
    unsigned width = 0, char type = 0, wchar_t fill = ' ')
  : AlignSpec(width, fill), flags_(0), precision_(-1), type_(type) {}
  bool flag(unsigned f) const { return (flags_ & f) != 0; }
  int precision() const { return precision_; }
  char type() const { return type_; }
};
template <typename T, typename SpecT = TypeSpec<0>, typename Char = char>
class IntFormatSpec : public SpecT {
 private:
  T value_;
 public:
  IntFormatSpec(T val, const SpecT &spec = SpecT())
  : SpecT(spec), value_(val) {}
  T value() const { return value_; }
};
template <typename T>
class StrFormatSpec : public AlignSpec {
 private:
  const T *str_;
 public:
  StrFormatSpec(const T *str, unsigned width, wchar_t fill)
  : AlignSpec(width, fill), str_(str) {}
  const T *str() const { return str_; }
};
IntFormatSpec<int, TypeSpec<'b'> > bin(int value);
IntFormatSpec<int, TypeSpec<'o'> > oct(int value);
IntFormatSpec<int, TypeSpec<'x'> > hex(int value);
IntFormatSpec<int, TypeSpec<'X'> > hexu(int value);
template <char TYPE_CODE, typename Char>
IntFormatSpec<int, AlignTypeSpec<TYPE_CODE>, Char> pad(
    int value, unsigned width, Char fill = ' ');
#define FMT_DEFINE_INT_FORMATTERS(TYPE) \
inline IntFormatSpec<TYPE, TypeSpec<'b'> > bin(TYPE value) { \
  return IntFormatSpec<TYPE, TypeSpec<'b'> >(value, TypeSpec<'b'>()); \
} \
 \
inline IntFormatSpec<TYPE, TypeSpec<'o'> > oct(TYPE value) { \
  return IntFormatSpec<TYPE, TypeSpec<'o'> >(value, TypeSpec<'o'>()); \
} \
 \
inline IntFormatSpec<TYPE, TypeSpec<'x'> > hex(TYPE value) { \
  return IntFormatSpec<TYPE, TypeSpec<'x'> >(value, TypeSpec<'x'>()); \
} \
 \
inline IntFormatSpec<TYPE, TypeSpec<'X'> > hexu(TYPE value) { \
  return IntFormatSpec<TYPE, TypeSpec<'X'> >(value, TypeSpec<'X'>()); \
} \
 \
template <char TYPE_CODE> \
inline IntFormatSpec<TYPE, AlignTypeSpec<TYPE_CODE> > pad( \
    IntFormatSpec<TYPE, TypeSpec<TYPE_CODE> > f, unsigned width) { \
  return IntFormatSpec<TYPE, AlignTypeSpec<TYPE_CODE> >( \
      f.value(), AlignTypeSpec<TYPE_CODE>(width, ' ')); \
} \
 \
 \
 \
 \
 \
template <char TYPE_CODE, typename Char> \
inline IntFormatSpec<TYPE, AlignTypeSpec<TYPE_CODE>, Char> pad( \
    IntFormatSpec<TYPE, TypeSpec<TYPE_CODE>, Char> f, \
    unsigned width, Char fill) { \
  return IntFormatSpec<TYPE, AlignTypeSpec<TYPE_CODE>, Char>( \
      f.value(), AlignTypeSpec<TYPE_CODE>(width, fill)); \
} \
 \
inline IntFormatSpec<TYPE, AlignTypeSpec<0> > pad( \
    TYPE value, unsigned width) { \
  return IntFormatSpec<TYPE, AlignTypeSpec<0> >( \
      value, AlignTypeSpec<0>(width, ' ')); \
} \
 \
template <typename Char> \
inline IntFormatSpec<TYPE, AlignTypeSpec<0>, Char> pad( \
   TYPE value, unsigned width, Char fill) { \
 return IntFormatSpec<TYPE, AlignTypeSpec<0>, Char>( \
     value, AlignTypeSpec<0>(width, fill)); \
}
FMT_DEFINE_INT_FORMATTERS(int)
FMT_DEFINE_INT_FORMATTERS(long)
FMT_DEFINE_INT_FORMATTERS(unsigned)
FMT_DEFINE_INT_FORMATTERS(unsigned long)
FMT_DEFINE_INT_FORMATTERS(LongLong)
FMT_DEFINE_INT_FORMATTERS(ULongLong)
template <typename Char>
inline StrFormatSpec<Char> pad(
    const Char *str, unsigned width, Char fill = ' ') {
  return StrFormatSpec<Char>(str, width, fill);
}
inline StrFormatSpec<wchar_t> pad(
    const wchar_t *str, unsigned width, char fill = ' ') {
  return StrFormatSpec<wchar_t>(str, width, fill);
}
# define FMT_GEN(n, f) FMT_GEN##n(f)
# define FMT_GEN1(f)  f(0)
# define FMT_GEN2(f)  FMT_GEN1(f),  f(1)
# define FMT_GEN3(f)  FMT_GEN2(f),  f(2)
# define FMT_GEN4(f)  FMT_GEN3(f),  f(3)
# define FMT_GEN5(f)  FMT_GEN4(f),  f(4)
# define FMT_GEN6(f)  FMT_GEN5(f),  f(5)
# define FMT_GEN7(f)  FMT_GEN6(f),  f(6)
# define FMT_GEN8(f)  FMT_GEN7(f),  f(7)
# define FMT_GEN9(f)  FMT_GEN8(f),  f(8)
# define FMT_GEN10(f) FMT_GEN9(f),  f(9)
# define FMT_GEN11(f) FMT_GEN10(f), f(10)
# define FMT_GEN12(f) FMT_GEN11(f), f(11)
# define FMT_GEN13(f) FMT_GEN12(f), f(12)
# define FMT_GEN14(f) FMT_GEN13(f), f(13)
# define FMT_GEN15(f) FMT_GEN14(f), f(14)
namespace internal {
inline uint64_t make_type() { return 0; }
template <typename T>
inline uint64_t make_type(const T &arg) { return MakeValue<char>::type(arg); }
#if FMT_USE_VARIADIC_TEMPLATES
template <typename Arg, typename... Args>
inline uint64_t make_type(const Arg &first, const Args & ... tail) {
  return make_type(first) | (make_type(tail...) << 4);
}
#else
struct ArgType {
  uint64_t type;
  ArgType() : type(0) {}
  template <typename T>
  ArgType(const T &arg) : type(make_type(arg)) {}
};
# define FMT_ARG_TYPE_DEFAULT(n) ArgType t##n = ArgType()
inline uint64_t make_type(FMT_GEN15(FMT_ARG_TYPE_DEFAULT)) {
  return t0.type | (t1.type << 4) | (t2.type << 8) | (t3.type << 12) |
      (t4.type << 16) | (t5.type << 20) | (t6.type << 24) | (t7.type << 28) |
      (t8.type << 32) | (t9.type << 36) | (t10.type << 40) | (t11.type << 44) |
      (t12.type << 48) | (t13.type << 52) | (t14.type << 56);
}
#endif
}
# define FMT_MAKE_TEMPLATE_ARG(n) typename T##n
# define FMT_MAKE_ARG_TYPE(n) T##n
# define FMT_MAKE_ARG(n) const T##n &v##n
# define FMT_MAKE_REF_char(n) fmt::internal::MakeValue<char>(v##n)
# define FMT_MAKE_REF_wchar_t(n) fmt::internal::MakeValue<wchar_t>(v##n)
#if FMT_USE_VARIADIC_TEMPLATES
# define FMT_VARIADIC_VOID(func, arg_type) \
  template <typename... Args> \
  void func(arg_type arg0, const Args & ... args) { \
    namespace internal = fmt::internal; \
    typedef typename internal::SelectValueType<sizeof...(Args)>::Type Value; \
    const Value array[internal::NonZero<sizeof...(Args)>::VALUE] = { \
      internal::MakeValue<Char>(args)... \
    }; \
    func(arg0, ArgList(internal::make_type(args...), array)); \
  }
# define FMT_VARIADIC_CTOR(ctor, func, arg0_type, arg1_type) \
  template <typename... Args> \
  ctor(arg0_type arg0, arg1_type arg1, const Args & ... args) { \
    namespace internal = fmt::internal; \
    typedef typename internal::SelectValueType<sizeof...(Args)>::Type Value; \
    const Value array[internal::NonZero<sizeof...(Args)>::VALUE] = { \
      internal::MakeValue<Char>(args)... \
    }; \
    func(arg0, arg1, ArgList(internal::make_type(args...), array)); \
  }
#else
# define FMT_MAKE_REF(n) fmt::internal::MakeValue<Char>(v##n)
# define FMT_MAKE_REF2(n) v##n
# define FMT_WRAP1(func, arg_type, n) \
  template <FMT_GEN(n, FMT_MAKE_TEMPLATE_ARG)> \
  inline void func(arg_type arg1, FMT_GEN(n, FMT_MAKE_ARG)) { \
    const fmt::internal::Value values[] = {FMT_GEN(n, FMT_MAKE_REF)}; \
    func(arg1, fmt::ArgList( \
      fmt::internal::make_type(FMT_GEN(n, FMT_MAKE_REF2)), values)); \
  }
# define FMT_VARIADIC_VOID(func, arg_type) \
  inline void func(arg_type arg) { func(arg, fmt::ArgList()); } \
  FMT_WRAP1(func, arg_type, 1) FMT_WRAP1(func, arg_type, 2) \
  FMT_WRAP1(func, arg_type, 3) FMT_WRAP1(func, arg_type, 4) \
  FMT_WRAP1(func, arg_type, 5) FMT_WRAP1(func, arg_type, 6) \
  FMT_WRAP1(func, arg_type, 7) FMT_WRAP1(func, arg_type, 8) \
  FMT_WRAP1(func, arg_type, 9) FMT_WRAP1(func, arg_type, 10)
# define FMT_CTOR(ctor, func, arg0_type, arg1_type, n) \
  template <FMT_GEN(n, FMT_MAKE_TEMPLATE_ARG)> \
  ctor(arg0_type arg0, arg1_type arg1, FMT_GEN(n, FMT_MAKE_ARG)) { \
    const fmt::internal::Value values[] = {FMT_GEN(n, FMT_MAKE_REF)}; \
    func(arg0, arg1, fmt::ArgList( \
      fmt::internal::make_type(FMT_GEN(n, FMT_MAKE_REF2)), values)); \
  }
# define FMT_VARIADIC_CTOR(ctor, func, arg0_type, arg1_type) \
  FMT_CTOR(ctor, func, arg0_type, arg1_type, 1) \
  FMT_CTOR(ctor, func, arg0_type, arg1_type, 2) \
  FMT_CTOR(ctor, func, arg0_type, arg1_type, 3) \
  FMT_CTOR(ctor, func, arg0_type, arg1_type, 4) \
  FMT_CTOR(ctor, func, arg0_type, arg1_type, 5) \
  FMT_CTOR(ctor, func, arg0_type, arg1_type, 6) \
  FMT_CTOR(ctor, func, arg0_type, arg1_type, 7) \
  FMT_CTOR(ctor, func, arg0_type, arg1_type, 8) \
  FMT_CTOR(ctor, func, arg0_type, arg1_type, 9) \
  FMT_CTOR(ctor, func, arg0_type, arg1_type, 10)
#endif
#define FMT_FOR_EACH1(f, x0) f(x0, 0)
#define FMT_FOR_EACH2(f, x0, x1) \
  FMT_FOR_EACH1(f, x0), f(x1, 1)
#define FMT_FOR_EACH3(f, x0, x1, x2) \
  FMT_FOR_EACH2(f, x0 ,x1), f(x2, 2)
#define FMT_FOR_EACH4(f, x0, x1, x2, x3) \
  FMT_FOR_EACH3(f, x0, x1, x2), f(x3, 3)
#define FMT_FOR_EACH5(f, x0, x1, x2, x3, x4) \
  FMT_FOR_EACH4(f, x0, x1, x2, x3), f(x4, 4)
#define FMT_FOR_EACH6(f, x0, x1, x2, x3, x4, x5) \
  FMT_FOR_EACH5(f, x0, x1, x2, x3, x4), f(x5, 5)
#define FMT_FOR_EACH7(f, x0, x1, x2, x3, x4, x5, x6) \
  FMT_FOR_EACH6(f, x0, x1, x2, x3, x4, x5), f(x6, 6)
#define FMT_FOR_EACH8(f, x0, x1, x2, x3, x4, x5, x6, x7) \
  FMT_FOR_EACH7(f, x0, x1, x2, x3, x4, x5, x6), f(x7, 7)
#define FMT_FOR_EACH9(f, x0, x1, x2, x3, x4, x5, x6, x7, x8) \
  FMT_FOR_EACH8(f, x0, x1, x2, x3, x4, x5, x6, x7), f(x8, 8)
#define FMT_FOR_EACH10(f, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9) \
  FMT_FOR_EACH9(f, x0, x1, x2, x3, x4, x5, x6, x7, x8), f(x9, 9)
class SystemError : public internal::RuntimeError {
 private:
  void init(int err_code, StringRef format_str, ArgList args);
 protected:
  int error_code_;
  typedef char Char;
  SystemError() {}
 public:
  SystemError(int error_code, StringRef message) {
    init(error_code, message, ArgList());
  }
  FMT_VARIADIC_CTOR(SystemError, init, int, StringRef)
  int error_code() const { return error_code_; }
};
template <typename Char>
class BasicWriter {
 private:
  Buffer<Char> &buffer_;
  FMT_DISALLOW_COPY_AND_ASSIGN(BasicWriter);
  typedef typename internal::CharTraits<Char>::CharPtr CharPtr;
#if _SECURE_SCL
  static Char *get(CharPtr p) { return p.base(); }
#else
  static Char *get(Char *p) { return p; }
#endif
  static CharPtr fill_padding(CharPtr buffer,
      unsigned total_size, std::size_t content_size, wchar_t fill);
  CharPtr grow_buffer(std::size_t n) {
    std::size_t size = buffer_.size();
    buffer_.resize(size + n);
    return internal::make_ptr(&buffer_[size], n);
  }
  CharPtr prepare_int_buffer(unsigned num_digits,
      const EmptySpec &, const char *prefix, unsigned prefix_size) {
    unsigned size = prefix_size + num_digits;
    CharPtr p = grow_buffer(size);
    std::copy(prefix, prefix + prefix_size, p);
    return p + size - 1;
  }
  template <typename Spec>
  CharPtr prepare_int_buffer(unsigned num_digits,
    const Spec &spec, const char *prefix, unsigned prefix_size);
  template <typename T, typename Spec>
  void write_int(T value, Spec spec);
  template <typename T>
  void write_double(T value, const FormatSpec &spec);
  template <typename StrChar>
  CharPtr write_str(
      const StrChar *s, std::size_t size, const AlignSpec &spec);
  template <typename StrChar>
  void write_str(
      const internal::Arg::StringValue<StrChar> &str, const FormatSpec &spec);
  void operator<<(typename internal::WCharHelper<wchar_t, Char>::Unsupported);
  void operator<<(
      typename internal::WCharHelper<const wchar_t *, Char>::Unsupported);
  void append_float_length(Char *&format_ptr, long double) {
    *format_ptr++ = 'L';
  }
  template<typename T>
  void append_float_length(Char *&, T) {}
  friend class internal::ArgFormatter<Char>;
  friend class internal::PrintfFormatter<Char>;
 protected:
  explicit BasicWriter(Buffer<Char> &b) : buffer_(b) {}
 public:
  virtual ~BasicWriter() {}
  std::size_t size() const { return buffer_.size(); }
  const Char *data() const FMT_NOEXCEPT { return &buffer_[0]; }
  const Char *c_str() const {
    std::size_t size = buffer_.size();
    buffer_.reserve(size + 1);
    buffer_[size] = '\0';
    return &buffer_[0];
  }
  std::basic_string<Char> str() const {
    return std::basic_string<Char>(&buffer_[0], buffer_.size());
  }
  void write(BasicStringRef<Char> format, ArgList args) {
    BasicFormatter<Char>(*this).format(format, args);
  }
  FMT_VARIADIC_VOID(write, BasicStringRef<Char>)
  BasicWriter &operator<<(int value) {
    return *this << IntFormatSpec<int>(value);
  }
  BasicWriter &operator<<(unsigned value) {
    return *this << IntFormatSpec<unsigned>(value);
  }
  BasicWriter &operator<<(long value) {
    return *this << IntFormatSpec<long>(value);
  }
  BasicWriter &operator<<(unsigned long value) {
    return *this << IntFormatSpec<unsigned long>(value);
  }
  BasicWriter &operator<<(LongLong value) {
    return *this << IntFormatSpec<LongLong>(value);
  }
  BasicWriter &operator<<(ULongLong value) {
    return *this << IntFormatSpec<ULongLong>(value);
  }
  BasicWriter &operator<<(double value) {
    write_double(value, FormatSpec());
    return *this;
  }
  BasicWriter &operator<<(long double value) {
    write_double(value, FormatSpec());
    return *this;
  }
  BasicWriter &operator<<(char value) {
    buffer_.push_back(value);
    return *this;
  }
  BasicWriter &operator<<(
      typename internal::WCharHelper<wchar_t, Char>::Supported value) {
    buffer_.push_back(value);
    return *this;
  }
  BasicWriter &operator<<(fmt::BasicStringRef<Char> value) {
    const Char *str = value.c_str();
    buffer_.append(str, str + value.size());
    return *this;
  }
  template <typename T, typename Spec, typename FillChar>
  BasicWriter &operator<<(IntFormatSpec<T, Spec, FillChar> spec) {
    internal::CharTraits<Char>::convert(FillChar());
    write_int(spec.value(), spec);
    return *this;
  }
  template <typename StrChar>
  BasicWriter &operator<<(const StrFormatSpec<StrChar> &spec) {
    const StrChar *s = spec.str();
    write_str(s, std::char_traits<Char>::length(s), spec);
    return *this;
  }
  void clear() FMT_NOEXCEPT { buffer_.clear(); }
};
template <typename Char>
template <typename StrChar>
typename BasicWriter<Char>::CharPtr BasicWriter<Char>::write_str(
      const StrChar *s, std::size_t size, const AlignSpec &spec) {
  CharPtr out = CharPtr();
  if (spec.width() > size) {
    out = grow_buffer(spec.width());
    Char fill = static_cast<Char>(spec.fill());
    if (spec.align() == ALIGN_RIGHT) {
      std::fill_n(out, spec.width() - size, fill);
      out += spec.width() - size;
    } else if (spec.align() == ALIGN_CENTER) {
      out = fill_padding(out, spec.width(), size, fill);
    } else {
      std::fill_n(out + size, spec.width() - size, fill);
    }
  } else {
    out = grow_buffer(size);
  }
  std::copy(s, s + size, out);
  return out;
}
template <typename Char>
typename BasicWriter<Char>::CharPtr
  BasicWriter<Char>::fill_padding(
    CharPtr buffer, unsigned total_size,
    std::size_t content_size, wchar_t fill) {
  std::size_t padding = total_size - content_size;
  std::size_t left_padding = padding / 2;
  Char fill_char = static_cast<Char>(fill);
  std::fill_n(buffer, left_padding, fill_char);
  buffer += left_padding;
  CharPtr content = buffer;
  std::fill_n(buffer + content_size, padding - left_padding, fill_char);
  return content;
}
template <typename Char>
template <typename Spec>
typename BasicWriter<Char>::CharPtr
  BasicWriter<Char>::prepare_int_buffer(
    unsigned num_digits, const Spec &spec,
    const char *prefix, unsigned prefix_size) {
  unsigned width = spec.width();
  Alignment align = spec.align();
  Char fill = static_cast<Char>(spec.fill());
  if (spec.precision() > static_cast<int>(num_digits)) {
    if (prefix_size > 0 && prefix[prefix_size - 1] == '0')
      --prefix_size;
    unsigned number_size = prefix_size + spec.precision();
    AlignSpec subspec(number_size, '0', ALIGN_NUMERIC);
    if (number_size >= width)
      return prepare_int_buffer(num_digits, subspec, prefix, prefix_size);
    buffer_.reserve(width);
    unsigned fill_size = width - number_size;
    if (align != ALIGN_LEFT) {
      CharPtr p = grow_buffer(fill_size);
      std::fill(p, p + fill_size, fill);
    }
    CharPtr result = prepare_int_buffer(
        num_digits, subspec, prefix, prefix_size);
    if (align == ALIGN_LEFT) {
      CharPtr p = grow_buffer(fill_size);
      std::fill(p, p + fill_size, fill);
    }
    return result;
  }
  unsigned size = prefix_size + num_digits;
  if (width <= size) {
    CharPtr p = grow_buffer(size);
    std::copy(prefix, prefix + prefix_size, p);
    return p + size - 1;
  }
  CharPtr p = grow_buffer(width);
  CharPtr end = p + width;
  if (align == ALIGN_LEFT) {
    std::copy(prefix, prefix + prefix_size, p);
    p += size;
    std::fill(p, end, fill);
  } else if (align == ALIGN_CENTER) {
    p = fill_padding(p, width, size, fill);
    std::copy(prefix, prefix + prefix_size, p);
    p += size;
  } else {
    if (align == ALIGN_NUMERIC) {
      if (prefix_size != 0) {
        p = std::copy(prefix, prefix + prefix_size, p);
        size -= prefix_size;
      }
    } else {
      std::copy(prefix, prefix + prefix_size, end - size);
    }
    std::fill(p, end - size, fill);
    p = end;
  }
  return p - 1;
}
template <typename Char>
template <typename T, typename Spec>
void BasicWriter<Char>::write_int(T value, Spec spec) {
  unsigned prefix_size = 0;
  typedef typename internal::IntTraits<T>::MainType UnsignedType;
  UnsignedType abs_value = value;
  char prefix[4] = "";
  if (internal::is_negative(value)) {
    prefix[0] = '-';
    ++prefix_size;
    abs_value = 0 - abs_value;
  } else if (spec.flag(SIGN_FLAG)) {
    prefix[0] = spec.flag(PLUS_FLAG) ? '+' : ' ';
    ++prefix_size;
  }
  switch (spec.type()) {
  case 0: case 'd': {
    unsigned num_digits = internal::count_digits(abs_value);
    CharPtr p = prepare_int_buffer(
      num_digits, spec, prefix, prefix_size) + 1 - num_digits;
    internal::format_decimal(get(p), abs_value, num_digits);
    break;
  }
  case 'x': case 'X': {
    UnsignedType n = abs_value;
    if (spec.flag(HASH_FLAG)) {
      prefix[prefix_size++] = '0';
      prefix[prefix_size++] = spec.type();
    }
    unsigned num_digits = 0;
    do {
      ++num_digits;
    } while ((n >>= 4) != 0);
    Char *p = get(prepare_int_buffer(
      num_digits, spec, prefix, prefix_size));
    n = abs_value;
    const char *digits = spec.type() == 'x' ?
        "0123456789abcdef" : "0123456789ABCDEF";
    do {
      *p-- = digits[n & 0xf];
    } while ((n >>= 4) != 0);
    break;
  }
  case 'b': case 'B': {
    UnsignedType n = abs_value;
    if (spec.flag(HASH_FLAG)) {
      prefix[prefix_size++] = '0';
      prefix[prefix_size++] = spec.type();
    }
    unsigned num_digits = 0;
    do {
      ++num_digits;
    } while ((n >>= 1) != 0);
    Char *p = get(prepare_int_buffer(num_digits, spec, prefix, prefix_size));
    n = abs_value;
    do {
      *p-- = '0' + (n & 1);
    } while ((n >>= 1) != 0);
    break;
  }
  case 'o': {
    UnsignedType n = abs_value;
    if (spec.flag(HASH_FLAG))
      prefix[prefix_size++] = '0';
    unsigned num_digits = 0;
    do {
      ++num_digits;
    } while ((n >>= 3) != 0);
    Char *p = get(prepare_int_buffer(num_digits, spec, prefix, prefix_size));
    n = abs_value;
    do {
      *p-- = '0' + (n & 7);
    } while ((n >>= 3) != 0);
    break;
  }
  default:
    internal::report_unknown_type(
      spec.type(), spec.flag(CHAR_FLAG) ? "char" : "integer");
    break;
  }
}
template <typename Char>
template <typename T>
void BasicWriter<Char>::write_double(
    T value, const FormatSpec &spec) {
  char type = spec.type();
  bool upper = false;
  switch (type) {
  case 0:
    type = 'g';
    break;
  case 'e': case 'f': case 'g': case 'a':
    break;
  case 'F':
#ifdef _MSC_VER
    type = 'f';
#endif
  case 'E': case 'G': case 'A':
    upper = true;
    break;
  default:
    internal::report_unknown_type(type, "double");
    break;
  }
  char sign = 0;
  if (internal::getsign(static_cast<double>(value))) {
    sign = '-';
    value = -value;
  } else if (spec.flag(SIGN_FLAG)) {
    sign = spec.flag(PLUS_FLAG) ? '+' : ' ';
  }
  if (value != value) {
    std::size_t nan_size = 4;
    const char *nan = upper ? " NAN" : " nan";
    if (!sign) {
      --nan_size;
      ++nan;
    }
    CharPtr out = write_str(nan, nan_size, spec);
    if (sign)
      *out = sign;
    return;
  }
  if (internal::isinfinity(value)) {
    std::size_t inf_size = 4;
    const char *inf = upper ? " INF" : " inf";
    if (!sign) {
      --inf_size;
      ++inf;
    }
    CharPtr out = write_str(inf, inf_size, spec);
    if (sign)
      *out = sign;
    return;
  }
  std::size_t offset = buffer_.size();
  unsigned width = spec.width();
  if (sign) {
    buffer_.reserve(buffer_.size() + (std::max)(width, 1u));
    if (width > 0)
      --width;
    ++offset;
  }
  enum { MAX_FORMAT_SIZE = 10};
  Char format[MAX_FORMAT_SIZE];
  Char *format_ptr = format;
  *format_ptr++ = '%';
  unsigned width_for_sprintf = width;
  if (spec.flag(HASH_FLAG))
    *format_ptr++ = '#';
  if (spec.align() == ALIGN_CENTER) {
    width_for_sprintf = 0;
  } else {
    if (spec.align() == ALIGN_LEFT)
      *format_ptr++ = '-';
    if (width != 0)
      *format_ptr++ = '*';
  }
  if (spec.precision() >= 0) {
    *format_ptr++ = '.';
    *format_ptr++ = '*';
  }
  append_float_length(format_ptr, value);
  *format_ptr++ = type;
  *format_ptr = '\0';
  Char fill = static_cast<Char>(spec.fill());
  for (;;) {
    std::size_t buffer_size = buffer_.capacity() - offset;
#if _MSC_VER
    if (buffer_size == 0) {
      buffer_.reserve(offset + 1);
      buffer_size = buffer_.capacity() - offset;
    }
#endif
    Char *start = &buffer_[offset];
    int n = internal::CharTraits<Char>::format_float(
        start, buffer_size, format, width_for_sprintf, spec.precision(), value);
    if (n >= 0 && offset + n < buffer_.capacity()) {
      if (sign) {
        if ((spec.align() != ALIGN_RIGHT && spec.align() != ALIGN_DEFAULT) ||
            *start != ' ') {
          *(start - 1) = sign;
          sign = 0;
        } else {
          *(start - 1) = fill;
        }
        ++n;
      }
      if (spec.align() == ALIGN_CENTER &&
          spec.width() > static_cast<unsigned>(n)) {
        width = spec.width();
        CharPtr p = grow_buffer(width);
        std::copy(p, p + n, p + (width - n) / 2);
        fill_padding(p, spec.width(), n, fill);
        return;
      }
      if (spec.fill() != ' ' || sign) {
        while (*start == ' ')
          *start++ = fill;
        if (sign)
          *(start - 1) = sign;
      }
      grow_buffer(n);
      return;
    }
    buffer_.reserve(n >= 0 ? offset + n + 1 : buffer_.capacity() + 1);
  }
}
template <typename Char, typename Allocator = std::allocator<Char> >
class BasicMemoryWriter : public BasicWriter<Char> {
 private:
  internal::MemoryBuffer<Char, internal::INLINE_BUFFER_SIZE, Allocator> buffer_;
 public:
  explicit BasicMemoryWriter(const Allocator& alloc = Allocator())
    : BasicWriter<Char>(buffer_), buffer_(alloc) {}
#if FMT_USE_RVALUE_REFERENCES
  BasicMemoryWriter(BasicMemoryWriter &&other)
    : BasicWriter<Char>(buffer_), buffer_(std::move(other.buffer_)) {
  }
  BasicMemoryWriter &operator=(BasicMemoryWriter &&other) {
    buffer_ = std::move(other.buffer_);
    return *this;
  }
#endif
};
typedef BasicMemoryWriter<char> MemoryWriter;
typedef BasicMemoryWriter<wchar_t> WMemoryWriter;
template <typename Char>
class BasicArrayWriter : public BasicWriter<Char> {
 private:
  internal::FixedBuffer<Char> buffer_;
 public:
  BasicArrayWriter(Char *array, std::size_t size)
    : BasicWriter<Char>(buffer_), buffer_(array, size) {}
  template <std::size_t SIZE>
  explicit BasicArrayWriter(Char (&array)[SIZE])
    : BasicWriter<Char>(buffer_), buffer_(array, SIZE) {}
};
typedef BasicArrayWriter<char> ArrayWriter;
typedef BasicArrayWriter<wchar_t> WArrayWriter;
template <typename Char, typename T>
void format(BasicFormatter<Char> &f, const Char *&format_str, const T &value) {
  std::basic_ostringstream<Char> os;
  os << value;
  std::basic_string<Char> str = os.str();
  internal::Arg arg = internal::MakeValue<Char>(str);
  arg.type = static_cast<internal::Arg::Type>(
        internal::MakeValue<Char>::type(str));
  format_str = f.format(format_str, arg);
}
void report_system_error(int error_code, StringRef message) FMT_NOEXCEPT;
#ifdef _WIN32
class WindowsError : public SystemError {
 private:
  void init(int error_code, StringRef format_str, ArgList args);
 public:
  WindowsError(int error_code, StringRef message) {
    init(error_code, message, ArgList());
  }
  FMT_VARIADIC_CTOR(WindowsError, init, int, StringRef)
};
void report_windows_error(int error_code, StringRef message) FMT_NOEXCEPT;
#endif
enum Color { BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE };
void print_colored(Color c, StringRef format, ArgList args);
inline std::string format(StringRef format_str, ArgList args) {
  MemoryWriter w;
  w.write(format_str, args);
  return w.str();
}
inline std::wstring format(WStringRef format_str, ArgList args) {
  WMemoryWriter w;
  w.write(format_str, args);
  return w.str();
}
void print(std::FILE *f, StringRef format_str, ArgList args);
void print(StringRef format_str, ArgList args);
void print(std::ostream &os, StringRef format_str, ArgList args);
template <typename Char>
void printf(BasicWriter<Char> &w, BasicStringRef<Char> format, ArgList args) {
  internal::PrintfFormatter<Char>().format(w, format, args);
}
inline std::string sprintf(StringRef format, ArgList args) {
  MemoryWriter w;
  printf(w, format, args);
  return w.str();
}
int fprintf(std::FILE *f, StringRef format, ArgList args);
inline int printf(StringRef format, ArgList args) {
  return fprintf(stdout, format, args);
}
class FormatInt {
 private:
  enum {BUFFER_SIZE = std::numeric_limits<ULongLong>::digits10 + 3};
  mutable char buffer_[BUFFER_SIZE];
  char *str_;
  char *format_decimal(ULongLong value) {
    char *buffer_end = buffer_ + BUFFER_SIZE - 1;
    while (value >= 100) {
      unsigned index = (value % 100) * 2;
      value /= 100;
      *--buffer_end = internal::Data::DIGITS[index + 1];
      *--buffer_end = internal::Data::DIGITS[index];
    }
    if (value < 10) {
      *--buffer_end = static_cast<char>('0' + value);
      return buffer_end;
    }
    unsigned index = static_cast<unsigned>(value * 2);
    *--buffer_end = internal::Data::DIGITS[index + 1];
    *--buffer_end = internal::Data::DIGITS[index];
    return buffer_end;
  }
  void FormatSigned(LongLong value) {
    ULongLong abs_value = static_cast<ULongLong>(value);
    bool negative = value < 0;
    if (negative)
      abs_value = 0 - abs_value;
    str_ = format_decimal(abs_value);
    if (negative)
      *--str_ = '-';
  }
 public:
  explicit FormatInt(int value) { FormatSigned(value); }
  explicit FormatInt(long value) { FormatSigned(value); }
  explicit FormatInt(LongLong value) { FormatSigned(value); }
  explicit FormatInt(unsigned value) : str_(format_decimal(value)) {}
  explicit FormatInt(unsigned long value) : str_(format_decimal(value)) {}
  explicit FormatInt(ULongLong value) : str_(format_decimal(value)) {}
  std::size_t size() const { return buffer_ - str_ + BUFFER_SIZE - 1; }
  const char *data() const { return str_; }
  const char *c_str() const {
    buffer_[BUFFER_SIZE - 1] = '\0';
    return str_;
  }
  std::string str() const { return std::string(str_, size()); }
};
template <typename T>
inline void format_decimal(char *&buffer, T value) {
  typename internal::IntTraits<T>::MainType abs_value = value;
  if (internal::is_negative(value)) {
    *buffer++ = '-';
    abs_value = 0 - abs_value;
  }
  if (abs_value < 100) {
    if (abs_value < 10) {
      *buffer++ = static_cast<char>('0' + abs_value);
      return;
    }
    unsigned index = static_cast<unsigned>(abs_value * 2);
    *buffer++ = internal::Data::DIGITS[index];
    *buffer++ = internal::Data::DIGITS[index + 1];
    return;
  }
  unsigned num_digits = internal::count_digits(abs_value);
  internal::format_decimal(buffer, abs_value, num_digits);
  buffer += num_digits;
}
}
#if FMT_GCC_VERSION
# pragma GCC system_header
#endif
#define FMT_EXPAND(args) args
#define FMT_NARG(...) FMT_NARG_(__VA_ARGS__, FMT_RSEQ_N())
#define FMT_NARG_(...) FMT_EXPAND(FMT_ARG_N(__VA_ARGS__))
#define FMT_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define FMT_RSEQ_N() 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define FMT_CONCAT(a, b) a##b
#define FMT_FOR_EACH_(N, f, ...) \
  FMT_EXPAND(FMT_CONCAT(FMT_FOR_EACH, N)(f, __VA_ARGS__))
#define FMT_FOR_EACH(f, ...) \
  FMT_EXPAND(FMT_FOR_EACH_(FMT_NARG(__VA_ARGS__), f, __VA_ARGS__))
#define FMT_ADD_ARG_NAME(type, index) type arg##index
#define FMT_GET_ARG_NAME(type, index) arg##index
#if FMT_USE_VARIADIC_TEMPLATES
namespace fmt {
namespace internal {
inline void do_set_types(Arg *) {}
template <typename T, typename... Args>
inline void do_set_types(Arg *args, const T &arg, const Args & ... tail) {
  args->type = static_cast<Arg::Type>(MakeValue<T>::type(arg));
  do_set_types(args + 1, tail...);
}
template <typename... Args>
inline void set_types(Arg *array, const Args & ... args) {
  do_set_types(array, args...);
  array[sizeof...(Args)].type = Arg::NONE;
}
template <typename... Args>
inline void set_types(Value *, const Args & ...) {
}
template <unsigned N>
struct ArgArraySize {
  enum { VALUE = N + (N == 0 || N > ArgList::MAX_PACKED_ARGS ? 1 : 0) };
};
}
}
# define FMT_VARIADIC_(Char, ReturnType, func, call, ...) \
  template <typename... Args> \
  ReturnType func(FMT_FOR_EACH(FMT_ADD_ARG_NAME, __VA_ARGS__), \
      const Args & ... args) { \
    namespace internal = fmt::internal; \
    typedef typename internal::SelectValueType<sizeof...(Args)>::Type Value; \
    Value array[internal::ArgArraySize<sizeof...(Args)>::VALUE] = { \
      internal::MakeValue<Char>(args)... \
    }; \
    if (internal::check((sizeof...(Args) > fmt::ArgList::MAX_PACKED_ARGS))) \
      set_types(array, args...); \
    call(FMT_FOR_EACH(FMT_GET_ARG_NAME, __VA_ARGS__), \
      fmt::ArgList(internal::make_type(args...), array)); \
  }
#else
# define FMT_WRAP(Char, ReturnType, func, call, n, ...) \
  template <FMT_GEN(n, FMT_MAKE_TEMPLATE_ARG)> \
  inline ReturnType func(FMT_FOR_EACH(FMT_ADD_ARG_NAME, __VA_ARGS__), \
      FMT_GEN(n, FMT_MAKE_ARG)) { \
    const fmt::internal::Value values[] = {FMT_GEN(n, FMT_MAKE_REF_##Char)}; \
    call(FMT_FOR_EACH(FMT_GET_ARG_NAME, __VA_ARGS__), fmt::ArgList( \
      fmt::internal::make_type(FMT_GEN(n, FMT_MAKE_REF2)), values)); \
  }
# define FMT_VARIADIC_(Char, ReturnType, func, call, ...) \
  inline ReturnType func(FMT_FOR_EACH(FMT_ADD_ARG_NAME, __VA_ARGS__)) { \
    call(FMT_FOR_EACH(FMT_GET_ARG_NAME, __VA_ARGS__), fmt::ArgList()); \
  } \
  FMT_WRAP(Char, ReturnType, func, call, 1, __VA_ARGS__) \
  FMT_WRAP(Char, ReturnType, func, call, 2, __VA_ARGS__) \
  FMT_WRAP(Char, ReturnType, func, call, 3, __VA_ARGS__) \
  FMT_WRAP(Char, ReturnType, func, call, 4, __VA_ARGS__) \
  FMT_WRAP(Char, ReturnType, func, call, 5, __VA_ARGS__) \
  FMT_WRAP(Char, ReturnType, func, call, 6, __VA_ARGS__) \
  FMT_WRAP(Char, ReturnType, func, call, 7, __VA_ARGS__) \
  FMT_WRAP(Char, ReturnType, func, call, 8, __VA_ARGS__) \
  FMT_WRAP(Char, ReturnType, func, call, 9, __VA_ARGS__) \
  FMT_WRAP(Char, ReturnType, func, call, 10, __VA_ARGS__) \
  FMT_WRAP(Char, ReturnType, func, call, 11, __VA_ARGS__) \
  FMT_WRAP(Char, ReturnType, func, call, 12, __VA_ARGS__) \
  FMT_WRAP(Char, ReturnType, func, call, 13, __VA_ARGS__) \
  FMT_WRAP(Char, ReturnType, func, call, 14, __VA_ARGS__) \
  FMT_WRAP(Char, ReturnType, func, call, 15, __VA_ARGS__)
#endif
#define FMT_VARIADIC(ReturnType, func, ...) \
  FMT_VARIADIC_(char, ReturnType, func, return func, __VA_ARGS__)
#define FMT_VARIADIC_W(ReturnType, func, ...) \
  FMT_VARIADIC_(wchar_t, ReturnType, func, return func, __VA_ARGS__)
namespace fmt {
FMT_VARIADIC(std::string, format, StringRef)
FMT_VARIADIC_W(std::wstring, format, WStringRef)
FMT_VARIADIC(void, print, StringRef)
FMT_VARIADIC(void, print, std::FILE *, StringRef)
FMT_VARIADIC(void, print, std::ostream &, StringRef)
FMT_VARIADIC(void, print_colored, Color, StringRef)
FMT_VARIADIC(std::string, sprintf, StringRef)
FMT_VARIADIC(int, printf, StringRef)
FMT_VARIADIC(int, fprintf, std::FILE *, StringRef)
}
#if FMT_GCC_VERSION >= 406
# pragma GCC diagnostic pop
#endif
#ifdef __clang__
# pragma clang diagnostic pop
#endif
#ifdef FMT_HEADER_ONLY
/*# include "format.cc"*/
/*#include "format.h" skipped */
#include <string.h>
#include <cctype>
#include <cerrno>
#include <climits>
#include <cmath>
#include <cstdarg>
#ifdef _WIN32
# ifdef __MINGW32__
#  include <cstring>
# endif
# include <windows.h>
#endif
using fmt::internal::Arg;
#if __GNUC__ && !__EXCEPTIONS
# define FMT_EXCEPTIONS 0
#endif
#if _MSC_VER && !_HAS_EXCEPTIONS
# define FMT_EXCEPTIONS 0
#endif
#ifndef FMT_EXCEPTIONS
# define FMT_EXCEPTIONS 1
#endif
#if FMT_EXCEPTIONS
# define FMT_TRY try
# define FMT_CATCH(x) catch (x)
#else
# define FMT_TRY if (true)
# define FMT_CATCH(x) if (false)
#endif
#ifndef FMT_THROW
# if FMT_EXCEPTIONS
#  define FMT_THROW(x) throw x
# else
#  define FMT_THROW(x) assert(false)
# endif
#endif
#ifdef FMT_HEADER_ONLY
# define FMT_FUNC inline
#else
# define FMT_FUNC
#endif
#if _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4127)
# pragma warning(disable: 4702)
# pragma warning(disable: 4996)
#endif
static inline fmt::internal::None<> strerror_r(int, char *, ...) {
  return fmt::internal::None<>();
}
static inline fmt::internal::None<> strerror_s(char *, std::size_t, ...) {
  return fmt::internal::None<>();
}
namespace {
#ifndef _MSC_VER
# define FMT_SNPRINTF snprintf
#else
inline int fmt_snprintf(char *buffer, size_t size, const char *format, ...) {
  va_list args;
  va_start(args, format);
  int result = vsnprintf_s(buffer, size, _TRUNCATE, format, args);
  va_end(args);
  return result;
}
# define FMT_SNPRINTF fmt_snprintf
#endif
#if defined(_WIN32) && defined(__MINGW32__) && !defined(__NO_ISOCEXT)
# define FMT_SWPRINTF snwprintf
#else
# define FMT_SWPRINTF swprintf
#endif
template <bool IsSigned>
struct IntChecker {
  template <typename T>
  static bool fits_in_int(T value) {
    unsigned max = INT_MAX;
    return value <= max;
  }
};
template <>
struct IntChecker<true> {
  template <typename T>
  static bool fits_in_int(T value) {
    return value >= INT_MIN && value <= INT_MAX;
  }
};
const char RESET_COLOR[] = "\x1b[0m";
typedef void (*FormatFunc)(fmt::Writer &, int, fmt::StringRef);
int safe_strerror(
    int error_code, char *&buffer, std::size_t buffer_size) FMT_NOEXCEPT {
  assert(buffer != 0 && buffer_size != 0);
  class StrError {
   private:
    int error_code_;
    char *&buffer_;
    std::size_t buffer_size_;
    void operator=(const StrError &) {}
    int handle(int result) {
      return result == -1 ? errno : result;
    }
    int handle(char *message) {
      if (message == buffer_ && strlen(buffer_) == buffer_size_ - 1)
        return ERANGE;
      buffer_ = message;
      return 0;
    }
    int handle(fmt::internal::None<>) {
      return fallback(strerror_s(buffer_, buffer_size_, error_code_));
    }
    int fallback(int result) {
      return result == 0 && strlen(buffer_) == buffer_size_ - 1 ?
            ERANGE : result;
    }
    int fallback(fmt::internal::None<>) {
      errno = 0;
      buffer_ = strerror(error_code_);
      return errno;
    }
   public:
    StrError(int error_code, char *&buffer, std::size_t buffer_size)
      : error_code_(error_code), buffer_(buffer), buffer_size_(buffer_size) {}
    int run() { return handle(strerror_r(error_code_, buffer_, buffer_size_)); }
  };
  return StrError(error_code, buffer, buffer_size).run();
}
void format_error_code(fmt::Writer &out, int error_code,
                       fmt::StringRef message) FMT_NOEXCEPT {
  out.clear();
  static const char SEP[] = ": ";
  static const char ERROR_STR[] = "error ";
  fmt::internal::IntTraits<int>::MainType ec_value = error_code;
  std::size_t error_code_size = sizeof(SEP) + sizeof(ERROR_STR) - 2;
  error_code_size += fmt::internal::count_digits(ec_value);
  if (message.size() <= fmt::internal::INLINE_BUFFER_SIZE - error_code_size)
    out << message << SEP;
  out << ERROR_STR << error_code;
  assert(out.size() <= fmt::internal::INLINE_BUFFER_SIZE);
}
void report_error(FormatFunc func,
    int error_code, fmt::StringRef message) FMT_NOEXCEPT {
  fmt::MemoryWriter full_message;
  func(full_message, error_code, message);
  std::fwrite(full_message.data(), full_message.size(), 1, stderr);
  std::fputc('\n', stderr);
}
class IsZeroInt : public fmt::internal::ArgVisitor<IsZeroInt, bool> {
 public:
  template <typename T>
  bool visit_any_int(T value) { return value == 0; }
};
template <typename Char>
int parse_nonnegative_int(const Char *&s) {
  assert('0' <= *s && *s <= '9');
  unsigned value = 0;
  do {
    unsigned new_value = value * 10 + (*s++ - '0');
    if (new_value < value) {
      value = UINT_MAX;
      break;
    }
    value = new_value;
  } while ('0' <= *s && *s <= '9');
  if (value > INT_MAX)
    FMT_THROW(fmt::FormatError("number is too big"));
  return value;
}
inline void require_numeric_argument(const Arg &arg, char spec) {
  if (arg.type > Arg::LAST_NUMERIC_TYPE) {
    std::string message =
        fmt::format("format specifier '{}' requires numeric argument", spec);
    FMT_THROW(fmt::FormatError(message));
  }
}
template <typename Char>
void check_sign(const Char *&s, const Arg &arg) {
  char sign = static_cast<char>(*s);
  require_numeric_argument(arg, sign);
  if (arg.type == Arg::UINT || arg.type == Arg::ULONG_LONG) {
    FMT_THROW(fmt::FormatError(fmt::format(
      "format specifier '{}' requires signed argument", sign)));
  }
  ++s;
}
class WidthHandler : public fmt::internal::ArgVisitor<WidthHandler, unsigned> {
 private:
  fmt::FormatSpec &spec_;
  FMT_DISALLOW_COPY_AND_ASSIGN(WidthHandler);
 public:
  explicit WidthHandler(fmt::FormatSpec &spec) : spec_(spec) {}
  void report_unhandled_arg() {
    FMT_THROW(fmt::FormatError("width is not integer"));
  }
  template <typename T>
  unsigned visit_any_int(T value) {
    typedef typename fmt::internal::IntTraits<T>::MainType UnsignedType;
    UnsignedType width = value;
    if (fmt::internal::is_negative(value)) {
      spec_.align_ = fmt::ALIGN_LEFT;
      width = 0 - width;
    }
    if (width > INT_MAX)
      FMT_THROW(fmt::FormatError("number is too big"));
    return static_cast<unsigned>(width);
  }
};
class PrecisionHandler :
    public fmt::internal::ArgVisitor<PrecisionHandler, int> {
 public:
  void report_unhandled_arg() {
    FMT_THROW(fmt::FormatError("precision is not integer"));
  }
  template <typename T>
  int visit_any_int(T value) {
    if (!IntChecker<std::numeric_limits<T>::is_signed>::fits_in_int(value))
      FMT_THROW(fmt::FormatError("number is too big"));
    return static_cast<int>(value);
  }
};
template <typename T>
class ArgConverter : public fmt::internal::ArgVisitor<ArgConverter<T>, void> {
 private:
  fmt::internal::Arg &arg_;
  wchar_t type_;
  FMT_DISALLOW_COPY_AND_ASSIGN(ArgConverter);
 public:
  ArgConverter(fmt::internal::Arg &arg, wchar_t type)
    : arg_(arg), type_(type) {}
  template <typename U>
  void visit_any_int(U value) {
    bool is_signed = type_ == 'd' || type_ == 'i';
    using fmt::internal::Arg;
    if (sizeof(T) <= sizeof(int)) {
      if (is_signed) {
        arg_.type = Arg::INT;
        arg_.int_value = static_cast<int>(static_cast<T>(value));
      } else {
        arg_.type = Arg::UINT;
        arg_.uint_value = static_cast<unsigned>(
            static_cast<typename fmt::internal::MakeUnsigned<T>::Type>(value));
      }
    } else {
      if (is_signed) {
        arg_.type = Arg::LONG_LONG;
        arg_.long_long_value =
            static_cast<typename fmt::internal::MakeUnsigned<U>::Type>(value);
      } else {
        arg_.type = Arg::ULONG_LONG;
        arg_.ulong_long_value =
            static_cast<typename fmt::internal::MakeUnsigned<U>::Type>(value);
      }
    }
  }
};
class CharConverter : public fmt::internal::ArgVisitor<CharConverter, void> {
 private:
  fmt::internal::Arg &arg_;
  FMT_DISALLOW_COPY_AND_ASSIGN(CharConverter);
 public:
  explicit CharConverter(fmt::internal::Arg &arg) : arg_(arg) {}
  template <typename T>
  void visit_any_int(T value) {
    arg_.type = Arg::CHAR;
    arg_.int_value = static_cast<char>(value);
  }
};
template <typename Char>
Arg::StringValue<Char> ignore_incompatible_str(Arg::StringValue<wchar_t>);
template <>
inline Arg::StringValue<char> ignore_incompatible_str(
    Arg::StringValue<wchar_t>) { return Arg::StringValue<char>(); }
template <>
inline Arg::StringValue<wchar_t> ignore_incompatible_str(
    Arg::StringValue<wchar_t> s) { return s; }
}
FMT_FUNC void fmt::SystemError::init(
    int err_code, StringRef format_str, ArgList args) {
  error_code_ = err_code;
  MemoryWriter w;
  internal::format_system_error(w, err_code, format(format_str, args));
  std::runtime_error &base = *this;
  base = std::runtime_error(w.str());
}
template <typename T>
int fmt::internal::CharTraits<char>::format_float(
    char *buffer, std::size_t size, const char *format,
    unsigned width, int precision, T value) {
  if (width == 0) {
    return precision < 0 ?
        FMT_SNPRINTF(buffer, size, format, value) :
        FMT_SNPRINTF(buffer, size, format, precision, value);
  }
  return precision < 0 ?
      FMT_SNPRINTF(buffer, size, format, width, value) :
      FMT_SNPRINTF(buffer, size, format, width, precision, value);
}
template <typename T>
int fmt::internal::CharTraits<wchar_t>::format_float(
    wchar_t *buffer, std::size_t size, const wchar_t *format,
    unsigned width, int precision, T value) {
  if (width == 0) {
    return precision < 0 ?
        FMT_SWPRINTF(buffer, size, format, value) :
        FMT_SWPRINTF(buffer, size, format, precision, value);
  }
  return precision < 0 ?
      FMT_SWPRINTF(buffer, size, format, width, value) :
      FMT_SWPRINTF(buffer, size, format, width, precision, value);
}
template <typename T>
const char fmt::internal::BasicData<T>::DIGITS[] =
    "0001020304050607080910111213141516171819"
    "2021222324252627282930313233343536373839"
    "4041424344454647484950515253545556575859"
    "6061626364656667686970717273747576777879"
    "8081828384858687888990919293949596979899";
#define FMT_POWERS_OF_10(factor) \
  factor * 10, \
  factor * 100, \
  factor * 1000, \
  factor * 10000, \
  factor * 100000, \
  factor * 1000000, \
  factor * 10000000, \
  factor * 100000000, \
  factor * 1000000000
template <typename T>
const uint32_t fmt::internal::BasicData<T>::POWERS_OF_10_32[] = {
  0, FMT_POWERS_OF_10(1)
};
template <typename T>
const uint64_t fmt::internal::BasicData<T>::POWERS_OF_10_64[] = {
  0,
  FMT_POWERS_OF_10(1),
  FMT_POWERS_OF_10(fmt::ULongLong(1000000000)),
  fmt::ULongLong(1000000000) * fmt::ULongLong(1000000000) * 10
};
FMT_FUNC void fmt::internal::report_unknown_type(char code, const char *type) {
  (void)type;
  if (std::isprint(static_cast<unsigned char>(code))) {
    FMT_THROW(fmt::FormatError(
        fmt::format("unknown format code '{}' for {}", code, type)));
  }
  FMT_THROW(fmt::FormatError(
      fmt::format("unknown format code '\\x{:02x}' for {}",
        static_cast<unsigned>(code), type)));
}
#ifdef _WIN32
FMT_FUNC fmt::internal::UTF8ToUTF16::UTF8ToUTF16(fmt::StringRef s) {
  int length = MultiByteToWideChar(
      CP_UTF8, MB_ERR_INVALID_CHARS, s.c_str(), -1, 0, 0);
  static const char ERROR_MSG[] = "cannot convert string from UTF-8 to UTF-16";
  if (length == 0)
    FMT_THROW(WindowsError(GetLastError(), ERROR_MSG));
  buffer_.resize(length);
  length = MultiByteToWideChar(
    CP_UTF8, MB_ERR_INVALID_CHARS, s.c_str(), -1, &buffer_[0], length);
  if (length == 0)
    FMT_THROW(WindowsError(GetLastError(), ERROR_MSG));
}
FMT_FUNC fmt::internal::UTF16ToUTF8::UTF16ToUTF8(fmt::WStringRef s) {
  if (int error_code = convert(s)) {
    FMT_THROW(WindowsError(error_code,
        "cannot convert string from UTF-16 to UTF-8"));
  }
}
FMT_FUNC int fmt::internal::UTF16ToUTF8::convert(fmt::WStringRef s) {
  int length = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, 0, 0, 0, 0);
  if (length == 0)
    return GetLastError();
  buffer_.resize(length);
  length = WideCharToMultiByte(
    CP_UTF8, 0, s.c_str(), -1, &buffer_[0], length, 0, 0);
  if (length == 0)
    return GetLastError();
  return 0;
}
FMT_FUNC void fmt::WindowsError::init(
    int err_code, StringRef format_str, ArgList args) {
  error_code_ = err_code;
  MemoryWriter w;
  internal::format_windows_error(w, err_code, format(format_str, args));
  std::runtime_error &base = *this;
  base = std::runtime_error(w.str());
}
#endif
FMT_FUNC void fmt::internal::format_system_error(
    fmt::Writer &out, int error_code,
    fmt::StringRef message) FMT_NOEXCEPT {
  FMT_TRY {
    MemoryBuffer<char, INLINE_BUFFER_SIZE> buffer;
    buffer.resize(INLINE_BUFFER_SIZE);
    for (;;) {
      char *system_message = &buffer[0];
      int result = safe_strerror(error_code, system_message, buffer.size());
      if (result == 0) {
        out << message << ": " << system_message;
        return;
      }
      if (result != ERANGE)
        break;
      buffer.resize(buffer.size() * 2);
    }
  } FMT_CATCH(...) {}
  format_error_code(out, error_code, message);
}
#ifdef _WIN32
FMT_FUNC void fmt::internal::format_windows_error(
    fmt::Writer &out, int error_code,
    fmt::StringRef message) FMT_NOEXCEPT {
  class String {
   private:
    LPWSTR str_;
   public:
    String() : str_() {}
    ~String() { LocalFree(str_); }
    LPWSTR *ptr() { return &str_; }
    LPCWSTR c_str() const { return str_; }
  };
  FMT_TRY {
    String system_message;
    if (FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0,
        error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPWSTR>(system_message.ptr()), 0, 0)) {
      UTF16ToUTF8 utf8_message;
      if (utf8_message.convert(system_message.c_str()) == ERROR_SUCCESS) {
        out << message << ": " << utf8_message;
        return;
      }
    }
  } FMT_CATCH(...) {}
  format_error_code(out, error_code, message);
}
#endif
template <typename Char>
class fmt::internal::ArgFormatter :
    public fmt::internal::ArgVisitor<fmt::internal::ArgFormatter<Char>, void> {
 private:
  fmt::BasicFormatter<Char> &formatter_;
  fmt::BasicWriter<Char> &writer_;
  fmt::FormatSpec &spec_;
  const Char *format_;
  FMT_DISALLOW_COPY_AND_ASSIGN(ArgFormatter);
 public:
  ArgFormatter(
      fmt::BasicFormatter<Char> &f,fmt::FormatSpec &s, const Char *fmt)
  : formatter_(f), writer_(f.writer()), spec_(s), format_(fmt) {}
  template <typename T>
  void visit_any_int(T value) { writer_.write_int(value, spec_); }
  template <typename T>
  void visit_any_double(T value) { writer_.write_double(value, spec_); }
  void visit_char(int value) {
    if (spec_.type_ && spec_.type_ != 'c') {
      spec_.flags_ |= CHAR_FLAG;
      writer_.write_int(value, spec_);
      return;
    }
    if (spec_.align_ == ALIGN_NUMERIC || spec_.flags_ != 0)
      FMT_THROW(FormatError("invalid format specifier for char"));
    typedef typename fmt::BasicWriter<Char>::CharPtr CharPtr;
    Char fill = static_cast<Char>(spec_.fill());
    if (spec_.precision_ == 0) {
      std::fill_n(writer_.grow_buffer(spec_.width_), spec_.width_, fill);
      return;
    }
    CharPtr out = CharPtr();
    if (spec_.width_ > 1) {
      out = writer_.grow_buffer(spec_.width_);
      if (spec_.align_ == fmt::ALIGN_RIGHT) {
        std::fill_n(out, spec_.width_ - 1, fill);
        out += spec_.width_ - 1;
      } else if (spec_.align_ == fmt::ALIGN_CENTER) {
        out = writer_.fill_padding(out, spec_.width_, 1, fill);
      } else {
        std::fill_n(out + 1, spec_.width_ - 1, fill);
      }
    } else {
      out = writer_.grow_buffer(1);
    }
    *out = static_cast<Char>(value);
  }
  void visit_string(Arg::StringValue<char> value) {
    writer_.write_str(value, spec_);
  }
  void visit_wstring(Arg::StringValue<wchar_t> value) {
    writer_.write_str(ignore_incompatible_str<Char>(value), spec_);
  }
  void visit_pointer(const void *value) {
    if (spec_.type_ && spec_.type_ != 'p')
      fmt::internal::report_unknown_type(spec_.type_, "pointer");
    spec_.flags_ = fmt::HASH_FLAG;
    spec_.type_ = 'x';
    writer_.write_int(reinterpret_cast<uintptr_t>(value), spec_);
  }
  void visit_custom(Arg::CustomValue c) {
    c.format(&formatter_, c.value, &format_);
  }
};
template <typename Char>
void fmt::internal::FixedBuffer<Char>::grow(std::size_t) {
  FMT_THROW(std::runtime_error("buffer overflow"));
}
template <typename Char>
template <typename StrChar>
void fmt::BasicWriter<Char>::write_str(
    const Arg::StringValue<StrChar> &s, const FormatSpec &spec) {
  internal::CharTraits<Char>::convert(StrChar());
  if (spec.type_ && spec.type_ != 's')
    internal::report_unknown_type(spec.type_, "string");
  const StrChar *str_value = s.value;
  std::size_t str_size = s.size;
  if (str_size == 0) {
    if (!str_value)
      FMT_THROW(FormatError("string pointer is null"));
    if (*str_value)
      str_size = std::char_traits<StrChar>::length(str_value);
  }
  std::size_t precision = spec.precision_;
  if (spec.precision_ >= 0 && precision < str_size)
    str_size = spec.precision_;
  write_str(str_value, str_size, spec);
}
template <typename Char>
inline Arg fmt::BasicFormatter<Char>::parse_arg_index(const Char *&s) {
  const char *error = 0;
  Arg arg = *s < '0' || *s > '9' ?
        next_arg(error) : get_arg(parse_nonnegative_int(s), error);
  if (error) {
    FMT_THROW(FormatError(
                *s != '}' && *s != ':' ? "invalid format string" : error));
  }
  return arg;
}
FMT_FUNC Arg fmt::internal::FormatterBase::do_get_arg(
    unsigned arg_index, const char *&error) {
  Arg arg = args_[arg_index];
  if (arg.type == Arg::NONE)
    error = "argument index out of range";
  return arg;
}
inline Arg fmt::internal::FormatterBase::next_arg(const char *&error) {
  if (next_arg_index_ >= 0)
    return do_get_arg(next_arg_index_++, error);
  error = "cannot switch from manual to automatic argument indexing";
  return Arg();
}
inline Arg fmt::internal::FormatterBase::get_arg(
    unsigned arg_index, const char *&error) {
  if (next_arg_index_ <= 0) {
    next_arg_index_ = -1;
    return do_get_arg(arg_index, error);
  }
  error = "cannot switch from automatic to manual argument indexing";
  return Arg();
}
template <typename Char>
void fmt::internal::PrintfFormatter<Char>::parse_flags(
    FormatSpec &spec, const Char *&s) {
  for (;;) {
    switch (*s++) {
      case '-':
        spec.align_ = ALIGN_LEFT;
        break;
      case '+':
        spec.flags_ |= SIGN_FLAG | PLUS_FLAG;
        break;
      case '0':
        spec.fill_ = '0';
        break;
      case ' ':
        spec.flags_ |= SIGN_FLAG;
        break;
      case '#':
        spec.flags_ |= HASH_FLAG;
        break;
      default:
        --s;
        return;
    }
  }
}
template <typename Char>
Arg fmt::internal::PrintfFormatter<Char>::get_arg(
    const Char *s, unsigned arg_index) {
  (void)s;
  const char *error = 0;
  Arg arg = arg_index == UINT_MAX ?
    next_arg(error) : FormatterBase::get_arg(arg_index - 1, error);
  if (error)
    FMT_THROW(FormatError(!*s ? "invalid format string" : error));
  return arg;
}
template <typename Char>
unsigned fmt::internal::PrintfFormatter<Char>::parse_header(
  const Char *&s, FormatSpec &spec) {
  unsigned arg_index = UINT_MAX;
  Char c = *s;
  if (c >= '0' && c <= '9') {
    unsigned value = parse_nonnegative_int(s);
    if (*s == '$') {
      ++s;
      arg_index = value;
    } else {
      if (c == '0')
        spec.fill_ = '0';
      if (value != 0) {
        spec.width_ = value;
        return arg_index;
      }
    }
  }
  parse_flags(spec, s);
  if (*s >= '0' && *s <= '9') {
    spec.width_ = parse_nonnegative_int(s);
  } else if (*s == '*') {
    ++s;
    spec.width_ = WidthHandler(spec).visit(get_arg(s));
  }
  return arg_index;
}
template <typename Char>
void fmt::internal::PrintfFormatter<Char>::format(
    BasicWriter<Char> &writer, BasicStringRef<Char> format_str,
    const ArgList &args) {
  const Char *start = format_str.c_str();
  set_args(args);
  const Char *s = start;
  while (*s) {
    Char c = *s++;
    if (c != '%') continue;
    if (*s == c) {
      write(writer, start, s);
      start = ++s;
      continue;
    }
    write(writer, start, s - 1);
    FormatSpec spec;
    spec.align_ = ALIGN_RIGHT;
    unsigned arg_index = parse_header(s, spec);
    if (*s == '.') {
      ++s;
      if ('0' <= *s && *s <= '9') {
        spec.precision_ = parse_nonnegative_int(s);
      } else if (*s == '*') {
        ++s;
        spec.precision_ = PrecisionHandler().visit(get_arg(s));
      }
    }
    Arg arg = get_arg(s, arg_index);
    if (spec.flag(HASH_FLAG) && IsZeroInt().visit(arg))
      spec.flags_ &= ~HASH_FLAG;
    if (spec.fill_ == '0') {
      if (arg.type <= Arg::LAST_NUMERIC_TYPE)
        spec.align_ = ALIGN_NUMERIC;
      else
        spec.fill_ = ' ';
    }
    switch (*s++) {
    case 'h':
      if (*s == 'h')
        ArgConverter<signed char>(arg, *++s).visit(arg);
      else
        ArgConverter<short>(arg, *s).visit(arg);
      break;
    case 'l':
      if (*s == 'l')
        ArgConverter<fmt::LongLong>(arg, *++s).visit(arg);
      else
        ArgConverter<long>(arg, *s).visit(arg);
      break;
    case 'j':
      ArgConverter<intmax_t>(arg, *s).visit(arg);
      break;
    case 'z':
      ArgConverter<size_t>(arg, *s).visit(arg);
      break;
    case 't':
      ArgConverter<ptrdiff_t>(arg, *s).visit(arg);
      break;
    case 'L':
      break;
    default:
      --s;
      ArgConverter<int>(arg, *s).visit(arg);
    }
    if (!*s)
      FMT_THROW(FormatError("invalid format string"));
    spec.type_ = static_cast<char>(*s++);
    if (arg.type <= Arg::LAST_INTEGER_TYPE) {
      switch (spec.type_) {
      case 'i': case 'u':
        spec.type_ = 'd';
        break;
      case 'c':
        CharConverter(arg).visit(arg);
        break;
      }
    }
    start = s;
    switch (arg.type) {
    case Arg::INT:
      writer.write_int(arg.int_value, spec);
      break;
    case Arg::UINT:
      writer.write_int(arg.uint_value, spec);
      break;
    case Arg::LONG_LONG:
      writer.write_int(arg.long_long_value, spec);
      break;
    case Arg::ULONG_LONG:
      writer.write_int(arg.ulong_long_value, spec);
      break;
    case Arg::CHAR: {
      if (spec.type_ && spec.type_ != 'c')
        writer.write_int(arg.int_value, spec);
      typedef typename BasicWriter<Char>::CharPtr CharPtr;
      CharPtr out = CharPtr();
      if (spec.width_ > 1) {
        Char fill = ' ';
        out = writer.grow_buffer(spec.width_);
        if (spec.align_ != ALIGN_LEFT) {
          std::fill_n(out, spec.width_ - 1, fill);
          out += spec.width_ - 1;
        } else {
          std::fill_n(out + 1, spec.width_ - 1, fill);
        }
      } else {
        out = writer.grow_buffer(1);
      }
      *out = static_cast<Char>(arg.int_value);
      break;
    }
    case Arg::DOUBLE:
      writer.write_double(arg.double_value, spec);
      break;
    case Arg::LONG_DOUBLE:
      writer.write_double(arg.long_double_value, spec);
      break;
    case Arg::CSTRING:
      arg.string.size = 0;
      writer.write_str(arg.string, spec);
      break;
    case Arg::STRING:
      writer.write_str(arg.string, spec);
      break;
    case Arg::WSTRING:
      writer.write_str(ignore_incompatible_str<Char>(arg.wstring), spec);
      break;
    case Arg::POINTER:
      if (spec.type_ && spec.type_ != 'p')
        internal::report_unknown_type(spec.type_, "pointer");
      spec.flags_= HASH_FLAG;
      spec.type_ = 'x';
      writer.write_int(reinterpret_cast<uintptr_t>(arg.pointer), spec);
      break;
    case Arg::CUSTOM: {
      if (spec.type_)
        internal::report_unknown_type(spec.type_, "object");
      const void *str_format = "s";
      arg.custom.format(&writer, arg.custom.value, &str_format);
      break;
    }
    default:
      assert(false);
      break;
    }
  }
  write(writer, start, s);
}
template <typename Char>
const Char *fmt::BasicFormatter<Char>::format(
    const Char *&format_str, const Arg &arg) {
  const Char *s = format_str;
  FormatSpec spec;
  if (*s == ':') {
    if (arg.type == Arg::CUSTOM) {
      arg.custom.format(this, arg.custom.value, &s);
      return s;
    }
    ++s;
    if (Char c = *s) {
      const Char *p = s + 1;
      spec.align_ = ALIGN_DEFAULT;
      do {
        switch (*p) {
          case '<':
            spec.align_ = ALIGN_LEFT;
            break;
          case '>':
            spec.align_ = ALIGN_RIGHT;
            break;
          case '=':
            spec.align_ = ALIGN_NUMERIC;
            break;
          case '^':
            spec.align_ = ALIGN_CENTER;
            break;
        }
        if (spec.align_ != ALIGN_DEFAULT) {
          if (p != s) {
            if (c == '}') break;
            if (c == '{')
              FMT_THROW(FormatError("invalid fill character '{'"));
            s += 2;
            spec.fill_ = c;
          } else ++s;
          if (spec.align_ == ALIGN_NUMERIC)
            require_numeric_argument(arg, '=');
          break;
        }
      } while (--p >= s);
    }
    switch (*s) {
      case '+':
        check_sign(s, arg);
        spec.flags_ |= SIGN_FLAG | PLUS_FLAG;
        break;
      case '-':
        check_sign(s, arg);
        spec.flags_ |= MINUS_FLAG;
        break;
      case ' ':
        check_sign(s, arg);
        spec.flags_ |= SIGN_FLAG;
        break;
    }
    if (*s == '#') {
      require_numeric_argument(arg, '#');
      spec.flags_ |= HASH_FLAG;
      ++s;
    }
    if ('0' <= *s && *s <= '9') {
      if (*s == '0') {
        require_numeric_argument(arg, '0');
        spec.align_ = ALIGN_NUMERIC;
        spec.fill_ = '0';
      }
      spec.width_ = parse_nonnegative_int(s);
    }
    if (*s == '.') {
      ++s;
      spec.precision_ = 0;
      if ('0' <= *s && *s <= '9') {
        spec.precision_ = parse_nonnegative_int(s);
      } else if (*s == '{') {
        ++s;
        const Arg &precision_arg = parse_arg_index(s);
        if (*s++ != '}')
          FMT_THROW(FormatError("invalid format string"));
        ULongLong value = 0;
        switch (precision_arg.type) {
          case Arg::INT:
            if (precision_arg.int_value < 0)
              FMT_THROW(FormatError("negative precision"));
            value = precision_arg.int_value;
            break;
          case Arg::UINT:
            value = precision_arg.uint_value;
            break;
          case Arg::LONG_LONG:
            if (precision_arg.long_long_value < 0)
              FMT_THROW(FormatError("negative precision"));
            value = precision_arg.long_long_value;
            break;
          case Arg::ULONG_LONG:
            value = precision_arg.ulong_long_value;
            break;
          default:
            FMT_THROW(FormatError("precision is not integer"));
        }
        if (value > INT_MAX)
          FMT_THROW(FormatError("number is too big"));
        spec.precision_ = static_cast<int>(value);
      } else {
        FMT_THROW(FormatError("missing precision specifier"));
      }
      if (arg.type < Arg::LAST_INTEGER_TYPE || arg.type == Arg::POINTER) {
        FMT_THROW(FormatError(
            fmt::format("precision not allowed in {} format specifier",
            arg.type == Arg::POINTER ? "pointer" : "integer")));
      }
    }
    if (*s != '}' && *s)
      spec.type_ = static_cast<char>(*s++);
  }
  if (*s++ != '}')
    FMT_THROW(FormatError("missing '}' in format string"));
  start_ = s;
  internal::ArgFormatter<Char>(*this, spec, s - 1).visit(arg);
  return s;
}
template <typename Char>
void fmt::BasicFormatter<Char>::format(
    BasicStringRef<Char> format_str, const ArgList &args) {
  const Char *s = start_ = format_str.c_str();
  set_args(args);
  while (*s) {
    Char c = *s++;
    if (c != '{' && c != '}') continue;
    if (*s == c) {
      write(writer_, start_, s);
      start_ = ++s;
      continue;
    }
    if (c == '}')
      FMT_THROW(FormatError("unmatched '}' in format string"));
    write(writer_, start_, s - 1);
    Arg arg = parse_arg_index(s);
    s = format(s, arg);
  }
  write(writer_, start_, s);
}
FMT_FUNC void fmt::report_system_error(
    int error_code, fmt::StringRef message) FMT_NOEXCEPT {
  report_error(internal::format_system_error, error_code, message);
}
#ifdef _WIN32
FMT_FUNC void fmt::report_windows_error(
    int error_code, fmt::StringRef message) FMT_NOEXCEPT {
  report_error(internal::format_windows_error, error_code, message);
}
#endif
FMT_FUNC void fmt::print(std::FILE *f, StringRef format_str, ArgList args) {
  MemoryWriter w;
  w.write(format_str, args);
  std::fwrite(w.data(), 1, w.size(), f);
}
FMT_FUNC void fmt::print(StringRef format_str, ArgList args) {
  print(stdout, format_str, args);
}
FMT_FUNC void fmt::print(std::ostream &os, StringRef format_str, ArgList args) {
  MemoryWriter w;
  w.write(format_str, args);
  os.write(w.data(), w.size());
}
FMT_FUNC void fmt::print_colored(Color c, StringRef format, ArgList args) {
  char escape[] = "\x1b[30m";
  escape[3] = '0' + static_cast<char>(c);
  std::fputs(escape, stdout);
  print(format, args);
  std::fputs(RESET_COLOR, stdout);
}
FMT_FUNC int fmt::fprintf(std::FILE *f, StringRef format, ArgList args) {
  MemoryWriter w;
  printf(w, format, args);
  std::size_t size = w.size();
  return std::fwrite(w.data(), 1, size, f) < size ? -1 : static_cast<int>(size);
}
#ifndef FMT_HEADER_ONLY
template void fmt::internal::FixedBuffer<char>::grow(std::size_t);
template const char *fmt::BasicFormatter<char>::format(
    const char *&format_str, const fmt::internal::Arg &arg);
template void fmt::BasicFormatter<char>::format(
  BasicStringRef<char> format, const ArgList &args);
template void fmt::internal::PrintfFormatter<char>::format(
  BasicWriter<char> &writer, BasicStringRef<char> format, const ArgList &args);
template int fmt::internal::CharTraits<char>::format_float(
    char *buffer, std::size_t size, const char *format,
    unsigned width, int precision, double value);
template int fmt::internal::CharTraits<char>::format_float(
    char *buffer, std::size_t size, const char *format,
    unsigned width, int precision, long double value);
template void fmt::internal::FixedBuffer<wchar_t>::grow(std::size_t);
template const wchar_t *fmt::BasicFormatter<wchar_t>::format(
    const wchar_t *&format_str, const fmt::internal::Arg &arg);
template void fmt::BasicFormatter<wchar_t>::format(
    BasicStringRef<wchar_t> format, const ArgList &args);
template void fmt::internal::PrintfFormatter<wchar_t>::format(
    BasicWriter<wchar_t> &writer, BasicStringRef<wchar_t> format,
    const ArgList &args);
template int fmt::internal::CharTraits<wchar_t>::format_float(
    wchar_t *buffer, std::size_t size, const wchar_t *format,
    unsigned width, int precision, double value);
template int fmt::internal::CharTraits<wchar_t>::format_float(
    wchar_t *buffer, std::size_t size, const wchar_t *format,
    unsigned width, int precision, long double value);
#endif
#if _MSC_VER
# pragma warning(pop)
#endif
#endif
#endif
namespace spdlog
{
namespace details
{
struct log_msg
{
    log_msg() = default;
    log_msg(level::level_enum l):
        logger_name(),
        level(l),
        raw(),
        formatted() {}
    log_msg(const log_msg& other) :
        logger_name(other.logger_name),
        level(other.level),
        time(other.time),
        thread_id(other.thread_id)
    {
        if (other.raw.size())
            raw << fmt::BasicStringRef<char>(other.raw.data(), other.raw.size());
        if (other.formatted.size())
            formatted << fmt::BasicStringRef<char>(other.formatted.data(), other.formatted.size());
    }
    log_msg(log_msg&& other) :
        logger_name(std::move(other.logger_name)),
        level(other.level),
        time(std::move(other.time)),
        thread_id(other.thread_id),
        raw(std::move(other.raw)),
        formatted(std::move(other.formatted))
    {
        other.clear();
    }
    log_msg& operator=(log_msg&& other)
    {
        if (this == &other)
            return *this;
        logger_name = std::move(other.logger_name);
        level = other.level;
        time = std::move(other.time);
        thread_id = other.thread_id;
        raw = std::move(other.raw);
        formatted = std::move(other.formatted);
        other.clear();
        return *this;
    }
    void clear()
    {
        level = level::off;
        raw.clear();
        formatted.clear();
    }
    std::string logger_name;
    level::level_enum level;
    log_clock::time_point time;
    size_t thread_id;
    fmt::MemoryWriter raw;
    fmt::MemoryWriter formatted;
};
}
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_DETAILS_LOG_MSG_H_ */
namespace spdlog
{
namespace sinks
{
class sink
{
public:
    virtual ~sink() {}
    virtual void log(const details::log_msg& msg) = 0;
    virtual void flush() = 0;
};
}
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_SINKS_SINK_H_ */
/*#include "../formatter.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_FORMATTER_H_
#define _AUTOMATIC_GUARD_SPDLOG_FORMATTER_H_

/*#include "details/log_msg.h" skipped */
namespace spdlog
{
namespace details
{
class flag_formatter;
}
class formatter
{
public:
    virtual ~formatter() {}
    virtual void format(details::log_msg& msg) = 0;
};
class pattern_formatter : public formatter
{
public:
    explicit pattern_formatter(const std::string& pattern);
    pattern_formatter(const pattern_formatter&) = delete;
    pattern_formatter& operator=(const pattern_formatter&) = delete;
    void format(details::log_msg& msg) override;
private:
    const std::string _pattern;
    std::vector<std::unique_ptr<details::flag_formatter>> _formatters;
    void handle_flag(char flag);
    void compile_pattern(const std::string& pattern);
};
}
/*#include "details/pattern_formatter_impl.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_DETAILS_PATTERN_FORMATTER_IMPL_H_
#define _AUTOMATIC_GUARD_SPDLOG_DETAILS_PATTERN_FORMATTER_IMPL_H_

#include <string>
#include <chrono>
#include <memory>
#include <vector>
#include <thread>
/*#include "../formatter.h" skipped */
/*#include "./log_msg.h" skipped */
/*#include "./os.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_DETAILS_OS_H_
#define _AUTOMATIC_GUARD_SPDLOG_DETAILS_OS_H_

#include<string>
#include<cstdio>
#include<ctime>
#ifdef _WIN32
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <Windows.h>
#ifdef __MINGW32__
#include <share.h>
#endif
#elif __linux__
#include <sys/syscall.h>
#include <unistd.h>
#else
#include <thread>
#endif
/*#include "../common.h" skipped */
namespace spdlog
{
namespace details
{
namespace os
{
inline spdlog::log_clock::time_point now()
{
#if defined __linux__ && defined SPDLOG_CLOCK_COARSE
    timespec ts;
    ::clock_gettime(CLOCK_REALTIME_COARSE, &ts);
    return std::chrono::time_point<log_clock, typename log_clock::duration>(
               std::chrono::duration_cast<typename log_clock::duration>(
                   std::chrono::seconds(ts.tv_sec) + std::chrono::nanoseconds(ts.tv_nsec)));
#else
    return log_clock::now();
#endif
}
inline std::tm localtime(const std::time_t &time_tt)
{
#ifdef _WIN32
    std::tm tm;
    localtime_s(&tm, &time_tt);
#else
    std::tm tm;
    localtime_r(&time_tt, &tm);
#endif
    return tm;
}
inline std::tm localtime()
{
    std::time_t now_t = time(nullptr);
    return localtime(now_t);
}
inline std::tm gmtime(const std::time_t &time_tt)
{
#ifdef _WIN32
    std::tm tm;
    gmtime_s(&tm, &time_tt);
#else
    std::tm tm;
    gmtime_r(&time_tt, &tm);
#endif
    return tm;
}
inline std::tm gmtime()
{
    std::time_t now_t = time(nullptr);
    return gmtime(now_t);
}
inline bool operator==(const std::tm& tm1, const std::tm& tm2)
{
    return (tm1.tm_sec == tm2.tm_sec &&
            tm1.tm_min == tm2.tm_min &&
            tm1.tm_hour == tm2.tm_hour &&
            tm1.tm_mday == tm2.tm_mday &&
            tm1.tm_mon == tm2.tm_mon &&
            tm1.tm_year == tm2.tm_year &&
            tm1.tm_isdst == tm2.tm_isdst);
}
inline bool operator!=(const std::tm& tm1, const std::tm& tm2)
{
    return !(tm1 == tm2);
}
#ifdef _WIN32
inline const char* eol()
{
    return "\r\n";
}
#else
constexpr inline const char* eol()
{
    return "\n";
}
#endif
#ifdef _WIN32
inline unsigned short eol_size()
{
    return 2;
}
#else
constexpr inline unsigned short eol_size()
{
    return 1;
}
#endif
inline int fopen_s(FILE** fp, const std::string& filename, const char* mode)
{
#ifdef _WIN32
    *fp = _fsopen((filename.c_str()), mode, _SH_DENYWR);
    return *fp == nullptr;
#else
    *fp = fopen((filename.c_str()), mode);
    return *fp == nullptr;
#endif
}
inline int utc_minutes_offset(const std::tm& tm = details::os::localtime())
{
#ifdef _WIN32
    (void)tm;
    DYNAMIC_TIME_ZONE_INFORMATION tzinfo;
    auto rv = GetDynamicTimeZoneInformation(&tzinfo);
    if (!rv)
        return -1;
    return -1 * (tzinfo.Bias + tzinfo.DaylightBias);
#else
    return static_cast<int>(tm.tm_gmtoff / 60);
#endif
}
inline size_t thread_id()
{
#ifdef _WIN32
    return  static_cast<size_t>(::GetCurrentThreadId());
#elif __linux__
    return  static_cast<size_t>(syscall(SYS_gettid));
#else
    return static_cast<size_t>(std::hash<std::thread::id>()(std::this_thread::get_id()));
#endif
}
}
}
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_DETAILS_OS_H_ */
namespace spdlog
{
namespace details
{
class flag_formatter
{
public:
    virtual ~flag_formatter() {}
    virtual void format(details::log_msg& msg, const std::tm& tm_time) = 0;
};
namespace
{
class name_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm&) override
    {
        msg.formatted << msg.logger_name;
    }
};
}
class level_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm&) override
    {
        msg.formatted << level::to_str(msg.level);
    }
};
class short_level_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm&) override
    {
        msg.formatted << level::to_short_str(msg.level);
    }
};
static const char* ampm(const tm& t)
{
    return t.tm_hour >= 12 ? "PM" : "AM";
}
static int to12h(const tm& t)
{
    return t.tm_hour > 12 ? t.tm_hour - 12 : t.tm_hour;
}
static const std::string days[] { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
class a_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        msg.formatted << days[tm_time.tm_wday];
    }
};
static const std::string full_days[] { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
class A_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        msg.formatted << full_days[tm_time.tm_wday];
    }
};
static const std::string  months[] { "Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sept", "Oct", "Nov", "Dec" };
class b_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        msg.formatted<< months[tm_time.tm_mon];
    }
};
static const std::string full_months[] { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
class B_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        msg.formatted << full_months[tm_time.tm_mon];
    }
};
static fmt::MemoryWriter& pad_n_join(fmt::MemoryWriter& w, int v1, int v2, char sep)
{
    w << fmt::pad(v1, 2, '0') << sep << fmt::pad(v2, 2, '0');
    return w;
}
static fmt::MemoryWriter& pad_n_join(fmt::MemoryWriter& w, int v1, int v2, int v3, char sep)
{
    w << fmt::pad(v1, 2, '0') << sep << fmt::pad(v2, 2, '0') << sep << fmt::pad(v3, 2, '0');
    return w;
}
class c_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        msg.formatted << days[tm_time.tm_wday] << ' ' << months[tm_time.tm_mon] << ' ' << tm_time.tm_mday << ' ';
        pad_n_join(msg.formatted, tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, ':') << ' ' << tm_time.tm_year + 1900;
    }
};
class C_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        msg.formatted << fmt::pad(tm_time.tm_year % 100, 2, '0');
    }
};
class D_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        pad_n_join(msg.formatted, tm_time.tm_mon + 1, tm_time.tm_mday, tm_time.tm_year % 100, '/');
    }
};
class Y_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        msg.formatted << tm_time.tm_year + 1900;
    }
};
class m_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        msg.formatted << fmt::pad(tm_time.tm_mon + 1, 2, '0');
    }
};
class d_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        msg.formatted << fmt::pad(tm_time.tm_mday, 2, '0');
    }
};
class H_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        msg.formatted << fmt::pad(tm_time.tm_hour, 2, '0');
    }
};
class I_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        msg.formatted << fmt::pad(to12h(tm_time), 2, '0');
    }
};
class M_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        msg.formatted << fmt::pad(tm_time.tm_min, 2, '0');
    }
};
class S_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        msg.formatted << fmt::pad(tm_time.tm_sec, 2, '0');
    }
};
class e_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm&) override
    {
        auto duration = msg.time.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
        msg.formatted << fmt::pad(static_cast<int>(millis), 3, '0');
    }
};
class f_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm&) override
    {
        auto duration = msg.time.time_since_epoch();
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(duration).count() % 1000000;
        msg.formatted << fmt::pad(static_cast<int>(micros), 6, '0');
    }
};
class p_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        msg.formatted << ampm(tm_time);
    }
};
class r_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        pad_n_join(msg.formatted, to12h(tm_time), tm_time.tm_min, tm_time.tm_sec, ':') << ' ' << ampm(tm_time);
    }
};
class R_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        pad_n_join(msg.formatted, tm_time.tm_hour, tm_time.tm_min, ':');
    }
};
class T_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
        pad_n_join(msg.formatted, tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, ':');
    }
};
class z_formatter :public flag_formatter
{
public:
    const std::chrono::seconds cache_refresh = std::chrono::seconds(5);
    z_formatter() :_last_update(std::chrono::seconds(0)) {}
    z_formatter(const z_formatter&) = delete;
    z_formatter& operator=(const z_formatter&) = delete;
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
#ifdef _WIN32
        int total_minutes = get_cached_offset(msg, tm_time);
#else
        int total_minutes = os::utc_minutes_offset(tm_time);
#endif
        int h = total_minutes / 60;
        int m = total_minutes % 60;
        char sign = h >= 0 ? '+' : '-';
        msg.formatted << sign;
        pad_n_join(msg.formatted, h, m, ':');
    }
private:
    log_clock::time_point _last_update;
    int _offset_minutes;
    std::mutex _mutex;
    int get_cached_offset(const log_msg& msg, const std::tm& tm_time)
    {
        using namespace std::chrono;
        std::lock_guard<std::mutex> l(_mutex);
        if (msg.time - _last_update >= cache_refresh)
        {
            _offset_minutes = os::utc_minutes_offset(tm_time);
            _last_update = msg.time;
        }
        return _offset_minutes;
    }
};
class t_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm&) override
    {
        msg.formatted << msg.thread_id;
    }
};
class v_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm&) override
    {
        msg.formatted << fmt::StringRef(msg.raw.data(), msg.raw.size());
    }
};
class ch_formatter :public flag_formatter
{
public:
    explicit ch_formatter(char ch) : _ch(ch)
    {}
    void format(details::log_msg& msg, const std::tm&) override
    {
        msg.formatted << _ch;
    }
private:
    char _ch;
};
class aggregate_formatter :public flag_formatter
{
public:
    aggregate_formatter()
    {}
    void add_ch(char ch)
    {
        _str += ch;
    }
    void format(details::log_msg& msg, const std::tm&) override
    {
        msg.formatted << _str;
    }
private:
    std::string _str;
};
class full_formatter :public flag_formatter
{
    void format(details::log_msg& msg, const std::tm& tm_time) override
    {
#ifndef SPDLOG_NO_DATETIME
        auto duration = msg.time.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
        msg.formatted << '[' << static_cast<unsigned int>(tm_time.tm_year + 1900) << '-'
                      << fmt::pad(static_cast<unsigned int>(tm_time.tm_mon + 1), 2, '0') << '-'
                      << fmt::pad(static_cast<unsigned int>(tm_time.tm_mday), 2, '0') << ' '
                      << fmt::pad(static_cast<unsigned int>(tm_time.tm_hour), 2, '0') << ':'
                      << fmt::pad(static_cast<unsigned int>(tm_time.tm_min), 2, '0') << ':'
                      << fmt::pad(static_cast<unsigned int>(tm_time.tm_sec), 2, '0') << '.'
                      << fmt::pad(static_cast<unsigned int>(millis), 3, '0') << "] ";
#else
        (void)tm_time;
#endif
#ifndef SPDLOG_NO_NAME
        msg.formatted << '[' << msg.logger_name << "] ";
#endif
        msg.formatted << '[' << level::to_str(msg.level) << "] ";
        msg.formatted << fmt::StringRef(msg.raw.data(), msg.raw.size());
    }
};
}
}
inline spdlog::pattern_formatter::pattern_formatter(const std::string& pattern)
{
    compile_pattern(pattern);
}
inline void spdlog::pattern_formatter::compile_pattern(const std::string& pattern)
{
    auto end = pattern.end();
    std::unique_ptr<details::aggregate_formatter> user_chars;
    for (auto it = pattern.begin(); it != end; ++it)
    {
        if (*it == '%')
        {
            if (user_chars)
                _formatters.push_back(std::move(user_chars));
            if (++it != end)
                handle_flag(*it);
            else
                break;
        }
        else
        {
            if (!user_chars)
                user_chars = std::unique_ptr<details::aggregate_formatter>(new details::aggregate_formatter());
            user_chars->add_ch(*it);
        }
    }
    if (user_chars)
    {
        _formatters.push_back(std::move(user_chars));
    }
}
inline void spdlog::pattern_formatter::handle_flag(char flag)
{
    switch (flag)
    {
    case 'n':
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::name_formatter()));
        break;
    case 'l':
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::level_formatter()));
        break;
    case 'L':
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::short_level_formatter()));
        break;
    case('t') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::t_formatter()));
        break;
    case('v') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::v_formatter()));
        break;
    case('a') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::a_formatter()));
        break;
    case('A') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::A_formatter()));
        break;
    case('b') :
    case('h') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::b_formatter()));
        break;
    case('B') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::B_formatter()));
        break;
    case('c') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::c_formatter()));
        break;
    case('C') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::C_formatter()));
        break;
    case('Y') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::Y_formatter()));
        break;
    case('D') :
    case('x') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::D_formatter()));
        break;
    case('m') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::m_formatter()));
        break;
    case('d') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::d_formatter()));
        break;
    case('H') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::H_formatter()));
        break;
    case('I') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::I_formatter()));
        break;
    case('M') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::M_formatter()));
        break;
    case('S') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::S_formatter()));
        break;
    case('e') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::e_formatter()));
        break;
    case('f') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::f_formatter()));
        break;
    case('p') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::p_formatter()));
        break;
    case('r') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::r_formatter()));
        break;
    case('R') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::R_formatter()));
        break;
    case('T') :
    case('X') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::T_formatter()));
        break;
    case('z') :
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::z_formatter()));
        break;
    case ('+'):
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::full_formatter()));
        break;
    default:
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::ch_formatter('%')));
        _formatters.push_back(std::unique_ptr<details::flag_formatter>(new details::ch_formatter(flag)));
        break;
    }
}
inline void spdlog::pattern_formatter::format(details::log_msg& msg)
{
    try
    {
        auto tm_time = details::os::localtime(log_clock::to_time_t(msg.time));
        for (auto &f : _formatters)
        {
            f->format(msg, tm_time);
        }
        msg.formatted << details::os::eol();
    }
    catch(const fmt::FormatError& e)
    {
        throw spdlog_ex(fmt::format("formatting error while processing format string: {}", e.what()));
    }
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_DETAILS_PATTERN_FORMATTER_IMPL_H_ */
#endif /* END _AUTOMATIC_GUARD_SPDLOG_FORMATTER_H_ */
/*#include "../common.h" skipped */
/*#include "../details/log_msg.h" skipped */
namespace spdlog
{
namespace sinks
{
template<class Mutex>
class base_sink:public sink
{
public:
    base_sink():_mutex() {}
    virtual ~base_sink() = default;
    base_sink(const base_sink&) = delete;
    base_sink& operator=(const base_sink&) = delete;
    void log(const details::log_msg& msg) override
    {
        std::lock_guard<Mutex> lock(_mutex);
        _sink_it(msg);
    }
protected:
    virtual void _sink_it(const details::log_msg& msg) = 0;
    Mutex _mutex;
};
}
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_SINKS_BASE_SINK_H_ */
/*#include "common.h" skipped */
namespace spdlog
{
namespace details
{
class line_logger;
}
class logger
{
public:
    logger(const std::string& logger_name, sink_ptr single_sink);
    logger(const std::string& name, sinks_init_list);
    template<class It>
    logger(const std::string& name, const It& begin, const It& end);
    virtual ~logger();
    logger(const logger&) = delete;
    logger& operator=(const logger&) = delete;
    void set_level(level::level_enum);
    level::level_enum level() const;
    const std::string& name() const;
    bool should_log(level::level_enum) const;
    template <typename... Args> details::line_logger trace(const char* fmt, const Args&... args);
    template <typename... Args> details::line_logger debug(const char* fmt, const Args&... args);
    template <typename... Args> details::line_logger info(const char* fmt, const Args&... args);
    template <typename... Args> details::line_logger notice(const char* fmt, const Args&... args);
    template <typename... Args> details::line_logger warn(const char* fmt, const Args&... args);
    template <typename... Args> details::line_logger error(const char* fmt, const Args&... args);
    template <typename... Args> details::line_logger critical(const char* fmt, const Args&... args);
    template <typename... Args> details::line_logger alert(const char* fmt, const Args&... args);
    template <typename... Args> details::line_logger emerg(const char* fmt, const Args&... args);
    template <typename T> details::line_logger trace(const T&);
    template <typename T> details::line_logger debug(const T&);
    template <typename T> details::line_logger info(const T&);
    template <typename T> details::line_logger notice(const T&);
    template <typename T> details::line_logger warn(const T&);
    template <typename T> details::line_logger error(const T&);
    template <typename T> details::line_logger critical(const T&);
    template <typename T> details::line_logger alert(const T&);
    template <typename T> details::line_logger emerg(const T&);
    details::line_logger trace();
    details::line_logger debug();
    details::line_logger info();
    details::line_logger notice();
    details::line_logger warn();
    details::line_logger error();
    details::line_logger critical();
    details::line_logger alert();
    details::line_logger emerg();
    template <typename... Args>
    details::line_logger force_log(level::level_enum lvl, const char* fmt, const Args&... args);
    void set_pattern(const std::string&);
    void set_formatter(formatter_ptr);
    void flush();
protected:
    virtual void _log_msg(details::log_msg&);
    virtual void _set_pattern(const std::string&);
    virtual void _set_formatter(formatter_ptr);
    details::line_logger _log_if_enabled(level::level_enum lvl);
    template <typename... Args>
    details::line_logger _log_if_enabled(level::level_enum lvl, const char* fmt, const Args&... args);
    template<typename T>
    inline details::line_logger _log_if_enabled(level::level_enum lvl, const T& msg);
    friend details::line_logger;
    std::string _name;
    std::vector<sink_ptr> _sinks;
    formatter_ptr _formatter;
    std::atomic_int _level;
};
}
/*#include "./details/logger_impl.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_DETAILS_LOGGER_IMPL_H_
#define _AUTOMATIC_GUARD_SPDLOG_DETAILS_LOGGER_IMPL_H_

/*#include "./line_logger.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_DETAILS_LINE_LOGGER_H_
#define _AUTOMATIC_GUARD_SPDLOG_DETAILS_LINE_LOGGER_H_

#include <type_traits>
/*#include "../common.h" skipped */
/*#include "../logger.h" skipped */
namespace spdlog
{
namespace details
{
class line_logger
{
public:
    line_logger(logger* callback_logger, level::level_enum msg_level, bool enabled):
        _callback_logger(callback_logger),
        _log_msg(msg_level),
        _enabled(enabled)
    {}
    line_logger(const line_logger& other) = delete;
    line_logger& operator=(const line_logger&) = delete;
    line_logger& operator=(line_logger&&) = delete;
    line_logger(line_logger&& other) :
        _callback_logger(other._callback_logger),
        _log_msg(std::move(other._log_msg)),
        _enabled(other._enabled)
    {
        other.disable();
    }
    ~line_logger()
    {
        if (_enabled)
        {
#ifndef SPDLOG_NO_NAME
            _log_msg.logger_name = _callback_logger->name();
#endif
#ifndef SPDLOG_NO_DATETIME
            _log_msg.time = os::now();
#endif
#ifndef SPDLOG_NO_THREAD_ID
            _log_msg.thread_id = os::thread_id();
#endif
            _callback_logger->_log_msg(_log_msg);
        }
    }
    void write(const char* what)
    {
        if (_enabled)
            _log_msg.raw << what;
    }
    template <typename... Args>
    void write(const char* fmt, const Args&... args)
    {
        if (!_enabled)
            return;
        try
        {
            _log_msg.raw.write(fmt, args...);
        }
        catch (const fmt::FormatError& e)
        {
            throw spdlog_ex(fmt::format("formatting error while processing format string '{}': {}", fmt, e.what()));
        }
    }
    line_logger& operator<<(const char* what)
    {
        if (_enabled)
            _log_msg.raw << what;
        return *this;
    }
    line_logger& operator<<(const std::string& what)
    {
        if (_enabled)
            _log_msg.raw << what;
        return *this;
    }
    line_logger& operator<<(int what)
    {
        if (_enabled)
            _log_msg.raw << what;
        return *this;
    }
    line_logger& operator<<(unsigned int what)
    {
        if (_enabled)
            _log_msg.raw << what;
        return *this;
    }
    line_logger& operator<<(long what)
    {
        if (_enabled)
            _log_msg.raw << what;
        return *this;
    }
    line_logger& operator<<(unsigned long what)
    {
        if (_enabled)
            _log_msg.raw << what;
        return *this;
    }
    line_logger& operator<<(long long what)
    {
        if (_enabled)
            _log_msg.raw << what;
        return *this;
    }
    line_logger& operator<<(unsigned long long what)
    {
        if (_enabled)
            _log_msg.raw << what;
        return *this;
    }
    line_logger& operator<<(double what)
    {
        if (_enabled)
            _log_msg.raw << what;
        return *this;
    }
    line_logger& operator<<(long double what)
    {
        if (_enabled)
            _log_msg.raw << what;
        return *this;
    }
    line_logger& operator<<(float what)
    {
        if (_enabled)
            _log_msg.raw << what;
        return *this;
    }
    line_logger& operator<<(char what)
    {
        if (_enabled)
            _log_msg.raw << what;
        return *this;
    }
    template<typename T>
    line_logger& operator<<(const T& what)
    {
        if (_enabled)
            _log_msg.raw.write("{}", what);
        return *this;
    }
    void disable()
    {
        _enabled = false;
    }
    bool is_enabled() const
    {
        return _enabled;
    }
private:
    logger* _callback_logger;
    log_msg _log_msg;
    bool _enabled;
};
}
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_DETAILS_LINE_LOGGER_H_ */
template<class It>
inline spdlog::logger::logger(const std::string& logger_name, const It& begin, const It& end) :
    _name(logger_name),
    _sinks(begin, end),
    _formatter(std::make_shared<pattern_formatter>("%+"))
{
    _level = level::info;
}
inline spdlog::logger::logger(const std::string& logger_name, sinks_init_list sinks_list) :
    logger(logger_name, sinks_list.begin(), sinks_list.end()) {}
inline spdlog::logger::logger(const std::string& logger_name, spdlog::sink_ptr single_sink) :
    logger(logger_name, {
    single_sink
}) {}
inline spdlog::logger::~logger() = default;
inline void spdlog::logger::set_formatter(spdlog::formatter_ptr msg_formatter)
{
    _set_formatter(msg_formatter);
}
inline void spdlog::logger::set_pattern(const std::string& pattern)
{
    _set_pattern(pattern);
}
template <typename... Args>
inline spdlog::details::line_logger spdlog::logger::_log_if_enabled(level::level_enum lvl, const char* fmt, const Args&... args)
{
    bool msg_enabled = should_log(lvl);
    details::line_logger l(this, lvl, msg_enabled);
    l.write(fmt, args...);
    return l;
}
inline spdlog::details::line_logger spdlog::logger::_log_if_enabled(level::level_enum lvl)
{
    return details::line_logger(this, lvl, should_log(lvl));
}
template<typename T>
inline spdlog::details::line_logger spdlog::logger::_log_if_enabled(level::level_enum lvl, const T& msg)
{
    bool msg_enabled = should_log(lvl);
    details::line_logger l(this, lvl, msg_enabled);
    l << msg;
    return l;
}
template <typename... Args>
inline spdlog::details::line_logger spdlog::logger::trace(const char* fmt, const Args&... args)
{
    return _log_if_enabled(level::trace, fmt, args...);
}
template <typename... Args>
inline spdlog::details::line_logger spdlog::logger::debug(const char* fmt, const Args&... args)
{
    return _log_if_enabled(level::debug, fmt, args...);
}
template <typename... Args>
inline spdlog::details::line_logger spdlog::logger::info(const char* fmt, const Args&... args)
{
    return _log_if_enabled(level::info, fmt, args...);
}
template <typename... Args>
inline spdlog::details::line_logger spdlog::logger::notice(const char* fmt, const Args&... args)
{
    return _log_if_enabled(level::notice, fmt, args...);
}
template <typename... Args>
inline spdlog::details::line_logger spdlog::logger::warn(const char* fmt, const Args&... args)
{
    return _log_if_enabled(level::warn, fmt, args...);
}
template <typename... Args>
inline spdlog::details::line_logger spdlog::logger::error(const char* fmt, const Args&... args)
{
    return _log_if_enabled(level::err, fmt, args...);
}
template <typename... Args>
inline spdlog::details::line_logger spdlog::logger::critical(const char* fmt, const Args&... args)
{
    return _log_if_enabled(level::critical, fmt, args...);
}
template <typename... Args>
inline spdlog::details::line_logger spdlog::logger::alert(const char* fmt, const Args&... args)
{
    return _log_if_enabled(level::alert, fmt, args...);
}
template <typename... Args>
inline spdlog::details::line_logger spdlog::logger::emerg(const char* fmt, const Args&... args)
{
    return _log_if_enabled(level::emerg, fmt, args...);
}
template<typename T>
inline spdlog::details::line_logger spdlog::logger::trace(const T& msg)
{
    return _log_if_enabled(level::trace, msg);
}
template<typename T>
inline spdlog::details::line_logger spdlog::logger::debug(const T& msg)
{
    return _log_if_enabled(level::debug, msg);
}
template<typename T>
inline spdlog::details::line_logger spdlog::logger::info(const T& msg)
{
    return _log_if_enabled(level::info, msg);
}
template<typename T>
inline spdlog::details::line_logger spdlog::logger::notice(const T& msg)
{
    return _log_if_enabled(level::notice, msg);
}
template<typename T>
inline spdlog::details::line_logger spdlog::logger::warn(const T& msg)
{
    return _log_if_enabled(level::warn, msg);
}
template<typename T>
inline spdlog::details::line_logger spdlog::logger::error(const T& msg)
{
    return _log_if_enabled(level::err, msg);
}
template<typename T>
inline spdlog::details::line_logger spdlog::logger::critical(const T& msg)
{
    return _log_if_enabled(level::critical, msg);
}
template<typename T>
inline spdlog::details::line_logger spdlog::logger::alert(const T& msg)
{
    return _log_if_enabled(level::alert, msg);
}
template<typename T>
inline spdlog::details::line_logger spdlog::logger::emerg(const T& msg)
{
    return _log_if_enabled(level::emerg, msg);
}
inline spdlog::details::line_logger spdlog::logger::trace()
{
    return _log_if_enabled(level::trace);
}
inline spdlog::details::line_logger spdlog::logger::debug()
{
    return _log_if_enabled(level::debug);
}
inline spdlog::details::line_logger spdlog::logger::info()
{
    return _log_if_enabled(level::info);
}
inline spdlog::details::line_logger spdlog::logger::notice()
{
    return _log_if_enabled(level::notice);
}
inline spdlog::details::line_logger spdlog::logger::warn()
{
    return _log_if_enabled(level::warn);
}
inline spdlog::details::line_logger spdlog::logger::error()
{
    return _log_if_enabled(level::err);
}
inline spdlog::details::line_logger spdlog::logger::critical()
{
    return _log_if_enabled(level::critical);
}
inline spdlog::details::line_logger spdlog::logger::alert()
{
    return _log_if_enabled(level::alert);
}
inline spdlog::details::line_logger spdlog::logger::emerg()
{
    return _log_if_enabled(level::emerg);
}
template <typename... Args>
inline spdlog::details::line_logger spdlog::logger::force_log(level::level_enum lvl, const char* fmt, const Args&... args)
{
    details::line_logger l(this, lvl, true);
    l.write(fmt, args...);
    return l;
}
inline const std::string& spdlog::logger::name() const
{
    return _name;
}
inline void spdlog::logger::set_level(spdlog::level::level_enum log_level)
{
    _level.store(log_level);
}
inline spdlog::level::level_enum spdlog::logger::level() const
{
    return static_cast<spdlog::level::level_enum>(_level.load(std::memory_order_relaxed));
}
inline bool spdlog::logger::should_log(spdlog::level::level_enum msg_level) const
{
    return msg_level >= _level.load(std::memory_order_relaxed);
}
inline void spdlog::logger::_log_msg(details::log_msg& msg)
{
    _formatter->format(msg);
    for (auto &sink : _sinks)
        sink->log(msg);
}
inline void spdlog::logger::_set_pattern(const std::string& pattern)
{
    _formatter = std::make_shared<pattern_formatter>(pattern);
}
inline void spdlog::logger::_set_formatter(formatter_ptr msg_formatter)
{
    _formatter = msg_formatter;
}
inline void spdlog::logger::flush() {
    for (auto& sink : _sinks)
        sink->flush();
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_DETAILS_LOGGER_IMPL_H_ */
#endif /* END _AUTOMATIC_GUARD_SPDLOG_LOGGER_H_ */
namespace spdlog
{
std::shared_ptr<logger> get(const std::string& name);
void set_pattern(const std::string& format_string);
void set_formatter(formatter_ptr f);
void set_level(level::level_enum log_level);
void set_async_mode(size_t queue_size, const async_overflow_policy overflow_policy = async_overflow_policy::block_retry, const std::function<void()>& worker_warmup_cb = nullptr, const std::chrono::milliseconds& flush_interval_ms = std::chrono::milliseconds::zero());
void set_sync_mode();
std::shared_ptr<logger> rotating_logger_mt(const std::string& logger_name, const std::string& filename, size_t max_file_size, size_t max_files, bool force_flush = false);
std::shared_ptr<logger> rotating_logger_st(const std::string& logger_name, const std::string& filename, size_t max_file_size, size_t max_files, bool force_flush = false);
std::shared_ptr<logger> daily_logger_mt(const std::string& logger_name, const std::string& filename, int hour=0, int minute=0, bool force_flush = false);
std::shared_ptr<logger> daily_logger_st(const std::string& logger_name, const std::string& filename, int hour=0, int minute=0, bool force_flush = false);
std::shared_ptr<logger> stdout_logger_mt(const std::string& logger_name);
std::shared_ptr<logger> stdout_logger_st(const std::string& logger_name);
std::shared_ptr<logger> stderr_logger_mt(const std::string& logger_name);
std::shared_ptr<logger> stderr_logger_st(const std::string& logger_name);
#ifdef __linux__
std::shared_ptr<logger> syslog_logger(const std::string& logger_name, const std::string& ident = "", int syslog_option = 0);
#endif
std::shared_ptr<logger> create(const std::string& logger_name, sinks_init_list sinks);
template<class It>
std::shared_ptr<logger> create(const std::string& logger_name, const It& sinks_begin, const It& sinks_end);
template <typename Sink, typename... Args>
std::shared_ptr<spdlog::logger> create(const std::string& logger_name, const Args&...);
void register_logger(std::shared_ptr<logger> logger);
void drop(const std::string &name);
void drop_all();
#ifdef SPDLOG_TRACE_ON
#define SPDLOG_TRACE(logger, ...) logger->trace(__VA_ARGS__) << " (" << __FILE__ << " #" << __LINE__ <<")";
#else
#define SPDLOG_TRACE(logger, ...)
#endif
#ifdef SPDLOG_DEBUG_ON
#define SPDLOG_DEBUG(logger, ...) logger->debug(__VA_ARGS__)  << " (" << __FILE__ << " #" << __LINE__ <<")";
#else
#define SPDLOG_DEBUG(logger, ...)
#endif
}
/*#include "details/spdlog_impl.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_DETAILS_SPDLOG_IMPL_H_
#define _AUTOMATIC_GUARD_SPDLOG_DETAILS_SPDLOG_IMPL_H_

/*#include "registry.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_DETAILS_REGISTRY_H_
#define _AUTOMATIC_GUARD_SPDLOG_DETAILS_REGISTRY_H_

#include <string>
#include <mutex>
#include <unordered_map>
#include <functional>
/*#include "./null_mutex.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_DETAILS_NULL_MUTEX_H_
#define _AUTOMATIC_GUARD_SPDLOG_DETAILS_NULL_MUTEX_H_

namespace spdlog
{
namespace details
{
struct null_mutex
{
    void lock() {}
    void unlock() {}
    bool try_lock()
    {
        return true;
    }
};
}
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_DETAILS_NULL_MUTEX_H_ */
/*#include "../logger.h" skipped */
/*#include "../async_logger.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_ASYNC_LOGGER_H_
#define _AUTOMATIC_GUARD_SPDLOG_ASYNC_LOGGER_H_

#include <chrono>
#include <functional>
/*#include "common.h" skipped */
/*#include "logger.h" skipped */
/*#include "spdlog.h" skipped */
namespace spdlog
{
namespace details
{
class async_log_helper;
}
class async_logger :public logger
{
public:
    template<class It>
    async_logger(const std::string& name,
                 const It& begin,
                 const It& end,
                 size_t queue_size,
                 const async_overflow_policy overflow_policy =  async_overflow_policy::block_retry,
                 const std::function<void()>& worker_warmup_cb = nullptr,
                 const std::chrono::milliseconds& flush_interval_ms = std::chrono::milliseconds::zero());
    async_logger(const std::string& logger_name,
                 sinks_init_list sinks,
                 size_t queue_size,
                 const async_overflow_policy overflow_policy = async_overflow_policy::block_retry,
                 const std::function<void()>& worker_warmup_cb = nullptr,
                 const std::chrono::milliseconds& flush_interval_ms = std::chrono::milliseconds::zero());
    async_logger(const std::string& logger_name,
                 sink_ptr single_sink,
                 size_t queue_size,
                 const async_overflow_policy overflow_policy =  async_overflow_policy::block_retry,
                 const std::function<void()>& worker_warmup_cb = nullptr,
                 const std::chrono::milliseconds& flush_interval_ms = std::chrono::milliseconds::zero());
protected:
    void _log_msg(details::log_msg& msg) override;
    void _set_formatter(spdlog::formatter_ptr msg_formatter) override;
    void _set_pattern(const std::string& pattern) override;
private:
    std::unique_ptr<details::async_log_helper> _async_log_helper;
};
}
/*#include "./details/async_logger_impl.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_DETAILS_ASYNC_LOGGER_IMPL_H_
#define _AUTOMATIC_GUARD_SPDLOG_DETAILS_ASYNC_LOGGER_IMPL_H_

/*#include "./async_log_helper.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_DETAILS_ASYNC_LOG_HELPER_H_
#define _AUTOMATIC_GUARD_SPDLOG_DETAILS_ASYNC_LOG_HELPER_H_

#include <chrono>
#include <thread>
#include <atomic>
#include <functional>
/*#include "../common.h" skipped */
/*#include "../sinks/sink.h" skipped */
/*#include "./mpmc_bounded_q.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_DETAILS_MPMC_BOUNDED_Q_H_
#define _AUTOMATIC_GUARD_SPDLOG_DETAILS_MPMC_BOUNDED_Q_H_

#include <atomic>
/*#include "../common.h" skipped */
namespace spdlog
{
namespace details
{
template<typename T>
class mpmc_bounded_queue
{
public:
    using item_type = T;
    mpmc_bounded_queue(size_t buffer_size)
        : buffer_(new cell_t [buffer_size]),
          buffer_mask_(buffer_size - 1)
    {
        if(!((buffer_size >= 2) && ((buffer_size & (buffer_size - 1)) == 0)))
            throw spdlog_ex("async logger queue size must be power of two");
        for (size_t i = 0; i != buffer_size; i += 1)
            buffer_[i].sequence_.store(i, std::memory_order_relaxed);
        enqueue_pos_.store(0, std::memory_order_relaxed);
        dequeue_pos_.store(0, std::memory_order_relaxed);
    }
    ~mpmc_bounded_queue()
    {
        delete [] buffer_;
    }
    bool enqueue(T&& data)
    {
        cell_t* cell;
        size_t pos = enqueue_pos_.load(std::memory_order_relaxed);
        for (;;)
        {
            cell = &buffer_[pos & buffer_mask_];
            size_t seq = cell->sequence_.load(std::memory_order_acquire);
            intptr_t dif = (intptr_t)seq - (intptr_t)pos;
            if (dif == 0)
            {
                if (enqueue_pos_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
                    break;
            }
            else if (dif < 0)
            {
                return false;
            }
            else
            {
                pos = enqueue_pos_.load(std::memory_order_relaxed);
            }
        }
        cell->data_ = std::move(data);
        cell->sequence_.store(pos + 1, std::memory_order_release);
        return true;
    }
    bool dequeue(T& data)
    {
        cell_t* cell;
        size_t pos = dequeue_pos_.load(std::memory_order_relaxed);
        for (;;)
        {
            cell = &buffer_[pos & buffer_mask_];
            size_t seq =
                cell->sequence_.load(std::memory_order_acquire);
            intptr_t dif = (intptr_t)seq - (intptr_t)(pos + 1);
            if (dif == 0)
            {
                if (dequeue_pos_.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
                    break;
            }
            else if (dif < 0)
                return false;
            else
                pos = dequeue_pos_.load(std::memory_order_relaxed);
        }
        data = std::move(cell->data_);
        cell->sequence_.store(pos + buffer_mask_ + 1, std::memory_order_release);
        return true;
    }
private:
    struct cell_t
    {
        std::atomic<size_t>   sequence_;
        T                     data_;
    };
    static size_t const     cacheline_size = 64;
    typedef char            cacheline_pad_t [cacheline_size];
    cacheline_pad_t         pad0_;
    cell_t* const           buffer_;
    size_t const            buffer_mask_;
    cacheline_pad_t         pad1_;
    std::atomic<size_t>     enqueue_pos_;
    cacheline_pad_t         pad2_;
    std::atomic<size_t>     dequeue_pos_;
    cacheline_pad_t         pad3_;
    mpmc_bounded_queue(mpmc_bounded_queue const&);
    void operator = (mpmc_bounded_queue const&);
};
}
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_DETAILS_MPMC_BOUNDED_Q_H_ */
/*#include "./log_msg.h" skipped */
/*#include "./format.h" skipped */
/*#include "os.h" skipped */
namespace spdlog
{
namespace details
{
class async_log_helper
{
    struct async_msg
    {
        std::string logger_name;
        level::level_enum level;
        log_clock::time_point time;
        size_t thread_id;
        std::string txt;
        async_msg() = default;
        ~async_msg() = default;
async_msg(async_msg&& other) SPDLOG_NOEXCEPT:
        logger_name(std::move(other.logger_name)),
                    level(std::move(other.level)),
                    time(std::move(other.time)),
                    txt(std::move(other.txt))
        {}
        async_msg& operator=(async_msg&& other) SPDLOG_NOEXCEPT
        {
            logger_name = std::move(other.logger_name);
            level = other.level;
            time = std::move(other.time);
            thread_id = other.thread_id;
            txt = std::move(other.txt);
            return *this;
        }
        async_msg(const async_msg&) = delete;
        async_msg& operator=(async_msg& other) = delete;
        async_msg(const details::log_msg& m) :
            logger_name(m.logger_name),
            level(m.level),
            time(m.time),
            thread_id(m.thread_id),
            txt(m.raw.data(), m.raw.size())
        {}
        void fill_log_msg(log_msg &msg)
        {
            msg.clear();
            msg.logger_name = logger_name;
            msg.level = level;
            msg.time = time;
            msg.thread_id = thread_id;
            msg.raw << txt;
        }
    };
public:
    using item_type = async_msg;
    using q_type = details::mpmc_bounded_queue<item_type>;
    using clock = std::chrono::steady_clock;
    async_log_helper(formatter_ptr formatter,
                     const std::vector<sink_ptr>& sinks,
                     size_t queue_size,
                     const async_overflow_policy overflow_policy = async_overflow_policy::block_retry,
                     const std::function<void()>& worker_warmup_cb = nullptr,
                     const std::chrono::milliseconds& flush_interval_ms = std::chrono::milliseconds::zero());
    void log(const details::log_msg& msg);
    ~async_log_helper();
    void set_formatter(formatter_ptr);
private:
    formatter_ptr _formatter;
    std::vector<std::shared_ptr<sinks::sink>> _sinks;
    q_type _q;
    std::shared_ptr<spdlog_ex> _last_workerthread_ex;
    const async_overflow_policy _overflow_policy;
    const std::function<void()> _worker_warmup_cb;
    const std::chrono::milliseconds _flush_interval_ms;
    std::thread _worker_thread;
    void throw_if_bad_worker();
    void worker_loop();
    bool process_next_msg(log_clock::time_point& last_pop, log_clock::time_point& last_flush);
    void handle_flush_interval(log_clock::time_point& now, log_clock::time_point& last_flush);
    static void sleep_or_yield(const spdlog::log_clock::time_point& now, const log_clock::time_point& last_op_time);
};
}
}
inline spdlog::details::async_log_helper::async_log_helper(formatter_ptr formatter, const std::vector<sink_ptr>& sinks, size_t queue_size, const async_overflow_policy overflow_policy, const std::function<void()>& worker_warmup_cb, const std::chrono::milliseconds& flush_interval_ms):
    _formatter(formatter),
    _sinks(sinks),
    _q(queue_size),
    _overflow_policy(overflow_policy),
    _worker_warmup_cb(worker_warmup_cb),
    _flush_interval_ms(flush_interval_ms),
    _worker_thread(&async_log_helper::worker_loop, this)
{}
inline spdlog::details::async_log_helper::~async_log_helper()
{
    try
    {
        log(log_msg(level::off));
        _worker_thread.join();
    }
    catch (...)
    {}
}
inline void spdlog::details::async_log_helper::log(const details::log_msg& msg)
{
    throw_if_bad_worker();
    async_msg new_msg(msg);
    if (!_q.enqueue(std::move(new_msg)) && _overflow_policy != async_overflow_policy::discard_log_msg)
    {
        auto last_op_time = details::os::now();
        auto now = last_op_time;
        do
        {
            now = details::os::now();
            sleep_or_yield(now, last_op_time);
        }
        while (!_q.enqueue(std::move(new_msg)));
    }
}
inline void spdlog::details::async_log_helper::worker_loop()
{
    try
    {
        if (_worker_warmup_cb) _worker_warmup_cb();
        auto last_pop = details::os::now();
        auto last_flush = last_pop;
        while(process_next_msg(last_pop, last_flush));
    }
    catch (const std::exception& ex)
    {
        _last_workerthread_ex = std::make_shared<spdlog_ex>(std::string("async_logger worker thread exception: ") + ex.what());
    }
    catch (...)
    {
        _last_workerthread_ex = std::make_shared<spdlog_ex>("async_logger worker thread exception");
    }
}
inline bool spdlog::details::async_log_helper::process_next_msg(log_clock::time_point& last_pop, log_clock::time_point& last_flush)
{
    async_msg incoming_async_msg;
    log_msg incoming_log_msg;
    if (_q.dequeue(incoming_async_msg))
    {
        last_pop = details::os::now();
        if(incoming_async_msg.level == level::off)
            return false;
        incoming_async_msg.fill_log_msg(incoming_log_msg);
        _formatter->format(incoming_log_msg);
        for (auto &s : _sinks)
            s->log(incoming_log_msg);
    }
    else
    {
        auto now = details::os::now();
        handle_flush_interval(now, last_flush);
        sleep_or_yield(now, last_pop);
    }
    return true;
}
inline void spdlog::details::async_log_helper::handle_flush_interval(log_clock::time_point& now, log_clock::time_point& last_flush)
{
    if (_flush_interval_ms != std::chrono::milliseconds::zero() && now - last_flush >= _flush_interval_ms)
    {
        for (auto &s : _sinks)
            s->flush();
        now = last_flush = details::os::now();
    }
}
inline void spdlog::details::async_log_helper::set_formatter(formatter_ptr msg_formatter)
{
    _formatter = msg_formatter;
}
inline void spdlog::details::async_log_helper::sleep_or_yield(const spdlog::log_clock::time_point& now, const spdlog::log_clock::time_point& last_op_time)
{
    using std::chrono::milliseconds;
    using namespace std::this_thread;
    auto time_since_op = now - last_op_time;
    if (time_since_op <= milliseconds(1))
        return;
    if (time_since_op <= milliseconds(10))
        return yield();
    if (time_since_op <= milliseconds(100))
        return sleep_for(time_since_op / 2);
    return sleep_for(milliseconds(100));
}
inline void spdlog::details::async_log_helper::throw_if_bad_worker()
{
    if (_last_workerthread_ex)
    {
        auto ex = std::move(_last_workerthread_ex);
        throw *ex;
    }
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_DETAILS_ASYNC_LOG_HELPER_H_ */
template<class It>
inline spdlog::async_logger::async_logger(const std::string& logger_name,
        const It& begin,
        const It& end,
        size_t queue_size,
        const  async_overflow_policy overflow_policy,
        const std::function<void()>& worker_warmup_cb,
        const std::chrono::milliseconds& flush_interval_ms) :
    logger(logger_name, begin, end),
    _async_log_helper(new details::async_log_helper(_formatter, _sinks, queue_size, overflow_policy, worker_warmup_cb, flush_interval_ms))
{
}
inline spdlog::async_logger::async_logger(const std::string& logger_name,
        sinks_init_list sinks,
        size_t queue_size,
        const  async_overflow_policy overflow_policy,
        const std::function<void()>& worker_warmup_cb,
        const std::chrono::milliseconds& flush_interval_ms) :
    async_logger(logger_name, sinks.begin(), sinks.end(), queue_size, overflow_policy, worker_warmup_cb, flush_interval_ms) {}
inline spdlog::async_logger::async_logger(const std::string& logger_name,
        sink_ptr single_sink,
        size_t queue_size,
        const  async_overflow_policy overflow_policy,
        const std::function<void()>& worker_warmup_cb,
        const std::chrono::milliseconds& flush_interval_ms) :
    async_logger(logger_name, { single_sink }, queue_size, overflow_policy, worker_warmup_cb, flush_interval_ms) {}
inline void spdlog::async_logger::_set_formatter(spdlog::formatter_ptr msg_formatter)
{
    _formatter = msg_formatter;
    _async_log_helper->set_formatter(_formatter);
}
inline void spdlog::async_logger::_set_pattern(const std::string& pattern)
{
    _formatter = std::make_shared<pattern_formatter>(pattern);
    _async_log_helper->set_formatter(_formatter);
}
inline void spdlog::async_logger::_log_msg(details::log_msg& msg)
{
    _async_log_helper->log(msg);
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_DETAILS_ASYNC_LOGGER_IMPL_H_ */
#endif /* END _AUTOMATIC_GUARD_SPDLOG_ASYNC_LOGGER_H_ */
/*#include "../common.h" skipped */
namespace spdlog
{
namespace details
{
template <class Mutex> class registry_t
{
public:
    void register_logger(std::shared_ptr<logger> logger)
    {
        std::lock_guard<Mutex> lock(_mutex);
        register_logger_impl(logger);
    }
    std::shared_ptr<logger> get(const std::string& logger_name)
    {
        std::lock_guard<Mutex> lock(_mutex);
        auto found = _loggers.find(logger_name);
        return found == _loggers.end() ? nullptr : found->second;
    }
    template<class It>
    std::shared_ptr<logger> create(const std::string& logger_name, const It& sinks_begin, const It& sinks_end)
    {
        std::shared_ptr<logger> new_logger;
        std::lock_guard<Mutex> lock(_mutex);
        if (_async_mode)
            new_logger = std::make_shared<async_logger>(logger_name, sinks_begin, sinks_end, _async_q_size, _overflow_policy, _worker_warmup_cb, _flush_interval_ms);
        else
            new_logger = std::make_shared<logger>(logger_name, sinks_begin, sinks_end);
        if (_formatter)
            new_logger->set_formatter(_formatter);
        new_logger->set_level(_level);
        register_logger_impl(new_logger);
        return new_logger;
    }
    void drop(const std::string& logger_name)
    {
        std::lock_guard<Mutex> lock(_mutex);
        _loggers.erase(logger_name);
    }
    void drop_all()
    {
        std::lock_guard<Mutex> lock(_mutex);
        _loggers.clear();
    }
    std::shared_ptr<logger> create(const std::string& logger_name, sinks_init_list sinks)
    {
        return create(logger_name, sinks.begin(), sinks.end());
    }
    std::shared_ptr<logger> create(const std::string& logger_name, sink_ptr sink)
    {
        return create(logger_name, { sink });
    }
    void formatter(formatter_ptr f)
    {
        std::lock_guard<Mutex> lock(_mutex);
        _formatter = f;
        for (auto& l : _loggers)
            l.second->set_formatter(_formatter);
    }
    void set_pattern(const std::string& pattern)
    {
        std::lock_guard<Mutex> lock(_mutex);
        _formatter = std::make_shared<pattern_formatter>(pattern);
        for (auto& l : _loggers)
            l.second->set_formatter(_formatter);
    }
    void set_level(level::level_enum log_level)
    {
        std::lock_guard<Mutex> lock(_mutex);
        for (auto& l : _loggers)
            l.second->set_level(log_level);
        _level = log_level;
    }
    void set_async_mode(size_t q_size, const async_overflow_policy overflow_policy, const std::function<void()>& worker_warmup_cb, const std::chrono::milliseconds& flush_interval_ms)
    {
        std::lock_guard<Mutex> lock(_mutex);
        _async_mode = true;
        _async_q_size = q_size;
        _overflow_policy = overflow_policy;
        _worker_warmup_cb = worker_warmup_cb;
        _flush_interval_ms = flush_interval_ms;
    }
    void set_sync_mode()
    {
        std::lock_guard<Mutex> lock(_mutex);
        _async_mode = false;
    }
    static registry_t<Mutex>& instance()
    {
        static registry_t<Mutex> s_instance;
        return s_instance;
    }
private:
    void register_logger_impl(std::shared_ptr<logger> logger)
    {
        auto logger_name = logger->name();
        if (_loggers.find(logger_name) != std::end(_loggers))
            throw spdlog_ex("logger with name " + logger_name + " already exists");
        _loggers[logger->name()] = logger;
    }
    registry_t<Mutex>(){}
    registry_t<Mutex>(const registry_t<Mutex>&) = delete;
    registry_t<Mutex>& operator=(const registry_t<Mutex>&) = delete;
    Mutex _mutex;
    std::unordered_map <std::string, std::shared_ptr<logger>> _loggers;
    formatter_ptr _formatter;
    level::level_enum _level = level::info;
    bool _async_mode = false;
    size_t _async_q_size = 0;
    async_overflow_policy _overflow_policy = async_overflow_policy::block_retry;
    std::function<void()> _worker_warmup_cb = nullptr;
    std::chrono::milliseconds _flush_interval_ms;
};
#ifdef SPDLOG_NO_REGISTRY_MUTEX
typedef registry_t<spdlog::details::null_mutex> registry;
#else
typedef registry_t<std::mutex> registry;
#endif
}
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_DETAILS_REGISTRY_H_ */
/*#include "../sinks/file_sinks.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_SINKS_FILE_SINKS_H_
#define _AUTOMATIC_GUARD_SPDLOG_SINKS_FILE_SINKS_H_

#include <mutex>
/*#include "base_sink.h" skipped */
/*#include "../details/null_mutex.h" skipped */
/*#include "../details/file_helper.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_DETAILS_FILE_HELPER_H_
#define _AUTOMATIC_GUARD_SPDLOG_DETAILS_FILE_HELPER_H_

#include <string>
#include <thread>
#include <chrono>
/*#include "os.h" skipped */
namespace spdlog
{
namespace details
{
class file_helper
{
public:
    const int open_tries = 5;
    const int open_interval = 10;
    explicit file_helper(bool force_flush):
        _fd(nullptr),
        _force_flush(force_flush)
    {}
    file_helper(const file_helper&) = delete;
    file_helper& operator=(const file_helper&) = delete;
    ~file_helper()
    {
        close();
    }
    void open(const std::string& fname, bool truncate=false)
    {
        close();
        const char* mode = truncate ? "wb" : "ab";
        _filename = fname;
        for (int tries = 0; tries < open_tries; ++tries)
        {
            if(!os::fopen_s(&_fd, fname, mode))
                return;
            std::this_thread::sleep_for(std::chrono::milliseconds(open_interval));
        }
        throw spdlog_ex("Failed opening file " + fname + " for writing");
    }
    void reopen(bool truncate)
    {
        if(_filename.empty())
            throw spdlog_ex("Failed re opening file - was not opened before");
        open(_filename, truncate);
    }
    void flush() {
        std::fflush(_fd);
    }
    void close()
    {
        if (_fd)
        {
            std::fclose(_fd);
            _fd = nullptr;
        }
    }
    void write(const log_msg& msg)
    {
        size_t size = msg.formatted.size();
        auto data = msg.formatted.data();
        if(std::fwrite(data, 1, size, _fd) != size)
            throw spdlog_ex("Failed writing to file " + _filename);
        if(_force_flush)
            std::fflush(_fd);
    }
    const std::string& filename() const
    {
        return _filename;
    }
    static bool file_exists(const std::string& name)
    {
        FILE* file;
        if (!os::fopen_s(&file, name.c_str(), "r"))
        {
            fclose(file);
            return true;
        }
        else
        {
            return false;
        }
    }
private:
    FILE* _fd;
    std::string _filename;
    bool _force_flush;
};
}
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_DETAILS_FILE_HELPER_H_ */
/*#include "../details/format.h" skipped */
namespace spdlog
{
namespace sinks
{
template<class Mutex>
class simple_file_sink : public base_sink < Mutex >
{
public:
    explicit simple_file_sink(const std::string &filename,
                              bool force_flush = false) :
        _file_helper(force_flush)
    {
        _file_helper.open(filename);
    }
    void flush() override
    {
        _file_helper.flush();
    }
protected:
    void _sink_it(const details::log_msg& msg) override
    {
        _file_helper.write(msg);
    }
private:
    details::file_helper _file_helper;
};
typedef simple_file_sink<std::mutex> simple_file_sink_mt;
typedef simple_file_sink<details::null_mutex> simple_file_sink_st;
template<class Mutex>
class rotating_file_sink : public base_sink < Mutex >
{
public:
    rotating_file_sink(const std::string &base_filename, const std::string &extension,
                       std::size_t max_size, std::size_t max_files,
                       bool force_flush = false) :
        _base_filename(base_filename),
        _extension(extension),
        _max_size(max_size),
        _max_files(max_files),
        _current_size(0),
        _file_helper(force_flush)
    {
        _file_helper.open(calc_filename(_base_filename, 0, _extension));
    }
    void flush() override
    {
        _file_helper.flush();
    }
protected:
    void _sink_it(const details::log_msg& msg) override
    {
        _current_size += msg.formatted.size();
        if (_current_size > _max_size)
        {
            _rotate();
            _current_size = msg.formatted.size();
        }
        _file_helper.write(msg);
    }
private:
    static std::string calc_filename(const std::string& filename, std::size_t index, const std::string& extension)
    {
        fmt::MemoryWriter w;
        if (index)
            w.write("{}.{}.{}", filename, index, extension);
        else
            w.write("{}.{}", filename, extension);
        return w.str();
    }
    void _rotate()
    {
        _file_helper.close();
        for (auto i = _max_files; i > 0; --i)
        {
            std::string src = calc_filename(_base_filename, i - 1, _extension);
            std::string target = calc_filename(_base_filename, i, _extension);
            if (details::file_helper::file_exists(target))
            {
                if (std::remove(target.c_str()) != 0)
                {
                    throw spdlog_ex("rotating_file_sink: failed removing " + target);
                }
            }
            if (details::file_helper::file_exists(src) && std::rename(src.c_str(), target.c_str()))
            {
                throw spdlog_ex("rotating_file_sink: failed renaming " + src + " to " + target);
            }
        }
        _file_helper.reopen(true);
    }
    std::string _base_filename;
    std::string _extension;
    std::size_t _max_size;
    std::size_t _max_files;
    std::size_t _current_size;
    details::file_helper _file_helper;
};
typedef rotating_file_sink<std::mutex> rotating_file_sink_mt;
typedef rotating_file_sink<details::null_mutex>rotating_file_sink_st;
template<class Mutex>
class daily_file_sink :public base_sink < Mutex >
{
public:
    daily_file_sink(
        const std::string& base_filename,
        const std::string& extension,
        int rotation_hour,
        int rotation_minute,
        bool force_flush = false) : _base_filename(base_filename),
        _extension(extension),
        _rotation_h(rotation_hour),
        _rotation_m(rotation_minute),
        _file_helper(force_flush)
    {
        if (rotation_hour < 0 || rotation_hour > 23 || rotation_minute < 0 || rotation_minute > 59)
            throw spdlog_ex("daily_file_sink: Invalid rotation time in ctor");
        _rotation_tp = _next_rotation_tp();
        _file_helper.open(calc_filename(_base_filename, _extension));
    }
    void flush() override
    {
        _file_helper.flush();
    }
protected:
    void _sink_it(const details::log_msg& msg) override
    {
        if (std::chrono::system_clock::now() >= _rotation_tp)
        {
            _file_helper.open(calc_filename(_base_filename, _extension));
            _rotation_tp = _next_rotation_tp();
        }
        _file_helper.write(msg);
    }
private:
    std::chrono::system_clock::time_point _next_rotation_tp()
    {
        using namespace std::chrono;
        auto now = system_clock::now();
        time_t tnow = std::chrono::system_clock::to_time_t(now);
        tm date = spdlog::details::os::localtime(tnow);
        date.tm_hour = _rotation_h;
        date.tm_min = _rotation_m;
        date.tm_sec = 0;
        auto rotation_time = std::chrono::system_clock::from_time_t(std::mktime(&date));
        if (rotation_time > now)
            return rotation_time;
        else
            return system_clock::time_point(rotation_time + hours(24));
    }
    static std::string calc_filename(const std::string& basename, const std::string& extension)
    {
        std::tm tm = spdlog::details::os::localtime();
        fmt::MemoryWriter w;
        w.write("{}_{:04d}-{:02d}-{:02d}_{:02d}-{:02d}.{}", basename, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, extension);
        return w.str();
    }
    std::string _base_filename;
    std::string _extension;
    int _rotation_h;
    int _rotation_m;
    std::chrono::system_clock::time_point _rotation_tp;
    details::file_helper _file_helper;
};
typedef daily_file_sink<std::mutex> daily_file_sink_mt;
typedef daily_file_sink<details::null_mutex> daily_file_sink_st;
}
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_SINKS_FILE_SINKS_H_ */
/*#include "../sinks/stdout_sinks.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_SINKS_STDOUT_SINKS_H_
#define _AUTOMATIC_GUARD_SPDLOG_SINKS_STDOUT_SINKS_H_

#include <iostream>
#include <mutex>
/*#include "./ostream_sink.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_SINKS_OSTREAM_SINK_H_
#define _AUTOMATIC_GUARD_SPDLOG_SINKS_OSTREAM_SINK_H_

#include <ostream>
#include <mutex>
#include <memory>
/*#include "../details/null_mutex.h" skipped */
/*#include "./base_sink.h" skipped */
namespace spdlog
{
namespace sinks
{
template<class Mutex>
class ostream_sink: public base_sink<Mutex>
{
public:
    explicit ostream_sink(std::ostream& os, bool force_flush=false) :_ostream(os), _force_flush(force_flush) {}
    ostream_sink(const ostream_sink&) = delete;
    ostream_sink& operator=(const ostream_sink&) = delete;
    virtual ~ostream_sink() = default;
protected:
    void _sink_it(const details::log_msg& msg) override
    {
        _ostream.write(msg.formatted.data(), msg.formatted.size());
        if (_force_flush)
            _ostream.flush();
    }
    void flush() override
    {
        _ostream.flush();
    }
    std::ostream& _ostream;
    bool _force_flush;
};
typedef ostream_sink<std::mutex> ostream_sink_mt;
typedef ostream_sink<details::null_mutex> ostream_sink_st;
}
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_SINKS_OSTREAM_SINK_H_ */
/*#include "../details/null_mutex.h" skipped */
namespace spdlog
{
namespace sinks
{
template <class Mutex>
class stdout_sink : public ostream_sink<Mutex>
{
    using MyType = stdout_sink<Mutex>;
public:
    stdout_sink() : ostream_sink<Mutex>(std::cout, true) {}
    static std::shared_ptr<MyType> instance()
    {
        static std::shared_ptr<MyType> instance = std::make_shared<MyType>();
        return instance;
    }
};
typedef stdout_sink<details::null_mutex> stdout_sink_st;
typedef stdout_sink<std::mutex> stdout_sink_mt;
template <class Mutex>
class stderr_sink : public ostream_sink<Mutex>
{
    using MyType = stderr_sink<Mutex>;
public:
    stderr_sink() : ostream_sink<Mutex>(std::cerr, true) {}
    static std::shared_ptr<MyType> instance()
    {
        static std::shared_ptr<MyType> instance = std::make_shared<MyType>();
        return instance;
    }
};
typedef stderr_sink<std::mutex> stderr_sink_mt;
typedef stderr_sink<details::null_mutex> stderr_sink_st;
}
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_SINKS_STDOUT_SINKS_H_ */
/*#include "../sinks/syslog_sink.h"*/
#ifndef _AUTOMATIC_GUARD_SPDLOG_SINKS_SYSLOG_SINK_H_
#define _AUTOMATIC_GUARD_SPDLOG_SINKS_SYSLOG_SINK_H_

#ifdef __linux__
#include <array>
#include <string>
#include <syslog.h>
/*#include "./sink.h" skipped */
/*#include "../common.h" skipped */
/*#include "../details/log_msg.h" skipped */
namespace spdlog
{
namespace sinks
{
class syslog_sink : public sink
{
public:
    syslog_sink(const std::string& ident = "", int syslog_option=0, int syslog_facility=LOG_USER):
        _ident(ident)
    {
        _priorities[static_cast<int>(level::trace)] = LOG_DEBUG;
        _priorities[static_cast<int>(level::debug)] = LOG_DEBUG;
        _priorities[static_cast<int>(level::info)] = LOG_INFO;
        _priorities[static_cast<int>(level::notice)] = LOG_NOTICE;
        _priorities[static_cast<int>(level::warn)] = LOG_WARNING;
        _priorities[static_cast<int>(level::err)] = LOG_ERR;
        _priorities[static_cast<int>(level::critical)] = LOG_CRIT;
        _priorities[static_cast<int>(level::alert)] = LOG_ALERT;
        _priorities[static_cast<int>(level::emerg)] = LOG_EMERG;
        _priorities[static_cast<int>(level::off)] = LOG_INFO;
        ::openlog(_ident.empty()? nullptr:_ident.c_str(), syslog_option, syslog_facility);
    }
    ~syslog_sink()
    {
        ::closelog();
    }
    syslog_sink(const syslog_sink&) = delete;
    syslog_sink& operator=(const syslog_sink&) = delete;
    void log(const details::log_msg &msg) override
    {
        ::syslog(syslog_prio_from_level(msg), "%s", msg.formatted.str().c_str());
    }
    void flush() override
    {
    }
private:
    std::array<int, 10> _priorities;
    const std::string _ident;
    int syslog_prio_from_level(const details::log_msg &msg) const
    {
        return _priorities[static_cast<int>(msg.level)];
    }
};
}
}
#endif
#endif /* END _AUTOMATIC_GUARD_SPDLOG_SINKS_SYSLOG_SINK_H_ */
inline void spdlog::register_logger(std::shared_ptr<logger> logger)
{
    return details::registry::instance().register_logger(logger);
}
inline std::shared_ptr<spdlog::logger> spdlog::get(const std::string& name)
{
    return details::registry::instance().get(name);
}
inline void spdlog::drop(const std::string &name)
{
    details::registry::instance().drop(name);
}
inline std::shared_ptr<spdlog::logger> spdlog::rotating_logger_mt(const std::string& logger_name, const std::string& filename, size_t max_file_size, size_t max_files, bool force_flush)
{
    return create<spdlog::sinks::rotating_file_sink_mt>(logger_name, filename, "txt", max_file_size, max_files, force_flush);
}
inline std::shared_ptr<spdlog::logger> spdlog::rotating_logger_st(const std::string& logger_name, const std::string& filename, size_t max_file_size, size_t max_files, bool force_flush)
{
    return create<spdlog::sinks::rotating_file_sink_st>(logger_name, filename, "txt", max_file_size, max_files, force_flush);
}
inline std::shared_ptr<spdlog::logger> spdlog::daily_logger_mt(const std::string& logger_name, const std::string& filename, int hour, int minute, bool force_flush)
{
    return create<spdlog::sinks::daily_file_sink_mt>(logger_name, filename, "txt", hour, minute, force_flush);
}
inline std::shared_ptr<spdlog::logger> spdlog::daily_logger_st(const std::string& logger_name, const std::string& filename, int hour, int minute, bool force_flush)
{
    return create<spdlog::sinks::daily_file_sink_st>(logger_name, filename, "txt", hour, minute, force_flush);
}
inline std::shared_ptr<spdlog::logger> spdlog::stdout_logger_mt(const std::string& logger_name)
{
    return details::registry::instance().create(logger_name, spdlog::sinks::stdout_sink_mt::instance());
}
inline std::shared_ptr<spdlog::logger> spdlog::stdout_logger_st(const std::string& logger_name)
{
    return details::registry::instance().create(logger_name, spdlog::sinks::stdout_sink_st::instance());
}
inline std::shared_ptr<spdlog::logger> spdlog::stderr_logger_mt(const std::string& logger_name)
{
    return details::registry::instance().create(logger_name, spdlog::sinks::stderr_sink_mt::instance());
}
inline std::shared_ptr<spdlog::logger> spdlog::stderr_logger_st(const std::string& logger_name)
{
    return details::registry::instance().create(logger_name, spdlog::sinks::stderr_sink_st::instance());
}
#ifdef __linux__
inline std::shared_ptr<spdlog::logger> spdlog::syslog_logger(const std::string& logger_name, const std::string& syslog_ident, int syslog_option)
{
    return create<spdlog::sinks::syslog_sink>(logger_name, syslog_ident, syslog_option);
}
#endif
inline std::shared_ptr<spdlog::logger> spdlog::create(const std::string& logger_name, spdlog::sinks_init_list sinks)
{
    return details::registry::instance().create(logger_name, sinks);
}
template <typename Sink, typename... Args>
inline std::shared_ptr<spdlog::logger> spdlog::create(const std::string& logger_name, const Args&... args)
{
    sink_ptr sink = std::make_shared<Sink>(args...);
    return details::registry::instance().create(logger_name, { sink });
}
template<class It>
inline std::shared_ptr<spdlog::logger> spdlog::create(const std::string& logger_name, const It& sinks_begin, const It& sinks_end)
{
    return details::registry::instance().create(logger_name, sinks_begin, sinks_end);
}
inline void spdlog::set_formatter(spdlog::formatter_ptr f)
{
    details::registry::instance().formatter(f);
}
inline void spdlog::set_pattern(const std::string& format_string)
{
    return details::registry::instance().set_pattern(format_string);
}
inline void spdlog::set_level(level::level_enum log_level)
{
    return details::registry::instance().set_level(log_level);
}
inline void spdlog::set_async_mode(size_t queue_size, const async_overflow_policy overflow_policy, const std::function<void()>& worker_warmup_cb, const std::chrono::milliseconds& flush_interval_ms)
{
    details::registry::instance().set_async_mode(queue_size, overflow_policy, worker_warmup_cb, flush_interval_ms);
}
inline void spdlog::set_sync_mode()
{
    details::registry::instance().set_sync_mode();
}
inline void spdlog::drop_all()
{
    details::registry::instance().drop_all();
}
#endif /* END _AUTOMATIC_GUARD_SPDLOG_DETAILS_SPDLOG_IMPL_H_ */
#endif /* END _AUTOMATIC_GUARD_SPDLOG_SPDLOG_H_ */