# Conflict-Based Search For Optimal Multi-Agent Path Finding

(Prompt: translate title of this article in Chinese)

基于冲突的搜索算法，用于最优多智能体路径规划

https://ojs.aaai.org/index.php/AAAI/article/view/8140

https://github.com/ssfc/papers/blob/main/2012%20Conflict-Based%20Search%20For%20Optimal%20Multi-Agent%20Path%20Finding.pdf

https://github.com/ssfc/papers/blob/main/2012%20Conflict-Based%20Search%20For%20Optimal%20Multi-Agent%20Path%20Finding.md

## Abstract

(Prompt: Generalize Section "Abstract" of this article in Chinese)

这篇文章介绍了一种新的搜索算法，叫做基于冲突的搜索（Conflict-Based Search, CBS）。CBS可以解决多智能体路径规划问题（MAPF），即在一个图中为多个智能体分别找到从起点到终点的不冲突的路径，同时最小化总的路径代价。CBS是一种两层的算法，高层搜索一个基于冲突的树，每个节点包含一些对单个智能体的位置和时间的约束；低层为每个智能体在满足约束的情况下寻找最短的路径。CBS可以保证找到最优解，并且在很多情况下比以前的算法快一个数量级。

Keywords:  Conflict-Based Search => Multi-Agent Path Finding

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

AAAI 2012. CCF A. Cited by 902 times. 

### Q: AAAI是什么会议？

这还用问？

### Q: Introduce authors of this article in Chinese one by one

这篇文章的作者有四位，分别是：

- Guni Sharon，来自以色列本-古里安大学的工业与系统工程系，主要研究人工智能、自动规划和多智能体系统。
- Roni Stern，也来自以色列本-古里安大学的工业与系统工程系，主要研究启发式搜索、自动规划和机器学习。
- Ariel Felner，同样来自以色列本-古里安大学的工业与系统工程系，主要研究启发式搜索、自动规划和约束满足问题。
- Nathan Sturtevant，来自美国丹佛大学的计算机科学系，主要研究启发式搜索、游戏人工智能和路径规划。

## Introduction

(Prompt: Generalize content from "In the multi-agent path finding (MAPF) problem" to "solvers by up to a full order of magnitude" of this article in Chinese)

多智能体路径规划（MAPF）问题是这样的：给定一个图G(V, E)和一组k个智能体，标记为a1 . . . ak。每个智能体ai都有一个起始位置si ∈ V和一个目标位置gi ∈ V。在每个时间步，智能体可以移动到相邻的位置或者在当前位置等待。任务是为每个智能体返回一组动作，使得所有智能体都能在不与其他智能体冲突（即不同时在同一位置）的情况下移动到各自的目标，同时最小化累积代价函数。常用的代价函数是所有智能体到达目标位置所需的时间步数之和。因此，移动和等待动作都花费1.0。

MAPF在机器人、视频游戏、车辆路由等方面有实际应用（Silver 2005; Dresner and Stone 2008）。在一般形式下，MAPF是NP完全的，因为它是滑动拼图问题的推广，而滑动拼图问题已知是NP完全的（Ratner and Warrnuth 1986）。

我们假设一个集中式计算环境，有一个单独的CPU需要解决MAPF问题。这在逻辑上等价于一个分散式环境，其中每个智能体都有自己的计算能力，但智能体之间是完全合作的，有完全的知识共享和免费的通信。 解决MAPF问题有两种主要方法。在解耦方法中，为每个智能体分别规划路径。一个典型的例子是HCA*（Silver 2005）。给定一个智能体排序，为智能体ai找到一条路径，然后将其写入（预留）到一个全局预留表中。对于后续的智能体，必须避免前面的智能体预留的位置和时间点。类似的方法被用于指导需要穿越交通路口的汽车（Dresner and Stone 2008）。 其他解耦方法建立了类似于交通法规的流量限制，指导智能体在给定位置只能按照指定方向移动（Wang and Botea 2008; Jansen and Sturtevant 2008）。解耦方法运行相对快速，但不一定保证最优性甚至完备性。最近提出了一些新的完备解耦算法，适用于树（Khorshid, Holte, and Sturtevant 2011）和一般图（Luna and Bekris 2011）。

由于我们旨在优化地解决MAPF问题，本文的重点是耦合方法。在这种方法中，MAPF被形式化为一个全局的单智能体搜索问题。这种形式化可以通过一个基于A*的算法来解决，该算法搜索一个随着智能体数量增长而呈指数增长的状态空间。耦合（全局） 搜索通常以显著的计算代价返回最优解。以前的耦合方法以不同的方式处理大型搜索空间（Ryan 2008; 2010; Standley 2010; Standley and Korf 2011; Sharon et al. 2011a）。

Sharon等人（2011a; 2011b）指出，最优MAPF算法的行为对于给定问题实例的特征（如图的拓扑和大小、智能体的数量、分支因子等）非常敏感。没有普遍优势的算法；不同的算法在不同的情况下表现良好。 我们提出了一种新的算法，称为基于冲突的搜索（CBS）。CBS是耦合和解耦方法的连续体。CBS保证了最优解，像大多数耦合方法一样，但CBS执行的路径规划是严格的单智能体搜索，类似于解耦方法。CBS是一种两层算法，其中高层搜索在一个约束树（CT）中进行，其节点包含对单个智能体的时间和位置的约束。在约束树的每个节点，执行一个低层搜索，以在高层节点给定的约束下为所有智能体找到新的路径。与基于A*的搜索不同，CBS的高层搜索树是指数级的，在解决过程中遇到的冲突数目是指数级的。

我们分析CBS，并研究CBS与基于A*的方法相比在哪些情况下弱，在哪些情况下强。在许多情况下，CBS比其他最优解算法快一个数量级。

## Previous Optimal Solvers

求解最优MAPF问题的直接方法是把问题形式化为一个全局的搜索空间，然后用A\*算法来解决。状态是指把k个智能体放在V个顶点中的不同方式，没有冲突。在初始（目标）状态下，智能体ai位于顶点si (gi)。状态之间的操作是所有智能体的非冲突动作（包括等待）。设bbase为单个智能体的分支因子。全局的分支因子是b = O((bbase)k)。所有(bbase)k种动作组合都应该被考虑，只有那些没有冲突的才是合法的邻居。然后可以用任何基于A*的算法来解决问题。

一个常用的MAPF启发式函数（Standley 2010; Sharon et al. 2011a）是单个代价之和启发式函数（sum of individual costs, SIC）。对于每个智能体ai，我们假设没有其他智能体存在，计算它的最优单个路径代价。我们然后返回这些代价的和。对于没有障碍物的网格，这就是曼哈顿距离。组成SIC启发式函数的代价可以很容易地预先计算并存储在k个查找表中，每个表的大小是|V |。

### Q: 什么是曼哈顿距离？

曼哈顿距离（Manhattan distance），也称为城市街区距离或L1距离，是在笛卡尔坐标系中两点之间的距离度量方法。

曼哈顿距离是通过计算两点之间在水平和垂直方向上的距离之和来定义的，忽略了斜向的距离。这种距离计算方法得名于曼哈顿的城市街区布局，其中只能沿着道路的水平和垂直方向移动。

在二维平面上，对于两个点P₁(x₁, y₁)和P₂(x₂, y₂)，它们之间的曼哈顿距离（记为d₁）可以通过以下公式计算：

d₁ = |x₁ - x₂| + |y₁ - y₂|

其中，|x|表示x的绝对值。

曼哈顿距离的特点是它只考虑了水平和垂直方向上的移动，而不考虑斜向的移动。这使得曼哈顿距离在某些应用中更为实用，例如计算城市导航路径中的最短路径，或者在计算机视觉领域中的对象识别和目标追踪中用于特征匹配。

总而言之，曼哈顿距离是两点之间在水平和垂直方向上的距离之和，适用于许多实际问题中的距离度量。

### Standley’s enhancements

(Prompt: Generalize Section "Standley’s enhancements" of this article in Chinese)

Standley提出了三种方法来改进基本的A\*算法，分别是：独立检测（Independence detection, ID）、冲突避免表（Conflict voidance table, CAT）和操作分解（Operator decomposition, OD）。

独立检测（Independence detection, ID）是一个通用的框架，可以在其上使用任何可能的MAPF求解器。ID的基本思想是把智能体分成独立的组，每个组内的智能体可以找到一个不冲突的最优解。对于每个组，单独求解最优解，然后检查是否有组之间的冲突。如果有，就把冲突的组合并成一个新的组，并重新求解这个新的多智能体问题。这个过程重复直到没有组之间的冲突。

冲突避免表（Conflict voidance table, CAT）是一个动态的查找表，存储了每个组中每个智能体的位置和时间。当为一个给定的组使用MAPF求解器时，在具有相同f值的节点之间，优先选择在CAT中有更少条目的节点。

操作分解（Operator decomposition, OD）是针对基于A*的求解器的一种方法，它通过在正常状态之间引入中间状态来减少分支因子。中间状态是通过对单个智能体应用一个操作来生成的。这有助于在中间阶段剪枝误导方向，而不需要考虑所有智能体的移动。

### The Increasing Cost Tree Search (ICTS)

(Prompt: Generalize Section "The Increasing Cost Tree Search (ICTS)" of this article in Chinese)

ICTS是一种基于理解的算法，即一个完整的解决方案是由单个智能体的路径组成的。ICTS把MAPF问题分成两层。在高层，它搜索一个叫做递增代价树（increasing cost tree, ICT）的树。每个节点与一个代价向量相关联，每个代价向量包含每个单个智能体的代价。在低层，它对高层的ICT节点进行目标测试。给定一个代价向量，它搜索单个智能体路径的组合，每个路径的代价与向量中的相应代价相等，要么找到一个不冲突的解决方案，要么验证这样的解决方案不存在。ICTS在∆（最优解与初始状态的SIC启发式之差）小于k（智能体数量）的情况下比A\*算法更有效，在相反的情况下则无效。后来提出了一些剪枝技术，可以让高层识别一个给定节点不是目标而不需要调用低层。最好的这些技术叫做ICTS+3E。这种增强使用了关于最多3个智能体及其内部冲突的信息。这相当于使用了一种基于类似3个智能体组的启发式函数的A*搜索，例如加法PDB（Felner, Korf, and Hanan 2004）。但是，如何有效地构建这样的启发式函数对于一般MAPF算法来说还是一个开放问题。

## The Conflict Based Search Algorithm (CBS)

(Prompt: Generalize content from "The state space of MAPF is exponential in k" to "each part of this process in more detail below" of this article in Chinese)

MAPF的状态空间随着智能体数量k呈指数增长。相反，在单智能体路径规划问题中（k = 1），状态空间只与图的大小成线性关系。CBS通过**把MAPF问题分解为大量的单智能体路径规划问题来解决**。每个问题都相对容易解决，但是可能有指数数量的这样的单智能体问题。

CBS的定义。我们只在单agent的语境中使用path这个术语，并使用solution这个术语来表示给定的k个agent的k条路径的集合。对于给定的智能体ai，约束是一个元组（ai，v，t），其中智能体ai被禁止在时间步t占用顶点v。在算法的过程中，智能体与约束相关联。consistent path对于智能体ai是满足它所有约束的路径。同样，consistent solution是由path组成的解决方案，使得智能体ai的path与ai的约束一致。冲突是一个元组（ai，aj，v，t），其中智能体ai和智能体aj在时间点t占用顶点v。（Comment: 看样子是顶点冲突？其实也可以用来解释边冲突，把v理解成路径上的一个点就可以了 ）。如果所有路径没有冲突，则solution（k条路径）是valid。consistent solution可能是invalid的. 如果，尽管这些路径满足它们各自agent的约束，但这些路径之间有冲突。

CBS的关键思想是**为每个智能体生成一组约束，并找到与满足这些约束的路径。如果这些路径之间有冲突，就通过添加新的约束来解决冲突**。CBS分为两层。**在高层，找到冲突并添加约束。在低层，更新智能体的路径以使其与新的约束一致**。我们在下面详细介绍这个过程的每个部分。

### Q: 什么是状态空间？

状态空间（state space）是指在特定问题或系统中，所有可能的状态的集合。它是描述问题或系统的所有可能状态的抽象概念。

在状态空间中，每个状态代表着问题或系统在某个特定时间点的一个确定的状态。这些状态可以是离散的、连续的或抽象的，具体取决于问题的特性和建模需求。

状态空间在问题建模和求解中起着重要的作用，它为我们提供了对问题进行分析和解决的基础。通过定义状态空间，我们可以确定问题的初始状态、目标状态以及状态之间的转移关系。

状态空间的大小取决于问题的复杂性和描述精度。对于简单的问题，状态空间可能只包含几个离散的状态。而对于复杂的问题，状态空间可能非常庞大，甚至是无穷的。

在搜索问题中，例如路径规划或人工智能中的状态空间搜索，我们使用算法来遍历状态空间，找到最优或满足特定条件的状态序列。搜索算法的性能和效率很大程度上取决于状态空间的大小和结构。

总而言之，状态空间是一个包含问题或系统中所有可能状态的集合，它对于问题建模和求解至关重要，为我们提供了对问题进行分析、搜索和优化的基础。

### High-level: Search the Constraint Tree (CT)

(Prompt: Generalize Section "High-level: Search the Constraint Tree (CT)" of this article in Chinese)

在高层，CBS搜索一个叫做约束树（Constraint Tree, CT）的二叉树。每个CT树节点N包含以下数据：（1）一组约束（N.constraints）。CT的根节点包含一个空的约束集合。CT的一个子节点继承父节点的约束，并为一个智能体添加一个新的约束。（2）一个解决方案（N.solution）。一组k条路径，每个智能体一条。智能体ai的路径必须与N中ai的约束一致。这些路径是由低层搜索找到的。（3）当前解决方案的总代价（N.cost），即所有单个智能体路径代价的和。我们把这个代价称为节点的f值。

当N.solution是有效的时候，即所有智能体的路径没有冲突时，CT节点N是一个目标节点。高层在CT上进行最佳优先搜索 (best-first search) ，按照节点的代价排序。如果代价相同，就用冲突避免表（Conflict voidance table, CAT）来打破平局。

**在CT中处理一个节点**。 给定CT的一个节点N的约束列表，调用低层搜索。这个搜索返回每个智能体ai的一条最短路径，该路径满足节点N中与ai相关联的所有约束。一旦找到了每个智能体满足其约束的路径，这些路径就会相对于其他智能体进行验证。验证是通过模拟k条路径的集合来进行的。如果所有智能体都没有任何冲突地达到目标，那么这个CT节点N被声明为目标节点，并返回包含这组路径的当前解决方案（N.solution）。然而，如果在执行验证时发现两个或多个智能体ai和aj之间存在冲突C =（ai，aj，v，t），则验证停止，并将该节点声明为非目标节点。

**解决冲突**。 给定一个非目标CT节点N，其解决方案N.solution包含一个冲突Cn = (ai, aj , v, t)，我们知道在任何valid的solution中，最多只有一个冲突的agent（ai和aj）可以在时间t占据顶点v。因此，至少有一个约束（ai, v, t）或（aj , v, t）必须添加到N.constraints中的约束集合中。为了保证最优性，两种可能性都要检查，N被分割成两个子节点。两个子节点都继承了N的约束集合。左子节点通过添加约束（ai, v, t）来解决冲突，右子节点添加约束（aj , v, t）。

注意，对于给定的CT节点N，不必保存所有的累积约束。相反，它只需保存它的最新约束，并通过其祖先遍历从N到根的路径来提取其他约束。同样地，除了根节点外，低级搜索只应该对与新添加的约束相关联的智能体ai进行。其他代理的路径保持不变，因为没有为它们添加新的约束。

### Q: 什么是constraint tree?

Constraint Tree（约束树）是一种数据结构，用于表示和管理约束的层次结构。

在许多问题中，涉及到多个约束条件之间的关系和依赖。约束树提供了一种组织和表示这些约束的方式，以便更好地理解和处理它们。

约束树以树的形式呈现约束的层次结构。树的根节点表示最高级别的约束，而子节点表示该约束的更具体的子约束。通过这种层次结构，约束树可以展示约束之间的层级关系和依赖关系。

约束树的节点可以表示不同类型的约束，例如布尔约束、关系约束、优化约束等。每个节点可能包含与约束相关的信息，如约束的条件、目标、范围等。通过分析约束树的结构和节点的信息，可以帮助我们更好地理解约束之间的关系和限制条件。

约束树在许多领域和问题中都有应用，包括规划和调度问题、优化问题、人工智能和专家系统等。它可以用于表示和解决复杂的约束问题，帮助我们组织和管理约束，并在问题求解过程中进行有效的推理和决策。

总结而言，约束树是一种用于表示和管理约束的层次结构。它通过树的结构和节点的信息展示约束之间的层级关系和依赖关系，为问题的约束建模和求解提供了一种有组织的方式。

### Q: best-first search是什么？

Best-First Search（最佳优先搜索）是一种启发式搜索算法，用于在图或树结构中寻找路径或解决问题。

在最佳优先搜索中，每个节点都有一个估计的优先级或价值，该优先级用于指导搜索过程中的节点扩展顺序。算法会优先扩展具有最高优先级的节点，即具有最佳估计值的节点。这种选择策略使得算法倾向于先探索最有希望的节点，从而更有可能找到最优解或最接近最优解的解。

最佳优先搜索可以通过多种方式来确定节点的优先级。其中一种常见的方法是使用启发式函数（heuristic function），它为每个节点提供一个估计的代价或价值，用于评估该节点到目标的距离或期望的优势。这样，节点的优先级可以根据启发式函数的值来确定，而启发式函数的设计通常基于问题的特点和领域知识。

最佳优先搜索的关键在于节点扩展的顺序选择，它会根据优先级队列（priority queue）来管理节点的顺序。优先级队列按照节点的优先级排序，每次选择具有最高优先级的节点进行扩展。这样，在搜索过程中，算法会根据节点的优先级进行选择，以便更加集中地探索可能更接近目标的路径或解。

需要注意的是，最佳优先搜索并不能保证找到最优解，尤其在存在问题的复杂性和搜索空间的限制时。然而，它通常能够在合理的时间内找到较好的解，并且可以通过适当的启发式函数和搜索策略进行改进。

总结起来，最佳优先搜索是一种启发式搜索算法，根据节点的优先级或价值来选择节点的扩展顺序。它通常用于图或树结构中的路径搜索问题，并通过启发式函数来指导搜索过程，以更高效地找到较好的解。

### CBS Example

(Prompt: Generalize Section "CBS Example" of this article in Chinese)

CBS的高层算法如算法1所示，它有一个最优先搜索的结构。我们用图1（i）中的例子来说明，其中老鼠需要到达各自的奶酪。相应的CT如图1（ii）所示。根节点包含一个空的约束集合。低层现在为每个智能体返回一个最优路径（算法1的第2行），对于a1是< S1, A1, C, G1 >，对于a2是< S2, B1, C, G2 >。因此，这个节点的总代价是6。所有这些信息都保存在这个节点中。根节点然后被插入到OPEN列表中，并将被下一个扩展。

当验证两个智能体的路径时（第7行），发现了一个冲突，当两个智能体在时间步2到达顶点C时。这产生了冲突(a1, a2, C, 2)。因此，根节点被声明为非目标节点，并生成两个子节点来解决冲突（第11行）。左子节点添加了约束(a1, C, 2)，而右子节点添加了约束(a2, C, 2)。现在调用低层搜索（第15行），找到一个满足新约束的最优路径。对于左子节点，a1必须等待一步，要么在S1（或A1），路径< S1, A1, A1, C, G1 >被返回给a1。a2的路径< S2, B1, C, G2 >对于左子节点保持不变。左子节点的总代价现在是7。以类似的方式，生成右子节点，也是代价7。两个子节点都被加入到OPEN中（第17行）。在最后一步，左子节点被选择扩展，并验证底层路径。由于没有冲突，左子节点被声明为目标节点（第9行），并返回它的解决方案作为最优解。

```pseudocode
// Algorithm 1: high-level of CBS
Input: MAPF instance
    
// A1 LINE 1
Root.constraints = ∅ // 最开始无约束
// A1 LINE 2
Root.solution = find individual paths using the low-level() // 用低层算法计算每个智能体的path
// A1 LINE 3
Root.cost = SIC(Root.solution) // 计算目标函数
// SIC: abbreviation of sum of individual costs heuristic
// A1 LINE 4
insert Root to OPEN
// A1 LINE 5
while OPEN not empty do
    // A1 LINE 6
    best_node ← best node from OPEN // lowest solution cost
    // A1 LINE 7
    Validate the paths in best_node until a conflict occurs.
    // A1 LINE 8
    if best_node has no conflict then
        // A1 LINE 9
    	return best_node.solution // best_node is goal
    // A1 LINE 10
    // 这里的first_conflict不是计算出所有的conflicts然后返回第一个，而是发现第一个直接停止计算就行了。
    first_conflict ← first conflict (ai, aj, v, t) in best_node
    // A1 LINE 11
    for each agent ai in first_conflict do // namely ai (left child) and aj (right child)
        // A1 LINE 12
        new_node ← new node
        // A1 LINE 13
        new_node.constraints ← best_node.constraints + (ai, s, t)
        // A1 LINE 14
        new_node.solution ← best_node.solution.
        // A1 LINE 15
        Update new_node.solution by invoking low-level(ai)
        // A1 LINE 16
        new_node.cost = SIC(new_node.solution)
        // A1 LINE 17
        Insert new_node to OPEN
```

**k > 2个智能体的冲突**。在执行不同路径之间的验证（第7行）时，可能会发现一个k个智能体的冲突，其中k > 2。有两种方法可以处理这样的k个代理的冲突。我们可以生成k个子节点，每个子节点对k - 1个代理添加一个约束（即，每个子节点只允许一个代理在时间t占据冲突的顶点v）。或者，一个等价的形式化方法是只关注发现冲突的前两个代理，并根据它们的冲突进行分支。这样就把进一步的冲突留给树的更深层次。

这在图1（iii）中说明。顶部的树表示一个k路分支的CT，用于解决一个在时间t在顶点v有3个代理冲突的问题。底部的树表示同一个问题的一个二叉CT。可以看到，两棵树最深层的大小是相似的。两种方法的复杂度是相同的，因为它们都会以k个节点结束，每个节点有k - 1个新约束。为了简化描述，我们选择了第二种选项。

### Q: algorithm 1中的OPEN是干啥的？

OPEN是一个优先队列，用于存储和选择最有希望的节点进行扩展。最有希望的节点是指根据某种规则（例如启发式函数）评估距离目标节点最近的节点。OPEN中的每个节点都是通过前一个节点的扩展操作生成的，也就是说，它们是前一个节点的邻居节点。OPEN中的节点按照它们的评估值排序，每次从OPEN中取出评估值最小的节点进行扩展，直到找到目标节点或者OPEN为空为止。OPEN可以看作是一种存储未访问过的节点的列表，而CLOSED则是一种存储已访问过的节点的集合。OPEN和CLOSED可以帮助算法避免重复访问相同的节点，提高搜索效率和准确性。

### Q: Algorithm 1中为什么探测的是first conflict？

Algorithm 1中探测的是first conflict的原因是为了简化高层搜索树的分支过程。如果探测所有的冲突，那么每个节点可能会产生多个子节点，每个子节点对应一个不同的冲突解决方案。这样会导致高层搜索树的规模指数级增长，增加了计算复杂度和内存消耗。而如果只探测第一个冲突，那么每个节点只会产生两个子节点，每个子节点对应一个不同的约束条件。这样可以保证高层搜索树的规模是线性增长，降低了计算复杂度和内存消耗。

另外，探测第一个冲突并不会影响算法的最优性和完备性。因为算法会在高层搜索树中保留所有可能的解决方案，直到找到一个没有冲突的目标节点。如果第一个冲突的解决方案不可行，那么算法会回溯到上一层节点，并尝试另一个解决方案。这样可以保证算法不会遗漏任何有效的解决方案。

### Q: Algorithm 1中line 11的agent是不是包含ai aj两个？

Algorithm 1中line 11的agent是包含ai aj两个的。这一行的目的是为了找到与当前冲突相关的所有智能体，并将它们加入到高层搜索树的当前节点的约束集合中。这样做的原因是为了避免在后续的搜索过程中重复出现相同的冲突，从而提高搜索效率和解决方案质量。

### Low-level: Find Paths for CT Nodes

(Prompt: Generalize Section "Low-level: Find Paths for CT Nodes" of this article in Chinese)

低层给定一个智能体ai和一组相关的约束。它在底层图中进行搜索，为智能体ai找到一个最优路径，同时满足所有的约束。智能体ai被分离地解决，即忽略其他智能体。这个搜索是三维的，因为它包括两个空间维度和一个时间维度。可以使用任何单智能体路径规划算法来找到智能体ai的路径，同时验证约束是否满足。我们使用了一个在两个空间维度上有完美启发式函数的A*算法。每当生成一个状态x，且g(x) = t，并且在当前CT节点中存在一个约束(ai, x, t)，这个状态就被丢弃。

对于每个CT节点N，我们使用了一个低层的冲突避免表（conflict avoidance table, CAT）。它由节点N的当前路径初始化。当两个低层状态具有相同的f值时，优先选择在低层CAT中有更少冲突的状态。这会导致每个高层节点的更高质量的解决方案（更少的冲突智能体）。

## Theoretical Analysis: Optimality of CBS

(Prompt: Translate Section "Theoretical Analysis: Optimality of CBS" of this article in Chinese)

我们现在证明CBS会返回一个最优解，如果存在的话。首先，我们提供一些支持的命题。

定义1：对于给定的CT节点node，让CV (node)是所有解决方案的集合，它们：（1）满足node的约束集合，（2）也是有效的（即没有冲突）。

如果node不是一个目标节点，那么它的解决方案不会是CV (node)的一部分，因为它不是有效的。

定义2：对于任何解决方案p ∈ CV (node)，我们说节点node允许解决方案p。

Comment: 允许 = 满足约束且无冲突。因为无冲突也可能不满足当前约束。(2024年1月9日)

例如，CT的根节点有一个空的约束集合。任何有效解都满足空的约束集合。因此，根节点允许所有有效解。

一个解决方案在CV (node)中的代价是单个智能体代价的和。让min_cost(CV (node))表示CV (node)中所有解决方案的最小代价。

引理1：CT节点node的代价是min_cost(CV (node))的下界。

证明：node.cost是满足node约束的一组路径的最优代价。这组路径不一定是一个有效解。因此，node.cost是任何一组路径的下界，这些路径构成了node的一个有效解，因为没有单个智能体可以更快地达到它的目标。

Comment: 可以理解，因为node.cost算的是满足当前node约束，但不计冲突的理论最小路径。但是min_cost(CV (node))是要计算冲突的。在满足同样约束的条件下，不计冲突的代价当然要比避免冲突的代价要小一些。(2024年1月9日)

引理2：对于任何有效解p，在所有时间步组成的集合中，存在一个CT节点node在OPEN中，它允许p。

证明：通过归纳法对扩展周期进行归纳：对于基本情况，OPEN只包含根节点，它没有约束。因此，根节点允许所有有效解，也允许p。

现在假设这对前i个扩展周期是成立的。在周期(i+1)中，假设节点node，它允许p，在OPEN中被扩展，并且它的子节点N‘1 , N’2 被生成。任何在VS(node)中的有效解必须在VS(N‘1 )或VS(N’2)中，因为任何有效解必须满足至少一个新的约束。

Comment: 如果两个新约束都不满足，就会发生点冲突或者边冲突，所以任何有效解必须满足至少一个新的约束。(2024年1月9日)

定理1：CBS返回最优解。

证明：考虑一个目标节点G被高层选择扩展的扩展周期。在那个时候，所有有效解都被OPEN中的至少一个节点允许（引理2）。

令p表示任意一个有效解（代价为cost(p)），令node(p)表示在OPEN中允许p的节点，令cost(node)表示节点node的代价。

根据引理1，cost(node(p)) ≤ cost(p)。

由于G是一个目标节点，所以cost(G)是一个有效解的代价。

由于高层搜索以最优先方式探索代价，我们得到cost(G) ≤ cost(node(p)) ≤ cost(p)。

由于p是任意选择的，因此cost(G) ≤ cost(p)代表G是最优解。

Comment: open_set组成树的下沿。(2024年1月9日)

## Comparison with other algorithms

(Prompt: Generalize Section "Comparison with other algorithms" of this article in Chinese)

让C ∗是最优解的代价，让χ是f < C∗的所有节点的集合，让X = |χ|。众所周知，A\*必须扩展χ中的所有节点以保证最优性。此外，A\*被称为“最优有效的”，这意味着A\*扩展了保证最优解所必需的最小数量的节点（Dechter and Pearl 1985），对于给定的h函数。Sharon等人（2011a）表明，A\*可能生成多达X × (bbase)  k个节点，而ICTS可能生成多达X × k  ∆个节点。由于ICTS也扩展了χ中的所有节点，我们目前只限于讨论A\*。但是，χ的大小是多少呢？给定一个启发式函数h和解决方案深度d，一些预测算法被开发用于IDA\*，但它们是基于抽样的（Korf, Reid, and Edelkamp 2001; Zahavi et al. 2010; Lelis, Zilles, and Holte 2011）。X的最简单的上界是整个状态空间，尽管这可能是X的一个很大的过估。在MAPF的情况下，这是单个智能体路径的笛卡尔积，其中f = 3。相比之下，CBS只尝试了两条这样的路径来意识到代价为6的解不存在。此外，A\*计数的节点是多智能体节点，而CBS计数的是单智能体状态。这是CBS的另一个优势——每个节点的常数时间更小。A\*优于CBS的情况（开放空间）图1（iv）展示了一个Y >> X并且A\*将优于CBS的情况。对于这个问题，初始SIC启发式值是8。每个智能体有4条最优路径，但每个16条路径组合都在灰色单元格中有冲突。因此，C ∗ = 9，因为一个智能体必须至少等待一步来避免碰撞。对于这个问题，A\*将扩展5个f = 8的节点：{(D2, C1),(D3, C2),(D3, B1),(C2, B1),(C3, B2)}和3个f = 9的节点{(B3, B2),(A3, B3),(A3, B4)}直到找到目标，并且总共扩展了8个节点。现在考虑CBS。每个智能体有4条不同的最优路径。所有16种组合都在4个灰色单元格{C2, C3, B2, B3}中有冲突。因此，对于f = 8，总共会扩展16个CT节点，每个会扩展4个单智能体状态，总共是16 × 4 = 64个低层节点。接下来，我们考虑代价为9的目标CT节点。它将扩展7个新状态。因此，CBS总共扩展了Y = 71个状态。虽然很难预测这些算法在实际领域中的性能，但上述观察可以提供一些指导。如果有更多瓶颈，CBS将比基于A\*的方法有优势。如果有更多开放空间，A*将比CBS有优势。接下来我们展示了支持两种情况的实验结果。

## Experimental results

(Prompt: Generalize Section "Experimental results" of this article in Chinese)

我们在A\*，A\*增强了OD（记为A\*+OD），ICTS，ICTS+3E和CBS上进行了实验。除了ICTS+3E之外，所有算法都基于(sum of individual costs, SIC)启发式函数。ICTS+3E使用了更先进的剪枝技术，这些技术将来可能应用于CBS和A\*作为先进的启发式函数。尽管如此，CBS没有这种先进的启发式函数仍然在许多场景中优于ICTS+3E。在我们所有的实验中，我们也尝试了没有CAT-based tie braking的CBS。它比有CAT-based tie braking的CBS慢两倍。只显示更强版本。

**8x8 4-connected grid** 我们从一个8x8 4-connected grid开始，其中智能体的数量从3增加到13。我们遵循（Sharon et al. 2011a）并且只在预处理级别使用ID框架。我们报告了由ID发现的所有智能体在同一个独立组中的结果。图2（左）展示了**生成的节点数**和**运行时间**，平均在100个实例上。对于CBS的情况，报告了高层节点和低层状态。我们设置了5分钟的时间限制。如果一个算法不能在时间限制内解决一个实例，它就会停止并返回失败（“NA”在表中）。图2（右）显示了成功率，即能够在5分钟内被不同算法解决的实例百分比，随着智能体数量的增加。与（Sharon et al. 2011a）类似，我们没有报告ICTS变体的节点数。这是因为这个算法不仅仅基于搜索。显然，CBS显著优于A\*和A\*+OD，高达一个数量级。注意，虽然CBS有时生成比A\*+OD更多的节点，但它在许多情况下仍然更快，因为CBS（单智能体状态）每个节点的常数时间比A\*+OD（多智能体）小。CBS对于≥ 8和≥ 9个智能体分别比ICTS和ICTS+3E快。

Comment: 我还想着删除num_of_generated算了，原来这个数量也是评测的指标。

Comment: 原来节点数不仅统计高层的，还统计低层的。

**DAO maps** 我们还在来自游戏Dragon Age: Origins (Sturtevant 2012)的3个基准地图上进行了实验。在这里我们的目标是解决尽可能多智能体的问题，因此ID框架被作为问题解决的一个整体部分激活。在这个实验中，我们只显示最强的三种算法：A\*+OD，ICTS+3E和CBS的结果。表1显示了CBS和A\*在图3（左）中展示的三个DAO地图上扩展的状态数。显然，CBS在所有这些情况下都比A\*扩展了更少的状态。另外，请记住CBS节点扩展的代价小于A\*。唯一的例外是25个智能体的den520d地图。

图3（右）显示了给定智能体数量时三个地图的成功率。这里的结果是混合的，并没有全局赢家。可以清楚地看到ICTS+3E总是比A*+OD好。但是，CBS在这些地图上的性能支持了我们理论上的主张，即CBS在处理走廊和瓶颈时非常有效，但在开放空间中相当低效。对于den520d（顶部），没有瓶颈但有大量开放空间；CBS是第三名。对于ost003d（中间），有一些瓶颈和小的开放空间；CBS是中间的。最后，对于brc202b（底部），有许多狭窄的走廊和瓶颈，但很少有开放空间，因此CBS是最好的。注意，虽然den520和ost003都有开放空间，但它们在瓶颈数量上有所不同。

### Q: 文章中node generated和node expanded分别指代什么？两者是什么关系？

文章中的node generated和node expanded是用来衡量算法性能的两个指标。它们的含义如下：

- node generated是指算法在搜索过程中产生的节点的总数，包括已经扩展的节点和还未扩展的节点。node generated反映了算法的空间复杂度，即算法需要占用的内存空间。
- node expanded是指算法在搜索过程中实际访问的节点的数量，也就是从未扩展节点集合中取出并检查其邻居节点的节点。node expanded反映了算法的时间复杂度，即算法需要花费的计算时间。

两者之间的关系是：node expanded ≤ node generated，因为不是所有生成的节点都会被扩展。一般来说，node generated和node expanded越小，说明算法越高效。

Comment: 每当high_level_open_list里面添加一个new_node, num_node_generated就加一。每当high_level_open_list里面pop一个node出来, num_node_expanded就加一。num_node_generated是num_node_expanded的两倍，因为每一轮循环pop一个best_node, 但是却generate两个new_node, 因为1个collision被分成了两个。(2023年9月13日)

### Q: Figure 2中的k是什么意思? 

agent的数量。

### Q: 文中提到的 ID framework是什么？

文中提到的ID framework是指独立检测框架（Independence Detection Framework），它是一种解决多智能体路径规划（MAPF）问题的通用方法。ID框架的基本思想是将智能体分成若干个独立的组，每个组内的智能体之间不存在冲突。然后，对每个组分别使用任意的MAPF求解器来寻找最优路径。如果不同组之间出现冲突，就将冲突的组合并成一个新的组，并重新求解这个新的子问题。这个过程重复进行，直到没有任何组之间发生冲突为止。ID框架可以有效地减少搜索空间，提高求解效率。

### Q: 文中提到的CAT-based tie breaking是什么？

CAT-based tie breaking是一种用于多智能体路径规划（MAPF）的技术，它可以在搜索过程中优先选择那些与其他智能体冲突较少的节点。CAT是冲突避免表（Conflict Avoidance Table）的缩写，它是一个动态的查找表，用于记录每个智能体在每个时间步的位置。当搜索算法需要在具有相同f值的节点之间进行选择时，它会使用CAT来打破平局，选择那个在CAT中条目最少的节点。这样做的目的是为了生成更高质量的解决方案，即冲突较少的解决方案。

文中提到的CAT-based tie breaking是基于Standley（2010）提出的独立检测框架（ID framework）和操作分解（OD）技术的改进。ID框架是一种将智能体分成独立组的方法，每个组内的智能体之间没有冲突。OD技术是一种通过引入中间状态来降低分支因子的方法，中间状态是通过对单个智能体应用操作而产生的。CAT-based tie breaking可以与这些技术结合使用，以提高搜索效率和解决方案质量。

## Discussion and future work

(Prompt: Generalize Section "Discussion and future work" of this article in Chinese)

本文介绍了CBS算法，用于优化地解决MAPF问题。CBS的独特之处在于所有低层搜索都是作为单智能体搜索进行的。CBS的性能取决于问题的结构。我们展示了有瓶颈（图1（i））的情况，CBS表现良好，以及开放空间（图1（iv））的情况，CBS表现不佳。测试床地图问题中的实验结果支持我们的理论主张。这些领域有不同的开放空间和瓶颈比率。CBS在走廊和瓶颈更占主导地位的情况下优于其他算法。最近，一些优化MAPF求解器被引入，每个都有利弊，但没有通用的赢家。我们已经触及了表面；关于MAPF的特征以及它们如何影响不同算法的行为还需要进一步研究。CBS的一些进行中的方向包括：（1）适应ICTS+3E的元智能体思想。（2）对CT节点执行独立检测。（3）为CT找到可接受的启发式函数。（4）混合约束和搜索的方法与最近关于A*和SAT算法理论性质的工作有关（Rintanen 2011），这是一个需要更多研究的重要联系。（5）与CSP求解器的关系还不是很清楚。

Keywords:  Conflict-Based Search => Multi-Agent Path Finding