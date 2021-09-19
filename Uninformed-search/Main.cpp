#include <string>
#include <iostream>

struct RodState
{
	std::string rodState;
};

struct State
{
	RodState rodStates[3];
};

struct Problem
{
	State INITIAL_STATE = { "1LMS", "2", "3" };
	State** operation;
	State GOAL_TEST = { "1", "2", "3LMS" };
	int pathCost;
};

struct Vertex
{
	State* state;
	Vertex* parentNode;
	State* action;
	int depth;
	int pathCost;
	Vertex** childNode;
};

State** initSuccessor(State* initState);
void searchPossibleStates(State* initState, int index, State** newStates, int& n);
bool checkCondition(const char* ch1, const char* ch2);
bool isEqualStates(State* one, State* two);
void printSolve(Vertex* parent);

Vertex* generalSearch(Problem* problem, Vertex** (*queueingFn)(Vertex**, Vertex**));

Vertex* makeNode(State* state);
Vertex* makeNode(State* state, Vertex* parent, int count);
Vertex** makeQueue(Vertex* elements);
bool empty(Vertex** queue);
Vertex* removeFront(Vertex**& queue);
bool goalTest(State* state, Problem* problem);
Vertex** expand(Vertex* node, State* operators);
Vertex** queueingFn(Vertex** queue, Vertex** elements);
Vertex** enqueueAtEnd(Vertex** queue, Vertex** elements);

Vertex* breadthFirstSearch(Problem* problem);

int main()
{
	Problem* problem = new Problem();
	
	Vertex* solve = breadthFirstSearch(problem);

	printSolve(solve);
}

State** initSuccessor(State* initState)
{
	State** newStates = new State* [100];
	for (int i = 0; i < 100; i++)
	{
		newStates[i] = new State();
	}
	int n = 0;
	for (int i = 0; i < 3; i++)
	{
		std::string* str = &initState->rodStates[i].rodState;
		if ((*str).length() > 1)
		{
			searchPossibleStates(initState, i, newStates, n);
		}
	}
	return newStates;
}

void searchPossibleStates(State* initState, int index, State** newStates, int& n)
{
	for (int i = 0; i < 3; i++)
	{
		std::string* str1 = &(initState->rodStates[index]).rodState;
		std::string* str2 = &(initState->rodStates[i]).rodState;
		if (&initState->rodStates[index] != &initState->rodStates[i] &&
			checkCondition((*str1).substr((*str1).size() - 1).c_str(),
				(*str2).substr((*str2).size() - 1).c_str()))
		{
			*newStates[n++] = *initState;
			(*newStates[n - 1]).rodStates[i].rodState.push_back(*(*str1).substr((*str1).size() - 1).c_str());
			(*newStates[n - 1]).rodStates[index].rodState.pop_back();
		}
	}
}

bool checkCondition(const char* ch1, const char* ch2)
{
	if (*ch1 == 'S' ||
		(*ch1 == 'M' && *ch2 != 'S') ||
		(*ch1 == 'L' && *ch2 != 'S' && *ch2 != 'M'))
	{
		return true;
	}
	return false;
}

bool isEqualStates(State* one, State* two)
{
	if (one->rodStates[0].rodState == two->rodStates[0].rodState &&
		one->rodStates[1].rodState == two->rodStates[1].rodState)
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
		std::cout << parent->state->rodStates[i].rodState << " ";
	}
	std::cout << std::endl << "depth = " << parent->depth << std::endl;
}

Vertex* generalSearch(Problem* problem, Vertex** (*queueingFn)(Vertex**, Vertex**))
{
	Vertex** nodes = makeQueue(makeNode(&problem->INITIAL_STATE)); // создаём кайму
	Vertex* node;

	while (true)
	{
		for (int j = 0; nodes[j]; j++)
		{
			for (int i = 0; i < 3; i++)
			{
				std::cout << nodes[j]->state->rodStates[i].rodState << " ";
			}
			std::cout << std::endl << "depth = " << nodes[j]->depth << std::endl;
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
		nodes = queueingFn(nodes, expand(node, node->action));
	}
}

Vertex* makeNode(State* state)
{
	Vertex* vertex = new Vertex();
	vertex->state = state;
	vertex->parentNode = nullptr;
	vertex->action = nullptr;
	vertex->depth = 0;
	vertex->pathCost = 0;
	vertex->childNode = nullptr;

	return vertex;
}

Vertex* makeNode(State* state, Vertex* parent, int count)
{
	Vertex* vertex = new Vertex();
	vertex->state = state;
	vertex->parentNode = parent;
	vertex->action = parent->state;
	vertex->depth = parent->depth + 1;
	vertex->pathCost = parent->pathCost + 1;

	int i = 0;
	if (parent->childNode)
	{
		while (parent->childNode[i])
		{
			i++;
		}
	}
	else
	{
		parent->childNode = new Vertex* [count];
		for (int j = 0; j < count; j++)
		{
			parent->childNode[j] = nullptr;
		}
	}
	parent->childNode[i] = vertex;

	return vertex;
}

Vertex** makeQueue(Vertex* elements)
{
	State** states = initSuccessor(elements->state);
	int i = 0;
	while (states[i]->rodStates->rodState != "")
	{
		i++;
	}
	Vertex** vertex = new Vertex * [i + 1];
	for (int j = 0; j < i; j++)
	{
		vertex[j] = makeNode(states[j], elements, i);
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

Vertex** expand(Vertex* node, State* operators)
{
	int counter = 0;
	State** states = initSuccessor(node->state);
	int i = 0;
	while (states[i]->rodStates->rodState != "")
	{
		if (isEqualStates(node->action, states[i]) && counter != i)
		{
			counter = i;
			i--;
		}
		i++;
	}
	Vertex** vertex = new Vertex * [i];
	for (int j = 0; j < counter; j++)
	{
		vertex[j] = makeNode(states[j], node, i);
		vertex[j + 1] = nullptr;
	}
	for (int j = counter + 1; j < i; j++)
	{
		vertex[j - 1] = makeNode(states[j], node, i);
		vertex[j] = nullptr;
	}
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

Vertex* breadthFirstSearch(Problem* problem)
{
	return generalSearch(problem, enqueueAtEnd);
}
