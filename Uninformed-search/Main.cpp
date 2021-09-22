#include <string>
#include <iostream>

struct State
{
	std::string field[3];
	int emptyFieldStr;
	int emptyFieldCol;
};

struct Problem
{
	State INITIAL_STATE = { "724", "5 6", "831", 1, 1 };
	std::string operation[4] = {"Left", "Right", "Up", "Down"};
	State GOAL_TEST = { " 12", "345", "678", 0, 0 };
	int pathCost = 1;
};

struct Vertex
{
	State* state;
	Vertex* parentNode;
	std::string action;
	int depth;
	int pathCost;
};

State** initSuccessor(State* initState, std::string* action);
void searchPossibleStates(State* initState, State** newStates, std::string* action, int left, int right, int up, int down);
bool isEqualStates(State* one, State* two);
bool isRepeatState(Vertex* vertex, State* state);
void printSolve(Vertex* parent);
void printSolveBack(Vertex* parent);
void printSolve(Vertex** parent);

Vertex* generalSearch(Problem* problem, Vertex** (*queueingFn)(Vertex**, Vertex**));
Vertex* generalSearch(Problem* problem, Vertex** (*queueingFn)(Vertex**, Vertex**, int), int depth);
Vertex** generalBiDirectionalSearch(Problem* problem, Vertex** (*queueingFn)(Vertex**, Vertex**));

Vertex* makeNode(State* state);
Vertex* makeNode(State* state, Vertex* parent, std::string action);
Vertex** makeQueue(Vertex* elements);
bool empty(Vertex** queue);
Vertex* removeFront(Vertex**& queue);
bool goalTest(State* state, Problem* problem);
Vertex** expand(Vertex* node);
Vertex** queueingFn(Vertex** queue, Vertex** elements);
Vertex** enqueueAtEnd(Vertex** queue, Vertex** elements);
Vertex** enqueueAtCost(Vertex** queue, Vertex** elements);
Vertex** enqueueAtFront(Vertex** queue, Vertex** elements);
Vertex** enqueueAtFrontLimited(Vertex** queue, Vertex** elements, int depth);

Vertex* breadthFirstSearch(Problem* problem);
Vertex* uniformCostSearch(Problem* problem);
Vertex* depthFirstSearch(Problem* problem);
Vertex* depthLimitedSearch(Problem* problem, int depth);
Vertex* iterativeDeepeningSearch(Problem* problem);
Vertex** biDirectionalSearch(Problem* problem);

int main()
{
	Problem* problem = new Problem();
	
	//Vertex* solve = breadthFirstSearch(problem);
	//Vertex* solve = depthFirstSearch(problem);
	//Vertex* solve = uniformCostSearch(problem);
	//Vertex* solve = depthLimitedSearch(problem, 40);
	Vertex* solve = iterativeDeepeningSearch(problem);
	//Vertex** solve = biDirectionalSearch(problem);

	printSolve(solve);
}

State** initSuccessor(State* initState, std::string* action)
{
	int left = 1, right = 1, up = 1, down = 1;
	if (initState->emptyFieldStr == 0) down = 0;
	if (initState->emptyFieldStr == 2) up = 0;
	if (initState->emptyFieldCol == 0) right = 0;
	if (initState->emptyFieldCol == 2) left = 0;

	State** newStates = new State* [left + right + up + down + 1];
	for (int i = 0; i < left + right + up + down; i++)
	{
		newStates[i] = new State();
	}
	newStates[left + right + up + down] = nullptr;

	searchPossibleStates(initState, newStates, action, left, right, up, down);

	return newStates;
}

void searchPossibleStates(State* initState, State** newStates, std::string* action, int left, int right, int up, int down)
{
	int n = 0;
	int str = initState->emptyFieldStr;
	int col = initState->emptyFieldCol;
	if (left)
	{
		action[n] = "Left";
		for (int i = 0; i < 3; i++)
		{
			newStates[n]->field[i] = initState->field[i];
		}
		newStates[n]->field[str][col] = initState->field[str][col + 1];
		newStates[n]->field[str][col + 1] = ' ';
		newStates[n]->emptyFieldStr = str;
		newStates[n++]->emptyFieldCol = col + 1;
	}
	if (right)
	{
		action[n] = "Right";
		for (int i = 0; i < 3; i++)
		{
			newStates[n]->field[i] = initState->field[i];
		}
		newStates[n]->field[str][col] = initState->field[str][col - 1];
		newStates[n]->field[str][col - 1] = ' ';
		newStates[n]->emptyFieldStr = str;
		newStates[n++]->emptyFieldCol = col - 1;
	}
	if (up)
	{
		action[n] = "Up";
		for (int i = 0; i < 3; i++)
		{
			newStates[n]->field[i] = initState->field[i];
		}
		newStates[n]->field[str][col] = initState->field[str + 1][col];
		newStates[n]->field[str + 1][col] = ' ';
		newStates[n]->emptyFieldStr = str + 1;
		newStates[n++]->emptyFieldCol = col;
	}
	if (down)
	{
		action[n] = "Down";
		for (int i = 0; i < 3; i++)
		{
			newStates[n]->field[i] = initState->field[i];
		}
		newStates[n]->field[str][col] = initState->field[str - 1][col];
		newStates[n]->field[str - 1][col] = ' ';
		newStates[n]->emptyFieldStr = str - 1;
		newStates[n++]->emptyFieldCol = col;
	}
}

bool isEqualStates(State* one, State* two)
{
	if (one->field[0] == two->field[0] && one->field[1] == two->field[1] &&
		one->field[2] == two->field[2])
	{
		return true;
	}
	return false;
}

bool isRepeatState(Vertex* vertex, State* state)
{
	if (vertex->parentNode)
	{
		if (isRepeatState(vertex->parentNode, state)) return true;
	}
	if (isEqualStates(vertex->state, state))
	{
		return true;
	}
	return false;
}

void printSolve(Vertex* parent)
{
	if (parent->parentNode)
	{
		printSolve(parent->parentNode);
	}

	for (int i = 0; i < 3; i++)
	{
		std::cout << parent->state->field[i] << "\n";
	}
	std::cout << std::endl << "depth = " << parent->depth << std::endl;
}

void printSolveBack(Vertex* parent)
{
	for (int i = 0; i < 3; i++)
	{
		std::cout << parent->state->field[i] << "\n";
	}
	std::cout << std::endl << "depth = " << parent->depth << std::endl;

	if (parent->parentNode)
	{
		printSolveBack(parent->parentNode);
	}
}

void printSolve(Vertex** parent)
{
	printSolve(parent[0]);
	printSolveBack(parent[1]->parentNode);
}

Vertex* generalSearch(Problem* problem, Vertex** (*queueingFn)(Vertex**, Vertex**))
{
	Vertex** nodes = makeQueue(makeNode(&problem->INITIAL_STATE));
	Vertex* node;

	while (true)
	{
		for (int j = 0; nodes[j]; j++)
		{
			for (int i = 0; i < 3; i++)
			{
				std::cout << nodes[j]->state->field[i] << "\n";
			}
			std::cout << "depth = " << nodes[j]->depth << std::endl;
		}
		std::cout << "\n";

		if (empty(nodes))
		{
			return nullptr;
		}
		node = removeFront(nodes);
		if (goalTest(node->state, problem))
		{
			return node;
		}
		nodes = queueingFn(nodes, expand(node));
	}
}

Vertex* generalSearch(Problem* problem, Vertex** (*queueingFn)(Vertex**, Vertex**, int), int depth)
{
	Vertex** nodes = makeQueue(makeNode(&problem->INITIAL_STATE));
	Vertex* node;

	while (true)
	{
		for (int j = 0; nodes[j]; j++)
		{
			for (int i = 0; i < 3; i++)
			{
				std::cout << nodes[j]->state->field[i] << "\n";
			}
			std::cout << "depth = " << nodes[j]->depth << std::endl;
		}
		std::cout << "\n";

		if (empty(nodes))
		{
			return nullptr;
		}
		node = removeFront(nodes);
		if (goalTest(node->state, problem))
		{
			return node;
		}
		nodes = queueingFn(nodes, expand(node), depth);
	}
}

Vertex** generalBiDirectionalSearch(Problem* problem, Vertex** (*queueingFn)(Vertex**, Vertex**))
{
	Vertex** nodes1 = makeQueue(makeNode(&problem->INITIAL_STATE));
	Vertex** nodes2 = makeQueue(makeNode(&problem->GOAL_TEST));
	Vertex* node1, *node2;

	while (true)
	{
		for (int j = 0; nodes1[j]; j++)
		{
			for (int i = 0; i < 3; i++)
			{
				std::cout << nodes1[j]->state->field[i] << "\n";
			}
			std::cout << "depth = " << nodes1[j]->depth << std::endl;
		}
		std::cout << "\n";

		if (empty(nodes1))
		{
			return nullptr;
		}
		node1 = removeFront(nodes1);
		for (int i = 1; nodes2[i]; i++)
			if (isEqualStates(node1->state, nodes2[i]->state))
			{
				Vertex** solve = new Vertex * [2];
				solve[0] = node1;
				solve[1] = nodes2[i];
				return solve;
			}
		nodes1 = queueingFn(nodes1, expand(node1));

		for (int j = 0; nodes2[j]; j++)
		{
			for (int i = 0; i < 3; i++)
			{
				std::cout << nodes2[j]->state->field[i] << "\n";
			}
			std::cout << "depth = " << nodes2[j]->depth << std::endl;
		}
		std::cout << "\n";

		if (empty(nodes2))
		{
			return nullptr;
		}
		node2 = removeFront(nodes2);
		for (int i = 1; nodes1[i]; i++)
			if (isEqualStates(node2->state, nodes1[i]->state))
			{
				Vertex** solve = new Vertex * [2];
				solve[0] = nodes1[i];
				solve[1] = node2;
				return solve;
			}
		nodes2 = queueingFn(nodes2, expand(node2));
	}
}

Vertex* makeNode(State* state)
{
	Vertex* vertex = new Vertex();
	vertex->state = state;
	vertex->parentNode = nullptr;
	vertex->action = "";
	vertex->depth = 0;
	vertex->pathCost = 0;

	return vertex;
}

Vertex* makeNode(State* state, Vertex* parent, std::string action)
{
	Vertex* vertex = new Vertex();
	vertex->state = state;
	vertex->parentNode = parent;
	vertex->action = action;
	vertex->depth = parent->depth + 1;
	vertex->pathCost = parent->pathCost + 1;

	return vertex;
}

Vertex** makeQueue(Vertex* elements)
{
	std::string action[4];
	State** states = initSuccessor(elements->state, action);
	int i = 0;
	while (states[i])
	{
		i++;
	}
	Vertex** vertex = new Vertex * [i + 1];
	for (int j = 0; j < i; j++)
	{
		vertex[j] = makeNode(states[j], elements, action[j]);
		vertex[j + 1] = nullptr;
	}
	return vertex;
}

bool empty(Vertex** queue)
{
	if (*queue)
	{
		return false;
	}
	return true;
}

Vertex* removeFront(Vertex**& queue)
{
	Vertex* node = *queue;
	queue++;
	return node;
}

bool goalTest(State* state, Problem* problem)
{
	if (isEqualStates(state, &problem->GOAL_TEST))
	{
		return true;
	}
	return false;
}

Vertex** expand(Vertex* node)
{
	int counter = -1;
	std::string action[4];
	State** states = initSuccessor(node->state, action);
	int i = 0;
	while (states[i])
	{
		if (isRepeatState(node, states[i]))
		{
			for (int j = 0; states[i + j]; j++)
			{
				states[i + j] = states[i + j + 1];
			}
		}
		else i++;
	}
	Vertex** vertex = new Vertex * [i + 1];
	for (int j = 0; j < i; j++)
	{
		vertex[j] = makeNode(states[j], node, action[j]);
	}
	vertex[i] = nullptr;
	return vertex;
}

Vertex** queueingFn(Vertex** queue, Vertex** elements)
{
	return nullptr;
}

Vertex** enqueueAtEnd(Vertex** queue, Vertex** elements)
{
	int i = 0;
	while (elements[i])
	{
		i++;
	}
	int j = 0;
	while (queue[j])
	{
		j++;
	}

	Vertex** newQueue = new Vertex* [i + j + 1];
	int c = 0;
	for (; c < j; c++)
	{
		newQueue[c] = queue[c];
	}
	for (; c - j < i; c++)
	{
		newQueue[c] = elements[c - j];
	}
	newQueue[c] = nullptr;

	return newQueue;
}

Vertex** enqueueAtCost(Vertex** queue, Vertex** elements)
{
	int i = 0;
	while (elements[i])
	{
		i++;
	}
	int j = 0;
	while (queue[j])
	{
		j++;
	}

	Vertex** newQueue = new Vertex * [i + j + 1];
	int c = 0;
	int counter1 = 0, counter2 = 0;
	for (; c < i + j; c++)
	{
		while (!empty(&queue[counter1]) && !empty(&elements[counter2]))
		{
			if (queue[counter1]->pathCost <= elements[counter2]->pathCost)
			{
				newQueue[c] = queue[counter1];
				counter1++;
			}
			else
			{
				newQueue[c] = elements[counter2];
				counter2++;
			}
			c++;
		}
		if (counter1 == j)
		{
			newQueue[c] = elements[counter2];
			counter2++;
		}
		else
		{
			newQueue[c] = queue[counter1];
			counter1++;
		}
	}
	newQueue[c] = nullptr;

	return newQueue;
}

Vertex** enqueueAtFront(Vertex** queue, Vertex** elements)
{
	int i = 0;
	while (elements[i])
	{
		i++;
	}
	int j = 0;
	while (queue[j])
	{
		j++;
	}

	Vertex** newQueue = new Vertex * [i + j + 1];
	int c = 0;
	for (; c < i; c++)
	{
		newQueue[c] = elements[c];
	}
	for (; c - i < j; c++)
	{
		newQueue[c] = queue[c - i];
	}
	newQueue[c] = nullptr;

	return newQueue;
}

Vertex** enqueueAtFrontLimited(Vertex** queue, Vertex** elements, int depth)
{
	int i = 0;
	while (elements[i])
	{
		if (elements[i]->depth <= depth) i++;
		else break;
	}
	int j = 0;
	while (queue[j])
	{
		j++;
	}

	Vertex** newQueue = new Vertex * [i + j + 1];
	int c = 0;
	for (; c < i; c++)
	{
		newQueue[c] = elements[c];
	}
	for (; c - i < j; c++)
	{
		newQueue[c] = queue[c - i];
	}
	newQueue[c] = nullptr;

	return newQueue;
}

Vertex* breadthFirstSearch(Problem* problem)
{
	return generalSearch(problem, enqueueAtEnd);
}

Vertex* uniformCostSearch(Problem* problem)
{
	return generalSearch(problem, enqueueAtCost);
}

Vertex* depthFirstSearch(Problem* problem)
{
	return generalSearch(problem, enqueueAtFront);
}

Vertex* depthLimitedSearch(Problem* problem, int depth)
{
	return generalSearch(problem, enqueueAtFrontLimited, depth);
}

Vertex* iterativeDeepeningSearch(Problem* problem)
{
	for (int i = 0; true; i++)
	{
		Vertex* solution = depthLimitedSearch(problem, i);
		if (solution)
		{
			return solution;
		}
	}
}

Vertex** biDirectionalSearch(Problem* problem)
{
	return generalBiDirectionalSearch(problem, enqueueAtEnd);
}
