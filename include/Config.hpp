#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

struct Repository
{
    std::string name;
    std::string url;
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

    Config(const std::string &yaml_file);

private:
    bool load_yaml_config(const std::string &yaml_file);
};

#endif // CONFIG_H
