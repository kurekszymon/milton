#include <array>  // for array
#include <atomic> // for atomic
#include <chrono> // for operator""s, chrono_literals
#include <iostream>
#include <string> // for string, basic_string, char_traits, operator+, to_string
#include <thread> // for sleep_for, thread
#include <vector> // for vector

#include "ftxui/component/component.hpp"          // for Checkbox, Renderer, Horizontal, Vertical, Input, Menu, Radiobox, ResizableSplitLeft, Tab
#include "ftxui/component/component_base.hpp"     // for ComponentBase, Component
#include "ftxui/component/component_options.hpp"  // for MenuOption, InputOption
#include "ftxui/component/event.hpp"              // for Event, Event::Custom
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"                 // for text, color, operator|, bgcolor, filler, Element, vbox, size, hbox, separator, flex, window, graph, EQUAL, paragraph, WIDTH, hcenter, Elements, bold, vscroll_indicator, HEIGHT, flexbox, hflow, border, frame, flex_grow, gauge, paragraphAlignCenter, paragraphAlignJustify, paragraphAlignLeft, paragraphAlignRight, dim, spinner, LESS_THAN, center, yframe, GREATER_THAN

#include "Config.hpp"
#include "Console.hpp"

using namespace ftxui;

int main()
{
    auto config = new Config("config.yaml");
    auto screen = ScreenInteractive::Fullscreen();

    int shift = 0;
    int tab_index = 0;
    std::vector<std::string> tab_entries = {};

    auto tab_content = Container::Tab(
        {},
        &tab_index);

    // ---------------------------------------------------------------------------
    // Custom Scripts
    // ---------------------------------------------------------------------------

    auto custom_scripts_config = config->custom_scripts;
    std::string cs_console_output;
    int cs_selected_index = 0;

    std::vector<std::string> cs_entries;
    for (const auto &script : custom_scripts_config)
    {
        cs_entries.push_back(script.name);
    }

    auto cs_option = MenuOption::VerticalAnimated();

    cs_option.on_enter = [&]()
    {
        const CustomScript &selected_script = custom_scripts_config.at(cs_selected_index);

        std::string output = Console::execute_custom_command(selected_script, cs_console_output);
        cs_console_output = output;
    };

    auto cs_menu = Menu(&cs_entries, &cs_selected_index, cs_option);

    auto cs_render_console_output = [&cs_console_output]()
    {
        return hbox({text("console output: "), text(cs_console_output)});
    };
    auto cs_renderer = Renderer([cs_render_console_output]
                                { return vbox({cs_render_console_output()}) | frame; });

    auto cs_container = Container::Horizontal({cs_menu, cs_renderer});

    if (config->is_item_loaded(ConfigItem::CUSTOM_SCRIPTS))
    {
        tab_entries.push_back("custom scripts");
        tab_content->Add(cs_container);
    }
    // ---------------------------------------------------------------------------
    // Repositories
    // ---------------------------------------------------------------------------

    auto repo_config = config->repositories;
    std::string repositories_console_output;
    int repositories_selected_index = 0;

    std::vector<std::string> repositories_entries;
    for (const auto &repo : repo_config.vector)
    {
        repositories_entries.push_back(repo.name);
    }

    auto repositories_option = MenuOption::VerticalAnimated();
    repositories_option.on_enter = [&]()
    {
        const Repository &selected_repo = repo_config.vector.at(repositories_selected_index);
        const std::string clone_to = repo_config.clone_path + '/' + selected_repo.name;

        std::string output = Console::execute_git_clone(selected_repo.url, clone_to);
        repositories_console_output = output;
    };

    auto repos_menu = Menu(&repositories_entries, &repositories_selected_index, repositories_option);

    auto render_repositories_console_output = [&repositories_console_output]()
    {
        return hbox({text("console output: "), text(repositories_console_output)});
    };
    auto repositories_renderer = Renderer([render_repositories_console_output]
                                          { return vbox({render_repositories_console_output()}) | frame; });

    auto repositories_container = Container::Horizontal({repos_menu, repositories_renderer});

    if (config->is_item_loaded(ConfigItem::REPOSITORIES))
    {
        tab_entries.push_back("repositories");
        tab_content->Add(repositories_container);
    }

    // ---------------------------------------------------------------------------
    // Main render
    // ---------------------------------------------------------------------------

    auto tab_selection =
        Menu(&tab_entries, &tab_index, MenuOption::HorizontalAnimated());

    auto exit_button = Button(
        "Exit", [&]
        { screen.Exit(); }, ButtonOption::Animated());

    auto main_container = Container::Vertical({
        Container::Horizontal({
            tab_selection,
            exit_button,
        }),
        tab_content,
    });

    auto main_renderer = Renderer(main_container, [&]
                                  { return vbox({
                                        text("Milton.sh") | bold | hcenter,
                                        hbox({
                                            tab_selection->Render() | flex,
                                            exit_button->Render(),
                                        }),
                                        tab_content->Render() | flex,
                                    }); });

    std::atomic<bool> refresh_ui_continue = true;
    std::thread refresh_ui([&]
                           {
    while (refresh_ui_continue) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(0.05s);
      // The |shift| variable belong to the main thread. `screen.Post(task)`
      // will execute the update on the thread where |screen| lives (e.g. the
      // main thread). Using `screen.Post(task)` is threadsafe.
      screen.Post([&] { shift++; });
      // After updating the state, request a new frame to be drawn. This is done
      // by simulating a new "custom" event to be handled.
      screen.Post(Event::Custom);
    } });

    screen.Loop(main_renderer);
    refresh_ui_continue = false;
    refresh_ui.join();

    return 0;
}