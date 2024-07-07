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
  -d, --dateset     Path to the dataset
  -s, --salesmen    The number of salesmen
  -t, --time        Cutoff time in second(s)
  -i, --iterations  Cutoff iteration
  -t, --threads     The number of CPU threads
  -r, --run         The number of run
  -v, --variation   Variation

Dataset and salesmen are required.
If neither the cutoff time nor the iteration is specified, the program will default to using a cutoff of 1000 iterations. If both are specified, then both will be used as stopping condition.
If the number of run is not specified, the program will default to running 30 times.
```

The dataset are located in `dataset` folder. To solve the `pcb3038` instance with 10 salesmen, using 4 CPU threads with 2000 iterations cutoff, run:
```
./RRAP --dataset=./dataset/pcb3038.tsp --salesmen=10 --iterations=2000 --threads=4
```