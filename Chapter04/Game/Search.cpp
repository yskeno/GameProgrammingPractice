#include <vector>
#include <unordered_map>
#include <queue>
#include <iostream>

// 基本的に、ゴールから親のノードを順に辿っていって探索する。
// その親のノードを決めるアルゴリズムとしてBFS, GBFS, A*を使用している。

struct GraphNode {
	// Adjacency list
	// Each node has pointers to adjacent nodes
	std::vector<GraphNode*> mAdjacent;
};

struct Graph {
	// A graph contains nodes
	std::vector<GraphNode*> mNodes;
};

struct WeightedEdge {
	// Which nodes are connected by this edge?
	struct WeightedGraphNode* mFrom;
	struct WeightedGraphNode* mTo;

	// Weight of this edge
	float mWeight;
};

struct WeightedGraphNode {
	// Stores outgoing edges
	std::vector<WeightedEdge*> mEdges;
};

struct WeightedGraph {
	std::vector<WeightedGraphNode*> mNodes;
};

struct GBFSScratch {
	const WeightedEdge* mParentEdge = nullptr;
	float mHeuristic = 0.0f;
	bool mInOpenSet = false;
	bool mInCloseSet = false;
};

using GBFSMap =
std::unordered_map<const WeightedGraphNode*, GBFSScratch>;

struct ASterScratch {
	const WeightedEdge* mParentEdge = nullptr;
	float mHeuristic = 0.0f;
	float mActualFromStart = 0.0f;	// stores the path-cost
	bool mInOpenSet = false;
	bool mInCloseSet = false;
};

using AsterMap =
std::unordered_map<const WeightedGraphNode*, ASterScratch>;

// ConputeHeuristic() can use any heuristic h(x) function, such as Manhattan or Euclidean distance.
float ComputeHeuristic(const WeightedGraphNode* a, const WeightedGraphNode* b) {
	return 0.0f;
}

// A* Search
// A* adds a path-cost component, which is the actual cost from start to goal.
bool AStar(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, AsterMap& outMap)
{
	std::vector<const WeightedGraphNode*> openSet;

	// Set current node to start, and mark in closed set
	const WeightedGraphNode* current = start;
	outMap[current].mInCloseSet = true;

	do {
		// Add adjacent nodes to open set
		for (const WeightedEdge* edge : current->mEdges) {
			const WeightedGraphNode* neighbor = edge->mTo;
			// Get scratch data for this node
			ASterScratch& data = outMap[neighbor];
			// Only check nodes that aren't in the closed set
			if (!data.mInCloseSet) {
				if (!data.mInOpenSet) {
					// Not in the open set, so parent must be current
					data.mParentEdge = edge;
					data.mHeuristic = ComputeHeuristic(neighbor, goal);
					// Actual cost is the parent's plus cost of traversing edge
					data.mActualFromStart = outMap[current].mActualFromStart + edge->mWeight;
					data.mInOpenSet = true;
					openSet.emplace_back(neighbor);
				}
				else {
					// Compute what new actual cost is if current becomes parent
					float newG = outMap[current].mActualFromStart + edge->mWeight;
					if (newG < data.mActualFromStart) {
						// Current should adopt this node
						data.mParentEdge = edge;
						data.mActualFromStart = newG;
					}
				}
			}
		}

		// If open set is empty, all possible paths are exhausted
		if (openSet.empty())
			break;

		// Find lowest cost node in open set
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
			// Calculate f(x) for nodes a/b
			float fOfA = outMap[a].mHeuristic + outMap[a].mActualFromStart;
			float fOfB = outMap[b].mHeuristic + outMap[b].mActualFromStart;
			return fOfA < fOfB;
		});
		// Set to current and move from open to closed
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = true;
		outMap[current].mInCloseSet = true;
	} while (current != goal);

	// Did we find a path?
	return (current == goal) ? true; false;
}

// Greedy Best-First Search
// The argorithm of select the node which has the lowest heuristic cost.
// It means this path is not optimal path.
bool GBFS(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, GBFSMap& outMap)
{
	std::vector<const WeightedGraphNode*> openSet;

	// Set current node to start, and mark in closed set.
	// a variable to track the current node under evaluation.
	// Initially, current is the start node, and add it to the closed set
	// by marking it as closed in the scratch map.
	const WeightedGraphNode* current = start;
	outMap[current].mInCloseSet = true;

	// Main loop of of GBFS
	do {
		// First, it looks at all nodes adjacent to the current node.
		// Add adjacent nodes to open set
		for (const WeightedEdge* edge : current->mEdges) {
			// Get scratch data for this node
			GBFSScratch& data = outMap[edge->mTo];
			// Add it only if it's not in the closed set
			if (!data.mInCloseSet) {
				// Set the adjacent node's parent edge, incoming from the current node.
				data.mParentEdge = edge;
				// For nodes not already in the openset,
				// compute the heuristic from the node to goal
				// and adds the nodes to the open set.
				if (!data.mInOpenSet) {
					// Compute the heuristic for this node, and add to open set
					data.mHeuristic = ComputeHeuristic(edge->mTo, goal);
					data.mInOpenSet = true;
					openSet.emplace_back(edge->mTo);
				}
			}
		}

		// If open set is empty, all possible paths are exhausted.
		// This happens only if there is no path from start to goal.
		if (openSet.empty())
			break;

		// If there are still nodes in the open set, the algorithm continues.
		// Find lowest cost node in open set
		// NOTE: 3rd param of min_element() is lambda expression.
		// NOTE: min_element() returns an iterator to the minimum element.
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
			return outMap[a].mHeuristic < outMap[b].mHeuristic;
		});

		// Set to current and move from open set to closed set
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInCloseSet = true;

		// the main loop continues if the current node is not the goal node.
	} while (current != goal);

	// Did we find a path?
	// Loop ends when the while condition fails or hit the break statement(the open set is empty)
	return (current == goal) ? true : false;
}

using NodeToParentMap =
std::unordered_map<const GraphNode*, const GraphNode*>;

// Breadth-First Search
bool BFS(const Graph& graph, const GraphNode* start, const GraphNode* goal,
	NodeToParentMap& outMap) {
	// Wheather we found a path
	bool pathFound = false;
	// Nodes to consider
	std::queue<const GraphNode*> q;
	// Enqueue the first node
	q.emplace(start);

	while (!q.empty()) {
		// Dequeue a node
		const GraphNode* current = q.front();
		q.pop();
		if (current == goal) {
			pathFound = true;
			break;
		}

		// Enqueue adjacent nodes that aren't already in the queue
		for (const GraphNode* node : current->mAdjacent) {
			// If the parent is null, it hasn't been enqueued
			// (except for the start node)
			const GraphNode* parent = outMap[node];
			if (parent == nullptr && node != start) {
				// Enqueue this node, setting its parent
				outMap[node] = current;
				q.emplace(node);
			}
		}
	}

	return pathFound;
}

void testBFS() {
	Graph g;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			GraphNode* node = new GraphNode;
			g.mNodes.emplace_back(node);
		}
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			GraphNode* node = g.mNodes[i * 5 + j];
			if (i > 0)
				node->mAdjacent.emplace_back(g.mNodes[(i - 1) * 5 + j]);
			if (i < 4)
				node->mAdjacent.emplace_back(g.mNodes[(i + 1) * 5 + j]);
			if (j > 0)
				node->mAdjacent.emplace_back(g.mNodes[i * 5 + j - 1]);
			if (j < 4)
				node->mAdjacent.emplace_back(g.mNodes[i * 5 + j + 1]);
		}
	}

	NodeToParentMap map;
	bool found = BFS(g, g.mNodes[0], g.mNodes[9], map);
	std::cout << found << '\n';
}

void testHeuristic(bool useASter) {
	WeightedGraph g;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			WeightedGraphNode* node = new WeightedGraphNode;
			g.mNodes.emplace_back(node);
		}
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			WeightedGraphNode* node = g.mNodes[i * 5 + j];
			if (i > 0) {
				WeightedEdge* e = new WeightedEdge;
				e->mFrom = node;
				e->mTo = g.mNodes[(i - 1) * 5 + j];
				e->mWeight = 1.0f;
				node->mEdges.emplace_back(e);
			}
			if (i < 4) {
				WeightedEdge* e = new WeightedEdge;
				e->mFrom = node;
				e->mTo = g.mNodes[(i + 1) * 5 + j];
				e->mWeight = 1.0f;
				node->mEdges.emplace_back(e);
			}
			if (j > 0) {
				WeightedEdge* e = new WeightedEdge;
				e->mFrom = node;
				e->mTo = g.mNodes[i * 5 + j - 1];
				e->mWeight = 1.0f;
				node->mEdges.emplace_back(e);
			}
			if (j < 4) {
				WeightedEdge* e = new WeightedEdge;
				e->mFrom = node;
				e->mTo = g.mNodes[i * 5 + j + 1];
				e->mWeight = 1.0f;
				node->mEdges.emplace_back(e);
			}
		}
	}
	bool found = false;
	if (useASter) {
		AsterMap map;
		found = AStar(g, g.mNodes[0], g.mNodes[9], map);
	}
	else {
		GBFSMap map;
		found = GBFS(g, g.mNodes[0], g.mNodes[9], map);
	}
	std::cout << found << '\n';
}