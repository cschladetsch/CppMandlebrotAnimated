#include "config.hpp"
#include <boost/program_options.hpp>
#include <iostream>

AppConfig parseCommandLine(int argc, char** argv) {
    namespace po = boost::program_options;

    AppConfig cfg;
    po::options_description desc("Options");
    desc.add_options()
        ("help", "Show help")
        ("width", po::value<int>(&cfg.width)->default_value(800), "Window width")
        ("height", po::value<int>(&cfg.height)->default_value(600), "Window height")
        ("scale", po::value<float>(&cfg.scale)->default_value(2.5f), "Initial zoom scale")
        ("center-x", po::value<float>(&cfg.center.x)->default_value(-0.75f), "Center X")
        ("center-y", po::value<float>(&cfg.center.y)->default_value(0.0f), "Center Y");

    po::variables_map vm;
    store(po::parse_command_line(argc, argv, desc), vm);
    notify(vm);
    if (vm.count("help")) {
        std::cout << desc << "\n";
        std::exit(0);
    }
    
    return cfg;
}
