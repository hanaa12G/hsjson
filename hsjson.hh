#ifndef HSJSON_HH
#define HSJSON_HH

#include <type_traits>
#include <map>
#include <vector>
#include <string>
#include <any>

namespace hs
{
    namespace json
    {
        static constexpr int parse_error = 1;
        static constexpr int invalid_access = 2;
        static constexpr int conversion_error = 3;

        class json_number
        {
        public:
            json_number(double d);

            double get_value() const noexcept;
            double &value() noexcept;
            void set_value(double) noexcept;

        private:
            double m_value;
        };

        class json_boolean
        {
        public:
            json_boolean(bool);

            bool get_value() const noexcept;
            bool &value() noexcept;
            void set_value(bool) noexcept;

        private:
            bool m_value;
        };

        struct json_null
        {
        };

        using json_string = std::string;


        struct json_value
        {
        public:
            json_value() = default;

            /*
             * Construct value using one of the json types
             */
            template<typename T>
            json_value(T);

            /*
             * Assign value from one of the json types
             */
            template<typename T>
            json_value& operator=(T);



            /*
             * Get underlying value
             * Will throw <conversion_error> if type not match
             */
            template<typename T>
            T get_as() const;

            template<typename T>
            T& as() &;

        private:
            std::any m_value;
        };

        

        class json_object
        {
        public:
            json_object() = default;

            /*
             * Check if an attribute exists
             */
            bool has_attribute(json_string const &name) const noexcept;

            /*
             * Get existing attribute
             * Will throw <invalid_access> if attribute does not exist
             */
            json_value get_attribute(json_string const &name) const;
            json_value &attribute(json_string const &name) &;

            /*
             * Get an attribute
             * Will throw <invalid_access> if attribute does not exist
             */
            template<typename T>
            void set_attribute(json_string const &name, T value);

            void set_attribute(json_string const& name, json_value const&);
            void set_attribute(json_string const& name, json_value&&);


            /*
             * Insert an attribute into object
             * Will throw <invalid_access> if attribute have already existed
             */
            template<typename T>
            void insert_attribute(json_string const& key, T value);

            void insert_attribute(json_string const& key, json_value const& value);
            void insert_attribute(json_string const& key, json_value&& value);

            /*
             * Get reference to an attribute,
             * Will add requested attribute if it does not exist
             */
            json_value &operator[](json_string const &name) &;


            int size() const noexcept;

        private:
            std::map<json_string, json_value> m_attributes;
        };

        class json_array
        {
        public:
            size_t size() const noexcept;
            bool empty() const noexcept;

            template<typename T>
            void push_back(T);
            void push_back(json_value const&);
            void push_back(json_value&&);

            json_value get_at(size_t index) const;
            json_value& at(size_t index);

            template<typename T>
            void set(size_t index, T);
            void set(size_t index, json_value const&);
            void set(size_t index, json_value&&);

            json_value& operator[](size_t index);
        private:
            std::vector<json_value> m_values;
        };

        json_value parse(json_string const& s);
    }


}


#endif