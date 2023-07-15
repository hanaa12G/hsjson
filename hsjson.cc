#include "hsjson.hh"
#include <sstream>

namespace hs::json
{
    hs::json::json_number::json_number(double value)
        : m_value{value}
    {
    }

    inline double
    hs::json::json_number::get_value() const noexcept
    {
        return m_value;
    }

    inline double &
    hs::json::json_number::value() noexcept
    {
        return m_value;
    }

    inline void
    hs::json::json_number::set_value(double value) noexcept
    {
        m_value = value;
    }

    hs::json::json_boolean::json_boolean(bool b)
        : m_value{b}
    {
    }

    inline bool
    hs::json::json_boolean::get_value() const noexcept
    {
        return m_value;
    }

    inline bool &
    hs::json::json_boolean::value() noexcept
    {
        return m_value;
    }

    inline void
    hs::json::json_boolean::set_value(bool value) noexcept
    {
        m_value = value;
    }

#define DEFINE_CONSTRUCTOR(from) \
    template <>                        \
    json_value::json_value<from>(from v) : m_value(std::move(v)) {}

    DEFINE_CONSTRUCTOR(json_null);
    DEFINE_CONSTRUCTOR(json_number);
    DEFINE_CONSTRUCTOR(json_boolean);
    DEFINE_CONSTRUCTOR(json_string);
    DEFINE_CONSTRUCTOR(json_object);
    DEFINE_CONSTRUCTOR(json_array);


#undef DEFINE_CONSTRUCTOR

#define DEFINE_ASSIGN_OPERATOR(from) \
    template <>                            \
    json_value &json_value::operator=<from>(from v)    \
    {                                      \
        m_value = std::move(v);                       \
        return *this;                      \
    }

    DEFINE_ASSIGN_OPERATOR(json_null);
    DEFINE_ASSIGN_OPERATOR(json_boolean);
    DEFINE_ASSIGN_OPERATOR(json_number);
    DEFINE_ASSIGN_OPERATOR(json_string);
    DEFINE_ASSIGN_OPERATOR(json_object);
    DEFINE_ASSIGN_OPERATOR(json_array);

#undef DEFINE_ASSIGN_OPERATOR

#define GET_AS(type)                             \
    template <>                                  \
    type json_value::get_as<type>() const        \
    {                                            \
        try                                      \
        {                                        \
            return std::any_cast<type>(m_value); \
        }                                        \
        catch (const std::bad_any_cast &)        \
        {                                        \
            throw conversion_error;              \
        }                                        \
    }

    GET_AS(json_null);
    GET_AS(json_boolean);
    GET_AS(json_number);
    GET_AS(json_string);
    GET_AS(json_object);
    GET_AS(json_array);

#undef GET_AS

#define AS(type)                                   \
    template <>                                    \
    type &json_value::as<type>() &                 \
    {                                              \
        try                                        \
        {                                          \
            return std::any_cast<type &>(m_value); \
        }                                          \
        catch (const std::bad_any_cast &)          \
        {                                          \
            throw conversion_error;                \
        }                                          \
    }

    AS(json_null);
    AS(json_boolean);
    AS(json_number);
    AS(json_string);
    AS(json_object);
    AS(json_array);

#undef AS

    bool
    json_object::has_attribute(json_string const &name) const noexcept
    {
        auto it = m_attributes.find(name);
        return it != m_attributes.end();
    }

    json_value
    json_object::get_attribute(json_string const &name) const
    {
        auto it = m_attributes.find(name);
        if (it == m_attributes.end())
            throw invalid_access;
        return it->second;
    }

    json_value &
    json_object::attribute(json_string const &name) &
    {
        auto it = m_attributes.find(name);
        if (it == m_attributes.end())
            throw invalid_access;
        return it->second;
    }

#define SET_ATTRIBUTE(type)                                               \
    template <>                                                           \
    void                                                                  \
    json_object::set_attribute<type>(json_string const &name, type value) \
    {                                                                     \
        if (not has_attribute(name))                                      \
            throw invalid_access;                                         \
        m_attributes[name] = std::move(value);                            \
    }

    SET_ATTRIBUTE(json_null);
    SET_ATTRIBUTE(json_boolean);
    SET_ATTRIBUTE(json_number);
    SET_ATTRIBUTE(json_string);
    SET_ATTRIBUTE(json_object);
    SET_ATTRIBUTE(json_array);

    void
    json_object::set_attribute(json_string const &name, json_value const &value)
    {
        if (not has_attribute(name))
            throw invalid_access;
        m_attributes[name] = value;
    }

    void
    json_object::set_attribute(json_string const &name, json_value &&value)
    {
        if (not has_attribute(name))
            throw invalid_access;
        m_attributes[name] = value;
    }

#undef SET_ATTRIBUTE

#define INSERT_ATTRIBUTE(type)                                                    \
    template <>                                                                   \
    void json_object::insert_attribute<type>(json_string const &name, type value) \
    {                                                                             \
        if (has_attribute(name))                                                  \
            throw invalid_access;                                                 \
        m_attributes[name] = std::move(value);                                    \
    }

    INSERT_ATTRIBUTE(json_null);
    INSERT_ATTRIBUTE(json_boolean);
    INSERT_ATTRIBUTE(json_number);
    INSERT_ATTRIBUTE(json_string);
    INSERT_ATTRIBUTE(json_object);
    INSERT_ATTRIBUTE(json_array);

#undef INSERT_ATTRIBUTE

    json_value &
    json_object::operator[](json_string const &name) &
    {
        return m_attributes[name];
    }

    int json_object::size() const noexcept
    {
        return m_attributes.size();
    }

    size_t
    json_array::size() const noexcept
    {
        return m_values.size();
    }

    bool
    json_array::empty() const noexcept
    {
        return m_values.empty();
    }

#define PUSH_BACK(type)                      \
    template <>                              \
    void json_array::push_back<type>(type v) \
    {                                        \
        m_values.push_back(std::move(v));               \
    }

    PUSH_BACK(json_null);
    PUSH_BACK(json_boolean);
    PUSH_BACK(json_number);
    PUSH_BACK(json_string);
    PUSH_BACK(json_object);
    PUSH_BACK(json_array);

    void
    json_array::push_back(json_value const &v)
    {
        m_values.push_back(v);
    }

    void
    json_array::push_back(json_value &&v)
    {
        m_values.push_back(v);
    }

#undef PUSH_BACK

    json_value
    json_array::get_at(size_t index) const
    {
        if (index >= size())
            throw invalid_access;
        return m_values[index];
    }

    json_value &
    json_array::at(size_t index)
    {
        if (index >= size())
            throw invalid_access;
        return m_values[index];
    }

#define SET(type)                                    \
    template <>                                      \
    void json_array::set<type>(size_t index, type t) \
    {                                                \
        if (index >= size())                         \
            throw invalid_access;                    \
        m_values[index] = std::move(t);                         \
    }

    SET(json_null);
    SET(json_boolean);
    SET(json_number);
    SET(json_string);
    SET(json_object);
    SET(json_array);

    void
    json_array::set(size_t index, json_value const &v)
    {
        if (index >= size())
            throw invalid_access;
        m_values[index] = v;
    }
    void
    json_array::set(size_t index, json_value &&v)
    {
        if (index >= size())
            throw invalid_access;
        m_values[index] = v;
    }

#undef SET

    json_value &
    json_array::operator[](size_t index)
    {
        return m_values[index];
    }

    namespace
    {

        using json_stream = std::stringstream;

        json_array parse_array(json_stream &s);
        json_string parse_string(json_stream &s);
        json_number parse_number(json_stream &s);
        json_object parse_object(json_stream &s);
        json_value parse_value(json_stream &s);
        json_boolean parse_boolean(json_stream &s);
        json_null parse_null(json_stream &s);

#define SKIP_WHITESPACE()          \
    while (std::isspace(s.peek())) \
        s.get();

#define CONSUME() c = s.get();

        json_object parse_object(json_stream &s)
        {
            using traits = std::char_traits<char>;
            SKIP_WHITESPACE();

            char c = s.peek();
            if (c != '{')
            {
                throw parse_error;
            }
            CONSUME();

            json_object object{};
            bool last_attribute = true;
            for (char c = s.peek(); c != traits::eof(); c = s.peek())
            {
                if (std::isspace(c))
                {
                    CONSUME();
                }
                else if (c == '"')
                {
                    auto key{parse_string(s)};

                    SKIP_WHITESPACE();

                    c = s.peek();
                    if (c != ':')
                        throw parse_error;
                    CONSUME();

                    SKIP_WHITESPACE();

                    auto value{parse_value(s)};

                    object[key] = value;
                    last_attribute = true;
                }
                else if (c == ',')
                {
                    last_attribute = false;
                    CONSUME();
                }
                else if (c == '}')
                {
                    if (not last_attribute)
                        throw parse_error;
                    CONSUME();
                    break;
                }
                else
                {
                    throw parse_error;
                }
            }

            return object;
        }

        json_array parse_array(json_stream &s)
        {
            json_array array{};

            SKIP_WHITESPACE();

            bool last_attribute = true;
            char c;
            if (s.peek() != '[')
                throw parse_error;
            CONSUME();

            for (c = s.peek(); c != json_stream::traits_type::eof(); c = s.peek())
            {
                if (std::isspace(c))
                {
                    CONSUME();
                    continue;
                }
                else if (c == ',')
                {
                    CONSUME();
                    last_attribute = false;
                }
                else if (c == ']')
                {
                    if (not last_attribute)
                        throw parse_error;
                    CONSUME();
                    break;
                }
                else
                {
                    auto res{parse_value(s)};
                    array.push_back(res);
                    last_attribute = true;
                }
            }
            return array;
        }

        json_string parse_string(json_stream &s)
        {

            SKIP_WHITESPACE();
            char c;
            if (s.peek() != '"')
                throw parse_error;
            CONSUME();

            json_string string{};

            for (c = s.peek(); c != json_stream::traits_type::eof(); c = s.peek())
            {
                if (c == '"')
                {
                    CONSUME();
                    return string;
                }
                else
                {
                    string += c;
                    CONSUME();
                }
            }
            throw parse_error;
        }

        json_number parse_number(json_stream &s)
        {
            auto parse_fraction = [](json_stream &s, std::stringstream &os)
            {
                char c = s.peek();
                if (c != '.')
                    return;
                os << c;
                CONSUME();

                c = s.peek();
                if (not std::isdigit(c))
                    throw parse_error;
                os << c;
                CONSUME();

                for (c = s.peek(); std::isdigit(c); c = s.peek())
                {
                    os << c;
                    CONSUME();
                }
            };
            auto parse_exponent = [](json_stream &s, std::stringstream &os)
            {
                char c = s.peek();
                if (c != 'e' or c != 'E')
                    return;
                os << c;
                CONSUME();

                c = s.peek();
                if (c == '-' or c == '+')
                {
                    os << c;
                    CONSUME();
                }

                c = s.peek();
                if (not std::isdigit(c))
                    throw parse_error;
                os << c;
                CONSUME();

                for (c = s.peek(); std::isdigit(c); c = s.peek())
                {
                    os << c;
                    CONSUME();
                }
            };

            SKIP_WHITESPACE();
            char c;

            c = s.peek();

            std::stringstream os{};

            if (c == '-')
            {
                os << c;
                CONSUME();
            }

            c = s.peek();

            if (c == '0')
            {
                os << c;
                CONSUME();
            }
            else if (std::isdigit(c))
            {
                os << c;
                CONSUME();

                for (c = s.peek(); std::isdigit(c); c = s.peek())
                {
                    os << c;
                    CONSUME();
                }
            }
            else
                throw parse_error;
            parse_fraction(s, os);
            parse_exponent(s, os);

            return {std::strtod(os.str().c_str(), nullptr)};
        }

        json_value parse_value(json_stream &s)
        {
            json_value value{};
            for (char c = s.peek(); c != json_stream::traits_type::eof(); c = s.peek())
            {
                if (std::isspace(c))
                {
                    CONSUME();
                    continue;
                }
                else if (c == '{')
                {
                    auto object = parse_object(s);
                    value = object;
                    return value;
                }
                else if (c == '[')
                {
                    auto array = parse_array(s);
                    value = array;
                    return value;
                }
                else if (c == '"')
                {
                    auto string = parse_string(s);
                    value = string;
                    return value;
                }
                else if (c == '-' or std::isdigit(c))
                {
                    auto number = parse_number(s);
                    value = number;
                    return value;
                }
                else if (c == 'f' or c == 't')
                {
                    auto boolean = parse_boolean(s);
                    value = boolean;
                    return value;
                }
                else if (c == 'n')
                {
                    auto null = parse_null(s);
                    value = null;
                    return value;
                }
                else
                    throw parse_error;
            }
            throw parse_error;
        }

        json_boolean parse_boolean(json_stream &s)
        {
            char c;
            std::string string;
            for (c = s.peek(); std::isalpha(c); c = s.peek())
            {
                string += c;
                CONSUME();
            }
            if (string == "false")
                return {false};
            else if (string == "true")
                return {true};
            else
                throw parse_error;
        }

        json_null parse_null(json_stream &s)
        {
            char c;
            std::string string;
            for (c = s.peek(); std::isalpha(c); c = s.peek())
            {
                string += c;
                CONSUME();
            }
            if (string == "null")
                return {};
            else
                throw parse_error;
        }

#undef SKIP_WHITESPACE
#undef CONSUME
    }

    json_value parse(json_string const &s)
    {
        json_stream stream(s);
        return parse_value(stream);
    }

}