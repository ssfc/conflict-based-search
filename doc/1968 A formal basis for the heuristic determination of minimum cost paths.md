# A Formal Basis for the Heuristic Determination of Minimum Cost Paths

(Prompt: translate title of article in this page in Chinese)

一种启发式确定最小成本路径的形式基础

https://ieeexplore.ieee.org/abstract/document/4082128/

https://github.com/ssfc/papers/blob/main/1968%20A%20formal%20basis%20for%20the%20heuristic%20determination%20of%20minimum%20cost%20paths.pdf

https://github.com/ssfc/papers/blob/main/1968%20A%20formal%20basis%20for%20the%20heuristic%20determination%20of%20minimum%20cost%20paths.md

## Abstract

(Prompt: Generalize abstract of article in this page in Chinese)

- **研究主题**：一种基于启发式信息的图搜索算法
- **研究方法**：将启发式信息从问题域引入到图搜索的数学理论中，定义一个评估函数f(n)来选择下一个扩展的节点，证明了算法的可行性和最优性
- **研究结果**：提出了一种称为A*的图搜索算法，它可以保证找到从起点到目标点的最小代价路径，并且在给定的信息下，它比其他任何可行算法扩展的节点数都少
- **研究意义**：为图搜索问题提供了一种有效的解决方案，可以应用于多种工程和科学领域，如电话交换、迷宫导航、电路板布局等

Keywords:  A* => Path finding

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

IEEE TRANSACTIONS OF SYSTEMS SCIENCE AND CYBERNETICS. Cited by 14444. 

### Q: IEEE TRANSACTIONS OF SYSTEMS SCIENCE AND CYBERNETICS是什么期刊？

[A: IEEE TRANSACTIONS OF SYSTEMS SCIENCE AND CYBERNETICS是一个由IEEE出版的期刊，它涵盖了系统科学和控制论的领域，包括系统工程、系统分析、系统建模、系统优化、系统决策、系统仿真等方面的研究。这个期刊创刊于1965年，后来于1971年改名为IEEE TRANSACTIONS ON SYSTEMS, MAN, AND CYBERNETICS，再后来于2013年分为三个子期刊，分别是IEEE TRANSACTIONS ON SYSTEMS, MAN, AND CYBERNETICS: SYSTEMS，IEEE TRANSACTIONS ON SYSTEMS, MAN, AND CYBERNETICS: HUMAN-MACHINE SYSTEMS，和IEEE TRANSACTIONS ON CYBERNETICS](https://ieeexplore.ieee.org/xpl/RecentIssue.jsp?punumber=6221021)[1](https://ieeexplore.ieee.org/xpl/RecentIssue.jsp?punumber=6221021)[2](https://www.ieeesmc.org/publications/transactions-on-smc-systems/)[3](https://www.worldcat.org/title/ieee-transactions-on-systems-science-and-cybernetics/oclc/1127097570)。

### Q: Introduce authors of article in this page in Chinese one by one

- **彼得·E·哈特**：他是IEEE的会员，曾在斯坦福研究所的应用物理实验室的人工智能组工作，主要研究图搜索和模式识别方面的问题。
- **尼尔斯·J·尼尔森**：他也是IEEE的会员，同样曾在斯坦福研究所的应用物理实验室的人工智能组工作，主要研究人工智能和机器人学方面的问题。
- **伯特拉姆·拉斐尔**：他是一位计算机科学家，曾在麻省理工学院和斯坦福大学学习，后来加入了斯坦福研究所的人工智能组，主要研究人工智能和自然语言处理方面的问题。

## 1 INTRODUCTION

### 1.A The Problem of Finding Paths Through Graphs

(Prompt: Generalize Section "1.A The Problem of Finding Paths Through Graphs" of article in this page in Chinese)

这篇文章的第一节A部分概括了如何通过图来表示和解决一些工程和科学问题，例如电话路由、迷宫导航、印刷电路板布局等。文章介绍了两种方法来寻找图中的最小代价路径：数学方法和启发式方法。

数学方法关注抽象图的性质和有序地检查图中的节点以确定最小代价路径，但不考虑算法的计算可行性。

启发式方法利用问题域的特殊知识来提高特定图搜索问题的计算效率，但不能保证总能找到最小代价路径。

文章的目的是将这两种方法结合起来，描述如何将问题域的信息纳入图搜索的数学理论，并提出了一种通用的算法来利用这些信息来寻找图中的最小代价路径。

### 1.B Some Definitions About Graphs

(Prompt: Generalize Section "1.B Some Definitions About Graphs" of this article in Chinese)

这篇文章的第一部分B节介绍了一些关于图和搜索算法的定义，如下：

- 一个图G由一组称为节点的元素{ni}和一组有向线段称为弧的{eij}组成。如果epq是{eij}的一个元素，那么我们说从节点np到节点nq有一条弧，且nq是np的后继节点。我们只考虑那些每条弧都有一个与之相关的代价cij的图。我们用8表示图中所有弧的代价的最小值，称这样的图为8图。
- 很多问题的图不是显式地给出一组节点和弧，而是隐式地给出一组源节点{sc ni}和一个后继操作符P，它定义在{ni}上，对每个节点ni的值是一组对{(nj, cij)}。也就是说，对节点ni应用P会得到所有ni的后继节点nj和与之相关的弧的代价cij。对源节点、它们的后继节点等反复应用P，直到生成所有新的节点，就可以得到由此定义的图的显式表示。我们在本文中假设一个图总是以隐式形式给出的。
- 从任意节点n的子图Gn是由单个源节点n和在{ni}上定义的某个P隐式定义的图。我们说Gn中的每个节点都可以从n访问到。
- 从n到nk的路径是一个有序的节点集合(n1, n2, …, nk)，其中每个n_(i+1)都是ni的后继节点。当且仅当nj可以从ni访问到时，从ni到nj存在一条路径。每条路径都有一个代价，它是路径上每条弧的代价c(i,i+1)的和。从ni到nj的最优路径是在所有从ni到nj的路径中代价最小的那条路径。我们用h(ni, nj)表示这个代价。
- 本文关注的是从某个指定的起始节点s的子图Gs。我们定义一个非空的节点集T为目标节点。对于Gs中的任意节点n，如果从n到t的最优路径的代价不超过从n到T中任意节点的任何路径的代价，那么t是n的一个首选目标节点。为了简单起见，我们用h(n)表示从n到n的一个首选目标节点的唯一代价，即h(n) = min h(n,t)。 teT
- 我们感兴趣的算法是搜索Gs来找到从s到s的一个首选目标节点的最优路径。我们通过描述这样的算法一般如何进行来说明我们所说的搜索一个图和找到一个最优路径的意思。从节点s开始，它们通过重复应用后继操作符P来生成Gs的一部分。在算法的过程中，如果P被应用到一个节点，我们说算法扩展了那个节点。我们可以通过以下方式来记录从s到每个遇到的节点的最小代价路径。每次一个节点被扩展，我们就把到达每个后继节点n的最小代价路径的代价和沿着那条路径的n的前驱节点的指针存储在n中。最终，算法在某个目标节点t终止，不再扩展更多的节点。我们可以通过从t到s通过指针回溯来重构已知的从s到t的最小代价路径。我们称一个算法是可接受的，如果它能保证对任意8图找到从s到s的一个首选目标节点的最优路径。不同的可接受算法可能在扩展节点的顺序和数量上有所不同。在下一节中，我们将提出一种扩展节点的顺序，并证明由此产生的算法是可接受的。然后，在后面的一节中，我们将在一个温和的假设下证明，这个算法以一种最优的方式使用了由图表示的问题的信息。也就是说，它扩展了保证找到最优路径所必需的最少数量的节点。

### 1.C Algorithms for Finding Minimun Cost Paths

(Prompt: Generalize Section "1.C Algorithms for Finding Minimun Cost Paths" of this article in Chinese)

这篇文章的第一部分C节介绍了寻找图中最小代价路径的算法。主要内容如下：

- 定义了一个评估函数f(n)，用于选择下一个扩展的节点。f(n)由两部分组成：g(n)是从起点到节点n的已知最小代价，h(n)是从节点n到目标节点的估计最小代价。
- 提出了一个搜索算法A\*，它根据f(n)的大小来决定扩展哪个节点，直到找到一个目标节点为止。证明了如果h(n)是h(n)的一个下界，那么A\*是一个可行的算法，即它能保证找到一个最优路径。
- 假设h(n)满足一致性条件，即h(m,n)+h(n)≥h(m)对任意节点m和n成立，那么证明了A*是一个最优的算法，即它能在保证找到最优路径的前提下，扩展尽可能少的节点。
- 讨论了如何根据问题域的信息来计算h(n)，以及在存在多个f(n)相等的节点时，如何选择合适的规则来打破平局。

## 2  AN ADMISSIBLE SEARCHING ALGORITHM

### 2.A Description of the Algorithm

(Prompt: Generalize Section "2.A Description of the Algorithm" of this article in Chinese)

这篇文章的第二节“A.算法的描述”用中文概括如下：

- 这一节介绍了一种搜索算法A*，它的目的是在一个图中找到从起点到终点的最小代价路径。
- 算法A*的核心思想是**用一个评估函数f(n)来选择下一个要扩展的节点**，其中f(n)是从起点到终点经过节点n的最优路径的代价的估计。
- 算法A*的具体步骤是：
  - 把起点s标记为“开放”并计算f(s)。
  - 选择开放节点中f值最小的节点n。如果有多个f值相同的节点，优先选择终点节点。
  - 如果n是终点节点，把n标记为“关闭”并结束算法。
  - 否则，把n标记为“关闭”并对n应用后继操作符P。对n的每个后继节点计算f值，并把未被标记为“关闭”的后继节点标记为“开放”。如果有被标记为“关闭”的后继节点n’，并且f(n’)比之前的值更小，就重新把n’标记为“开放”。回到步骤2。

### 2.B The Evaluation Function

(Prompt: Generalize Section "2.B The Evaluation Function" of this article in Chinese)

这篇文章的第二节B部分介绍了评估函数的概念和选择。评估函数f(n)用于估计从起始节点s到目标节点t经过节点n的最优路径的代价。评估函数可以分为两部分：g(n)是从s到n的已知最优路径的代价，h(n)是从n到t的预估最优路径的代价。文章提出了一种特定的评估函数f(n) = g(n) + h(n)，并证明了如果h(n)是h(n)的一个下界，那么使用这个评估函数的搜索算法A\*是可行的，即能够保证找到从s到t的最优路径。文章还讨论了如何利用问题域的信息来计算h(n)的下界，以及这样做对算法效率的影响。文章假设h(n)满足一致性条件，即h(m,n) + h(n) > h(m)对任意节点m和n成立，其中h(m,n)是从m到n的最优路径的代价。文章证明了在这个条件下，算法A*是最优的，即没有其他可行算法能够比它展开更少的节点。

### 2.C The Admissibility of A*

(Prompt: Generalize Section "2.C The Admissibility of A*" of this article in Chinese)

这篇文章的第二节C部分用中文概括如下：

- 这部分介绍了一个评估函数f(n)，用来估计从起点s到终点t的最优路径经过节点n的代价。
- 评估函数f(n)由两部分组成：g(n)是从s到n的已知最小代价，h(n)是从n到t的预估最小代价。
- 作者证明了如果h(n)是h(n)的一个下界，即h(n)不大于从n到t的真实最小代价，那么使用f(n)作为评估函数的搜索算法A\*是可行的，即它能保证找到从s到t的最优路径。
- 作者还假设h(n)满足一致性条件，即对于任意两个节点m和n，h(m,n) + h(n) > h(m)，其中h(m,n)是从m到n的最小代价。这个条件意味着h(n)的估计不会因为其他节点的信息而改变。
- 作者利用一致性条件证明了A\*算法的一个重要性质，即如果节点n被A\*算法关闭，那么g(n) = g(n)。这个性质说明**A\*算法不需要重新打开一个已关闭的节点**，因为它已经找到了从s到n的最优路径。

## 3 ON THE OPTIMALITY OF A*

### 3.A Limitation of Subgraphs by Informationfrom the Problem

(Prompt: Generalize Section "3.A Limitation of Subgraphs by Informationfrom the Problem" of this article in Chinese)

这篇文章的第三节讨论了如何利用问题域中的信息来限制每个节点的子图集合。作者假设每个节点n有一个索引集合Qn，表示所有可能的子图，以及一个更小的索引集合On，表示根据问题域中的信息而剔除的不可能的子图。作者定义了一个估计函数h(n)，表示从节点n到其最优目标节点的最小代价，为Qn中的所有子图的最小值。作者还提出了一个一致性假设，要求h(n)不会因为使用其他节点的信息而改变。作者证明了在满足一致性假设的情况下，使用h(n)作为评估函数的算法A*是最优的，即它比任何其他不更多地使用问题域信息的可行算法展开更少的节点。

### 3.B A Consistency Assumption

(Prompt: Generalize Section "3.B A Consistency Assumption" of this article in Chinese)

这篇文章的第三节B部分，用中文概括如下：

- 这部分介绍了一种一致性假设，即评估函数h(n)在所有节点上满足一种三角不等式，即h(m,n)+h(n)≥h(m)，其中h(m,n)是从节点m到节点n的最优路径的代价，h(n)是从节点n到一个首选目标节点的最优路径的代价，h(m)是从节点m到一个首选目标节点的最优路径的代价。
- 这种假设意味着从问题域中获取的信息在计算h(n)时是一致使用的，不会因为节点的不同而改变或忽略。这种假设在实际问题中通常是成立的，例如在城市和道路的例子中，h(n)可以取为节点n到最近目标城市的直线距离，这就满足了一致性假设。
- 这种假设对于证明A\*算法的最优性是必要的，即在满足一致性假设的情况下，A\*算法展开的节点数是最少的，不会多于任何其他可行的算法。

Comment: 怪不得一致性假设就是三角不等式，原来出处在这里。(2023年11月23日)

### 3.C Proof of the Optimality of A *

(Prompt: Generalize Section "3.C Proof of the Optimality of A *" of this article in Chinese)

这篇文章的第三节C部分用中文概括如下：

- 本节证明了在一定的一致性假设下，A*算法是最优的，即它比任何其他可行算法扩展的节点数都少。
- 证明的方法是反证法，假设存在一个可行算法A，它比A*扩展的节点数少，那么就会出现以下三种情况之一：
  - 情况一：A算法终止于一个非目标节点。这与A算法的终止条件矛盾，所以排除这种情况。
  - 情况二：A算法无法终止。这说明存在一个有限步数的最优路径，但A\*算法一直没有找到。由引理1可知，A\*算法必然忽略了某个开放节点，它在最优路径上且其f值小于最优路径的f值。这与A\*算法的可行性矛盾，所以排除这种情况。
  - 情况三：A算法终止于一个目标节点，但没有达到最小代价。这说明A算法在终止前选择了一个f值大于最优路径的f值的节点。由引理1可知，此时必然存在一个开放节点，它在最优路径上且其f值小于最优路径的f值。这与A算法的选择规则矛盾，所以排除这种情况。
- 由于以上三种情况都不可能发生，所以假设不成立，即A*算法是最优的。

## 4 DISCUSSION AND CONCLUSIONS

### 4.A Comparisons Between A * and Other Search Techniques

(Prompt: Generalize Section "4.A Comparisons Between A * and Other Search Techniques" of this article in Chinese)

这篇文章的第四节A部分大致是这样的：

- **比较A\*算法和其他搜索技术**：这部分介绍了A\*算法与其他一些搜索图的最小代价路径的算法的区别和优劣。
- **A\*算法的特点**：A\*算法使用了一个评估函数f(n)，它是从起点到节点n的已知代价g(n)和从节点n到目标节点的估计代价h(n)的和。如果h(n)是h(n)的一个下界，那么A\*算法是可接受的，即能保证找到最优路径。如果h(n)满足一致性假设，即h(m,n)+h(n)≥h(m)，那么A\*算法是最优的，即不会比其他可接受算法扩展更多的节点。
- **其他搜索技术的特点**：一些算法，如Moore的“算法D”和Busacker和Saaty的动态规划实现，只记录g(n)，但不用它来决定扩展节点的顺序。这些算法采用“广度优先”的顺序，即先扩展离起点一步的所有节点，然后是两步的，依此类推。这些算法必须允许g(n)的值在之后被更低代价的路径更新。另一些算法，如Minty提出的算法，使用f(n)=g(n)作为评估函数，这相当于没有利用问题域的任何信息。这样的评估函数也满足一致性假设，但可能会扩展很多不必要的节点。

```pseudocode
function reconstruct_path(came_from, current)
    total_path := {current}
    while current in came_from.Keys:
        current := came_from[current]
        total_path.prepend(current)
    return total_path

// A* LINE 1            
// A* finds a path from start to goal.
// h is the heuristic function. h(n) estimates the cost to reach goal from node n.
function A_Star(start, goal, h)
	// A* LINE 2
    // For node n, came_from[n] is the node immediately preceding it on the cheapest path from the start
    // to n currently known.
    came_from := an empty map

    // A* LINE 3
    // For node n, g_score[n] is the cost of the cheapest path from start to n currently known.
    g_score := map with default value of Infinity
    // A* LINE 4
    g_score[start] := 0

    // A* LINE 5
    // For node n, f_score[n] := g_score[n] + h(n). f_score[n] represents our current best guess as to
    // how cheap a path could be from start to finish if it goes through n.
    f_score := map with default value of Infinity
    // A* LINE 6
    f_score[start] := h(start)
        
    // A* LINE 7
    // The set of discovered nodes that may need to be (re-)expanded.
    // Initially, only the start node is known.
    // This is usually implemented as a min-heap or priority queue rather than a hash-set.
    open_set := {start}

    // A* LINE 8
    while open_set is not empty
        // A* LINE 9
        // This operation can occur in O(Log(N)) time if open_set is a min-heap or a priority queue
        current := the node in open_set having the lowest f_score[] value
        // A* LINE 10
        if current = goal
            // A* LINE 11
            return reconstruct_path(came_from, current)

        // A* LINE 12
        open_set.Remove(current)
        // A* LINE 13
        for each neighbor of current
            // A* LINE 14
            // d(current,neighbor) is the weight of the edge from current to neighbor
            // tentative_g_score is the distance from start to the neighbor through current
            tentative_g_score := g_score[current] + d(current, neighbor)
            // A* LINE 15
            if tentative_g_score < g_score[neighbor]
                // A* LINE 16
                // This path to neighbor is better than any previous one. Record it!
                came_from[neighbor] := current
                // A* LINE 17
                g_score[neighbor] := tentative_g_score
                // A* LINE 18
                f_score[neighbor] := tentative_g_score + h(neighbor)
                // A* LINE 19
                if neighbor not in open_set
                    // A* LINE 20
                    open_set.add(neighbor)

    // A* LINE 21
    // Open set is empty but goal was never reached
    return failure
```

Version 2

```pseudocode
// A* LINE 1            
// A* finds a path from start to goal.
// h is the heuristic function. h(n) estimates the cost to reach goal from node n.
function A_Star(start, goal, h)
	// A* LINE 2
    // For node n, came_from[n] is the node immediately preceding it on the cheapest path from the start
    // to n currently known.
    came_from := an empty map

    // A* LINE 3
    // For node n, g_score[n] is the cost of the cheapest path from start to n currently known.
    g_score := map with default value of Infinity
    // A* LINE 4
    g_score[start] := 0

    // A* LINE 5
    // For node n, f_score[n] := g_score[n] + h(n). f_score[n] represents our current best guess as to
    // how cheap a path could be from start to finish if it goes through n.
    f_score := map with default value of Infinity
    // A* LINE 6
    f_score[start] := h(start)
        
    // A* LINE 7
    // The set of discovered nodes that may need to be (re-)expanded.
    // Initially, only the start node is known.
    // This is usually implemented as a min-heap or priority queue rather than a hash-set.
    open_set := {start}

	// A* LINE 8
	// node that has already been evaluated. In other words, already been poped from open_set. 
	closed_set := the empty set

    // A* LINE 9
    while open_set is not empty
        // A* LINE 10
        // This operation can occur in O(Log(N)) time if open_set is a min-heap or a priority queue
        current := the node in open_set having the lowest f_score[] value
        // A* LINE 11
        if current = goal
            // A* LINE 12
            total_path := {current}
			// A* LINE 13
            while current in came_from.Keys:
				// A* LINE 14
                current := came_from[current]
                // A* LINE 15
                total_path.prepend(current)
            // A* LINE 16
            return total_path

        // A* LINE 17
        open_set.Remove(current)
        // A* LINE 18
        add current to closed_set. 
            
        // A* LINE 19
        for each neighbor of current
            // A* LINE 20
            if neighbor not in closed_set
                // A* LINE 21
                // d(current,neighbor) is the weight of the edge from current to neighbor
                // tentative_g_score is the distance from start to the neighbor through current
                tentative_g_score := g_score[current] + d(current, neighbor)
                // A* LINE 22
                if neighbor not in open_set 
                    // A* LINE 23
                    // This path to neighbor is better than any previous one. Record it!
                    came_from[neighbor] := current
                    // A* LINE 24
                    g_score[neighbor] := tentative_g_score
                    // A* LINE 25
                    f_score[neighbor] := tentative_g_score + h(neighbor)
                    // A* LINE 26
                    open_set.add(neighbor)
                // A* LINE 27
                else // neighbor in open_set
                    // A* LINE 28
                    // This path to neighbor is better than any previous one. Record it!
                    if tentative_g_score < g_score[neighbor]
                        // A* LINE 29
                        came_from[neighbor] := current
                        // A* LINE 30
                        g_score[neighbor] := tentative_g_score
                        // A* LINE 31
                        f_score[neighbor] := tentative_g_score + h(neighbor)
                        // A* LINE 32
                        open_set.update(neighbor)

    // A* LINE 33
    // Open set is empty but goal was never reached
    return failure
```

### 4.B The Heuristic Power of the Estimate A

(Prompt: Generalize Section "4.B The Heuristic Power of the Estimate A" of this article in Chinese)

这篇文章的第四部分B节讨论了估计函数h(n)的启发性能力。主要内容如下：

- 估计函数h(n)是用来评估从节点n到目标节点的最优路径的代价的一个函数，它可以利用问题域中的一些信息来计算。
- 作者提出了一种一致性假设，即h(n)在所有节点上的计算方法是一致的，不会因为节点的不同而改变。这个假设可以用一个类似于三角不等式的公式来表示，即h(m,n) + h(n) > h(m)，其中h(m,n)是从节点m到节点n的最优路径的代价。
- 作者证明了在满足一致性假设的情况下，A\*算法是一个最优的搜索算法，即它比任何其他不比它更有信息的可行算法展开的节点数都少。这是因为A\*算法总是选择估计函数f(n)最小的节点来展开，而f(n)是由g(n)和h(n)相加得到的，其中g(n)是从起始节点到节点n的已知最优路径的代价。
- 作者还讨论了在存在估计函数f(n)相等的节点时，A\*算法如何通过改变解决平局的规则来保持最优性。作者给出了一个定理，说明对于任何一个可行算法A\*，总是可以找到一个A\*算法的变种，使得它展开的节点都是A\*展开的节点。作者还给出了一个推论，说明A\*算法展开的节点数和A\*展开的节点数之间的差值，等于A*算法遇到的临界平局的次数。

Keywords:  A* => Path finding