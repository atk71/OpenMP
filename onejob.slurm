#! /bin/bash
#SBATCH --time=00:10:00 
#SBATCH -N 2 -n 4 -c 4
#SBATCH -p class # GPU partition
#SBATCH --gres=gpu:2
#SBATCH -A sxg125_csds438 
#SBATCH --mem=4gb # Assign 4gb memory

module load gcc

gcc -o mergesort-omp -fopenmp merge/mergesort-omp.c
gcc -o quicksort-omp -fopenmp quick/quicksort-omp.c

echo mergesort nearly sorted
./mergesort-omp testfiles/nearly_sorted/nearly1000.txt
./mergesort-omp testfiles/nearly_sorted/nearly10000.txt
./mergesort-omp testfiles/nearly_sorted/nearly100000.txt
./mergesort-omp testfiles/nearly_sorted/nearly500000.txt
./mergesort-omp testfiles/nearly_sorted/nearly1000000.txt

echo mergesort random order
./mergesort-omp testfiles/random_order/rand1000.txt
./mergesort-omp testfiles/random_order/rand10000.txt
./mergesort-omp testfiles/random_order/rand100000.txt
./mergesort-omp testfiles/random_order/rand500000.txt
./mergesort-omp testfiles/random_order/rand1000000.txt

echo mergesort reverse order
./mergesort-omp testfiles/reverse_order/reverse1000.txt
./mergesort-omp testfiles/reverse_order/reverse10000.txt
./mergesort-omp testfiles/reverse_order/reverse100000.txt
./mergesort-omp testfiles/reverse_order/reverse500000.txt
./mergesort-omp testfiles/reverse_order/reverse1000000.txt

echo quicksort nearly sorted
./quicksort-omp testfiles/nearly_sorted/nearly1000.txt
./quicksort-omp testfiles/nearly_sorted/nearly10000.txt
./quicksort-omp testfiles/nearly_sorted/nearly100000.txt
./quicksort-omp testfiles/nearly_sorted/nearly500000.txt
./quicksort-omp testfiles/nearly_sorted/nearly1000000.txt

echo quicksort random order
./quicksort-omp testfiles/random_order/rand1000.txt
./quicksort-omp testfiles/random_order/rand10000.txt
./quicksort-omp testfiles/random_order/rand100000.txt
./quicksort-omp testfiles/random_order/rand500000.txt
./quicksort-omp testfiles/random_order/rand1000000.txt

echo quicksort reverse order
./quicksort-omp testfiles/reverse_order/reverse1000.txt
./quicksort-omp testfiles/reverse_order/reverse10000.txt
./quicksort-omp testfiles/reverse_order/reverse100000.txt
./quicksort-omp testfiles/reverse_order/reverse500000.txt
./quicksort-omp testfiles/reverse_order/reverse1000000.txt