#!/bin/bash

# Directory containing the source files
SOURCE_DIR="./src"
BUILD_DIR="./build"

TEST_LENGTHTS_ALL=(100 1000 10000)

#Compile and run each file in the directory
for file in "$SOURCE_DIR"/*.cpp; do
    if [[ -f "$file" ]]; then
        # Compile the file
        filename=$(basename -- "$file")
        printf "Compiling file: $filename\n"
        if [[ "$file" == *"omp"* ]]; then
            g++-13 -std=c++20 -fopenmp -O3 "$file" -o "$BUILD_DIR"/"${filename%.*}".x
        else
            g++ -std=c++20 -pthread -O3 "$file" -o "$BUILD_DIR"/${filename%.*}.x
        fi
    fi
done

for lenght in "${TEST_LENGTHTS_ALL[@]}"; do
    printf "Testing with lenght: $lenght\n"

    python3 generator.py 20 "$lenght" > test.in
    for file in "$BUILD_DIR"/*.x; do
        filename=$(basename -- "$file")
        filename=${filename%.*}
        if [[ -f "$file" ]]; then
            echo "Testing file: $filename"
            ./"$file" < test.in > output/"$lenght"_"$filename".out
        fi
    done
done

TEST_LENGTHTS_BEST=(20000 40000 80000 160000)
BEST=("build/Hirschberg_threads.x" "build/LCS_threads3_1.x" "build/LCS_threads3_2.x" "build/LCS_omp3.x")

for lenght in "${TEST_LENGTHTS_BEST[@]}"; do
    printf "Testing with lenght: $lenght\n"

    python3 generator.py 5 "$lenght" > test.in
    for file in ${BEST[@]}; do
        filename=$(basename -- "$file")
        filename=${filename%.*}

        if [[ -f "$file" ]]; then
            echo "Testing file: $filename"
            ./"$file" < test.in > output/"$lenght"_"$filename".out
        fi
    done
done
