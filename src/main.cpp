#include <iostream>
#include <list>
#include <numeric>
#include <boost/program_options.hpp>
#include "./config.hpp"
#include "./core.hpp"

int main(int argc, char *argv[])
{
    namespace po = boost::program_options;

    po::options_description optionsDescription("Allowed options");
    optionsDescription.add_options()("help,h", "Show help message")               // help option
        ("version,v", "Show version information")                                 // version option
        ("input", po::value<std::vector<std::string>>(), "File path to process"); // input option

    po::positional_options_description positionalOptionsDescription;
    positionalOptionsDescription.add("input", -1);

    po::variables_map variablesMap;
    try
    {
        po::store(po::command_line_parser(argc, argv)
                      .options(optionsDescription)
                      .positional(positionalOptionsDescription)
                      .run(),
                  variablesMap);
        po::notify(variablesMap);
    }
    catch (std::exception &e)
    {
        throw std::runtime_error("bad options: " + std::string(e.what()));
    }

    if (variablesMap.count("help"))
    {
        std::cout << optionsDescription << std::endl;
    }
    else if (variablesMap.count("version"))
    {
        std::cout << DISPLAY_NAME << " " << PROJECT_VERSION << std::endl;
    }
    else if (variablesMap.count("input"))
    {
        std::vector<std::string> arguments = variablesMap["input"].as<std::vector<std::string>>();
        Core core;
        core.start(arguments);
    }
    else
    {
        std::cout << optionsDescription << std::endl;
    }

    return 0;
}
