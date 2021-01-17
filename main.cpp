#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Pentominoes.hpp"
#include "Pyramid.hpp"
#include "Queens.hpp"

#include "boost/any.hpp"
#include "boost/program_options.hpp"

using namespace std;
namespace po = boost::program_options;


void validate(
    boost::any& v,
    const std::vector<std::string>& values,
    Pentominoes** target_type, int)
{
    static map<string, function<Pentominoes *()>> factories = {
        { "6x10", [](){ return new Pentominoes6x10(); }},
        { "5x12", [](){ return new Pentominoes5x12(); }},
        { "4x15", [](){ return new Pentominoes4x15(); }},
        { "3x20", [](){ return new Pentominoes3x20(); }}
    };

    po::validators::check_first_occurrence(v);
    auto s = po::validators::get_single_string(values);

    auto it = factories.find(s);
    if (it != factories.end()) {
        v = boost::any(it->second());
    } else {
        throw po::validation_error(po::validation_error::invalid_option_value);
    }
}


void checkGreaterZero(int value)
{
   if (value <= 0) {
       throw po::validation_error(po::validation_error::invalid_option_value);
   }
}


int main(int argc, const char *argv[])
{
    po::options_description opts;

    po::options_description puzzleOpts("Puzzle options (exactly one required)");
    puzzleOpts.add_options()
        ("pentominoes", po::value<Pentominoes *>(), 
            "pentominoes mode (one of: 6x10, 5x12, 4x15, or 3x20)")
        ("pyramid", po::bool_switch(),
            "3d pyramid puzzle")
        ("queens", po::value<int>()->notifier(checkGreaterZero), 
            "n-queens mode (number of queens, greater than zero)");

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

        if (vm.count("pentominoes") + vm.count("queens") + (vm["pyramid"].defaulted() ? 0 : 1) != 1) {
            throw po::error_with_no_option_name("must specify exactly one puzzle option");
        }

        if (vm.count("pentominoes")) {
            unique_ptr<Pentominoes> puzzle(vm["pentominoes"].as<Pentominoes *>());
            puzzle->solve(vm["count"].as<bool>());
        } else if (vm.count("queens")) {
            auto puzzle = make_unique<Queens>(vm["queens"].as<int>());
            puzzle->solve(vm["count"].as<bool>());
        } else if (vm["pyramid"].as<bool>()) {
            auto puzzle = make_unique<Pyramid>();
            puzzle->solve(vm["count"].as<bool>());
        }

        return 0;
    }

    catch(po::error const& e) {
        cerr << e.what() << endl;
        usage();
        return 1;
    }
}
