#include "parameters.h"

#include <iostream>
#include <functional>
#include <vector>

#include "include/cxxopts/cxxopts.hpp"

Parameter PARAMETER = Parameter();

template<typename T>
std::string to_string(const T& value) { return std::to_string(value); }

std::string to_string(const std::string& value) { return value; }

struct ParameterParser {
public:
	ParameterParser() {
		options.add_options()("h,help", "Print usage");
	}

	void parse(int argc, char** argv) {
		auto args = options.parse(argc, argv);
		
		if (args.count("help")) {
			std::cout << options.help() << '\n';
			exit(0);
		}

		for (const auto& func: parse_callbacks) {
			func(args);
		}
	}

	template<typename T>
	void add(const std::string& switches,
	         const std::string& description,
	         T& arg) {
		options.add_options()(switches, description, cxxopts::value<T>()->default_value(to_string(arg)));

		auto position = switches.find(',');
		std::string flag = ((position != std::string::npos) ? switches.substr(0, position) : switches);

		parse_callbacks.emplace_back([&arg, flag] (cxxopts::ParseResult& args) {
			arg = args[flag].as<T>();
		});
	}

private:
	cxxopts::Options options{"LetterC67", "RRAP for mTSP"};
	std::vector<std::function<void (cxxopts::ParseResult&)>> parse_callbacks;
};

Parameter parse_parameter_options(int argc, char* argv[]) {
	Parameter parameter;
	ParameterParser parser;

	parser.add("b,beta", "The relative influence of the pheromone intensity and the heuristic information", parameter.SMMAS_PARAMETER.BETA);
	parser.add("rho-base", "The evaporation base rate of the pheromone", parameter.SMMAS_PARAMETER.RHO_BASE);
	parser.add("rho-coef", "The addition of the evaporation coefficient rate of the pheromone", parameter.SMMAS_PARAMETER.RHO_COEFF);
	parser.add("a,ants", "The number of ants", parameter.SMMAS_PARAMETER.LAMBDA);
	parser.add("ratio", "The addition ratio on tau_min", parameter.SMMAS_PARAMETER.RATIO);

	parser.add("p,population", "The size of the population", parameter.MU);
	parser.add("d,delta", "The diversity penalty on individuals", parameter.DELTA);
	parser.add("max-rnr", "The maximum rate of the Ruin and Recreate", parameter.MAXIMUM_RUIN_AND_RECREATE_RATE);

	parser.add("a-nearness", "The a-nearness in 1-tree", parameter.ALPHA);
	parser.add("n-nearness", "The n-nearness when constructing solutions", parameter.N_NEAREST);
	parser.add("eax-tsp", "The iteration starts using EAX-TSP", parameter.RUN_TSP_THRESHOLD);
	
	parser.add("s,salesmen", "The number of salesmen", parameter.SALESMEN);
	parser.add("iterations", "The number of iterations that RRAP will run", parameter.CUTOFF_ITERATIONS);
	parser.add("time", "The cutoff time that RRAP will run", parameter.CUTOFF_TIME);
	parser.add("output-path", "The path to store results", parameter.OUTPUT_PATH);
	parser.add("i,instance", "The name of the instance that RRAP will solve", parameter.INSTANCE_PATH);
	parser.add("alg", "The name of the algorithm (RRAP, RRAP2, PACO)", parameter.ALGORITHM_NAME);
	parser.add("repeat", "The number of times that RRAP will execute on instances", parameter.NUM_RUNS);
	parser.add("t,thread", "The number of threads  that RRAP will use to execute on instances", parameter.NUM_THREADS);
	parser.add("seed", "The random number generator seed", parameter.SEED);

	parser.parse(argc, argv);
	return parameter;
}

const int DEPOT = 0;
const int OPT_SIZE = 4; // Use for or-opt