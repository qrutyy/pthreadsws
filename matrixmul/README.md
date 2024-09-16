# Matrix Mutliplication

This project implements mutlithreaded mutliplication of matrix and vector-column. For computing, the simplest multiplication principle was used.

## Usage

**Initialization:**
```bash
mv ../Makefile .
make file=matrixmultiplication.c
```

**Running with custom args:**
```bash
./matrixmultiplication --n=x --threadnum=y
```
*Where x - matrix dimension, threadnum - number of threads ;)*

**Testing using the script + plot generation**
```bash
python3 testnplots.py
```

*So far - to modify the testing script - edit it explicitly. 
Implicit args will be added in future release.*

## Plots and  Performance Analysis: Execution Time vs Thread Count & Matrix Dimension

The [*plots*](https://github.com/qrutyy/uthreadsworkshop/tree/main/matrixmul/plots) generated in this performance analysis provide insight into how matrix multiplication scales with different matrix dimensions and varying numbers of threads. The experiment* was conducted using matrix sizes ranging from 100 to 10,000 elements, with thread counts from 1 to 50.

#### Plot 1: Execution Time vs Matrix Dimension for Different Thread Counts

- **Overview**: This plot shows the execution time for matrix multiplication as a function of matrix dimension (`n`) across varying numbers of threads (`threadnum`). Each curve represents the performance for a different number of threads, where:
  - **X-axis**: Matrix Dimension (`n`) — The size of the square matrix (i.e., `n x n`).
  - **Y-axis**: Execution Time (seconds) — The time taken to complete the matrix multiplication.
  - **Lines and Markers**: Each thread count has a distinct line with points representing the average execution time over multiple trials (e.g., 10 repetitions).

- **Insights**:
  - As matrix size increases, the execution time grows significantly, especially for lower thread counts.
  - Higher thread counts generally reduce the execution time, but the performance gains diminish for thread counts > 4 (e.g., 64 and 128 threads). See [Conclusion](#conclusion).

#### Plot 2: Execution Time vs Thread Count for Each Matrix Dimension

- **Overview**: For each matrix dimension (`n`), this plot shows how execution time changes as the number of threads increases. Each matrix dimension has its own plot, where:
  - **X-axis**: Number of Threads (`threadnum`) — The number of threads used for parallel computation.
  - **Y-axis**: Execution Time (seconds) — The time taken to complete the matrix multiplication.
  - **Lines and Markers**: Each thread count is represented by a series of points, with lines connecting them to show trends across the number of threads.

- **Insights**:
  - For smaller matrix dimensions (e.g., `n = 100`), adding threads can improve performance, but after a certain point (e.g., 4-16 threads), the benefits diminish.
  - For larger matrices (e.g., `n = 10,000`), the use of more threads significantly improves performance up to a point. However, at higher thread counts (50+), performance gains are minimal or may even regress due to factors like thread management overhead.
  - These plots help identify the optimal number of threads for different matrix sizes, showing where parallelism is most effective.

### Notes about setup

As for setup was used simple compilation using GCC without any optimizations (see [Makefile](https://github.com/qrutyy/uthreadsworkshop/tree/main/Makefile)). Granularity of the multiplication operation is 'fine-grained' and equal (`matrixdim // threadnum`).

## Conclusion

Analysing the plots we can note a pretty interesting behaviour. Execution time, at some point, stops being directly depended on the number of threads. 

In parallel computing there is a limit of performance optimization that is described in [*Amdahls law*](https://en.wikipedia.org/wiki/Amdahl%27s_law), as mentioned [*here*](https://unix.stackexchange.com/a/82476) by *DavAlPi* and *Revious*, that especially explains this behaviour. 

By adding more threads we're just making a huge overhead for CPU, spending time on context switching and runnning into memory speed limit.

Another thing you have to realise - having more threads than CPU has cores - just triggers the serialization process. 

Last thing that i want to mention - when using ***more than 10 threads***, a decrease in performance can be observed. This happens because threads require additional resources: they need to be synchronized, switched, and the workload needs to be balanced. Once the number of threads exceeds the available CPU cores, the overhead of managing them can outweigh the benefits of parallelism, leading to reduced performance.

## License

Content of link to *Stack Exchange* in conclusion is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg

