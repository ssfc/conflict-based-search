//
// Created by take_ on 2023/8/22.
//

#ifndef DRAW_PATH_PY_LOW_LEVEL_HPP
#define DRAW_PATH_PY_LOW_LEVEL_HPP

#include <algorithm>
#include <iostream>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <boost/heap/d_ary_heap.hpp>

#include "util.hpp"

class LowLevelNode
{
public:
    TimeLocation time_location;
    int g_score;
    int f_score; // 类似于a_star，f=g+h

public:
    LowLevelNode(TimeLocation input_time_location,
                 int input_g_value,
                 int input_f_value):
            time_location(input_time_location),
            g_score(input_g_value),
            f_score(input_f_value)
            {}

    bool operator<(const LowLevelNode& other) const
    {
        // 实现比较逻辑，与你的 < 运算符一致
        // 返回 true 表示 lhs 应该排在 rhs 之前
        if (f_score != other.f_score)
        {
            return f_score > other.f_score; // 把数值小的放在堆顶
        }
        else
        {
            return g_score < other.g_score; // 把数值小的放在堆顶
        }
    }
};

class LowLevel
{
    size_t num_rows;
    size_t num_columns;
    std::unordered_set<Location> obstacles;
    int agent_id;
    std::vector<Location> starts;
    Location start;
    std::vector<Location> goals;
    Location goal;
    // multi_agent_h_values仅仅用在set_low_level_context中, 为了避免大规模参数传递
    std::vector<std::unordered_map<Location, int>> multi_agent_h_values;
    std::unordered_map<Location, int> agent_h_values;
    AgentConstraints agent_constraints;
    int max_goal_constraint_time;

public:
    // Q: 页面中函数LowLevel(std::vector<std::vector<int>> input_map,
    //             Location input_start,
    //             Location input_goal,
    //             std::unordered_map<Location, int> input_heuristics,
    //             int input_agent_id,
    //             std::vector<NegativeConstraint> input_multi_agent_constraints)的功能是什么？
    // 初始化一个CBS对象，即一个用于解决多智能体路径规划问题的冲突检测搜索对象。
    // Q: 页面中函数LowLevel()输入参数的数据类型和含义分别是什么？
    // @param input_map (vector<vector<int>>): 表示一个地图
    // @param input_start (Location): start location for this agent
    // @param input_goal (Location): goal location for this agent
    // @param input_heuristics (unordered_map<Location, int>):
    // [{(3, 0): 0, (4, 0): 1, (3, 1): 1, (2, 0): 1, (2, 1): 2, (1, 0): 2, (4, 1): 2, (3, 2): 2,
    // (5, 0): 2, (1, 1): 3, (0, 0): 3, (4, 2): 3, (3, 3): 3, (5, 1): 3, (6, 0): 3, (0, 1): 4, (1, 2): 4, (4, 3): 4,
    // (3, 4): 4, (2, 3): 4, (5, 2): 4, (6, 1): 4, (7, 0): 4, (2, 4): 5, (4, 4): 5, (3, 5): 5, (5, 3): 5, (6, 2): 5,
    // (7, 1): 5, (2, 5): 6, (1, 4): 6, (4, 5): 6, (3, 6): 6, (5, 4): 6, (6, 3): 6, (7, 2): 6, (1, 5): 7, (0, 4): 7,
    // (2, 6): 7, (4, 6): 7, (3, 7): 7, (6, 4): 7, (7, 3): 7, (0, 3): 8, (0, 5): 8, (1, 6): 8, (2, 7): 8, (5, 6): 8,
    // (7, 4): 8, (6, 5): 8, (0, 6): 9, (1, 7): 9, (5, 7): 9, (7, 5): 9, (0, 7): 10, (6, 7): 10, (7, 6): 10, (7, 7): 11},
    // Comment: goal(3, 0), 障碍物的点不在其中, 距离是折线的最短路径。
    // @param input_agent (int): agent_id
    // @param input_multi_agent_constraints (list of dict):
    // [{'agent': 3, 'location': [(2, 0)], 'time_step': 2},
    // {'agent': 3, 'location': [(2, 1), (3, 1)], 'time_step': 4},
    // {'agent': 1, 'location': [(3, 4), (3, 3)], 'time_step': 7},
    // {'agent': 1, 'location': [(1, 1)], 'time_step: 11},
    // {'agent': 0, 'location': [(1, 1), (1, 0)], 'time_step': 1}]
    // Q: 页面中函数LowLevel()输出结果的数据类型和含义分别是什么？
    // @return (void)
    LowLevel(int input_num_columns,
             int input_num_rows,
             std::unordered_set<Location> input_obstacles,
             std::vector<Location> input_starts,
             std::vector<Location> input_goals,
             std::vector<std::unordered_map<Location, int>> input_multi_agent_h_values,
             int input_agent_id,
             AgentConstraints input_constraints);


    void set_low_level_context(int input_agent_id,
                               AgentConstraints input_constraints);

    int calculate_h(const Location& location) const;

    // Q: 页面中函数is_element_of_vertex_constraints的功能是什么？
    // 检查当前智能体在给定的时间步是否违反了vertex_constraints。
    // Q: 页面中函数is_constraint_violated输入参数的数据类型和含义分别是什么？
    // @param current_location (Location): 这是一个元组，表示当前agent的位置坐标。例如，(1, 2)表示当前代理在第一行第二列的位置。
    // @param next_loc (Location): 这是一个元组，表示当前agent的下一个位置坐标。例如，(2, 2)表示当前代理要移动到第二行第二列的位置。
    // @param time_step (int): 表示当前的时间步。例如，0表示初始状态，1表示第一次移动后的状态，依此类推。
    // Q: 页面中函数def get_constraint_violated(self, current_location, next_loc, time_step)输出结果的数据类型和含义分别是什么？
    // return constraint (True/False): if a move at time_step violates a "negative" constraint in constraint table
    bool is_element_of_vertex_constraints(int next_time_step, Location next_location);

    // Q: 页面中函数is_element_of_edge_constraints的功能是什么？
    // 检查当前智能体在给定的时间步是否违反了edge_constraints。
    bool is_element_of_edge_constraints(Location current_location, Location next_location, int next_time_step);

    // 函数功能
    // 判断当前位置是否在地图范围内。
    bool is_in_range(const Location& location) const;

    // 函数功能
    // 判断当前位置是障碍物。
    bool is_obstacle(const Location& location) const;

    // Q: 页面中函数generate_children的功能是什么？
    // 根据当前节点生成一组子节点，用于后续的搜索。
    // Q: 页面中函数generate_children输入参数的数据类型和含义分别是什么？
    // @param current_node (dict): ‘location’: tuple，例如(1, 2), 表示agent的位置是(1, 2)。
    // ‘g_score’: 一个整数，表示从根节点到当前节点的路径长度。
    // ‘h_score’: 一个整数，表示从当前节点到目标节点的启发式估计。
    // ‘parent’: 一个字典，表示当前节点的父节点，如果当前节点是根节点，则为None。
    // ‘time_step’: 一个整数，表示当前节点的时间步。
    // ‘reached_goal’: 一个布尔值列表，表示每个代理是否已经到达目标位置，列表中的每个元素对应一个代理，例如[True, False]表示第一个代理已经到达目标位置，第二个代理还没有到达目标位置。
    // Q: 页面中函数generate_children输出结果的数据类型和含义分别是什么？
    // return children (list): 每个元素是一个dict. ‘location’: tuple，例如(1, 2), 表示agent的位置是(1, 2)。
    // ‘g_score’: 一个整数，表示从根节点到当前节点的路径长度。
    // ‘h_score’: 一个整数，表示从当前节点到目标节点的启发式估计。
    // ‘parent’: 一个字典，表示当前节点的父节点，如果当前节点是根节点，则为None。
    // ‘time_step’: 一个整数，表示当前节点的时间步。
    // ‘reached_goal’: 一个布尔值，表示agent是否已经到达目标位置。
    std::vector<TimeLocation> get_neighbors(TimeLocation current_time_location);

    bool is_solution(TimeLocation current_time_location);

    // Q: 页面中函数low_level_search的功能是什么？
    // 使用时空A*算法来寻找a group of agents的最优路径，即从起点到终点的最短且不冲突的路径。
    // Q: 页面中函数low_level_search输入参数的数据类型和含义分别是什么？
    // None
    // Q: 页面中函数low_level_search输出结果的数据类型和含义分别是什么？
    // return bool: true代表找到路径, false代表没找到.
    bool low_level_search(std::vector<TimeLocation>& solution_path, size_t& num_expanded_low_level_nodes);

    // debug func
};



#endif //DRAW_PATH_PY_LOW_LEVEL_HPP