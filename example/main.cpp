#include <ftxui/component/event.hpp>
#include <filesystem>
#include <iostream>
#include <vector>
#include <random>

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"

#include "image_view.hpp"

namespace fs = std::filesystem;

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " [picture dir]" << std::endl;
        return 1;
    }

    auto dirpath = fs::directory_iterator(argv[1]);
    std::vector<std::string> pictures;
    for(auto& file : dirpath)
    {
        if(file.is_regular_file() && file.path().extension() == ".png")
        {
            pictures.emplace_back(file.path().native());
        }
    }

    auto cell = [](const std::string& path){ return ftxui::image_view(path) | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 30) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 30); };

    int displayIndexLeft{0};
    int displayIndexRight{1};
    int displayIndexLeft1{2};
    int displayIndexRight1{3};
    auto leftDisplay = ftxui::Renderer([&]{
        return ftxui::vbox(
            cell(pictures[displayIndexLeft]),
            cell(pictures[displayIndexLeft1])
        );
    });

    auto rightDisplay = ftxui::Renderer([&]{
        return ftxui::vbox(
            cell(pictures[displayIndexRight]),
            cell(pictures[displayIndexRight1])
        );
    });

    auto selectButton = ftxui::Button("Click", [&]{
        std::random_device seed;
        std::default_random_engine en(seed());
        std::uniform_int_distribution<> dist(0, pictures.size() - 1);

        displayIndexLeft = dist(en);
        displayIndexRight = dist(en);

        displayIndexLeft1 = dist(en);
        displayIndexRight1 = dist(en);
    });

    auto mainPannelContainer = ftxui::Container::Vertical({
        leftDisplay,
        rightDisplay,
        selectButton
    });

    auto screen = ftxui::ScreenInteractive::FitComponent();

    auto mainPannelRender = ftxui::Renderer(mainPannelContainer, [&]{
        static auto last_frame_time = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();

        auto frame_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_frame_time).count();
        last_frame_time = now;

        screen.PostEvent(ftxui::Event::Custom);

        return ftxui::vbox({
            ftxui::hbox({
                leftDisplay->Render() | ftxui::size(ftxui::WIDTH, ftxui::LESS_THAN, 60) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 30),
                ftxui::separator(),
                rightDisplay->Render() | ftxui::size(ftxui::WIDTH, ftxui::LESS_THAN, 60) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 30),
            }),
            ftxui::separator(),
            ftxui::text("Frame Time: " + std::to_string(frame_time) + " ms"),
            selectButton->Render(),
        });
    });

    screen.Loop(mainPannelRender);
}
