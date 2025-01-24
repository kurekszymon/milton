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
    // TODO: fix to check seperately and tell user which modules were loaded
    if (config["repositories"] && config["custom_scripts"])
    {
        auto repos_node = config["repositories"];
        if (repos_node["clone_path"])
        {
            repositories.clone_path = repos_node["clone_path"].as<std::string>();
        }

        for (const auto &repo : repos_node["repos"])
        {
            Repository r;
            auto _repo = repo.begin();
            r.name = _repo->first.as<std::string>();
            r.url = _repo->second.as<std::string>();
            repositories.vector.push_back(r);
        }
        auto custom_scripts_node = config["custom_scripts"];

        std::cout << ":hello:" << custom_scripts_node.IsDefined();
        for (const auto &script : custom_scripts_node)
        {
            CustomScript s;
            auto _script = script.begin();
            s.name = _script->first.as<std::string>();
            s.cmd = _script->second.as<std::string>();
            custom_scripts.push_back(s);
        }

        return true;
    }
    else
    {
        std::cerr << "Repositories or custom_scripts section missing in YAML!" << std::endl;
        return false;
    }
}