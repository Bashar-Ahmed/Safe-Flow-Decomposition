# Improved Safe Flow Decomposition Algorithms for RNA Assembly

## Compile
```
mkdir build
cd build
cmake ..
cmake --build .
```

## Running Algorithms
```
./build/[ALGORITHM_NAME] [INPUT_FILE] [STORE_BOOL] [PRINT_BOOL]
```

## Running Tests
- All Tests
```
cd build
ctest
```
- For particular algorithm
```
cd build
ctest -R [ALGORITHM_NAME] 
```
- For particular dataset
```
cd build
ctest -R [DATASET_NAME] 
```

## Algorithms  
[ALGORITHM_NAME] ([ALGORITHM_NAME_AS_MENTIONED_IN_PAPER])
- RawRep (RawRep<sub>o</sub>)
- RawRepT (RawRep)
- ConRep (ConRep<sub>o</sub>)
- ConRepT (ConRep)
- OptRaw (OptRawRep)
- OptCon (OptConRep)
- OptConH (OptConRep<sup>#</sup>)
- OptRep (OptRep)
- OptRepH (OptRep<sup>#</sup>)

## Datasets
- Reference-Sim
    - refsim
- Catfish
    - human
    - mouse
    - salmon
    - zebrafish
    - simulation
 - Randomly Generated
    - Uniform Random
    - Power Law
    - Improved Random 
