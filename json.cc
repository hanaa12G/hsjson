#include <string>
#include <iostream>
#include <cassert>
#include <sstream>
#include "types_and_concepts.hh"
#include "containers.hh"

#define TRY_GET(c, s) if (s.eof()) return null_optional; c = s.get();

inline bool is_digit(char c) { return c >= '0' and c <= '9'; }


template<typename T, typename E>
class result_or
{
public:
  result_or(T value): m_type { value_category::success }, m_value { value } {}
  result_or(E error): m_type { value_category::failure }, m_error { error } {}

  ~result_or() {
    if (m_type == value_category::success) {
      m_value.~T();
    } else {
      m_error.~E();
    }
  }

  operator bool() const noexcept {
    return m_type == value_category::success;
  }

  T get_value() const {
    if (m_type == value_category::failure) throw 1;
    return m_value;
  }

private:

  enum class value_category {
    success,
    failure
  };

  value_category m_type;

  union {
    T m_value;
    E m_error;
  };
};





struct null_optional_t {};

static null_optional_t null_optional;


template<typename T>
struct optional {

  optional(null_optional_t t) : m_is_null {true} {}

  optional(T const& t) : m_is_null { false } {
    auto ref = reinterpret_cast<T*>(m_raw);
    new (ref) T(t);
  }

  optional(T&& t) : m_is_null { false } {
    auto ref = reinterpret_cast<T*>(m_raw);
    new (ref) T(t);
  }

  ~optional() {
    if (not m_is_null) {
      auto ref = reinterpret_cast<T*>(m_raw);
      ref->~T();
    }
  }

  operator bool() {
    return not m_is_null;
  }

  T& operator* () {
    return *reinterpret_cast<T*>(m_raw);
  }

private:
  bool m_is_null;
  char m_raw[sizeof(T)];
};

enum class json_error {
  invalid_type,
  field_not_exists,
  mismatch_type
};


struct json_stream {
  json_stream (std::string const& buf) : m_buffer { buf }, m_index { 0l } {}

  char get() {
    if (m_index < m_buffer.size()) {
      return m_buffer[m_index++];
    }
    return 0;
  }

  void unget() {
    if (m_index > 0) m_index--;
  }

  bool eof() {
    return m_index >= m_buffer.size();
  }

private:
  std::string m_buffer;
  long        m_index;
};


#define JSON_TYPES json_number, json_string, json_array, json_object

struct json_object;
struct json_array;
struct json_string;
struct json_number;

template<typename T>
concept JsonValueType = is_one_of_types_v<remove_cvref_t<T>, JSON_TYPES>;

struct json_string : public std::string {
	using std::string::string;
};

struct json_number {
  long   integral;
  double fractional;
  int    exponental;
  
  template<typename T>
  T get_as() const {
  }
  
  int get_as_int() const;
  long get_as_long() const;
  float get_as_float() const;
  double get_as_double() const;
};



struct json_value {
public:

  json_value() : m_type_index { -1 }, m_value { nullptr } {}

  template<JsonValueType T>
  json_value& operator=(T&& t)  { 
	using TT = remove_cvref_t<T>;
	int const N = index_of_v<TT, JSON_TYPES>;
	
	if (m_type_index >= 0) {
		clear_();
	}
	
	auto ptr = new TT(std::forward<T>(t));
	m_type_index = N;
	m_value = ptr;
	return *this;
  }

  template<JsonValueType T>
  result_or<T, json_error> get_as() const {
    if (m_type_index != index_of_v<T, JSON_TYPES>) {
      return { json_error::mismatch_type };
    }

    return { *reinterpret_cast<T*>(m_value) };
  }

private:
  void clear_();

  int m_type_index;
  void* m_value;
};

struct json_array {

  void push_back(json_value const& v) {}
};


struct json_object {
	
  json_object() {
	assert(true);
  }

  template<JsonValueType T>
  result_or<T, json_error> get_attribute_as(json_string const& name) const noexcept {
    uint64_t i { 0 };
    for (; i < m_keys.size(); ++i) {
      if (m_keys[i] == name) {
        break;
      }
    }
    if (i == m_keys.size()) {
      return { json_error::field_not_exists };
    }

    json_value const& val = m_values[i];

    return val.get_as<T>();

  }


  json_value& operator[] (json_string const key) {
	uint64_t i = 0;
	for (; i < m_keys.size(); ++i) {
		if (key == m_keys[i]) {
			break;
		}
	}
	if (i == m_keys.size()) {
		m_keys.push_back (std::move(key));
		m_values.push_back ({ });
	}
	return m_values[i];
  }

  vector<json_string> m_keys;
  vector<json_value>  m_values;
};

void json_value::clear_() {
#define CASE(n) case n: \
{ \
	using type = nth_type_t<n, JSON_TYPES>; \
	auto ptr = reinterpret_cast<type*>(m_value); \
	delete ptr; \
} break; \

	switch (m_type_index) {
		CASE(0);
		CASE(1);
		CASE(2);
		CASE(3);
		
		case -1:
			break;
		default:
			assert(false);
	}
	m_type_index = -1;
	m_value = nullptr;
#undef CASE
  }



optional<json_array>  parse_array(json_stream& s);
optional<json_string> parse_string(json_stream& s);
optional<json_number> parse_number(json_stream& s);
optional<json_object> parse_object(json_stream& s);
optional<json_value>  parse_value(json_stream& s);



inline void skip_whitespace(json_stream& s) {
  while (not s.eof()) {
    char c = s.get();
    if (c != ' ' and c != '\n' and c != '\t') break;
  }
  if (not s.eof()) s.unget();
}

optional<json_object> parse_object(json_stream& s) {
  std::cout << "parse object" << std::endl;
  if (s.eof()) return null_optional;
  json_object object {};

  bool last_attribute = true;
  skip_whitespace(s);
  char c = s.get();
  if (c != '{') return null_optional;
  for (char c = s.get(); not s.eof(); c = s.get()) {
    if (c == ' ' or c == '\n') continue;
    else if (c == '"') {
      s.unget();
      auto res { parse_string(s) };
      if (not res) {
        return null_optional;
      }
      json_string key { *res };

      skip_whitespace(s);

      if (s.eof()) return null_optional;

      c = s.get();
      if (c != ':') return null_optional;

      skip_whitespace(s);
      if (s.eof()) return null_optional;
      auto value_res { parse_value(s) };
      if (not value_res) {
        return null_optional;
      }

      object[key] = *value_res;
      last_attribute = true;
    } else if (c == ',') {
      last_attribute = false;
    } else if (c == '}') {
      if (not last_attribute) return null_optional;
    } else {
      return null_optional;
    }
  }

  return { object };
}

optional<json_array> parse_array(json_stream& s) {
  std::cout << "parse array" << std::endl;
  if (s.eof()) return null_optional;
  json_array array {};

  bool last_attribute = true;
  char c;
  skip_whitespace(s);
  TRY_GET(c, s);
  if (c != '[') return null_optional;

  for (char c = s.get(); not s.eof(); c = s.get()) {
    if (c == ' ' or c == '\t' or c == '\n') {
      continue;
    } else if (c == ',') {
      last_attribute = false;
    } else if (c == ']') {
      if (not last_attribute) return null_optional;
      break;
    } else {
      auto res { parse_value(s) };
      if (not res) return null_optional;
      array.push_back(*res);
      last_attribute = true;
    }
  }
  return { array };
}

optional<json_string> parse_string(json_stream& s) {
  std::cout << "parse string" << std::endl;
  char c;
  skip_whitespace(s);
  TRY_GET(c, s);
  if (c != '"') return null_optional;
  json_string string {};

  while (true) {
    TRY_GET(c, s);
    if (c == '"') return { string };
    string += c;
  }
  return null_optional;
}

optional<json_number> parse_number(json_stream& s) {
  std::cout << "parse number" << std::endl;
  char c;
  skip_whitespace(s);

  TRY_GET(c, s);
  std::stringstream os {};
  long integral = 0;
  bool negate = false;

  if (c == '-') {
    negate = true;
    os << c;
  }
  else if (c == '0') {
    integral = 0;
    os << 'c';
  } else if (c >= '1' and c <= '9') {
    os << c;
    integral += c - '0';
    while (not s.eof()) {
      c = s.get();
      if (not is_digit(c)) break;
      os << c;
      integral = integral * 10 + (c - '0');
    }
  } else return null_optional;

  if (s.eof()) {
    return { json_number { integral, 0.0, 1 } };
  }

  double fractional { 0.0f };

  if (c == '.') {
    os << c;
    if (s.eof()) return null_optional;
    c = s.get();
    if (not (c >= '0' and c <= '9')) return null_optional;

    os << c;
    double i { 10.0 };
    fractional += (c - '0') / i;

    while (not s.eof()) {
      c = s.get();
      if (not (c >= '0' and c <= '9')) break;
      i *= 10.0;
      fractional += (c - '0') * i;
      os << c;
    }
  }

  if (s.eof()) {
    return { json_number { integral, fractional, 1 } };
  }

  if (c == 'e' or c == 'E') {
    os << c;
    TRY_GET(c, s);
    if (c == '+' or c == '-') {
      os << c;
      TRY_GET(c, s);
    } else if (c >= '0' and c <= '9') {
      os << c;
      while (c >= '0' and c <= '9') {
        TRY_GET(c, s);
        os << c;
      }
    } else {
      return null_optional;
    }
  }
}

optional<json_value> parse_value(json_stream& s) {
  if (s.eof()) return null_optional;
  json_value value {};
  for (char c = s.get(); not s.eof(); c = s.get()) {
    if (c == ' ') {
      continue;
    } else if (c == '{') {
      s.unget();
      auto res = parse_object(s);
      if (not res) {
        return null_optional;
      }
      auto& object = *res;
      value = object;
      return { value };
    } else if (c == '[') {
      s.unget();
      auto res = parse_array(s);
      if (not res) {
        return null_optional;
      }
      auto& array = *res;
      value = array;
      return { value };
    } else if (c == '"') {
      s.unget();
      auto res = parse_string(s);
      if (not res) {
        return null_optional;
      }
      auto& string = *res;
      value = string;
      return { value };
    } else if (c == '-' or (c >= '0' and c <= '9')) {
      s.unget();
      auto res = parse_number(s);
      if (not res) {
        return null_optional;
      }
      auto& number = *res;
      value = number;
      return { value };
    }
    else {
      return null_optional;
    }
  }

}




int main() {
  std::string json { R"({"name" : "John Doe", "age": 14})" };

  json_stream stream { json };

  auto res = parse_object(stream);
  assert(res);
  auto const& person = *res;


  std::string name {};
  auto name_res = person.get_attribute_as<json_string>({ "name" });
  assert (name_res);
  name = name_res.get_value();
  assert (name == "John Doe");
  auto age_res = person.get_attribute_as<json_number>("age");
  assert(age_res);
  int age = age_res.get_as<int>();
  assert(age == 14);
  auto agef = age_res.get_as<float>();
  assert(agef == 14.0f);
}
