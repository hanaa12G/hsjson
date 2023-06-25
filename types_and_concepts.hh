#pragma once

template<typename T1, typename T2>

struct is_same_type {
  static constexpr bool value = false;
};

template<typename T>
struct is_same_type<T, T> {
  static constexpr bool value = true;
};


template<typename T1, typename T2>
constexpr bool is_same_type_v = is_same_type<T1, T2>::value;

template<typename T, typename ...Ts>
struct is_one_of_types;


template<typename T, typename T0>
struct is_one_of_types<T, T0> {
  static constexpr bool value = is_same_type_v<T, T0>;
};

template<typename T, typename T0, typename ...Ts>
struct is_one_of_types<T, T0, Ts...> {
  static constexpr bool value = (is_same_type_v<T, T0> ? true : is_one_of_types<T, Ts...>::value );
};

template<typename T, typename ...Ts>
constexpr bool is_one_of_types_v = is_one_of_types<T, Ts...>::value;




template<typename T>
struct remove_cvref {
  using type = T;
};


template<typename T>
struct remove_cvref<T&> {
  using type = T;
};

template<typename T>
struct remove_cvref<T const&> {
  using type = T;
};

template<typename T>
struct remove_cvref<T const> {
  using type = T;
};

template<typename T>
struct remove_cvref<T&&> {
  using type = T;
};

template<typename T>
using remove_cvref_t = typename remove_cvref<T>::type;





template<typename T>
concept DefaultConstructible = requires () {
  T();
};
