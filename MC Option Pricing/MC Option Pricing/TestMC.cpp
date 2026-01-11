// Author: Ben Kovalick
// Purpose: Provides the implementation point for the MC option pricing program.

#define _SCL_SECURE_NO_WARNINGS

#include <vector>
#include <memory>
#include <iostream>
#include <random>
#include <tuple>
#include <chrono>

// Pricer classes
#include "Pricer.hpp"
#include "EuropeanPricer.hpp"
#include "AsianPricer.hpp"
#include "BarrierPricer.hpp"

// Builder/Mediator Classes
#include "MCBuilder.hpp"
#include "MCMediator.hpp"

// Option Data Struct
#include "OptionData.hpp"

int main()
{
	// 1. Initialise the option data
	// Batch 1
	OptionData myOption((OptionParams::strike = 65.0, OptionParams::expiration = 0.25,
		OptionParams::volatility = 0.30, OptionParams::dividend = 0.0,
		OptionParams::optionType = 1, OptionParams::interestRate = 0.08,
		OptionParams::beta = 1, OptionParams::initialPrice = 60.0));

	// Put
	OptionData myOption2((OptionParams::strike = 65.0, OptionParams::expiration = 0.25,
		OptionParams::volatility = 0.30, OptionParams::dividend = 0.0,
		OptionParams::optionType = -1, OptionParams::interestRate = 0.08,
		OptionParams::beta = 1, OptionParams::initialPrice = 60.0));

	// Batch 2
	//OptionData myOption((OptionParams::strike = 100.0, OptionParams::expiration = 1.0,
	//	OptionParams::volatility = 0.20, OptionParams::dividend = 0.0,
	//	OptionParams::optionType = 1, OptionParams::interestRate = 0.0,
	//	OptionParams::beta = 1, OptionParams::initialPrice = 100.0));

	// Put
	//OptionData myOption2((OptionParams::strike = 100.0, OptionParams::expiration = 1.0,
	//	OptionParams::volatility = 0.20, OptionParams::dividend = 0.0,
	//	OptionParams::optionType = -1, OptionParams::interestRate = 0.0,
	//	OptionParams::beta = 1, OptionParams::initialPrice = 100.0));

	// Batch 3
	//OptionData myOption((OptionParams::strike = 10.0, OptionParams::expiration = 1.0,
	//	OptionParams::volatility = 0.50, OptionParams::dividend = 0.0,
	//	OptionParams::optionType = 1, OptionParams::interestRate = 0.12,
	//	OptionParams::beta = 1, OptionParams::initialPrice = 5.0));

	// Put
	//OptionData myOption2((OptionParams::strike = 10.0, OptionParams::expiration = 1.0,
	//	OptionParams::volatility = 0.50, OptionParams::dividend = 0.0,
	//	OptionParams::optionType = -1, OptionParams::interestRate = 0.12,
	//	OptionParams::beta = 1, OptionParams::initialPrice = 5.0));

	// Batch 4
	//OptionData myOption((OptionParams::strike = 100.0, OptionParams::expiration = 30.0,
	//	OptionParams::volatility = 0.30, OptionParams::dividend = 0.0,
	//	OptionParams::optionType = 1, OptionParams::interestRate = 0.08,
	//	OptionParams::beta = 1, OptionParams::initialPrice = 100.0));

	//// Put
	//OptionData myOption2((OptionParams::strike = 100.0, OptionParams::expiration = 30.0,
	//	OptionParams::volatility = 0.30, OptionParams::dividend = 0.0,
	//	OptionParams::optionType = -1, OptionParams::interestRate = 0.08,
	//	OptionParams::beta = 1, OptionParams::initialPrice = 100.0));

	// 2. Choose the builder that creates the necessary components
	int choice = 1;
	auto parts = MCBuilder::ChooseBuilder(choice, myOption);

	// 3. Create the mediator (SUD)
	int NSim = 1000000;
	std::cout << "How many simulations? "; std::cin >> NSim;
	MCMediator mcp(parts, NSim);

	// 4. Create the payoff functions -> Can use this payoff function or the one inherent in the OptionData struct
	auto payoff = [&myOption](double x) {
		if (myOption.type == 1)
		{
			return std::max(0.0, x - myOption.K);
		}
		else
		{
			return std::max(0.0, myOption.K - x);
		}
	};

	auto discounter = [&myOption]() {return std::exp(-myOption.r * myOption.T); };

	// 5. Create prices and link them to the mediator.
	std::shared_ptr<Pricer> op = std::make_shared<EuropeanPricer>(std::bind(&OptionData::myPayOffFunction, myOption, std::placeholders::_1), discounter);
	std::shared_ptr<Pricer> op2 = std::make_shared<EuropeanPricer>(std::bind(&OptionData::myPayOffFunction, myOption2, std::placeholders::_1), discounter);
	std::shared_ptr<Pricer> op3 = std::make_shared<AsianPricer>(std::bind(&OptionData::myPayOffFunction, myOption2, std::placeholders::_1), discounter);
	std::shared_ptr<Pricer> op4 = std::make_shared<BarrierPricer>(std::bind(&OptionData::myPayOffFunction, myOption2, std::placeholders::_1), discounter);

	// Define slots for path information.
	mcp.path.connect(0, std::bind(&Pricer::ProcessPath, op, std::placeholders::_1));
	mcp.path.connect(1, std::bind(&Pricer::ProcessPath, op2, std::placeholders::_1));
	mcp.path.connect(2, std::bind(&Pricer::ProcessPath, op3, std::placeholders::_1));
	mcp.path.connect(3, std::bind(&Pricer::ProcessPath, op4, std::placeholders::_1));

	// Signal end of simulation.
	mcp.finish.connect(0, std::bind(&Pricer::PostProcess, op));
	mcp.finish.connect(1, std::bind(&Pricer::PostProcess, op2));
	mcp.finish.connect(2, std::bind(&Pricer::PostProcess, op3));
	mcp.finish.connect(3, std::bind(&Pricer::PostProcess, op4));

	// 6. Run the program and examine the output.
	auto start = std::chrono::system_clock::now();
	
	auto fn1 = [&]() { mcp.start(); }; // Uncomment to run single-threaded
	auto fn2 = [&]() {mcp.startPPL(); }; // Uncomment to run multi-threaded using PPL.
	auto fn3 = [&]() {mcp.startOpenMP();}; // Uncomment to run multi-threaded using OpenMP.

	std::thread t1(fn1);
	std::thread t2(fn2);
	std::thread t3(fn3);

	t1.join();
	t2.join();
	t3.join();

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "Total Computation time: " << elapsed_seconds.count() << std::endl;

	system("pause");
	return 0;
}