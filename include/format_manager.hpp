/*
    * format_manager.hpp
    *
    * Header-only library for managing output formats: JSON, XML, YAML, HTML, CSV, and Plain Text in C++.
    * Supports serialization of data into different formats and provides an easy way to switch between them.
    * 
    * Key Components:
    * - FormatManager class: Manages the current output format.
    * - SerializerFactory class: Serializes data into different formats.
    * - Templates: Handle different data types and custom format functions.
    *
    * External Dependencies:
    * - nlohmann/json
    * - yaml-cpp
    *
    * (c) 2024, Benjamin Gorlick | github.com/bgorlick
    * Distributed under the Boost Software License, Version 1.0.
    * (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
    * 
    * Version: 0.0.1
*/

/*
Key Classes and Functions:
1. FormatManager Class
   - Manages the current output format.
   - Functions:
     - `void set_format(OutputFormat format)`: Sets the current output format.
     - `OutputFormat get_format() const`: Gets the current output format.
     - `std::vector<OutputFormat> list_formats() const`: Lists all supported formats.
2. OutputFormat Enum
   - Enum class defining supported formats.
3. format_to_string() Function
   - Converts `OutputFormat` enum to string representation.
4. string_to_format() Function
   - Converts string representation to `OutputFormat` enum.
5. json_to_yaml() and yaml_to_json() Functions
   - Convert between JSON and YAML formats.
6. SerializerFactory Class
   - A factory class to serialize data into different formats.
   - Template Function: `static void serialize(std::ostream &stream, const T &data, OutputFormat format)`
7. apply_output_format() Functions
   - Overloaded functions to apply the current format to different types of data (`std::string`, 
     `nlohmann::json`, generic types).

*/

/*
------------------
Usage Examples:
------------------

#include "format_manager.hpp"

using namespace output_format;

*/


/* Example: Set the output format to JSON 

set_output_format(OutputFormat::JSON);
std::stringstream ss;
apply_output_format(ss, "Hello, World!");
std::cout << ss.str(); 

*/

/* Expected result (JSON): 

{ "output": "Hello, World!" }

*/

/* Example: Set the output format to YAML */
/*

set_output_format(OutputFormat::YAML);
apply_output_format(ss, "Hello, World!");
std::cout << ss.str(); 

*/

/* Expected result (YAML): 

output: Hello, World!

*/

/* Example: Serialize a JSON object */
/*

nlohmann::json json_obj = {{"key", "value"}};
ss.str(""); // Clear the stringstream
apply_output_format(ss, json_obj);
std::cout << ss.str(); 

*/

/* Expected result (JSON): 

{ "key": "value" }

*/

/* Expected result (YAML): 

key: value

*/

/* Example: Serialize a map */
/*

std::unordered_map<std::string, nlohmann::json> map_obj = {{"key", "value"}};
ss.str(""); // Clear the stringstream
apply_output_format(ss, map_obj);
std::cout << ss.str(); 

*/

/* Expected result (JSON): 

{ "key": "value" }

*/

/* Expected result (YAML): 

key: value

*/

/* Example: Nested JSON object */
/*

nlohmann::json nested_json = {{"outer", {{"inner", "value"}}}};
ss.str(""); // Clear the stringstream
apply_output_format(ss, nested_json);
std::cout << ss.str(); 

*/

/* Expected result (JSON): 

{ "outer": { "inner": "value" } }

*/

/* Expected result (YAML): 

outer:
  inner: value

*/

// File: format_manager.hpp


#ifndef FORMAT_MANAGER_HPP
#define FORMAT_MANAGER_HPP

#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

namespace output_format
{
    enum class OutputFormat
    {
        PLAIN_TEXT,
        JSON,
        XML,
        YAML,
        HTML,
        CSV
    };

    constexpr std::string_view format_to_string(OutputFormat format) noexcept
    {
        constexpr std::array<std::pair<OutputFormat, std::string_view>, 6> formatStrings = {{
            {OutputFormat::PLAIN_TEXT, "Plain Text"},
            {OutputFormat::JSON, "JSON"},
            {OutputFormat::XML, "XML"},
            {OutputFormat::YAML, "YAML"},
            {OutputFormat::HTML, "HTML"},
            {OutputFormat::CSV, "CSV"},
        }};
        for (const auto &pair : formatStrings)
        {
            if (pair.first == format)
            {
                return pair.second;
            }
        }
        return "Unknown";
    }

    inline OutputFormat string_to_format(const std::string &str)
    {
        static const std::unordered_map<std::string, OutputFormat> formatMap = {
            {"Plain Text", OutputFormat::PLAIN_TEXT},
            {"JSON", OutputFormat::JSON},
            {"XML", OutputFormat::XML},
            {"YAML", OutputFormat::YAML},
            {"HTML", OutputFormat::HTML},
            {"CSV", OutputFormat::CSV}
        };
        auto it = formatMap.find(str);
        if (it != formatMap.end())
        {
            return it->second;
        }
        throw std::invalid_argument("Unknown format: " + str);
    }

    class FormatManager
    {
    public:
        FormatManager() : current_format(OutputFormat::PLAIN_TEXT) {}
        void set_format(OutputFormat format)
        {
            std::unique_lock lock(mutex_);
            current_format = format;
        }
        OutputFormat get_format() const
        {
            std::shared_lock lock(mutex_);
            return current_format;
        }
        std::vector<OutputFormat> list_formats() const
        {
            return {OutputFormat::PLAIN_TEXT, OutputFormat::JSON, OutputFormat::XML, OutputFormat::YAML, OutputFormat::HTML, OutputFormat::CSV};
        }

    private:
        OutputFormat current_format;
        mutable std::shared_mutex mutex_;
    };

    // Singleton to get the FormatManager instance
    inline FormatManager &get_format_manager()
    {
        static FormatManager manager;
        return manager;
    }

    // JSON to YAML conversion
    inline YAML::Node json_to_yaml(const nlohmann::json &j)
    {
        YAML::Node node;
        switch (j.type())
        {
        case nlohmann::json::value_t::null:
            node = YAML::Node();
            break;
        case nlohmann::json::value_t::boolean:
            node = j.get<bool>();
            break;
        case nlohmann::json::value_t::number_integer:
            node = j.get<int>();
            break;
        case nlohmann::json::value_t::number_unsigned:
            node = j.get<unsigned>();
            break;
        case nlohmann::json::value_t::number_float:
            node = j.get<double>();
            break;
        case nlohmann::json::value_t::string:
            node = j.get<std::string>();
            break;
        case nlohmann::json::value_t::array:
            for (const auto &el : j)
                node.push_back(json_to_yaml(el));
            break;
        case nlohmann::json::value_t::object:
            for (auto it = j.begin(); it != j.end(); ++it)
                node[it.key()] = json_to_yaml(it.value());
            break;
        default:
            throw std::invalid_argument("Unsupported JSON type");
        }
        return node;
    }

    // YAML to JSON conversion
    inline nlohmann::json yaml_to_json(const YAML::Node &node)
    {
        nlohmann::json j;
        switch (node.Type())
        {
        case YAML::NodeType::Null:
            j = nullptr;
            break;
        case YAML::NodeType::Scalar:
            try
            {
                j = node.as<bool>();
            }
            catch (const YAML::BadConversion &)
            {
                try
                {
                    j = node.as<int>();
                }
                catch (const YAML::BadConversion &)
                {
                    try
                    {
                        j = node.as<double>();
                    }
                    catch (const YAML::BadConversion &)
                    {
                        j = node.as<std::string>();
                    }
                }
            }
            break;
        case YAML::NodeType::Sequence:
            for (auto it = node.begin(); it != node.end(); ++it)
            {
                j.push_back(yaml_to_json(*it));
            }
            break;
        case YAML::NodeType::Map:
            for (auto it = node.begin(); it != node.end(); ++it)
            {
                j[it->first.as<std::string>()] = yaml_to_json(it->second);
            }
            break;
        case YAML::NodeType::Undefined:
        default:
            throw std::runtime_error("Unsupported YAML node type");
        }
        return j;
    }

    // Helper functions for output formatting (std::string)
    inline void json_format(std::ostream &stream, const std::string &data)
    {
        std::stringstream ss;
        ss << "{\n \"output\": \"" << data << "\"\n}\n";
        stream << ss.str();
    }

    inline void xml_format(std::ostream &stream, const std::string &data)
    {
        std::stringstream ss;
        ss << "<output>\n " << data << "\n</output>\n";
        stream << ss.str();
    }

    inline void yaml_format(std::ostream &stream, const std::string &data)
    {
        std::stringstream ss;
        ss << "output:\n " << data << "\n";
        stream << ss.str();
    }

    inline void plain_text_format(std::ostream &stream, const std::string &data)
    {
        std::stringstream ss;
        ss << data << "\n";
        stream << ss.str();
    }

    inline void html_format(std::ostream &stream, const std::string &data)
    {
        std::stringstream ss;
        ss << "<html><body><p>" << data << "</p></body></html>\n";
        stream << ss.str();
    }

    inline void csv_format(std::ostream &stream, const std::string &data)
    {
        std::stringstream ss;
        ss << "\"output\"," << "\"" << data << "\"\n";
        stream << ss.str();
    }

    // Helper functions for output formatting (nlohmann::json)
    inline void json_format(std::ostream &stream, const nlohmann::json &data)
    {
        stream << data.dump(4) << "\n"; // Add newline at the end
    }

    inline void xml_format(std::ostream &stream, const nlohmann::json &data)
    {
        stream << "<output>\n";
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            stream << "  <" << it.key() << ">" << it.value() << "</" << it.key() << ">\n";
        }
        stream << "</output>\n";
    }

    inline void yaml_format(std::ostream &stream, const nlohmann::json &data)
    {
        YAML::Emitter out;
        out << json_to_yaml(data);
        stream << out.c_str() << "\n"; // Add newline at the end
    }

    inline void plain_text_format(std::ostream &stream, const nlohmann::json &data)
    {
        stream << data.dump() << "\n"; // Add newline at the end
    }

    inline void html_format(std::ostream &stream, const nlohmann::json &data)
    {
        stream << "<html><body><pre>" << data.dump(4) << "</pre></body></html>\n";
    }

    inline void csv_format(std::ostream &stream, const nlohmann::json &data)
    {
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            stream << "\"" << it.key() << "\",\"" << it.value() << "\"\n";
        }
    }

    // Specialization for std::unordered_map<std::string, nlohmann::json>
    inline void json_format(std::ostream &stream, const std::unordered_map<std::string, nlohmann::json> &data)
    {
        nlohmann::json json_obj(data);
        stream << json_obj.dump(4) << "\n"; // Add newline at the end
    }

    inline void xml_format(std::ostream &stream, const std::unordered_map<std::string, nlohmann::json> &data)
    {
        stream << "<output>\n";
        for (const auto &[key, value] : data)
        {
            stream << "  <" << key << ">" << value << "</" << key << ">\n";
        }
        stream << "</output>\n";
    }

    inline void yaml_format(std::ostream &stream, const std::unordered_map<std::string, nlohmann::json> &data)
    {
        YAML::Emitter out;
        out << json_to_yaml(nlohmann::json(data));
        stream << out.c_str() << "\n"; // Add newline at the end
    }

    inline void plain_text_format(std::ostream &stream, const std::unordered_map<std::string, nlohmann::json> &data)
    {
        nlohmann::json json_obj(data);
        stream << json_obj.dump() << "\n"; // Add newline at the end
    }

    inline void html_format(std::ostream &stream, const std::unordered_map<std::string, nlohmann::json> &data)
    {
        nlohmann::json json_obj(data);
        stream << "<html><body><pre>" << json_obj.dump(4) << "</pre></body></html>\n";
    }

    inline void csv_format(std::ostream &stream, const std::unordered_map<std::string, nlohmann::json> &data)
    {
        for (const auto &[key, value] : data)
        {
            stream << "\"" << key << "\",\"" << value << "\"\n";
        }
    }

    // Specialization for std::vector<T>
    template <typename T>
    inline void json_format(std::ostream &stream, const std::vector<T> &data)
    {
        nlohmann::json json_obj(data);
        stream << json_obj.dump(4) << "\n"; // Add newline at the end
    }

    template <typename T>
    inline void xml_format(std::ostream &stream, const std::vector<T> &data)
    {
        stream << "<output>\n";
        for (const auto &item : data)
        {
            stream << "  <item>" << item << "</item>\n";
        }
        stream << "</output>\n";
    }

    template <typename T>
    inline void yaml_format(std::ostream &stream, const std::vector<T> &data)
    {
        YAML::Emitter out;
        out << json_to_yaml(nlohmann::json(data));
        stream << out.c_str() << "\n";
    }

    template <typename T>
    inline void plain_text_format(std::ostream &stream, const std::vector<T> &data)
    {
        nlohmann::json json_obj(data);
        stream << json_obj.dump() << "\n"; // Add newline at the end
    }

    template <typename T>
    inline void html_format(std::ostream &stream, const std::vector<T> &data)
    {
        nlohmann::json json_obj(data);
        stream << "<html><body><pre>" << json_obj.dump(4) << "</pre></body></html>\n";
    }

    template <typename T>
    inline void csv_format(std::ostream &stream, const std::vector<T> &data)
    {
        for (const auto &item : data)
        {
            stream << "\"" << item << "\"\n";
        }
    }

    // Custom format detection templates
    template <typename T, typename = void>
    struct has_custom_json_format : std::false_type {};

    // Specialization for when custom_json_format exists
    template <typename T>
    struct has_custom_json_format<T, std::void_t<decltype(custom_json_format(std::declval<std::ostream &>(), std::declval<const T &>()))>> : std::true_type {};

    template <typename T, typename = void>
    struct has_custom_xml_format : std::false_type {};

    // Specialization for when custom_xml_format exists
    template <typename T>
    struct has_custom_xml_format<T, std::void_t<decltype(custom_xml_format(std::declval<std::ostream &>(), std::declval<const T &>()))>> : std::true_type {};

    template <typename T, typename = void>
    struct has_custom_yaml_format : std::false_type {};

    // Specialization for when custom_yaml_format exists
    template <typename T>
    struct has_custom_yaml_format<T, std::void_t<decltype(custom_yaml_format(std::declval<std::ostream &>(), std::declval<const T &>()))>> : std::true_type {};

    template <typename T, typename = void>
    struct has_custom_plain_text_format : std::false_type {};

    // Specialization for when custom_plain_text_format exists
    template <typename T>
    struct has_custom_plain_text_format<T, std::void_t<decltype(custom_plain_text_format(std::declval<std::ostream &>(), std::declval<const T &>()))>> : std::true_type {};

    template <typename T, typename = void>
    struct has_custom_html_format : std::false_type {};

    // Specialization for when custom_html_format exists
    template <typename T>
    struct has_custom_html_format<T, std::void_t<decltype(custom_html_format(std::declval<std::ostream &>(), std::declval<const T &>()))>> : std::true_type {};

    template <typename T, typename = void>
    struct has_custom_csv_format : std::false_type {};

    // Specialization for when custom_csv_format exists
    template <typename T>
    struct has_custom_csv_format<T, std::void_t<decltype(custom_csv_format(std::declval<std::ostream &>(), std::declval<const T &>()))>> : std::true_type {};

    // Generic template format functions
    template <typename T>
    inline std::enable_if_t<has_custom_json_format<T>::value> json_format(std::ostream &stream, const T &data)
    {
        custom_json_format(stream, data);
    }

    template <typename T>
    inline std::enable_if_t<!has_custom_json_format<T>::value> json_format(std::ostream &stream, const T &data)
    {
        stream << "{\"unsupported_type\": \"No custom JSON format available.\"}";
    }

    template <typename T>
    inline std::enable_if_t<has_custom_xml_format<T>::value> xml_format(std::ostream &stream, const T &data)
    {
        custom_xml_format(stream, data);
    }

    template <typename T>
    inline std::enable_if_t<!has_custom_xml_format<T>::value> xml_format(std::ostream &stream, const T &data)
    {
        stream << "<unsupported_type>No custom XML format available.</unsupported_type>";
    }

    template <typename T>
    inline std::enable_if_t<has_custom_yaml_format<T>::value> yaml_format(std::ostream &stream, const T &data)
    {
        custom_yaml_format(stream, data);
    }

    template <typename T>
    inline std::enable_if_t<!has_custom_yaml_format<T>::value> yaml_format(std::ostream &stream, const T &data)
    {
        stream << "unsupported_type: No custom YAML format available.\n";
    }

    template <typename T>
    inline std::enable_if_t<has_custom_plain_text_format<T>::value> plain_text_format(std::ostream &stream, const T &data)
    {
        custom_plain_text_format(stream, data);
    }

    template <typename T>
    inline std::enable_if_t<!has_custom_plain_text_format<T>::value> plain_text_format(std::ostream &stream, const T &data)
    {
        stream << "No custom plain text format available.";
    }

    template <typename T>
    inline std::enable_if_t<has_custom_html_format<T>::value> html_format(std::ostream &stream, const T &data)
    {
        custom_html_format(stream, data);
    }

    template <typename T>
    inline std::enable_if_t<!has_custom_html_format<T>::value> html_format(std::ostream &stream, const T &data)
    {
        stream << "<html><body><p>No custom HTML format available.</p></body></html>";
    }

    template <typename T>
    inline std::enable_if_t<has_custom_csv_format<T>::value> csv_format(std::ostream &stream, const T &data)
    {
        custom_csv_format(stream, data);
    }

    template <typename T>
    inline std::enable_if_t<!has_custom_csv_format<T>::value> csv_format(std::ostream &stream, const T &data)
    {
        stream << "key,value\nNo custom CSV format available,";
    }

    // Factory class for creating serializers
    class SerializerFactory
    {
    public:
        template <typename T>
        static void serialize(std::ostream &stream, const T &data, OutputFormat format)
        {
            switch (format)
            {
            case OutputFormat::PLAIN_TEXT:
                plain_text_format(stream, data);
                break;
            case OutputFormat::JSON:
                json_format(stream, data);
                break;
            case OutputFormat::XML:
                xml_format(stream, data);
                break;
            case OutputFormat::YAML:
                yaml_format(stream, data);
                break;
            case OutputFormat::HTML:
                html_format(stream, data);
                break;
            case OutputFormat::CSV:
                csv_format(stream, data);
                break;
            default:
                throw std::invalid_argument("Unsupported format");
            }
        }
    };

    // Function to handle std::string with specified format
    inline void apply_output_format(std::ostream &stream, const std::string &data, OutputFormat format)
    {
        try
        {
            SerializerFactory::serialize(stream, data, format);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error applying output format: " << e.what() << std::endl;
        }
    }

    // Overloaded function to handle std::ostream and OutputFormat for nlohmann::json
    inline void apply_output_format(std::ostream &stream, const nlohmann::json &data, OutputFormat format)
    {
        try
        {
            SerializerFactory::serialize(stream, data, format);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error applying output format: " << e.what() << std::endl;
        }
    }

    // Template function to handle generic types
    template <typename T>
    inline void apply_output_format(std::ostream &stream, const T &data)
    {
        auto format = get_format_manager().get_format();
        try
        {
            SerializerFactory::serialize(stream, data, format);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error applying output format: " << e.what() << std::endl;
        }
    }

    // Overloaded function to handle std::ostream and OutputFormat for generic types
    template <typename T>
    inline void apply_output_format(std::ostream &stream, const T &data, OutputFormat format)
    {
        try
        {
            SerializerFactory::serialize(stream, data, format);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error applying output format: " << e.what() << std::endl;
        }
    }

    inline void set_output_format(OutputFormat format)
    {
        get_format_manager().set_format(format);
    }

    inline OutputFormat get_output_format()
    {
        return get_format_manager().get_format();
    }

    inline std::vector<OutputFormat> list_output_formats()
    {
        return get_format_manager().list_formats();
    }

    // Function to reset the output format to default
    inline void reset_output_format()
    {
        set_output_format(OutputFormat::PLAIN_TEXT);
    }

    template<typename ConfigType>
    inline void set_format_and_output(OutputFormat format, ConfigType &config, std::ostream &os)
    {
        set_output_format(format);
        config.template output_config<std::ostream&>(os);
    }

} // namespace output_format

#endif // FORMAT_MANAGER_HPP
