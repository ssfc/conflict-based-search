//
// Created by take_ on 2024/1/22.
//
// A c++ implementation for conflict-based search.

#include "high_level.hpp"

#include <boost/program_options.hpp>


using namespace std;


// Driver program to test above function
int main(int argc, char *argv[])
{
    namespace po = boost::program_options;
    // Declare the supported options.
    po::options_description desc("Allowed options");

    string input_filename;
    string output_filename;
    bool is_disappear_at_goal;

    desc.add_options()("help", "produce help message")
            ("input,i", po::value<string>(&input_filename)->required(), "input file (txt)")
            ("output,o", po::value<string>(&output_filename)->required(), "output file (csv)")
            ("disappear-at-goal", po::bool_switch(&is_disappear_at_goal), "make agents to disappear at goal rather than staying there");

    try
    {
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help") != 0u)
        {
            cout << desc << "\n";

            return 0;
        }
    }
    catch (po::error& e)
    {
        cerr << e.what() << endl << endl;
        cerr << desc << endl;

        return 1;
    }

    ///*  CBS test
    // string instance_filename = argv[1];
    HighLevel test_cbs(input_filename, output_filename);
    // test_cbs.print_map();
    test_cbs.high_level_search();

    // LowLevel test_low_level(test_cbs.get_map(), test_cbs.get_agents());
    // test_low_level.print_map();
     //*/

    return 0;
}

// Test on ubuntu platform:
// cmake . ; make
// ./cbs_mine -i ./benchmark/8x8_obst12_txt/map_8by8_obst12_agents8_ex10.txt -o ./result/experimental_results.csv

// Test on windows platform:
//  -i ../benchmark/8x8_obst12_txt/map_8by8_obst12_agents8_ex10.txt -o ../result/experimental_results.csv
