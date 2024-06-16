// File: test_configuration_no_format_manager.cpp
/*
    * External Dependencies:
    * - nlohmann/json
    * - yaml-cpp
    *
    * (c) 2024, Benjamin Gorlick | github.com/bgorlick/config_manager/
    * Distributed under the Boost Software License, Version 1.0.
    * (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
    * 
    * 
    * This file is a simplified version of the test_configuration.cpp file.
    * It tests the Config class API functions without the FormatManager class.
    * It does not use the FormatManager class to handle different output formats.
    * A quick way of testing the raw Config class API functions without the the FormatManager class.
    *    
*/


#include <iostream>
#include "../include/configuration.hpp"

void test_configuration_no_format_manager() {
    // Create a Config instance
    config::Config& config = config::Config::instance("test_instance");

    // Set some configuration values
    config.set("key1", "value1");
    config.set("key2", 123);
    config.set("key3", true);

    // Get and print configuration values
    std::cout << "key1: " << config.get("key1") << std::endl;
    std::cout << "key2: " << config.get("key2") << std::endl;
    std::cout << "key3: " << config.get("key3") << std::endl;

    // Check if a key exists
    std::cout << "key1 exists: " << config.exists("key1") << std::endl;
    std::cout << "key4 exists: " << config.exists("key4") << std::endl;

    // Remove a key and check if it still exists
    config.remove("key1");
    std::cout << "key1 exists after removal: " << config.exists("key1") << std::endl;

    // Clear all configurations
    config.clear();
    std::cout << "key2 exists after clear: " << config.exists("key2") << std::endl;
}

int main() {
    test_configuration_no_format_manager();
    return 0;
}
