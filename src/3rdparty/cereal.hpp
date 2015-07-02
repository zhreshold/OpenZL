/*#include <cereal/types/array.hpp>*/
#ifndef CEREAL_TYPES_ARRAY_HPP_
#define CEREAL_TYPES_ARRAY_HPP_
/*#include <cereal/cereal.hpp>*/
#ifndef CEREAL_CEREAL_HPP_
#define CEREAL_CEREAL_HPP_
#include <type_traits>
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <functional>
/*#include <cereal/macros.hpp>*/
#ifndef CEREAL_MACROS_HPP_
#define CEREAL_MACROS_HPP_
#ifndef CEREAL_SERIALIZE_FUNCTION_NAME
#define CEREAL_SERIALIZE_FUNCTION_NAME serialize
#endif
#ifndef CEREAL_LOAD_FUNCTION_NAME
#define CEREAL_LOAD_FUNCTION_NAME load
#endif
#ifndef CEREAL_SAVE_FUNCTION_NAME
#define CEREAL_SAVE_FUNCTION_NAME save
#endif
#ifndef CEREAL_LOAD_MINIMAL_FUNCTION_NAME
#define CEREAL_LOAD_MINIMAL_FUNCTION_NAME load_minimal
#endif
#ifndef CEREAL_SAVE_MINIMAL_FUNCTION_NAME
#define CEREAL_SAVE_MINIMAL_FUNCTION_NAME save_minimal
#endif
#endif
/*#include <cereal/details/traits.hpp>*/
#ifndef CEREAL_DETAILS_TRAITS_HPP_
#define CEREAL_DETAILS_TRAITS_HPP_
#ifndef __clang__
#if (__GNUC__ == 4 && __GNUC_MINOR__ <= 7)
#define CEREAL_OLDER_GCC
#endif
#endif
#include <type_traits>
#include <typeindex>
/*#include <cereal/macros.hpp> skipped */
/*#include <cereal/access.hpp>*/
#ifndef CEREAL_ACCESS_HPP_
#define CEREAL_ACCESS_HPP_
#include <type_traits>
#include <iostream>
#include <cstdint>
/*#include <cereal/macros.hpp> skipped */
/*#include <cereal/details/helpers.hpp>*/
#ifndef CEREAL_DETAILS_HELPERS_HPP_
#define CEREAL_DETAILS_HELPERS_HPP_
#include <type_traits>
#include <cstdint>
#include <utility>
#include <memory>
#include <unordered_map>
#include <stdexcept>
/*#include <cereal/macros.hpp> skipped */
/*#include <cereal/details/static_object.hpp>*/
#ifndef CEREAL_DETAILS_STATIC_OBJECT_HPP_
#define CEREAL_DETAILS_STATIC_OBJECT_HPP_
#ifdef _MSC_VER
#   define CEREAL_DLL_EXPORT __declspec(dllexport)
#   define CEREAL_USED
#else
#   define CEREAL_DLL_EXPORT
#   define CEREAL_USED __attribute__ ((__used__))
#endif
namespace cereal
{
  namespace detail
  {
    template <class T>
    class CEREAL_DLL_EXPORT StaticObject
    {
      private:
        static void instantiate( T const & ) {}
        static T & create()
        {
          static T t;
          instantiate(instance);
          return t;
        }
        StaticObject( StaticObject const &  ) {}
      public:
        static T & getInstance()
        {
          return create();
        }
      private:
        static T & instance;
    };
    template <class T> T & StaticObject<T>::instance = StaticObject<T>::create();
  }
}
#endif
namespace cereal
{
  struct Exception : public std::runtime_error
  {
    explicit Exception( const std::string & what_ ) : std::runtime_error(what_) {}
    explicit Exception( const char * what_ ) : std::runtime_error(what_) {}
  };
  using size_type = uint64_t;
  class BinaryOutputArchive;
  class BinaryInputArchive;
  namespace detail
  {
    struct NameValuePairCore {};
  }
  template <class T>
  class NameValuePair : detail::NameValuePairCore
  {
    private:
      using Type = typename std::conditional<std::is_array<typename std::remove_reference<T>::type>::value,
                                             typename std::remove_cv<T>::type,
                                             typename std::conditional<std::is_lvalue_reference<T>::value,
                                                                       T,
                                                                       typename std::decay<T>::type>::type>::type;
      static_assert( !std::is_base_of<detail::NameValuePairCore, T>::value,
                     "Cannot pair a name to a NameValuePair" );
      NameValuePair & operator=( NameValuePair const & ) = delete;
    public:
      NameValuePair( char const * n, T && v ) : name(n), value(std::forward<T>(v)) {}
      char const * name;
      Type value;
  };
  template<class Archive, class T> inline
  typename
  std::enable_if<std::is_same<Archive, ::cereal::BinaryInputArchive>::value ||
                 std::is_same<Archive, ::cereal::BinaryOutputArchive>::value,
  T && >::type
  make_nvp( const char *, T && value )
  {
    return std::forward<T>(value);
  }
  template<class Archive, class T> inline
  typename
  std::enable_if<!std::is_same<Archive, ::cereal::BinaryInputArchive>::value &&
                 !std::is_same<Archive, ::cereal::BinaryOutputArchive>::value,
  NameValuePair<T> >::type
  make_nvp( const char * name, T && value)
  {
    return {name, std::forward<T>(value)};
  }
  #define CEREAL_NVP_(name, value) ::cereal::make_nvp<Archive>(name, value)
  template <class T>
  struct BinaryData
  {
    using PT = typename std::conditional<std::is_const<typename std::remove_pointer<T>::type>::value,
                                         const void *,
                                         void *>::type;
    BinaryData( T && d, uint64_t s ) : data(std::forward<T>(d)), size(s) {}
    PT data;
    uint64_t size;
  };
  namespace detail
  {
    class OutputArchiveBase { private: virtual void rtti(){} };
    class InputArchiveBase { private: virtual void rtti(){} };
    template <class Archive, class T> struct polymorphic_serialization_support;
    struct adl_tag;
    static const int32_t msb_32bit  = 0x80000000;
    static const int32_t msb2_32bit = 0x40000000;
  }
  template <class T>
  class SizeTag
  {
    private:
      using Type = typename std::conditional<std::is_lvalue_reference<T>::value,
                                             T,
                                             typename std::decay<T>::type>::type;
      SizeTag & operator=( SizeTag const & ) = delete;
    public:
      SizeTag( T && sz ) : size(std::forward<T>(sz)) {}
      Type size;
  };
  template <class Key, class Value>
  struct MapItem
  {
    using KeyType = typename std::conditional<
      std::is_lvalue_reference<Key>::value,
      Key,
      typename std::decay<Key>::type>::type;
    using ValueType = typename std::conditional<
      std::is_lvalue_reference<Value>::value,
      Value,
      typename std::decay<Value>::type>::type;
    MapItem( Key && key_, Value && value_ ) : key(std::forward<Key>(key_)), value(std::forward<Value>(value_)) {}
    MapItem & operator=( MapItem const & ) = delete;
    KeyType key;
    ValueType value;
    template <class Archive> inline
    void CEREAL_SERIALIZE_FUNCTION_NAME(Archive & archive)
    {
      archive( make_nvp<Archive>("key",   key),
               make_nvp<Archive>("value", value) );
    }
  };
  template <class KeyType, class ValueType> inline
  MapItem<KeyType, ValueType> make_map_item(KeyType && key, ValueType && value)
  {
    return {std::forward<KeyType>(key), std::forward<ValueType>(value)};
  }
  namespace detail
  {
    namespace{ struct version_binding_tag {}; }
    template <class T, class BindingTag = version_binding_tag> struct Version
    {
      static const std::uint32_t version = 0;
    };
    struct Versions
    {
      std::unordered_map<std::size_t, std::uint32_t> mapping;
      std::uint32_t find( std::size_t hash, std::uint32_t version )
      {
        const auto result = mapping.emplace( hash, version );
        return result.first->second;
      }
    };
  }
}
#endif
namespace cereal
{
  template <class T>
  struct LoadAndConstruct
  {
    static std::false_type load_and_construct(...)
    { return std::false_type(); }
  };
  namespace memory_detail{ template <class Ar, class T> struct LoadAndConstructLoadWrapper; }
  template <class T>
  class construct
  {
    public:
      template <class ... Args>
      void operator()( Args && ... args );
      T * operator->()
      {
        if( !itsValid )
          throw Exception("Object must be initialized prior to accessing members");
        return itsPtr;
      }
      T * ptr()
      {
        return operator->();
      }
    private:
      template <class A, class B> friend struct ::cereal::memory_detail::LoadAndConstructLoadWrapper;
      construct( T * p ) : itsPtr( p ), itsValid( false ) {}
      construct( construct const & ) = delete;
      construct & operator=( construct const & ) = delete;
      T * itsPtr;
      bool itsValid;
  };
  class access
  {
    public:
      template<class Archive, class T> inline
      static auto member_serialize(Archive & ar, T & t) -> decltype(t.CEREAL_SERIALIZE_FUNCTION_NAME(ar))
      { return t.CEREAL_SERIALIZE_FUNCTION_NAME(ar); }
      template<class Archive, class T> inline
      static auto member_save(Archive & ar, T const & t) -> decltype(t.CEREAL_SAVE_FUNCTION_NAME(ar))
      { return t.CEREAL_SAVE_FUNCTION_NAME(ar); }
      template<class Archive, class T> inline
      static auto member_save_non_const(Archive & ar, T & t) -> decltype(t.CEREAL_SAVE_FUNCTION_NAME(ar))
      { return t.CEREAL_SAVE_FUNCTION_NAME(ar); }
      template<class Archive, class T> inline
      static auto member_load(Archive & ar, T & t) -> decltype(t.CEREAL_LOAD_FUNCTION_NAME(ar))
      { return t.CEREAL_LOAD_FUNCTION_NAME(ar); }
      template<class Archive, class T> inline
      static auto member_save_minimal(Archive const & ar, T const & t) -> decltype(t.CEREAL_SAVE_MINIMAL_FUNCTION_NAME(ar))
      { return t.CEREAL_SAVE_MINIMAL_FUNCTION_NAME(ar); }
      template<class Archive, class T> inline
      static auto member_save_minimal_non_const(Archive const & ar, T & t) -> decltype(t.CEREAL_SAVE_MINIMAL_FUNCTION_NAME(ar))
      { return t.CEREAL_SAVE_MINIMAL_FUNCTION_NAME(ar); }
      template<class Archive, class T, class U> inline
      static auto member_load_minimal(Archive const & ar, T & t, U && u) -> decltype(t.CEREAL_LOAD_MINIMAL_FUNCTION_NAME(ar, std::forward<U>(u)))
      { return t.CEREAL_LOAD_MINIMAL_FUNCTION_NAME(ar, std::forward<U>(u)); }
      template<class Archive, class T> inline
      static auto member_serialize(Archive & ar, T & t, const std::uint32_t version ) -> decltype(t.CEREAL_SERIALIZE_FUNCTION_NAME(ar, version))
      { return t.CEREAL_SERIALIZE_FUNCTION_NAME(ar, version); }
      template<class Archive, class T> inline
      static auto member_save(Archive & ar, T const & t, const std::uint32_t version ) -> decltype(t.CEREAL_SAVE_FUNCTION_NAME(ar, version))
      { return t.CEREAL_SAVE_FUNCTION_NAME(ar, version); }
      template<class Archive, class T> inline
      static auto member_save_non_const(Archive & ar, T & t, const std::uint32_t version ) -> decltype(t.CEREAL_SAVE_FUNCTION_NAME(ar, version))
      { return t.CEREAL_SAVE_FUNCTION_NAME(ar, version); }
      template<class Archive, class T> inline
      static auto member_load(Archive & ar, T & t, const std::uint32_t version ) -> decltype(t.CEREAL_LOAD_FUNCTION_NAME(ar, version))
      { return t.CEREAL_LOAD_FUNCTION_NAME(ar, version); }
      template<class Archive, class T> inline
      static auto member_save_minimal(Archive const & ar, T const & t, const std::uint32_t version) -> decltype(t.CEREAL_SAVE_MINIMAL_FUNCTION_NAME(ar, version))
      { return t.CEREAL_SAVE_MINIMAL_FUNCTION_NAME(ar, version); }
      template<class Archive, class T> inline
      static auto member_save_minimal_non_const(Archive const & ar, T & t, const std::uint32_t version) -> decltype(t.CEREAL_SAVE_MINIMAL_FUNCTION_NAME(ar, version))
      { return t.CEREAL_SAVE_MINIMAL_FUNCTION_NAME(ar, version); }
      template<class Archive, class T, class U> inline
      static auto member_load_minimal(Archive const & ar, T & t, U && u, const std::uint32_t version) -> decltype(t.CEREAL_LOAD_MINIMAL_FUNCTION_NAME(ar, std::forward<U>(u), version))
      { return t.CEREAL_LOAD_MINIMAL_FUNCTION_NAME(ar, std::forward<U>(u), version); }
      template <class T> inline
      static auto shared_from_this(T & t) -> decltype(t.shared_from_this());
      template <class T, class ... Args> inline
      static void construct( T *& ptr, Args && ... args )
      {
        new (ptr) T( std::forward<Args>( args )... );
      }
      template <class T> inline
      static T * construct()
      {
        return new T();
      }
      template <class T> inline
      static std::false_type load_and_construct(...)
      { return std::false_type(); }
      template<class T, class Archive> inline
      static auto load_and_construct(Archive & ar, ::cereal::construct<T> & construct) -> decltype(T::load_and_construct(ar, construct))
      {
        T::load_and_construct( ar, construct );
      }
  };
  enum class specialization
  {
    member_serialize,
    member_load_save,
    member_load_save_minimal,
    non_member_serialize,
    non_member_load_save,
    non_member_load_save_minimal
  };
  template <class Archive, class T, specialization S>
  struct specialize : public std::false_type {};
  #define CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES( Type, Specialization )                                \
  namespace cereal { template <class Archive> struct specialize<Archive, Type, Specialization> {}; }
  #define CEREAL_SPECIALIZE_FOR_ARCHIVE( Archive, Type, Specialization )               \
  namespace cereal { template <> struct specialize<Archive, Type, Specialization> {}; }
  template <class T> template <class ... Args> inline
  void construct<T>::operator()( Args && ... args )
  {
    if( itsValid )
      throw Exception("Attempting to construct an already initialized object");
    ::cereal::access::construct( itsPtr, std::forward<Args>( args )... );
    itsValid = true;
  }
}
#endif
namespace cereal
{
  namespace traits
  {
    using yes = std::true_type;
    using no  = std::false_type;
    namespace detail
    {
      template <class T>
      struct delay_static_assert : std::false_type {};
      #ifdef CEREAL_OLDER_GCC
      template<typename> struct Void { typedef void type; };
      #endif
      enum class sfinae {};
      template <bool H, bool ... T> struct meta_bool_and : std::integral_constant<bool, H && meta_bool_and<T...>::value> {};
      template <bool B> struct meta_bool_and<B> : std::integral_constant<bool, B> {};
      template <bool H, bool ... T> struct meta_bool_or : std::integral_constant<bool, H || meta_bool_or<T...>::value> {};
      template <bool B> struct meta_bool_or<B> : std::integral_constant<bool, B> {};
      template <bool ... Conditions>
      struct EnableIfHelper : std::enable_if<meta_bool_and<Conditions...>::value, sfinae> {};
      template <bool ... Conditions>
      struct DisableIfHelper : std::enable_if<!meta_bool_or<Conditions...>::value, sfinae> {};
    }
    static const detail::sfinae sfinae = {};
    template <bool ... Conditions>
    using EnableIf = typename detail::EnableIfHelper<Conditions...>::type;
    template <bool ... Conditions>
    using DisableIf = typename detail::DisableIfHelper<Conditions...>::type;
    namespace detail
    {
      template <class InputArchive>
      struct get_output_from_input : no
      {
        static_assert( detail::delay_static_assert<InputArchive>::value,
            "Could not find an associated output archive for input archive." );
      };
      template <class OutputArchive>
      struct get_input_from_output : no
      {
        static_assert( detail::delay_static_assert<OutputArchive>::value,
            "Could not find an associated input archive for output archive." );
      };
    }
    #define CEREAL_SETUP_ARCHIVE_TRAITS(InputArchive, OutputArchive)  \
    namespace cereal { namespace traits { namespace detail {          \
      template <> struct get_output_from_input<InputArchive>          \
      { using type = OutputArchive; };                                \
      template <> struct get_input_from_output<OutputArchive>         \
      { using type = InputArchive; }; } } }
    #define CEREAL_MAKE_VERSIONED_TEST ,0
    #ifdef CEREAL_OLDER_GCC
    #define CEREAL_MAKE_HAS_MEMBER_TEST(name, test_name, versioned)                                                                         \
    template <class T, class A, class SFINAE = void>                                                                                        \
    struct has_member_##test_name : no {};                                                                                                  \
    template <class T, class A>                                                                                                             \
    struct has_member_##test_name<T, A,                                                                                                     \
      typename detail::Void< decltype( cereal::access::member_##name( std::declval<A&>(), std::declval<T&>() versioned ) ) >::type> : yes {}
    #else
    #define CEREAL_MAKE_HAS_MEMBER_TEST(name, test_name, versioned)                                                                     \
    namespace detail                                                                                                                    \
    {                                                                                                                                   \
      template <class T, class A>                                                                                                       \
      struct has_member_##name##_##versioned##_impl                                                                                     \
      {                                                                                                                                 \
        template <class TT, class AA>                                                                                                   \
        static auto test(int) -> decltype( cereal::access::member_##name( std::declval<AA&>(), std::declval<TT&>() versioned ), yes()); \
        template <class, class>                                                                                                         \
        static no test(...);                                                                                                            \
        static const bool value = std::is_same<decltype(test<T, A>(0)), yes>::value;                                                    \
      };                                                                                                                                \
    }                                                                                                         \
    template <class T, class A>                                                                                                         \
    struct has_member_##test_name : std::integral_constant<bool, detail::has_member_##name##_##versioned##_impl<T, A>::value> {}
    #endif
    #define CEREAL_MAKE_HAS_NON_MEMBER_TEST(test_name, func, versioned)                                                         \
    namespace detail                                                                                                            \
    {                                                                                                                           \
      template <class T, class A>                                                                                               \
      struct has_non_member_##test_name##_impl                                                                                  \
      {                                                                                                                         \
        template <class TT, class AA>                                                                                           \
        static auto test(int) -> decltype( func( std::declval<AA&>(), std::declval<TT&>() versioned ), yes());                  \
        template <class, class>                                                                                                 \
        static no test( ... );                                                                                                  \
        static const bool value = std::is_same<decltype( test<T, A>( 0 ) ), yes>::value;                                        \
      };                                                                                                                        \
    }                                                                                                 \
    template <class T, class A>                                                                                                 \
    struct has_non_member_##test_name : std::integral_constant<bool, detail::has_non_member_##test_name##_impl<T, A>::value> {}
    CEREAL_MAKE_HAS_MEMBER_TEST(serialize, serialize,);
    CEREAL_MAKE_HAS_MEMBER_TEST(serialize, versioned_serialize, CEREAL_MAKE_VERSIONED_TEST);
    CEREAL_MAKE_HAS_NON_MEMBER_TEST(serialize, CEREAL_SERIALIZE_FUNCTION_NAME,);
    CEREAL_MAKE_HAS_NON_MEMBER_TEST(versioned_serialize, CEREAL_SERIALIZE_FUNCTION_NAME, CEREAL_MAKE_VERSIONED_TEST);
    CEREAL_MAKE_HAS_MEMBER_TEST(load, load,);
    CEREAL_MAKE_HAS_MEMBER_TEST(load, versioned_load, CEREAL_MAKE_VERSIONED_TEST);
    CEREAL_MAKE_HAS_NON_MEMBER_TEST(load, CEREAL_LOAD_FUNCTION_NAME,);
    CEREAL_MAKE_HAS_NON_MEMBER_TEST(versioned_load, CEREAL_LOAD_FUNCTION_NAME, CEREAL_MAKE_VERSIONED_TEST);
    #undef CEREAL_MAKE_HAS_NON_MEMBER_TEST
    #undef CEREAL_MAKE_HAS_MEMBER_TEST
    #ifdef CEREAL_OLDER_GCC
    #define CEREAL_MAKE_HAS_MEMBER_SAVE_IMPL(test_name, versioned)                                                                  \
    namespace detail                                                                                                                \
    {                                                                                                                               \
    template <class T, class A>                                                                                                     \
    struct has_member_##test_name##_impl                                                                                            \
      {                                                                                                                             \
        template <class TT, class AA, class SFINAE = void> struct test : no {};                                                     \
        template <class TT, class AA>                                                                                               \
        struct test<TT, AA,                                                                                                         \
          typename detail::Void< decltype( cereal::access::member_save( std::declval<AA&>(),                                        \
                                                                        std::declval<TT const &>() versioned ) ) >::type> : yes {}; \
        static const bool value = test<T, A>();                                                                                     \
                                                                                                                                    \
        template <class TT, class AA, class SFINAE = void> struct test2 : no {};                                                    \
        template <class TT, class AA>                                                                                               \
        struct test2<TT, AA,                                                                                                        \
          typename detail::Void< decltype( cereal::access::member_save_non_const(                                                   \
                                            std::declval<AA&>(),                                                                    \
                                            std::declval<typename std::remove_const<TT>::type&>() versioned ) ) >::type> : yes {};  \
        static const bool not_const_type = test2<T, A>();                                                                           \
      };                                                                                                                            \
    }
    #else
    #define CEREAL_MAKE_HAS_MEMBER_SAVE_IMPL(test_name, versioned)                                                                  \
    namespace detail                                                                                                                \
    {                                                                                                                               \
    template <class T, class A>                                                                                                     \
    struct has_member_##test_name##_impl                                                                                            \
      {                                                                                                                             \
        template <class TT, class AA>                                                                                               \
        static auto test(int) -> decltype( cereal::access::member_save( std::declval<AA&>(),                                        \
                                                                        std::declval<TT const &>() versioned ), yes());             \
        template <class, class> static no test(...);                                                                                \
        static const bool value = std::is_same<decltype(test<T, A>(0)), yes>::value;                                                \
                                                                                                                                    \
        template <class TT, class AA>                                                                                               \
        static auto test2(int) -> decltype( cereal::access::member_save_non_const(                                                  \
                                              std::declval<AA &>(),                                                                 \
                                              std::declval<typename std::remove_const<TT>::type&>() versioned ), yes());            \
        template <class, class> static no test2(...);                                                                               \
        static const bool not_const_type = std::is_same<decltype(test2<T, A>(0)), yes>::value;                                      \
      };                                                                                                                            \
    }
    #endif
    CEREAL_MAKE_HAS_MEMBER_SAVE_IMPL(save, )
    template <class T, class A>
    struct has_member_save : std::integral_constant<bool, detail::has_member_save_impl<T, A>::value>
    {
      typedef typename detail::has_member_save_impl<T, A> check;
      static_assert( check::value || !check::not_const_type,
        "cereal detected a non-const save. \n "
        "save member functions must always be const" );
    };
    CEREAL_MAKE_HAS_MEMBER_SAVE_IMPL(versioned_save, CEREAL_MAKE_VERSIONED_TEST)
    template <class T, class A>
    struct has_member_versioned_save : std::integral_constant<bool, detail::has_member_versioned_save_impl<T, A>::value>
    {
      typedef typename detail::has_member_versioned_save_impl<T, A> check;
      static_assert( check::value || !check::not_const_type,
        "cereal detected a versioned non-const save. \n "
        "save member functions must always be const" );
    };
    #undef CEREAL_MAKE_HAS_MEMBER_SAVE_IMPL
    #define CEREAL_MAKE_HAS_NON_MEMBER_SAVE_TEST(test_name, versioned)                                                       \
    namespace detail                                                                                                         \
    {                                                                                                                        \
      template <class T, class A>                                                                                            \
      struct has_non_member_##test_name##_impl                                                                               \
      {                                                                                                                      \
        template <class TT, class AA>                                                                                        \
        static auto test(int) -> decltype( CEREAL_SAVE_FUNCTION_NAME(                                                        \
                                              std::declval<AA&>(),                                                           \
                                              std::declval<TT const &>() versioned ), yes());                                \
        template <class, class> static no test(...);                                                                         \
        static const bool value = std::is_same<decltype(test<T, A>(0)), yes>::value;                                         \
                                                                                                                             \
        template <class TT, class AA>                                                                                        \
        static auto test2(int) -> decltype( CEREAL_SAVE_FUNCTION_NAME(                                                       \
                                              std::declval<AA &>(),                                                          \
                                              std::declval<typename std::remove_const<TT>::type&>() versioned ), yes());     \
        template <class, class> static no test2(...);                                                                        \
        static const bool not_const_type = std::is_same<decltype(test2<T, A>(0)), yes>::value;                               \
      };                                                                                                                     \
    }                                                                                              \
                                                                                                                             \
    template <class T, class A>                                                                                              \
    struct has_non_member_##test_name : std::integral_constant<bool, detail::has_non_member_##test_name##_impl<T, A>::value> \
    {                                                                                                                        \
      using check = typename detail::has_non_member_##test_name##_impl<T, A>;                                                \
      static_assert( check::value || !check::not_const_type,                                                                 \
        "cereal detected a non-const type parameter in non-member " #test_name ". \n "                                       \
        #test_name " non-member functions must always pass their types as const" );                                          \
    };
    CEREAL_MAKE_HAS_NON_MEMBER_SAVE_TEST(save, )
    CEREAL_MAKE_HAS_NON_MEMBER_SAVE_TEST(versioned_save, CEREAL_MAKE_VERSIONED_TEST)
    #undef CEREAL_MAKE_HAS_NON_MEMBER_SAVE_TEST
    namespace detail
    {
      template <class> struct is_string : std::false_type {};
      template <class CharT, class Traits, class Alloc>
      struct is_string<std::basic_string<CharT, Traits, Alloc>> : std::true_type {};
    }
    template <class T>
    struct is_minimal_type : std::integral_constant<bool,
      detail::is_string<T>::value || std::is_arithmetic<T>::value> {};
    #ifdef CEREAL_OLDER_GCC
    #define CEREAL_MAKE_HAS_MEMBER_SAVE_MINIMAL_IMPL(test_name, versioned)                                                                        \
    namespace detail                                                                                                                              \
    {                                                                                                                                             \
      template <class T, class A>                                                                                                                 \
      struct has_member_##test_name##_impl                                                                                                        \
      {                                                                                                                                           \
        template <class TT, class AA, class SFINAE = void> struct test : no {};                                                                   \
        template <class TT, class AA>                                                                                                             \
        struct test<TT, AA, typename detail::Void< decltype(                                                                                      \
            cereal::access::member_save_minimal( std::declval<AA const &>(),                                                                      \
                                                 std::declval<TT const &>() versioned ) ) >::type> : yes {};                                      \
                                                                                                                                                  \
        static const bool value = test<T, A>();                                                                                                   \
                                                                                                                                                  \
        template <class TT, class AA, class SFINAE = void> struct test2 : no {};                                                                  \
        template <class TT, class AA>                                                                                                             \
        struct test2<TT, AA, typename detail::Void< decltype(                                                                                     \
            cereal::access::member_save_minimal_non_const( std::declval<AA const &>(),                                                            \
                                                           std::declval<typename std::remove_const<TT>::type&>() versioned ) ) >::type> : yes {}; \
        static const bool not_const_type = test2<T, A>();                                                                                         \
                                                                                                                                                  \
        static const bool valid = value || !not_const_type;                                                                                       \
      };                                                                                                                                          \
    }
    #else
    #define CEREAL_MAKE_HAS_MEMBER_SAVE_MINIMAL_IMPL(test_name, versioned)                     \
    namespace detail                                                                           \
    {                                                                                          \
      template <class T, class A>                                                              \
      struct has_member_##test_name##_impl                                                     \
      {                                                                                        \
        template <class TT, class AA>                                                          \
        static auto test(int) -> decltype( cereal::access::member_save_minimal(                \
              std::declval<AA const &>(),                                                      \
              std::declval<TT const &>() versioned ), yes());                                  \
        template <class, class> static no test(...);                                           \
        static const bool value = std::is_same<decltype(test<T, A>(0)), yes>::value;           \
                                                                                               \
        template <class TT, class AA>                                                          \
        static auto test2(int) -> decltype( cereal::access::member_save_minimal_non_const(     \
              std::declval<AA const &>(),                                                      \
              std::declval<typename std::remove_const<TT>::type&>() versioned ), yes());       \
        template <class, class> static no test2(...);                                          \
        static const bool not_const_type = std::is_same<decltype(test2<T, A>(0)), yes>::value; \
                                                                                               \
        static const bool valid = value || !not_const_type;                                    \
      };                                                                                       \
    }
    #endif
    #define CEREAL_MAKE_HAS_MEMBER_SAVE_MINIMAL_HELPERS_IMPL(test_name, versioned)                           \
    namespace detail                                                                                         \
    {                                                                                                        \
      template <class T, class A, bool Valid>                                                                \
      struct get_member_##test_name##_type { using type = void; };                                           \
                                                                                                             \
      template <class T, class A>                                                                            \
      struct get_member_##test_name##_type<T, A, true>                                                       \
      {                                                                                                      \
        using type = decltype( cereal::access::member_save_minimal( std::declval<A const &>(),               \
                                                                    std::declval<T const &>() versioned ) ); \
      };                                                                                                     \
    }
    #define CEREAL_MAKE_HAS_MEMBER_SAVE_MINIMAL_TEST(test_name)                                                      \
    template <class T, class A>                                                                                      \
    struct has_member_##test_name : std::integral_constant<bool, detail::has_member_##test_name##_impl<T, A>::value> \
    {                                                                                                                \
      using check = typename detail::has_member_##test_name##_impl<T, A>;                                            \
      static_assert( check::valid,                                                                                   \
        "cereal detected a non-const member " #test_name ". \n "                                                     \
        #test_name " member functions must always be const" );                                                       \
                                                                                                                     \
      using type = typename detail::get_member_##test_name##_type<T, A, check::value>::type;                         \
      static_assert( (check::value && is_minimal_type<type>::value) || !check::value,                                \
        "cereal detected a member " #test_name " with an invalid return type. \n "                                   \
        "return type must be arithmetic or string" );                                                                \
    };
    CEREAL_MAKE_HAS_MEMBER_SAVE_MINIMAL_IMPL(save_minimal, )
    CEREAL_MAKE_HAS_MEMBER_SAVE_MINIMAL_HELPERS_IMPL(save_minimal, )
    CEREAL_MAKE_HAS_MEMBER_SAVE_MINIMAL_TEST(save_minimal)
    CEREAL_MAKE_HAS_MEMBER_SAVE_MINIMAL_IMPL(versioned_save_minimal, CEREAL_MAKE_VERSIONED_TEST)
    CEREAL_MAKE_HAS_MEMBER_SAVE_MINIMAL_HELPERS_IMPL(versioned_save_minimal, CEREAL_MAKE_VERSIONED_TEST)
    CEREAL_MAKE_HAS_MEMBER_SAVE_MINIMAL_TEST(versioned_save_minimal)
    #undef CEREAL_MAKE_HAS_MEMBER_SAVE_MINIMAL_IMPL
    #undef CEREAL_MAKE_HAS_MEMBER_SAVE_MINIMAL_HELPERS_IMPL
    #undef CEREAL_MAKE_HAS_MEMBER_SAVE_MINIMAL_TEST
    #define CEREAL_MAKE_HAS_NON_MEMBER_SAVE_MINIMAL_TEST(test_name, versioned)                                               \
    namespace detail                                                                                                         \
    {                                                                                                                        \
      template <class T, class A>                                                                                            \
      struct has_non_member_##test_name##_impl                                                                               \
      {                                                                                                                      \
        template <class TT, class AA>                                                                                        \
        static auto test(int) -> decltype( CEREAL_SAVE_MINIMAL_FUNCTION_NAME(                                                \
              std::declval<AA const &>(),                                                                                    \
              std::declval<TT const &>() versioned ), yes());                                                                \
        template <class, class> static no test(...);                                                                         \
        static const bool value = std::is_same<decltype(test<T, A>(0)), yes>::value;                                         \
                                                                                                                             \
        template <class TT, class AA>                                                                                        \
        static auto test2(int) -> decltype( CEREAL_SAVE_MINIMAL_FUNCTION_NAME(                                               \
              std::declval<AA const &>(),                                                                                    \
              std::declval<typename std::remove_const<TT>::type&>() versioned ), yes());                                     \
        template <class, class> static no test2(...);                                                                        \
        static const bool not_const_type = std::is_same<decltype(test2<T, A>(0)), yes>::value;                               \
                                                                                                                             \
        static const bool valid = value || !not_const_type;                                                                  \
      };                                                                                                                     \
                                                                                                                             \
      template <class T, class A, bool Valid>                                                                                \
      struct get_non_member_##test_name##_type { using type = void; };                                                       \
                                                                                                                             \
      template <class T, class A>                                                                                            \
      struct get_non_member_##test_name##_type <T, A, true>                                                                  \
      {                                                                                                                      \
        using type = decltype( CEREAL_SAVE_MINIMAL_FUNCTION_NAME( std::declval<A const &>(),                                 \
                                                                  std::declval<T const &>() versioned ) );                   \
      };                                                                                                                     \
    }                                                                                              \
                                                                                                                             \
    template <class T, class A>                                                                                              \
    struct has_non_member_##test_name : std::integral_constant<bool, detail::has_non_member_##test_name##_impl<T, A>::value> \
    {                                                                                                                        \
      using check = typename detail::has_non_member_##test_name##_impl<T, A>;                                                \
      static_assert( check::valid,                                                                                           \
        "cereal detected a non-const type parameter in non-member " #test_name ". \n "                                       \
        #test_name " non-member functions must always pass their types as const" );                                          \
                                                                                                                             \
      using type = typename detail::get_non_member_##test_name##_type<T, A, check::value>::type;                             \
      static_assert( (check::value && is_minimal_type<type>::value) || !check::value,                                        \
        "cereal detected a non-member " #test_name " with an invalid return type. \n "                                       \
        "return type must be arithmetic or string" );                                                                        \
    };
    CEREAL_MAKE_HAS_NON_MEMBER_SAVE_MINIMAL_TEST(save_minimal, )
    CEREAL_MAKE_HAS_NON_MEMBER_SAVE_MINIMAL_TEST(versioned_save_minimal, CEREAL_MAKE_VERSIONED_TEST)
    #undef CEREAL_MAKE_HAS_NON_MEMBER_SAVE_MINIMAL_TEST
    namespace detail
    {
      struct NoConvertBase {};
      template <class Source>
      struct NoConvertConstRef : NoConvertBase
      {
        using type = Source;
        template <class Dest, class = typename std::enable_if<std::is_same<Source, Dest>::value>::type>
        operator Dest () = delete;
        template <class Dest, class = typename std::enable_if<std::is_same<Source, Dest>::value>::type>
        operator Dest const & ();
      };
      template <class Source>
      struct NoConvertRef : NoConvertBase
      {
        using type = Source;
        template <class Dest, class = typename std::enable_if<std::is_same<Source, Dest>::value>::type>
        operator Dest () = delete;
        #ifdef __clang__
        template <class Dest, class = typename std::enable_if<std::is_same<Source, Dest>::value>::type>
        operator Dest const & () = delete;
        #endif
        template <class Dest, class = typename std::enable_if<std::is_same<Source, Dest>::value>::type>
        operator Dest & ();
      };
      struct AnyConvert
      {
        template <class Dest>
        operator Dest & ();
        template <class Dest>
        operator Dest const & () const;
      };
    }
    #ifdef CEREAL_OLDER_GCC
    #define CEREAL_MAKE_HAS_MEMBER_LOAD_MINIMAL_IMPL(test_name, versioned)                                                    \
    namespace detail                                                                                                          \
    {                                                                                                                         \
      template <class T, class A, class SFINAE = void> struct has_member_##test_name##_impl : no {};                          \
      template <class T, class A>                                                                                             \
      struct has_member_##test_name##_impl<T, A, typename detail::Void< decltype(                                             \
          cereal::access::member_load_minimal( std::declval<A const &>(),                                                     \
                                               std::declval<T &>(), AnyConvert() versioned ) ) >::type> : yes {};             \
                                                                                                                              \
        template <class T, class A, class U, class SFINAE = void> struct has_member_##test_name##_type_impl : no {};          \
        template <class T, class A, class U>                                                                                  \
        struct has_member_##test_name##_type_impl<T, A, U, typename detail::Void< decltype(                                   \
            cereal::access::member_load_minimal( std::declval<A const &>(),                                                   \
                                                 std::declval<T &>(), NoConvertConstRef<U>() versioned ) ) >::type> : yes {}; \
    }
    #else
    #define CEREAL_MAKE_HAS_MEMBER_LOAD_MINIMAL_IMPL(test_name, versioned)              \
    namespace detail                                                                    \
    {                                                                                   \
      template <class T, class A>                                                       \
      struct has_member_##test_name##_impl                                              \
      {                                                                                 \
        template <class TT, class AA>                                                   \
        static auto test(int) -> decltype( cereal::access::member_load_minimal(         \
              std::declval<AA const &>(),                                               \
              std::declval<TT &>(), AnyConvert() versioned ), yes());                   \
        template <class, class> static no test(...);                                    \
        static const bool value = std::is_same<decltype(test<T, A>(0)), yes>::value;    \
      };                                                                                \
      template <class T, class A, class U>                                              \
      struct has_member_##test_name##_type_impl                                         \
      {                                                                                 \
        template <class TT, class AA, class UU>                                         \
        static auto test(int) -> decltype( cereal::access::member_load_minimal(         \
              std::declval<AA const &>(),                                               \
              std::declval<TT &>(), NoConvertConstRef<UU>() versioned ), yes());        \
        template <class, class, class> static no test(...);                             \
        static const bool value = std::is_same<decltype(test<T, A, U>(0)), yes>::value; \
                                                                                        \
      };                                                                                \
    }
    #endif
    #define CEREAL_MAKE_HAS_MEMBER_LOAD_MINIMAL_HELPERS_IMPL(load_test_name, save_test_name, save_test_prefix, versioned) \
    namespace detail                                                                                                      \
    {                                                                                                                     \
      template <class T, class A, bool Valid>                                                                             \
      struct has_member_##load_test_name##_wrapper : std::false_type {};                                                  \
                                                                                                                          \
      template <class T, class A>                                                                                         \
      struct has_member_##load_test_name##_wrapper<T, A, true>                                                            \
      {                                                                                                                   \
        using AOut = typename detail::get_output_from_input<A>::type;                                                     \
                                                                                                                          \
        static_assert( has_member_##save_test_prefix##_minimal<T, AOut>::value,                                           \
          "cereal detected member " #load_test_name " but no valid member " #save_test_name ". \n "                       \
          "cannot evaluate correctness of " #load_test_name " without valid " #save_test_name "." );                      \
                                                                                                                          \
        using SaveType = typename detail::get_member_##save_test_prefix##_minimal_type<T, AOut, true>::type;              \
        const static bool value = has_member_##load_test_name##_impl<T, A>::value;                                        \
        const static bool valid = has_member_##load_test_name##_type_impl<T, A, SaveType>::value;                         \
                                                                                                                          \
        static_assert( valid || !value, "cereal detected different or invalid types in corresponding member "             \
            #load_test_name " and " #save_test_name " functions. \n "                                                     \
            "the paramater to " #load_test_name " must be a constant reference to the type that "                         \
            #save_test_name " returns." );                                                                                \
      };                                                                                                                  \
    }
    #define CEREAL_MAKE_HAS_MEMBER_LOAD_MINIMAL_TEST(load_test_name, load_test_prefix)                                         \
    template <class T, class A>                                                                                                \
    struct has_member_##load_test_prefix##_minimal : std::integral_constant<bool,                                              \
      detail::has_member_##load_test_name##_wrapper<T, A, detail::has_member_##load_test_name##_impl<T, A>::value>::value> {};
    CEREAL_MAKE_HAS_MEMBER_LOAD_MINIMAL_IMPL(load_minimal, )
    CEREAL_MAKE_HAS_MEMBER_LOAD_MINIMAL_HELPERS_IMPL(load_minimal, save_minimal, save, )
    CEREAL_MAKE_HAS_MEMBER_LOAD_MINIMAL_TEST(load_minimal, load)
    CEREAL_MAKE_HAS_MEMBER_LOAD_MINIMAL_IMPL(versioned_load_minimal, CEREAL_MAKE_VERSIONED_TEST)
    CEREAL_MAKE_HAS_MEMBER_LOAD_MINIMAL_HELPERS_IMPL(versioned_load_minimal, versioned_save_minimal, versioned_save, CEREAL_MAKE_VERSIONED_TEST)
    CEREAL_MAKE_HAS_MEMBER_LOAD_MINIMAL_TEST(versioned_load_minimal, versioned_load)
    #undef CEREAL_MAKE_HAS_MEMBER_LOAD_MINIMAL_IMPL
    #undef CEREAL_MAKE_HAS_MEMBER_LOAD_MINIMAL_HELPERS_IMPL
    #undef CEREAL_MAKE_HAS_MEMBER_LOAD_MINIMAL_TEST
    namespace detail
    {
      #ifdef CEREAL_OLDER_GCC
      void CEREAL_LOAD_MINIMAL_FUNCTION_NAME();
      void CEREAL_SAVE_MINIMAL_FUNCTION_NAME();
      #endif
    }
    #define CEREAL_MAKE_HAS_NON_MEMBER_LOAD_MINIMAL_TEST(test_name, save_name, versioned)                                    \
    namespace detail                                                                                                         \
    {                                                                                                                        \
      template <class T, class A, class U = void>                                                                            \
      struct has_non_member_##test_name##_impl                                                                               \
      {                                                                                                                      \
        template <class TT, class AA>                                                                                        \
        static auto test(int) -> decltype( CEREAL_LOAD_MINIMAL_FUNCTION_NAME(                                                \
              std::declval<AA const &>(), std::declval<TT&>(), AnyConvert() versioned ), yes() );                            \
        template <class, class> static no test( ... );                                                                       \
        static const bool exists = std::is_same<decltype( test<T, A>( 0 ) ), yes>::value;                                    \
                                                                                                                             \
        template <class TT, class AA, class UU>                                                                              \
        static auto test2(int) -> decltype( CEREAL_LOAD_MINIMAL_FUNCTION_NAME(                                               \
              std::declval<AA const &>(), std::declval<TT&>(), NoConvertConstRef<UU>() versioned ), yes() );                 \
        template <class, class, class> static no test2( ... );                                                               \
        static const bool valid = std::is_same<decltype( test2<T, A, U>( 0 ) ), yes>::value;                                 \
                                                                                                                             \
        template <class TT, class AA>                                                                                        \
        static auto test3(int) -> decltype( CEREAL_LOAD_MINIMAL_FUNCTION_NAME(                                               \
              std::declval<AA const &>(), NoConvertRef<TT>(), AnyConvert() versioned ), yes() );                             \
        template <class, class> static no test3( ... );                                                                      \
        static const bool const_valid = std::is_same<decltype( test3<T, A>( 0 ) ), yes>::value;                              \
      };                                                                                                                     \
                                                                                                                             \
      template <class T, class A, bool Valid>                                                                                \
      struct has_non_member_##test_name##_wrapper : std::false_type {};                                                      \
                                                                                                                             \
      template <class T, class A>                                                                                            \
      struct has_non_member_##test_name##_wrapper<T, A, true>                                                                \
      {                                                                                                                      \
        using AOut = typename detail::get_output_from_input<A>::type;                                                        \
                                                                                                                             \
        static_assert( detail::has_non_member_##save_name##_impl<T, AOut>::valid,                                            \
          "cereal detected non-member " #test_name " but no valid non-member " #save_name ". \n "                            \
          "cannot evaluate correctness of " #test_name " without valid " #save_name "." );                                   \
                                                                                                                             \
        using SaveType = typename detail::get_non_member_##save_name##_type<T, AOut, true>::type;                            \
        using check = has_non_member_##test_name##_impl<T, A, SaveType>;                                                     \
        static const bool value = check::exists;                                                                             \
                                                                                                                             \
        static_assert( check::valid || !check::exists, "cereal detected different types in corresponding non-member "        \
            #test_name " and " #save_name " functions. \n "                                                                  \
            "the paramater to " #test_name " must be a constant reference to the type that " #save_name " returns." );       \
        static_assert( check::const_valid || !check::exists,                                                                 \
            "cereal detected an invalid serialization type parameter in non-member " #test_name ".  "                        \
            #test_name " non-member functions must accept their serialization type by non-const reference" );                \
      };                                                                                                                     \
    }                                                                                                  \
                                                                                                                             \
    template <class T, class A>                                                                                              \
    struct has_non_member_##test_name : std::integral_constant<bool,                                                         \
      detail::has_non_member_##test_name##_wrapper<T, A, detail::has_non_member_##test_name##_impl<T, A>::exists>::value> {};
    CEREAL_MAKE_HAS_NON_MEMBER_LOAD_MINIMAL_TEST(load_minimal, save_minimal, )
    CEREAL_MAKE_HAS_NON_MEMBER_LOAD_MINIMAL_TEST(versioned_load_minimal, versioned_save_minimal, CEREAL_MAKE_VERSIONED_TEST)
    #undef CEREAL_MAKE_HAS_NON_MEMBER_LOAD_MINIMAL_TEST
    #undef CEREAL_MAKE_VERSIONED_TEST
    template <class T, class InputArchive, class OutputArchive>
    struct has_member_split : std::integral_constant<bool,
      (has_member_load<T, InputArchive>::value && has_member_save<T, OutputArchive>::value) ||
      (has_member_versioned_load<T, InputArchive>::value && has_member_versioned_save<T, OutputArchive>::value)> {};
    template <class T, class InputArchive, class OutputArchive>
    struct has_non_member_split : std::integral_constant<bool,
      (has_non_member_load<T, InputArchive>::value && has_non_member_save<T, OutputArchive>::value) ||
      (has_non_member_versioned_load<T, InputArchive>::value && has_non_member_versioned_save<T, OutputArchive>::value)> {};
    template <class T, class OutputArchive>
    struct has_invalid_output_versioning : std::integral_constant<bool,
      (has_member_versioned_save<T, OutputArchive>::value && has_member_save<T, OutputArchive>::value) ||
      (has_non_member_versioned_save<T, OutputArchive>::value && has_non_member_save<T, OutputArchive>::value) ||
      (has_member_versioned_serialize<T, OutputArchive>::value && has_member_serialize<T, OutputArchive>::value) ||
      (has_non_member_versioned_serialize<T, OutputArchive>::value && has_non_member_serialize<T, OutputArchive>::value) ||
      (has_member_versioned_save_minimal<T, OutputArchive>::value && has_member_save_minimal<T, OutputArchive>::value) ||
      (has_non_member_versioned_save_minimal<T, OutputArchive>::value &&  has_non_member_save_minimal<T, OutputArchive>::value)> {};
    template <class T, class InputArchive>
    struct has_invalid_input_versioning : std::integral_constant<bool,
      (has_member_versioned_load<T, InputArchive>::value && has_member_load<T, InputArchive>::value) ||
      (has_non_member_versioned_load<T, InputArchive>::value && has_non_member_load<T, InputArchive>::value) ||
      (has_member_versioned_serialize<T, InputArchive>::value && has_member_serialize<T, InputArchive>::value) ||
      (has_non_member_versioned_serialize<T, InputArchive>::value && has_non_member_serialize<T, InputArchive>::value) ||
      (has_member_versioned_load_minimal<T, InputArchive>::value && has_member_load_minimal<T, InputArchive>::value) ||
      (has_non_member_versioned_load_minimal<T, InputArchive>::value &&  has_non_member_load_minimal<T, InputArchive>::value)> {};
    namespace detail
    {
      #define CEREAL_MAKE_IS_SPECIALIZED_IMPL(name)                                          \
      template <class T, class A>                                                            \
      struct is_specialized_##name : std::integral_constant<bool,                            \
        !std::is_base_of<std::false_type, specialize<A, T, specialization::name>>::value> {}
      CEREAL_MAKE_IS_SPECIALIZED_IMPL(member_serialize);
      CEREAL_MAKE_IS_SPECIALIZED_IMPL(member_load_save);
      CEREAL_MAKE_IS_SPECIALIZED_IMPL(member_load_save_minimal);
      CEREAL_MAKE_IS_SPECIALIZED_IMPL(non_member_serialize);
      CEREAL_MAKE_IS_SPECIALIZED_IMPL(non_member_load_save);
      CEREAL_MAKE_IS_SPECIALIZED_IMPL(non_member_load_save_minimal);
      #undef CEREAL_MAKE_IS_SPECIALIZED_IMPL
      template <class T, class A>
      struct count_specializations : std::integral_constant<int,
        is_specialized_member_serialize<T, A>::value +
        is_specialized_member_load_save<T, A>::value +
        is_specialized_member_load_save_minimal<T, A>::value +
        is_specialized_non_member_serialize<T, A>::value +
        is_specialized_non_member_load_save<T, A>::value +
        is_specialized_non_member_load_save_minimal<T, A>::value> {};
    }
    template <class T, class A>
    struct is_specialized : std::integral_constant<bool,
      detail::is_specialized_member_serialize<T, A>::value ||
      detail::is_specialized_member_load_save<T, A>::value ||
      detail::is_specialized_member_load_save_minimal<T, A>::value ||
      detail::is_specialized_non_member_serialize<T, A>::value ||
      detail::is_specialized_non_member_load_save<T, A>::value ||
      detail::is_specialized_non_member_load_save_minimal<T, A>::value>
    {
      static_assert(detail::count_specializations<T, A>::value <= 1, "More than one explicit specialization detected for type.");
    };
    #define CEREAL_MAKE_IS_SPECIALIZED_ASSERT(name, versioned_name, print_name, spec_name)                      \
    static_assert( (is_specialized<T, A>::value && detail::is_specialized_##spec_name<T, A>::value &&           \
                   (has_##name<T, A>::value || has_##versioned_name<T, A>::value))                              \
                   || !(is_specialized<T, A>::value && detail::is_specialized_##spec_name<T, A>::value),        \
                   "cereal detected " #print_name " specialization but no " #print_name " serialize function" )
    #define CEREAL_MAKE_IS_SPECIALIZED(name, versioned_name, spec_name)                     \
    template <class T, class A>                                                             \
    struct is_specialized_##name : std::integral_constant<bool,                             \
      is_specialized<T, A>::value && detail::is_specialized_##spec_name<T, A>::value>       \
    { CEREAL_MAKE_IS_SPECIALIZED_ASSERT(name, versioned_name, name, spec_name); };          \
    template <class T, class A>                                                             \
    struct is_specialized_##versioned_name : std::integral_constant<bool,                   \
      is_specialized<T, A>::value && detail::is_specialized_##spec_name<T, A>::value>       \
    { CEREAL_MAKE_IS_SPECIALIZED_ASSERT(name, versioned_name, versioned_name, spec_name); }
    CEREAL_MAKE_IS_SPECIALIZED(member_serialize, member_versioned_serialize, member_serialize);
    CEREAL_MAKE_IS_SPECIALIZED(non_member_serialize, non_member_versioned_serialize, non_member_serialize);
    CEREAL_MAKE_IS_SPECIALIZED(member_save, member_versioned_save, member_load_save);
    CEREAL_MAKE_IS_SPECIALIZED(non_member_save, non_member_versioned_save, non_member_load_save);
    CEREAL_MAKE_IS_SPECIALIZED(member_load, member_versioned_load, member_load_save);
    CEREAL_MAKE_IS_SPECIALIZED(non_member_load, non_member_versioned_load, non_member_load_save);
    CEREAL_MAKE_IS_SPECIALIZED(member_save_minimal, member_versioned_save_minimal, member_load_save_minimal);
    CEREAL_MAKE_IS_SPECIALIZED(non_member_save_minimal, non_member_versioned_save_minimal, non_member_load_save_minimal);
    CEREAL_MAKE_IS_SPECIALIZED(member_load_minimal, member_versioned_load_minimal, member_load_save_minimal);
    CEREAL_MAKE_IS_SPECIALIZED(non_member_load_minimal, non_member_versioned_load_minimal, non_member_load_save_minimal);
    #undef CEREAL_MAKE_IS_SPECIALIZED_ASSERT
    #undef CEREAL_MAKE_IS_SPECIALIZED
    template <class T, class OutputArchive>
    struct has_minimal_output_serialization : std::integral_constant<bool,
      is_specialized_member_save_minimal<T, OutputArchive>::value ||
      ((has_member_save_minimal<T, OutputArchive>::value ||
        has_non_member_save_minimal<T, OutputArchive>::value ||
        has_member_versioned_save_minimal<T, OutputArchive>::value ||
        has_non_member_versioned_save_minimal<T, OutputArchive>::value) &&
       !(is_specialized_member_serialize<T, OutputArchive>::value ||
         is_specialized_member_save<T, OutputArchive>::value))> {};
    template <class T, class InputArchive>
    struct has_minimal_input_serialization : std::integral_constant<bool,
      is_specialized_member_load_minimal<T, InputArchive>::value ||
      ((has_member_load_minimal<T, InputArchive>::value ||
        has_non_member_load_minimal<T, InputArchive>::value ||
        has_member_versioned_load_minimal<T, InputArchive>::value ||
        has_non_member_versioned_load_minimal<T, InputArchive>::value) &&
       !(is_specialized_member_serialize<T, InputArchive>::value ||
         is_specialized_member_load<T, InputArchive>::value))> {};
    namespace detail
    {
      template <class T, class OutputArchive>
      struct count_output_serializers : std::integral_constant<int,
        count_specializations<T, OutputArchive>::value ? count_specializations<T, OutputArchive>::value :
        has_member_save<T, OutputArchive>::value +
        has_non_member_save<T, OutputArchive>::value +
        has_member_serialize<T, OutputArchive>::value +
        has_non_member_serialize<T, OutputArchive>::value +
        has_member_save_minimal<T, OutputArchive>::value +
        has_non_member_save_minimal<T, OutputArchive>::value +
        has_member_versioned_save<T, OutputArchive>::value +
        has_non_member_versioned_save<T, OutputArchive>::value +
        has_member_versioned_serialize<T, OutputArchive>::value +
        has_non_member_versioned_serialize<T, OutputArchive>::value +
        has_member_versioned_save_minimal<T, OutputArchive>::value +
        has_non_member_versioned_save_minimal<T, OutputArchive>::value> {};
    }
    template <class T, class OutputArchive>
    struct is_output_serializable : std::integral_constant<bool,
      detail::count_output_serializers<T, OutputArchive>::value == 1> {};
    namespace detail
    {
      template <class T, class InputArchive>
      struct count_input_serializers : std::integral_constant<int,
        count_specializations<T, InputArchive>::value ? count_specializations<T, InputArchive>::value :
        has_member_load<T, InputArchive>::value +
        has_non_member_load<T, InputArchive>::value +
        has_member_serialize<T, InputArchive>::value +
        has_non_member_serialize<T, InputArchive>::value +
        has_member_load_minimal<T, InputArchive>::value +
        has_non_member_load_minimal<T, InputArchive>::value +
        has_member_versioned_load<T, InputArchive>::value +
        has_non_member_versioned_load<T, InputArchive>::value +
        has_member_versioned_serialize<T, InputArchive>::value +
        has_non_member_versioned_serialize<T, InputArchive>::value +
        has_member_versioned_load_minimal<T, InputArchive>::value +
        has_non_member_versioned_load_minimal<T, InputArchive>::value> {};
    }
    template <class T, class InputArchive>
    struct is_input_serializable : std::integral_constant<bool,
      detail::count_input_serializers<T, InputArchive>::value == 1> {};
    namespace detail
    {
      struct base_class_id
      {
        template<class T>
          base_class_id(T const * const t) :
          type(typeid(T)),
          ptr(t),
          hash(std::hash<std::type_index>()(typeid(T)) ^ (std::hash<void const *>()(t) << 1))
          { }
          bool operator==(base_class_id const & other) const
          { return (type == other.type) && (ptr == other.ptr); }
          std::type_index type;
          void const * ptr;
          size_t hash;
      };
      struct base_class_id_hash { size_t operator()(base_class_id const & id) const { return id.hash; }  };
    }
    namespace detail
    {
      struct BaseCastBase {};
      template <class>
      struct get_base_class;
      template <template<typename> class Cast, class Base>
      struct get_base_class<Cast<Base>>
      {
        using type = Base;
      };
      template <class Cast, template<class, class> class Test, class Archive,
                bool IsBaseCast = std::is_base_of<BaseCastBase, Cast>::value>
      struct has_minimal_base_class_serialization_impl : Test<typename get_base_class<Cast>::type, Archive>
      { };
      template <class Cast, template<class, class> class Test, class Archive>
      struct has_minimal_base_class_serialization_impl<Cast,Test, Archive, false> : std::false_type
      { };
    }
    template <class Cast, template<class, class> class Test, class Archive>
    struct has_minimal_base_class_serialization : detail::has_minimal_base_class_serialization_impl<Cast, Test, Archive>
    { };
    namespace detail
    {
      struct shared_from_this_wrapper
      {
        template <class U>
        static auto check( U const & t ) -> decltype( ::cereal::access::shared_from_this(t), std::true_type() );
        static auto check( ... ) -> decltype( std::false_type() );
        template <class U>
        static auto get( U const & t ) -> decltype( t.shared_from_this() );
      };
    }
    template<class T>
    struct has_shared_from_this : decltype(detail::shared_from_this_wrapper::check(std::declval<T>()))
    { };
    template <class T>
    struct get_shared_from_this_base
    {
      private:
        using PtrType = decltype(detail::shared_from_this_wrapper::get(std::declval<T>()));
      public:
        using type = typename std::decay<typename PtrType::element_type>::type;
    };
    template <class T, bool IsCerealMinimalTrait = std::is_base_of<detail::NoConvertBase, T>::value>
    struct strip_minimal
    {
      using type = T;
    };
    template <class T>
    struct strip_minimal<T, true>
    {
      using type = typename T::type;
    };
    template<typename T, typename A>
    struct has_member_load_and_construct : std::integral_constant<bool,
      std::is_same<decltype( access::load_and_construct<T>( std::declval<A&>(), std::declval< ::cereal::construct<T>&>() ) ), void>::value>
    { };
    template<typename T, typename A>
    struct has_non_member_load_and_construct : std::integral_constant<bool,
      std::is_same<decltype( LoadAndConstruct<T>::load_and_construct( std::declval<A&>(), std::declval< ::cereal::construct<T>&>() ) ), void>::value>
    { };
    template<typename T, typename A>
    struct has_load_and_construct : std::integral_constant<bool,
      has_member_load_and_construct<T, A>::value || has_non_member_load_and_construct<T, A>::value>
    { };
    template <class T>
    struct is_default_constructible
    {
      #ifdef CEREAL_OLDER_GCC
      template <class TT, class SFINAE = void>
      struct test : no {};
      template <class TT>
      struct test<TT, typename detail::Void< decltype( cereal::access::construct<TT>() ) >::type> : yes {};
      static const bool value = test<T>();
      #else
      template <class TT>
      static auto test(int) -> decltype( cereal::access::construct<TT>(), yes());
      template <class>
      static no test(...);
      static const bool value = std::is_same<decltype(test<T>(0)), yes>::value;
      #endif
    };
    namespace detail
    {
      template <class A>
      using decay_archive = typename std::decay<typename strip_minimal<A>::type>::type;
    }
    template <class ArchiveT, class CerealArchiveT>
    struct is_same_archive : std::integral_constant<bool,
      std::is_same<detail::decay_archive<ArchiveT>, CerealArchiveT>::value>
    { };
    #define CEREAL_ARCHIVE_RESTRICT(INTYPE, OUTTYPE) \
    typename std::enable_if<cereal::traits::is_same_archive<Archive, INTYPE>::value || cereal::traits::is_same_archive<Archive, OUTTYPE>::value, void>::type
    struct TextArchive {};
    template <class A>
    struct is_text_archive : std::integral_constant<bool,
      std::is_base_of<TextArchive, detail::decay_archive<A>>::value>
    { };
  }
  namespace detail
  {
    template <class T, class A, bool Member = traits::has_member_load_and_construct<T, A>::value, bool NonMember = traits::has_non_member_load_and_construct<T, A>::value>
    struct Construct
    {
      static_assert( cereal::traits::detail::delay_static_assert<T>::value,
        "Cereal detected both member and non member load_and_construct functions!" );
      static T * load_andor_construct( A & , construct<T> &  )
      { return nullptr; }
    };
    template <class T, class A>
    struct Construct<T, A, false, false>
    {
      static_assert( ::cereal::traits::is_default_constructible<T>::value,
                     "Trying to serialize a an object with no default constructor. \n\n "
                     "Types must either be default constructible or define either a member or non member Construct function. \n "
                     "Construct functions generally have the signature: \n\n "
                     "template <class Archive> \n "
                     "static void load_and_construct(Archive & ar, cereal::construct<T> & construct) \n "
                     "{ \n "
                     "  var a; \n "
                     "  ar( a ) \n "
                     "  construct( a ); \n "
                     "} \n\n" );
      static T * load_andor_construct()
      { return ::cereal::access::construct<T>(); }
    };
    template <class T, class A>
    struct Construct<T, A, true, false>
    {
      static void load_andor_construct( A & ar, construct<T> & construct )
      {
        access::load_and_construct<T>( ar, construct );
      }
    };
    template <class T, class A>
    struct Construct<T, A, false, true>
    {
      static void load_andor_construct( A & ar, construct<T> & construct )
      {
        LoadAndConstruct<T>::load_and_construct( ar, construct );
      }
    };
  }
}
#endif
/*#include <cereal/details/helpers.hpp> skipped */
/*#include <cereal/types/base_class.hpp>*/
#ifndef CEREAL_TYPES_BASE_CLASS_HPP_
#define CEREAL_TYPES_BASE_CLASS_HPP_
/*#include <cereal/details/traits.hpp> skipped */
namespace cereal
{
  template<class Base>
    struct base_class : private traits::detail::BaseCastBase
    {
      template<class Derived>
        base_class(Derived const * derived) :
          base_ptr(const_cast<Base*>(static_cast<Base const *>(derived)))
      { static_assert( std::is_base_of<Base, Derived>::value, "Can only use base_class on a valid base class" ); }
        Base * base_ptr;
    };
  template<class Base>
    struct virtual_base_class : private traits::detail::BaseCastBase
    {
      template<class Derived>
        virtual_base_class(Derived const * derived) :
          base_ptr(const_cast<Base*>(static_cast<Base const *>(derived)))
      { static_assert( std::is_base_of<Base, Derived>::value, "Can only use base_class on a valid base class" ); }
        Base * base_ptr;
    };
}
#endif
namespace cereal
{
  template <class T> inline
  NameValuePair<T> make_nvp( std::string const & name, T && value )
  {
    return {name.c_str(), std::forward<T>(value)};
  }
  template <class T> inline
  NameValuePair<T> make_nvp( const char * name, T && value )
  {
    return {name, std::forward<T>(value)};
  }
  #define CEREAL_NVP(T) ::cereal::make_nvp(#T, T)
  template <class T> inline
  BinaryData<T> binary_data( T && data, size_t size )
  {
    return {std::forward<T>(data), size};
  }
  template <class T>
  SizeTag<T> make_size_tag( T && sz )
  {
    return {std::forward<T>(sz)};
  }
  template <class Archive, class T>
  void prologue( Archive & , T const & )
  { }
  template <class Archive, class T>
  void epilogue( Archive & , T const & )
  { }
  enum Flags { AllowEmptyClassElision = 1 };
  #define CEREAL_REGISTER_ARCHIVE(Archive)                              \
  namespace cereal { namespace detail {                                 \
  template <class T, class BindingTag>                                  \
  typename polymorphic_serialization_support<Archive, T>::type          \
  instantiate_polymorphic_binding( T*, Archive*, BindingTag, adl_tag ); \
  } }
  #define CEREAL_CLASS_VERSION(TYPE, VERSION_NUMBER)                             \
  namespace cereal { namespace detail {                                          \
    template <> struct Version<TYPE>                                             \
    {                                                                            \
      static const std::uint32_t version;                                        \
      static std::uint32_t registerVersion()                                     \
      {                                                                          \
        ::cereal::detail::StaticObject<Versions>::getInstance().mapping.emplace( \
             std::type_index(typeid(TYPE)).hash_code(), VERSION_NUMBER );        \
        return VERSION_NUMBER;                                                   \
      }                                                                          \
      static void unused() { (void)version; }                                    \
    };                                                          \
    const std::uint32_t Version<TYPE>::version =                                 \
      Version<TYPE>::registerVersion();                                          \
  } }
  template<class ArchiveType, std::uint32_t Flags = 0>
  class OutputArchive : public detail::OutputArchiveBase
  {
    public:
      OutputArchive(ArchiveType * const derived) : self(derived), itsCurrentPointerId(1), itsCurrentPolymorphicTypeId(1)
      { }
      OutputArchive & operator=( OutputArchive const & ) = delete;
      template <class ... Types> inline
      ArchiveType & operator()( Types && ... args )
      {
        self->process( std::forward<Types>( args )... );
        return *self;
      }
      template <class T> inline
      ArchiveType & operator&( T && arg )
      {
        self->process( std::forward<T>( arg ) );
        return *self;
      }
      template <class T> inline
      ArchiveType & operator<<( T && arg )
      {
        self->process( std::forward<T>( arg ) );
        return *self;
      }
      inline std::uint32_t registerSharedPointer( void const * addr )
      {
        if(addr == 0) return 0;
        auto id = itsSharedPointerMap.find( addr );
        if( id == itsSharedPointerMap.end() )
        {
          auto ptrId = itsCurrentPointerId++;
          itsSharedPointerMap.insert( {addr, ptrId} );
          return ptrId | detail::msb_32bit;
        }
        else
          return id->second;
      }
      inline std::uint32_t registerPolymorphicType( char const * name )
      {
        auto id = itsPolymorphicTypeMap.find( name );
        if( id == itsPolymorphicTypeMap.end() )
        {
          auto polyId = itsCurrentPolymorphicTypeId++;
          itsPolymorphicTypeMap.insert( {name, polyId} );
          return polyId | detail::msb_32bit;
        }
        else
          return id->second;
      }
    private:
      template <class T> inline
      void process( T && head )
      {
        prologue( *self, head );
        self->processImpl( head );
        epilogue( *self, head );
      }
      template <class T, class ... Other> inline
      void process( T && head, Other && ... tail )
      {
        self->process( std::forward<T>( head ) );
        self->process( std::forward<Other>( tail )... );
      }
      template <class T> inline
      ArchiveType & processImpl(virtual_base_class<T> const & b)
      {
        traits::detail::base_class_id id(b.base_ptr);
        if(itsBaseClassSet.count(id) == 0)
        {
          itsBaseClassSet.insert(id);
          self->processImpl( *b.base_ptr );
        }
        return *self;
      }
      template <class T> inline
      ArchiveType & processImpl(base_class<T> const & b)
      {
        self->processImpl( *b.base_ptr );
        return *self;
      }
      #define PROCESS_IF(name)                                                             \
      traits::EnableIf<traits::has_##name<T, ArchiveType>::value,                          \
                       !traits::has_invalid_output_versioning<T, ArchiveType>::value,      \
                       (traits::is_output_serializable<T, ArchiveType>::value &&           \
                        (traits::is_specialized_##name<T, ArchiveType>::value ||           \
                         !traits::is_specialized<T, ArchiveType>::value))> = traits::sfinae
      template <class T, PROCESS_IF(member_serialize)> inline
      ArchiveType & processImpl(T const & t)
      {
        access::member_serialize(*self, const_cast<T &>(t));
        return *self;
      }
      template <class T, PROCESS_IF(non_member_serialize)> inline
      ArchiveType & processImpl(T const & t)
      {
        CEREAL_SERIALIZE_FUNCTION_NAME(*self, const_cast<T &>(t));
        return *self;
      }
      template <class T, PROCESS_IF(member_save)> inline
      ArchiveType & processImpl(T const & t)
      {
        access::member_save(*self, t);
        return *self;
      }
      template <class T, PROCESS_IF(non_member_save)> inline
      ArchiveType & processImpl(T const & t)
      {
        CEREAL_SAVE_FUNCTION_NAME(*self, t);
        return *self;
      }
      template <class T, PROCESS_IF(member_save_minimal)> inline
      ArchiveType & processImpl(T const & t)
      {
        self->process( access::member_save_minimal(*self, t) );
        return *self;
      }
      template <class T, PROCESS_IF(non_member_save_minimal)> inline
      ArchiveType & processImpl(T const & t)
      {
        self->process( CEREAL_SAVE_MINIMAL_FUNCTION_NAME(*self, t) );
        return *self;
      }
      template <class T, traits::EnableIf<(Flags & AllowEmptyClassElision),
                                          !traits::is_output_serializable<T, ArchiveType>::value,
                                          std::is_empty<T>::value> = traits::sfinae> inline
      ArchiveType & processImpl(T const &)
      {
        return *self;
      }
      template <class T, traits::EnableIf<traits::has_invalid_output_versioning<T, ArchiveType>::value ||
                                          (!traits::is_output_serializable<T, ArchiveType>::value &&
                                           (!(Flags & AllowEmptyClassElision) || ((Flags & AllowEmptyClassElision) && !std::is_empty<T>::value)))> = traits::sfinae> inline
      ArchiveType & processImpl(T const &)
      {
        static_assert(traits::detail::count_output_serializers<T, ArchiveType>::value != 0,
            "cereal could not find any output serialization functions for the provided type and archive combination. \n\n "
            "Types must either have a serialize function, load/save pair, or load_minimal/save_minimal pair (you may not mix these). \n "
            "Serialize functions generally have the following signature: \n\n "
            "template<class Archive> \n "
            "  void serialize(Archive & ar) \n "
            "  { \n "
            "    ar( member1, member2, member3 ); \n "
            "  } \n\n " );
        static_assert(traits::detail::count_output_serializers<T, ArchiveType>::value < 2,
            "cereal found more than one compatible output serialization function for the provided type and archive combination. \n\n "
            "Types must either have a serialize function, load/save pair, or load_minimal/save_minimal pair (you may not mix these). \n "
            "Use specialization (see access.hpp) if you need to disambiguate between serialize vs load/save functions.  \n "
            "Note that serialization functions can be inherited which may lead to the aforementioned ambiguities. \n "
            "In addition, you may not mix versioned with non-versioned serialization functions. \n\n ");
        return *self;
      }
      template <class T> inline
      std::uint32_t registerClassVersion()
      {
        static const auto hash = std::type_index(typeid(T)).hash_code();
        const auto insertResult = itsVersionedTypes.insert( hash );
        const auto version =
          detail::StaticObject<detail::Versions>::getInstance().find( hash, detail::Version<T>::version );
        if( insertResult.second )
          process( make_nvp<ArchiveType>("cereal_class_version", version) );
        return version;
      }
      template <class T, PROCESS_IF(member_versioned_serialize)> inline
      ArchiveType & processImpl(T const & t)
      {
        access::member_serialize(*self, const_cast<T &>(t), registerClassVersion<T>());
        return *self;
      }
      template <class T, PROCESS_IF(non_member_versioned_serialize)> inline
      ArchiveType & processImpl(T const & t)
      {
        CEREAL_SERIALIZE_FUNCTION_NAME(*self, const_cast<T &>(t), registerClassVersion<T>());
        return *self;
      }
      template <class T, PROCESS_IF(member_versioned_save)> inline
      ArchiveType & processImpl(T const & t)
      {
        access::member_save(*self, t, registerClassVersion<T>());
        return *self;
      }
      template <class T, PROCESS_IF(non_member_versioned_save)> inline
      ArchiveType & processImpl(T const & t)
      {
        CEREAL_SAVE_FUNCTION_NAME(*self, t, registerClassVersion<T>());
        return *self;
      }
      template <class T, PROCESS_IF(member_versioned_save_minimal)> inline
      ArchiveType & processImpl(T const & t)
      {
        self->process( access::member_save_minimal(*self, t, registerClassVersion<T>()) );
        return *self;
      }
      template <class T, PROCESS_IF(non_member_versioned_save_minimal)> inline
      ArchiveType & processImpl(T const & t)
      {
        self->process( CEREAL_SAVE_MINIMAL_FUNCTION_NAME(*self, t, registerClassVersion<T>()) );
        return *self;
      }
    #undef PROCESS_IF
    private:
      ArchiveType * const self;
      std::unordered_set<traits::detail::base_class_id, traits::detail::base_class_id_hash> itsBaseClassSet;
      std::unordered_map<void const *, std::uint32_t> itsSharedPointerMap;
      std::uint32_t itsCurrentPointerId;
      std::unordered_map<char const *, std::uint32_t> itsPolymorphicTypeMap;
      std::uint32_t itsCurrentPolymorphicTypeId;
      std::unordered_set<size_type> itsVersionedTypes;
  };
  template<class ArchiveType, std::uint32_t Flags = 0>
  class InputArchive : public detail::InputArchiveBase
  {
    public:
      InputArchive(ArchiveType * const derived) :
        self(derived),
        itsBaseClassSet(),
        itsSharedPointerMap(),
        itsPolymorphicTypeMap(),
        itsVersionedTypes()
      { }
      InputArchive & operator=( InputArchive const & ) = delete;
      template <class ... Types> inline
      ArchiveType & operator()( Types && ... args )
      {
        process( std::forward<Types>( args )... );
        return *self;
      }
      template <class T> inline
      ArchiveType & operator&( T && arg )
      {
        self->process( std::forward<T>( arg ) );
        return *self;
      }
      template <class T> inline
      ArchiveType & operator>>( T && arg )
      {
        self->process( std::forward<T>( arg ) );
        return *self;
      }
      inline std::shared_ptr<void> getSharedPointer(std::uint32_t const id)
      {
        if(id == 0) return std::shared_ptr<void>(nullptr);
        auto iter = itsSharedPointerMap.find( id );
        if(iter == itsSharedPointerMap.end())
          throw Exception("Error while trying to deserialize a smart pointer. Could not find id " + std::to_string(id));
        return iter->second;
      }
      inline void registerSharedPointer(std::uint32_t const id, std::shared_ptr<void> ptr)
      {
        std::uint32_t const stripped_id = id & ~detail::msb_32bit;
        itsSharedPointerMap[stripped_id] = ptr;
      }
      inline std::string getPolymorphicName(std::uint32_t const id)
      {
        auto name = itsPolymorphicTypeMap.find( id );
        if(name == itsPolymorphicTypeMap.end())
        {
          throw Exception("Error while trying to deserialize a polymorphic pointer. Could not find type id " + std::to_string(id));
        }
        return name->second;
      }
      inline void registerPolymorphicName(std::uint32_t const id, std::string const & name)
      {
        std::uint32_t const stripped_id = id & ~detail::msb_32bit;
        itsPolymorphicTypeMap.insert( {stripped_id, name} );
      }
    private:
      template <class T> inline
      void process( T && head )
      {
        prologue( *self, head );
        self->processImpl( head );
        epilogue( *self, head );
      }
      template <class T, class ... Other> inline
      void process( T && head, Other && ... tail )
      {
        process( std::forward<T>( head ) );
        process( std::forward<Other>( tail )... );
      }
      template <class T> inline
      ArchiveType & processImpl(virtual_base_class<T> & b)
      {
        traits::detail::base_class_id id(b.base_ptr);
        if(itsBaseClassSet.count(id) == 0)
        {
          itsBaseClassSet.insert(id);
          self->processImpl( *b.base_ptr );
        }
        return *self;
      }
      template <class T> inline
      ArchiveType & processImpl(base_class<T> & b)
      {
        self->processImpl( *b.base_ptr );
        return *self;
      }
      #define PROCESS_IF(name)                                                              \
      traits::EnableIf<traits::has_##name<T, ArchiveType>::value,                           \
                       !traits::has_invalid_input_versioning<T, ArchiveType>::value,        \
                       (traits::is_input_serializable<T, ArchiveType>::value &&             \
                        (traits::is_specialized_##name<T, ArchiveType>::value ||            \
                         !traits::is_specialized<T, ArchiveType>::value))> = traits::sfinae
      template <class T, PROCESS_IF(member_serialize)> inline
      ArchiveType & processImpl(T & t)
      {
        access::member_serialize(*self, t);
        return *self;
      }
      template <class T, PROCESS_IF(non_member_serialize)> inline
      ArchiveType & processImpl(T & t)
      {
        CEREAL_SERIALIZE_FUNCTION_NAME(*self, t);
        return *self;
      }
      template <class T, PROCESS_IF(member_load)> inline
      ArchiveType & processImpl(T & t)
      {
        access::member_load(*self, t);
        return *self;
      }
      template <class T, PROCESS_IF(non_member_load)> inline
      ArchiveType & processImpl(T & t)
      {
        CEREAL_LOAD_FUNCTION_NAME(*self, t);
        return *self;
      }
      template <class T, PROCESS_IF(member_load_minimal)> inline
      ArchiveType & processImpl(T & t)
      {
        using OutArchiveType = typename traits::detail::get_output_from_input<ArchiveType>::type;
        typename traits::has_member_save_minimal<T, OutArchiveType>::type value;
        self->process( value );
        access::member_load_minimal(*self, t, value);
        return *self;
      }
      template <class T, PROCESS_IF(non_member_load_minimal)> inline
      ArchiveType & processImpl(T & t)
      {
        using OutArchiveType = typename traits::detail::get_output_from_input<ArchiveType>::type;
        typename traits::has_non_member_save_minimal<T, OutArchiveType>::type value;
        self->process( value );
        CEREAL_LOAD_MINIMAL_FUNCTION_NAME(*self, t, value);
        return *self;
      }
      template <class T, traits::EnableIf<(Flags & AllowEmptyClassElision),
                                          !traits::is_input_serializable<T, ArchiveType>::value,
                                          std::is_empty<T>::value> = traits::sfinae> inline
      ArchiveType & processImpl(T const &)
      {
        return *self;
      }
      template <class T, traits::EnableIf<traits::has_invalid_input_versioning<T, ArchiveType>::value ||
                                          (!traits::is_input_serializable<T, ArchiveType>::value &&
                                           (!(Flags & AllowEmptyClassElision) || ((Flags & AllowEmptyClassElision) && !std::is_empty<T>::value)))> = traits::sfinae> inline
      ArchiveType & processImpl(T const &)
      {
        static_assert(traits::detail::count_input_serializers<T, ArchiveType>::value != 0,
            "cereal could not find any input serialization functions for the provided type and archive combination. \n\n "
            "Types must either have a serialize function, load/save pair, or load_minimal/save_minimal pair (you may not mix these). \n "
            "Serialize functions generally have the following signature: \n\n "
            "template<class Archive> \n "
            "  void serialize(Archive & ar) \n "
            "  { \n "
            "    ar( member1, member2, member3 ); \n "
            "  } \n\n " );
        static_assert(traits::detail::count_input_serializers<T, ArchiveType>::value < 2,
            "cereal found more than one compatible input serialization function for the provided type and archive combination. \n\n "
            "Types must either have a serialize function, load/save pair, or load_minimal/save_minimal pair (you may not mix these). \n "
            "Use specialization (see access.hpp) if you need to disambiguate between serialize vs load/save functions.  \n "
            "Note that serialization functions can be inherited which may lead to the aforementioned ambiguities. \n "
            "In addition, you may not mix versioned with non-versioned serialization functions. \n\n ");
        return *self;
      }
      template <class T> inline
      std::uint32_t loadClassVersion()
      {
        static const auto hash = std::type_index(typeid(T)).hash_code();
        auto lookupResult = itsVersionedTypes.find( hash );
        if( lookupResult != itsVersionedTypes.end() )
          return lookupResult->second;
        else
        {
          std::uint32_t version;
          process( make_nvp<ArchiveType>("cereal_class_version", version) );
          itsVersionedTypes.emplace_hint( lookupResult, hash, version );
          return version;
        }
      }
      template <class T, PROCESS_IF(member_versioned_serialize)> inline
      ArchiveType & processImpl(T & t)
      {
        const auto version = loadClassVersion<T>();
        access::member_serialize(*self, t, version);
        return *self;
      }
      template <class T, PROCESS_IF(non_member_versioned_serialize)> inline
      ArchiveType & processImpl(T & t)
      {
        const auto version = loadClassVersion<T>();
        CEREAL_SERIALIZE_FUNCTION_NAME(*self, t, version);
        return *self;
      }
      template <class T, PROCESS_IF(member_versioned_load)> inline
      ArchiveType & processImpl(T & t)
      {
        const auto version = loadClassVersion<T>();
        access::member_load(*self, t, version);
        return *self;
      }
      template <class T, PROCESS_IF(non_member_versioned_load)> inline
      ArchiveType & processImpl(T & t)
      {
        const auto version = loadClassVersion<T>();
        CEREAL_LOAD_FUNCTION_NAME(*self, t, version);
        return *self;
      }
      template <class T, PROCESS_IF(member_versioned_load_minimal)> inline
      ArchiveType & processImpl(T & t)
      {
        using OutArchiveType = typename traits::detail::get_output_from_input<ArchiveType>::type;
        const auto version = loadClassVersion<T>();
        typename traits::has_member_versioned_save_minimal<T, OutArchiveType>::type value;
        self->process(value);
        access::member_load_minimal(*self, t, value, version);
        return *self;
      }
      template <class T, PROCESS_IF(non_member_versioned_load_minimal)> inline
      ArchiveType & processImpl(T & t)
      {
        using OutArchiveType = typename traits::detail::get_output_from_input<ArchiveType>::type;
        const auto version = loadClassVersion<T>();
        typename traits::has_non_member_versioned_save_minimal<T, OutArchiveType>::type value;
        self->process(value);
        CEREAL_LOAD_MINIMAL_FUNCTION_NAME(*self, t, value, version);
        return *self;
      }
      #undef PROCESS_IF
    private:
      ArchiveType * const self;
      std::unordered_set<traits::detail::base_class_id, traits::detail::base_class_id_hash> itsBaseClassSet;
      std::unordered_map<std::uint32_t, std::shared_ptr<void>> itsSharedPointerMap;
      std::unordered_map<std::uint32_t, std::string> itsPolymorphicTypeMap;
      std::unordered_map<std::size_t, std::uint32_t> itsVersionedTypes;
  };
}
/*#include <cereal/types/common.hpp>*/
#ifndef CEREAL_TYPES_COMMON_HPP_
#define CEREAL_TYPES_COMMON_HPP_
/*#include <cereal/cereal.hpp> skipped */
namespace cereal
{
  namespace common_detail
  {
    template <class Archive, class T> inline
    void serializeArray( Archive & ar, T & array, std::true_type  )
    {
      ar( binary_data( array, sizeof(array) ) );
    }
    template <class Archive, class T> inline
    void serializeArray( Archive & ar, T & array, std::false_type  )
    {
      for( auto & i : array )
        ar( i );
    }
    namespace
    {
      template <class T, bool IsEnum>
      struct enum_underlying_type : std::false_type {};
      template <class T>
      struct enum_underlying_type<T, true> { using type = typename std::underlying_type<T>::type; };
    }
    template <class T>
    class is_enum
    {
      private:
        using DecayedT  = typename std::decay<T>::type;
        using StrippedT = typename ::cereal::traits::strip_minimal<DecayedT>::type;
      public:
        static const bool value = std::is_enum<StrippedT>::value;
        using type = StrippedT;
        using base_type = typename enum_underlying_type<StrippedT, value>::type;
    };
  }
  template <class Archive, class T> inline
  typename std::enable_if<common_detail::is_enum<T>::value,
                          typename common_detail::is_enum<T>::base_type>::type
  CEREAL_SAVE_MINIMAL_FUNCTION_NAME( Archive const &, T const & t )
  {
    return static_cast<typename common_detail::is_enum<T>::base_type>(t);
  }
  template <class Archive, class T> inline
  typename std::enable_if<common_detail::is_enum<T>::value, void>::type
  CEREAL_LOAD_MINIMAL_FUNCTION_NAME( Archive const &, T && t,
                                     typename common_detail::is_enum<T>::base_type const & value )
  {
    t = reinterpret_cast<typename common_detail::is_enum<T>::type const &>( value );
  }
  template <class Archive, class T> inline
  void CEREAL_SERIALIZE_FUNCTION_NAME( Archive &, T * & )
  {
    static_assert(cereal::traits::detail::delay_static_assert<T>::value,
      "Cereal does not support serializing raw pointers - please use a smart pointer");
  }
  template <class Archive, class T> inline
  typename std::enable_if<std::is_array<T>::value, void>::type
  CEREAL_SERIALIZE_FUNCTION_NAME(Archive & ar, T & array)
  {
    common_detail::serializeArray( ar, array,
        std::integral_constant<bool, traits::is_output_serializable<BinaryData<T>, Archive>::value &&
                                     std::is_arithmetic<typename std::remove_all_extents<T>::type>::value>() );
  }
}
#endif
#endif
#include <array>
namespace cereal
{
  template <class Archive, class T, size_t N> inline
  typename std::enable_if<traits::is_output_serializable<BinaryData<T>, Archive>::value
                          && std::is_arithmetic<T>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::array<T, N> const & array )
  {
    ar( binary_data( array.data(), sizeof(array) ) );
  }
  template <class Archive, class T, size_t N> inline
  typename std::enable_if<traits::is_input_serializable<BinaryData<T>, Archive>::value
                          && std::is_arithmetic<T>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::array<T, N> & array )
  {
    ar( binary_data( array.data(), sizeof(array) ) );
  }
  template <class Archive, class T, size_t N> inline
  typename std::enable_if<!traits::is_output_serializable<BinaryData<T>, Archive>::value
                          || !std::is_arithmetic<T>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::array<T, N> const & array )
  {
    for( auto const & i : array )
      ar( i );
  }
  template <class Archive, class T, size_t N> inline
  typename std::enable_if<!traits::is_input_serializable<BinaryData<T>, Archive>::value
                          || !std::is_arithmetic<T>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::array<T, N> & array )
  {
    for( auto & i : array )
      ar( i );
  }
}
#endif
/*#include <cereal/types/base_class.hpp> skipped */
/*#include <cereal/types/bitset.hpp>*/
#ifndef CEREAL_TYPES_BITSET_HPP_
#define CEREAL_TYPES_BITSET_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <bitset>
namespace cereal
{
  namespace bitset_detail
  {
    enum class type : uint8_t
    {
      ulong,
      ullong,
      string
    };
  }
  template <class Archive, size_t N> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::bitset<N> const & bits )
  {
    try
    {
      auto const b = bits.to_ulong();
      ar( CEREAL_NVP_("type", bitset_detail::type::ulong) );
      ar( CEREAL_NVP_("data", b) );
    }
    catch( std::overflow_error const & )
    {
      try
      {
        auto const b = bits.to_ullong();
        ar( CEREAL_NVP_("type", bitset_detail::type::ullong) );
        ar( CEREAL_NVP_("data", b) );
      }
      catch( std::overflow_error const & )
      {
        ar( CEREAL_NVP_("type", bitset_detail::type::string) );
        ar( CEREAL_NVP_("data", bits.to_string()) );
      }
    }
  }
  template <class Archive, size_t N> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::bitset<N> & bits )
  {
    bitset_detail::type t;
    ar( CEREAL_NVP_("type", t) );
    switch( t )
    {
      case bitset_detail::type::ulong:
      {
        unsigned long b;
        ar( CEREAL_NVP_("data", b) );
        bits = std::bitset<N>( b );
        break;
      }
      case bitset_detail::type::ullong:
      {
        unsigned long long b;
        ar( CEREAL_NVP_("data", b) );
        bits = std::bitset<N>( b );
        break;
      }
      case bitset_detail::type::string:
      {
        std::string b;
        ar( CEREAL_NVP_("data", b) );
        bits = std::bitset<N>( b );
        break;
      }
      default:
        throw Exception("Invalid bitset data representation");
    }
  }
}
#endif
/*#include <cereal/types/chrono.hpp>*/
#ifndef CEREAL_TYPES_CHRONO_HPP_
#define CEREAL_TYPES_CHRONO_HPP_
#include <chrono>
namespace cereal
{
  template <class Archive, class R, class P> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::chrono::duration<R, P> const & dur )
  {
    ar( CEREAL_NVP_("count", dur.count()) );
  }
  template <class Archive, class R, class P> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::chrono::duration<R, P> & dur )
  {
    R count;
    ar( CEREAL_NVP_("count", count) );
    dur = std::chrono::duration<R, P>{count};
  }
  template <class Archive, class C, class D> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::chrono::time_point<C, D> const & dur )
  {
    ar( CEREAL_NVP_("time_since_epoch", dur.time_since_epoch()) );
  }
  template <class Archive, class C, class D> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::chrono::time_point<C, D> & dur )
  {
    D elapsed;
    ar( CEREAL_NVP_("time_since_epoch", elapsed) );
    dur = std::chrono::time_point<C, D>{elapsed};
  }
}
#endif
/*#include <cereal/types/common.hpp> skipped */
/*#include <cereal/types/complex.hpp>*/
#ifndef CEREAL_TYPES_COMPLEX_HPP_
#define CEREAL_TYPES_COMPLEX_HPP_
#include <complex>
namespace cereal
{
  template <class Archive, class T> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::complex<T> const & comp )
  {
    ar( CEREAL_NVP_("real", comp.real()),
        CEREAL_NVP_("imag", comp.imag()) );
  }
  template <class Archive, class T> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::complex<T> & bits )
  {
    T real, imag;
    ar( CEREAL_NVP_("real", real),
        CEREAL_NVP_("imag", imag) );
    bits = {real, imag};
  }
}
#endif
/*#include <cereal/types/deque.hpp>*/
#ifndef CEREAL_TYPES_DEQUE_HPP_
#define CEREAL_TYPES_DEQUE_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <deque>
namespace cereal
{
  template <class Archive, class T, class A> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::deque<T, A> const & deque )
  {
    ar( make_size_tag( static_cast<size_type>(deque.size()) ) );
    for( auto const & i : deque )
      ar( i );
  }
  template <class Archive, class T, class A> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::deque<T, A> & deque )
  {
    size_type size;
    ar( make_size_tag( size ) );
    deque.resize( static_cast<size_t>( size ) );
    for( auto & i : deque )
      ar( i );
  }
}
#endif
/*#include <cereal/types/forward_list.hpp>*/
#ifndef CEREAL_TYPES_FORWARD_LIST_HPP_
#define CEREAL_TYPES_FORWARD_LIST_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <forward_list>
namespace cereal
{
  template <class Archive, class T, class A> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::forward_list<T, A> const & forward_list )
  {
    size_type const size = std::distance( forward_list.begin(), forward_list.end() );
    ar( make_size_tag( size ) );
    for( const auto & i : forward_list )
      ar( i );
  }
  template <class Archive, class T, class A>
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::forward_list<T, A> & forward_list )
  {
    size_type size;
    ar( make_size_tag( size ) );
    forward_list.resize( static_cast<size_t>( size ) );
    for( auto & i : forward_list )
      ar( i );
  }
}
#endif
/*#include <cereal/types/list.hpp>*/
#ifndef CEREAL_TYPES_LIST_HPP_
#define CEREAL_TYPES_LIST_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <list>
namespace cereal
{
  template <class Archive, class T, class A> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::list<T, A> const & list )
  {
    ar( make_size_tag( static_cast<size_type>(list.size()) ) );
    for( auto const & i : list )
      ar( i );
  }
  template <class Archive, class T, class A> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::list<T, A> & list )
  {
    size_type size;
    ar( make_size_tag( size ) );
    list.resize( static_cast<size_t>( size ) );
    for( auto & i : list )
      ar( i );
  }
}
#endif
/*#include <cereal/types/map.hpp>*/
#ifndef CEREAL_TYPES_MAP_HPP_
#define CEREAL_TYPES_MAP_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <map>
namespace cereal
{
  namespace map_detail
  {
    template <class Archive, class MapT> inline
    void save( Archive & ar, MapT const & map )
    {
      ar( make_size_tag( static_cast<size_type>(map.size()) ) );
      for( const auto & i : map )
      {
        ar( make_map_item(i.first, i.second) );
      }
    }
    template <class Archive, class MapT> inline
    void load( Archive & ar, MapT & map )
    {
      size_type size;
      ar( make_size_tag( size ) );
      map.clear();
      auto hint = map.begin();
      for( size_t i = 0; i < size; ++i )
      {
        typename MapT::key_type key;
        typename MapT::mapped_type value;
        ar( make_map_item(key, value) );
        #ifdef CEREAL_OLDER_GCC
        hint = map.insert( hint, std::make_pair(std::move(key), std::move(value)) );
        #else
        hint = map.emplace_hint( hint, std::move( key ), std::move( value ) );
        #endif
      }
    }
  }
  template <class Archive, class K, class T, class C, class A> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::map<K, T, C, A> const & map )
  {
    map_detail::save( ar, map );
  }
  template <class Archive, class K, class T, class C, class A> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::map<K, T, C, A> & map )
  {
    map_detail::load( ar, map );
  }
  template <class Archive, class K, class T, class C, class A> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::multimap<K, T, C, A> const & multimap )
  {
    map_detail::save( ar, multimap );
  }
  template <class Archive, class K, class T, class C, class A> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::multimap<K, T, C, A> & multimap )
  {
    map_detail::load( ar, multimap );
  }
}
#endif
/*#include <cereal/types/memory.hpp>*/
#ifndef CEREAL_TYPES_SHARED_PTR_HPP_
#define CEREAL_TYPES_SHARED_PTR_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <memory>
#include <cstring>
namespace cereal
{
  namespace memory_detail
  {
    template<class T>
    struct PtrWrapper
    {
      PtrWrapper(T && p) : ptr(std::forward<T>(p)) {}
      T & ptr;
      PtrWrapper & operator=( PtrWrapper const & ) = delete;
    };
    template<class T> inline
    PtrWrapper<T> make_ptr_wrapper(T && t)
    {
      return {std::forward<T>(t)};
    }
    template <class Archive, class T>
    struct LoadAndConstructLoadWrapper
    {
      LoadAndConstructLoadWrapper( T * ptr ) :
        construct( ptr )
      { }
      inline void CEREAL_SERIALIZE_FUNCTION_NAME( Archive & ar )
      {
        ::cereal::detail::Construct<T, Archive>::load_andor_construct( ar, construct );
      }
      ::cereal::construct<T> construct;
    };
    template <class T>
    class EnableSharedStateHelper
    {
      using BaseType = typename ::cereal::traits::get_shared_from_this_base<T>::type;
      using ParentType = std::enable_shared_from_this<BaseType>;
      using StorageType = typename std::aligned_storage<sizeof(ParentType)>::type;
      public:
        inline EnableSharedStateHelper( T * ptr ) :
          itsPtr( static_cast<ParentType *>( ptr ) ),
          itsState()
        {
          std::memcpy( &itsState, itsPtr, sizeof(ParentType) );
        }
        inline ~EnableSharedStateHelper()
        {
          std::memcpy( itsPtr, &itsState, sizeof(ParentType) );
        }
      private:
        ParentType * itsPtr;
        StorageType itsState;
    };
    template <class Archive, class T> inline
    void loadAndConstructSharedPtr( Archive & ar, T * ptr, std::true_type  )
    {
      memory_detail::LoadAndConstructLoadWrapper<Archive, T> loadWrapper( ptr );
      memory_detail::EnableSharedStateHelper<T> state( ptr );
      ar( CEREAL_NVP_("data", loadWrapper) );
    }
    template <class Archive, class T> inline
    void loadAndConstructSharedPtr( Archive & ar, T * ptr, std::false_type  )
    {
      memory_detail::LoadAndConstructLoadWrapper<Archive, T> loadWrapper( ptr );
      ar( CEREAL_NVP_("data", loadWrapper) );
    }
  }
  template <class Archive, class T> inline
  typename std::enable_if<!std::is_polymorphic<T>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::shared_ptr<T> const & ptr )
  {
    ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper( ptr )) );
  }
  template <class Archive, class T> inline
  typename std::enable_if<!std::is_polymorphic<T>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::shared_ptr<T> & ptr )
  {
    ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper( ptr )) );
  }
  template <class Archive, class T> inline
  typename std::enable_if<!std::is_polymorphic<T>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::weak_ptr<T> const & ptr )
  {
    auto const sptr = ptr.lock();
    ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper( sptr )) );
  }
  template <class Archive, class T> inline
  typename std::enable_if<!std::is_polymorphic<T>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::weak_ptr<T> & ptr )
  {
    std::shared_ptr<T> sptr;
    ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper( sptr )) );
    ptr = sptr;
  }
  template <class Archive, class T, class D> inline
  typename std::enable_if<!std::is_polymorphic<T>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::unique_ptr<T, D> const & ptr )
  {
    ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper( ptr )) );
  }
  template <class Archive, class T, class D> inline
  typename std::enable_if<!std::is_polymorphic<T>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::unique_ptr<T, D> & ptr )
  {
    ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper( ptr )) );
  }
  template <class Archive, class T> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, memory_detail::PtrWrapper<std::shared_ptr<T> const &> const & wrapper )
  {
    auto & ptr = wrapper.ptr;
    uint32_t id = ar.registerSharedPointer( ptr.get() );
    ar( CEREAL_NVP_("id", id) );
    if( id & detail::msb_32bit )
    {
      ar( CEREAL_NVP_("data", *ptr) );
    }
  }
  template <class Archive, class T> inline
  typename std::enable_if<traits::has_load_and_construct<T, Archive>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, memory_detail::PtrWrapper<std::shared_ptr<T> &> & wrapper )
  {
    auto & ptr = wrapper.ptr;
    uint32_t id;
    ar( CEREAL_NVP_("id", id) );
    if( id & detail::msb_32bit )
    {
      using ST = typename std::aligned_storage<sizeof(T)>::type;
      auto valid = std::make_shared<bool>( false );
      ptr.reset( reinterpret_cast<T *>( new ST() ),
          [=]( T * t )
          {
            if( valid )
              t->~T();
            delete reinterpret_cast<ST *>( t );
          } );
      ar.registerSharedPointer( id, ptr );
      memory_detail::loadAndConstructSharedPtr( ar, ptr.get(), typename ::cereal::traits::has_shared_from_this<T>::type() );
      *valid = true;
    }
    else
      ptr = std::static_pointer_cast<T>(ar.getSharedPointer(id));
  }
  template <class Archive, class T> inline
  typename std::enable_if<!traits::has_load_and_construct<T, Archive>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, memory_detail::PtrWrapper<std::shared_ptr<T> &> & wrapper )
  {
    auto & ptr = wrapper.ptr;
    uint32_t id;
    ar( CEREAL_NVP_("id", id) );
    if( id & detail::msb_32bit )
    {
      ptr.reset( detail::Construct<T, Archive>::load_andor_construct() );
      ar.registerSharedPointer( id, ptr );
      ar( CEREAL_NVP_("data", *ptr) );
    }
    else
      ptr = std::static_pointer_cast<T>(ar.getSharedPointer(id));
  }
  template <class Archive, class T, class D> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, memory_detail::PtrWrapper<std::unique_ptr<T, D> const &> const & wrapper )
  {
    auto & ptr = wrapper.ptr;
    if( !ptr )
      ar( CEREAL_NVP_("valid", uint8_t(0)) );
    else
    {
      ar( CEREAL_NVP_("valid", uint8_t(1)) );
      ar( CEREAL_NVP_("data", *ptr) );
    }
  }
  template <class Archive, class T, class D> inline
  typename std::enable_if<traits::has_load_and_construct<T, Archive>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, memory_detail::PtrWrapper<std::unique_ptr<T, D> &> & wrapper )
  {
    uint8_t isValid;
    ar( CEREAL_NVP_("valid", isValid) );
    auto & ptr = wrapper.ptr;
    if( isValid )
    {
      using ST = typename std::aligned_storage<sizeof(T)>::type;
      std::unique_ptr<ST> stPtr( new ST() );
      memory_detail::LoadAndConstructLoadWrapper<Archive, T> loadWrapper( reinterpret_cast<T *>( stPtr.get() ) );
      ar( CEREAL_NVP_("data", loadWrapper) );
      ptr.reset( reinterpret_cast<T *>( stPtr.release() ) );
    }
    else
      ptr.reset( nullptr );
  }
  template <class Archive, class T, class D> inline
  typename std::enable_if<!traits::has_load_and_construct<T, Archive>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, memory_detail::PtrWrapper<std::unique_ptr<T, D> &> & wrapper )
  {
    uint8_t isValid;
    ar( CEREAL_NVP_("valid", isValid) );
    auto & ptr = wrapper.ptr;
    if( isValid )
    {
      ptr.reset( detail::Construct<T, Archive>::load_andor_construct() );
      ar( CEREAL_NVP_( "data", *ptr ) );
    }
    else
    {
      ptr.reset( nullptr );
    }
  }
}
#endif
/*#include <cereal/types/polymorphic.hpp>*/
#ifndef CEREAL_TYPES_POLYMORPHIC_HPP_
#define CEREAL_TYPES_POLYMORPHIC_HPP_
/*#include <cereal/cereal.hpp> skipped */
/*#include <cereal/types/memory.hpp> skipped */
/*#include <cereal/details/util.hpp>*/
#ifndef CEREAL_DETAILS_UTIL_HPP_
#define CEREAL_DETAILS_UTIL_HPP_
#include <typeinfo>
#include <string>
#ifdef _MSC_VER
namespace cereal
{
  namespace util
  {
    inline std::string demangle( std::string const & name )
    { return name; }
    template <class T> inline
    std::string demangledName()
    { return typeid( T ).name(); }
  }
}
#else
/*#include <cxxabi.h> skipped */
#include <cstdlib>
namespace cereal
{
  namespace util
  {
    inline std::string demangle(std::string mangledName)
    {
      int status = 0;
      char *demangledName = nullptr;
      std::size_t len;
      demangledName = abi::__cxa_demangle(mangledName.c_str(), 0, &len, &status);
      std::string retName(demangledName);
      free(demangledName);
      return retName;
    }
    template<class T> inline
    std::string demangledName()
    { return demangle(typeid(T).name()); }
  }
}
#endif
#endif
/*#include <cereal/details/helpers.hpp> skipped */
/*#include <cereal/details/traits.hpp> skipped */
/*#include <cereal/details/polymorphic_impl.hpp>*/
#ifndef CEREAL_DETAILS_POLYMORPHIC_IMPL_HPP_
#define CEREAL_DETAILS_POLYMORPHIC_IMPL_HPP_
/*#include <cereal/details/static_object.hpp> skipped */
/*#include <cereal/types/memory.hpp> skipped */
/*#include <cereal/types/string.hpp>*/
#ifndef CEREAL_TYPES_STRING_HPP_
#define CEREAL_TYPES_STRING_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <string>
namespace cereal
{
  template<class Archive, class CharT, class Traits, class Alloc> inline
  typename std::enable_if<traits::is_output_serializable<BinaryData<CharT>, Archive>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME(Archive & ar, std::basic_string<CharT, Traits, Alloc> const & str)
  {
    ar( make_size_tag( static_cast<size_type>(str.size()) ) );
    ar( binary_data( str.data(), str.size() * sizeof(CharT) ) );
  }
  template<class Archive, class CharT, class Traits, class Alloc> inline
  typename std::enable_if<traits::is_input_serializable<BinaryData<CharT>, Archive>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME(Archive & ar, std::basic_string<CharT, Traits, Alloc> & str)
  {
    size_type size;
    ar( make_size_tag( size ) );
    str.resize(static_cast<std::size_t>(size));
    ar( binary_data( const_cast<CharT *>( str.data() ), static_cast<std::size_t>(size) * sizeof(CharT) ) );
  }
}
#endif
#include <functional>
#include <typeindex>
#include <map>
#define CEREAL_BIND_TO_ARCHIVES(T)                   \
    namespace cereal {                               \
    namespace detail {                               \
    template<>                                       \
    struct init_binding<T> {                         \
        static bind_to_archives<T> const & b;        \
        static void unused() { (void)b; }            \
    };                                               \
    bind_to_archives<T> const & init_binding<T>::b = \
        ::cereal::detail::StaticObject<              \
            bind_to_archives<T>                      \
        >::getInstance().bind();                     \
    }}
namespace cereal
{
  namespace detail
  {
    template <class T>
    struct binding_name {};
    template <class Archive>
    struct OutputBindingMap
    {
      typedef std::function<void(void*, void const *)> Serializer;
      struct Serializers
      {
        Serializer shared_ptr,
                   unique_ptr;
      };
      std::map<std::type_index, Serializers> map;
    };
    template<class T> struct EmptyDeleter { void operator()(T *) const {} };
    template <class Archive>
    struct InputBindingMap
    {
      typedef std::function<void(void*, std::shared_ptr<void> & )> SharedSerializer;
      typedef std::function<void(void*, std::unique_ptr<void, EmptyDeleter<void>> & )> UniqueSerializer;
      struct Serializers
      {
        SharedSerializer shared_ptr;
        UniqueSerializer unique_ptr;
      };
      std::map<std::string, Serializers> map;
    };
    class InputArchiveBase;
    class OutputArchiveBase;
    template <class Archive, class T> struct InputBindingCreator
    {
      InputBindingCreator()
      {
        auto & map = StaticObject<InputBindingMap<Archive>>::getInstance().map;
        auto key = std::string(binding_name<T>::name());
        auto lb = map.lower_bound(key);
        if (lb != map.end() && lb->first == key)
          return;
        typename InputBindingMap<Archive>::Serializers serializers;
        serializers.shared_ptr =
          [](void * arptr, std::shared_ptr<void> & dptr)
          {
            Archive & ar = *static_cast<Archive*>(arptr);
            std::shared_ptr<T> ptr;
            ar( CEREAL_NVP_("ptr_wrapper", ::cereal::memory_detail::make_ptr_wrapper(ptr)) );
            dptr = ptr;
          };
        serializers.unique_ptr =
          [](void * arptr, std::unique_ptr<void, EmptyDeleter<void>> & dptr)
          {
            Archive & ar = *static_cast<Archive*>(arptr);
            std::unique_ptr<T> ptr;
            ar( CEREAL_NVP_("ptr_wrapper", ::cereal::memory_detail::make_ptr_wrapper(ptr)) );
            dptr.reset(ptr.release());
          };
        map.insert( lb, { std::move(key), std::move(serializers) } );
      }
    };
    template <class Archive, class T> struct OutputBindingCreator
    {
      static void writeMetadata(Archive & ar)
      {
        char const * name = binding_name<T>::name();
        std::uint32_t id = ar.registerPolymorphicType(name);
        ar( CEREAL_NVP_("polymorphic_id", id) );
        if( id & detail::msb_32bit )
        {
          std::string namestring(name);
          ar( CEREAL_NVP_("polymorphic_name", namestring) );
        }
      }
      class PolymorphicSharedPointerWrapper
      {
        public:
          PolymorphicSharedPointerWrapper( void const * dptr ) : refCount()
          {
            #ifdef _LIBCPP_VERSION
            wrappedPtr = std::shared_ptr<T const>(
                std::const_pointer_cast<T const>(
                  std::shared_ptr<T>( refCount, static_cast<T *>(const_cast<void *>(dptr) ))));
            #else
            wrappedPtr = std::shared_ptr<T const>( refCount, static_cast<T const *>(dptr) );
            #endif
          }
          inline std::shared_ptr<T const> const & operator()() const { return wrappedPtr; }
        private:
          std::shared_ptr<void> refCount;
          std::shared_ptr<T const> wrappedPtr;
      };
      static inline void savePolymorphicSharedPtr( Archive & ar, void const * dptr, std::true_type  )
      {
        ::cereal::memory_detail::EnableSharedStateHelper<T> state( static_cast<T *>(const_cast<void *>(dptr)) );
        PolymorphicSharedPointerWrapper psptr( dptr );
        ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper( psptr() ) ) );
      }
      static inline void savePolymorphicSharedPtr( Archive & ar, void const * dptr, std::false_type  )
      {
        PolymorphicSharedPointerWrapper psptr( dptr );
        ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper( psptr() ) ) );
      }
      OutputBindingCreator()
      {
        auto & map = StaticObject<OutputBindingMap<Archive>>::getInstance().map;
        auto key = std::type_index(typeid(T));
        auto lb = map.lower_bound(key);
        if (lb != map.end() && lb->first == key)
          return;
        typename OutputBindingMap<Archive>::Serializers serializers;
        serializers.shared_ptr =
          [&](void * arptr, void const * dptr)
          {
            Archive & ar = *static_cast<Archive*>(arptr);
            writeMetadata(ar);
            #ifdef _MSC_VER
            savePolymorphicSharedPtr( ar, dptr, ::cereal::traits::has_shared_from_this<T>::type() );
            #else
            savePolymorphicSharedPtr( ar, dptr, typename ::cereal::traits::has_shared_from_this<T>::type() );
            #endif
          };
        serializers.unique_ptr =
          [&](void * arptr, void const * dptr)
          {
            Archive & ar = *static_cast<Archive*>(arptr);
            writeMetadata(ar);
            std::unique_ptr<T const, EmptyDeleter<T const>> const ptr(static_cast<T const *>(dptr));
            ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper(ptr)) );
          };
        map.insert( { std::move(key), std::move(serializers) } );
      }
    };
    struct adl_tag {};
    namespace { struct polymorphic_binding_tag {}; }
    template <class Archive, class T>
    struct create_bindings
    {
      static const InputBindingCreator<Archive, T> &
      load(std::true_type)
      {
        return cereal::detail::StaticObject<InputBindingCreator<Archive, T>>::getInstance();
      }
      static const OutputBindingCreator<Archive, T> &
      save(std::true_type)
      {
        return cereal::detail::StaticObject<OutputBindingCreator<Archive, T>>::getInstance();
      }
      inline static void load(std::false_type) {}
      inline static void save(std::false_type) {}
    };
    template <void(*)()>
    struct instantiate_function {};
    template <class Archive, class T>
    struct polymorphic_serialization_support
    {
      #if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
      virtual CEREAL_DLL_EXPORT void instantiate() CEREAL_USED;
      #else
      static CEREAL_DLL_EXPORT void instantiate() CEREAL_USED;
      typedef instantiate_function<instantiate> unused;
      #endif
    };
    template <class Archive, class T>
    CEREAL_DLL_EXPORT void polymorphic_serialization_support<Archive,T>::instantiate()
    {
      create_bindings<Archive,T>::save( std::integral_constant<bool,
                                          std::is_base_of<detail::OutputArchiveBase, Archive>::value &&
                                          traits::is_output_serializable<T, Archive>::value>{} );
      create_bindings<Archive,T>::load( std::integral_constant<bool,
                                          std::is_base_of<detail::InputArchiveBase, Archive>::value &&
                                          traits::is_input_serializable<T, Archive>::value>{} );
    }
    template <class T, class Tag = polymorphic_binding_tag>
    struct bind_to_archives
    {
      void bind(std::false_type) const
	    {
		    instantiate_polymorphic_binding((T*) 0, 0, Tag{}, adl_tag{});
      }
      void bind(std::true_type) const
      { }
      bind_to_archives const & bind() const
      {
        static_assert( std::is_polymorphic<T>::value,
                       "Attempting to register non polymorphic type" );
        bind( std::is_abstract<T>() );
        return *this;
      }
    };
    template <class T, class Tag = polymorphic_binding_tag>
    struct init_binding;
    template <class T, typename BindingTag>
    void instantiate_polymorphic_binding( T*, int, BindingTag, adl_tag ) {}
  }
}
#endif
#ifdef _MSC_VER
#define STATIC_CONSTEXPR static
#else
#define STATIC_CONSTEXPR static constexpr
#endif
#define CEREAL_REGISTER_TYPE(T)                         \
  namespace cereal {                                    \
  namespace detail {                                    \
  template <>                                           \
  struct binding_name<T>                                \
  {                                                     \
    STATIC_CONSTEXPR char const * name() { return #T; } \
  };                                                    \
  } }                               \
  CEREAL_BIND_TO_ARCHIVES(T)
#define CEREAL_REGISTER_TYPE_WITH_NAME(T, Name)              \
  namespace cereal {                                         \
  namespace detail {                                         \
  template <>                                                \
  struct binding_name<T>                                     \
  { STATIC_CONSTEXPR char const * name() { return Name; } }; \
  } }                                    \
  CEREAL_BIND_TO_ARCHIVES(T)
#define CEREAL_REGISTER_DYNAMIC_INIT(LibName)                \
  namespace cereal {                                         \
  namespace detail {                                         \
    void CEREAL_DLL_EXPORT dynamic_init_dummy_##LibName() {} \
  } }
#define CEREAL_FORCE_DYNAMIC_INIT(LibName)              \
  namespace cereal {                                    \
  namespace detail {                                    \
    void dynamic_init_dummy_##LibName();                \
  }                                     \
  namespace {                                           \
    void dynamic_init_##LibName()                       \
    {                                                   \
      ::cereal::detail::dynamic_init_dummy_##LibName(); \
    }                                                   \
  } }
#ifdef _MSC_VER
#undef CONSTEXPR
#endif
namespace cereal
{
  namespace polymorphic_detail
  {
    #define UNREGISTERED_POLYMORPHIC_EXCEPTION(LoadSave, Name)                                                                                      \
      throw cereal::Exception("Trying to " #LoadSave " an unregistered polymorphic type (" + Name + ").\n"                                          \
                              "Make sure your type is registered with CEREAL_REGISTER_TYPE and that the archive "                                   \
                              "you are using was included (and registered with CEREAL_REGISTER_ARCHIVE) prior to calling CEREAL_REGISTER_TYPE.\n"   \
                              "If your type is already registered and you still see this error, you may need to use CEREAL_REGISTER_DYNAMIC_INIT.");
    template<class Archive> inline
    typename ::cereal::detail::InputBindingMap<Archive>::Serializers getInputBinding(Archive & ar, std::uint32_t const nameid)
    {
      if(nameid == 0)
      {
        typename ::cereal::detail::InputBindingMap<Archive>::Serializers emptySerializers;
        emptySerializers.shared_ptr = [](void*, std::shared_ptr<void> & ptr) { ptr.reset(); };
        emptySerializers.unique_ptr = [](void*, std::unique_ptr<void, ::cereal::detail::EmptyDeleter<void>> & ptr) { ptr.reset( nullptr ); };
        return emptySerializers;
      }
      std::string name;
      if(nameid & detail::msb_32bit)
      {
        ar( CEREAL_NVP_("polymorphic_name", name) );
        ar.registerPolymorphicName(nameid, name);
      }
      else
        name = ar.getPolymorphicName(nameid);
      auto & bindingMap = detail::StaticObject<detail::InputBindingMap<Archive>>::getInstance().map;
      auto binding = bindingMap.find(name);
      if(binding == bindingMap.end())
        UNREGISTERED_POLYMORPHIC_EXCEPTION(load, name)
      return binding->second;
    }
    template<class Archive, class T> inline
    typename std::enable_if<(traits::is_default_constructible<T>::value
                             || traits::has_load_and_construct<T, Archive>::value)
                             && !std::is_abstract<T>::value, bool>::type
    serialize_wrapper(Archive & ar, std::shared_ptr<T> & ptr, std::uint32_t const nameid)
    {
      if(nameid & detail::msb2_32bit)
      {
        ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper(ptr)) );
        return true;
      }
      return false;
    }
    template<class Archive, class T, class D> inline
    typename std::enable_if<(traits::is_default_constructible<T>::value
                             || traits::has_load_and_construct<T, Archive>::value)
                             && !std::is_abstract<T>::value, bool>::type
    serialize_wrapper(Archive & ar, std::unique_ptr<T, D> & ptr, std::uint32_t const nameid)
    {
      if(nameid & detail::msb2_32bit)
      {
        ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper(ptr)) );
        return true;
      }
      return false;
    }
    template<class Archive, class T> inline
    typename std::enable_if<(!traits::is_default_constructible<T>::value
                             && !traits::has_load_and_construct<T, Archive>::value)
                             || std::is_abstract<T>::value, bool>::type
    serialize_wrapper(Archive &, std::shared_ptr<T> &, std::uint32_t const nameid)
    {
      if(nameid & detail::msb2_32bit)
        throw cereal::Exception("Cannot load a polymorphic type that is not default constructable and does not have a load_and_construct function");
      return false;
    }
    template<class Archive, class T, class D> inline
     typename std::enable_if<(!traits::is_default_constructible<T>::value
                               && !traits::has_load_and_construct<T, Archive>::value)
                               || std::is_abstract<T>::value, bool>::type
    serialize_wrapper(Archive &, std::unique_ptr<T, D> &, std::uint32_t const nameid)
    {
      if(nameid & detail::msb2_32bit)
        throw cereal::Exception("Cannot load a polymorphic type that is not default constructable and does not have a load_and_construct function");
      return false;
    }
  }
  template <class Archive, class T> inline
  typename std::enable_if<std::is_polymorphic<T>::value && std::is_abstract<T>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::shared_ptr<T> const & ptr )
  {
    if(!ptr)
    {
      ar( CEREAL_NVP_("polymorphic_id", std::uint32_t(0)) );
      return;
    }
    std::type_info const & ptrinfo = typeid(*ptr.get());
    auto & bindingMap = detail::StaticObject<detail::OutputBindingMap<Archive>>::getInstance().map;
    auto binding = bindingMap.find(std::type_index(ptrinfo));
    if(binding == bindingMap.end())
      UNREGISTERED_POLYMORPHIC_EXCEPTION(save, cereal::util::demangle(ptrinfo.name()))
    binding->second.shared_ptr(&ar, ptr.get());
  }
  template <class Archive, class T> inline
  typename std::enable_if<std::is_polymorphic<T>::value && !std::is_abstract<T>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::shared_ptr<T> const & ptr )
  {
    if(!ptr)
    {
      ar( CEREAL_NVP_("polymorphic_id", std::uint32_t(0)) );
      return;
    }
    std::type_info const & ptrinfo = typeid(*ptr.get());
    static std::type_info const & tinfo = typeid(T);
    if(ptrinfo == tinfo)
    {
      ar( CEREAL_NVP_("polymorphic_id", detail::msb2_32bit) );
      ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper(ptr)) );
      return;
    }
    auto & bindingMap = detail::StaticObject<detail::OutputBindingMap<Archive>>::getInstance().map;
    auto binding = bindingMap.find(std::type_index(ptrinfo));
    if(binding == bindingMap.end())
      UNREGISTERED_POLYMORPHIC_EXCEPTION(save, cereal::util::demangle(ptrinfo.name()))
    binding->second.shared_ptr(&ar, ptr.get());
  }
  template <class Archive, class T> inline
  typename std::enable_if<std::is_polymorphic<T>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::shared_ptr<T> & ptr )
  {
    std::uint32_t nameid;
    ar( CEREAL_NVP_("polymorphic_id", nameid) );
    if(polymorphic_detail::serialize_wrapper(ar, ptr, nameid))
      return;
    auto binding = polymorphic_detail::getInputBinding(ar, nameid);
    std::shared_ptr<void> result;
    binding.shared_ptr(&ar, result);
    ptr = std::static_pointer_cast<T>(result);
  }
  template <class Archive, class T> inline
  typename std::enable_if<std::is_polymorphic<T>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::weak_ptr<T> const & ptr )
  {
    auto const sptr = ptr.lock();
    ar( CEREAL_NVP_("locked_ptr", sptr) );
  }
  template <class Archive, class T> inline
  typename std::enable_if<std::is_polymorphic<T>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::weak_ptr<T> & ptr )
  {
    std::shared_ptr<T> sptr;
    ar( CEREAL_NVP_("locked_ptr", sptr) );
    ptr = sptr;
  }
  template <class Archive, class T, class D> inline
  typename std::enable_if<std::is_polymorphic<T>::value && std::is_abstract<T>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::unique_ptr<T, D> const & ptr )
  {
    if(!ptr)
    {
      ar( CEREAL_NVP_("polymorphic_id", std::uint32_t(0)) );
      return;
    }
    std::type_info const & ptrinfo = typeid(*ptr.get());
    auto & bindingMap = detail::StaticObject<detail::OutputBindingMap<Archive>>::getInstance().map;
    auto binding = bindingMap.find(std::type_index(ptrinfo));
    if(binding == bindingMap.end())
      UNREGISTERED_POLYMORPHIC_EXCEPTION(save, cereal::util::demangle(ptrinfo.name()))
    binding->second.unique_ptr(&ar, ptr.get());
  }
  template <class Archive, class T, class D> inline
  typename std::enable_if<std::is_polymorphic<T>::value && !std::is_abstract<T>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::unique_ptr<T, D> const & ptr )
  {
    if(!ptr)
    {
      ar( CEREAL_NVP_("polymorphic_id", std::uint32_t(0)) );
      return;
    }
    std::type_info const & ptrinfo = typeid(*ptr.get());
    static std::type_info const & tinfo = typeid(T);
    if(ptrinfo == tinfo)
    {
      ar( CEREAL_NVP_("polymorphic_id", detail::msb2_32bit) );
      ar( CEREAL_NVP_("ptr_wrapper", memory_detail::make_ptr_wrapper(ptr)) );
      return;
    }
    auto & bindingMap = detail::StaticObject<detail::OutputBindingMap<Archive>>::getInstance().map;
    auto binding = bindingMap.find(std::type_index(ptrinfo));
    if(binding == bindingMap.end())
      UNREGISTERED_POLYMORPHIC_EXCEPTION(save, cereal::util::demangle(ptrinfo.name()))
    binding->second.unique_ptr(&ar, ptr.get());
  }
  template <class Archive, class T, class D> inline
  typename std::enable_if<std::is_polymorphic<T>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::unique_ptr<T, D> & ptr )
  {
    std::uint32_t nameid;
    ar( CEREAL_NVP_("polymorphic_id", nameid) );
    if(polymorphic_detail::serialize_wrapper(ar, ptr, nameid))
      return;
    auto binding = polymorphic_detail::getInputBinding(ar, nameid);
    std::unique_ptr<void, ::cereal::detail::EmptyDeleter<void>> result;
    binding.unique_ptr(&ar, result);
    ptr.reset(static_cast<T*>(result.release()));
  }
  #undef UNREGISTERED_POLYMORPHIC_EXCEPTION
}
#endif
/*#include <cereal/types/queue.hpp>*/
#ifndef CEREAL_TYPES_QUEUE_HPP_
#define CEREAL_TYPES_QUEUE_HPP_
/*#include <cereal/details/helpers.hpp> skipped */
#include <queue>
/*#include <cereal/types/deque.hpp> skipped */
namespace cereal
{
  namespace queue_detail
  {
    template <class T, class C> inline
    C const & container( std::queue<T, C> const & queue )
    {
      struct H : public std::queue<T, C>
      {
        static C const & get( std::queue<T, C> const & q )
        {
          return q.*(&H::c);
        }
      };
      return H::get( queue );
    }
    template <class T, class C, class Comp> inline
    C const & container( std::priority_queue<T, C, Comp> const & priority_queue )
    {
      struct H : public std::priority_queue<T, C, Comp>
      {
        static C const & get( std::priority_queue<T, C, Comp> const & pq )
        {
          return pq.*(&H::c);
        }
      };
      return H::get( priority_queue );
    }
    template <class T, class C, class Comp> inline
    Comp const & comparator( std::priority_queue<T, C, Comp> const & priority_queue )
    {
      struct H : public std::priority_queue<T, C, Comp>
      {
        static Comp const & get( std::priority_queue<T, C, Comp> const & pq )
        {
          return pq.*(&H::comp);
        }
      };
      return H::get( priority_queue );
    }
  }
  template <class Archive, class T, class C> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::queue<T, C> const & queue )
  {
    ar( CEREAL_NVP_("container", queue_detail::container( queue )) );
  }
  template <class Archive, class T, class C> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::queue<T, C> & queue )
  {
    C container;
    ar( CEREAL_NVP_("container", container) );
    queue = std::queue<T, C>( std::move( container ) );
  }
  template <class Archive, class T, class C, class Comp> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::priority_queue<T, C, Comp> const & priority_queue )
  {
    ar( CEREAL_NVP_("comparator", queue_detail::comparator( priority_queue )) );
    ar( CEREAL_NVP_("container", queue_detail::container( priority_queue )) );
  }
  template <class Archive, class T, class C, class Comp> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::priority_queue<T, C, Comp> & priority_queue )
  {
    Comp comparator;
    ar( CEREAL_NVP_("comparator", comparator) );
    C container;
    ar( CEREAL_NVP_("container", container) );
    priority_queue = std::priority_queue<T, C, Comp>( comparator, std::move( container ) );
  }
}
#endif
/*#include <cereal/types/set.hpp>*/
#ifndef CEREAL_TYPES_SET_HPP_
#define CEREAL_TYPES_SET_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <set>
namespace cereal
{
  namespace set_detail
  {
    template <class Archive, class SetT> inline
    void save( Archive & ar, SetT const & set )
    {
      ar( make_size_tag( static_cast<size_type>(set.size()) ) );
      for( const auto & i : set )
        ar( i );
    }
    template <class Archive, class SetT> inline
    void load( Archive & ar, SetT & set )
    {
      size_type size;
      ar( make_size_tag( size ) );
      set.clear();
      auto hint = set.begin();
      for( size_type i = 0; i < size; ++i )
      {
        typename SetT::key_type key;
        ar( key );
        #ifdef CEREAL_OLDER_GCC
        hint = set.insert( hint, std::move( key ) );
        #else
        hint = set.emplace_hint( hint, std::move( key ) );
        #endif
      }
    }
  }
  template <class Archive, class K, class C, class A> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::set<K, C, A> const & set )
  {
    set_detail::save( ar, set );
  }
  template <class Archive, class K, class C, class A> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::set<K, C, A> & set )
  {
    set_detail::load( ar, set );
  }
  template <class Archive, class K, class C, class A> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::multiset<K, C, A> const & multiset )
  {
    set_detail::save( ar, multiset );
  }
  template <class Archive, class K, class C, class A> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::multiset<K, C, A> & multiset )
  {
    set_detail::load( ar, multiset );
  }
}
#endif
/*#include <cereal/types/stack.hpp>*/
#ifndef CEREAL_TYPES_STACK_HPP_
#define CEREAL_TYPES_STACK_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <stack>
/*#include <cereal/types/deque.hpp> skipped */
namespace cereal
{
  namespace stack_detail
  {
    template <class T, class C> inline
    C const & container( std::stack<T, C> const & stack )
    {
      struct H : public std::stack<T, C>
      {
        static C const & get( std::stack<T, C> const & s )
        {
          return s.*(&H::c);
        }
      };
      return H::get( stack );
    }
  }
  template <class Archive, class T, class C> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::stack<T, C> const & stack )
  {
    ar( CEREAL_NVP_("container", stack_detail::container( stack )) );
  }
  template <class Archive, class T, class C> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::stack<T, C> & stack )
  {
    C container;
    ar( CEREAL_NVP_("container", container) );
    stack = std::stack<T, C>( std::move( container ) );
  }
}
#endif
/*#include <cereal/types/string.hpp> skipped */
/*#include <cereal/types/tuple.hpp>*/
#ifndef CEREAL_TYPES_TUPLE_HPP_
#define CEREAL_TYPES_TUPLE_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <tuple>
namespace cereal
{
  namespace tuple_detail
  {
    template<char...Cs>
    struct char_seq_to_c_str
    {
      static const int size = 14;
      typedef const char (&arr_type)[sizeof...(Cs) + size];
      static const char str[sizeof...(Cs) + size];
    };
    template<char...Cs>
    const char char_seq_to_c_str<Cs...>::str[sizeof...(Cs) + size] =
      {'t','u','p','l','e','_','e','l','e','m','e','n','t', Cs..., '\0'};
    template <size_t Q, size_t R, char ... C>
    struct to_string_impl
    {
      using type = typename to_string_impl<Q/10, Q%10, R+'0', C...>::type;
    };
    template <size_t R, char ... C>
    struct to_string_impl<0, R, C...>
    {
      using type = char_seq_to_c_str<R+'0', C...>;
    };
    template<size_t T>
    struct tuple_element_name
    {
      using type = typename to_string_impl<T/10, T%10>::type;
      static const typename type::arr_type c_str(){ return type::str; };
    };
    template <size_t Height>
    struct serialize
    {
      template <class Archive, class ... Types> inline
      static void apply( Archive & ar, std::tuple<Types...> & tuple )
      {
        serialize<Height - 1>::template apply( ar, tuple );
        ar( CEREAL_NVP_(tuple_element_name<Height - 1>::c_str(),
            std::get<Height - 1>( tuple )) );
      }
    };
    template <>
    struct serialize<0>
    {
      template <class Archive, class ... Types> inline
      static void apply( Archive &, std::tuple<Types...> & )
      { }
    };
  }
  template <class Archive, class ... Types> inline
  void CEREAL_SERIALIZE_FUNCTION_NAME( Archive & ar, std::tuple<Types...> & tuple )
  {
    tuple_detail::serialize<std::tuple_size<std::tuple<Types...>>::value>::template apply( ar, tuple );
  }
}
#endif
/*#include <cereal/types/unordered_map.hpp>*/
#ifndef CEREAL_TYPES_UNORDERED_MAP_HPP_
#define CEREAL_TYPES_UNORDERED_MAP_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <unordered_map>
namespace cereal
{
  namespace unordered_map_detail
  {
    template <class Archive, class MapT> inline
    void save( Archive & ar, MapT const & map )
    {
      ar( make_size_tag( static_cast<size_type>(map.size()) ) );
      for( const auto & i : map )
        ar( make_map_item(i.first, i.second) );
    }
    template <class Archive, class MapT> inline
    void load( Archive & ar, MapT & map )
    {
      size_type size;
      ar( make_size_tag( size ) );
      map.clear();
      map.reserve( static_cast<std::size_t>( size ) );
      for( size_type i = 0; i < size; ++i )
      {
        typename MapT::key_type key;
        typename MapT::mapped_type value;
        ar( make_map_item(key, value) );
        map.emplace( std::move( key ), std::move( value ) );
      }
    }
  }
  template <class Archive, class K, class T, class H, class KE, class A> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::unordered_map<K, T, H, KE, A> const & unordered_map )
  {
    unordered_map_detail::save( ar, unordered_map );
  }
  template <class Archive, class K, class T, class H, class KE, class A> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::unordered_map<K, T, H, KE, A> & unordered_map )
  {
    unordered_map_detail::load( ar, unordered_map );
  }
  template <class Archive, class K, class T, class H, class KE, class A> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::unordered_multimap<K, T, H, KE, A> const & unordered_multimap )
  {
    unordered_map_detail::save( ar, unordered_multimap );
  }
  template <class Archive, class K, class T, class H, class KE, class A> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::unordered_multimap<K, T, H, KE, A> & unordered_multimap )
  {
    unordered_map_detail::load( ar, unordered_multimap );
  }
}
#endif
/*#include <cereal/types/unordered_set.hpp>*/
#ifndef CEREAL_TYPES_UNORDERED_SET_HPP_
#define CEREAL_TYPES_UNORDERED_SET_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <unordered_set>
namespace cereal
{
  namespace unordered_set_detail
  {
    template <class Archive, class SetT> inline
    void save( Archive & ar, SetT const & set )
    {
      ar( make_size_tag( static_cast<size_type>(set.size()) ) );
      for( const auto & i : set )
        ar( i );
    }
    template <class Archive, class SetT> inline
    void load( Archive & ar, SetT & set )
    {
      size_type size;
      ar( make_size_tag( size ) );
      set.clear();
      set.reserve( static_cast<std::size_t>( size ) );
      for( size_type i = 0; i < size; ++i )
      {
        typename SetT::key_type key;
        ar( key );
        set.emplace( std::move( key ) );
      }
    }
  }
  template <class Archive, class K, class H, class KE, class A> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::unordered_set<K, H, KE, A> const & unordered_set )
  {
    unordered_set_detail::save( ar, unordered_set );
  }
  template <class Archive, class K, class H, class KE, class A> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::unordered_set<K, H, KE, A> & unordered_set )
  {
    unordered_set_detail::load( ar, unordered_set );
  }
  template <class Archive, class K, class H, class KE, class A> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::unordered_multiset<K, H, KE, A> const & unordered_multiset )
  {
    unordered_set_detail::save( ar, unordered_multiset );
  }
  template <class Archive, class K, class H, class KE, class A> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::unordered_multiset<K, H, KE, A> & unordered_multiset )
  {
    unordered_set_detail::load( ar, unordered_multiset );
  }
}
#endif
/*#include <cereal/types/utility.hpp>*/
#ifndef CEREAL_TYPES_UTILITY_HPP_
#define CEREAL_TYPES_UTILITY_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <utility>
namespace cereal
{
  template <class Archive, class T1, class T2> inline
  void CEREAL_SERIALIZE_FUNCTION_NAME( Archive & ar, std::pair<T1, T2> & pair )
  {
    ar( CEREAL_NVP_("first",  pair.first),
        CEREAL_NVP_("second", pair.second) );
  }
}
#endif
/*#include <cereal/types/valarray.hpp>*/
#ifndef CEREAL_TYPES_VALARRAY_HPP_
#define CEREAL_TYPES_VALARRAY_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <valarray>
namespace cereal
{
  template <class Archive, class T> inline
  typename std::enable_if<traits::is_output_serializable<BinaryData<T>, Archive>::value
                          && std::is_arithmetic<T>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::valarray<T> const & valarray )
  {
    ar( make_size_tag( static_cast<size_type>(valarray.size()) ) );
    ar( binary_data( &valarray[0], valarray.size() * sizeof(T) ) );
  }
  template <class Archive, class T> inline
  typename std::enable_if<traits::is_input_serializable<BinaryData<T>, Archive>::value
                          && std::is_arithmetic<T>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::valarray<T> & valarray )
  {
    size_type valarraySize;
    ar( make_size_tag( valarraySize ) );
    valarray.resize( static_cast<std::size_t>( valarraySize ) );
    ar( binary_data( &valarray[0], static_cast<std::size_t>( valarraySize ) * sizeof(T) ) );
  }
  template <class Archive, class T> inline
  typename std::enable_if<!traits::is_output_serializable<BinaryData<T>, Archive>::value
                          || !std::is_arithmetic<T>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::valarray<T> const & valarray )
  {
    ar( make_size_tag( static_cast<size_type>(valarray.size()) ) );
    for(auto && v : valarray)
      ar(v);
  }
  template <class Archive, class T> inline
  typename std::enable_if<!traits::is_input_serializable<BinaryData<T>, Archive>::value
                          || !std::is_arithmetic<T>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::valarray<T> & valarray )
  {
    size_type valarraySize;
    ar( make_size_tag( valarraySize ) );
    valarray.resize( static_cast<size_t>( valarraySize ) );
    for(auto && v : valarray)
      ar(v);
  }
}
#endif
/*#include <cereal/types/vector.hpp>*/
#ifndef CEREAL_TYPES_VECTOR_HPP_
#define CEREAL_TYPES_VECTOR_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <vector>
namespace cereal
{
  template <class Archive, class T, class A> inline
  typename std::enable_if<traits::is_output_serializable<BinaryData<T>, Archive>::value
                          && std::is_arithmetic<T>::value && !std::is_same<T, bool>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::vector<T, A> const & vector )
  {
    ar( make_size_tag( static_cast<size_type>(vector.size()) ) );
    ar( binary_data( vector.data(), vector.size() * sizeof(T) ) );
  }
  template <class Archive, class T, class A> inline
  typename std::enable_if<traits::is_input_serializable<BinaryData<T>, Archive>::value
                          && std::is_arithmetic<T>::value && !std::is_same<T, bool>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::vector<T, A> & vector )
  {
    size_type vectorSize;
    ar( make_size_tag( vectorSize ) );
    vector.resize( static_cast<std::size_t>( vectorSize ) );
    ar( binary_data( vector.data(), static_cast<std::size_t>( vectorSize ) * sizeof(T) ) );
  }
  template <class Archive, class T, class A> inline
  typename std::enable_if<!traits::is_output_serializable<BinaryData<T>, Archive>::value
                          || !std::is_arithmetic<T>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::vector<T, A> const & vector )
  {
    ar( make_size_tag( static_cast<size_type>(vector.size()) ) );
    for(auto && v : vector)
      ar( v );
  }
  template <class Archive, class T, class A> inline
  typename std::enable_if<!traits::is_input_serializable<BinaryData<T>, Archive>::value
                          || !std::is_arithmetic<T>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::vector<T, A> & vector )
  {
    size_type size;
    ar( make_size_tag( size ) );
    vector.resize( static_cast<std::size_t>( size ) );
    for(auto && v : vector)
      ar( v );
  }
  template <class Archive, class A> inline
  void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::vector<bool, A> const & vector )
  {
    ar( make_size_tag( static_cast<size_type>(vector.size()) ) );
    for(auto && v : vector)
      ar( static_cast<bool>(v) );
  }
  template <class Archive, class A> inline
  void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::vector<bool, A> & vector )
  {
    size_type size;
    ar( make_size_tag( size ) );
    vector.resize( static_cast<std::size_t>( size ) );
    for(auto && v : vector)
    {
      bool b;
      ar( b );
      v = b;
    }
  }
}
#endif
/*#include <cereal/archives/adapters.hpp>*/
#ifndef CEREAL_ARCHIVES_ADAPTERS_HPP_
#define CEREAL_ARCHIVES_ADAPTERS_HPP_
/*#include <cereal/details/helpers.hpp> skipped */
#include <utility>
namespace cereal
{
  #ifdef CEREAL_FUTURE_EXPERIMENTAL
  template <class U, class A> U & get_user_data( A & );
  template <class UserData, class Archive>
  class UserDataAdapter : public Archive
  {
    public:
      template <class ... Args>
      UserDataAdapter( UserData & ud, Args && ... args ) :
        Archive( std::forward<Args>( args )... ),
        userdata( ud )
      { }
    private:
      void rtti() {}
      friend UserData & get_user_data<UserData>( Archive & ar );
      UserData & userdata;
  };
  template <class UserData, class Archive>
  UserData & get_user_data( Archive & ar )
  {
    try
    {
      return dynamic_cast<UserDataAdapter<UserData, Archive> &>( ar ).userdata;
    }
    catch( std::bad_cast const & )
    {
      throw ::cereal::Exception("Attempting to get user data from archive not wrapped in UserDataAdapter");
    }
  }
  #endif
}
#endif
/*#include <cereal/archives/binary.hpp>*/
#ifndef CEREAL_ARCHIVES_BINARY_HPP_
#define CEREAL_ARCHIVES_BINARY_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <sstream>
namespace cereal
{
  class BinaryOutputArchive : public OutputArchive<BinaryOutputArchive, AllowEmptyClassElision>
  {
    public:
      BinaryOutputArchive(std::ostream & stream) :
        OutputArchive<BinaryOutputArchive, AllowEmptyClassElision>(this),
        itsStream(stream)
      { }
      void saveBinary( const void * data, std::size_t size )
      {
        auto const writtenSize = static_cast<std::size_t>( itsStream.rdbuf()->sputn( reinterpret_cast<const char*>( data ), size ) );
        if(writtenSize != size)
          throw Exception("Failed to write " + std::to_string(size) + " bytes to output stream! Wrote " + std::to_string(writtenSize));
      }
    private:
      std::ostream & itsStream;
  };
  class BinaryInputArchive : public InputArchive<BinaryInputArchive, AllowEmptyClassElision>
  {
    public:
      BinaryInputArchive(std::istream & stream) :
        InputArchive<BinaryInputArchive, AllowEmptyClassElision>(this),
        itsStream(stream)
    { }
      void loadBinary( void * const data, std::size_t size )
      {
        auto const readSize = static_cast<std::size_t>( itsStream.rdbuf()->sgetn( reinterpret_cast<char*>( data ), size ) );
        if(readSize != size)
          throw Exception("Failed to read " + std::to_string(size) + " bytes from input stream! Read " + std::to_string(readSize));
      }
    private:
      std::istream & itsStream;
  };
  template<class T> inline
  typename std::enable_if<std::is_arithmetic<T>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME(BinaryOutputArchive & ar, T const & t)
  {
    ar.saveBinary(std::addressof(t), sizeof(t));
  }
  template<class T> inline
  typename std::enable_if<std::is_arithmetic<T>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME(BinaryInputArchive & ar, T & t)
  {
    ar.loadBinary(std::addressof(t), sizeof(t));
  }
  template <class Archive, class T> inline
  CEREAL_ARCHIVE_RESTRICT(BinaryInputArchive, BinaryOutputArchive)
  CEREAL_SERIALIZE_FUNCTION_NAME( Archive & ar, NameValuePair<T> & t )
  {
    ar( t.value );
  }
  template <class Archive, class T> inline
  CEREAL_ARCHIVE_RESTRICT(BinaryInputArchive, BinaryOutputArchive)
  CEREAL_SERIALIZE_FUNCTION_NAME( Archive & ar, SizeTag<T> & t )
  {
    ar( t.size );
  }
  template <class T> inline
  void CEREAL_SAVE_FUNCTION_NAME(BinaryOutputArchive & ar, BinaryData<T> const & bd)
  {
    ar.saveBinary( bd.data, static_cast<std::size_t>( bd.size ) );
  }
  template <class T> inline
  void CEREAL_LOAD_FUNCTION_NAME(BinaryInputArchive & ar, BinaryData<T> & bd)
  {
    ar.loadBinary(bd.data, static_cast<std::size_t>(bd.size));
  }
}
CEREAL_REGISTER_ARCHIVE(cereal::BinaryOutputArchive)
CEREAL_REGISTER_ARCHIVE(cereal::BinaryInputArchive)
CEREAL_SETUP_ARCHIVE_TRAITS(cereal::BinaryInputArchive, cereal::BinaryOutputArchive)
#endif
/*#include <cereal/archives/json.hpp>*/
#ifndef CEREAL_ARCHIVES_JSON_HPP_
#define CEREAL_ARCHIVES_JSON_HPP_
/*#include <cereal/cereal.hpp> skipped */
/*#include <cereal/details/util.hpp> skipped */
namespace cereal
{
  struct RapidJSONException : Exception
  { RapidJSONException( const char * what_ ) : Exception( what_ ) {} };
}
#ifndef RAPIDJSON_ASSERT
#define RAPIDJSON_ASSERT(x) if(!(x)){ \
  throw ::cereal::RapidJSONException("rapidjson internal assertion failure: " #x); }
#endif
/*#include <cereal/external/rapidjson/prettywriter.h>*/
#ifndef RAPIDJSON_PRETTYWRITER_H_
#define RAPIDJSON_PRETTYWRITER_H_
/*#include "writer.h"*/
#ifndef RAPIDJSON_WRITER_H_
#define RAPIDJSON_WRITER_H_
/*#include "rapidjson.h"*/
#ifndef RAPIDJSON_RAPIDJSON_H_
#define RAPIDJSON_RAPIDJSON_H_
// Copyright (c) 2011-2012 Milo Yip (miloyip@gmail.com)
// Version 0.11
#include <cstdlib>	// malloc(), realloc(), free()
#include <cstring>	// memcpy()
///////////////////////////////////////////////////////////////////////////////
// RAPIDJSON_NO_INT64DEFINE
// Here defines int64_t and uint64_t types in global namespace.
// If user have their own definition, can define RAPIDJSON_NO_INT64DEFINE to disable this.
#ifndef RAPIDJSON_NO_INT64DEFINE
#ifdef _MSC_VER
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
/*#include <inttypes.h> skipped */
#endif
#endif // RAPIDJSON_NO_INT64TYPEDEF
///////////////////////////////////////////////////////////////////////////////
// RAPIDJSON_ENDIAN
#define RAPIDJSON_LITTLEENDIAN	0	//!< Little endian machine
#define RAPIDJSON_BIGENDIAN		1	//!< Big endian machine
//! Endianness of the machine.
#ifndef RAPIDJSON_ENDIAN
#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define RAPIDJSON_ENDIAN RAPIDJSON_LITTLEENDIAN
#else
#define RAPIDJSON_ENDIAN RAPIDJSON_BIGENDIAN
#endif // __BYTE_ORDER__
#else
#define RAPIDJSON_ENDIAN RAPIDJSON_LITTLEENDIAN	// Assumes little endian otherwise.
#endif
#endif // RAPIDJSON_ENDIAN
///////////////////////////////////////////////////////////////////////////////
// RAPIDJSON_SSE2/RAPIDJSON_SSE42/RAPIDJSON_SIMD
// Enable SSE2 optimization.
//#define RAPIDJSON_SSE2
// Enable SSE4.2 optimization.
//#define RAPIDJSON_SSE42
#if defined(RAPIDJSON_SSE2) || defined(RAPIDJSON_SSE42)
#define RAPIDJSON_SIMD
#endif
///////////////////////////////////////////////////////////////////////////////
// RAPIDJSON_NO_SIZETYPEDEFINE
#ifndef RAPIDJSON_NO_SIZETYPEDEFINE
namespace rapidjson {
//! Use 32-bit array/string indices even for 64-bit platform, instead of using size_t.
typedef unsigned SizeType;
} // namespace rapidjson
#endif
///////////////////////////////////////////////////////////////////////////////
// RAPIDJSON_ASSERT
//! Assertion.
#ifndef RAPIDJSON_ASSERT
#include <cassert>
#define RAPIDJSON_ASSERT(x) assert(x)
#endif // RAPIDJSON_ASSERT
///////////////////////////////////////////////////////////////////////////////
// Helpers
#define RAPIDJSON_MULTILINEMACRO_BEGIN do {
#define RAPIDJSON_MULTILINEMACRO_END \
} while((void)0, 0)
namespace rapidjson {
///////////////////////////////////////////////////////////////////////////////
// Allocator
///////////////////////////////////////////////////////////////////////////////
// CrtAllocator
//! C-runtime library allocator.
class CrtAllocator {
public:
	static const bool kNeedFree = true;
	void* Malloc(size_t size) { return malloc(size); }
	void* Realloc(void* originalPtr, size_t originalSize, size_t newSize) { (void)originalSize; return realloc(originalPtr, newSize); }
	static void Free(void *ptr) { free(ptr); }
};
///////////////////////////////////////////////////////////////////////////////
// MemoryPoolAllocator
//! Default memory allocator used by the parser and DOM.
template <typename BaseAllocator = CrtAllocator>
class MemoryPoolAllocator {
public:
	static const bool kNeedFree = false;	//!< Tell users that no need to call Free() with this allocator. (concept Allocator)
	//! Constructor with chunkSize.
	MemoryPoolAllocator(size_t chunkSize = kDefaultChunkCapacity, BaseAllocator* baseAllocator = 0) :
		chunkHead_(0), chunk_capacity_(chunkSize), userBuffer_(0), baseAllocator_(baseAllocator), ownBaseAllocator_(0)
	{
		if (!baseAllocator_)
			ownBaseAllocator_ = baseAllocator_ = new BaseAllocator();
		AddChunk(chunk_capacity_);
	}
	//! Constructor with user-supplied buffer.
	MemoryPoolAllocator(char *buffer, size_t size, size_t chunkSize = kDefaultChunkCapacity, BaseAllocator* baseAllocator = 0) :
		chunkHead_(0), chunk_capacity_(chunkSize), userBuffer_(buffer), baseAllocator_(baseAllocator), ownBaseAllocator_(0)
	{
		RAPIDJSON_ASSERT(buffer != 0);
		RAPIDJSON_ASSERT(size > sizeof(ChunkHeader));
		chunkHead_ = (ChunkHeader*)buffer;
		chunkHead_->capacity = size - sizeof(ChunkHeader);
		chunkHead_->size = 0;
		chunkHead_->next = 0;
	}
	//! Destructor.
	~MemoryPoolAllocator() {
		Clear();
		delete ownBaseAllocator_;
	}
	//! Deallocates all memory chunks, excluding the user-supplied buffer.
	void Clear() {
		while(chunkHead_ != 0 && chunkHead_ != (ChunkHeader *)userBuffer_) {
			ChunkHeader* next = chunkHead_->next;
			baseAllocator_->Free(chunkHead_);
			chunkHead_ = next;
		}
	}
	//! Computes the total capacity of allocated memory chunks.
	size_t Capacity() {
		size_t capacity = 0;
		for (ChunkHeader* c = chunkHead_; c != 0; c = c->next)
			capacity += c->capacity;
		return capacity;
	}
	//! Computes the memory blocks allocated.
	size_t Size() {
		size_t size = 0;
		for (ChunkHeader* c = chunkHead_; c != 0; c = c->next)
			size += c->size;
		return size;
	}
	//! Allocates a memory block. (concept Allocator)
	void* Malloc(size_t size) {
		size = (size + 3) & ~3;	// Force aligning size to 4
		if (chunkHead_->size + size > chunkHead_->capacity)
			AddChunk(chunk_capacity_ > size ? chunk_capacity_ : size);
		char *buffer = (char *)(chunkHead_ + 1) + chunkHead_->size;
		RAPIDJSON_ASSERT(((uintptr_t)buffer & 3) == 0);	// returned buffer is aligned to 4
		chunkHead_->size += size;
		return buffer;
	}
	//! Resizes a memory block (concept Allocator)
	void* Realloc(void* originalPtr, size_t originalSize, size_t newSize) {
		if (originalPtr == 0)
			return Malloc(newSize);
		// Do not shrink if new size is smaller than original
		if (originalSize >= newSize)
			return originalPtr;
		// Simply expand it if it is the last allocation and there is sufficient space
		if (originalPtr == (char *)(chunkHead_ + 1) + chunkHead_->size - originalSize) {
			size_t increment = newSize - originalSize;
			increment = (increment + 3) & ~3;	// Force aligning size to 4
			if (chunkHead_->size + increment <= chunkHead_->capacity) {
				chunkHead_->size += increment;
				RAPIDJSON_ASSERT(((uintptr_t)originalPtr & 3) == 0);	// returned buffer is aligned to 4
				return originalPtr;
			}
		}
		// Realloc process: allocate and copy memory, do not free original buffer.
		void* newBuffer = Malloc(newSize);
		RAPIDJSON_ASSERT(newBuffer != 0);	// Do not handle out-of-memory explicitly.
		return memcpy(newBuffer, originalPtr, originalSize);
	}
	//! Frees a memory block (concept Allocator)
	static void Free(void *) {} // Do nothing
private:
	//! Creates a new chunk.
	void AddChunk(size_t capacity) {
		ChunkHeader* chunk = (ChunkHeader*)baseAllocator_->Malloc(sizeof(ChunkHeader) + capacity);
		chunk->capacity = capacity;
		chunk->size = 0;
		chunk->next = chunkHead_;
		chunkHead_ =  chunk;
	}
	static const int kDefaultChunkCapacity = 64 * 1024; //!< Default chunk capacity.
	//! Chunk header for perpending to each chunk.
	struct ChunkHeader {
		size_t capacity;	//!< Capacity of the chunk in bytes (excluding the header itself).
		size_t size;		//!< Current size of allocated memory in bytes.
		ChunkHeader *next;	//!< Next chunk in the linked list.
	};
	ChunkHeader *chunkHead_;	//!< Head of the chunk linked-list. Only the head chunk serves allocation.
	size_t chunk_capacity_;		//!< The minimum capacity of chunk when they are allocated.
	char *userBuffer_;			//!< User supplied buffer.
	BaseAllocator* baseAllocator_;	//!< base allocator for allocating memory chunks.
	BaseAllocator* ownBaseAllocator_;	//!< base allocator created by this object.
};
///////////////////////////////////////////////////////////////////////////////
// Encoding
///////////////////////////////////////////////////////////////////////////////
// UTF8
//! UTF-8 encoding.
template<typename CharType = char>
struct UTF8 {
	typedef CharType Ch;
	static Ch* Encode(Ch *buffer, unsigned codepoint) {
		if (codepoint <= 0x7F)
			*buffer++ = codepoint & 0xFF;
		else if (codepoint <= 0x7FF) {
			*buffer++ = 0xC0 | ((codepoint >> 6) & 0xFF);
			*buffer++ = 0x80 | ((codepoint & 0x3F));
		}
		else if (codepoint <= 0xFFFF) {
			*buffer++ = 0xE0 | ((codepoint >> 12) & 0xFF);
			*buffer++ = 0x80 | ((codepoint >> 6) & 0x3F);
			*buffer++ = 0x80 | (codepoint & 0x3F);
		}
		else {
			RAPIDJSON_ASSERT(codepoint <= 0x10FFFF);
			*buffer++ = 0xF0 | ((codepoint >> 18) & 0xFF);
			*buffer++ = 0x80 | ((codepoint >> 12) & 0x3F);
			*buffer++ = 0x80 | ((codepoint >> 6) & 0x3F);
			*buffer++ = 0x80 | (codepoint & 0x3F);
		}
		return buffer;
	}
};
///////////////////////////////////////////////////////////////////////////////
// UTF16
//! UTF-16 encoding.
template<typename CharType = wchar_t>
struct UTF16 {
	typedef CharType Ch;
	static Ch* Encode(Ch* buffer, unsigned codepoint) {
		if (codepoint <= 0xFFFF) {
			RAPIDJSON_ASSERT(codepoint < 0xD800 || codepoint > 0xDFFF); // Code point itself cannot be surrogate pair
			*buffer++ = static_cast<Ch>(codepoint);
		}
		else {
			RAPIDJSON_ASSERT(codepoint <= 0x10FFFF);
			unsigned v = codepoint - 0x10000;
			*buffer++ = static_cast<Ch>((v >> 10) + 0xD800);
			*buffer++ = (v & 0x3FF) + 0xDC00;
		}
		return buffer;
	}
};
///////////////////////////////////////////////////////////////////////////////
// UTF32
//! UTF-32 encoding.
template<typename CharType = unsigned>
struct UTF32 {
	typedef CharType Ch;
	static Ch *Encode(Ch* buffer, unsigned codepoint) {
		RAPIDJSON_ASSERT(codepoint <= 0x10FFFF);
		*buffer++ = codepoint;
		return buffer;
	}
};
///////////////////////////////////////////////////////////////////////////////
//  Stream
//! Put N copies of a character to a stream.
template<typename Stream, typename Ch>
inline void PutN(Stream& stream, Ch c, size_t n) {
	for (size_t i = 0; i < n; i++)
		stream.Put(c);
}
///////////////////////////////////////////////////////////////////////////////
// StringStream
//! Read-only string stream.
template <typename Encoding>
struct GenericStringStream {
	typedef typename Encoding::Ch Ch;
	GenericStringStream(const Ch *src) : src_(src), head_(src) {}
	Ch Peek() const { return *src_; }
	Ch Take() { return *src_++; }
	size_t Tell() const { return src_ - head_; }
	Ch* PutBegin() { RAPIDJSON_ASSERT(false); return 0; }
	void Put(Ch) { RAPIDJSON_ASSERT(false); }
	size_t PutEnd(Ch*) { RAPIDJSON_ASSERT(false); return 0; }
	const Ch* src_;		//!< Current read position.
	const Ch* head_;	//!< Original head of the string.
};
typedef GenericStringStream<UTF8<> > StringStream;
///////////////////////////////////////////////////////////////////////////////
// InsituStringStream
//! A read-write string stream.
template <typename Encoding>
struct GenericInsituStringStream {
	typedef typename Encoding::Ch Ch;
	GenericInsituStringStream(Ch *src) : src_(src), dst_(0), head_(src) {}
	// Read
	Ch Peek() { return *src_; }
	Ch Take() { return *src_++; }
	size_t Tell() { return src_ - head_; }
	// Write
	Ch* PutBegin() { return dst_ = src_; }
	void Put(Ch c) { RAPIDJSON_ASSERT(dst_ != 0); *dst_++ = c; }
	size_t PutEnd(Ch* begin) { return dst_ - begin; }
	Ch* src_;
	Ch* dst_;
	Ch* head_;
};
typedef GenericInsituStringStream<UTF8<> > InsituStringStream;
///////////////////////////////////////////////////////////////////////////////
// Type
//! Type of JSON value
enum Type {
	kNull_Type = 0,		//!< null
	kFalseType = 1,		//!< false
	kTrueType = 2,		//!< true
	kObjectType = 3,	//!< object
	kArrayType = 4,		//!< array
	kStringType = 5,	//!< string
	kNumberType = 6,	//!< number
};
} // namespace rapidjson
#endif // RAPIDJSON_RAPIDJSON_H_
/*#include "internal/stack.h"*/
#ifndef RAPIDJSON_INTERNAL_STACK_H_
#define RAPIDJSON_INTERNAL_STACK_H_
namespace rapidjson {
namespace internal {
///////////////////////////////////////////////////////////////////////////////
// Stack
//! A type-unsafe stack for storing different types of data.
template <typename Allocator>
class Stack {
public:
	Stack(Allocator* allocator, size_t stack_capacity) : allocator_(allocator), own_allocator_(0), stack_(0), stack_top_(0), stack_end_(0), stack_capacity_(stack_capacity) {
		RAPIDJSON_ASSERT(stack_capacity_ > 0);
		if (!allocator_)
			own_allocator_ = allocator_ = new Allocator();
		stack_top_ = stack_ = (char*)allocator_->Malloc(stack_capacity_);
		stack_end_ = stack_ + stack_capacity_;
	}
	~Stack() {
		Allocator::Free(stack_);
		delete own_allocator_; // Only delete if it is owned by the stack
	}
	void Clear() {  stack_top_ = stack_; }
	template<typename T>
	T* Push(size_t count = 1) {
		 // Expand the stack if needed
		if (stack_top_ + sizeof(T) * count >= stack_end_) {
			size_t new_capacity = stack_capacity_ * 2;
			size_t size = GetSize();
			size_t new_size = GetSize() + sizeof(T) * count;
			if (new_capacity < new_size)
				new_capacity = new_size;
			stack_ = (char*)allocator_->Realloc(stack_, stack_capacity_, new_capacity);
			stack_capacity_ = new_capacity;
			stack_top_ = stack_ + size;
			stack_end_ = stack_ + stack_capacity_;
		}
		T* ret = (T*)stack_top_;
		stack_top_ += sizeof(T) * count;
		return ret;
	}
	template<typename T>
	T* Pop(size_t count) {
		RAPIDJSON_ASSERT(GetSize() >= count * sizeof(T));
		stack_top_ -= count * sizeof(T);
		return (T*)stack_top_;
	}
	template<typename T>
	T* Top() {
		RAPIDJSON_ASSERT(GetSize() >= sizeof(T));
		return (T*)(stack_top_ - sizeof(T));
	}
	template<typename T>
	T* Bottom() { return (T*)stack_; }
	Allocator& GetAllocator() { return *allocator_; }
	size_t GetSize() const { return stack_top_ - stack_; }
	size_t GetCapacity() const { return stack_capacity_; }
private:
	Allocator* allocator_;
	Allocator* own_allocator_;
	char *stack_;
	char *stack_top_;
	char *stack_end_;
	size_t stack_capacity_;
};
} // namespace internal
} // namespace rapidjson
#endif // RAPIDJSON_STACK_H_
/*#include "internal/strfunc.h"*/
#ifndef RAPIDJSON_INTERNAL_STRFUNC_H_
#define RAPIDJSON_INTERNAL_STRFUNC_H_
namespace rapidjson {
namespace internal {
//! Custom strlen() which works on different character types.
template <typename Ch>
inline SizeType StrLen(const Ch* s) {
	const Ch* p = s;
	while (*p != '\0')
		++p;
	return SizeType(p - s);
}
} // namespace internal
} // namespace rapidjson
#endif // RAPIDJSON_INTERNAL_STRFUNC_H_
#include <cstdio>
#include <new>
#include <limits>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4127)
#endif
namespace rapidjson {
template<typename Stream, typename Encoding = UTF8<>, typename Allocator = MemoryPoolAllocator<> >
class Writer {
public:
	typedef typename Encoding::Ch Ch;
	Writer(Stream& stream, int precision = 20, Allocator* allocator = 0, size_t levelDepth = kDefaultLevelDepth) :
		stream_(stream), level_stack_(allocator, levelDepth * sizeof(Level))
  {
#if _MSC_VER
    (void) sprintf_s(double_format, sizeof(double_format), "%%0.%dg", precision);
    (void) sprintf_s( long_double_format, sizeof( long_double_format ), "%%0.%dLg", precision );
#else
    (void) snprintf(double_format, sizeof(double_format), "%%0.%dg", precision);
    (void) snprintf( long_double_format, sizeof( long_double_format ), "%%0.%dLg", precision );
#endif
  }
protected:
  char double_format[32];
  char long_double_format[32];
public:
	Writer& Null_()                          { Prefix(kNull_Type);   WriteNull_();			return *this;             }
	Writer& Bool_(bool b)                    { Prefix(b ? kTrueType : kFalseType); WriteBool_(b); return *this; }
	Writer& Int(int i)                      { Prefix(kNumberType); WriteInt(i);			return *this;             }
	Writer& Uint(unsigned u)                { Prefix(kNumberType); WriteUint(u);		return *this;             }
	Writer& Int64(int64_t i64)              { Prefix(kNumberType); WriteInt64(i64);		return *this;           }
	Writer& Uint64(uint64_t u64)            { Prefix(kNumberType); WriteUint64(u64);	return *this;           }
	Writer& Double(double d)                { Prefix(kNumberType); WriteDouble(d);		return *this;           }
	Writer& LongDouble(long double d)       { Prefix(kNumberType); WriteLongDouble(d);		return *this;       }
	Writer& LongLong(long long d)           { Prefix(kNumberType); WriteLongLong(d);		return *this;         }
	Writer& ULongLong(unsigned long long d) { Prefix(kNumberType); WriteULongLong(d);		return *this;         }
	Writer& String(const Ch* str, SizeType length, bool copy = false) {
		(void)copy;
		Prefix(kStringType);
		WriteString(str, length);
		return *this;
	}
	Writer& StartObject() {
		Prefix(kObjectType);
		new (level_stack_.template Push<Level>()) Level(false);
		WriteStartObject();
		return *this;
	}
	Writer& EndObject(SizeType memberCount = 0) {
		(void)memberCount;
		RAPIDJSON_ASSERT(level_stack_.GetSize() >= sizeof(Level));
		RAPIDJSON_ASSERT(!level_stack_.template Top<Level>()->inArray);
		level_stack_.template Pop<Level>(1);
		WriteEndObject();
		return *this;
	}
	Writer& StartArray() {
		Prefix(kArrayType);
		new (level_stack_.template Push<Level>()) Level(true);
		WriteStartArray();
		return *this;
	}
	Writer& EndArray(SizeType elementCount = 0) {
		(void)elementCount;
		RAPIDJSON_ASSERT(level_stack_.GetSize() >= sizeof(Level));
		RAPIDJSON_ASSERT(level_stack_.template Top<Level>()->inArray);
		level_stack_.template Pop<Level>(1);
		WriteEndArray();
		return *this;
	}
	Writer& String(const Ch* str) { return String(str, internal::StrLen(str)); }
protected:
	struct Level {
		Level(bool inArray_) : inArray(inArray_), valueCount(0) {}
		bool inArray;
		size_t valueCount;
	};
	static const size_t kDefaultLevelDepth = 32;
	void WriteNull_()  {
		stream_.Put('n'); stream_.Put('u'); stream_.Put('l'); stream_.Put('l');
	}
	void WriteBool_(bool b)  {
		if (b) {
			stream_.Put('t'); stream_.Put('r'); stream_.Put('u'); stream_.Put('e');
		}
		else {
			stream_.Put('f'); stream_.Put('a'); stream_.Put('l'); stream_.Put('s'); stream_.Put('e');
		}
	}
	void WriteInt(int i) {
		if (i < 0) {
			stream_.Put('-');
			i = -i;
		}
		WriteUint((unsigned)i);
	}
	void WriteUint(unsigned u) {
		char buffer[10];
		char *p = buffer;
		do {
			*p++ = (u % 10) + '0';
			u /= 10;
		} while (u > 0);
		do {
			--p;
			stream_.Put(*p);
		} while (p != buffer);
	}
	void WriteInt64(int64_t i64) {
		if (i64 < 0) {
			stream_.Put('-');
			i64 = -i64;
		}
		WriteUint64((uint64_t)i64);
	}
	void WriteUint64(uint64_t u64) {
		char buffer[20];
		char *p = buffer;
		do {
			*p++ = char(u64 % 10) + '0';
			u64 /= 10;
		} while (u64 > 0);
		do {
			--p;
			stream_.Put(*p);
		} while (p != buffer);
	}
#ifdef _MSC_VER
  template <class Ch>
  bool characterOk( Ch c )
  {
    return c < 256;
  }
  template <>
  bool characterOk<char>( char )
  {
    return true;
  }
#else
  template <class T>
  static constexpr int to_int( T t ){ return t; }
  template<class Ch>
  typename std::enable_if < to_int(std::numeric_limits<Ch>::max()) < to_int(256), bool>::type
    characterOk( Ch )
  {
    return true;
  }
  template<class Ch>
  typename std::enable_if< to_int(std::numeric_limits<Ch>::max()) >= to_int(256), bool>::type
    characterOk(Ch c)
  { return c < 256; }
#endif
	void WriteDouble(double d) {
		char buffer[100];
#if _MSC_VER
		int ret = sprintf_s(buffer, sizeof(buffer), double_format, d);
#else
		int ret = snprintf(buffer, sizeof(buffer), double_format, d);
#endif
		RAPIDJSON_ASSERT(ret >= 1);
		for (int i = 0; i < ret; i++)
			stream_.Put(buffer[i]);
	}
	void WriteLongDouble(long double d) {
		char buffer[256];
#if _MSC_VER
		int ret = sprintf_s(buffer, sizeof(buffer), long_double_format, d);
#else
		int ret = snprintf(buffer, sizeof(buffer), long_double_format, d);
#endif
		RAPIDJSON_ASSERT(ret >= 1);
		for (int i = 0; i < ret; i++)
			stream_.Put(buffer[i]);
	}
	void WriteLongLong(long long d) {
		char buffer[256];
#if _MSC_VER
		int ret = sprintf_s(buffer, sizeof(buffer), "%lld", d);
#else
		int ret = snprintf(buffer, sizeof(buffer), "%lld", d);
#endif
		RAPIDJSON_ASSERT(ret >= 1);
		for (int i = 0; i < ret; i++)
			stream_.Put(buffer[i]);
	}
	void WriteULongLong(unsigned long long d) {
		char buffer[256];
#if _MSC_VER
		int ret = sprintf_s(buffer, sizeof(buffer), "%llu", d);
#else
		int ret = snprintf(buffer, sizeof(buffer), "%llu", d);
#endif
		RAPIDJSON_ASSERT(ret >= 1);
		for (int i = 0; i < ret; i++)
			stream_.Put(buffer[i]);
	}
	void WriteString(const Ch* str, SizeType length)  {
		static const char hexDigits[] = "0123456789ABCDEF";
		static const char escape[256] = {
#define Z16 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
			'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'b', 't', 'n', 'u', 'f', 'r', 'u', 'u',
			'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u',
			  0,   0, '"',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			Z16, Z16,
			  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,'\\',   0,   0,   0,
			Z16, Z16, Z16, Z16, Z16, Z16, Z16, Z16, Z16, Z16
#undef Z16
		};
		stream_.Put('\"');
		for (const Ch* p = str; p != str + length; ++p) {
			if ((sizeof(Ch) == 1 || characterOk(*p)) && escape[(unsigned char)*p])  {
				stream_.Put('\\');
				stream_.Put(escape[(unsigned char)*p]);
				if (escape[(unsigned char)*p] == 'u') {
					stream_.Put('0');
					stream_.Put('0');
					stream_.Put(hexDigits[(*p) >> 4]);
					stream_.Put(hexDigits[(*p) & 0xF]);
				}
			}
			else
				stream_.Put(*p);
		}
		stream_.Put('\"');
	}
	void WriteStartObject()	{ stream_.Put('{'); }
	void WriteEndObject()	{ stream_.Put('}'); }
	void WriteStartArray()	{ stream_.Put('['); }
	void WriteEndArray()	{ stream_.Put(']'); }
	void Prefix(Type type) {
		(void)type;
		if (level_stack_.GetSize() != 0) {
			Level* level = level_stack_.template Top<Level>();
			if (level->valueCount > 0) {
				if (level->inArray)
					stream_.Put(',');
				else
					stream_.Put((level->valueCount % 2 == 0) ? ',' : ':');
			}
			if (!level->inArray && level->valueCount % 2 == 0)
				RAPIDJSON_ASSERT(type == kStringType);
			level->valueCount++;
		}
		else
			RAPIDJSON_ASSERT(type == kObjectType || type == kArrayType);
	}
	Stream& stream_;
	internal::Stack<Allocator> level_stack_;
private:
	Writer& operator=(const Writer& w);
};
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif
namespace rapidjson {
template<typename Stream, typename Encoding = UTF8<>, typename Allocator = MemoryPoolAllocator<> >
class PrettyWriter : public Writer<Stream, Encoding, Allocator> {
public:
	typedef Writer<Stream, Encoding, Allocator> Base;
	typedef typename Base::Ch Ch;
	PrettyWriter(Stream& stream, int precision = 20, Allocator* allocator = 0, size_t levelDepth = Base::kDefaultLevelDepth) :
		Base(stream, precision, allocator, levelDepth), indentChar_(' '), indentCharCount_(4) {}
	PrettyWriter& SetIndent(Ch indentChar, unsigned indentCharCount) {
		RAPIDJSON_ASSERT(indentChar == ' ' || indentChar == '\t' || indentChar == '\n' || indentChar == '\r');
		indentChar_ = indentChar;
		indentCharCount_ = indentCharCount;
		return *this;
	}
	PrettyWriter& Null_()				{ PrettyPrefix(kNull_Type);   Base::WriteNull_();			return *this; }
	PrettyWriter& Bool_(bool b)			{ PrettyPrefix(b ? kTrueType : kFalseType); Base::WriteBool_(b); return *this; }
	PrettyWriter& Int(int i)			{ PrettyPrefix(kNumberType); Base::WriteInt(i);			return *this; }
	PrettyWriter& Uint(unsigned u)		{ PrettyPrefix(kNumberType); Base::WriteUint(u);		return *this; }
	PrettyWriter& Int64(int64_t i64)	{ PrettyPrefix(kNumberType); Base::WriteInt64(i64);		return *this; }
	PrettyWriter& Uint64(uint64_t u64)	{ PrettyPrefix(kNumberType); Base::WriteUint64(u64);	return *this; }
	PrettyWriter& Double(double d)		{ PrettyPrefix(kNumberType); Base::WriteDouble(d);		return *this; }
	PrettyWriter& String(const Ch* str, SizeType length, bool copy = false) {
		(void)copy;
		PrettyPrefix(kStringType);
		Base::WriteString(str, length);
		return *this;
	}
	PrettyWriter& StartObject() {
		PrettyPrefix(kObjectType);
		new (Base::level_stack_.template Push<typename Base::Level>()) typename Base::Level(false);
		Base::WriteStartObject();
		return *this;
	}
	PrettyWriter& EndObject(SizeType memberCount = 0) {
		(void)memberCount;
		RAPIDJSON_ASSERT(Base::level_stack_.GetSize() >= sizeof(typename Base::Level));
		RAPIDJSON_ASSERT(!Base::level_stack_.template Top<typename Base::Level>()->inArray);
		bool empty = Base::level_stack_.template Pop<typename Base::Level>(1)->valueCount == 0;
		if (!empty) {
			Base::stream_.Put('\n');
			WriteIndent();
		}
		Base::WriteEndObject();
		return *this;
	}
	PrettyWriter& StartArray() {
		PrettyPrefix(kArrayType);
		new (Base::level_stack_.template Push<typename Base::Level>()) typename Base::Level(true);
		Base::WriteStartArray();
		return *this;
	}
	PrettyWriter& EndArray(SizeType memberCount = 0) {
		(void)memberCount;
		RAPIDJSON_ASSERT(Base::level_stack_.GetSize() >= sizeof(typename Base::Level));
		RAPIDJSON_ASSERT(Base::level_stack_.template Top<typename Base::Level>()->inArray);
		bool empty = Base::level_stack_.template Pop<typename Base::Level>(1)->valueCount == 0;
		if (!empty) {
			Base::stream_.Put('\n');
			WriteIndent();
		}
		Base::WriteEndArray();
		return *this;
	}
	PrettyWriter& String(const Ch* str) { return String(str, internal::StrLen(str)); }
protected:
	void PrettyPrefix(Type type) {
		(void)type;
		if (Base::level_stack_.GetSize() != 0) {
			typename Base::Level* level = Base::level_stack_.template Top<typename Base::Level>();
			if (level->inArray) {
				if (level->valueCount > 0) {
					Base::stream_.Put(',');
					Base::stream_.Put('\n');
				}
				else
					Base::stream_.Put('\n');
				WriteIndent();
			}
			else {
				if (level->valueCount > 0) {
					if (level->valueCount % 2 == 0) {
						Base::stream_.Put(',');
						Base::stream_.Put('\n');
					}
					else {
						Base::stream_.Put(':');
						Base::stream_.Put(' ');
					}
				}
				else
					Base::stream_.Put('\n');
				if (level->valueCount % 2 == 0)
					WriteIndent();
			}
			if (!level->inArray && level->valueCount % 2 == 0)
				RAPIDJSON_ASSERT(type == kStringType);
			level->valueCount++;
		}
		else
			RAPIDJSON_ASSERT(type == kObjectType || type == kArrayType);
	}
	void WriteIndent()  {
		size_t count = (Base::level_stack_.GetSize() / sizeof(typename Base::Level)) * indentCharCount_;
		PutN(Base::stream_, indentChar_, count);
	}
	Ch indentChar_;
	unsigned indentCharCount_;
};
}
#endif
/*#include <cereal/external/rapidjson/genericstream.h>*/
#ifndef RAPIDJSON_GENERICSTREAM_H_
#define RAPIDJSON_GENERICSTREAM_H_
/*#include "rapidjson.h" skipped */
#include <iostream>
#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable: 4127)
  #pragma warning(disable: 4512)
  #pragma warning(disable: 4100)
#endif
namespace rapidjson {
  class GenericReadStream {
    public:
      typedef char Ch;
      GenericReadStream(std::istream & is) : is_(&is) {
      }
      Ch Peek() const {
        if(is_->eof()) return '\0';
        return static_cast<char>(is_->peek());
      }
      Ch Take() {
        if(is_->eof()) return '\0';
        return static_cast<char>(is_->get());
      }
      size_t Tell() const {
        return (int)is_->tellg();
      }
      void Put(Ch)       { RAPIDJSON_ASSERT(false); }
      void Flush()       { RAPIDJSON_ASSERT(false); }
      Ch* PutBegin()     { RAPIDJSON_ASSERT(false); return 0; }
      size_t PutEnd(Ch*) { RAPIDJSON_ASSERT(false); return 0; }
      std::istream * is_;
  };
  class GenericWriteStream {
    public:
      typedef char Ch;
      GenericWriteStream(std::ostream& os) : os_(os) {
      }
      void Put(char c) {
        os_.put(c);
      }
      void PutN(char c, size_t n) {
        for (size_t i = 0; i < n; ++i) {
          Put(c);
        }
      }
      void Flush() {
        os_.flush();
      }
      size_t Tell() const {
        return (int)os_.tellp();
      }
      char Peek() const    { RAPIDJSON_ASSERT(false); }
      char Take()          { RAPIDJSON_ASSERT(false); }
      char* PutBegin()     { RAPIDJSON_ASSERT(false); return 0; }
      size_t PutEnd(char*) { RAPIDJSON_ASSERT(false); return 0; }
    private:
      std::ostream& os_;
  };
  template<>
    inline void PutN(GenericWriteStream& stream, char c, size_t n) {
      stream.PutN(c, n);
    }
}
#ifdef _MSC_VER
    #pragma warning(pop)
#endif
#endif
/*#include <cereal/external/rapidjson/reader.h>*/
#ifndef RAPIDJSON_READER_H_
#define RAPIDJSON_READER_H_
/*#include "rapidjson.h" skipped */
/*#include "internal/pow10.h"*/
#ifndef RAPIDJSON_POW10_
#define RAPIDJSON_POW10_
namespace rapidjson {
namespace internal {
inline double Pow10(int n) {
	static const double e[] = {
		1e-308,1e-307,1e-306,1e-305,1e-304,1e-303,1e-302,1e-301,1e-300,
		1e-299,1e-298,1e-297,1e-296,1e-295,1e-294,1e-293,1e-292,1e-291,1e-290,1e-289,1e-288,1e-287,1e-286,1e-285,1e-284,1e-283,1e-282,1e-281,1e-280,
		1e-279,1e-278,1e-277,1e-276,1e-275,1e-274,1e-273,1e-272,1e-271,1e-270,1e-269,1e-268,1e-267,1e-266,1e-265,1e-264,1e-263,1e-262,1e-261,1e-260,
		1e-259,1e-258,1e-257,1e-256,1e-255,1e-254,1e-253,1e-252,1e-251,1e-250,1e-249,1e-248,1e-247,1e-246,1e-245,1e-244,1e-243,1e-242,1e-241,1e-240,
		1e-239,1e-238,1e-237,1e-236,1e-235,1e-234,1e-233,1e-232,1e-231,1e-230,1e-229,1e-228,1e-227,1e-226,1e-225,1e-224,1e-223,1e-222,1e-221,1e-220,
		1e-219,1e-218,1e-217,1e-216,1e-215,1e-214,1e-213,1e-212,1e-211,1e-210,1e-209,1e-208,1e-207,1e-206,1e-205,1e-204,1e-203,1e-202,1e-201,1e-200,
		1e-199,1e-198,1e-197,1e-196,1e-195,1e-194,1e-193,1e-192,1e-191,1e-190,1e-189,1e-188,1e-187,1e-186,1e-185,1e-184,1e-183,1e-182,1e-181,1e-180,
		1e-179,1e-178,1e-177,1e-176,1e-175,1e-174,1e-173,1e-172,1e-171,1e-170,1e-169,1e-168,1e-167,1e-166,1e-165,1e-164,1e-163,1e-162,1e-161,1e-160,
		1e-159,1e-158,1e-157,1e-156,1e-155,1e-154,1e-153,1e-152,1e-151,1e-150,1e-149,1e-148,1e-147,1e-146,1e-145,1e-144,1e-143,1e-142,1e-141,1e-140,
		1e-139,1e-138,1e-137,1e-136,1e-135,1e-134,1e-133,1e-132,1e-131,1e-130,1e-129,1e-128,1e-127,1e-126,1e-125,1e-124,1e-123,1e-122,1e-121,1e-120,
		1e-119,1e-118,1e-117,1e-116,1e-115,1e-114,1e-113,1e-112,1e-111,1e-110,1e-109,1e-108,1e-107,1e-106,1e-105,1e-104,1e-103,1e-102,1e-101,1e-100,
		1e-99, 1e-98, 1e-97, 1e-96, 1e-95, 1e-94, 1e-93, 1e-92, 1e-91, 1e-90, 1e-89, 1e-88, 1e-87, 1e-86, 1e-85, 1e-84, 1e-83, 1e-82, 1e-81, 1e-80,
		1e-79, 1e-78, 1e-77, 1e-76, 1e-75, 1e-74, 1e-73, 1e-72, 1e-71, 1e-70, 1e-69, 1e-68, 1e-67, 1e-66, 1e-65, 1e-64, 1e-63, 1e-62, 1e-61, 1e-60,
		1e-59, 1e-58, 1e-57, 1e-56, 1e-55, 1e-54, 1e-53, 1e-52, 1e-51, 1e-50, 1e-49, 1e-48, 1e-47, 1e-46, 1e-45, 1e-44, 1e-43, 1e-42, 1e-41, 1e-40,
		1e-39, 1e-38, 1e-37, 1e-36, 1e-35, 1e-34, 1e-33, 1e-32, 1e-31, 1e-30, 1e-29, 1e-28, 1e-27, 1e-26, 1e-25, 1e-24, 1e-23, 1e-22, 1e-21, 1e-20,
		1e-19, 1e-18, 1e-17, 1e-16, 1e-15, 1e-14, 1e-13, 1e-12, 1e-11, 1e-10, 1e-9,  1e-8,  1e-7,  1e-6,  1e-5,  1e-4,  1e-3,  1e-2,  1e-1,  1e+0,
		1e+1,  1e+2,  1e+3,  1e+4,  1e+5,  1e+6,  1e+7,  1e+8,  1e+9,  1e+10, 1e+11, 1e+12, 1e+13, 1e+14, 1e+15, 1e+16, 1e+17, 1e+18, 1e+19, 1e+20,
		1e+21, 1e+22, 1e+23, 1e+24, 1e+25, 1e+26, 1e+27, 1e+28, 1e+29, 1e+30, 1e+31, 1e+32, 1e+33, 1e+34, 1e+35, 1e+36, 1e+37, 1e+38, 1e+39, 1e+40,
		1e+41, 1e+42, 1e+43, 1e+44, 1e+45, 1e+46, 1e+47, 1e+48, 1e+49, 1e+50, 1e+51, 1e+52, 1e+53, 1e+54, 1e+55, 1e+56, 1e+57, 1e+58, 1e+59, 1e+60,
		1e+61, 1e+62, 1e+63, 1e+64, 1e+65, 1e+66, 1e+67, 1e+68, 1e+69, 1e+70, 1e+71, 1e+72, 1e+73, 1e+74, 1e+75, 1e+76, 1e+77, 1e+78, 1e+79, 1e+80,
		1e+81, 1e+82, 1e+83, 1e+84, 1e+85, 1e+86, 1e+87, 1e+88, 1e+89, 1e+90, 1e+91, 1e+92, 1e+93, 1e+94, 1e+95, 1e+96, 1e+97, 1e+98, 1e+99, 1e+100,
		1e+101,1e+102,1e+103,1e+104,1e+105,1e+106,1e+107,1e+108,1e+109,1e+110,1e+111,1e+112,1e+113,1e+114,1e+115,1e+116,1e+117,1e+118,1e+119,1e+120,
		1e+121,1e+122,1e+123,1e+124,1e+125,1e+126,1e+127,1e+128,1e+129,1e+130,1e+131,1e+132,1e+133,1e+134,1e+135,1e+136,1e+137,1e+138,1e+139,1e+140,
		1e+141,1e+142,1e+143,1e+144,1e+145,1e+146,1e+147,1e+148,1e+149,1e+150,1e+151,1e+152,1e+153,1e+154,1e+155,1e+156,1e+157,1e+158,1e+159,1e+160,
		1e+161,1e+162,1e+163,1e+164,1e+165,1e+166,1e+167,1e+168,1e+169,1e+170,1e+171,1e+172,1e+173,1e+174,1e+175,1e+176,1e+177,1e+178,1e+179,1e+180,
		1e+181,1e+182,1e+183,1e+184,1e+185,1e+186,1e+187,1e+188,1e+189,1e+190,1e+191,1e+192,1e+193,1e+194,1e+195,1e+196,1e+197,1e+198,1e+199,1e+200,
		1e+201,1e+202,1e+203,1e+204,1e+205,1e+206,1e+207,1e+208,1e+209,1e+210,1e+211,1e+212,1e+213,1e+214,1e+215,1e+216,1e+217,1e+218,1e+219,1e+220,
		1e+221,1e+222,1e+223,1e+224,1e+225,1e+226,1e+227,1e+228,1e+229,1e+230,1e+231,1e+232,1e+233,1e+234,1e+235,1e+236,1e+237,1e+238,1e+239,1e+240,
		1e+241,1e+242,1e+243,1e+244,1e+245,1e+246,1e+247,1e+248,1e+249,1e+250,1e+251,1e+252,1e+253,1e+254,1e+255,1e+256,1e+257,1e+258,1e+259,1e+260,
		1e+261,1e+262,1e+263,1e+264,1e+265,1e+266,1e+267,1e+268,1e+269,1e+270,1e+271,1e+272,1e+273,1e+274,1e+275,1e+276,1e+277,1e+278,1e+279,1e+280,
		1e+281,1e+282,1e+283,1e+284,1e+285,1e+286,1e+287,1e+288,1e+289,1e+290,1e+291,1e+292,1e+293,1e+294,1e+295,1e+296,1e+297,1e+298,1e+299,1e+300,
		1e+301,1e+302,1e+303,1e+304,1e+305,1e+306,1e+307,1e+308
	};
	RAPIDJSON_ASSERT(n <= 308);
	return n < -308 ? 0.0 : e[n + 308];
}
}
}
#endif
/*#include "internal/stack.h" skipped */
#include <csetjmp>
#include <limits>
#include <cmath>
#include <sstream>
#ifdef RAPIDJSON_SSE42
/*#include <nmmintrin.h> skipped */
#elif defined(RAPIDJSON_SSE2)
/*#include <emmintrin.h> skipped */
#endif
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4127)
#pragma warning(disable : 4702)
#endif
#ifndef RAPIDJSON_PARSE_ERROR
#define RAPIDJSON_PARSE_ERROR(msg, offset) \
	RAPIDJSON_MULTILINEMACRO_BEGIN \
	parseError_ = msg; \
	errorOffset_ = offset; \
	longjmp(jmpbuf_, 1); \
	RAPIDJSON_MULTILINEMACRO_END
#endif
namespace rapidjson {
enum ParseFlag {
	kParseDefaultFlags = 0,
	kParseInsituFlag = 1
};
template<typename Encoding = UTF8<> >
struct BaseReaderHandler {
	typedef typename Encoding::Ch Ch;
	void Default() {}
	void Null_() { Default(); }
	void Bool_(bool) { Default(); }
	void Int(int) { Default(); }
	void Uint(unsigned) { Default(); }
	void Int64(int64_t) { Default(); }
	void Uint64(uint64_t) { Default(); }
	void Double(double) { Default(); }
	void String(const Ch*, SizeType, bool) { Default(); }
	void StartObject() { Default(); }
	void EndObject(SizeType) { Default(); }
	void StartArray() { Default(); }
	void EndArray(SizeType) { Default(); }
};
template<typename Stream>
void SkipWhitespace(Stream& stream) {
	Stream s = stream;
	while (s.Peek() == ' ' || s.Peek() == '\n' || s.Peek() == '\r' || s.Peek() == '\t')
		s.Take();
	stream = s;
}
#ifdef RAPIDJSON_SSE42
inline const char *SkipWhitespace_SIMD(const char* p) {
	static const char whitespace[16] = " \n\r\t";
	__m128i w = _mm_loadu_si128((const __m128i *)&whitespace[0]);
	for (;;) {
		__m128i s = _mm_loadu_si128((const __m128i *)p);
		unsigned r = _mm_cvtsi128_si32(_mm_cmpistrm(w, s, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_BIT_MASK | _SIDD_NEGATIVE_POLARITY));
		if (r == 0)
			p += 16;
		else {
#ifdef _MSC_VER
			unsigned long offset;
			if (_BitScanForward(&offset, r))
				return p + offset;
#else
			if (r != 0)
				return p + __builtin_ffs(r) - 1;
#endif
		}
	}
}
#elif defined(RAPIDJSON_SSE2)
inline const char *SkipWhitespace_SIMD(const char* p) {
	static const char whitespaces[4][17] = {
		"                ",
		"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n",
		"\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r",
		"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"};
	__m128i w0 = _mm_loadu_si128((const __m128i *)&whitespaces[0][0]);
	__m128i w1 = _mm_loadu_si128((const __m128i *)&whitespaces[1][0]);
	__m128i w2 = _mm_loadu_si128((const __m128i *)&whitespaces[2][0]);
	__m128i w3 = _mm_loadu_si128((const __m128i *)&whitespaces[3][0]);
	for (;;) {
		__m128i s = _mm_loadu_si128((const __m128i *)p);
		__m128i x = _mm_cmpeq_epi8(s, w0);
		x = _mm_or_si128(x, _mm_cmpeq_epi8(s, w1));
		x = _mm_or_si128(x, _mm_cmpeq_epi8(s, w2));
		x = _mm_or_si128(x, _mm_cmpeq_epi8(s, w3));
		unsigned short r = ~_mm_movemask_epi8(x);
		if (r == 0)
			p += 16;
		else {
#ifdef _MSC_VER
			unsigned long offset;
			if (_BitScanForward(&offset, r))
				return p + offset;
#else
			if (r != 0)
				return p + __builtin_ffs(r) - 1;
#endif
		}
	}
}
#endif
#ifdef RAPIDJSON_SIMD
template<> inline void SkipWhitespace(InsituStringStream& stream) {
	stream.src_ = const_cast<char*>(SkipWhitespace_SIMD(stream.src_));
}
template<> inline void SkipWhitespace(StringStream& stream) {
	stream.src_ = SkipWhitespace_SIMD(stream.src_);
}
#endif
template <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
class GenericReader {
public:
	typedef typename Encoding::Ch Ch;
	GenericReader(Allocator* allocator = 0, size_t stackCapacity = kDefaultStackCapacity) : stack_(allocator, stackCapacity), parseError_(0), errorOffset_(0) {}
	template <unsigned parseFlags, typename Stream, typename Handler>
	bool Parse(Stream& stream, Handler& handler) {
		parseError_ = 0;
		errorOffset_ = 0;
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4611)
#endif
		if (setjmp(jmpbuf_)) {
#ifdef _MSC_VER
#pragma warning(pop)
#endif
			stack_.Clear();
			return false;
		}
		SkipWhitespace(stream);
		if (stream.Peek() == '\0')
			RAPIDJSON_PARSE_ERROR("Text only contains white space(s)", stream.Tell());
		else {
			switch (stream.Peek()) {
				case '{': ParseObject<parseFlags>(stream, handler); break;
				case '[': ParseArray<parseFlags>(stream, handler); break;
				default: RAPIDJSON_PARSE_ERROR("Expect either an object or array at root", stream.Tell());
			}
			SkipWhitespace(stream);
			if (stream.Peek() != '\0' && stream.Peek() != static_cast<Ch>(std::char_traits<Ch>::eof()))
				RAPIDJSON_PARSE_ERROR("Nothing should follow the root object or array.", stream.Tell());
		}
		return true;
	}
	bool HasParseError() const { return parseError_ != 0; }
	const char* GetParseError() const { return parseError_; }
	size_t GetErrorOffset() const { return errorOffset_; }
private:
	template<unsigned parseFlags, typename Stream, typename Handler>
	void ParseObject(Stream& stream, Handler& handler) {
		RAPIDJSON_ASSERT(stream.Peek() == '{');
		stream.Take();
		handler.StartObject();
		SkipWhitespace(stream);
		if (stream.Peek() == '}') {
			stream.Take();
			handler.EndObject(0);
			return;
		}
		for (SizeType memberCount = 0;;) {
			if (stream.Peek() != '"') {
				RAPIDJSON_PARSE_ERROR("Name of an object member must be a string", stream.Tell());
				break;
			}
			ParseString<parseFlags>(stream, handler);
			SkipWhitespace(stream);
			if (stream.Take() != ':') {
				RAPIDJSON_PARSE_ERROR("There must be a colon after the name of object member", stream.Tell());
				break;
			}
			SkipWhitespace(stream);
			ParseValue<parseFlags>(stream, handler);
			SkipWhitespace(stream);
			++memberCount;
			switch(stream.Take()) {
				case ',': SkipWhitespace(stream); break;
				case '}': handler.EndObject(memberCount); return;
				default:  RAPIDJSON_PARSE_ERROR("Must be a comma or '}' after an object member", stream.Tell());
			}
		}
	}
	template<unsigned parseFlags, typename Stream, typename Handler>
	void ParseArray(Stream& stream, Handler& handler) {
		RAPIDJSON_ASSERT(stream.Peek() == '[');
		stream.Take();
		handler.StartArray();
		SkipWhitespace(stream);
		if (stream.Peek() == ']') {
			stream.Take();
			handler.EndArray(0);
			return;
		}
		for (SizeType elementCount = 0;;) {
			ParseValue<parseFlags>(stream, handler);
			++elementCount;
			SkipWhitespace(stream);
			switch (stream.Take()) {
				case ',': SkipWhitespace(stream); break;
				case ']': handler.EndArray(elementCount); return;
				default:  RAPIDJSON_PARSE_ERROR("Must be a comma or ']' after an array element.", stream.Tell());
			}
		}
	}
	template<unsigned parseFlags, typename Stream, typename Handler>
	void ParseNaNNull_(Stream& stream, Handler& handler) {
		RAPIDJSON_ASSERT(stream.Peek() == 'n');
		stream.Take();
    if( stream.Peek() == 'a' && stream.Take() == 'a' && stream.Take() == 'n' )
      handler.Double( std::numeric_limits<double>::quiet_NaN() );
    else if (stream.Take() == 'u' && stream.Take() == 'l' && stream.Take() == 'l')
			handler.Null_();
    else
			RAPIDJSON_PARSE_ERROR("Invalid value", stream.Tell() - 1);
	}
	template<unsigned parseFlags, typename Stream, typename Handler>
	void ParseInfinity(Stream& stream, Handler& handler) {
		RAPIDJSON_ASSERT(stream.Peek() == 'i');
		stream.Take();
    if (stream.Take() == 'n' && stream.Take() == 'f')
      handler.Double( std::numeric_limits<double>::infinity() );
    else
			RAPIDJSON_PARSE_ERROR("Invalid value", stream.Tell() - 1);
	}
	template<unsigned parseFlags, typename Stream, typename Handler>
	void ParseTrue(Stream& stream, Handler& handler) {
		RAPIDJSON_ASSERT(stream.Peek() == 't');
		stream.Take();
		if (stream.Take() == 'r' && stream.Take() == 'u' && stream.Take() == 'e')
			handler.Bool_(true);
		else
			RAPIDJSON_PARSE_ERROR("Invalid value", stream.Tell());
	}
	template<unsigned parseFlags, typename Stream, typename Handler>
	void ParseFalse(Stream& stream, Handler& handler) {
		RAPIDJSON_ASSERT(stream.Peek() == 'f');
		stream.Take();
		if (stream.Take() == 'a' && stream.Take() == 'l' && stream.Take() == 's' && stream.Take() == 'e')
			handler.Bool_(false);
		else
			RAPIDJSON_PARSE_ERROR("Invalid value", stream.Tell() - 1);
	}
	template<typename Stream>
	unsigned ParseHex4(Stream& stream) {
		Stream s = stream;
		unsigned codepoint = 0;
		for (int i = 0; i < 4; i++) {
			Ch c = s.Take();
			codepoint <<= 4;
			codepoint += c;
			if (c >= '0' && c <= '9')
				codepoint -= '0';
			else if (c >= 'A' && c <= 'F')
				codepoint -= 'A' - 10;
			else if (c >= 'a' && c <= 'f')
				codepoint -= 'a' - 10;
			else
				RAPIDJSON_PARSE_ERROR("Incorrect hex digit after \\u escape", s.Tell() - 1);
		}
		stream = s;
		return codepoint;
	}
#ifdef _MSC_VER
  template <class Ch>
  bool characterOk( Ch c )
  {
    return c < 256;
  }
  template <>
  bool characterOk<char>( char )
  {
    return true;
  }
#else
  template <class T>
  static constexpr int to_int( T t ){ return t; }
  template<class Ch>
  typename std::enable_if < to_int(std::numeric_limits<Ch>::max()) < to_int(256), bool>::type
    characterOk( Ch )
  {
    return true;
  }
  template<class Ch>
  typename std::enable_if< to_int(std::numeric_limits<Ch>::max()) >= to_int(256), bool>::type
    characterOk(Ch c)
  { return c < 256; }
#endif
	template<unsigned parseFlags, typename Stream, typename Handler>
	void ParseString(Stream& stream, Handler& handler) {
#define Z16 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		static const Ch escape[256] = {
			Z16, Z16, 0, 0,'\"', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,'/',
			Z16, Z16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,'\\', 0, 0, 0,
			0, 0,'\b', 0, 0, 0,'\f', 0, 0, 0, 0, 0, 0, 0,'\n', 0,
			0, 0,'\r', 0,'\t', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			Z16, Z16, Z16, Z16, Z16, Z16, Z16, Z16
		};
#undef Z16
		Stream s = stream;
		RAPIDJSON_ASSERT(s.Peek() == '\"');
		s.Take();
		Ch *head;
		SizeType len;
		if (parseFlags & kParseInsituFlag)
			head = s.PutBegin();
		else
			len = 0;
#define RAPIDJSON_PUT(x) \
	do { \
		if (parseFlags & kParseInsituFlag) \
			s.Put(x); \
		else { \
			*stack_.template Push<Ch>() = x; \
			++len; \
		} \
	} while(false)
		for (;;) {
			Ch c = s.Take();
			if (c == '\\') {
				Ch e = s.Take();
				if ((sizeof(Ch) == 1 || characterOk(e)) && escape[(unsigned char)e])
					RAPIDJSON_PUT(escape[(unsigned char)e]);
				else if (e == 'u') {
					unsigned codepoint = ParseHex4(s);
					if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
						if (s.Take() != '\\' || s.Take() != 'u') {
							RAPIDJSON_PARSE_ERROR("Missing the second \\u in surrogate pair", s.Tell() - 2);
							return;
						}
						unsigned codepoint2 = ParseHex4(s);
						if (codepoint2 < 0xDC00 || codepoint2 > 0xDFFF) {
							RAPIDJSON_PARSE_ERROR("The second \\u in surrogate pair is invalid", s.Tell() - 2);
							return;
						}
						codepoint = (((codepoint - 0xD800) << 10) | (codepoint2 - 0xDC00)) + 0x10000;
					}
					Ch buffer[4];
					SizeType count = SizeType(Encoding::Encode(buffer, codepoint) - &buffer[0]);
					if (parseFlags & kParseInsituFlag)
						for (SizeType i = 0; i < count; i++)
							s.Put(buffer[i]);
					else {
						memcpy(stack_.template Push<Ch>(count), buffer, count * sizeof(Ch));
						len += count;
					}
				}
				else {
					RAPIDJSON_PARSE_ERROR("Unknown escape character", stream.Tell() - 1);
					return;
				}
			}
			else if (c == '"') {
				if (parseFlags & kParseInsituFlag) {
					size_t length = s.PutEnd(head);
					RAPIDJSON_ASSERT(length <= 0xFFFFFFFF);
					RAPIDJSON_PUT('\0');
					handler.String(head, SizeType(length), false);
				}
				else {
					RAPIDJSON_PUT('\0');
					handler.String(stack_.template Pop<Ch>(len), len - 1, true);
				}
				stream = s;
				return;
			}
			else if (c == '\0') {
				RAPIDJSON_PARSE_ERROR("lacks ending quotation before the end of string", stream.Tell() - 1);
				return;
			}
			else if ((unsigned)c < 0x20) {
				RAPIDJSON_PARSE_ERROR("Incorrect unescaped character in string", stream.Tell() - 1);
				return;
			}
			else
				RAPIDJSON_PUT(c);
		}
#undef RAPIDJSON_PUT
	}
	template<unsigned parseFlags, typename Stream, typename Handler>
	void ParseNumber(Stream& stream, Handler& handler) {
		Stream s = stream;
		bool minus = false;
		if (s.Peek() == '-') {
			minus = true;
			s.Take();
		}
		unsigned i;
		bool try64bit = false;
		if (s.Peek() == '0') {
			i = 0;
			s.Take();
		}
		else if (s.Peek() >= '1' && s.Peek() <= '9') {
			i = s.Take() - '0';
			if (minus)
				while (s.Peek() >= '0' && s.Peek() <= '9') {
					if (i >= 214748364) {
						if (i != 214748364 || s.Peek() > '8') {
							try64bit = true;
							break;
						}
					}
					i = i * 10 + (s.Take() - '0');
				}
			else
				while (s.Peek() >= '0' && s.Peek() <= '9') {
					if (i >= 429496729) {
						if (i != 429496729 || s.Peek() > '5') {
							try64bit = true;
							break;
						}
					}
					i = i * 10 + (s.Take() - '0');
				}
		}
		else {
			RAPIDJSON_PARSE_ERROR("Expect a value here.", stream.Tell());
			return;
		}
		uint64_t i64 = 0;
		bool useDouble = false;
		if (try64bit) {
			i64 = i;
			if (minus)
				while (s.Peek() >= '0' && s.Peek() <= '9') {
					if (i64 >= 922337203685477580uLL)
						if (i64 != 922337203685477580uLL || s.Peek() > '8') {
							useDouble = true;
							break;
						}
					i64 = i64 * 10 + (s.Take() - '0');
				}
			else
				while (s.Peek() >= '0' && s.Peek() <= '9') {
					if (i64 >= 1844674407370955161uLL)
						if (i64 != 1844674407370955161uLL || s.Peek() > '5') {
							useDouble = true;
							break;
						}
					i64 = i64 * 10 + (s.Take() - '0');
				}
		}
		double d = 0.0;
		if (useDouble) {
			d = (double)i64;
			while (s.Peek() >= '0' && s.Peek() <= '9') {
				if (d >= 1E307) {
					RAPIDJSON_PARSE_ERROR("Number too big to store in double", stream.Tell());
					return;
				}
				d = d * 10 + (s.Take() - '0');
			}
		}
		int expFrac = 0;
		if (s.Peek() == '.') {
			if (!useDouble) {
				d = try64bit ? (double)i64 : (double)i;
				useDouble = true;
			}
			s.Take();
			if (s.Peek() >= '0' && s.Peek() <= '9') {
				d = d * 10 + (s.Take() - '0');
				--expFrac;
			}
			else {
				RAPIDJSON_PARSE_ERROR("At least one digit in fraction part", stream.Tell());
				return;
			}
			while (s.Peek() >= '0' && s.Peek() <= '9') {
				if (expFrac > -20) {
					d = d * 10 + (s.Peek() - '0');
					--expFrac;
				}
				s.Take();
			}
		}
		int exp = 0;
		if (s.Peek() == 'e' || s.Peek() == 'E') {
			if (!useDouble) {
				d = try64bit ? (double)i64 : (double)i;
				useDouble = true;
			}
			s.Take();
			bool expMinus = false;
			if (s.Peek() == '+')
				s.Take();
			else if (s.Peek() == '-') {
				s.Take();
				expMinus = true;
			}
			if (s.Peek() >= '0' && s.Peek() <= '9') {
				exp = s.Take() - '0';
				while (s.Peek() >= '0' && s.Peek() <= '9') {
					exp = exp * 10 + (s.Take() - '0');
					if (exp > 308) {
            std::stringstream ss;
            ss.precision( std::numeric_limits<double>::max_digits10 );
            ss << d * internal::Pow10(expFrac) << 'e' << (expMinus ? '-' : '+') << exp;
            double dd;
            ss >> dd;
            if( std::fpclassify( dd ) == FP_SUBNORMAL )
              handler.Double( dd );
            else
						  RAPIDJSON_PARSE_ERROR("Number too big to store in double", stream.Tell());
						return;
					}
				}
			}
			else {
				RAPIDJSON_PARSE_ERROR("At least one digit in exponent", s.Tell());
				return;
			}
			if (expMinus)
				exp = -exp;
		}
		if (useDouble) {
			d *= internal::Pow10(exp + expFrac);
			handler.Double(minus ? -d : d);
		}
		else {
			if (try64bit) {
				if (minus)
					handler.Int64(-(int64_t)i64);
				else
					handler.Uint64(i64);
			}
			else {
				if (minus)
					handler.Int(-(int)i);
				else
					handler.Uint(i);
			}
		}
		stream = s;
	}
	template<unsigned parseFlags, typename Stream, typename Handler>
	void ParseValue(Stream& stream, Handler& handler) {
		switch (stream.Peek()) {
			case 'n': ParseNaNNull_  <parseFlags>(stream, handler); break;
			case 'i': ParseInfinity  <parseFlags>(stream, handler); break;
			case 't': ParseTrue      <parseFlags>(stream, handler); break;
			case 'f': ParseFalse     <parseFlags>(stream, handler); break;
			case '"': ParseString    <parseFlags>(stream, handler); break;
			case '{': ParseObject    <parseFlags>(stream, handler); break;
			case '[': ParseArray     <parseFlags>(stream, handler); break;
			default : ParseNumber    <parseFlags>(stream, handler);
		}
	}
	static const size_t kDefaultStackCapacity = 256;
	internal::Stack<Allocator> stack_;
	jmp_buf jmpbuf_;
	const char* parseError_;
	size_t errorOffset_;
};
typedef GenericReader<UTF8<> > Reader;
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif
/*#include <cereal/external/rapidjson/document.h>*/
#ifndef RAPIDJSON_DOCUMENT_H_
#define RAPIDJSON_DOCUMENT_H_
/*#include "reader.h" skipped */
/*#include "internal/strfunc.h" skipped */
#include <new>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4127)
#endif
namespace rapidjson {
#pragma pack (push, 4)
template <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
class GenericValue {
public:
	struct Member {
		GenericValue<Encoding, Allocator> name;
		GenericValue<Encoding, Allocator> value;
	};
	typedef Encoding EncodingType;
	typedef Allocator AllocatorType;
	typedef typename Encoding::Ch Ch;
	typedef Member* MemberIterator;
	typedef const Member* ConstMemberIterator;
	typedef GenericValue* ValueIterator;
	typedef const GenericValue* ConstValueIterator;
	GenericValue() : flags_(kNull_Flag) {}
private:
	GenericValue(const GenericValue& rhs);
public:
	GenericValue(Type type) {
		static const unsigned defaultFlags[7] = {
			kNull_Flag, kFalseFlag, kTrueFlag, kObjectFlag, kArrayFlag, kConstStringFlag,
			kNumberFlag | kIntFlag | kUintFlag | kInt64Flag | kUint64Flag | kDoubleFlag
		};
		RAPIDJSON_ASSERT(type <= kNumberType);
		flags_ = defaultFlags[type];
		memset(&data_, 0, sizeof(data_));
	}
	GenericValue(bool b) : flags_(b ? kTrueFlag : kFalseFlag) {}
	GenericValue(int i) : flags_(kNumberIntFlag) {
		data_.n.i64 = i;
		if (i >= 0)
			flags_ |= kUintFlag | kUint64Flag;
	}
	GenericValue(unsigned u) : flags_(kNumberUintFlag) {
		data_.n.u64 = u;
		if (!(u & 0x80000000))
			flags_ |= kIntFlag | kInt64Flag;
	}
	GenericValue(int64_t i64) : flags_(kNumberInt64Flag) {
		data_.n.i64 = i64;
		if (i64 >= 0) {
			flags_ |= kNumberUint64Flag;
			if (!(i64 & 0xFFFFFFFF00000000LL))
				flags_ |= kUintFlag;
			if (!(i64 & 0xFFFFFFFF80000000LL))
				flags_ |= kIntFlag;
		}
		else if (i64 >= -2147483648LL)
			flags_ |= kIntFlag;
	}
	GenericValue(uint64_t u64) : flags_(kNumberUint64Flag) {
		data_.n.u64 = u64;
		if (!(u64 & 0x8000000000000000ULL))
			flags_ |= kInt64Flag;
		if (!(u64 & 0xFFFFFFFF00000000ULL))
			flags_ |= kUintFlag;
		if (!(u64 & 0xFFFFFFFF80000000ULL))
			flags_ |= kIntFlag;
	}
	GenericValue(double d) : flags_(kNumberDoubleFlag) { data_.n.d = d; }
	GenericValue(const Ch* s, SizeType length) {
		RAPIDJSON_ASSERT(s != NULL);
		flags_ = kConstStringFlag;
		data_.s.str = s;
		data_.s.length = length;
	}
	GenericValue(const Ch* s) { SetStringRaw(s, internal::StrLen(s)); }
	GenericValue(const Ch* s, SizeType length, Allocator& allocator) { SetStringRaw(s, length, allocator); }
	GenericValue(const Ch*s, Allocator& allocator) { SetStringRaw(s, internal::StrLen(s), allocator); }
	~GenericValue() {
		if (Allocator::kNeedFree) {
			switch(flags_) {
			case kArrayFlag:
				for (GenericValue* v = data_.a.elements; v != data_.a.elements + data_.a.size; ++v)
					v->~GenericValue();
				Allocator::Free(data_.a.elements);
				break;
			case kObjectFlag:
				for (Member* m = data_.o.members; m != data_.o.members + data_.o.size; ++m) {
					m->name.~GenericValue();
					m->value.~GenericValue();
				}
				Allocator::Free(data_.o.members);
				break;
			case kCopyStringFlag:
				Allocator::Free(const_cast<Ch*>(data_.s.str));
				break;
			}
		}
	}
	GenericValue& operator=(GenericValue& rhs) {
		RAPIDJSON_ASSERT(this != &rhs);
		this->~GenericValue();
		memcpy(this, &rhs, sizeof(GenericValue));
		rhs.flags_ = kNull_Flag;
		return *this;
	}
	template <typename T>
	GenericValue& operator=(T value) {
		this->~GenericValue();
		new (this) GenericValue(value);
		return *this;
	}
	Type GetType()	const { return static_cast<Type>(flags_ & kTypeMask); }
	bool IsNull_()	const { return flags_ == kNull_Flag; }
	bool IsFalse()	const { return flags_ == kFalseFlag; }
	bool IsTrue()	const { return flags_ == kTrueFlag; }
	bool IsBool_()	const { return (flags_ & kBool_Flag) != 0; }
	bool IsObject()	const { return flags_ == kObjectFlag; }
	bool IsArray()	const { return flags_ == kArrayFlag; }
	bool IsNumber() const { return (flags_ & kNumberFlag) != 0; }
	bool IsInt()	const { return (flags_ & kIntFlag) != 0; }
	bool IsUint()	const { return (flags_ & kUintFlag) != 0; }
	bool IsInt64()	const { return (flags_ & kInt64Flag) != 0; }
	bool IsUint64()	const { return (flags_ & kUint64Flag) != 0; }
	bool IsDouble() const { return (flags_ & kDoubleFlag) != 0; }
	bool IsString() const { return (flags_ & kStringFlag) != 0; }
	GenericValue& SetNull_() { this->~GenericValue(); new (this) GenericValue(); return *this; }
	bool GetBool_() const { RAPIDJSON_ASSERT(IsBool_()); return flags_ == kTrueFlag; }
	GenericValue& SetBool_(bool b) { this->~GenericValue(); new (this) GenericValue(b); return *this; }
	GenericValue& SetObject() { this->~GenericValue(); new (this) GenericValue(kObjectType); return *this; }
	GenericValue& operator[](const Ch* name) {
		if (Member* member = FindMember(name))
			return member->value;
		else {
			static GenericValue Null_Value;
			return Null_Value;
		}
	}
	const GenericValue& operator[](const Ch* name) const { return const_cast<GenericValue&>(*this)[name]; }
	ConstMemberIterator MemberBegin() const	{ RAPIDJSON_ASSERT(IsObject()); return data_.o.members; }
	ConstMemberIterator MemberEnd()	const	{ RAPIDJSON_ASSERT(IsObject()); return data_.o.members + data_.o.size; }
	MemberIterator MemberBegin()			{ RAPIDJSON_ASSERT(IsObject()); return data_.o.members; }
	MemberIterator MemberEnd()				{ RAPIDJSON_ASSERT(IsObject()); return data_.o.members + data_.o.size; }
	bool HasMember(const Ch* name) const { return FindMember(name) != 0; }
	GenericValue& AddMember(GenericValue& name, GenericValue& value, Allocator& allocator) {
		RAPIDJSON_ASSERT(IsObject());
		RAPIDJSON_ASSERT(name.IsString());
		Object& o = data_.o;
		if (o.size >= o.capacity) {
			if (o.capacity == 0) {
				o.capacity = kDefaultObjectCapacity;
				o.members = (Member*)allocator.Malloc(o.capacity * sizeof(Member));
			}
			else {
				SizeType oldCapacity = o.capacity;
				o.capacity *= 2;
				o.members = (Member*)allocator.Realloc(o.members, oldCapacity * sizeof(Member), o.capacity * sizeof(Member));
			}
		}
		o.members[o.size].name.RawAssign(name);
		o.members[o.size].value.RawAssign(value);
		o.size++;
		return *this;
	}
	GenericValue& AddMember(const Ch* name, Allocator& nameAllocator, GenericValue& value, Allocator& allocator) {
		GenericValue n(name, internal::StrLen(name), nameAllocator);
		return AddMember(n, value, allocator);
	}
	GenericValue& AddMember(const Ch* name, GenericValue& value, Allocator& allocator) {
		GenericValue n(name, internal::StrLen(name));
		return AddMember(n, value, allocator);
	}
	template <typename T>
	GenericValue& AddMember(const Ch* name, T value, Allocator& allocator) {
		GenericValue n(name, internal::StrLen(name));
		GenericValue v(value);
		return AddMember(n, v, allocator);
	}
	bool RemoveMember(const Ch* name) {
		RAPIDJSON_ASSERT(IsObject());
		if (Member* m = FindMember(name)) {
			RAPIDJSON_ASSERT(data_.o.size > 0);
			RAPIDJSON_ASSERT(data_.o.members != 0);
			Member* last = data_.o.members + (data_.o.size - 1);
			if (data_.o.size > 1 && m != last) {
				m->name = last->name;
				m->value = last->value;
			}
			else {
				m->name.~GenericValue();
				m->value.~GenericValue();
			}
			--data_.o.size;
			return true;
		}
		return false;
	}
	GenericValue& SetArray() {	this->~GenericValue(); new (this) GenericValue(kArrayType); return *this; }
	SizeType Size() const { RAPIDJSON_ASSERT(IsArray()); return data_.a.size; }
	SizeType Capacity() const { RAPIDJSON_ASSERT(IsArray()); return data_.a.capacity; }
	bool Empty() const { RAPIDJSON_ASSERT(IsArray()); return data_.a.size == 0; }
	void Clear() {
		RAPIDJSON_ASSERT(IsArray());
		for (SizeType i = 0; i < data_.a.size; ++i)
			data_.a.elements[i].~GenericValue();
		data_.a.size = 0;
	}
	GenericValue& operator[](SizeType index) {
		RAPIDJSON_ASSERT(IsArray());
		RAPIDJSON_ASSERT(index < data_.a.size);
		return data_.a.elements[index];
	}
	const GenericValue& operator[](SizeType index) const { return const_cast<GenericValue&>(*this)[index]; }
	ValueIterator Begin() { RAPIDJSON_ASSERT(IsArray()); return data_.a.elements; }
	ValueIterator End() { RAPIDJSON_ASSERT(IsArray()); return data_.a.elements + data_.a.size; }
	ConstValueIterator Begin() const { return const_cast<GenericValue&>(*this).Begin(); }
	ConstValueIterator End() const { return const_cast<GenericValue&>(*this).End(); }
	GenericValue& Reserve(SizeType newCapacity, Allocator &allocator) {
		RAPIDJSON_ASSERT(IsArray());
		if (newCapacity > data_.a.capacity) {
			data_.a.elements = (GenericValue*)allocator.Realloc(data_.a.elements, data_.a.capacity * sizeof(GenericValue), newCapacity * sizeof(GenericValue));
			data_.a.capacity = newCapacity;
		}
		return *this;
	}
	GenericValue& PushBack(GenericValue& value, Allocator& allocator) {
		RAPIDJSON_ASSERT(IsArray());
		if (data_.a.size >= data_.a.capacity)
			Reserve(data_.a.capacity == 0 ? kDefaultArrayCapacity : data_.a.capacity * 2, allocator);
		data_.a.elements[data_.a.size++].RawAssign(value);
		return *this;
	}
	template <typename T>
	GenericValue& PushBack(T value, Allocator& allocator) {
		GenericValue v(value);
		return PushBack(v, allocator);
	}
	GenericValue& PopBack() {
		RAPIDJSON_ASSERT(IsArray());
		RAPIDJSON_ASSERT(!Empty());
		data_.a.elements[--data_.a.size].~GenericValue();
		return *this;
	}
	int GetInt() const			{ RAPIDJSON_ASSERT(flags_ & kIntFlag);   return data_.n.i.i;   }
	unsigned GetUint() const	{ RAPIDJSON_ASSERT(flags_ & kUintFlag);  return data_.n.u.u;   }
	int64_t GetInt64() const	{ RAPIDJSON_ASSERT(flags_ & kInt64Flag); return data_.n.i64; }
	uint64_t GetUint64() const	{ RAPIDJSON_ASSERT(flags_ & kUint64Flag); return data_.n.u64; }
	double GetDouble() const {
		RAPIDJSON_ASSERT(IsNumber());
		if ((flags_ & kDoubleFlag) != 0)				return data_.n.d;
		if ((flags_ & kIntFlag) != 0)					return data_.n.i.i;
		if ((flags_ & kUintFlag) != 0)					return data_.n.u.u;
		if ((flags_ & kInt64Flag) != 0)					return (double)data_.n.i64;
		RAPIDJSON_ASSERT((flags_ & kUint64Flag) != 0);	return (double)data_.n.u64;
	}
	GenericValue& SetInt(int i)				{ this->~GenericValue(); new (this) GenericValue(i);	return *this; }
	GenericValue& SetUint(unsigned u)		{ this->~GenericValue(); new (this) GenericValue(u);	return *this; }
	GenericValue& SetInt64(int64_t i64)		{ this->~GenericValue(); new (this) GenericValue(i64);	return *this; }
	GenericValue& SetUint64(uint64_t u64)	{ this->~GenericValue(); new (this) GenericValue(u64);	return *this; }
	GenericValue& SetDouble(double d)		{ this->~GenericValue(); new (this) GenericValue(d);	return *this; }
	const Ch* GetString() const { RAPIDJSON_ASSERT(IsString()); return data_.s.str; }
	SizeType GetStringLength() const { RAPIDJSON_ASSERT(IsString()); return data_.s.length; }
	GenericValue& SetString(const Ch* s, SizeType length) { this->~GenericValue(); SetStringRaw(s, length); return *this; }
	GenericValue& SetString(const Ch* s) { return SetString(s, internal::StrLen(s)); }
	GenericValue& SetString(const Ch* s, SizeType length, Allocator& allocator) { this->~GenericValue(); SetStringRaw(s, length, allocator); return *this; }
	GenericValue& SetString(const Ch* s, Allocator& allocator) {	SetString(s, internal::StrLen(s), allocator); return *this; }
	template <typename Handler>
	const GenericValue& Accept(Handler& handler) const {
		switch(GetType()) {
		case kNull_Type:		handler.Null_(); break;
		case kFalseType:	handler.Bool_(false); break;
		case kTrueType:		handler.Bool_(true); break;
		case kObjectType:
			handler.StartObject();
			for (Member* m = data_.o.members; m != data_.o.members + data_.o.size; ++m) {
				handler.String(m->name.data_.s.str, m->name.data_.s.length, false);
				m->value.Accept(handler);
			}
			handler.EndObject(data_.o.size);
			break;
		case kArrayType:
			handler.StartArray();
			for (GenericValue* v = data_.a.elements; v != data_.a.elements + data_.a.size; ++v)
				v->Accept(handler);
			handler.EndArray(data_.a.size);
			break;
		case kStringType:
			handler.String(data_.s.str, data_.s.length, false);
			break;
		case kNumberType:
			if (IsInt())			handler.Int(data_.n.i.i);
			else if (IsUint())		handler.Uint(data_.n.u.u);
			else if (IsInt64())		handler.Int64(data_.n.i64);
			else if (IsUint64())	handler.Uint64(data_.n.u64);
			else					handler.Double(data_.n.d);
			break;
		}
		return *this;
	}
private:
	template <typename, typename>
	friend class GenericDocument;
	enum {
		kBool_Flag = 0x100,
		kNumberFlag = 0x200,
		kIntFlag = 0x400,
		kUintFlag = 0x800,
		kInt64Flag = 0x1000,
		kUint64Flag = 0x2000,
		kDoubleFlag = 0x4000,
		kStringFlag = 0x100000,
		kCopyFlag = 0x200000,
		kNull_Flag = kNull_Type,
		kTrueFlag = kTrueType | kBool_Flag,
		kFalseFlag = kFalseType | kBool_Flag,
		kNumberIntFlag = kNumberType | kNumberFlag | kIntFlag | kInt64Flag,
		kNumberUintFlag = kNumberType | kNumberFlag | kUintFlag | kUint64Flag | kInt64Flag,
		kNumberInt64Flag = kNumberType | kNumberFlag | kInt64Flag,
		kNumberUint64Flag = kNumberType | kNumberFlag | kUint64Flag,
		kNumberDoubleFlag = kNumberType | kNumberFlag | kDoubleFlag,
		kConstStringFlag = kStringType | kStringFlag,
		kCopyStringFlag = kStringType | kStringFlag | kCopyFlag,
		kObjectFlag = kObjectType,
		kArrayFlag = kArrayType,
		kTypeMask = 0xFF
	};
	static const SizeType kDefaultArrayCapacity = 16;
	static const SizeType kDefaultObjectCapacity = 16;
	struct String {
		const Ch* str;
		SizeType length;
		unsigned hashcode;
	};
	union Number {
#if RAPIDJSON_ENDIAN == RAPIDJSON_LITTLEENDIAN
		struct I {
			int i;
			char padding[4];
		}i;
		struct U {
			unsigned u;
			char padding2[4];
		}u;
#else
		struct I {
			char padding[4];
			int i;
		}i;
		struct U {
			char padding2[4];
			unsigned u;
		}u;
#endif
		int64_t i64;
		uint64_t u64;
		double d;
	};
	struct Object {
		Member* members;
		SizeType size;
		SizeType capacity;
	};
	struct Array {
		GenericValue<Encoding, Allocator>* elements;
		SizeType size;
		SizeType capacity;
	};
	union Data {
		String s;
		Number n;
		Object o;
		Array a;
	};
	Member* FindMember(const Ch* name) {
		RAPIDJSON_ASSERT(name);
		RAPIDJSON_ASSERT(IsObject());
		SizeType length = internal::StrLen(name);
		Object& o = data_.o;
		for (Member* member = o.members; member != data_.o.members + data_.o.size; ++member)
			if (length == member->name.data_.s.length && memcmp(member->name.data_.s.str, name, length * sizeof(Ch)) == 0)
				return member;
		return 0;
	}
	const Member* FindMember(const Ch* name) const { return const_cast<GenericValue&>(*this).FindMember(name); }
	void SetArrayRaw(GenericValue* values, SizeType count, Allocator& alloctaor) {
		flags_ = kArrayFlag;
		data_.a.elements = (GenericValue*)alloctaor.Malloc(count * sizeof(GenericValue));
		memcpy(data_.a.elements, values, count * sizeof(GenericValue));
		data_.a.size = data_.a.capacity = count;
	}
	void SetObjectRaw(Member* members, SizeType count, Allocator& alloctaor) {
		flags_ = kObjectFlag;
		data_.o.members = (Member*)alloctaor.Malloc(count * sizeof(Member));
		memcpy(data_.o.members, members, count * sizeof(Member));
		data_.o.size = data_.o.capacity = count;
	}
	void SetStringRaw(const Ch* s, SizeType length) {
		RAPIDJSON_ASSERT(s != NULL);
		flags_ = kConstStringFlag;
		data_.s.str = s;
		data_.s.length = length;
	}
	void SetStringRaw(const Ch* s, SizeType length, Allocator& allocator) {
		RAPIDJSON_ASSERT(s != NULL);
		flags_ = kCopyStringFlag;
		data_.s.str = (Ch *)allocator.Malloc((length + 1) * sizeof(Ch));
		data_.s.length = length;
		memcpy(const_cast<Ch*>(data_.s.str), s, length * sizeof(Ch));
		const_cast<Ch*>(data_.s.str)[length] = '\0';
	}
	void RawAssign(GenericValue& rhs) {
		memcpy(this, &rhs, sizeof(GenericValue));
		rhs.flags_ = kNull_Flag;
	}
	Data data_;
	unsigned flags_;
};
#pragma pack (pop)
typedef GenericValue<UTF8<> > Value;
template <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
class GenericDocument : public GenericValue<Encoding, Allocator> {
public:
	typedef typename Encoding::Ch Ch;
	typedef GenericValue<Encoding, Allocator> ValueType;
	typedef Allocator AllocatorType;
	GenericDocument(Allocator* allocator = 0, size_t stackCapacity = kDefaultStackCapacity) : stack_(allocator, stackCapacity), parseError_(0), errorOffset_(0) {}
	template <unsigned parseFlags, typename Stream>
	GenericDocument& ParseStream(Stream& stream) {
		ValueType::SetNull_();
		GenericReader<Encoding, Allocator> reader;
		if (reader.template Parse<parseFlags>(stream, *this)) {
			RAPIDJSON_ASSERT(stack_.GetSize() == sizeof(ValueType));
			this->RawAssign(*stack_.template Pop<ValueType>(1));
			parseError_ = 0;
			errorOffset_ = 0;
		}
		else {
			parseError_ = reader.GetParseError();
			errorOffset_ = reader.GetErrorOffset();
			ClearStack();
		}
		return *this;
	}
	template <unsigned parseFlags>
	GenericDocument& ParseInsitu(Ch* str) {
		GenericInsituStringStream<Encoding> s(str);
		return ParseStream<parseFlags | kParseInsituFlag>(s);
	}
	template <unsigned parseFlags>
	GenericDocument& Parse(const Ch* str) {
		RAPIDJSON_ASSERT(!(parseFlags & kParseInsituFlag));
		GenericStringStream<Encoding> s(str);
		return ParseStream<parseFlags>(s);
	}
	bool HasParseError() const { return parseError_ != 0; }
	const char* GetParseError() const { return parseError_; }
	size_t GetErrorOffset() const { return errorOffset_; }
	Allocator& GetAllocator() {	return stack_.GetAllocator(); }
	size_t GetStackCapacity() const { return stack_.GetCapacity(); }
private:
	GenericDocument& operator=(const GenericDocument&);
	friend class GenericReader<Encoding, Allocator>;
	void Null_()	{ new (stack_.template Push<ValueType>()) ValueType(); }
	void Bool_(bool b) { new (stack_.template Push<ValueType>()) ValueType(b); }
	void Int(int i) { new (stack_.template Push<ValueType>()) ValueType(i); }
	void Uint(unsigned i) { new (stack_.template Push<ValueType>()) ValueType(i); }
	void Int64(int64_t i) { new (stack_.template Push<ValueType>()) ValueType(i); }
	void Uint64(uint64_t i) { new (stack_.template Push<ValueType>()) ValueType(i); }
	void Double(double d) { new (stack_.template Push<ValueType>()) ValueType(d); }
	void String(const Ch* str, SizeType length, bool copy) {
		if (copy)
			new (stack_.template Push<ValueType>()) ValueType(str, length, GetAllocator());
		else
			new (stack_.template Push<ValueType>()) ValueType(str, length);
	}
	void StartObject() { new (stack_.template Push<ValueType>()) ValueType(kObjectType); }
	void EndObject(SizeType memberCount) {
		typename ValueType::Member* members = stack_.template Pop<typename ValueType::Member>(memberCount);
		stack_.template Top<ValueType>()->SetObjectRaw(members, (SizeType)memberCount, GetAllocator());
	}
	void StartArray() { new (stack_.template Push<ValueType>()) ValueType(kArrayType); }
	void EndArray(SizeType elementCount) {
		ValueType* elements = stack_.template Pop<ValueType>(elementCount);
		stack_.template Top<ValueType>()->SetArrayRaw(elements, elementCount, GetAllocator());
	}
	void ClearStack() {
		if (Allocator::kNeedFree)
			while (stack_.GetSize() > 0)
				(stack_.template Pop<ValueType>(1))->~ValueType();
		else
			stack_.Clear();
	}
	static const size_t kDefaultStackCapacity = 1024;
	internal::Stack<Allocator> stack_;
	const char* parseError_;
	size_t errorOffset_;
};
typedef GenericDocument<UTF8<> > Document;
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif
/*#include <cereal/external/base64.hpp>*/
#ifndef CEREAL_EXTERNAL_BASE64_HPP_
#define CEREAL_EXTERNAL_BASE64_HPP_
#include <string>
namespace base64
{
  static const std::string chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";
  static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
  }
  inline std::string encode(unsigned char const* bytes_to_encode, size_t in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    while (in_len--) {
      char_array_3[i++] = *(bytes_to_encode++);
      if (i == 3) {
        char_array_4[0] = (unsigned char) ((char_array_3[0] & 0xfc) >> 2);
        char_array_4[1] = (unsigned char) ( ( ( char_array_3[0] & 0x03 ) << 4 ) + ( ( char_array_3[1] & 0xf0 ) >> 4 ) );
        char_array_4[2] = (unsigned char) ( ( ( char_array_3[1] & 0x0f ) << 2 ) + ( ( char_array_3[2] & 0xc0 ) >> 6 ) );
        char_array_4[3] = (unsigned char) ( char_array_3[2] & 0x3f );
        for(i = 0; (i <4) ; i++)
          ret += chars[char_array_4[i]];
        i = 0;
      }
    }
    if (i)
    {
      for(j = i; j < 3; j++)
        char_array_3[j] = '\0';
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;
      for (j = 0; (j < i + 1); j++)
        ret += chars[char_array_4[j]];
      while((i++ < 3))
        ret += '=';
    }
    return ret;
  }
  inline std::string decode(std::string const& encoded_string) {
    size_t in_len = encoded_string.size();
    size_t i = 0;
    size_t j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;
    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
      char_array_4[i++] = encoded_string[in_]; in_++;
      if (i ==4) {
        for (i = 0; i <4; i++)
          char_array_4[i] = (unsigned char) chars.find( char_array_4[i] );
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        for (i = 0; (i < 3); i++)
          ret += char_array_3[i];
        i = 0;
      }
    }
    if (i) {
      for (j = i; j <4; j++)
        char_array_4[j] = 0;
      for (j = 0; j <4; j++)
        char_array_4[j] = (unsigned char) chars.find( char_array_4[j] );
      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
      for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    return ret;
  }
}
#endif
#include <limits>
#include <sstream>
#include <stack>
#include <vector>
#include <string>
namespace cereal
{
  class JSONOutputArchive : public OutputArchive<JSONOutputArchive>, public traits::TextArchive
  {
    enum class NodeType { StartObject, InObject, StartArray, InArray };
    typedef rapidjson::GenericWriteStream WriteStream;
    typedef rapidjson::PrettyWriter<WriteStream> JSONWriter;
    public:
      class Options
      {
        public:
          static Options Default(){ return Options(); }
          static Options NoIndent(){ return Options( std::numeric_limits<double>::max_digits10, IndentChar::space, 0 ); }
          enum class IndentChar : char
          {
            space = ' ',
            tab = '\t',
            newline = '\n',
            carriage_return = '\r'
          };
          explicit Options( int precision = std::numeric_limits<double>::max_digits10,
                            IndentChar indentChar = IndentChar::space,
                            unsigned int indentLength = 4 ) :
            itsPrecision( precision ),
            itsIndentChar( static_cast<char>(indentChar) ),
            itsIndentLength( indentLength ) { }
        private:
          friend class JSONOutputArchive;
          int itsPrecision;
          char itsIndentChar;
          unsigned int itsIndentLength;
      };
      JSONOutputArchive(std::ostream & stream, Options const & options = Options::Default() ) :
        OutputArchive<JSONOutputArchive>(this),
        itsWriteStream(stream),
        itsWriter(itsWriteStream, options.itsPrecision),
        itsNextName(nullptr)
      {
        itsWriter.SetIndent( options.itsIndentChar, options.itsIndentLength );
        itsNameCounter.push(0);
        itsNodeStack.push(NodeType::StartObject);
      }
      ~JSONOutputArchive()
      {
        if (itsNodeStack.top() == NodeType::InObject)
          itsWriter.EndObject();
      }
      void saveBinaryValue( const void * data, size_t size, const char * name = nullptr )
      {
        setNextName( name );
        writeName();
        auto base64string = base64::encode( reinterpret_cast<const unsigned char *>( data ), size );
        saveValue( base64string );
      };
      void startNode()
      {
        writeName();
        itsNodeStack.push(NodeType::StartObject);
        itsNameCounter.push(0);
      }
      void finishNode()
      {
        switch(itsNodeStack.top())
        {
          case NodeType::StartArray:
            itsWriter.StartArray();
          case NodeType::InArray:
            itsWriter.EndArray();
            break;
          case NodeType::StartObject:
            itsWriter.StartObject();
          case NodeType::InObject:
            itsWriter.EndObject();
            break;
        }
        itsNodeStack.pop();
        itsNameCounter.pop();
      }
      void setNextName( const char * name )
      {
        itsNextName = name;
      }
      void saveValue(bool b)                { itsWriter.Bool_(b);                                                         }
      void saveValue(int i)                 { itsWriter.Int(i);                                                          }
      void saveValue(unsigned u)            { itsWriter.Uint(u);                                                         }
      void saveValue(int64_t i64)           { itsWriter.Int64(i64);                                                      }
      void saveValue(uint64_t u64)          { itsWriter.Uint64(u64);                                                     }
      void saveValue(double d)              { itsWriter.Double(d);                                                       }
      void saveValue(std::string const & s) { itsWriter.String(s.c_str(), static_cast<rapidjson::SizeType>( s.size() )); }
      void saveValue(char const * s)        { itsWriter.String(s);                                                       }
    private:
      template <class T, traits::EnableIf<sizeof(T) == sizeof(std::int32_t),
                                          std::is_signed<T>::value> = traits::sfinae> inline
      void saveLong(T l){ saveValue( static_cast<std::int32_t>( l ) ); }
      template <class T, traits::EnableIf<sizeof(T) != sizeof(std::int32_t),
                                          std::is_signed<T>::value> = traits::sfinae> inline
      void saveLong(T l){ saveValue( static_cast<std::int64_t>( l ) ); }
      template <class T, traits::EnableIf<sizeof(T) == sizeof(std::int32_t),
                                          std::is_unsigned<T>::value> = traits::sfinae> inline
      void saveLong(T lu){ saveValue( static_cast<std::uint32_t>( lu ) ); }
      template <class T, traits::EnableIf<sizeof(T) != sizeof(std::int32_t),
                                          std::is_unsigned<T>::value> = traits::sfinae> inline
      void saveLong(T lu){ saveValue( static_cast<std::uint64_t>( lu ) ); }
    public:
#ifdef _MSC_VER
      void saveValue( unsigned long lu ){ saveLong( lu ); };
#else
      template <class T, traits::EnableIf<std::is_same<T, long>::value,
                                          !std::is_same<T, std::int32_t>::value,
                                          !std::is_same<T, std::int64_t>::value> = traits::sfinae> inline
      void saveValue( T t ){ saveLong( t ); }
      template <class T, traits::EnableIf<std::is_same<T, unsigned long>::value,
                                          !std::is_same<T, std::uint32_t>::value,
                                          !std::is_same<T, std::uint64_t>::value> = traits::sfinae> inline
      void saveValue( T t ){ saveLong( t ); }
#endif
      template <class T, traits::EnableIf<std::is_arithmetic<T>::value,
                                          !std::is_same<T, long>::value,
                                          !std::is_same<T, unsigned long>::value,
                                          !std::is_same<T, std::int64_t>::value,
                                          !std::is_same<T, std::uint64_t>::value,
                                          (sizeof(T) >= sizeof(long double) || sizeof(T) >= sizeof(long long))> = traits::sfinae> inline
      void saveValue(T const & t)
      {
        std::stringstream ss; ss.precision( std::numeric_limits<long double>::max_digits10 );
        ss << t;
        saveValue( ss.str() );
      }
      void writeName()
      {
        NodeType const & nodeType = itsNodeStack.top();
        if(nodeType == NodeType::StartArray)
        {
          itsWriter.StartArray();
          itsNodeStack.top() = NodeType::InArray;
        }
        else if(nodeType == NodeType::StartObject)
        {
          itsNodeStack.top() = NodeType::InObject;
          itsWriter.StartObject();
        }
        if(nodeType == NodeType::InArray) return;
        if(itsNextName == nullptr)
        {
          std::string name = "value" + std::to_string( itsNameCounter.top()++ ) + "\0";
          saveValue(name);
        }
        else
        {
          saveValue(itsNextName);
          itsNextName = nullptr;
        }
      }
      void makeArray()
      {
        itsNodeStack.top() = NodeType::StartArray;
      }
    private:
      WriteStream itsWriteStream;
      JSONWriter itsWriter;
      char const * itsNextName;
      std::stack<uint32_t> itsNameCounter;
      std::stack<NodeType> itsNodeStack;
  };
  class JSONInputArchive : public InputArchive<JSONInputArchive>, public traits::TextArchive
  {
    private:
      typedef rapidjson::GenericReadStream ReadStream;
      typedef rapidjson::GenericValue<rapidjson::UTF8<>> JSONValue;
      typedef JSONValue::ConstMemberIterator MemberIterator;
      typedef JSONValue::ConstValueIterator ValueIterator;
      typedef rapidjson::Document::GenericValue GenericValue;
    public:
      JSONInputArchive(std::istream & stream) :
        InputArchive<JSONInputArchive>(this),
        itsNextName( nullptr ),
        itsReadStream(stream)
      {
        itsDocument.ParseStream<0>(itsReadStream);
        itsIteratorStack.emplace_back(itsDocument.MemberBegin(), itsDocument.MemberEnd());
      }
      void loadBinaryValue( void * data, size_t size, const char * name = nullptr )
      {
        itsNextName = name;
        std::string encoded;
        loadValue( encoded );
        auto decoded = base64::decode( encoded );
        if( size != decoded.size() )
          throw Exception("Decoded binary data size does not match specified size");
        std::memcpy( data, decoded.data(), decoded.size() );
        itsNextName = nullptr;
      };
    private:
      class Iterator
      {
        public:
          Iterator() : itsIndex( 0 ), itsType(Null_) {}
          Iterator(MemberIterator begin, MemberIterator end) :
            itsMemberItBegin(begin), itsMemberItEnd(end), itsIndex(0), itsType(Member)
          { }
          Iterator(ValueIterator begin, ValueIterator end) :
            itsValueItBegin(begin), itsValueItEnd(end), itsIndex(0), itsType(Value)
          { }
          Iterator & operator++()
          {
            ++itsIndex;
            return *this;
          }
          GenericValue const & value()
          {
            switch(itsType)
            {
              case Value : return itsValueItBegin[itsIndex];
              case Member: return itsMemberItBegin[itsIndex].value;
              default: throw cereal::Exception("Invalid Iterator Type!");
            }
          }
          const char * name() const
          {
            if( itsType == Member && (itsMemberItBegin + itsIndex) != itsMemberItEnd )
              return itsMemberItBegin[itsIndex].name.GetString();
            else
              return nullptr;
          }
          inline void search( const char * searchName )
          {
            const auto len = std::strlen( searchName );
            size_t index = 0;
            for( auto it = itsMemberItBegin; it != itsMemberItEnd; ++it, ++index )
            {
              const auto currentName = it->name.GetString();
              if( ( std::strncmp( searchName, currentName, len ) == 0 ) &&
                  ( std::strlen( currentName ) == len ) )
              {
                itsIndex = index;
                return;
              }
            }
            throw Exception("JSON Parsing failed - provided NVP not found");
          }
        private:
          MemberIterator itsMemberItBegin, itsMemberItEnd;
          ValueIterator itsValueItBegin, itsValueItEnd;
          size_t itsIndex;
          enum Type {Value, Member, Null_} itsType;
      };
      inline void search()
      {
        if( itsNextName )
        {
          auto const actualName = itsIteratorStack.back().name();
          if( !actualName || std::strcmp( itsNextName, actualName ) != 0 )
            itsIteratorStack.back().search( itsNextName );
        }
        itsNextName = nullptr;
      }
    public:
      void startNode()
      {
        search();
        if(itsIteratorStack.back().value().IsArray())
          itsIteratorStack.emplace_back(itsIteratorStack.back().value().Begin(), itsIteratorStack.back().value().End());
        else
          itsIteratorStack.emplace_back(itsIteratorStack.back().value().MemberBegin(), itsIteratorStack.back().value().MemberEnd());
      }
      void finishNode()
      {
        itsIteratorStack.pop_back();
        ++itsIteratorStack.back();
      }
      const char * getNodeName() const
      {
        return itsIteratorStack.back().name();
      }
      void setNextName( const char * name )
      {
        itsNextName = name;
      }
      template <class T, traits::EnableIf<std::is_signed<T>::value,
                                          sizeof(T) < sizeof(int64_t)> = traits::sfinae> inline
      void loadValue(T & val)
      {
        search();
        val = static_cast<T>( itsIteratorStack.back().value().GetInt() );
        ++itsIteratorStack.back();
      }
      template <class T, traits::EnableIf<std::is_unsigned<T>::value,
                                          sizeof(T) < sizeof(uint64_t),
                                          !std::is_same<bool, T>::value> = traits::sfinae> inline
      void loadValue(T & val)
      {
        search();
        val = static_cast<T>( itsIteratorStack.back().value().GetUint() );
        ++itsIteratorStack.back();
      }
      void loadValue(bool & val)        { search(); val = itsIteratorStack.back().value().GetBool_();   ++itsIteratorStack.back(); }
      void loadValue(int64_t & val)     { search(); val = itsIteratorStack.back().value().GetInt64();  ++itsIteratorStack.back(); }
      void loadValue(uint64_t & val)    { search(); val = itsIteratorStack.back().value().GetUint64(); ++itsIteratorStack.back(); }
      void loadValue(float & val)       { search(); val = static_cast<float>(itsIteratorStack.back().value().GetDouble()); ++itsIteratorStack.back(); }
      void loadValue(double & val)      { search(); val = itsIteratorStack.back().value().GetDouble(); ++itsIteratorStack.back(); }
      void loadValue(std::string & val) { search(); val = itsIteratorStack.back().value().GetString(); ++itsIteratorStack.back(); }
      #ifndef _MSC_VER
    private:
      template <class T> inline
      typename std::enable_if<sizeof(T) == sizeof(std::int32_t) && std::is_signed<T>::value, void>::type
      loadLong(T & l){ loadValue( reinterpret_cast<std::int32_t&>( l ) ); }
      template <class T> inline
      typename std::enable_if<sizeof(T) == sizeof(std::int64_t) && std::is_signed<T>::value, void>::type
      loadLong(T & l){ loadValue( reinterpret_cast<std::int64_t&>( l ) ); }
      template <class T> inline
      typename std::enable_if<sizeof(T) == sizeof(std::uint32_t) && !std::is_signed<T>::value, void>::type
      loadLong(T & lu){ loadValue( reinterpret_cast<std::uint32_t&>( lu ) ); }
      template <class T> inline
      typename std::enable_if<sizeof(T) == sizeof(std::uint64_t) && !std::is_signed<T>::value, void>::type
      loadLong(T & lu){ loadValue( reinterpret_cast<std::uint64_t&>( lu ) ); }
    public:
      template <class T> inline
      typename std::enable_if<std::is_same<T, long>::value &&
                              !std::is_same<T, std::int32_t>::value &&
                              !std::is_same<T, std::int64_t>::value, void>::type
      loadValue( T & t ){ loadLong(t); }
      template <class T> inline
      typename std::enable_if<std::is_same<T, unsigned long>::value &&
                              !std::is_same<T, std::uint32_t>::value &&
                              !std::is_same<T, std::uint64_t>::value, void>::type
      loadValue( T & t ){ loadLong(t); }
      #endif
    private:
      void stringToNumber( std::string const & str, long long & val ) { val = std::stoll( str ); }
      void stringToNumber( std::string const & str, unsigned long long & val ) { val = std::stoull( str ); }
      void stringToNumber( std::string const & str, long double & val ) { val = std::stold( str ); }
    public:
      template <class T, traits::EnableIf<std::is_arithmetic<T>::value,
                                          !std::is_same<T, long>::value,
                                          !std::is_same<T, unsigned long>::value,
                                          !std::is_same<T, std::int64_t>::value,
                                          !std::is_same<T, std::uint64_t>::value,
                                          (sizeof(T) >= sizeof(long double) || sizeof(T) >= sizeof(long long))> = traits::sfinae>
      inline void loadValue(T & val)
      {
        std::string encoded;
        loadValue( encoded );
        stringToNumber( encoded, val );
      }
      void loadSize(size_type & size)
      {
        size = (itsIteratorStack.rbegin() + 1)->value().Size();
      }
    private:
      const char * itsNextName;
      ReadStream itsReadStream;
      std::vector<Iterator> itsIteratorStack;
      rapidjson::Document itsDocument;
  };
  template <class T> inline
  void prologue( JSONOutputArchive &, NameValuePair<T> const & )
  { }
  template <class T> inline
  void prologue( JSONInputArchive &, NameValuePair<T> const & )
  { }
  template <class T> inline
  void epilogue( JSONOutputArchive &, NameValuePair<T> const & )
  { }
  template <class T> inline
  void epilogue( JSONInputArchive &, NameValuePair<T> const & )
  { }
  template <class T> inline
  void prologue( JSONOutputArchive & ar, SizeTag<T> const & )
  {
    ar.makeArray();
  }
  template <class T> inline
  void prologue( JSONInputArchive &, SizeTag<T> const & )
  { }
  template <class T> inline
  void epilogue( JSONOutputArchive &, SizeTag<T> const & )
  { }
  template <class T> inline
  void epilogue( JSONInputArchive &, SizeTag<T> const & )
  { }
  template <class T, traits::DisableIf<std::is_arithmetic<T>::value ||
                                       traits::has_minimal_base_class_serialization<T, traits::has_minimal_output_serialization, JSONOutputArchive>::value ||
                                       traits::has_minimal_output_serialization<T, JSONOutputArchive>::value> = traits::sfinae>
  inline void prologue( JSONOutputArchive & ar, T const & )
  {
    ar.startNode();
  }
  template <class T, traits::DisableIf<std::is_arithmetic<T>::value ||
                                       traits::has_minimal_base_class_serialization<T, traits::has_minimal_input_serialization, JSONInputArchive>::value ||
                                       traits::has_minimal_input_serialization<T, JSONInputArchive>::value> = traits::sfinae>
  inline void prologue( JSONInputArchive & ar, T const & )
  {
    ar.startNode();
  }
  template <class T, traits::DisableIf<std::is_arithmetic<T>::value ||
                                       traits::has_minimal_base_class_serialization<T, traits::has_minimal_output_serialization, JSONOutputArchive>::value ||
                                       traits::has_minimal_output_serialization<T, JSONOutputArchive>::value> = traits::sfinae>
  inline void epilogue( JSONOutputArchive & ar, T const & )
  {
    ar.finishNode();
  }
  template <class T, traits::DisableIf<std::is_arithmetic<T>::value ||
                                       traits::has_minimal_base_class_serialization<T, traits::has_minimal_input_serialization, JSONInputArchive>::value ||
                                       traits::has_minimal_input_serialization<T, JSONInputArchive>::value> = traits::sfinae>
  inline void epilogue( JSONInputArchive & ar, T const & )
  {
    ar.finishNode();
  }
  template <class T, traits::EnableIf<std::is_arithmetic<T>::value> = traits::sfinae> inline
  void prologue( JSONOutputArchive & ar, T const & )
  {
    ar.writeName();
  }
  template <class T, traits::EnableIf<std::is_arithmetic<T>::value> = traits::sfinae> inline
  void prologue( JSONInputArchive &, T const & )
  { }
  template <class T, traits::EnableIf<std::is_arithmetic<T>::value> = traits::sfinae> inline
  void epilogue( JSONOutputArchive &, T const & )
  { }
  template <class T, traits::EnableIf<std::is_arithmetic<T>::value> = traits::sfinae> inline
  void epilogue( JSONInputArchive &, T const & )
  { }
  template<class CharT, class Traits, class Alloc> inline
  void prologue(JSONOutputArchive & ar, std::basic_string<CharT, Traits, Alloc> const &)
  {
    ar.writeName();
  }
  template<class CharT, class Traits, class Alloc> inline
  void prologue(JSONInputArchive &, std::basic_string<CharT, Traits, Alloc> const &)
  { }
  template<class CharT, class Traits, class Alloc> inline
  void epilogue(JSONOutputArchive &, std::basic_string<CharT, Traits, Alloc> const &)
  { }
  template<class CharT, class Traits, class Alloc> inline
  void epilogue(JSONInputArchive &, std::basic_string<CharT, Traits, Alloc> const &)
  { }
  template <class T> inline
  void CEREAL_SAVE_FUNCTION_NAME( JSONOutputArchive & ar, NameValuePair<T> const & t )
  {
    ar.setNextName( t.name );
    ar( t.value );
  }
  template <class T> inline
  void CEREAL_LOAD_FUNCTION_NAME( JSONInputArchive & ar, NameValuePair<T> & t )
  {
    ar.setNextName( t.name );
    ar( t.value );
  }
  template <class T, traits::EnableIf<std::is_arithmetic<T>::value> = traits::sfinae> inline
  void CEREAL_SAVE_FUNCTION_NAME(JSONOutputArchive & ar, T const & t)
  {
    ar.saveValue( t );
  }
  template <class T, traits::EnableIf<std::is_arithmetic<T>::value> = traits::sfinae> inline
  void CEREAL_LOAD_FUNCTION_NAME(JSONInputArchive & ar, T & t)
  {
    ar.loadValue( t );
  }
  template<class CharT, class Traits, class Alloc> inline
  void CEREAL_SAVE_FUNCTION_NAME(JSONOutputArchive & ar, std::basic_string<CharT, Traits, Alloc> const & str)
  {
    ar.saveValue( str );
  }
  template<class CharT, class Traits, class Alloc> inline
  void CEREAL_LOAD_FUNCTION_NAME(JSONInputArchive & ar, std::basic_string<CharT, Traits, Alloc> & str)
  {
    ar.loadValue( str );
  }
  template <class T> inline
  void CEREAL_SAVE_FUNCTION_NAME( JSONOutputArchive &, SizeTag<T> const & )
  {
  }
  template <class T> inline
  void CEREAL_LOAD_FUNCTION_NAME( JSONInputArchive & ar, SizeTag<T> & st )
  {
    ar.loadSize( st.size );
  }
}
CEREAL_REGISTER_ARCHIVE(cereal::JSONInputArchive)
CEREAL_REGISTER_ARCHIVE(cereal::JSONOutputArchive)
CEREAL_SETUP_ARCHIVE_TRAITS(cereal::JSONInputArchive, cereal::JSONOutputArchive)
#endif
/*#include <cereal/archives/portable_binary.hpp>*/
#ifndef CEREAL_ARCHIVES_PORTABLE_BINARY_HPP_
#define CEREAL_ARCHIVES_PORTABLE_BINARY_HPP_
/*#include <cereal/cereal.hpp> skipped */
#include <sstream>
#include <limits>
namespace cereal
{
  namespace portable_binary_detail
  {
    inline bool is_little_endian()
    {
      static std::int32_t test = 1;
      return *reinterpret_cast<std::int8_t*>( &test ) == 1;
    }
    template <std::size_t DataSize>
    inline void swap_bytes( std::uint8_t * data )
    {
      for( std::size_t i = 0, end = DataSize / 2; i < end; ++i )
        std::swap( data[i], data[DataSize - i - 1] );
    }
  }
  class PortableBinaryOutputArchive : public OutputArchive<PortableBinaryOutputArchive, AllowEmptyClassElision>
  {
    public:
      PortableBinaryOutputArchive(std::ostream & stream) :
        OutputArchive<PortableBinaryOutputArchive, AllowEmptyClassElision>(this),
        itsStream(stream)
      {
        this->operator()( portable_binary_detail::is_little_endian() );
      }
      void saveBinary( const void * data, std::size_t size )
      {
        auto const writtenSize = static_cast<std::size_t>( itsStream.rdbuf()->sputn( reinterpret_cast<const char*>( data ), size ) );
        if(writtenSize != size)
          throw Exception("Failed to write " + std::to_string(size) + " bytes to output stream! Wrote " + std::to_string(writtenSize));
      }
    private:
      std::ostream & itsStream;
  };
  class PortableBinaryInputArchive : public InputArchive<PortableBinaryInputArchive, AllowEmptyClassElision>
  {
    public:
      PortableBinaryInputArchive(std::istream & stream) :
        InputArchive<PortableBinaryInputArchive, AllowEmptyClassElision>(this),
        itsStream(stream),
        itsConvertEndianness( false )
      {
        bool streamLittleEndian;
        this->operator()( streamLittleEndian );
        itsConvertEndianness = portable_binary_detail::is_little_endian() ^ streamLittleEndian;
      }
      template <std::size_t DataSize>
      void loadBinary( void * const data, std::size_t size )
      {
        auto const readSize = static_cast<std::size_t>( itsStream.rdbuf()->sgetn( reinterpret_cast<char*>( data ), size ) );
        if(readSize != size)
          throw Exception("Failed to read " + std::to_string(size) + " bytes from input stream! Read " + std::to_string(readSize));
        if( itsConvertEndianness )
        {
          std::uint8_t * ptr = reinterpret_cast<std::uint8_t*>( data );
          for( std::size_t i = 0; i < size; i += DataSize )
            portable_binary_detail::swap_bytes<DataSize>( ptr );
        }
      }
    private:
      std::istream & itsStream;
      bool itsConvertEndianness;
  };
  template<class T> inline
  typename std::enable_if<std::is_arithmetic<T>::value, void>::type
  CEREAL_SAVE_FUNCTION_NAME(PortableBinaryOutputArchive & ar, T const & t)
  {
    static_assert( !std::is_floating_point<T>::value ||
                   (std::is_floating_point<T>::value && std::numeric_limits<T>::is_iec559),
                   "Portable binary only supports IEEE 754 standardized floating point" );
    ar.saveBinary(std::addressof(t), sizeof(t));
  }
  template<class T> inline
  typename std::enable_if<std::is_arithmetic<T>::value, void>::type
  CEREAL_LOAD_FUNCTION_NAME(PortableBinaryInputArchive & ar, T & t)
  {
    static_assert( !std::is_floating_point<T>::value ||
                   (std::is_floating_point<T>::value && std::numeric_limits<T>::is_iec559),
                   "Portable binary only supports IEEE 754 standardized floating point" );
    ar.template loadBinary<sizeof(T)>(std::addressof(t), sizeof(t));
  }
  template <class Archive, class T> inline
  CEREAL_ARCHIVE_RESTRICT(PortableBinaryInputArchive, PortableBinaryOutputArchive)
  CEREAL_SERIALIZE_FUNCTION_NAME( Archive & ar, NameValuePair<T> & t )
  {
    ar( t.value );
  }
  template <class Archive, class T> inline
  CEREAL_ARCHIVE_RESTRICT(PortableBinaryInputArchive, PortableBinaryOutputArchive)
  CEREAL_SERIALIZE_FUNCTION_NAME( Archive & ar, SizeTag<T> & t )
  {
    ar( t.size );
  }
  template <class T> inline
  void CEREAL_SAVE_FUNCTION_NAME(PortableBinaryOutputArchive & ar, BinaryData<T> const & bd)
  {
    typedef typename std::remove_pointer<T>::type TT;
    static_assert( !std::is_floating_point<TT>::value ||
                   (std::is_floating_point<TT>::value && std::numeric_limits<TT>::is_iec559),
                   "Portable binary only supports IEEE 754 standardized floating point" );
    ar.saveBinary( bd.data, static_cast<std::size_t>( bd.size ) );
  }
  template <class T> inline
  void CEREAL_LOAD_FUNCTION_NAME(PortableBinaryInputArchive & ar, BinaryData<T> & bd)
  {
    typedef typename std::remove_pointer<T>::type TT;
    static_assert( !std::is_floating_point<TT>::value ||
                   (std::is_floating_point<TT>::value && std::numeric_limits<TT>::is_iec559),
                   "Portable binary only supports IEEE 754 standardized floating point" );
    ar.template loadBinary<sizeof(TT)>( bd.data, static_cast<std::size_t>( bd.size ) );
  }
}
CEREAL_REGISTER_ARCHIVE(cereal::PortableBinaryOutputArchive)
CEREAL_REGISTER_ARCHIVE(cereal::PortableBinaryInputArchive)
CEREAL_SETUP_ARCHIVE_TRAITS(cereal::PortableBinaryInputArchive, cereal::PortableBinaryOutputArchive)
#endif
/*#include <cereal/archives/xml.hpp>*/
#ifndef CEREAL_ARCHIVES_XML_HPP_
#define CEREAL_ARCHIVES_XML_HPP_
/*#include <cereal/cereal.hpp> skipped */
/*#include <cereal/details/util.hpp> skipped */
/*#include <cereal/external/rapidxml/rapidxml.hpp>*/
#ifndef RAPIDXML_HPP_INCLUDED
#define RAPIDXML_HPP_INCLUDED
#if !defined(RAPIDXML_NO_STDLIB)
    #include <cstdlib>
    #include <cassert>
    #include <new>
#endif
#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4127)
    #pragma warning(disable:4100)
#endif
#if defined(RAPIDXML_NO_EXCEPTIONS)
#define RAPIDXML_PARSE_ERROR(what, where) { parse_error_handler(what, where); assert(0); }
namespace rapidxml
{
    void parse_error_handler(const char *what, void *where);
}
#else
#include <exception>
#define RAPIDXML_PARSE_ERROR(what, where) throw parse_error(what, where)
namespace rapidxml
{
    class parse_error: public std::exception
    {
    public:
        parse_error(const char *what_, void *where_)
            : m_what(what_)
            , m_where(where_)
        {
        }
        virtual const char *what() const throw()
        {
            return m_what;
        }
        template<class Ch>
        Ch *where() const
        {
            return reinterpret_cast<Ch *>(m_where);
        }
    private:
        const char *m_what;
        void *m_where;
    };
}
#endif
#ifndef RAPIDXML_STATIC_POOL_SIZE
    #define RAPIDXML_STATIC_POOL_SIZE (64 * 1024)
#endif
#ifndef RAPIDXML_DYNAMIC_POOL_SIZE
    #define RAPIDXML_DYNAMIC_POOL_SIZE (64 * 1024)
#endif
#ifndef RAPIDXML_ALIGNMENT
    #define RAPIDXML_ALIGNMENT sizeof(void *)
#endif
namespace rapidxml
{
    template<class Ch> class xml_node;
    template<class Ch> class xml_attribute;
    template<class Ch> class xml_document;
    enum node_type
    {
        node_document,
        node_element,
        node_data,
        node_cdata,
        node_comment,
        node_declaration,
        node_doctype,
        node_pi
    };
    const int parse_no_data_nodes = 0x1;
    const int parse_no_element_values = 0x2;
    const int parse_no_string_terminators = 0x4;
    const int parse_no_entity_translation = 0x8;
    const int parse_no_utf8 = 0x10;
    const int parse_declaration_node = 0x20;
    const int parse_comment_nodes = 0x40;
    const int parse_doctype_node = 0x80;
    const int parse_pi_nodes = 0x100;
    const int parse_validate_closing_tags = 0x200;
    const int parse_trim_whitespace = 0x400;
    const int parse_normalize_whitespace = 0x800;
    const int parse_default = 0;
    const int parse_non_destructive = parse_no_string_terminators | parse_no_entity_translation;
    const int parse_fastest = parse_non_destructive | parse_no_data_nodes;
    const int parse_full = parse_declaration_node | parse_comment_nodes | parse_doctype_node | parse_pi_nodes | parse_validate_closing_tags;
    namespace internal
    {
        template<int Dummy>
        struct lookup_tables
        {
            static const unsigned char lookup_whitespace[256];
            static const unsigned char lookup_node_name[256];
            static const unsigned char lookup_text[256];
            static const unsigned char lookup_text_pure_no_ws[256];
            static const unsigned char lookup_text_pure_with_ws[256];
            static const unsigned char lookup_attribute_name[256];
            static const unsigned char lookup_attribute_data_1[256];
            static const unsigned char lookup_attribute_data_1_pure[256];
            static const unsigned char lookup_attribute_data_2[256];
            static const unsigned char lookup_attribute_data_2_pure[256];
            static const unsigned char lookup_digits[256];
            static const unsigned char lookup_upcase[256];
        };
        template<class Ch>
        inline std::size_t measure(const Ch *p)
        {
            const Ch *tmp = p;
            while (*tmp)
                ++tmp;
            return tmp - p;
        }
        template<class Ch>
        inline bool compare(const Ch *p1, std::size_t size1, const Ch *p2, std::size_t size2, bool case_sensitive)
        {
            if (size1 != size2)
                return false;
            if (case_sensitive)
            {
                for (const Ch *end = p1 + size1; p1 < end; ++p1, ++p2)
                    if (*p1 != *p2)
                        return false;
            }
            else
            {
                for (const Ch *end = p1 + size1; p1 < end; ++p1, ++p2)
                    if (lookup_tables<0>::lookup_upcase[static_cast<unsigned char>(*p1)] != lookup_tables<0>::lookup_upcase[static_cast<unsigned char>(*p2)])
                        return false;
            }
            return true;
        }
        template<class Ch>
        inline bool preserve_space(xml_node<Ch>* node)
        {
            const Ch preserve_value[] = { Ch('p'), Ch('r'), Ch('e'), Ch('s'), Ch('e'), Ch('r'), Ch('v'), Ch('e') };
            const xml_attribute<Ch>* space = node->first_attribute("xml:space");
            return space && internal::compare(space->value(), space->value_size(), preserve_value, sizeof(preserve_value) / sizeof(Ch), true);
        }
    }
    template<class Ch = char>
    class memory_pool
    {
    public:
        typedef void *(alloc_func)(std::size_t);
        typedef void (free_func)(void *);
        memory_pool()
            : m_alloc_func(0)
            , m_free_func(0)
        {
            init();
        }
        ~memory_pool()
        {
            clear();
        }
        xml_node<Ch> *allocate_node(node_type type,
                                    const Ch *name = 0, const Ch *value = 0,
                                    std::size_t name_size = 0, std::size_t value_size = 0)
        {
            void *memory = allocate_aligned(sizeof(xml_node<Ch>));
            xml_node<Ch> *node = new(memory) xml_node<Ch>(type);
            if (name)
            {
                if (name_size > 0)
                    node->name(name, name_size);
                else
                    node->name(name);
            }
            if (value)
            {
                if (value_size > 0)
                    node->value(value, value_size);
                else
                    node->value(value);
            }
            return node;
        }
        xml_attribute<Ch> *allocate_attribute(const Ch *name = 0, const Ch *value = 0,
                                              std::size_t name_size = 0, std::size_t value_size = 0)
        {
            void *memory = allocate_aligned(sizeof(xml_attribute<Ch>));
            xml_attribute<Ch> *attribute = new(memory) xml_attribute<Ch>;
            if (name)
            {
                if (name_size > 0)
                    attribute->name(name, name_size);
                else
                    attribute->name(name);
            }
            if (value)
            {
                if (value_size > 0)
                    attribute->value(value, value_size);
                else
                    attribute->value(value);
            }
            return attribute;
        }
        Ch *allocate_string(const Ch *source = 0, std::size_t size = 0)
        {
            assert(source || size);
            if (size == 0)
                size = internal::measure(source) + 1;
            Ch *result = static_cast<Ch *>(allocate_aligned(size * sizeof(Ch)));
            if (source)
                for (std::size_t i = 0; i < size; ++i)
                    result[i] = source[i];
            return result;
        }
        xml_node<Ch> *clone_node(const xml_node<Ch> *source, xml_node<Ch> *result = 0)
        {
            if (result)
            {
                result->remove_all_attributes();
                result->remove_all_nodes();
                result->type(source->type());
            }
            else
                result = allocate_node(source->type());
            result->name(source->name(), source->name_size());
            result->value(source->value(), source->value_size());
            for (xml_node<Ch> *child = source->first_node(); child; child = child->next_sibling())
                result->append_node(clone_node(child));
            for (xml_attribute<Ch> *attr = source->first_attribute(); attr; attr = attr->next_attribute())
                result->append_attribute(allocate_attribute(attr->name(), attr->value(), attr->name_size(), attr->value_size()));
            return result;
        }
        void clear()
        {
            while (m_begin != m_static_memory)
            {
                char *previous_begin = reinterpret_cast<header *>(align(m_begin))->previous_begin;
                if (m_free_func)
                    m_free_func(m_begin);
                else
                    delete[] m_begin;
                m_begin = previous_begin;
            }
            init();
        }
        void set_allocator(alloc_func *af, free_func *ff)
        {
            assert(m_begin == m_static_memory && m_ptr == align(m_begin));
            m_alloc_func = af;
            m_free_func = ff;
        }
    private:
        struct header
        {
            char *previous_begin;
        };
        void init()
        {
            m_begin = m_static_memory;
            m_ptr = align(m_begin);
            m_end = m_static_memory + sizeof(m_static_memory);
        }
        char *align(char *ptr)
        {
            std::size_t alignment = ((RAPIDXML_ALIGNMENT - (std::size_t(ptr) & (RAPIDXML_ALIGNMENT - 1))) & (RAPIDXML_ALIGNMENT - 1));
            return ptr + alignment;
        }
        char *allocate_raw(std::size_t size)
        {
            void *memory;
            if (m_alloc_func)
            {
                memory = m_alloc_func(size);
                assert(memory);
            }
            else
            {
                memory = new char[size];
#ifdef RAPIDXML_NO_EXCEPTIONS
                if (!memory)
                    RAPIDXML_PARSE_ERROR("out of memory", 0);
#endif
            }
            return static_cast<char *>(memory);
        }
        void *allocate_aligned(std::size_t size)
        {
            char *result = align(m_ptr);
            if (result + size > m_end)
            {
                std::size_t pool_size = RAPIDXML_DYNAMIC_POOL_SIZE;
                if (pool_size < size)
                    pool_size = size;
                std::size_t alloc_size = sizeof(header) + (2 * RAPIDXML_ALIGNMENT - 2) + pool_size;
                char *raw_memory = allocate_raw(alloc_size);
                char *pool = align(raw_memory);
                header *new_header = reinterpret_cast<header *>(pool);
                new_header->previous_begin = m_begin;
                m_begin = raw_memory;
                m_ptr = pool + sizeof(header);
                m_end = raw_memory + alloc_size;
                result = align(m_ptr);
            }
            m_ptr = result + size;
            return result;
        }
        char *m_begin;
        char *m_ptr;
        char *m_end;
        char m_static_memory[RAPIDXML_STATIC_POOL_SIZE];
        alloc_func *m_alloc_func;
        free_func *m_free_func;
    };
    template<class Ch = char>
    class xml_base
    {
    public:
        xml_base()
            : m_name(0)
            , m_value(0)
            , m_parent(0)
        {
        }
        Ch *name() const
        {
            return m_name ? m_name : nullstr();
        }
        std::size_t name_size() const
        {
            return m_name ? m_name_size : 0;
        }
        Ch *value() const
        {
            return m_value ? m_value : nullstr();
        }
        std::size_t value_size() const
        {
            return m_value ? m_value_size : 0;
        }
        void name(const Ch *name_, std::size_t size)
        {
            m_name = const_cast<Ch *>(name_);
            m_name_size = size;
        }
        void name(const Ch *name_)
        {
            this->name(name_, internal::measure(name_));
        }
        void value(const Ch *value_, std::size_t size)
        {
            m_value = const_cast<Ch *>(value_);
            m_value_size = size;
        }
        void value(const Ch *value_)
        {
            this->value(value_, internal::measure(value_));
        }
        xml_node<Ch> *parent() const
        {
            return m_parent;
        }
    protected:
        static Ch *nullstr()
        {
            static Ch zero = Ch('\0');
            return &zero;
        }
        Ch *m_name;
        Ch *m_value;
        std::size_t m_name_size;
        std::size_t m_value_size;
        xml_node<Ch> *m_parent;
    };
    template<class Ch = char>
    class xml_attribute: public xml_base<Ch>
    {
        friend class xml_node<Ch>;
    public:
        xml_attribute()
        {
        }
        xml_document<Ch> *document() const
        {
            if (xml_node<Ch> *node = this->parent())
            {
                while (node->parent())
                    node = node->parent();
                return node->type() == node_document ? static_cast<xml_document<Ch> *>(node) : 0;
            }
            else
                return 0;
        }
        xml_attribute<Ch> *previous_attribute(const Ch *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const
        {
            if (name)
            {
                if (name_size == 0)
                    name_size = internal::measure(name);
                for (xml_attribute<Ch> *attribute = m_prev_attribute; attribute; attribute = attribute->m_prev_attribute)
                    if (internal::compare(attribute->name(), attribute->name_size(), name, name_size, case_sensitive))
                        return attribute;
                return 0;
            }
            else
                return this->m_parent ? m_prev_attribute : 0;
        }
        xml_attribute<Ch> *next_attribute(const Ch *name_ = 0, std::size_t name_size_ = 0, bool case_sensitive = true) const
        {
            if (name_)
            {
                if (name_size_ == 0)
                    name_size_ = internal::measure(name_);
                for (xml_attribute<Ch> *attribute = m_next_attribute; attribute; attribute = attribute->m_next_attribute)
                    if (internal::compare(attribute->name(), attribute->name_size(), name_, name_size_, case_sensitive))
                        return attribute;
                return 0;
            }
            else
                return this->m_parent ? m_next_attribute : 0;
        }
    private:
        xml_attribute<Ch> *m_prev_attribute;
        xml_attribute<Ch> *m_next_attribute;
    };
    template<class Ch = char>
    class xml_node: public xml_base<Ch>
    {
    public:
        xml_node(node_type type_)
            : m_type(type_)
            , m_first_node(0)
            , m_first_attribute(0)
        {
        }
        node_type type() const
        {
            return m_type;
        }
        xml_document<Ch> *document() const
        {
            xml_node<Ch> *node = const_cast<xml_node<Ch> *>(this);
            while (node->parent())
                node = node->parent();
            return node->type() == node_document ? static_cast<xml_document<Ch> *>(node) : 0;
        }
        xml_node<Ch> *first_node(const Ch *name_ = 0, std::size_t name_size_ = 0, bool case_sensitive = true) const
        {
            if (name_)
            {
                if (name_size_ == 0)
                    name_size_ = internal::measure(name_);
                for (xml_node<Ch> *child = m_first_node; child; child = child->next_sibling())
                    if (internal::compare(child->name(), child->name_size(), name_, name_size_, case_sensitive))
                        return child;
                return 0;
            }
            else
                return m_first_node;
        }
        xml_node<Ch> *last_node(const Ch *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const
        {
            assert(m_first_node);
            if (name)
            {
                if (name_size == 0)
                    name_size = internal::measure(name);
                for (xml_node<Ch> *child = m_last_node; child; child = child->previous_sibling())
                    if (internal::compare(child->name(), child->name_size(), name, name_size, case_sensitive))
                        return child;
                return 0;
            }
            else
                return m_last_node;
        }
        xml_node<Ch> *previous_sibling(const Ch *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const
        {
            assert(this->m_parent);
            if (name)
            {
                if (name_size == 0)
                    name_size = internal::measure(name);
                for (xml_node<Ch> *sibling = m_prev_sibling; sibling; sibling = sibling->m_prev_sibling)
                    if (internal::compare(sibling->name(), sibling->name_size(), name, name_size, case_sensitive))
                        return sibling;
                return 0;
            }
            else
                return m_prev_sibling;
        }
        xml_node<Ch> *next_sibling(const Ch *name_ = 0, std::size_t name_size_ = 0, bool case_sensitive = true) const
        {
            assert(this->m_parent);
            if (name_)
            {
                if (name_size_ == 0)
                    name_size_ = internal::measure(name_);
                for (xml_node<Ch> *sibling = m_next_sibling; sibling; sibling = sibling->m_next_sibling)
                    if (internal::compare(sibling->name(), sibling->name_size(), name_, name_size_, case_sensitive))
                        return sibling;
                return 0;
            }
            else
                return m_next_sibling;
        }
        xml_attribute<Ch> *first_attribute(const Ch *name_ = 0, std::size_t name_size_ = 0, bool case_sensitive = true) const
        {
            if (name_)
            {
                if (name_size_ == 0)
                    name_size_ = internal::measure(name_);
                for (xml_attribute<Ch> *attribute = m_first_attribute; attribute; attribute = attribute->m_next_attribute)
                    if (internal::compare(attribute->name(), attribute->name_size(), name_, name_size_, case_sensitive))
                        return attribute;
                return 0;
            }
            else
                return m_first_attribute;
        }
        xml_attribute<Ch> *last_attribute(const Ch *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const
        {
            if (name)
            {
                if (name_size == 0)
                    name_size = internal::measure(name);
                for (xml_attribute<Ch> *attribute = m_last_attribute; attribute; attribute = attribute->m_prev_attribute)
                    if (internal::compare(attribute->name(), attribute->name_size(), name, name_size, case_sensitive))
                        return attribute;
                return 0;
            }
            else
                return m_first_attribute ? m_last_attribute : 0;
        }
        void type(node_type type_)
        {
            m_type = type_;
        }
        void prepend_node(xml_node<Ch> *child)
        {
            assert(child && !child->parent() && child->type() != node_document);
            if (first_node())
            {
                child->m_next_sibling = m_first_node;
                m_first_node->m_prev_sibling = child;
            }
            else
            {
                child->m_next_sibling = 0;
                m_last_node = child;
            }
            m_first_node = child;
            child->m_parent = this;
            child->m_prev_sibling = 0;
        }
        void append_node(xml_node<Ch> *child)
        {
            assert(child && !child->parent() && child->type() != node_document);
            if (first_node())
            {
                child->m_prev_sibling = m_last_node;
                m_last_node->m_next_sibling = child;
            }
            else
            {
                child->m_prev_sibling = 0;
                m_first_node = child;
            }
            m_last_node = child;
            child->m_parent = this;
            child->m_next_sibling = 0;
        }
        void insert_node(xml_node<Ch> *where, xml_node<Ch> *child)
        {
            assert(!where || where->parent() == this);
            assert(child && !child->parent() && child->type() != node_document);
            if (where == m_first_node)
                prepend_node(child);
            else if (where == 0)
                append_node(child);
            else
            {
                child->m_prev_sibling = where->m_prev_sibling;
                child->m_next_sibling = where;
                where->m_prev_sibling->m_next_sibling = child;
                where->m_prev_sibling = child;
                child->m_parent = this;
            }
        }
        void remove_first_node()
        {
            assert(first_node());
            xml_node<Ch> *child = m_first_node;
            m_first_node = child->m_next_sibling;
            if (child->m_next_sibling)
                child->m_next_sibling->m_prev_sibling = 0;
            else
                m_last_node = 0;
            child->m_parent = 0;
        }
        void remove_last_node()
        {
            assert(first_node());
            xml_node<Ch> *child = m_last_node;
            if (child->m_prev_sibling)
            {
                m_last_node = child->m_prev_sibling;
                child->m_prev_sibling->m_next_sibling = 0;
            }
            else
                m_first_node = 0;
            child->m_parent = 0;
        }
        void remove_node(xml_node<Ch> *where)
        {
            assert(where && where->parent() == this);
            assert(first_node());
            if (where == m_first_node)
                remove_first_node();
            else if (where == m_last_node)
                remove_last_node();
            else
            {
                where->m_prev_sibling->m_next_sibling = where->m_next_sibling;
                where->m_next_sibling->m_prev_sibling = where->m_prev_sibling;
                where->m_parent = 0;
            }
        }
        void remove_all_nodes()
        {
            for (xml_node<Ch> *node = first_node(); node; node = node->m_next_sibling)
                node->m_parent = 0;
            m_first_node = 0;
        }
        void prepend_attribute(xml_attribute<Ch> *attribute)
        {
            assert(attribute && !attribute->parent());
            if (first_attribute())
            {
                attribute->m_next_attribute = m_first_attribute;
                m_first_attribute->m_prev_attribute = attribute;
            }
            else
            {
                attribute->m_next_attribute = 0;
                m_last_attribute = attribute;
            }
            m_first_attribute = attribute;
            attribute->m_parent = this;
            attribute->m_prev_attribute = 0;
        }
        void append_attribute(xml_attribute<Ch> *attribute)
        {
            assert(attribute && !attribute->parent());
            if (first_attribute())
            {
                attribute->m_prev_attribute = m_last_attribute;
                m_last_attribute->m_next_attribute = attribute;
            }
            else
            {
                attribute->m_prev_attribute = 0;
                m_first_attribute = attribute;
            }
            m_last_attribute = attribute;
            attribute->m_parent = this;
            attribute->m_next_attribute = 0;
        }
        void insert_attribute(xml_attribute<Ch> *where, xml_attribute<Ch> *attribute)
        {
            assert(!where || where->parent() == this);
            assert(attribute && !attribute->parent());
            if (where == m_first_attribute)
                prepend_attribute(attribute);
            else if (where == 0)
                append_attribute(attribute);
            else
            {
                attribute->m_prev_attribute = where->m_prev_attribute;
                attribute->m_next_attribute = where;
                where->m_prev_attribute->m_next_attribute = attribute;
                where->m_prev_attribute = attribute;
                attribute->m_parent = this;
            }
        }
        void remove_first_attribute()
        {
            assert(first_attribute());
            xml_attribute<Ch> *attribute = m_first_attribute;
            if (attribute->m_next_attribute)
            {
                attribute->m_next_attribute->m_prev_attribute = 0;
            }
            else
                m_last_attribute = 0;
            attribute->m_parent = 0;
            m_first_attribute = attribute->m_next_attribute;
        }
        void remove_last_attribute()
        {
            assert(first_attribute());
            xml_attribute<Ch> *attribute = m_last_attribute;
            if (attribute->m_prev_attribute)
            {
                attribute->m_prev_attribute->m_next_attribute = 0;
                m_last_attribute = attribute->m_prev_attribute;
            }
            else
                m_first_attribute = 0;
            attribute->m_parent = 0;
        }
        void remove_attribute(xml_attribute<Ch> *where)
        {
            assert(first_attribute() && where->parent() == this);
            if (where == m_first_attribute)
                remove_first_attribute();
            else if (where == m_last_attribute)
                remove_last_attribute();
            else
            {
                where->m_prev_attribute->m_next_attribute = where->m_next_attribute;
                where->m_next_attribute->m_prev_attribute = where->m_prev_attribute;
                where->m_parent = 0;
            }
        }
        void remove_all_attributes()
        {
            for (xml_attribute<Ch> *attribute = first_attribute(); attribute; attribute = attribute->m_next_attribute)
                attribute->m_parent = 0;
            m_first_attribute = 0;
        }
    private:
        xml_node(const xml_node &);
        void operator =(const xml_node &);
        node_type m_type;
        xml_node<Ch> *m_first_node;
        xml_node<Ch> *m_last_node;
        xml_attribute<Ch> *m_first_attribute;
        xml_attribute<Ch> *m_last_attribute;
        xml_node<Ch> *m_prev_sibling;
        xml_node<Ch> *m_next_sibling;
    };
    template<class Ch = char>
    class xml_document: public xml_node<Ch>, public memory_pool<Ch>
    {
    public:
        xml_document()
            : xml_node<Ch>(node_document)
        {
        }
        template<int Flags>
        void parse(Ch *text)
        {
            assert(text);
            this->remove_all_nodes();
            this->remove_all_attributes();
            parse_bom<Flags>(text);
            while (1)
            {
                skip<whitespace_pred, Flags>(text);
                if (*text == 0)
                    break;
                if (*text == Ch('<'))
                {
                    ++text;
                    if (xml_node<Ch> *node = parse_node<Flags>(text))
                        this->append_node(node);
                }
                else
                    RAPIDXML_PARSE_ERROR("expected <", text);
            }
        }
        void clear()
        {
            this->remove_all_nodes();
            this->remove_all_attributes();
            memory_pool<Ch>::clear();
        }
    private:
        struct whitespace_pred
        {
            static unsigned char test(Ch ch)
            {
                return internal::lookup_tables<0>::lookup_whitespace[static_cast<unsigned char>(ch)];
            }
        };
        struct node_name_pred
        {
            static unsigned char test(Ch ch)
            {
                return internal::lookup_tables<0>::lookup_node_name[static_cast<unsigned char>(ch)];
            }
        };
        struct attribute_name_pred
        {
            static unsigned char test(Ch ch)
            {
                return internal::lookup_tables<0>::lookup_attribute_name[static_cast<unsigned char>(ch)];
            }
        };
        struct text_pred
        {
            static unsigned char test(Ch ch)
            {
                return internal::lookup_tables<0>::lookup_text[static_cast<unsigned char>(ch)];
            }
        };
        struct text_pure_no_ws_pred
        {
            static unsigned char test(Ch ch)
            {
                return internal::lookup_tables<0>::lookup_text_pure_no_ws[static_cast<unsigned char>(ch)];
            }
        };
        struct text_pure_with_ws_pred
        {
            static unsigned char test(Ch ch)
            {
                return internal::lookup_tables<0>::lookup_text_pure_with_ws[static_cast<unsigned char>(ch)];
            }
        };
        template<Ch Quote>
        struct attribute_value_pred
        {
            static unsigned char test(Ch ch)
            {
                if (Quote == Ch('\''))
                    return internal::lookup_tables<0>::lookup_attribute_data_1[static_cast<unsigned char>(ch)];
                if (Quote == Ch('\"'))
                    return internal::lookup_tables<0>::lookup_attribute_data_2[static_cast<unsigned char>(ch)];
                return 0;       // Should never be executed, to avoid warnings on Comeau
            }
        };
        // Detect attribute value character
        template<Ch Quote>
        struct attribute_value_pure_pred
        {
            static unsigned char test(Ch ch)
            {
                if (Quote == Ch('\''))
                    return internal::lookup_tables<0>::lookup_attribute_data_1_pure[static_cast<unsigned char>(ch)];
                if (Quote == Ch('\"'))
                    return internal::lookup_tables<0>::lookup_attribute_data_2_pure[static_cast<unsigned char>(ch)];
                return 0;       // Should never be executed, to avoid warnings on Comeau
            }
        };
        // Insert coded character, using UTF8 or 8-bit ASCII
        template<int Flags>
        static void insert_coded_character(Ch *&text, unsigned long code)
        {
            if (Flags & parse_no_utf8)
            {
                // Insert 8-bit ASCII character
                // Todo: possibly verify that code is less than 256 and use replacement char otherwise?
                text[0] = static_cast<unsigned char>(code);
                text += 1;
            }
            else
            {
                // Insert UTF8 sequence
                if (code < 0x80)    // 1 byte sequence
                {
	                text[0] = static_cast<unsigned char>(code);
                    text += 1;
                }
                else if (code < 0x800)  // 2 byte sequence
                {
	                text[1] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
	                text[0] = static_cast<unsigned char>(code | 0xC0);
                    text += 2;
                }
	            else if (code < 0x10000)    // 3 byte sequence
                {
	                text[2] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
	                text[1] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
	                text[0] = static_cast<unsigned char>(code | 0xE0);
                    text += 3;
                }
	            else if (code < 0x110000)   // 4 byte sequence
                {
	                text[3] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
	                text[2] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
	                text[1] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
	                text[0] = static_cast<unsigned char>(code | 0xF0);
                    text += 4;
                }
                else    // Invalid, only codes up to 0x10FFFF are allowed in Unicode
                {
                    RAPIDXML_PARSE_ERROR("invalid numeric character entity", text);
                }
            }
        }
        // Skip characters until predicate evaluates to true
        template<class StopPred, int Flags>
        static void skip(Ch *&text)
        {
            Ch *tmp = text;
            while (StopPred::test(*tmp))
                ++tmp;
            text = tmp;
        }
        // Skip characters until predicate evaluates to true while doing the following:
        // - replacing XML character entity references with proper characters (&apos; &amp; &quot; &lt; &gt; &#...;)
        // - condensing whitespace sequences to single space character
        template<class StopPred, class StopPredPure, int Flags>
        static Ch *skip_and_expand_character_refs(Ch *&text, bool preserve_space)
        {
            // If entity translation, whitespace condense and whitespace trimming is disabled, use plain skip
            if (Flags & parse_no_entity_translation &&
                !(Flags & parse_normalize_whitespace) &&
                !(Flags & parse_trim_whitespace))
            {
                skip<StopPred, Flags>(text);
                return text;
            }
            // Use simple skip until first modification is detected
            skip<StopPredPure, Flags>(text);
            // Use translation skip
            Ch *src = text;
            Ch *dest = src;
            while (StopPred::test(*src))
            {
                // If entity translation is enabled
                if (!(Flags & parse_no_entity_translation))
                {
                    // Test if replacement is needed
                    if (src[0] == Ch('&'))
                    {
                        switch (src[1])
                        {
                        // &amp; &apos;
                        case Ch('a'):
                            if (src[2] == Ch('m') && src[3] == Ch('p') && src[4] == Ch(';'))
                            {
                                *dest = Ch('&');
                                ++dest;
                                src += 5;
                                continue;
                            }
                            if (src[2] == Ch('p') && src[3] == Ch('o') && src[4] == Ch('s') && src[5] == Ch(';'))
                            {
                                *dest = Ch('\'');
                                ++dest;
                                src += 6;
                                continue;
                            }
                            break;
                        case Ch('q'):
                            if (src[2] == Ch('u') && src[3] == Ch('o') && src[4] == Ch('t') && src[5] == Ch(';'))
                            {
                                *dest = Ch('"');
                                ++dest;
                                src += 6;
                                continue;
                            }
                            break;
                        case Ch('g'):
                            if (src[2] == Ch('t') && src[3] == Ch(';'))
                            {
                                *dest = Ch('>');
                                ++dest;
                                src += 4;
                                continue;
                            }
                            break;
                        case Ch('l'):
                            if (src[2] == Ch('t') && src[3] == Ch(';'))
                            {
                                *dest = Ch('<');
                                ++dest;
                                src += 4;
                                continue;
                            }
                            break;
                        case Ch('#'):
                            if (src[2] == Ch('x'))
                            {
                                unsigned long code = 0;
                                src += 3;
                                while (1)
                                {
                                    unsigned char digit = internal::lookup_tables<0>::lookup_digits[static_cast<unsigned char>(*src)];
                                    if (digit == 0xFF)
                                        break;
                                    code = code * 16 + digit;
                                    ++src;
                                }
                                insert_coded_character<Flags>(dest, code);
                            }
                            else
                            {
                                unsigned long code = 0;
                                src += 2;
                                while (1)
                                {
                                    unsigned char digit = internal::lookup_tables<0>::lookup_digits[static_cast<unsigned char>(*src)];
                                    if (digit == 0xFF)
                                        break;
                                    code = code * 10 + digit;
                                    ++src;
                                }
                                insert_coded_character<Flags>(dest, code);
                            }
                            if (*src == Ch(';'))
                                ++src;
                            else
                                RAPIDXML_PARSE_ERROR("expected ;", src);
                            continue;
                        default:
                            break;
                        }
                    }
                }
                if ((Flags & parse_normalize_whitespace) && !preserve_space)
                {
                    if (whitespace_pred::test(*src))
                    {
                        *dest = Ch(' '); ++dest;
                        ++src;
                        while (whitespace_pred::test(*src))
                            ++src;
                        continue;
                    }
                }
                *dest++ = *src++;
            }
            text = src;
            return dest;
        }
        template<int Flags>
        void parse_bom(Ch *&text)
        {
            if (static_cast<unsigned char>(text[0]) == 0xEF &&
                static_cast<unsigned char>(text[1]) == 0xBB &&
                static_cast<unsigned char>(text[2]) == 0xBF)
            {
                text += 3;
            }
        }
        template<int Flags>
        xml_node<Ch> *parse_xml_declaration(Ch *&text)
        {
            if (!(Flags & parse_declaration_node))
            {
                while (text[0] != Ch('?') || text[1] != Ch('>'))
                {
                    if (!text[0])
                        RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                    ++text;
                }
                text += 2;
                return 0;
            }
            xml_node<Ch> *declaration = this->allocate_node(node_declaration);
            skip<whitespace_pred, Flags>(text);
            parse_node_attributes<Flags>(text, declaration);
            if (text[0] != Ch('?') || text[1] != Ch('>'))
                RAPIDXML_PARSE_ERROR("expected ?>", text);
            text += 2;
            return declaration;
        }
        template<int Flags>
        xml_node<Ch> *parse_comment(Ch *&text)
        {
            if (!(Flags & parse_comment_nodes))
            {
                while (text[0] != Ch('-') || text[1] != Ch('-') || text[2] != Ch('>'))
                {
                    if (!text[0])
                        RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                    ++text;
                }
                text += 3;
                return 0;
            }
            Ch *value_ = text;
            while (text[0] != Ch('-') || text[1] != Ch('-') || text[2] != Ch('>'))
            {
                if (!text[0])
                    RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                ++text;
            }
            xml_node<Ch> *comment = this->allocate_node(node_comment);
            comment->value(value_, text - value_);
            if (!(Flags & parse_no_string_terminators))
                *text = Ch('\0');
            text += 3;
            return comment;
        }
        template<int Flags>
        xml_node<Ch> *parse_doctype(Ch *&text)
        {
            Ch *value_ = text;
            while (*text != Ch('>'))
            {
                switch (*text)
                {
                case Ch('['):
                {
                    ++text;
                    int depth = 1;
                    while (depth > 0)
                    {
                        switch (*text)
                        {
                            case Ch('['): ++depth; break;
                            case Ch(']'): --depth; break;
                            case 0: RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                        }
                        ++text;
                    }
                    break;
                }
                case Ch('\0'):
                    RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                default:
                    ++text;
                }
            }
            if (Flags & parse_doctype_node)
            {
                xml_node<Ch> *doctype = this->allocate_node(node_doctype);
                doctype->value(value_, text - value_);
                if (!(Flags & parse_no_string_terminators))
                    *text = Ch('\0');
                text += 1;
                return doctype;
            }
            else
            {
                text += 1;
                return 0;
            }
        }
        template<int Flags>
        xml_node<Ch> *parse_pi(Ch *&text)
        {
            if (Flags & parse_pi_nodes)
            {
                xml_node<Ch> *pi = this->allocate_node(node_pi);
                Ch *name_ = text;
                skip<node_name_pred, Flags>(text);
                if (text == name_)
                    RAPIDXML_PARSE_ERROR("expected PI target", text);
                pi->name(name_, text - name_);
                skip<whitespace_pred, Flags>(text);
                Ch *value_ = text;
                while (text[0] != Ch('?') || text[1] != Ch('>'))
                {
                    if (*text == Ch('\0'))
                        RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                    ++text;
                }
                pi->value(value_, text - value_);
                if (!(Flags & parse_no_string_terminators))
                {
                    pi->name()[pi->name_size()] = Ch('\0');
                    pi->value()[pi->value_size()] = Ch('\0');
                }
                text += 2;
                return pi;
            }
            else
            {
                while (text[0] != Ch('?') || text[1] != Ch('>'))
                {
                    if (*text == Ch('\0'))
                        RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                    ++text;
                }
                text += 2;
                return 0;
            }
        }
        template<int Flags>
        Ch parse_and_append_data(xml_node<Ch> *node, Ch *&text, Ch *contents_start)
        {
            if (!(Flags & parse_trim_whitespace))
                text = contents_start;
            const bool preserve_space =  internal::preserve_space(node);
            Ch *value_ = text, *end;
            if ((Flags & parse_normalize_whitespace) && !preserve_space)
                end = skip_and_expand_character_refs<text_pred, text_pure_with_ws_pred, Flags>(text, false);
            else
                end = skip_and_expand_character_refs<text_pred, text_pure_no_ws_pred, Flags>(text, preserve_space);
            if ((Flags & parse_trim_whitespace) && !preserve_space)
            {
                if (Flags & parse_normalize_whitespace)
                {
                    if (*(end - 1) == Ch(' '))
                        --end;
                }
                else
                {
                    while (whitespace_pred::test(*(end - 1)))
                        --end;
                }
            }
            if (!(Flags & parse_no_data_nodes))
            {
                xml_node<Ch> *data = this->allocate_node(node_data);
                data->value(value_, end - value_);
                node->append_node(data);
            }
            if (!(Flags & parse_no_element_values))
                if (*node->value() == Ch('\0'))
                    node->value(value_, end - value_);
            if (!(Flags & parse_no_string_terminators))
            {
                Ch ch = *text;
                *end = Ch('\0');
                return ch;
            }
            return *text;
        }
        template<int Flags>
        xml_node<Ch> *parse_cdata(Ch *&text)
        {
            if (Flags & parse_no_data_nodes)
            {
                while (text[0] != Ch(']') || text[1] != Ch(']') || text[2] != Ch('>'))
                {
                    if (!text[0])
                        RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                    ++text;
                }
                text += 3;
                return 0;
            }
            Ch *value_ = text;
            while (text[0] != Ch(']') || text[1] != Ch(']') || text[2] != Ch('>'))
            {
                if (!text[0])
                    RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                ++text;
            }
            xml_node<Ch> *cdata = this->allocate_node(node_cdata);
            cdata->value(value_, text - value_);
            if (!(Flags & parse_no_string_terminators))
                *text = Ch('\0');
            text += 3;
            return cdata;
        }
        template<int Flags>
        xml_node<Ch> *parse_element(Ch *&text)
        {
            xml_node<Ch> *element = this->allocate_node(node_element);
            Ch *name_ = text;
            skip<node_name_pred, Flags>(text);
            if (text == name_)
                RAPIDXML_PARSE_ERROR("expected element name", text);
            element->name(name_, text - name_);
            skip<whitespace_pred, Flags>(text);
            parse_node_attributes<Flags>(text, element);
            if (*text == Ch('>'))
            {
                ++text;
                parse_node_contents<Flags>(text, element);
            }
            else if (*text == Ch('/'))
            {
                ++text;
                if (*text != Ch('>'))
                    RAPIDXML_PARSE_ERROR("expected >", text);
                ++text;
            }
            else
                RAPIDXML_PARSE_ERROR("expected >", text);
            if (!(Flags & parse_no_string_terminators))
                element->name()[element->name_size()] = Ch('\0');
            return element;
        }
        template<int Flags>
        xml_node<Ch> *parse_node(Ch *&text)
        {
            switch (text[0])
            {
            default:
                return parse_element<Flags>(text);
            case Ch('?'):
                ++text;
                if ((text[0] == Ch('x') || text[0] == Ch('X')) &&
                    (text[1] == Ch('m') || text[1] == Ch('M')) &&
                    (text[2] == Ch('l') || text[2] == Ch('L')) &&
                    whitespace_pred::test(text[3]))
                {
                    text += 4;
                    return parse_xml_declaration<Flags>(text);
                }
                else
                {
                    return parse_pi<Flags>(text);
                }
            case Ch('!'):
                switch (text[1])
                {
                case Ch('-'):
                    if (text[2] == Ch('-'))
                    {
                        text += 3;
                        return parse_comment<Flags>(text);
                    }
                    break;
                case Ch('['):
                    if (text[2] == Ch('C') && text[3] == Ch('D') && text[4] == Ch('A') &&
                        text[5] == Ch('T') && text[6] == Ch('A') && text[7] == Ch('['))
                    {
                        text += 8;
                        return parse_cdata<Flags>(text);
                    }
                    break;
                case Ch('D'):
                    if (text[2] == Ch('O') && text[3] == Ch('C') && text[4] == Ch('T') &&
                        text[5] == Ch('Y') && text[6] == Ch('P') && text[7] == Ch('E') &&
                        whitespace_pred::test(text[8]))
                    {
                        text += 9;
                        return parse_doctype<Flags>(text);
                    }
                }
                ++text;
                while (*text != Ch('>'))
                {
                    if (*text == 0)
                        RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                    ++text;
                }
                ++text;
                return 0;
            }
        }
        template<int Flags>
        void parse_node_contents(Ch *&text, xml_node<Ch> *node)
        {
            while (1)
            {
                Ch *contents_start = text;
                skip<whitespace_pred, Flags>(text);
                Ch next_char = *text;
            after_data_node:
                switch (next_char)
                {
                case Ch('<'):
                    if (text[1] == Ch('/'))
                    {
                        Ch *contents_end = 0;
                        if (internal::preserve_space(node))
                        {
                            contents_end = text;
                        }
                        text += 2;
                        if (Flags & parse_validate_closing_tags)
                        {
                            Ch *closing_name = text;
                            skip<node_name_pred, Flags>(text);
                            if (!internal::compare(node->name(), node->name_size(), closing_name, text - closing_name, true))
                                RAPIDXML_PARSE_ERROR("invalid closing tag name", text);
                        }
                        else
                        {
                            skip<node_name_pred, Flags>(text);
                        }
                        skip<whitespace_pred, Flags>(text);
                        if (*text != Ch('>'))
                            RAPIDXML_PARSE_ERROR("expected >", text);
                        ++text;
                        if (contents_end && contents_end != contents_start)
                        {
                            node->value(contents_start, contents_end - contents_start);
                            node->value()[node->value_size()] = Ch('\0');
                        }
                        return;
                    }
                    else
                    {
                        ++text;
                        if (xml_node<Ch> *child = parse_node<Flags>(text))
                            node->append_node(child);
                    }
                    break;
                case Ch('\0'):
                    RAPIDXML_PARSE_ERROR("unexpected end of data", text);
                default:
                    next_char = parse_and_append_data<Flags>(node, text, contents_start);
                    goto after_data_node;
                }
            }
        }
        template<int Flags>
        void parse_node_attributes(Ch *&text, xml_node<Ch> *node)
        {
            while (attribute_name_pred::test(*text))
            {
                Ch *name_ = text;
                ++text;
                skip<attribute_name_pred, Flags>(text);
                if (text == name_)
                    RAPIDXML_PARSE_ERROR("expected attribute name", name_);
                xml_attribute<Ch> *attribute = this->allocate_attribute();
                attribute->name(name_, text - name_);
                node->append_attribute(attribute);
                skip<whitespace_pred, Flags>(text);
                if (*text != Ch('='))
                    RAPIDXML_PARSE_ERROR("expected =", text);
                ++text;
                if (!(Flags & parse_no_string_terminators))
                    attribute->name()[attribute->name_size()] = 0;
                skip<whitespace_pred, Flags>(text);
                Ch quote = *text;
                if (quote != Ch('\'') && quote != Ch('"'))
                    RAPIDXML_PARSE_ERROR("expected ' or \"", text);
                ++text;
                // Extract attribute value and expand char refs in it
                Ch *value_ = text, *end;
                const int AttFlags = Flags & ~parse_normalize_whitespace;   // No whitespace normalization in attributes
                if (quote == Ch('\''))
                    end = skip_and_expand_character_refs<attribute_value_pred<Ch('\'')>, attribute_value_pure_pred<Ch('\'')>, AttFlags>(text, false);
                else
                    end = skip_and_expand_character_refs<attribute_value_pred<Ch('"')>, attribute_value_pure_pred<Ch('"')>, AttFlags>(text, false);
                attribute->value(value_, end - value_);
                if (*text != quote)
                    RAPIDXML_PARSE_ERROR("expected ' or \"", text);
                ++text;     // Skip quote
                // Add terminating zero after value
                if (!(Flags & parse_no_string_terminators))
                    attribute->value()[attribute->value_size()] = 0;
                // Skip whitespace after attribute value
                skip<whitespace_pred, Flags>(text);
            }
        }
    };
    //! \cond internal
    namespace internal
    {
        // Whitespace (space \n \r \t)
        template<int Dummy>
        const unsigned char lookup_tables<Dummy>::lookup_whitespace[256] =
        {
          // 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
             0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  1,  0,  0,  // 0
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 1
             1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 2
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 3
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 4
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 5
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 6
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 7
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 8
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // 9
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // A
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // B
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // C
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // D
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  // E
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0   // F
        };
        // Node name (anything but space \n \r \t / > ? \0)
        template<int Dummy>
        const unsigned char lookup_tables<Dummy>::lookup_node_name[256] =
        {
          // 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
             0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  1,  1,  // 0
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
             0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  // 2
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  // 3
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
        };
        // Text (i.e. PCDATA) (anything but < \0)
        template<int Dummy>
        const unsigned char lookup_tables<Dummy>::lookup_text[256] =
        {
          // 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
             0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 0
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 2
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  // 3
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
        };
        // Text (i.e. PCDATA) that does not require processing when ws normalization is disabled
        // (anything but < \0 &)
        template<int Dummy>
        const unsigned char lookup_tables<Dummy>::lookup_text_pure_no_ws[256] =
        {
          // 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
             0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 0
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
             1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 2
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  // 3
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
        };
        // Text (i.e. PCDATA) that does not require processing when ws normalizationis is enabled
        // (anything but < \0 & space \n \r \t)
        template<int Dummy>
        const unsigned char lookup_tables<Dummy>::lookup_text_pure_with_ws[256] =
        {
          // 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
             0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  1,  1,  // 0
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
             0,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 2
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  // 3
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
        };
        // Attribute name (anything but space \n \r \t / < > = ? ! \0)
        template<int Dummy>
        const unsigned char lookup_tables<Dummy>::lookup_attribute_name[256] =
        {
          // 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
             0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  0,  1,  1,  // 0
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
             0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  // 2
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  // 3
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
        };
        // Attribute data with single quote (anything but ' \0)
        template<int Dummy>
        const unsigned char lookup_tables<Dummy>::lookup_attribute_data_1[256] =
        {
          // 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
             0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 0
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
             1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  // 2
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 3
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
        };
        // Attribute data with single quote that does not require processing (anything but ' \0 &)
        template<int Dummy>
        const unsigned char lookup_tables<Dummy>::lookup_attribute_data_1_pure[256] =
        {
          // 0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
             0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 0
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 1
             1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  // 2
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 3
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 4
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 5
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 6
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 7
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 8
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // 9
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // A
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // B
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // C
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // D
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  // E
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1   // F
        };
        // Attribute data with double quote (anything but " \0)
        template<int Dummy>
        const unsigned char lookup_tables<Dummy>::lookup_attribute_data_2[256] =
        {
             0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
        };
        template<int Dummy>
        const unsigned char lookup_tables<Dummy>::lookup_attribute_data_2_pure[256] =
        {
             0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
             1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
        };
        template<int Dummy>
        const unsigned char lookup_tables<Dummy>::lookup_digits[256] =
        {
           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
             0,  1,  2,  3,  4,  5,  6,  7,  8,  9,255,255,255,255,255,255,
           255, 10, 11, 12, 13, 14, 15,255,255,255,255,255,255,255,255,255,
           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
           255, 10, 11, 12, 13, 14, 15,255,255,255,255,255,255,255,255,255,
           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
        };
        template<int Dummy>
        const unsigned char lookup_tables<Dummy>::lookup_upcase[256] =
        {
           0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
           16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
           32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
           48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
           64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
           80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
           96, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
           80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 123,124,125,126,127,
           128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
           144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
           160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
           176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
           192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
           208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
           224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
           240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
        };
    }
}
#undef RAPIDXML_PARSE_ERROR
#ifdef _MSC_VER
    #pragma warning(pop)
#endif
#endif
/*#include <cereal/external/rapidxml/rapidxml_print.hpp>*/
#ifndef RAPIDXML_PRINT_HPP_INCLUDED
#define RAPIDXML_PRINT_HPP_INCLUDED
/*#include "rapidxml.hpp" skipped */
#ifndef RAPIDXML_NO_STREAMS
    #include <ostream>
    #include <iterator>
#endif
namespace rapidxml
{
    const int print_no_indenting = 0x1;
    namespace internal
    {
        template<class OutIt, class Ch>
        inline OutIt copy_chars(const Ch *begin, const Ch *end, OutIt out)
        {
            while (begin != end)
                *out++ = *begin++;
            return out;
        }
        template<class OutIt, class Ch>
        inline OutIt copy_and_expand_chars(const Ch *begin, const Ch *end, Ch noexpand, OutIt out)
        {
            while (begin != end)
            {
                if (*begin == noexpand)
                {
                    *out++ = *begin;
                }
                else
                {
                    switch (*begin)
                    {
                    case Ch('<'):
                        *out++ = Ch('&'); *out++ = Ch('l'); *out++ = Ch('t'); *out++ = Ch(';');
                        break;
                    case Ch('>'):
                        *out++ = Ch('&'); *out++ = Ch('g'); *out++ = Ch('t'); *out++ = Ch(';');
                        break;
                    case Ch('\''):
                        *out++ = Ch('&'); *out++ = Ch('a'); *out++ = Ch('p'); *out++ = Ch('o'); *out++ = Ch('s'); *out++ = Ch(';');
                        break;
                    case Ch('"'):
                        *out++ = Ch('&'); *out++ = Ch('q'); *out++ = Ch('u'); *out++ = Ch('o'); *out++ = Ch('t'); *out++ = Ch(';');
                        break;
                    case Ch('&'):
                        *out++ = Ch('&'); *out++ = Ch('a'); *out++ = Ch('m'); *out++ = Ch('p'); *out++ = Ch(';');
                        break;
                    default:
                        *out++ = *begin;    // No expansion, copy character
                    }
                }
                ++begin;    // Step to next character
            }
            return out;
        }
        // Fill given output iterator with repetitions of the same character
        template<class OutIt, class Ch>
        inline OutIt fill_chars(OutIt out, int n, Ch ch)
        {
            for (int i = 0; i < n; ++i)
                *out++ = ch;
            return out;
        }
        // Find character
        template<class Ch, Ch ch>
        inline bool find_char(const Ch *begin, const Ch *end)
        {
            while (begin != end)
                if (*begin++ == ch)
                    return true;
            return false;
        }
        ///////////////////////////////////////////////////////////////////////////
        // Internal printing operations
        // Print node
        template<class OutIt, class Ch>
        inline OutIt print_node(OutIt out, const xml_node<Ch> *node, int flags, int indent);
        // Print children of the node
        template<class OutIt, class Ch>
        inline OutIt print_children(OutIt out, const xml_node<Ch> *node, int flags, int indent)
        {
            for (xml_node<Ch> *child = node->first_node(); child; child = child->next_sibling())
                out = print_node(out, child, flags, indent);
            return out;
        }
        // Print attributes of the node
        template<class OutIt, class Ch>
        inline OutIt print_attributes(OutIt out, const xml_node<Ch> *node, int /*flags*/)
        {
            for (xml_attribute<Ch> *attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute())
            {
                if (attribute->name() && attribute->value())
                {
                    // Print attribute name
                    *out = Ch(' '), ++out;
                    out = copy_chars(attribute->name(), attribute->name() + attribute->name_size(), out);
                    *out = Ch('='), ++out;
                    // Print attribute value using appropriate quote type
                    if (find_char<Ch, Ch('"')>(attribute->value(), attribute->value() + attribute->value_size()))
                    {
                        *out = Ch('\''), ++out;
                        out = copy_and_expand_chars(attribute->value(), attribute->value() + attribute->value_size(), Ch('"'), out);
                        *out = Ch('\''), ++out;
                    }
                    else
                    {
                        *out = Ch('"'), ++out;
                        out = copy_and_expand_chars(attribute->value(), attribute->value() + attribute->value_size(), Ch('\''), out);
                        *out = Ch('"'), ++out;
                    }
                }
            }
            return out;
        }
        // Print data node
        template<class OutIt, class Ch>
        inline OutIt print_data_node(OutIt out, const xml_node<Ch> *node, int flags, int indent)
        {
            assert(node->type() == node_data);
            if (!(flags & print_no_indenting))
                out = fill_chars(out, indent, Ch('\t'));
            out = copy_and_expand_chars(node->value(), node->value() + node->value_size(), Ch(0), out);
            return out;
        }
        // Print data node
        template<class OutIt, class Ch>
        inline OutIt print_cdata_node(OutIt out, const xml_node<Ch> *node, int flags, int indent)
        {
            assert(node->type() == node_cdata);
            if (!(flags & print_no_indenting))
                out = fill_chars(out, indent, Ch('\t'));
            *out = Ch('<'); ++out;
            *out = Ch('!'); ++out;
            *out = Ch('['); ++out;
            *out = Ch('C'); ++out;
            *out = Ch('D'); ++out;
            *out = Ch('A'); ++out;
            *out = Ch('T'); ++out;
            *out = Ch('A'); ++out;
            *out = Ch('['); ++out;
            out = copy_chars(node->value(), node->value() + node->value_size(), out);
            *out = Ch(']'); ++out;
            *out = Ch(']'); ++out;
            *out = Ch('>'); ++out;
            return out;
        }
        // Print element node
        template<class OutIt, class Ch>
        inline OutIt print_element_node(OutIt out, const xml_node<Ch> *node, int flags, int indent)
        {
            assert(node->type() == node_element);
            // Print element name and attributes, if any
            if (!(flags & print_no_indenting))
                out = fill_chars(out, indent, Ch('\t'));
            *out = Ch('<'), ++out;
            out = copy_chars(node->name(), node->name() + node->name_size(), out);
            out = print_attributes(out, node, flags);
            // If node is childless
            if (node->value_size() == 0 && !node->first_node())
            {
                // Print childless node tag ending
                *out = Ch('/'), ++out;
                *out = Ch('>'), ++out;
            }
            else
            {
                // Print normal node tag ending
                *out = Ch('>'), ++out;
                // Test if node contains a single data node only (and no other nodes)
                xml_node<Ch> *child = node->first_node();
                if (!child)
                {
                    // If node has no children, only print its value without indenting
                    out = copy_and_expand_chars(node->value(), node->value() + node->value_size(), Ch(0), out);
                }
                else if (child->next_sibling() == 0 && child->type() == node_data)
                {
                    // If node has a sole data child, only print its value without indenting
                    out = copy_and_expand_chars(child->value(), child->value() + child->value_size(), Ch(0), out);
                }
                else
                {
                    // Print all children with full indenting
                    if (!(flags & print_no_indenting))
                        *out = Ch('\n'), ++out;
                    out = print_children(out, node, flags, indent + 1);
                    if (!(flags & print_no_indenting))
                        out = fill_chars(out, indent, Ch('\t'));
                }
                // Print node end
                *out = Ch('<'), ++out;
                *out = Ch('/'), ++out;
                out = copy_chars(node->name(), node->name() + node->name_size(), out);
                *out = Ch('>'), ++out;
            }
            return out;
        }
        // Print declaration node
        template<class OutIt, class Ch>
        inline OutIt print_declaration_node(OutIt out, const xml_node<Ch> *node, int flags, int indent)
        {
            // Print declaration start
            if (!(flags & print_no_indenting))
                out = fill_chars(out, indent, Ch('\t'));
            *out = Ch('<'), ++out;
            *out = Ch('?'), ++out;
            *out = Ch('x'), ++out;
            *out = Ch('m'), ++out;
            *out = Ch('l'), ++out;
            // Print attributes
            out = print_attributes(out, node, flags);
            // Print declaration end
            *out = Ch('?'), ++out;
            *out = Ch('>'), ++out;
            return out;
        }
        // Print comment node
        template<class OutIt, class Ch>
        inline OutIt print_comment_node(OutIt out, const xml_node<Ch> *node, int flags, int indent)
        {
            assert(node->type() == node_comment);
            if (!(flags & print_no_indenting))
                out = fill_chars(out, indent, Ch('\t'));
            *out = Ch('<'), ++out;
            *out = Ch('!'), ++out;
            *out = Ch('-'), ++out;
            *out = Ch('-'), ++out;
            out = copy_chars(node->value(), node->value() + node->value_size(), out);
            *out = Ch('-'), ++out;
            *out = Ch('-'), ++out;
            *out = Ch('>'), ++out;
            return out;
        }
        // Print doctype node
        template<class OutIt, class Ch>
        inline OutIt print_doctype_node(OutIt out, const xml_node<Ch> *node, int flags, int indent)
        {
            assert(node->type() == node_doctype);
            if (!(flags & print_no_indenting))
                out = fill_chars(out, indent, Ch('\t'));
            *out = Ch('<'), ++out;
            *out = Ch('!'), ++out;
            *out = Ch('D'), ++out;
            *out = Ch('O'), ++out;
            *out = Ch('C'), ++out;
            *out = Ch('T'), ++out;
            *out = Ch('Y'), ++out;
            *out = Ch('P'), ++out;
            *out = Ch('E'), ++out;
            *out = Ch(' '), ++out;
            out = copy_chars(node->value(), node->value() + node->value_size(), out);
            *out = Ch('>'), ++out;
            return out;
        }
        // Print pi node
        template<class OutIt, class Ch>
        inline OutIt print_pi_node(OutIt out, const xml_node<Ch> *node, int flags, int indent)
        {
            assert(node->type() == node_pi);
            if (!(flags & print_no_indenting))
                out = fill_chars(out, indent, Ch('\t'));
            *out = Ch('<'), ++out;
            *out = Ch('?'), ++out;
            out = copy_chars(node->name(), node->name() + node->name_size(), out);
            *out = Ch(' '), ++out;
            out = copy_chars(node->value(), node->value() + node->value_size(), out);
            *out = Ch('?'), ++out;
            *out = Ch('>'), ++out;
            return out;
        }
        // Print node
        template<class OutIt, class Ch>
        inline OutIt print_node(OutIt out, const xml_node<Ch> *node, int flags, int indent)
        {
            // Print proper node type
            switch (node->type())
            {
            // Document
            case node_document:
                out = print_children(out, node, flags, indent);
                break;
            // Element
            case node_element:
                out = print_element_node(out, node, flags, indent);
                break;
            // Data
            case node_data:
                out = print_data_node(out, node, flags, indent);
                break;
            // CDATA
            case node_cdata:
                out = print_cdata_node(out, node, flags, indent);
                break;
            // Declaration
            case node_declaration:
                out = print_declaration_node(out, node, flags, indent);
                break;
            // Comment
            case node_comment:
                out = print_comment_node(out, node, flags, indent);
                break;
            // Doctype
            case node_doctype:
                out = print_doctype_node(out, node, flags, indent);
                break;
            // Pi
            case node_pi:
                out = print_pi_node(out, node, flags, indent);
                break;
                // Unknown
            default:
                assert(0);
                break;
            }
            // If indenting not disabled, add line break after node
            if (!(flags & print_no_indenting))
                *out = Ch('\n'), ++out;
            return out;
        }
    }
    template<class OutIt, class Ch>
    inline OutIt print(OutIt out, const xml_node<Ch> &node, int flags = 0)
    {
        return internal::print_node(out, &node, flags, 0);
    }
#ifndef RAPIDXML_NO_STREAMS
    template<class Ch>
    inline std::basic_ostream<Ch> &print(std::basic_ostream<Ch> &out, const xml_node<Ch> &node, int flags = 0)
    {
        print(std::ostream_iterator<Ch>(out), node, flags);
        return out;
    }
    template<class Ch>
    inline std::basic_ostream<Ch> &operator <<(std::basic_ostream<Ch> &out, const xml_node<Ch> &node)
    {
        return print(out, node);
    }
#endif
}
#endif
/*#include <cereal/external/base64.hpp> skipped */
#include <sstream>
#include <stack>
#include <vector>
#include <limits>
#include <string>
#include <cstring>
#include <cmath>
namespace cereal
{
  namespace xml_detail
  {
    #ifndef CEREAL_XML_STRING_VALUE
    #define CEREAL_XML_STRING_VALUE "cereal"
    #endif
    static const char * CEREAL_XML_STRING = CEREAL_XML_STRING_VALUE;
    inline bool isWhitespace( char c )
    {
      return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }
  }
  class XMLOutputArchive : public OutputArchive<XMLOutputArchive>, public traits::TextArchive
  {
    public:
      class Options
      {
        public:
          static Options Default(){ return Options(); }
          static Options NoIndent(){ return Options( std::numeric_limits<double>::max_digits10, false ); }
          explicit Options( int precision = std::numeric_limits<double>::max_digits10,
                            bool indent = true,
                            bool outputType = false ) :
            itsPrecision( precision ),
            itsIndent( indent ),
            itsOutputType( outputType ) { }
        private:
          friend class XMLOutputArchive;
          int itsPrecision;
          bool itsIndent;
          bool itsOutputType;
      };
      XMLOutputArchive( std::ostream & stream, Options const & options = Options::Default() ) :
        OutputArchive<XMLOutputArchive>(this),
        itsStream(stream),
        itsOutputType( options.itsOutputType ),
        itsIndent( options.itsIndent )
      {
        auto node = itsXML.allocate_node( rapidxml::node_declaration );
        node->append_attribute( itsXML.allocate_attribute( "version", "1.0" ) );
        node->append_attribute( itsXML.allocate_attribute( "encoding", "utf-8" ) );
        itsXML.append_node( node );
        auto root = itsXML.allocate_node( rapidxml::node_element, xml_detail::CEREAL_XML_STRING );
        itsXML.append_node( root );
        itsNodes.emplace( root );
        itsStream << std::boolalpha;
        itsStream.precision( options.itsPrecision );
        itsOS << std::boolalpha;
        itsOS.precision( options.itsPrecision );
      }
      ~XMLOutputArchive()
      {
        const int flags = itsIndent ? 0x0 : rapidxml::print_no_indenting;
        rapidxml::print( itsStream, itsXML, flags );
        itsXML.clear();
      }
      void saveBinaryValue( const void * data, size_t size, const char * name = nullptr )
      {
        itsNodes.top().name = name;
        startNode();
        auto base64string = base64::encode( reinterpret_cast<const unsigned char *>( data ), size );
        saveValue( base64string );
        if( itsOutputType )
          itsNodes.top().node->append_attribute( itsXML.allocate_attribute( "type", "cereal binary data" ) );
        finishNode();
      };
      void startNode()
      {
        const auto nameString = itsNodes.top().getValueName();
        auto namePtr = itsXML.allocate_string( nameString.data(), nameString.length() + 1 );
        auto node = itsXML.allocate_node( rapidxml::node_element, namePtr, nullptr, nameString.size() );
        itsNodes.top().node->append_node( node );
        itsNodes.emplace( node );
      }
      void finishNode()
      {
        itsNodes.pop();
      }
      void setNextName( const char * name )
      {
        itsNodes.top().name = name;
      }
      template <class T> inline
      void saveValue( T const & value )
      {
        itsOS.clear(); itsOS.seekp( 0, std::ios::beg );
        itsOS << value << std::ends;
        const auto strValue = itsOS.str();
        const auto len = strValue.length();
        if ( len > 1 && ( xml_detail::isWhitespace( strValue[0] ) || xml_detail::isWhitespace( strValue[len - 2] ) ) )
        {
          itsNodes.top().node->append_attribute( itsXML.allocate_attribute( "xml:space", "preserve" ) );
        }
        auto dataPtr = itsXML.allocate_string( itsOS.str().c_str(), itsOS.str().length() + 1 );
        itsNodes.top().node->append_node( itsXML.allocate_node( rapidxml::node_data, nullptr, dataPtr ) );
      }
      void saveValue( uint8_t const & value )
      {
        saveValue( static_cast<uint32_t>( value ) );
      }
      void saveValue( int8_t const & value )
      {
        saveValue( static_cast<int32_t>( value ) );
      }
      template <class T> inline
      void insertType()
      {
        if( !itsOutputType )
          return;
        const auto nameString = util::demangledName<T>();
        auto namePtr = itsXML.allocate_string( nameString.data(), nameString.length() + 1 );
        itsNodes.top().node->append_attribute( itsXML.allocate_attribute( "type", namePtr ) );
      }
      void appendAttribute( const char * name, const char * value )
      {
        auto namePtr =  itsXML.allocate_string( name );
        auto valuePtr = itsXML.allocate_string( value );
        itsNodes.top().node->append_attribute( itsXML.allocate_attribute( namePtr, valuePtr ) );
      }
    protected:
      struct NodeInfo
      {
        NodeInfo( rapidxml::xml_node<> * n = nullptr,
                  const char * nm = nullptr ) :
          node( n ),
          counter( 0 ),
          name( nm )
        { }
        rapidxml::xml_node<> * node;
        size_t counter;
        const char * name;
        std::string getValueName()
        {
          if( name )
          {
            auto n = name;
            name = nullptr;
            return {n};
          }
          else
            return "value" + std::to_string( counter++ ) + "\0";
        }
      };
    private:
      std::ostream & itsStream;
      rapidxml::xml_document<> itsXML;
      std::stack<NodeInfo> itsNodes;
      std::ostringstream itsOS;
      bool itsOutputType;
      bool itsIndent;
  };
  class XMLInputArchive : public InputArchive<XMLInputArchive>, public traits::TextArchive
  {
    public:
      XMLInputArchive( std::istream & stream ) :
        InputArchive<XMLInputArchive>( this ),
        itsData( std::istreambuf_iterator<char>( stream ), std::istreambuf_iterator<char>() )
      {
        try
        {
          itsData.push_back('\0');
          itsXML.parse<rapidxml::parse_trim_whitespace | rapidxml::parse_no_data_nodes | rapidxml::parse_declaration_node>( reinterpret_cast<char *>( itsData.data() ) );
        }
        catch( rapidxml::parse_error const & )
        {
          throw Exception("XML Parsing failed - likely due to invalid characters or invalid naming");
        }
        auto root = itsXML.first_node( xml_detail::CEREAL_XML_STRING );
        if( root == nullptr )
          throw Exception("Could not detect cereal root node - likely due to empty or invalid input");
        else
          itsNodes.emplace( root );
      }
      void loadBinaryValue( void * data, size_t size, const char * name = nullptr )
      {
        setNextName( name );
        startNode();
        std::string encoded;
        loadValue( encoded );
        auto decoded = base64::decode( encoded );
        if( size != decoded.size() )
          throw Exception("Decoded binary data size does not match specified size");
        std::memcpy( data, decoded.data(), decoded.size() );
        finishNode();
      };
      void startNode()
      {
        auto next = itsNodes.top().child;
        auto const expectedName = itsNodes.top().name;
        if( expectedName && ( next == nullptr || std::strcmp( next->name(), expectedName ) != 0 ) )
        {
          next = itsNodes.top().search( expectedName );
          if( next == nullptr )
            throw Exception("XML Parsing failed - provided NVP not found");
        }
        itsNodes.emplace( next );
      }
      void finishNode()
      {
        itsNodes.pop();
        itsNodes.top().advance();
        itsNodes.top().name = nullptr;
      }
      const char * getNodeName() const
      {
        return itsNodes.top().node->name();
      }
      void setNextName( const char * name )
      {
        itsNodes.top().name = name;
      }
      template <class T, traits::EnableIf<std::is_unsigned<T>::value,
                                          std::is_same<T, bool>::value> = traits::sfinae> inline
      void loadValue( T & value )
      {
        std::istringstream is( itsNodes.top().node->value() );
        is.setf( std::ios::boolalpha );
        is >> value;
      }
      template <class T, traits::EnableIf<std::is_integral<T>::value,
                                          !std::is_same<T, bool>::value,
                                          sizeof(T) == sizeof(char)> = traits::sfinae> inline
      void loadValue( T & value )
      {
        value = *reinterpret_cast<T*>( itsNodes.top().node->value() );
      }
      void loadValue( int8_t & value )
      {
        int32_t val; loadValue( val ); value = static_cast<int8_t>( val );
      }
      void loadValue( uint8_t & value )
      {
        uint32_t val; loadValue( val ); value = static_cast<uint8_t>( val );
      }
      template <class T, traits::EnableIf<std::is_unsigned<T>::value,
                                          !std::is_same<T, bool>::value,
                                          !std::is_same<T, unsigned char>::value,
                                          sizeof(T) < sizeof(long long)> = traits::sfinae> inline
      void loadValue( T & value )
      {
        value = static_cast<T>( std::stoul( itsNodes.top().node->value() ) );
      }
      template <class T, traits::EnableIf<std::is_unsigned<T>::value,
                                          !std::is_same<T, bool>::value,
                                          sizeof(T) >= sizeof(long long)> = traits::sfinae> inline
      void loadValue( T & value )
      {
        value = static_cast<T>( std::stoull( itsNodes.top().node->value() ) );
      }
      template <class T, traits::EnableIf<std::is_signed<T>::value,
                                          !std::is_same<T, char>::value,
                                          sizeof(T) <= sizeof(int)> = traits::sfinae> inline
      void loadValue( T & value )
      {
        value = static_cast<T>( std::stoi( itsNodes.top().node->value() ) );
      }
      template <class T, traits::EnableIf<std::is_signed<T>::value,
                                          (sizeof(T) > sizeof(int)),
                                          sizeof(T) <= sizeof(long)> = traits::sfinae> inline
      void loadValue( T & value )
      {
        value = static_cast<T>( std::stol( itsNodes.top().node->value() ) );
      }
      template <class T, traits::EnableIf<std::is_signed<T>::value,
                                          (sizeof(T) > sizeof(long)),
                                          sizeof(T) <= sizeof(long long)> = traits::sfinae> inline
      void loadValue( T & value )
      {
        value = static_cast<T>( std::stoll( itsNodes.top().node->value() ) );
      }
      void loadValue( float & value )
      {
        try
        {
          value = std::stof( itsNodes.top().node->value() );
        }
        catch( std::out_of_range const & )
        {
          std::istringstream is( itsNodes.top().node->value() );
          is >> value;
          if( std::fpclassify( value ) != FP_SUBNORMAL )
            throw;
        }
      }
      void loadValue( double & value )
      {
        try
        {
          value = std::stod( itsNodes.top().node->value() );
        }
        catch( std::out_of_range const & )
        {
          std::istringstream is( itsNodes.top().node->value() );
          is >> value;
          if( std::fpclassify( value ) != FP_SUBNORMAL )
            throw;
        }
      }
      void loadValue( long double & value )
      {
        try
        {
          value = std::stold( itsNodes.top().node->value() );
        }
        catch( std::out_of_range const & )
        {
          std::istringstream is( itsNodes.top().node->value() );
          is >> value;
          if( std::fpclassify( value ) != FP_SUBNORMAL )
            throw;
        }
      }
      template<class CharT, class Traits, class Alloc> inline
      void loadValue( std::basic_string<CharT, Traits, Alloc> & str )
      {
        std::basic_istringstream<CharT, Traits> is( itsNodes.top().node->value() );
        str.assign( std::istreambuf_iterator<CharT, Traits>( is ),
                    std::istreambuf_iterator<CharT, Traits>() );
      }
      template <class T> inline
      void loadSize( T & value )
      {
        value = getNumChildren( itsNodes.top().node );
      }
    protected:
      static size_t getNumChildren( rapidxml::xml_node<> * node )
      {
        size_t size = 0;
        node = node->first_node();
        while( node != nullptr )
        {
          ++size;
          node = node->next_sibling();
        }
        return size;
      }
      struct NodeInfo
      {
        NodeInfo( rapidxml::xml_node<> * n = nullptr ) :
          node( n ),
          child( n->first_node() ),
          size( XMLInputArchive::getNumChildren( n ) ),
          name( nullptr )
        { }
        void advance()
        {
          if( size > 0 )
          {
            --size;
            child = child->next_sibling();
          }
        }
        rapidxml::xml_node<> * search( const char * searchName )
        {
          if( searchName )
          {
            size_t new_size = XMLInputArchive::getNumChildren( node );
            const size_t name_size = rapidxml::internal::measure( searchName );
            for( auto new_child = node->first_node(); new_child != nullptr; new_child = new_child->next_sibling() )
            {
              if( rapidxml::internal::compare( new_child->name(), new_child->name_size(), searchName, name_size, true ) )
              {
                size = new_size;
                child = new_child;
                return new_child;
              }
              --new_size;
            }
          }
          return nullptr;
        }
        rapidxml::xml_node<> * node;
        rapidxml::xml_node<> * child;
        size_t size;
        const char * name;
      };
    private:
      std::vector<char> itsData;
      rapidxml::xml_document<> itsXML;
      std::stack<NodeInfo> itsNodes;
  };
  template <class T> inline
  void prologue( XMLOutputArchive &, NameValuePair<T> const & )
  { }
  template <class T> inline
  void prologue( XMLInputArchive &, NameValuePair<T> const & )
  { }
  template <class T> inline
  void epilogue( XMLOutputArchive &, NameValuePair<T> const & )
  { }
  template <class T> inline
  void epilogue( XMLInputArchive &, NameValuePair<T> const & )
  { }
  template <class T> inline
  void prologue( XMLOutputArchive & ar, SizeTag<T> const & )
  {
    ar.appendAttribute( "size", "dynamic" );
  }
  template <class T> inline
  void prologue( XMLInputArchive &, SizeTag<T> const & )
  { }
  template <class T> inline
  void epilogue( XMLOutputArchive &, SizeTag<T> const & )
  { }
  template <class T> inline
  void epilogue( XMLInputArchive &, SizeTag<T> const & )
  { }
  template <class T, traits::DisableIf<traits::has_minimal_base_class_serialization<T, traits::has_minimal_output_serialization, XMLOutputArchive>::value ||
                                       traits::has_minimal_output_serialization<T, XMLOutputArchive>::value> = traits::sfinae> inline
  void prologue( XMLOutputArchive & ar, T const & )
  {
    ar.startNode();
    ar.insertType<T>();
  }
  template <class T, traits::DisableIf<traits::has_minimal_base_class_serialization<T, traits::has_minimal_input_serialization, XMLInputArchive>::value ||
                                       traits::has_minimal_input_serialization<T, XMLInputArchive>::value> = traits::sfinae> inline
  void prologue( XMLInputArchive & ar, T const & )
  {
    ar.startNode();
  }
  template <class T, traits::DisableIf<traits::has_minimal_base_class_serialization<T, traits::has_minimal_output_serialization, XMLOutputArchive>::value ||
                                       traits::has_minimal_output_serialization<T, XMLOutputArchive>::value> = traits::sfinae> inline
  void epilogue( XMLOutputArchive & ar, T const & )
  {
    ar.finishNode();
  }
  template <class T, traits::DisableIf<traits::has_minimal_base_class_serialization<T, traits::has_minimal_input_serialization, XMLInputArchive>::value ||
                                       traits::has_minimal_input_serialization<T, XMLInputArchive>::value> = traits::sfinae> inline
  void epilogue( XMLInputArchive & ar, T const & )
  {
    ar.finishNode();
  }
  template <class T> inline
  void CEREAL_SAVE_FUNCTION_NAME( XMLOutputArchive & ar, NameValuePair<T> const & t )
  {
    ar.setNextName( t.name );
    ar( t.value );
  }
  template <class T> inline
  void CEREAL_LOAD_FUNCTION_NAME( XMLInputArchive & ar, NameValuePair<T> & t )
  {
    ar.setNextName( t.name );
    ar( t.value );
  }
  template <class T> inline
  void CEREAL_SAVE_FUNCTION_NAME( XMLOutputArchive &, SizeTag<T> const & )
  { }
  template <class T> inline
  void CEREAL_LOAD_FUNCTION_NAME( XMLInputArchive & ar, SizeTag<T> & st )
  {
    ar.loadSize( st.size );
  }
  template <class T, traits::EnableIf<std::is_arithmetic<T>::value> = traits::sfinae> inline
  void CEREAL_SAVE_FUNCTION_NAME(XMLOutputArchive & ar, T const & t)
  {
    ar.saveValue( t );
  }
  template <class T, traits::EnableIf<std::is_arithmetic<T>::value> = traits::sfinae> inline
  void CEREAL_LOAD_FUNCTION_NAME(XMLInputArchive & ar, T & t)
  {
    ar.loadValue( t );
  }
  template<class CharT, class Traits, class Alloc> inline
  void CEREAL_SAVE_FUNCTION_NAME(XMLOutputArchive & ar, std::basic_string<CharT, Traits, Alloc> const & str)
  {
    ar.saveValue( str );
  }
  template<class CharT, class Traits, class Alloc> inline
  void CEREAL_LOAD_FUNCTION_NAME(XMLInputArchive & ar, std::basic_string<CharT, Traits, Alloc> & str)
  {
    ar.loadValue( str );
  }
}
CEREAL_REGISTER_ARCHIVE(cereal::XMLOutputArchive)
CEREAL_REGISTER_ARCHIVE(cereal::XMLInputArchive)
CEREAL_SETUP_ARCHIVE_TRAITS(cereal::XMLInputArchive, cereal::XMLOutputArchive)
#endif