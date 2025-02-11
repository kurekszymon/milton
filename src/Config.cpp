#include "Config.hpp" // find out why it squiggles without ../include - maybe some overwrite?
#include <iostream>
#include <yaml-cpp/yaml.h>

Config::Config(const std::string &yaml_file)
{
    try
    {
        load_yaml_config(yaml_file);

        if (loaded_items.size() == 0)
        {
            throw std::runtime_error("Make sure to provide correct YAML file.");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE); // Optionally exit if exception is caught
    }
}

void Config::load_yaml_config(const std::string &yaml_file)
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
            Repository r;
            auto _repo = repo.begin();
            r.name = _repo->first.as<std::string>();
            r.url = _repo->second.as<std::string>();
            repositories.vector.push_back(r);
        }

        load_item(MItem::REPOSITORIES);
    }
    if (config["custom_scripts"])
    {
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
        load_item(MItem::CUSTOM_SCRIPTS);
    }
}

void Config::load_item(MItem item)
{
    loaded_items.insert(item);
}

bool Config::is_item_loaded(MItem item)
{
    return loaded_items.find(item) != loaded_items.end();
}