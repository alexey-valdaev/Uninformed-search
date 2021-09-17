#include <string>
#include <iostream>

struct State
{
	std::string state;
};

struct Successor
{
	State stateVariation[3];
};

struct PathCost
{
	int pathCost;
};

struct Problem
{
	State INITIAL_STATE[3] = {"1LMS", "2", "3"};
	State** operation;
	State GOAL_TEST[3] = {"1", "2", "3LMS"};
	PathCost pathCost;
};

//struct Vertex
//{
//
//};

State** initSuccessor(State** initState);
void searchPossibleStates(State** initState, State* thisState, State** newStates, int n);
bool checkCondition(const char* ch1, const char* ch2);

int main()
{
	Problem* problem = new Problem();
	State** initState = new State*[3];
	initState[0] = new State();
	initState[0]->state = problem->INITIAL_STATE[0].state;
	initState[1] = new State();
	initState[1]->state = problem->INITIAL_STATE[1].state;
	initState[2] = new State();
	initState[2]->state = problem->INITIAL_STATE[2].state;

	problem->operation = initSuccessor(initState);
	for (int i = 0; problem->operation[i]->state != ""; i++)
	{
		std::cout << problem->operation[i]->state << std::endl;
	}

	delete[] initState;
}

State** initSuccessor(State** initState)
{
	State** newStates = new State*[100];
	for (int i = 0; i < 100; i++)
	{
		newStates[i] = new State();
	}
	int n = 0;
	for (int i = 0; i < 3; i++)
	{
		std::string* str = &(*initState[i]).state;
		if ((*str).length() > 1)
		{
			searchPossibleStates(initState, initState[i], newStates, n);
		}
	}
	return newStates;
}

void searchPossibleStates(State** initState, State* thisState, State** newStates, int n)
{
	for (int i = 0; i < 3; i++)
	{
		std::string* str1 = &(*thisState).state;
		std::string* str2 = &(*initState[i]).state;
		if (thisState != initState[i] &&
			checkCondition((*str1).substr((*str1).size() - 1).c_str(),
				(*str2).substr((*str2).size() - 1).c_str()))
		{
			*newStates[n++] = *thisState;
			*newStates[n] = *initState[i];
			(*newStates[n++]).state.push_back(*(*str1).substr((*str1).size() - 1).c_str());
			(*newStates[n - 2]).state.pop_back();
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