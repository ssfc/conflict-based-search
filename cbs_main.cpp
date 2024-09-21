//
// Created by take_ on 2024/1/22.
//
// A c++ implementation for conflict-based search.

#include "high_level.hpp"

#include <boost/program_options.hpp>


using namespace std;


struct ExperimentalResult {
    int id;
    std::string instance;
    std::string device;
    std::string method;
    bool disappear_at_goal;
    int rand_seed;
    double cost;
    double run_time;
    std::string comment;
    std::string method_source;
    std::string date_edit;
};

// 删除引号的辅助函数
static std::string remove_quotes(const std::string& str)
{
    if (!str.empty() && str.front() == '"' && str.back() == '"')
    {
        return str.substr(1, str.size() - 2);
    }

    return str;
}

// 统计当前实验结果在csv中已经出现的次数
[[nodiscard]] int count_freqs(const std::string& _from_map_name, const std::string& _csv_path)
{
    std::vector<ExperimentalResult> experimental_results;
    std::ifstream from_csv(_csv_path);

    if (!from_csv.is_open()) {
        std::cerr << "Failed to open from_csv " << _csv_path << std::endl;
        return -1;
    }

    std::string line;
    bool isFirstLine = true;

    // 读取文件逐行处理
    int iter = 0;
    while (std::getline(from_csv, line)) {
        // std::cout << "iter: " << iter << std::endl;
        if (isFirstLine) {
            isFirstLine = false; // 跳过表头
            continue;
        }

        std::stringstream ss(line);
        std::string token;
        ExperimentalResult result;

        // 逐字段读取
        std::getline(ss, token, ',');
        // std::cout << "result id: " << token << std::endl;
        result.id = std::stoi(remove_quotes(token));

        std::getline(ss, token, ',');
        // std::cout << "instance: " << token << std::endl;
        result.instance = remove_quotes(token);

        std::getline(ss, token, ',');
        // std::cout << "device: " << token << std::endl;
        result.device = remove_quotes(token);

        std::getline(ss, token, ',');
        // std::cout << "method: " << token << std::endl;
        result.method = remove_quotes(token);

        std::getline(ss, token, ',');
        // std::cout << "disappear: " << token << std::endl;
        result.disappear_at_goal = std::stoi(remove_quotes(token));

        std::getline(ss, token, ',');
        // std::cout << "rand seed: " << token << std::endl;
        result.rand_seed = std::stoi(remove_quotes(token));

        std::getline(ss, token, ',');
        // std::cout << "cost: " << token << std::endl;
        result.cost = std::stod(remove_quotes(token));

        std::getline(ss, token, ',');
        // std::cout << "run time: " << token << std::endl;
        result.run_time = std::stod(remove_quotes(token));

        std::getline(ss, token, ',');
        result.comment = remove_quotes(token);

        std::getline(ss, token, ',');
        result.method_source = remove_quotes(token);

        std::getline(ss, token, ',');
        result.date_edit = remove_quotes(token);

        experimental_results.push_back(result);

        iter++;
    }

    from_csv.close();

    int freq = 0;
    for(const auto& result : experimental_results)
    {
        // 使用 std::filesystem::path 提取文件名
        std::filesystem::path filePath(_from_map_name);
        std::string instance = filePath.filename().string(); // 提取文件名部分

        if(result.device == "12400F"
           && result.instance == instance
           && result.method == "cbs-mine"
           && result.disappear_at_goal == 0
           && result.rand_seed == -1
                )
        {
            // std::cout << result.disappear_at_goal << " " << disappear_at_goal << std::endl;
            freq++;
        }
    }

    return freq;
}


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
    if(input_filename == "../benchmark/8x8_obst12_txt/")
    {
        for(int i=5;i<10;i++)
        {
            string instance_name = "../benchmark/8x8_obst12_txt/map_8by8_obst12_agents8_ex" +
                    std::to_string(i) + ".txt";

            auto freq = count_freqs(instance_name, output_filename);
            std::cout << "freq: " << freq << std::endl;

            if (freq >= 5)
            {
                std::cout << "Already have more than 5 results on this Device-Instance-Method-Disappear-Seed"
                          << std::endl;
            }
            else
            {
                // 如果不够, 就算到够为止
                while(freq < 5)
                {
                    HighLevel test_cbs(instance_name, output_filename);
                    test_cbs.high_level_search();
                    freq++;
                }
            }
        }
    }
    else
    {
        HighLevel test_cbs(input_filename, output_filename);
        // test_cbs.print_map();
        test_cbs.high_level_search();
    }

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
