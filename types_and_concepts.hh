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
struct remove_const {
  using type = T;
};

template<typename T>
struct remove_const<T const> {
  using type = T;
};

template<typename T>
using remove_const_t = typename remove_const<T>::type;

template<typename T>
struct remove_volatile {
  using type = T;
};

template<typename T>
struct remove_volatile <T volatile> {
  using type = T;
};

template<typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

template<typename T>
struct remove_cv {
  using type = remove_volatile_t<remove_const_t<T>>;
};

template<typename T>
using remove_cv_t = typename remove_cv<T>::type;


template<typename T>
struct remove_ref {
  using type = T;
};

template<typename T>
struct remove_ref<T&> {
  using type = T;
};

template<typename T>
struct remove_ref<T&&> {
  using type = T;
};

template<typename T>
using remove_ref_t = typename remove_ref<T>::type;


template<typename T>
struct remove_cvref {
  using type = remove_ref_t<remove_cv_t<T>>;
};


template<typename T>
using remove_cvref_t = typename remove_cvref<T>::type;



template<typename T, typename... Ts>
struct index_of;

template<typename T, typename T0>
struct index_of<T, T0> {
	static constexpr int value = is_same_type_v<T, T0> ? 0 : -1;
};
template<typename T, typename T0, typename... Ts>
struct index_of<T, T0, Ts...> {
	static constexpr int value = is_same_type_v<T, T0> ? 0 : (index_of<T, Ts...>::value == -1 ? -1 : 1 + index_of<T, Ts...>::value);
};

template<typename T, typename... Ts>
static constexpr int index_of_v = index_of<T, Ts...>::value;

template<unsigned N, typename... Ts>
struct nth_type;

template<typename T, typename... Ts>
struct nth_type<0, T, Ts...> {
	using type = T;
};
template<unsigned N, typename T, typename... Ts>
struct nth_type<N, T, Ts...> {
	using type = typename nth_type<N - 1, Ts...>::type;
};

template<unsigned N, typename... Ts>
using nth_type_t = typename nth_type<N, Ts...>::type;



template<typename T>
concept DefaultConstructible = requires () {
  T();
};
