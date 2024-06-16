/*
    * configuration.hpp
    *
    * Header-only library for managing configurations in C++.
    * Supports loading, saving, and managing configurations from JSON, YAML, and environment variables.
    * 
    * Key Components:
    * - IConfigStorage interface: Defines the required configuration management functions.
    * - Config class: Implements the IConfigStorage interface and provides configuration management functionality.
    * - ConfigFactory class: Provides factory methods to create and manage Config instances.
    * - Templates: Handle different data types and custom format functions.
    *
    * External Dependencies:
    * - nlohmann/json
    * - yaml-cpp
    *
    * (c) 2024, Benjamin Gorlick | github.com/bgorlick/config_manager/
    * Distributed under the Boost Software License, Version 1.0.
    * (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
    * 
    * Version 0.0.1
*/

/*
Key Classes and Functions:
1. IConfigStorage Interface
   - Defines the required configuration management functions.
   - Functions:
     - `virtual nlohmann::json get(const std::string &key) const = 0`: Gets the value for a given key.
     - `virtual void set(const std::string &key, const nlohmann::json &value) = 0`: Sets the value for a given key.
     - `virtual std::unordered_map<std::string, nlohmann::json> get_all() const = 0`: Gets all key-value pairs.
     - `virtual void load_from_file(const std::string &file_path) = 0`: Loads configuration from a file.
     - `virtual void save_to_file(const std::string &file_path) const = 0`: Saves configuration to a file.
     - `virtual void remove(const std::string &key) = 0`: Removes a key-value pair.
     - `virtual bool exists(const std::string &key) const = 0`: Checks if a key exists.
     - `virtual void clear() = 0`: Clears all configurations.
     - `virtual void load_from_file(const std::string &file_path, const std::string &version) = 0`: Loads configuration from a file with a specific version.
     - `virtual void save_to_file(const std::string &file_path, const std::string &version) const = 0`: Saves configuration to a file with a specific version.
     - `virtual void load_from_env() = 0`: Loads configuration from environment variables.
     - `virtual void add_change_listener(const std::function<void(const std::string &, const nlohmann::json &)> &listener) = 0`: Adds a change listener.
     - `virtual void backup_to_file(const std::string &backup_file_path) const = 0`: Backs up the configuration to a file.
     - `template<typename T = void> typename std::enable_if<std::is_same<T, std::ostream&>::value, std::ostream&>::type output_config(std::ostream &os) const`: Outputs the configuration to an ostream.
     - `template<typename T = void> typename std::enable_if<std::is_same<T, void>::value, void>::type output_config(std::ostream &os) const`: Outputs the configuration to an ostream (void specialization).
2. Config Class
   - Implements the IConfigStorage interface and provides configuration management functionality.
   - Functions: Same as IConfigStorage interface, with additional functions for instance management.
3. ConfigFactory Class
   - Provides factory methods to create and manage Config instances.
   - Functions:
     - `static std::shared_ptr<Config> create_config(const std::string &name = "default")`: Creates a basic config instance.
     - `static std::shared_ptr<Config> create_new_config_from_existing(const std::string &name, const std::string &filePath)`: Creates a new config instance from an existing configuration file.
     - `static std::shared_ptr<Config> create_config_with_defaults(const std::string &name, const std::unordered_map<std::string, nlohmann::json> &defaults)`: Creates a config instance with default values.
     - `static std::shared_ptr<Config> create_env_config(const std::string &name, const std::string &environment)`: Creates a config instance for a specific environment.
     - `static std::shared_ptr<Config> create_env_loaded_config(const std::string &name)`: Creates a config instance and loads it from environment variables.
     - `static std::shared_ptr<Config> create_thread_safe_config(const std::string &name = "default")`: Thread-safe method to create or get a config instance.
     - `static std::shared_ptr<Config> get_pooled_config(const std::string &name = "default")`: Pools and reuses instances.
4. Template Functions
   - Handle different data types and custom format functions.
*/

/*
------------------
Usage Examples:
------------------

#include "configuration.hpp"

using namespace config;

*/

/* Example: Create a default config instance 

Config &config = Config::instance();
config.set("name", "example");
std::cout << config.get("name") << std::endl;

*/

/* Expected result: 

example

*/

/* Example: Load configuration from JSON file */
/*

config.load_from_file("config.json");
std::cout << config.get("name") << std::endl;

*/

/* Expected result: 

example

*/

/* Example: Save configuration to YAML file */
/*

config.save_to_file("config.yaml", "1.0.0");

*/

/* Example: Add change listener */
/*

bool listener_called = false;
config.add_change_listener([&listener_called](const std::string &key, const nlohmann::json &value) {
    if (key == "name" && value == "example_listener") {
        listener_called = true;
    }
});
config.set("name", "example_listener");

*/

/* Expected result: 

Listener is called, listener_called is true

*/

/* Example: Load configuration from environment variables */
/*

setenv("name", "env_example", 1);
config.load_from_env();
std::cout << config.get("name") << std::endl;

*/

/* Expected result: 

env_example

*/

/* Example: Backup configuration to file */
/*

config.backup_to_file("backup.json");

*/

/* Example: Validate configuration */
/*

std::unordered_map<std::string, std::function<bool(const nlohmann::json &)>> validators = {
    {"name", [](const nlohmann::json &value) { return value.is_string(); }},
    {"age", [](const nlohmann::json &value) { return value.is_number_integer() && value > 0; }}
};
config.validate(validators);

*/

/* Example: Inspect configuration keys */
/*

std::vector<nlohmann::json> inspected = config.inspect({"name", "complex"});
for (const auto &value : inspected) {
    std::cout << value.dump(4) << std::endl;
}

*/

/* Expected result: 

"example"
{
    "key1": "value1",
    "key2": 42,
    "key3": {
        "nestedKey": "nestedValue"
    }
}

*/

/* Example: Update multiple configuration values */
/*

std::unordered_map<std::string, nlohmann::json> new_cfg = {
    {"name", "new_example"},
    {"age", 30}
};
config.update_multiple(new_cfg);
std::cout << config.get("name") << std::endl;
std::cout << config.get("age") << std::endl;

*/

/* Expected result: 

new_example
30

*/

/* Example: Partial save and load (JSON) */
/*

config.save_partial_to_file("partial_config.json", {"complex"});
config.clear();
config.load_partial_from_file("partial_config.json", {"complex"});
std::cout << config.get("complex").dump(4) << std::endl;

*/

/* Expected result: 

{
    "key1": "value1",
    "key2": 42,
    "key3": {
        "nestedKey": "nestedValue"
    }
}

*/

/* Example: Output configuration in different formats */
/*

set_output_format(OutputFormat::JSON);
config.output_config<std::ostream&>(std::cout) << std::endl;

set_output_format(OutputFormat::YAML);
config.output_config<void>(std::cout);

*/

/* Expected result: 

{
    "complex": {
        "key1": "value1",
        "key2": 42,
        "key3": {
            "nestedKey": "nestedValue"
        }
    }
}

complex:
  key1: value1
  key2: 42
  key3:
    nestedKey: value

*/

// File: configuration.hpp


#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <mutex>
#include <fstream>
#include <stdexcept>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <unistd.h> // For environ
#include <string.h> 


#define FORMAT_MANAGER_INCLUDED  // Comment out line to exclude format manager functionality

#ifdef FORMAT_MANAGER_INCLUDED
#include "format_manager.hpp"
#endif

namespace config
{

    class IConfigStorage
    {
    public:
        virtual ~IConfigStorage() = default;
        virtual nlohmann::json get(const std::string &key) const = 0;
        virtual void set(const std::string &key, const nlohmann::json &value) = 0;
        virtual std::unordered_map<std::string, nlohmann::json> get_all() const = 0;
        virtual void load_from_file(const std::string &file_path) = 0;
        virtual void save_to_file(const std::string &file_path) const = 0;
        virtual void remove(const std::string &key) = 0;
        virtual bool exists(const std::string &key) const = 0;
        virtual void clear() = 0;
        virtual void load_from_file(const std::string &file_path, const std::string &version) = 0;
        virtual void save_to_file(const std::string &file_path, const std::string &version) const = 0;
        virtual void load_from_env() = 0;
        virtual void add_change_listener(const std::function<void(const std::string &, const nlohmann::json &)> &listener) = 0;
        virtual void backup_to_file(const std::string &backup_file_path) const = 0;

#ifdef FORMAT_MANAGER_INCLUDED
        template<typename T = void>
        typename std::enable_if<std::is_same<T, std::ostream&>::value, std::ostream&>::type
        output_config(std::ostream &os) const;

        template<typename T = void>
        typename std::enable_if<std::is_same<T, void>::value, void>::type
        output_config(std::ostream &os) const;
#endif
    };

    class Config : public IConfigStorage
    {
        friend Config& instance(const std::string &name);

    public:
        Config() = default;
        static Config& instance(const std::string &name = "default");

        // Public method declarations
        nlohmann::json get(const std::string &key) const override;
        void set(const std::string &key, const nlohmann::json &value) override;
        std::unordered_map<std::string, nlohmann::json> get_all() const override;
        void validate(const std::unordered_map<std::string, std::function<bool(const nlohmann::json &)>> &validators) const;
        void load_from_file(const std::string &file_path) override;
        void save_to_file(const std::string &file_path) const override;
        void remove(const std::string &key) override;
        bool exists(const std::string &key) const override;
        void clear() override;
        void display() const;
        std::vector<nlohmann::json> inspect(const std::vector<std::string> &keys) const;
        void update_multiple(const std::unordered_map<std::string, nlohmann::json> &new_cfg);
        void load_partial_from_file(const std::string &file_path, const std::vector<std::string> &keys);
        void save_partial_to_file(const std::string &file_path, const std::vector<std::string> &keys) const;
        void load_from_file(const std::string &file_path, const std::string &version) override;
        void save_to_file(const std::string &file_path, const std::string &version) const override;
        void load_from_env() override;
        void add_change_listener(const std::function<void(const std::string &, const nlohmann::json &)> &listener) override;
        void backup_to_file(const std::string &backup_file_path) const override;

#ifdef FORMAT_MANAGER_INCLUDED
        template<typename T = void>
        typename std::enable_if<std::is_same<T, std::ostream&>::value, std::ostream&>::type
        output_config(std::ostream &os) const;

        template<typename T = void>
        typename std::enable_if<std::is_same<T, void>::value, void>::type
        output_config(std::ostream &os) const;
#endif
        
    private:
        Config(const Config& other) = delete; // Do not allow copying
        Config& operator=(const Config& other) = delete; // Do not allow assignment
        Config(Config&& other) noexcept; // Custom move constructor
        Config& operator=(Config&& other) noexcept; // Custom move assignment

        std::unordered_map<std::string, nlohmann::json> config_map;
        std::vector<std::function<void(const std::string &, const nlohmann::json &)>> change_listeners_;
        mutable std::mutex mutex_;
        std::string version_;
        std::unordered_map<std::string, std::string> env_overrides_;
    };

#ifdef FORMAT_MANAGER_INCLUDED
    // Template function definitions
    template<typename T>
    typename std::enable_if<std::is_same<T, std::ostream&>::value, std::ostream&>::type
    Config::output_config(std::ostream &os) const
    {
        try
        {
            auto format = output_format::get_format_manager().get_format();
            std::stringstream ss;
            switch (format)
            {
            case output_format::OutputFormat::PLAIN_TEXT:
                for (const auto &[key, value] : config_map)
                {
                    ss << key << ": " << value.dump() << "\n";
                }
                output_format::plain_text_format(os, ss.str());
                break;
            case output_format::OutputFormat::JSON:
                output_format::json_format(os, config_map);
                break;
            case output_format::OutputFormat::XML:
                output_format::xml_format(os, config_map);
                break;
            case output_format::OutputFormat::YAML:
                output_format::yaml_format(os, config_map);
                break;
            case output_format::OutputFormat::HTML:
                output_format::html_format(os, config_map);
                break;
            case output_format::OutputFormat::CSV:
                output_format::csv_format(os, config_map);
                break;
            default:
                throw std::invalid_argument("Unsupported format");
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in output_config: " << e.what() << std::endl;
        }
        return os;
    }

    template<typename T>
    typename std::enable_if<std::is_same<T, void>::value, void>::type
    Config::output_config(std::ostream &os) const
    {
        output_config<std::ostream&>(os);
    }
#endif

    Config& Config::instance(const std::string &name)
    {
        static std::unordered_map<std::string, std::unique_ptr<Config>> instances;
        static std::mutex mutex_;
        std::lock_guard<std::mutex> lock(mutex_);
        if (instances.find(name) == instances.end())
        {
            instances[name] = std::make_unique<Config>();
        }
        return *instances[name];
    }

    // Move constructor definition
    Config::Config(Config&& other) noexcept
        : config_map(std::move(other.config_map)),
          change_listeners_(std::move(other.change_listeners_)),
          version_(std::move(other.version_)),
          env_overrides_(std::move(other.env_overrides_))
    {
        // Note: `std::mutex` cannot be moved, so we leave it default constructed.
    }

    // Move assignment operator definition
    Config& Config::operator=(Config&& other) noexcept
    {
        if (this != &other)
        {
            std::lock(mutex_, other.mutex_);
            std::lock_guard<std::mutex> lhs_lock(mutex_, std::adopt_lock);
            std::lock_guard<std::mutex> rhs_lock(other.mutex_, std::adopt_lock);

            config_map = std::move(other.config_map);
            change_listeners_ = std::move(other.change_listeners_);
            version_ = std::move(other.version_);
            env_overrides_ = std::move(other.env_overrides_);
        }
        return *this;
    }

    class ConfigFactory
    {
    public:
        // Create a basic config instance
        static std::shared_ptr<Config> create_config(const std::string &name = "default")
        {
            return std::shared_ptr<Config>(&Config::instance(name), [](Config*){});
        }

        // Create a new config instance from an existing configuration file
        static std::shared_ptr<Config> create_new_config_from_existing(const std::string &name, const std::string &filePath)
        {
            auto config = create_config(name);
            try
            {
                config->load_from_file(filePath);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Failed to load configuration from file: " << filePath << ". Error: " << e.what() << std::endl;
                return nullptr; // Return a null pointer to indicate failure
            }
            return config;
        }

        // Create a config instance with default values
        static std::shared_ptr<Config> create_config_with_defaults(const std::string &name, const std::unordered_map<std::string, nlohmann::json> &defaults)
        {
            auto config = create_config(name);
            for (const auto &[key, value] : defaults)
            {
                config->set(key, value);
            }
            return config;
        }

        // Create a config instance for a specific environment
        static std::shared_ptr<Config> create_env_config(const std::string &name, const std::string &environment)
        {
            auto config = create_config(name);
            if (environment == "development")
            {
                config->set("db_host", "localhost");
                config->set("db_port", 5432);
                config->set("api_endpoint", "https://dev.api.example.com");
                config->set("log_level", "debug");
                config->set("feature_x_enabled", true);
            }
            else if (environment == "production")
            {
                config->set("db_host", "prod.db.server");
                config->set("db_port", 5432);
                config->set("api_endpoint", "https://api.example.com");
                config->set("log_level", "error");
                config->set("feature_x_enabled", false);
            }
            else if (environment == "testing")
            {
                config->set("db_host", "test.db.server");
                config->set("db_port", 5432);
                config->set("api_endpoint", "https://test.api.example.com");
                config->set("log_level", "info");
                config->set("feature_x_enabled", true);
            }
            else
            {
                throw std::invalid_argument("Unsupported environment: " + environment);
            }
            return config;
        }

        // Create a config instance and load it from environment variables
        static std::shared_ptr<Config> create_env_loaded_config(const std::string &name)
        {
            auto config = create_config(name);
            try
            {
                config->load_from_env();
                std::cout << "Configuration loaded successfully from environment variables for: " << name << "\n";
            }
            catch (const std::exception &e)
            {
                std::cerr << "Failed to load configuration from environment variables for: " << name << ". Error: " << e.what() << std::endl;
                return nullptr; // Return a null pointer to indicate failure
            }
            return config;
        }

        // Thread-safe method to create or get a config instance
        static std::shared_ptr<Config> create_thread_safe_config(const std::string &name = "default")
        {
            static std::mutex mutex_;
            std::lock_guard<std::mutex> lock(mutex_);
            try
            {
                Config &instance = Config::instance(name);
                std::cout << "Thread-safe configuration created or retrieved for: " << name << "\n";
                return std::shared_ptr<Config>(&instance, [](Config*){});
            }
            catch (const std::exception &e)
            {
                std::cerr << "Failed to create or retrieve thread-safe configuration for: " << name << ". Error: " << e.what() << std::endl;
                return nullptr; // Return a null pointer to indicate failure
            }
        }

        // Pool and reuse instances
        static std::shared_ptr<Config> get_pooled_config(const std::string &name = "default")
        {
            static std::unordered_map<std::string, std::shared_ptr<Config>> pool;
            static std::mutex mutex_;
            std::lock_guard<std::mutex> lock(mutex_);
            try
            {
                if (pool.find(name) == pool.end())
                {
                    pool[name] = create_config(name);
                    std::cout << "New configuration created and added to pool for: " << name << "\n";
                }
                else
                {
                    std::cout << "Configuration retrieved from pool for: " << name << "\n";
                }
                return pool[name];
            }
            catch (const std::exception &e)
            {
                std::cerr << "Failed to create or retrieve pooled configuration for: " << name << ". Error: " << e.what() << std::endl;
                return nullptr; // Return a null pointer to indicate failure
            }
        }
    };

    // Enhanced Functions
    nlohmann::json Config::get(const std::string &key) const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = config_map.find(key);
        if (it != config_map.end())
        {
            return it->second;
        }
        throw std::invalid_argument("Unknown configuration key: " + key);
    }

    void Config::set(const std::string &key, const nlohmann::json &value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!key.empty())
        {
            if (key == "example" && !value.is_string())
            {
                throw std::invalid_argument("Value for 'example' must be a string");
            }
            config_map[key] = value;
            for (const auto &listener : change_listeners_)
            {
                listener(key, value);
            }
        }
        else
        {
            throw std::invalid_argument("Key cannot be empty");
        }
    }

    std::unordered_map<std::string, nlohmann::json> Config::get_all() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return config_map;
    }

    void Config::validate(const std::unordered_map<std::string, std::function<bool(const nlohmann::json &)>> &validators) const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        try
        {
            for (const auto &[key, validate_func] : validators)
            {
                auto it = config_map.find(key);
                if (it != config_map.end())
                {
                    if (!validate_func(it->second))
                    {
                        throw std::invalid_argument("Validation failed for key: " + key + " with value: " + it->second.dump());
                    }
                }
                else
                {
                    throw std::invalid_argument("Validation failed: key not found: " + key);
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in validate: " << e.what() << std::endl;
        }
    }

    void Config::load_from_file(const std::string &file_path)
    {
        try
        {
            load_from_file(file_path, "1.0.0");
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in load_from_file: " << e.what() << std::endl;
        }
    }

    void Config::save_to_file(const std::string &file_path) const
    {
        try
        {
            save_to_file(file_path, "1.0.0");
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in save_to_file: " << e.what() << std::endl;
        }
    }

    void Config::remove(const std::string &key)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        try
        {
            if (config_map.erase(key) == 0)
            {
                throw std::invalid_argument("Unknown configuration key: " + key);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in remove: " << e.what() << std::endl;
        }
    }

    bool Config::exists(const std::string &key) const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        try
        {
            return config_map.find(key) != config_map.end();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in exists: " << e.what() << std::endl;
            return false;
        }
    }

    void Config::clear()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        try
        {
            config_map.clear();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in clear: " << e.what() << std::endl;
        }
    }

    void Config::display() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        try
        {
            for (const auto &[key, value] : config_map)
            {
                std::cout << key << ": " << value.dump(4) << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in display: " << e.what() << std::endl;
        }
    }

    std::vector<nlohmann::json> Config::inspect(const std::vector<std::string> &keys) const
    {
        std::unique_lock<std::mutex> lock(mutex_);

        std::vector<nlohmann::json> values;
        values.reserve(keys.size());

        for (const auto &key : keys)
        {
            lock.unlock();
            try
            {
                values.push_back(get(key));
            }
            catch (const std::invalid_argument &e)
            {
                values.push_back(nlohmann::json::object());
            }
            lock.lock();
        }

        return values;
    }

    void Config::update_multiple(const std::unordered_map<std::string, nlohmann::json> &new_cfg)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        try
        {
            for (const auto &[key, value] : new_cfg)
            {
                set(key, value);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in update_multiple: " << e.what() << std::endl;
        }
    }

    void Config::load_partial_from_file(const std::string &file_path, const std::vector<std::string> &keys)
    {
        std::ifstream config_file(file_path);
        if (!config_file.is_open())
        {
            std::cerr << "Failed to open config file for reading: " + file_path << std::endl;
            return;
        }
        try
        {
            std::string extension = file_path.substr(file_path.find_last_of(".") + 1);
            if (extension == "json")
            {
                nlohmann::json j;
                config_file >> j;
                for (const auto &key : keys)
                {
                    if (j.contains(key))
                    {
                        config_map[key] = j[key];
                    }
                }
            }
            else if (extension == "yaml" || extension == "yml")
            {
                YAML::Node yaml_config = YAML::Load(config_file);
                for (const auto &key : keys)
                {
                    if (yaml_config[key])
                    {
                    #ifdef FORMAT_MANAGER_INCLUDED
                    config_map[key] = output_format::yaml_to_json(yaml_config[key]);
                    #endif
                    }
                }
            }
            else
            {
                throw std::runtime_error("Unsupported config file format: " + extension);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error while loading config file: " << e.what() << std::endl;
        }
    }

    void Config::save_partial_to_file(const std::string &file_path, const std::vector<std::string> &keys) const
    {
        std::ofstream config_file(file_path);
        if (!config_file.is_open())
        {
            std::cerr << "Failed to open config file for writing: " + file_path << std::endl;
            return;
        }
        try
        {
            std::string extension = file_path.substr(file_path.find_last_of(".") + 1);
            if (extension == "json")
            {
                nlohmann::json j;
                for (const auto &key : keys)
                {
                    auto it = config_map.find(key);
                    if (it != config_map.end())
                    {
                        j[key] = it->second;
                    }
                }
                config_file << j.dump(4);
            }
            else if (extension == "yaml" || extension == "yml")
            {
                YAML::Emitter out;
                out << YAML::BeginMap;
                for (const auto &key : keys)
                {
                    auto it = config_map.find(key);
                    if (it != config_map.end())
                    {
                        #ifdef FORMAT_MANAGER_INCLUDED
                        out << YAML::Key << key << YAML::Value << output_format::json_to_yaml(it->second);
                        #endif
                    }
                }
                out << YAML::EndMap;
                config_file << out.c_str();
            }
            else
            {
                throw std::runtime_error("Unsupported config file format: " + extension);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error while saving config file: " << e.what() << std::endl;
        }
    }

    void Config::load_from_file(const std::string &file_path, const std::string &version)
    {
        std::ifstream config_file(file_path);
        if (!config_file.is_open())
        {
            std::cerr << "Failed to open config file for reading: " << file_path << std::endl;
            return;
        }
        try
        {
            std::string extension = file_path.substr(file_path.find_last_of(".") + 1);
            if (extension == "json")
            {
                nlohmann::json j;
                config_file >> j;
                for (auto it = j.begin(); it != j.end(); ++it)
                {
                    config_map[it.key()] = it.value();
                }
            }
            else if (extension == "yaml" || extension == "yml")
            {
                YAML::Node yaml_config = YAML::Load(config_file);
                for (auto it = yaml_config.begin(); it != yaml_config.end(); ++it)
                {
                    #ifdef FORMAT_MANAGER_INCLUDED
                    config_map[it->first.as<std::string>()] = output_format::yaml_to_json(it->second);
                    #endif
                }
            }
            else
            {
                throw std::runtime_error("Unsupported config file format: " + extension);
            }
            version_ = version;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error while loading config file: " << e.what() << std::endl;
        }
    }

    void Config::save_to_file(const std::string &file_path, const std::string &version) const
    {
        std::ofstream config_file(file_path);
        if (!config_file.is_open())
        {
            std::cerr << "Failed to open config file for writing: " << file_path << std::endl;
            return;
        }
        try
        {
            std::string extension = file_path.substr(file_path.find_last_of(".") + 1);
            if (extension == "json")
            {
                nlohmann::json j(config_map);
                j["version"] = version;
                config_file << j.dump(4);
            }
            else if (extension == "yaml" || extension == "yml")
            {
                YAML::Emitter out;
                out << YAML::BeginMap;
                out << YAML::Key << "version" << YAML::Value << version;
                for (const auto &[key, value] : config_map)
                {
                    #ifdef FORMAT_MANAGER_INCLUDED
                    out << YAML::Key << key << YAML::Value << output_format::json_to_yaml(value);
                    #endif
                }
                out << YAML::EndMap;
                config_file << out.c_str();
            }
            else
            {
                throw std::runtime_error("Unsupported config file format: " + extension);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error while saving config file: " << e.what() << std::endl;
        }
    }

    void Config::load_from_env()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        try
        {
            // Load all existing keys in config_map from environment
            for (const auto &[key, value] : config_map)
            {
                const char *env_val = std::getenv(key.c_str());
                if (env_val)
                {
                    config_map[key] = nlohmann::json(env_val);
                    env_overrides_[key] = env_val;
                }
            }

            // Load all environment variables into config_map
            for (char **env = environ; *env != 0; env++)
            {
                std::string env_entry = *env;
                size_t pos = env_entry.find('=');
                if (pos != std::string::npos)
                {
                    std::string key = env_entry.substr(0, pos);
                    std::string value = env_entry.substr(pos + 1);
                    config_map[key] = nlohmann::json(value);
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error while loading from environment: " << e.what() << std::endl;
        }
    }

    void Config::add_change_listener(const std::function<void(const std::string &, const nlohmann::json &)> &listener)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        try
        {
            change_listeners_.push_back(listener);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in add_change_listener: " << e.what() << std::endl;
        }
    }

    void Config::backup_to_file(const std::string &backup_file_path) const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::ofstream backup_file(backup_file_path);
        if (!backup_file.is_open())
        {
            std::cerr << "Error in backup_to_file: Failed to open backup file for writing: " << backup_file_path << std::endl;
            return;
        }
        try
        {
            nlohmann::json j(config_map);
            backup_file << j.dump(4);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error in backup_to_file: " << e.what() << std::endl;
        }
    }

} // namespace config

#endif // CONFIGURATION_HPP
