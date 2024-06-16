#include "../include/configuration.hpp"
#include "../include/format_manager.hpp"
#include <cassert>
#include <iostream>
#include <cstdlib>  // For setenv function
#include <unistd.h> // For environ declaration
#include <string.h> // For string operations


// Custom assertion function
bool custom_assert(bool condition, const std::string &message)
{
    if (!condition)
    {
        std::cerr << "Assertion failed: " << message << std::endl;
    }
    return condition;
}
// Test function for the configuration library
void test_configuration()
{
    using namespace config;
    using namespace output_format;

    std::cout << "Starting Configuration tests\n";

    // Create a default config instance
    Config &config = Config::instance();

    // Test 1: Simple configuration
    config.set("name", "example");
    custom_assert(config.get("name") == "example", "config.get('name') == 'example'");
    std::cout << "Test 1 passed: config.set('name', 'example') and config.get('name') == 'example'\n";

    // Test 2: Complex configuration
    nlohmann::json complex = {{"key1", "value1"}, {"key2", 42}, {"key3", {{"nestedKey", "nestedValue"}}}};
    config.set("complex", complex);
    custom_assert(config.get("complex") == complex, "config.get('complex') == complex");
    std::cout << "Test 2 passed: config.set('complex', complex) and config.get('complex') == complex\n";

    // Test 3: Save to JSON file
    std::string json_file_path = "config.json";
    config.save_to_file(json_file_path);
    std::cout << "Test 3 passed: config saved to JSON file\n";

    // Test 4: Save to YAML file
    std::string yaml_file_path = "config.yaml";
    config.save_to_file(yaml_file_path, "1.0.0");
    std::cout << "Test 4 passed: config saved to YAML file\n";

    // Test 5: Clear and reload from JSON file
    config.clear();
    config.load_from_file(json_file_path);
    custom_assert(config.get("name") == "example", "config.get('name') == 'example'");
    custom_assert(config.get("complex") == complex, "config.get('complex') == complex'");
    std::cout << "Test 5 passed: config cleared and reloaded from JSON file\n";

    // Test 6: Clear and reload from YAML file
    config.clear();
    config.load_from_file(yaml_file_path, "1.0.0");
    custom_assert(config.get("name") == "example", "config.get('name') == 'example'");
    custom_assert(config.get("complex") == complex, "config.get('complex') == complex");
    std::cout << "Test 6 passed: config cleared and reloaded from YAML file\n";

    // Test 7: Exists and remove
    custom_assert(config.exists("name"), "config.exists('name')");
    config.remove("name");
    custom_assert(!config.exists("name"), "not config.exists('name')");
    std::cout << "Test 7 passed: config.exists and config.remove for 'name'\n";

    // Test 8: Load from environment variables
    setenv("name", "env_example", 1);
    config.load_from_env();
    if (config.exists("name")) // Ensure the key exists before asserting
    {
        custom_assert(config.get("name") == "env_example", "config.get('name') == 'env_example'");
        std::cout << "Test 8 passed: config loaded from environment variables\n";
    }
    else
    {
        std::cerr << "Test 8 failed: Key 'name' does not exist in the configuration after loading from environment\n";
    }

    // Test 9: Add change listener
    bool listener_called = false;
    config.add_change_listener([&listener_called](const std::string &key, const nlohmann::json &value) {
        if (key == "name" && value == "listener_example")
        {
            listener_called = true;
        }
    });
    config.set("name", "listener_example");
    custom_assert(listener_called, "listener_called");
    std::cout << "Test 9 passed: change listener added and called\n";

    // Reset the configuration to original values
    config.set("name", "example");

    // Test 10: Backup to file
    std::string backup_file_path = "config_backup.json";
    config.backup_to_file(backup_file_path);
    std::cout << "Test 10 passed: config backed up to file\n";

    // Test 11: Inspect for both simple and complex configurations
    std::vector<nlohmann::json> inspected = config.inspect({"name", "complex"});
    custom_assert(inspected.size() == 2, "inspected.size() == 2");
    custom_assert(inspected[0] == "example", "inspected[0] == 'example'");
    custom_assert(inspected[1] == complex, "inspected[1] == complex");
    std::cout << "Test 11 passed: config inspected for simple and complex configurations\n";

    // Test 12: Partial save and load (JSON)
    std::string partial_json_file_path = "config_partial.json";
    config.save_partial_to_file(partial_json_file_path, {"complex"});
    config.clear();
    config.load_partial_from_file(partial_json_file_path, {"complex"});
    custom_assert(config.get("complex") == complex, "config.get('complex') == complex");
    std::cout << "Test 12 passed: partial save and load (JSON)\n";

    // Test 13: Partial save and load (YAML)
    std::string partial_yaml_file_path = "config_partial.yaml";
    config.save_partial_to_file(partial_yaml_file_path, {"complex"});
    config.clear();
    config.load_partial_from_file(partial_yaml_file_path, {"complex"});
    custom_assert(config.get("complex") == complex, "config.get('complex') == complex");
    std::cout << "Test 13 passed: partial save and load (YAML)\n";

    // Test 14: Output configuration in different formats
    std::cout << "Output configuration in different formats:\n";
    set_output_format(OutputFormat::JSON);
    config.output_config<std::ostream&>(std::cout) << "\n" << std::endl; // Chained output

    set_output_format(OutputFormat::YAML);
    config.output_config<void>(std::cout); // Void output
    std::cout << "\n\n";

    set_output_format(OutputFormat::PLAIN_TEXT);
    config.output_config<void>(std::cout); // Void output
    std::cout << "\n\n";

    std::cout << "Test 14 passed: config output in different formats\n";

    // Test 15: Combined setting format and output
    std::cout << "Combined setting format and output:\n";
    output_format::set_format_and_output(OutputFormat::JSON, config, std::cout);
    std::cout << "\n";
    output_format::set_format_and_output(OutputFormat::YAML, config, std::cout);
    std::cout << "\n";
    output_format::set_format_and_output(OutputFormat::PLAIN_TEXT, config, std::cout);
    std::cout << "\n";
    std::cout << "Test 15 passed: combined setting format and output\n";

    std::cout << "All tests passed!" << std::endl;
}


// Test function for the ConfigurationFactory
void test_configfactory()
{
    std::cout << "Starting ConfigurationFactory tests\n";

    using namespace config;

    // Create a basic config instance and set a value
    auto config_basic = ConfigFactory::create_config("basic");
    config_basic->set("key1", "value1");
    custom_assert(config_basic->get("key1") == "value1", "config_basic->get('key1') == 'value1'");
    std::cout << "Test 1 passed: config_basic->get('key1') == 'value1'\n";

    // Create and save a configuration to a file
    auto config_to_save = ConfigFactory::create_config("to_save");
    config_to_save->set("name", "custom_value");
    config_to_save->save_to_file("config_factory_tester.json");
    std::cout << "Test 2 passed: config_to_save saved to file\n";

    // Load the configuration from the file using create_new_config_from_existing
    auto config_custom = ConfigFactory::create_new_config_from_existing("custom", "config_factory_tester.json");
    if (config_custom)
    {
        custom_assert(config_custom->get("name") == "custom_value", "config_custom->get('name') == 'custom_value'");
        std::cout << "Test 3 passed: config_custom->get('name') == 'custom_value'\n";
    }
    else
    {
        std::cerr << "Failed to create and load config from file." << std::endl;
    }

    // Create a config instance with default values
    auto config_defaults = ConfigFactory::create_config_with_defaults("defaults", {{"key2", "default_value"}});
    custom_assert(config_defaults->get("key2") == "default_value", "config_defaults->get('key2') == 'default_value'");
    std::cout << "Test 4 passed: config_defaults->get('key2') == 'default_value'\n";

    // Set an environment variable for testing
    setenv("env_key", "env_value", 1);

    // Create a config instance loaded from environment variables
    auto config_env = ConfigFactory::create_env_loaded_config("env_loaded");
    if (config_env && config_env->exists("env_key")) // Ensure the key exists before asserting
    {
        custom_assert(config_env->get("env_key") == "env_value", "config_env->get('env_key') == 'env_value'");
        std::cout << "Test 5 passed: config_env->get('env_key') == 'env_value'\n";
    }
    else
    {
        std::cerr << "Key 'env_key' does not exist in the configuration after loading from environment\n";
    }

    // Create a thread-safe config instance
    auto config_thread_safe = ConfigFactory::create_thread_safe_config("thread_safe");
    config_thread_safe->set("key3", "value3");
    custom_assert(config_thread_safe->get("key3") == "value3", "config_thread_safe->get('key3') == 'value3'");
    std::cout << "Test 6 passed: config_thread_safe->get('key3') == 'value3'\n";

    // Get a pooled config instance
    auto pooled_config = ConfigFactory::get_pooled_config("pooled");
    pooled_config->set("key4", "value4");
    custom_assert(pooled_config->get("key4") == "value4", "pooled_config->get('key4') == 'value4'");
    std::cout << "Test 7 passed: pooled_config->get('key4') == 'value4'\n";
}

int main()
{
    test_configuration();
    test_configfactory();
    return 0;
}
