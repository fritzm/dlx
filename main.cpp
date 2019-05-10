#include <iostream>

#include "polyos.hpp"

#include "boost/program_options.hpp"

using namespace std;
namespace po = boost::program_options;


std::istream& operator>>(std::istream& in, p5BoardType& boardType)
{
    static map<string, p5BoardType> tokens = {
        { "6x10", p5BoardType::P5_6X10 },
        { "5x12", p5BoardType::P5_5X12 },
        { "4x15", p5BoardType::P5_4X15 },
        { "3x20", p5BoardType::P5_3X20 }
    };

    std::string token;
    in >> token;

    auto it = tokens.find(token);
    if (it != tokens.end()) {
        boardType = it->second;
    } else {
        in.setstate(std::ios_base::failbit);
    }

    return in;
}


int main(int argc, const char *argv[])
{
    po::options_description opts;

    po::options_description puzzleOpts("Puzzle options (exactly one required)");
    puzzleOpts.add_options()
        ("p5", po::value<p5BoardType>(), "pentominoes mode (one of: 6x10, 5x12, 4x20, or 3x15)");

    po::options_description generalOpts("General options");
    generalOpts.add_options()
        ("count", po::bool_switch(), "just print count of solutions")
        ("help,h", "produce help message");

    opts.add(puzzleOpts).add(generalOpts);

    auto usage = [&opts](){
        cout << opts << endl;
    };

    try {

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, opts), vm);
        po::notify(vm);    

        if ((argc == 1) || vm.count("help")) {
            cout << "Exact cover puzzle solver using Don Knuth \"Dancing Links\" algorithm" << endl;
            cout << "(see https://arxiv.org/abs/cs/0011047)" << endl;
            usage();
            return 1;
        }

        if (vm.count("p5") != 1) {
            throw po::error_with_no_option_name("must specify exactly one puzzle option");
        }

        if (vm.count("p5")) {
            pentominoes(vm["p5"].as<p5BoardType>(), vm["count"].as<bool>());
        }

        return 0;
    }

    catch(po::error const& e) {
        cerr << e.what() << endl;
        usage();
        return 1;
    }
}
