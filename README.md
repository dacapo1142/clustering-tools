# clustering-tools

## Requirements
- GCC 5+
- cmake 2.8

## Compile
```console
cmake src
make -C src
```

## Fast Unfolding Algorithm

### Usage
```console
$ python fast_unfolding.py -h
Perform the fast unfolding algorithm over a bivariate distribution.

positional arguments:
  INPUT                 the input file that contains the bivariate
                        distribution
  N                     the number of vertices in the graph
  OUTPUT                the output file to store the detected communities

optional arguments:
  -h, --help            show this help message and exit
  -t FILE, --time FILE  record the consumption time to FILE
  -s FILE, --size FILE  record the number of vertices in each round to FILE
  -i FILE, --iter FILE  record the number of iterations in each round to FILE
  -3c, --three_column   use three-column formation for bivariate distribution
```

## Examples