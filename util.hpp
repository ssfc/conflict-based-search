//
// Created by take_ on 2023/8/3.
//

#ifndef DRAW_PATH_PY_UTIL_HPP
#define DRAW_PATH_PY_UTIL_HPP

#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>
#include <boost/functional/hash.hpp>

class Location
{
public:
    int x;
    int y;

public:
    Location() = default;

    Location(int input_x, int input_y) :
    x(input_x),
    y(input_y)
    {}

    bool operator==(const Location& other) const
    {
        return std::tie(x, y) == std::tie(other.x, other.y);
    }
};

// 对C++标准库中的std::hash特化，用于自定义Location类的哈希函数。
// 哈希函数的目的是将Location对象转换为一个哈希值，以便可以在哈希表等数据结构中使用。
namespace std
{
    template <>
    struct hash<Location>
    {
        size_t operator()(const Location& location) const
        {
            size_t seed = 0;
            boost::hash_combine(seed, location.x);
            boost::hash_combine(seed, location.y);

            return seed;
        }
    };
}

class TimeLocation
{
public:
    int time_step;
    Location location;

public:
    TimeLocation() = default;

    TimeLocation(int input_time, Location input_location) :
            time_step(input_time),
            location(input_location)
    {}

    bool operator==(const TimeLocation& other) const
    {
        return std::tie(location, time_step)
        == std::tie(other.location, other.time_step);
    }
};

namespace std
{
    template <>
    struct hash<TimeLocation>
    {
        size_t operator()(const TimeLocation& location_time) const
        {
            size_t seed = 0;
            boost::hash_combine(seed, hash<Location>()(location_time.location));
            boost::hash_combine(seed, hash<int>()(location_time.time_step));

            return seed;
        }
    };
}


struct Conflict
{
    enum Type
    {
        VertexConflict,
        EdgeConflict,
    };

    Type conflict_type;

    int agent_id_1; // first collision agent;
    int agent_id_2; // second collision agent;

    int time_step; // collision time

    // collision locations, vertex collision contains 1 locations, edge collision contains 2 locations.
    // 如果发生边冲突, locations的顺序是agent_1的行进顺序，和agent_2的行进顺序相反。
    std::vector<Location> locations;

};

// 由于是negative constraint, 所以记录的是禁止的state.
class NegativeConstraint
{
public:
    enum Type
    {
        VertexConstraint,
        EdgeConstraint,
    };

    Type constraint_type;

    int agent_id; // 人物
    int time_step; // 时间
    std::vector<Location> locations; // 可能是1个(点冲突), 也可能是2个(边冲突)
    // 如果发生边冲突, locations的顺序是agent的行进顺序

public:
    NegativeConstraint() = default;

    NegativeConstraint(Type input_type,
                       int input_agent_id,
                       int input_time,
                       std::vector<Location> input_locations) :
                       constraint_type(input_type),
                       agent_id(input_agent_id),
                       time_step(input_time),
                       locations(std::move(input_locations))
    {}

    bool operator==(const NegativeConstraint& other) const
    {
        return std::tie(agent_id, locations, time_step)
        == std::tie(other.agent_id, other.locations, other.time_step);
    }

    bool operator!=(const NegativeConstraint& other) const
    {
        return std::tie(agent_id, locations, time_step)
        != std::tie(other.agent_id, other.locations, other.time_step);
    }
};

class EdgeConstraint
{
public:
    int time_step;
    Location location_1;
    Location location_2;

public:
    EdgeConstraint(int time, Location input_location_1, Location input_location_2)
            : time_step(time),
              location_1(input_location_1),
              location_2(input_location_2)
    {}

    bool operator==(const EdgeConstraint& other) const
    {
        return std::tie(time_step, location_1, location_2) ==
               std::tie(other.time_step, other.location_1, other.location_2);
    }

    friend std::ostream& operator<<(std::ostream& os, const EdgeConstraint& edge_constraint)
    {
        return os << "EC(" << edge_constraint.time_step << ","
                  << edge_constraint.location_1.x << "," << edge_constraint.location_1.y << ","
                  << edge_constraint.location_2.x << "," << edge_constraint.location_2.y << ")";
    }
};

namespace std
{
    template <>
    struct hash<EdgeConstraint>
    {
        size_t operator()(const EdgeConstraint& edge_constraint) const
        {
            size_t seed = 0;

            boost::hash_combine(seed, hash<int>()(edge_constraint.time_step));
            boost::hash_combine(seed, hash<Location>()(edge_constraint.location_1));
            boost::hash_combine(seed, hash<Location>()(edge_constraint.location_2));

            return seed;
        }
    };
}

class AgentConstraints
{
public:
    std::vector<NegativeConstraint> agent_constraints;
    std::unordered_set<TimeLocation> vertex_constraints;
    std::unordered_set<EdgeConstraint> edge_constraints;
    int max_goal_constraint_time;
public:
    AgentConstraints() = default;

    AgentConstraints(std::vector<NegativeConstraint> input_agent_constraints,
                     int input_max_goal_constraint_time) :
            agent_constraints(std::move(input_agent_constraints)),
            max_goal_constraint_time(input_max_goal_constraint_time)
    {}

    // 将另一个对象中的所有元素插入到当前对象的集合中。
    void add_constraint(const NegativeConstraint& new_constraint)
    {
        if(new_constraint.constraint_type == NegativeConstraint::VertexConstraint)
        {
            vertex_constraints.insert(TimeLocation(new_constraint.time_step,
               new_constraint.locations[0]));

            max_goal_constraint_time = std::max(max_goal_constraint_time, new_constraint.time_step);

        }
        else
        {
            edge_constraints.insert(EdgeConstraint(new_constraint.time_step,
               new_constraint.locations[0], new_constraint.locations[1]));
        }
    }
};


using Path = std::vector<Location>;
void print_single_constraint(const NegativeConstraint& input_constraint);
Location move_agent(Location current_location, int action_choice);

#endif //DRAW_PATH_PY_UTIL_HPP
