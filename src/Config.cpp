#include "../include/Config.hpp"
#include <iostream>
#include <yaml-cpp/yaml.h>

Config::Config(const std::string &yaml_file)
{
    try
    {
        if (!load_yaml_config(yaml_file))
        {
            throw std::runtime_error("Failed to load config file.");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE); // Optionally exit if exception is caught
    }
}

bool Config::load_yaml_config(const std::string &yaml_file)
{
    YAML::Node config = YAML::LoadFile(yaml_file);
    if (config["repositories"])
    {
        auto repos_node = config["repositories"];
        if (repos_node["clone_path"])
        {
            repositories.clone_path = repos_node["clone_path"].as<std::string>();
        }

        for (const auto &repo : repos_node["repos"])
        {
            // The `repo` is a map (YAML::Node) with one key-value pair (name: URL)
            // Directly access the first key-value pair in the map:
            Repository r;
            r.name = repo.begin()->first.as<std::string>(); // Get the repository name (key)
            r.url = repo.begin()->second.as<std::string>(); // Get the repository URL (value)
            repositories.vector.push_back(r);
        }

        return true;
    }
    else
    {
        std::cerr << "Repositories section missing in YAML!" << std::endl;
        return false;
    }
}