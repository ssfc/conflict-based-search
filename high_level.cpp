//
// Created by take_ on 2023/8/2.
//

#include "high_level.hpp"

#include <utility>

using namespace std;

HighLevel::HighLevel(string input_filename,
                     string  _to_filename):
        instance_filename(std::move(input_filename)),
        to_log_name("output.yaml"),
        to_csv_path(std::move(_to_filename)),
        num_rows(0),
        num_columns(0),
        num_agents(0),
        start_time(clock()),
        num_generated_high_level_nodes(0),
        num_expanded_high_level_nodes(0),
        num_expanded_low_level_nodes(0),
        total_vertex_constraint(0),
        total_edge_constraint(0)
{
    // ifstream fromfile("map_8by8_obst12_agents5_ex_test.txt");
    // ifstream fromfile("./benchmark/8x8_obst12/map_8by8_obst12_agents5_ex_test.txt");
    ifstream fromfile(instance_filename);
    if (fromfile.is_open())
    {
        fromfile >> num_rows >> num_columns;

        map.resize(num_rows);
        for(int i=0;i<num_rows;i++)
        {
            map[i].resize(num_columns);
        }

        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_columns; j++)
            {
                char c;
                fromfile >> c;
                if(c=='@')
                {
                    map[i][j] = 0;
                    obstacles.insert(Location(j, i));
                }
                else if(c=='.')
                {
                    map[i][j] = 1;
                }
            }
        }

        fromfile >> num_agents;
        starts.resize(num_agents);
        goals.resize(num_agents);
        for(int i=0;i<num_agents;i++)
        {
            fromfile >> starts[i].x;
            fromfile >> starts[i].y;
            fromfile >> goals[i].x;
            fromfile >> goals[i].y;
        }

        // 检查地图读入是否正确。
        /*
        std::cout << "goals[0].x: " << goals[0].x << std::endl;
        std::cout << "goals[0].y: " << goals[0].y << std::endl;

        std::cout << "obstacles: ";
        for(int i=0; i<num_rows; i++)
        {
            for (int j = 0; j < num_columns; j++)
            {
                if(map[i][j] == 0)
                {
                    // 既然j对应x, i对应y, 那么(j, i)对应(x, y)
                    cout << "(" << j << "," << i << ") ";
                }
            }
        }
        cout << endl;
         */

        fromfile.close();

        // int iter_goal = 0;
        for(auto this_goal : goals)
        {
            // cout << "iter_goal: " << iter_goal << endl;
            auto this_agent_h_values = compute_agent_h_values(this_goal);
            multi_agent_h_values.emplace_back(this_agent_h_values);
            // print_multi_agent_h_values();
            // iter_goal++;
        }
    }
    else
    {
        cerr << "Error opening file." << endl;
    }
}

unordered_map<Location, int> HighLevel::compute_agent_h_values(Location input_goal)
{
    priority_queue<ConstructorListNode> open_list;
    unordered_map<Location, int> h_values;
    auto root = ConstructorListNode{0, input_goal};
    open_list.emplace(root);
    h_values[input_goal] = 0;
    // int iter_computed = 0;
    while(!open_list.empty())
    {
        // cout << "iter computed: " << iter_computed << endl;

        // print open_list
        // print_constructor_list(open_list);

        // print h_values
        // print_agent_h_values(h_values);

        auto current = open_list.top();
        // cout << "Current locations: "; print_constructor_list_node(current);
        open_list.pop();
        // 计算h_values的时候不需要考虑原地不动的情况(因为这种情况下h_value是不变的), 所以i从1开始。
        for(int i=1;i<5;i++)
        {
            auto child_location = move_agent(current.location, i);
            // cout << "child_location: (" << child_location.y << "," << child_location.x << ")" << endl;
            int child_cost = current.cost + 1;
            // cout << "child_cost: " << child_cost << endl;

            // test whether child locations is out of range.
            if(child_location.y < 0 || child_location.y >= num_rows
               || child_location.x < 0 || child_location.x >= num_columns)
            {
                continue;
            }
            // test whether child locations is passable.
            if(map[child_location.y][child_location.x] == 0)
            {
                continue;
            }

            auto child = ConstructorListNode{child_cost, child_location};
            // cout << "child: "; print_constructor_list_node(child);
            // child_location 存在于 closed_list 中
            if (h_values.find(child_location) != h_values.end())
            {
                // cout << "child_location in closed list" << endl;
                if(h_values[child_location] > child_cost)
                {
                    h_values[child_location] = child_cost;
                    open_list.emplace(child);
                }
            }
            else // child_location 不在 closed_list 中
            {
                // cout << "child_location not in closed list" << endl;
                h_values[child_location] = child_cost;
                open_list.emplace(child);
            }
        }

        // iter_computed++;
    }

    return h_values;
}

Location HighLevel::get_location(vector<TimeLocation> input_path, int input_time)
{
    if(input_time < 0)
    {
        return input_path[0].location;
    }
    else if(input_time < input_path.size())
    {
        return input_path[input_time].location;
    }
    else
    {
        return input_path.back().location;
    }
}


bool HighLevel::get_all_paths_first_conflict(vector<vector<TimeLocation>> input_paths, Conflict& first_conflict)
{
    size_t max_t = 0;
    for (const auto& path : input_paths)
    {
        max_t = std::max(max_t, path.size());
    }

    for(int i=0; i < input_paths.size() - 1; i++)
    {
        for(int j=i+1; j < input_paths.size(); j++)
        {
            for(int t=0; t<max_t; t++)
            {
                auto a1_current_location = get_location(input_paths[i], t);
                auto a1_next_location = get_location(input_paths[i], t + 1);
                auto a2_current_location = get_location(input_paths[j], t);
                auto a2_next_location = get_location(input_paths[j], t + 1);
                if(a1_next_location == a2_next_location)  // vertex conflict
                {
                    first_conflict.conflict_type = Conflict::VertexConflict;
                    first_conflict.agent_id_1 = i;
                    first_conflict.agent_id_2 = j;
                    first_conflict.locations.emplace_back(a1_next_location);
                    first_conflict.time_step = t + 1; // 因为是在next_location冲突的, 所以时间是t+1
                    total_vertex_constraint++;

                    return true;
                }
                // edge conflict: 可以看出在表达式形式上表现为一种交叉。
                if((a1_current_location == a2_next_location) && (a1_next_location == a2_current_location))
                {
                    first_conflict.conflict_type = Conflict::EdgeConflict;
                    first_conflict.agent_id_1 = i;
                    first_conflict.agent_id_2 = j;
                    // 注意到locations的顺序是按照a1的前进顺序决定的，和a2的前进顺序相反。
                    first_conflict.locations.emplace_back(a1_current_location);
                    first_conflict.locations.emplace_back(a1_next_location);
                    first_conflict.time_step = t + 1;
                    total_edge_constraint++;

                    return true;
                }
            }
        }
    }

    return false;
}

vector<NegativeConstraint> HighLevel::generate_constraints_from_conflict(const Conflict& input_conflict)
{
    vector<NegativeConstraint> constraints_from_conflict;
    if(input_conflict.conflict_type == Conflict::VertexConflict)  // vertex conflict
    {
        constraints_from_conflict.emplace_back(NegativeConstraint::VertexConstraint,
                                               input_conflict.agent_id_1, input_conflict.time_step, input_conflict.locations);
        constraints_from_conflict.emplace_back(NegativeConstraint::VertexConstraint,
                                               input_conflict.agent_id_2, input_conflict.time_step, input_conflict.locations);
    }
    else  // Edge conflict
    {
        constraints_from_conflict.emplace_back(NegativeConstraint::EdgeConstraint,
                                               input_conflict.agent_id_1, input_conflict.time_step, input_conflict.locations);
        vector<Location> reversed_locations = {input_conflict.locations[1], input_conflict.locations[0]};
        constraints_from_conflict.emplace_back(NegativeConstraint::EdgeConstraint,
                                               input_conflict.agent_id_2, input_conflict.time_step, reversed_locations);
    }

    return constraints_from_conflict;
}


bool HighLevel::high_level_search()
{
    auto freq = count_freqs();
    std::cout << "freq: " << freq << std::endl;

    if (freq >= 5)
    {
        std::cout << "Already have more than 5 results on this Device-Instance-Method-Disappear-Seed"
                  << std::endl;

        return false;
    }

    // A1 LINE 1
    // Root.multi_agent_constraints = ∅ // 最开始无约束
    HighLevelNode root;
    priority_queue<HighLevelNode> open_list;

    // A1 LINE 2
    // Root.solution = find individual paths using the low-level() // 用低层算法计算每个智能体的path
    root.paths.resize(num_agents);
    root.cost = 0;
    root.all_agents_constraints.resize(num_agents);
    auto low_level = LowLevel(num_columns, num_rows, obstacles, starts, goals, multi_agent_h_values, 0,
                              root.all_agents_constraints[0]);
    for(int i=0;i<num_agents;i++)
    {
        low_level.set_low_level_context(i, root.all_agents_constraints[0]);
        bool is_path_found = low_level.low_level_search(root.paths[i], num_expanded_low_level_nodes);

        if (!is_path_found)
        {
            return false;
        }

        // A1 LINE 3
        // Root.cost = SIC(Root.solution) // 计算目标函数
        root.cost += root.paths[i].size() - 1;
    }

    // Evaluate A1 LINE 2
    // print_node_paths(root);

    // Evaluate A1 LINE 3
    // cout << "root cost: " << root.cost << endl;


    // Implement A1 LINE 4
    // insert Root to OPEN
    open_list.emplace(root);
    num_generated_high_level_nodes++;
    // cout << "Generate node " << num_generated_high_level_nodes << endl;

    //  A1 LINE 5
    // while OPEN not empty do
    int iter_high_level = 0;  // Iteration需要走144个，重复实验结果一样。
    while(!open_list.empty())
    // while(!open_list.empty() && iter_high_level < 2)
    {
        // cout << "High level iteration: " << iter_high_level << endl;
        //  A1 LINE 6
        // P ← the best node from OPEN // the lowest solution cost
        auto best_node = open_list.top();
        Conflict best_node_conflict;
        bool is_found_conflict = get_all_paths_first_conflict(best_node.paths, best_node_conflict);
        num_expanded_high_level_nodes++;
        // cout << "Expand node " << num_expanded_high_level_nodes << endl;
        open_list.pop();

        // cout << "best node cost: " << best_node.cost << endl;
        //  A1 LINE 7
        // Validate the paths in P until a first_conflict occurs.
        //  A1 LINE 8
        // if P has no first_conflict then
        if(!is_found_conflict)  // 没有冲突，意味着找到了解。
        {
            std::ofstream to_log(to_log_name, std::ios::out);

            std::ofstream to_csv(to_csv_path, std::ios::app);  // 以追加模式打开文件
            if (!to_csv.is_open()) {
                std::cerr << "Error opening csv!" << std::endl;
                return false;
            }

            to_csv << -1 << ","; // id

            std::filesystem::path filePath(instance_filename);
            std::string instance = filePath.filename().string(); // 提取文件名部分
            to_csv << instance << ","; // instance

            to_csv << "12400F" << ","; // device
            to_csv << "cbs-mine" << ","; // method
            to_csv << 0 << ","; // disappear or not
            to_csv << -1 << ","; // random seed

            cout <<"\nFound a solution! \n";
            to_log <<"Found a solution! \n";

            double elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
            cout << "time(s): " << elapsed_time
                 << " frequency:" << double(iter_high_level) / elapsed_time << endl;
            to_log << "time(s): " << elapsed_time
                 << " frequency:" << double(iter_high_level) / elapsed_time << endl;

            cout << "Sum of costs: " << best_node.cost << endl;
            to_log << "Sum of costs: " << best_node.cost << endl;
            to_csv << best_node.cost << ",";

            to_csv << elapsed_time << ",";
            to_csv << "NULL" << ","; // comment
            to_csv << "https://github.com/ssfc/conflict-based-search" << ","; // method source

            // 获取当前时间点
            auto now = std::chrono::system_clock::now();

            // 转换为 time_t 格式
            std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

            // 输出时间
            to_csv << std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S")
                    << "\n";

            cout << "HighLevelExpanded: " << num_expanded_high_level_nodes << endl;
            to_log << "HighLevelExpanded: " << num_expanded_high_level_nodes << endl;

            // cout << "Generated nodes: " << num_generated_high_level_nodes << endl;
            cout << "lowLevelExpanded: " << num_expanded_low_level_nodes << endl;
            to_log << "lowLevelExpanded: " << num_expanded_low_level_nodes << endl;

            cout << "total vertex constraint: " << total_vertex_constraint << endl;
            to_log << "total vertex constraint: " << total_vertex_constraint << endl;

            cout << "total edge constraint: " << total_edge_constraint << endl;
            to_log << "total edge constraint: " << total_edge_constraint << endl;

            cout << "Solution:" << endl;
            to_log << "Solution:" << endl;

            print_node_paths(best_node);

            //  A1 LINE 9
            // return P.solution // P is goal
            return true;
        }

        //  A1 LINE 10
        // C ← first conflict (ai, aj , v, t) in P
        // print first_conflict
        // print_node_conflicts(best_node);

        auto new_constraints = generate_constraints_from_conflict(best_node_conflict);  // 将当前冲突分解为两个限制

        // A1 LINE 11
        // for each agent ai in C do
        for(const auto& new_constraint : new_constraints)
        {
            // A1 LINE 12
            // A ← new node
            // A1 LINE 14
            // new_node.solution ← best_node.solution.
            HighLevelNode new_node = best_node;

            int ai = new_constraint.agent_id;

            // A1 LINE 13
            // new_node.constriants ← best_node.constriants + (ai, s, t)
            new_node.all_agents_constraints[ai].agent_constraints.emplace_back(new_constraint); // (ai, s, t) or (aj, s, t)
            new_node.all_agents_constraints[ai].add_constraint(new_constraint);

            // 为什么这里的constraints不会和new_constraint重叠？
            // 因为low-level-search已经满足旧constraints, 所以新产生的constraint不可能和已有的constraint重叠，所以无需重叠检测。

            // A1 LINE 15
            // Update new_node.solution by invoking low-level(ai)
            // constraints是整个一坨放进LowLevel的，不分智能体，不分时间步。
            low_level.set_low_level_context(ai, new_node.all_agents_constraints[ai]);
            // 增量更新，先减再加
            new_node.cost -= new_node.paths[ai].size();
            bool is_path_found = low_level.low_level_search(new_node.paths[ai], num_expanded_low_level_nodes);
            // cout << "Agent " << ai << " new_constraint path: " << endl; print_path(new_node.paths[ai]);

            if(is_path_found)
            {
                // A1 LINE 16
                // new_node.cost = SIC(new_node.solution)
                new_node.cost += new_node.paths[ai].size();

                // A1 LINE 17
                // Insert new_node to OPEN
                open_list.emplace(new_node);
                num_generated_high_level_nodes++;
                // cout << "Generate node " << num_generated_high_level_nodes << endl;
            }
        }

        iter_high_level++;
    }

    return false;
}

// debug func
vector<vector<int>> HighLevel::get_map()
{
    return map;
}

// debug func
void HighLevel::print_map()
{
    cout << "num rows: " << num_rows << " num columns: " << num_columns << endl;
    for(auto & each_row : map)
    {
        for(int element : each_row)
        {
            cout << element << " ";
        }
        cout << endl;
    }
    cout << "num agents: " << num_agents << endl;
    for(int i=0;i<num_agents;i++)
    {
        cout << "(" << starts[i].y << "," << starts[i].x << ")->(";
        cout << goals[i].y << "," << goals[i].x << ")" << endl;
    }
}

// debug func
void HighLevel::print_constructor_list_node(ConstructorListNode input_node)
{
    cout << "(" << input_node.location.y << "," << input_node.location.x << ")" << endl;
}

// debug func
void HighLevel::print_constructor_list(std::priority_queue<ConstructorListNode> input_list)
{
    priority_queue<ConstructorListNode> temp = std::move(input_list);
    cout << "constructor open list: {";
    while (!temp.empty())
    {
        cout << "((" << temp.top().location.y << "," << temp.top().location.x;
        cout << ")," << temp.top().cost << "), ";
        temp.pop();
    }
    cout << "}" << endl;
}

// debug func
void HighLevel::print_agent_h_values(const unordered_map<Location, int>& input_h_values)
{
    cout << "input_h_values: {";
    for (const auto& pair : input_h_values)
    {
        const Location& coord = pair.first;
        int value = pair.second;
        cout << "((" << coord.y << "," << coord.x << ")," << value << "),";
    }
    cout << "}" << endl;
}

// debug func
void HighLevel::print_multi_agent_h_values()
{
    for(const auto& this_heuristic : multi_agent_h_values)
    {
        for (auto pair : this_heuristic)
        {
            const Location& coord = pair.first;
            cout << "Key: " << coord.y << " " << coord.x << " , Value: " << pair.second << endl;
        }
        cout << endl;
    }
}

// debug func
void HighLevel::print_path(const std::vector<TimeLocation>& input_path)
{
    for(auto element : input_path)
    {
        cout << "(" << element.location.y << "," << element.location.x << ")" << ", ";
    }
    cout << endl;
}

// debug func
void HighLevel::print_node_paths(HighLevelNode input_node)
{
    for(int i=0;i<input_node.paths.size();i++)
    {
        cout << "agent " << i << ": ";
        for(auto each_node : input_node.paths[i])
        {
            cout << "(" << each_node.location.x << "," << each_node.location.y << "),";
        }
        cout << endl;
    }
}

// debug func


