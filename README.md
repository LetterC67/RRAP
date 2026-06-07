# Ruin and Recreate Ant Population for Min-max Multiple Travelling Salesmen Problem
This repository contains the implementation for the Ruin and Recreate Ant Population (RRAP) algorithm to solve the Min-max Multiple Travelling Salasmen Problem (Min-max mTSP).

## Usage
First compile the program:
```
bash compile.sh
```

You can run the executable with `help` flag to display the usage:
```
Usage: ./RRAP [options]
  -h, --help        Show this help message
  -b, --beta        The relative influence of the pheromone intensity and the heuristic information
      --rho-base    The evaporation base rate
      --rho-coeff   The addition of the evaporation
      --ratio       The addition ratio on tau_min
  -a, --ants        The number of ants
  -p, --population  The size of the population
  -d, --delta       The diversity penalty on individuals
      --max-rnr     Maximum rate of the Ruin and Recreate
      --a-nearness  The a-nearness in 1-tree
      --n-nearness  
  -i, --instance    Path to the dataset
  -s, --salesmen    The number of salesmen
  -i, --iterations  Cutoff iteration
      --time        Cutoff time in second (s)
  -t, --thread      The number of CPU threads
      --seed        The random number generator seed
      --repeat      The number of run
      --alg         The name of the algorithm (RRAP, RRAP2, PACO)

Dataset and salesmen are required.
If neither the cutoff time nor the iteration is specified, the program will default to using a cutoff of (n / 100) * 4 minutes. If both are specified, then both will be used as stopping condition.
If the number of run is not specified, the program will default to running 1 time.
```

The dataset are located in `dataset` folder. To solve the `pcb3038` instance with 10 salesmen, using 4 CPU threads with 2000 iterations cutoff, run:
```
./RRAP --instance ./dataset/pcb3038.tsp --salesmen 10 --iterations 2000 --thread 4
```

## Acknowledgment
This implementation incorporates EAX-TSP code sourced from the repository available at: https://github.com/sugia/GA-for-TSP