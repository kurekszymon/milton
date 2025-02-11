#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_set>
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

enum MItem
{
    REPOSITORIES,
    CUSTOM_SCRIPTS,
};

class Config
{
public:
    std::unordered_set<MItem> loaded_items;

    Repositories repositories;
    std::vector<CustomScript> custom_scripts;

    Config(const std::string &yaml_file);

    bool is_item_loaded(MItem item);

private:
    void load_yaml_config(const std::string &yaml_file);
    void load_item(MItem item);
};

#endif // CONFIG_H
