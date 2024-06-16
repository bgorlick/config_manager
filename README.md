Powerful, Customizable, Header-only C++ library for configurations in JSON, YAML and env vars with configuration listeners and thread safety functionality.

## Why Another Config Manager
Config manager simplifies C++ configuration with high performance and ease of use. Unlike Python's many tools and libs, C++ lacks versatile APIs outside the STL for configuration, and Boost doesn't fully address complex configurations or JSON/YAML interoperable handling. So often, our projects grow from simple to complex with hardcoded settings. 

Config manager enables easy setting, loading, saving, and modifying of configurations, making it ideal for sharing projects. It provides a powerful, flexible, and performant solution for diverse configuration needs, ensuring quick startup and user-friendly operation.

## Simple API Example

```cpp
#include "configuration.hpp"
using namespace config;
// Create a default config instance
Config &config = Config::instance();
// Create a key called 'name' with a value 'example'
config.set("name", "example");
// Retrieve the value of the key 'name'
std::cout << config.get("name") << std::endl;
```
*Expected result:*
`example`

Out-of-the-box features include:

1. **Unified Configuration Handling**: Handles JSON, YAML, and environment 
   variables in one library. 
   - Includes the ability to convert configs between JSON, YAML, and ENV 
     with simple API calls, and output files in any of those formats.
   - (Hint: See format_manager.hpp in the include/ directory)

2. **Header-Only Implementation**: Easy integration with no extra compilation.

3. **Comprehensive API**: Functions for loading, saving, managing configurations, 
   adding listeners, and handling various data types.

4. **Factory Design Pattern**: Simplifies creating and managing instances, 
   supporting defaults, files, and environment-specific settings.

5. **Thread-Safe and Pooled Config Instances**: Essential for modern apps, offering 
   thread-safe configurations and instance pooling for better performance.

6. **Minimal External Dependencies**: Relies only on nlohmann/json and yaml-cpp, 
   ensuring lightweight yet powerful functionality.

7. **Boost Software License**: Permissive license allows free use, modification, and 
   distribution, even in commercial applications.

In short, I built this to offer a flexible, powerful, and user-friendly solution 
for both beginner and experienced C++ community members. Report issues on the repo. 
Contributions and feedback are welcome.

It supports many features including seamless conversion of configs between JSON and YAML, and the ability to access all environmental variables and use them in configurations.

Basic and more advanced features include loading, saving, config listeners, thread safe config creation, and managing configurations from JSON, YAML, and environment variables.


## Key Components
- **IConfigStorage Interface**: Defines the required configuration management functions.
- **Config Class**: Implements the IConfigStorage interface and provides configuration management functionality.
- **ConfigFactory Class**: Provides factory methods to create and manage Config instances.
- **Templates**: Handle different data types and custom format functions.

## External Dependencies
- nlohmann/json
- yaml-cpp

## Building this repo for Testing

This repo includes a unit test in tests/ which you can build with both a `CMakeLists.txt` and a `Makefile`, or directly with GCC/CLang.

## Using Make

To build the `test_configuration` in the `tests/` directory, run:

`make`

## Using CMake

To build with CMake, follow these steps:

```bash
mkdir -p build
cd build
cmake ..
make
```

## Using GCC

To build with GCC for C++20:

`g++ -std=c++20 -o test_configuration tests/test_configuration.cpp -lyaml-cpp -ljsoncpp -lc -Wall -Wextra -Wpedantic`

## Using Clang

To build with Clang for C++20:

`clang++ -std=c++20 -o test_configuration tests/test_configuration.cpp -lyaml-cpp -ljsoncpp -lc -Wall -Wextra -Wpedantic`




## Key Classes and Functions

### IConfigStorage Interface
Defines the required configuration management functions.
```cpp
virtual nlohmann::json get(const std::string &key) const = 0
```
Gets the value for a given key.

```cpp
virtual void set(const std::string &key, const nlohmann::json &value) = 0
```
Sets the value for a given key.

```cpp
virtual std::unordered_map<std::string, nlohmann::json> get_all() const = 0
```
Gets all key-value pairs.

```cpp
virtual void load_from_file(const std::string &file_path) = 0
```
Loads configuration from a file.

```cpp
virtual void save_to_file(const std::string &file_path) const = 0
```
Saves configuration to a file.

```cpp
virtual void remove(const std::string &key) = 0
```
Removes a key-value pair.

```cpp
virtual bool exists(const std::string &key) const = 0
```
Checks if a key exists.

```cpp
virtual void clear() = 0
```
Clears all configurations.

```cpp
virtual void load_from_file(const std::string &file_path, const std::string &version) = 0
```
Loads configuration from a file with a specific version.

```cpp
virtual void save_to_file(const std::string &file_path, const std::string &version) const = 0
```
Saves configuration to a file with a specific version.

```cpp
virtual void load_from_env() = 0
```
Loads configuration from environment variables.

```cpp
virtual void add_change_listener(const std::function<void(const std::string &, const nlohmann::json &)> &listener) = 0
```
Adds a change listener.

```cpp
virtual void backup_to_file(const std::string &backup_file_path) const = 0
```
Backs up the configuration to a file.

```cpp
template<typename T = void> typename std::enable_if<std::is_same<T, std::ostream&>::value, std::ostream&>::type output_config(std::ostream &os) const
```
Outputs the configuration to an ostream.

```cpp
template<typename T = void> typename std::enable_if<std::is_same<T, void>::value, void>::type output_config(std::ostream &os) const
```
Outputs the configuration to an ostream (void specialization).

### Config Class
Implements the IConfigStorage interface and provides configuration management functionality.
- Functions: Same as IConfigStorage interface, with additional functions for instance management.

### ConfigFactory Class
Provides factory methods to create and manage Config instances.
```cpp
static std::shared_ptr<Config> create_config(const std::string &name = "default")
```
Creates a basic config instance.

```cpp
static std::shared_ptr<Config> create_new_config_from_existing(const std::string &name, const std::string &filePath)
```
Creates a new config instance from an existing configuration file.

```cpp
static std::shared_ptr<Config> create_config_with_defaults(const std::string &name, const std::unordered_map<std::string, nlohmann::json> &defaults)
```
Creates a config instance with default values.

```cpp
static std::shared_ptr<Config> create_env_config(const std::string &name, const std::string &environment)
```
Creates a config instance for a specific environment.

```cpp
static std::shared_ptr<Config> create_env_loaded_config(const std::string &name)
```
Creates a config instance and loads it from environment variables.

```cpp
static std::shared_ptr<Config> create_thread_safe_config(const std::string &name = "default")
```
Thread-safe method to create or get a config instance.

```cpp
static std::shared_ptr<Config> get_pooled_config(const std::string &name = "default")
```
Pools and reuses instances.

### Template Functions
Handle different data types and custom format functions.

## Usage Examples

```cpp
#include "configuration.hpp"

using namespace config;

// Example: Create a default config instance
Config &config = Config::instance();
config.set("name", "example");
std::cout << config.get("name") << std::endl;
```
*Expected result:*
```
example
```

```cpp
// Example: Load configuration from JSON file
config.load_from_file("config.json");
std::cout << config.get("name") << std::endl;
```
*Expected result:*
```
example
```

```cpp
// Example: Save configuration to YAML file
config.save_to_file("config.yaml", "1.0.0");
```

```cpp
// Example: Add change listener
bool listener_called = false;
config.add_change_listener([&listener_called](const std::string &key, const nlohmann::json &value) {
    if (key == "name" && value == "example_listener") {
        listener_called = true;
    }
});
config.set("name", "example_listener");
```
*Expected result:*
```
Listener is called, listener_called is true
```

```cpp
// Example: Load configuration from environment variables
setenv("name", "env_example", 1);
config.load_from_env();
std::cout << config.get("name") << std::endl;
```
*Expected result:*
```
env_example
```

```cpp
// Example: Backup configuration to file
config.backup_to_file("backup.json");
```

```cpp
// Example: Validate configuration
std::unordered_map<std::string, std::function<bool(const nlohmann::json &)>> validators = {
    {"name", [](const nlohmann::json &value) { return value.is_string(); }},
    {"age", [](const nlohmann::json &value) { return value.is_number_integer() && value > 0; }}
};
config.validate(validators);
```

```cpp
// Example: Inspect configuration keys
std::vector<nlohmann::json> inspected = config.inspect({"name", "complex"});
for (const auto &value : inspected) {
    std::cout << value.dump(4) << std::endl;
}
```
*Expected result:*
```
"example"
{
    "key1": "value1",
    "key2": 42,
    "key3": {
        "nestedKey": "nestedValue"
    }
}
```

```cpp
// Example: Update multiple configuration values
std::unordered_map<std::string, nlohmann::json> new_cfg = {
    {"name", "new_example"},
    {"age", 30}
};
config.update_multiple(new_cfg);
std::cout << config.get("name") << std::endl;
std::cout << config.get("age") << std::endl;
```
*Expected result:*
```
new_example
30
```

```cpp
// Example: Partial save and load (JSON)
config.save_partial_to_file("partial_config.json", {"complex"});
config.clear();
config.load_partial_from_file("partial_config.json", {"complex"});
std::cout << config.get("complex").dump(4) << std::endl;
```
*Expected result:*
```
{
    "key1": "value1",
    "key2": 42,
    "key3": {
        "nestedKey": "nestedValue"
    }
}
```

```cpp
// Example: Output configuration in different formats
set_output_format(OutputFormat::JSON);
config.output_config<std::ostream&>(std::cout) << std::endl;

set_output_format(OutputFormat::YAML);
config.output_config<void>(std::cout);
```
*Expected result:*
```
{
    "complex": {
        "key1": "value1",
        "key2": 42,
        "key3": {
            "nestedKey": "nestedValue"
        }
    }
}
```
```
complex:
  key1: value1
  key2: 42
  key3:
    nestedKey: value
```

## Copyright
(c) 2024, Benjamin Gorlick | [github.com/bgorlick/config_manager](https://github.com/bgorlick/config_manager)

## License
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
