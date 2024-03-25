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
- RawRep
- RawRepT
- ConRep
- ConRepT
- OptRaw
- OptCon
- OptConH
- OptRep
- OptRepH

## Datasets
- Reference-Sim
    - refsim
- Catfish
    - human
    - mouse
    - salmon
    - zebrafish
    - simulation
