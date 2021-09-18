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

//struct Successor
//{
//	State newStates;
//};

struct PathCost
{
	int pathCost;
};

struct Problem
{
	State INITIAL_STATE = { "1LMS", "2", "3" };
	State** operation;
	State GOAL_TEST = { "1", "2", "3LMS" };
	PathCost pathCost;
};

//struct Vertex
//{
//
//};

State** initSuccessor(State* initState);
void searchPossibleStates(State* initState, int index, State** newStates, int& n);
bool checkCondition(const char* ch1, const char* ch2);

int main()
{
	Problem* problem = new Problem();
	State* initState = new State();
	*initState = problem->INITIAL_STATE;

	problem->operation = initSuccessor(initState);
	for (int i = 0; problem->operation[i]->rodStates[0].rodState != ""; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::cout << problem->operation[i]->rodStates[j].rodState << " ";
		}
		std::cout << std::endl;
	}

	problem->operation = initSuccessor(problem->operation[0]);
	for (int i = 0; problem->operation[i]->rodStates[0].rodState != ""; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::cout << problem->operation[i]->rodStates[j].rodState << " ";
		}
		std::cout << std::endl;
	}
}

State** initSuccessor(State* initState)
{
	State** newStates = new State * [100];
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