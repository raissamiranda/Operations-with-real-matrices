# Operations with real matrices

This project performs fundamental operations on real matrices and evaluates the performance of the algorithm through the analysis of time and memory access pattern.

## Description

The data needed for operations is provided on the command line during program execution. The user must choose the operation among the three options -sum, multiplication and transposition- and the files in which the matrices to be processed are found.

## Execution

To perform addition, multiplication, transposition, time analysis and recording of memory acesses, the following command must be used:
```
> make mem
``` 

This command will generate 6 output files with the following content:
```
sumares.out: Result of the sum of matrices.
multres.out: Result of the multiplication between matrices.
transpres.out: Result of transposing matrix 1.
somalog.out:: Addition time and memory log.
multres.out:: Timestamp and multiplication memory.
transpres.out: Transposition time stamp and memory.
```
