#pragma warning(disable : 4996)
#include "libraries.h"
#include "FundamentalConstants.h"

#include "ElementarElement.h"
#include "Simulation.h"


int main()
{

    int N;

    std::cout << "Simulation Starts!" << std::endl;
    std::cout << "Enter the required number of elements" << std::endl;

    std::cin >> N;

    Simulation solution(N);
    solution.WindowInteraction();

    return 0;
}