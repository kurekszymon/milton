#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "boost/process.hpp"
#include "boost/filesystem.hpp"

#include "Config.hpp"
namespace bp = boost::process;
namespace bfs = boost::filesystem;

class Console
{
public:
    inline static std::string parse_home_dir(std::string input_path)
    {
        if (input_path.empty() || input_path[0] != '~')
        {
            return input_path;
        }

        const char *env_home = std::getenv("HOME");               // Unix-like systems (Linux/macOS)
        const char *env_userprofile = std::getenv("USERPROFILE"); // Windows

        bfs::path home_dir = env_home ? env_home : env_userprofile;

        // handle home not found - decide for try catch / throwing

        bfs::path result = home_dir / input_path.substr(1);
        return result.string();
    };
    inline static std::string execute_custom_command(CustomScript command, std::string messages)
    {
        try
        {
            std::vector<std::string> args = {"-c", command.cmd};

            bp::child c(bp::search_path("sh"), args);

            c.wait();

            if (c.exit_code() == 0)
            {
                return "Command executed successfully.";
            }

            else
            {
                std::string message = command.name + " failed with code: " + std::to_string(c.exit_code());
                return message;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error during custom command execution: " << e.what() << "\n";
            std::string error_message = e.what();
            return error_message;
        }
    }
    inline static std::string execute_git_clone(const std::string &repo_url, const std::string &clone_dir)
    {
        try
        {
            std::vector<std::string> args = {"clone", repo_url, parse_home_dir(clone_dir)};

            bp::child c(bp::search_path("git"), args);

            c.wait();

            if (c.exit_code() == 0)
            {
                std::cout << "Git clone successful!\n";
                return "success! ";
            }
            if (c.exit_code() == 128)
            {
                std::cout << "Failed to clone repo, repo exists.";
                return "Repository exists.";
            }
            else
            {
                std::string message = "Git clone failed with exit code: " + std::to_string(c.exit_code());
                return message;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error during git clone: " << e.what() << "\n";
            std::string error_message = e.what();
            return error_message;
        }
    };
};

#endif // CONSOLE_H
