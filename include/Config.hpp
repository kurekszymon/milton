#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

struct Repository
{
    std::string name;
    std::string url;
};

struct CustomScript
{
    std::string name;
    std::string cmd;
};

struct Repositories
{
    std::string clone_path;
    std::vector<Repository> vector;
};

class Config
{
public:
    Repositories repositories;
    std::vector<CustomScript> custom_scripts;

    Config(const std::string &yaml_file);

private:
    bool load_yaml_config(const std::string &yaml_file);
};

#endif // CONFIG_H
