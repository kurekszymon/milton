#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <variant>
#include <vector>

enum class CPP
{
    emscripten,
    cmake,
    ninja
};

enum class Python
{
    CPYTHON,
    PYTHON3
};

enum class JavaScript
{
    NODEJS,
    V8
};

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

struct Language
{
    std::string name;
    std::string download_url;
};

struct Repositories
{
    std::string clone_path;
    std::vector<Repository> vector;
};

enum ConfigItem
{
    REPOSITORIES,
    CUSTOM_SCRIPTS,
};
using MLanguage = std::variant<CPP, Python, JavaScript>;

using MItem = std::variant<ConfigItem, MLanguage>;
class Config
{
public:
    std::unordered_set<MItem> loaded_items;

    Repositories repositories;
    std::vector<CustomScript> custom_scripts;

    Config(const std::string &yaml_file);

    bool is_item_loaded(MItem item); // Generic function to check if any tool from a specific language is loaded
    // Language Enums

    std::unordered_map<std::string, MLanguage> cpp_mapper = {
        {"emscripten", CPP::emscripten},
        {"cmake", CPP::cmake},
        {"ninja", CPP::ninja}};
    std::unordered_map<std::string, MLanguage> python_mapper = {
        {"python3", Python::PYTHON3},
        {"cpython", Python::CPYTHON}};
    std::unordered_map<std::string, MLanguage> js_mapper = {
        {"node", JavaScript::NODEJS},
        {"v8", JavaScript::V8}};

    std::unordered_map<std::string, std::unordered_map<std::string, MLanguage>> language_mapper = {
        {"cpp", cpp_mapper},
        {"python", python_mapper},
        {"js", js_mapper}};

    template <typename T>
    struct LanguageTools;

    template <>
    struct LanguageTools<CPP>
    {
        static constexpr std::array<CPP, 3> values = {
            CPP::emscripten,
            CPP::cmake,
            CPP::ninja};
    };

    template <>
    struct LanguageTools<Python>
    {
        static constexpr std::array<Python, 2> values = {
            Python::CPYTHON,
            Python::PYTHON3};
    };

    template <>
    struct LanguageTools<JavaScript>
    {
        static constexpr std::array<JavaScript, 2> values = {
            JavaScript::NODEJS,
            JavaScript::V8};
    };
    template <typename T>
    bool is_any_tool_from_language_loaded()
    {
        const auto &enum_values = LanguageTools<T>::values;
        std::cout << "Enum values for " << typeid(T).name() << ": ";

        for (const auto &value : enum_values)
        {
            std::cout << static_cast<int>(value) << "value.. ";
            return is_item_loaded(value);
        }

        return false;
    }

private:
    void load_yaml_config(const std::string &yaml_file);
    void load_item(MItem item);
};

#endif // CONFIG_H
