import subprocess
import matplotlib.pyplot as plt
import os
import re

program_path = './mvmultiplication'
n_values = [100, 200, 500, 1000, 2000, 3000, 4000, 5000, 7500, 10000]
threadnum_values = [1, 2, 4, 10, 25, 50]
repetitions = 50  # Number of repetitions for each test
execution_times = {}

output_dir = 'plots'
os.makedirs(output_dir, exist_ok=True)


def run_test(n, threadnum):
    command = [program_path, f'--n={n}', f'--threadnum={threadnum}']
    all_times = []
    for _ in range(repetitions):
        try:
            result = subprocess.run(command, check=True, text=True, capture_output=True)
            output = result.stdout
            times = re.findall(r"n = \d+, threadnum = \d+, time = ([\d.]+) seconds", output)
            all_times.extend([float(time) for time in times])
            # print(f"Test completed. n = {n}, threadnum = {threadnum}, time = {times}")
        except subprocess.CalledProcessError:
            print(f"Test failed for n = {n}, threadnum = {threadnum}")
    return all_times


# Collect execution times
for n in n_values:
    execution_times[n] = {}
    for threadnum in threadnum_values:
        times = run_test(n, threadnum)
        if times:
            execution_times[n][threadnum] = times
            print(f"Finished test with n = {n}, threadnum = {threadnum}, times = {times}")
        else:
            execution_times[n][threadnum] = [None] * repetitions

# Plot execution time vs matrix dimension for each thread count
plt.figure(figsize=(12, 8))
for threadnum in threadnum_values:
    all_times = []
    all_n = []
    for n in n_values:
        if threadnum in execution_times[n]:
            times = execution_times[n][threadnum]
            all_times.extend(times)
            all_n.extend([n] * len(times))  # Repeated n for each trial
    plt.plot(all_n, all_times, marker='o', linestyle='-', label=f'Threadnum = {threadnum}')

plt.xlabel('Matrix Dimension (n)')
plt.ylabel('Execution Time (seconds)')
plt.title('Execution Time vs Matrix Dimension for Different Thread Counts')
plt.legend(title="Thread Counts")
plt.grid(True)
plot_filename = os.path.join(output_dir, 'execution_time_vs_threads_all_dimensions.png')
plt.savefig(plot_filename)
plt.close()

# Plot execution time vs number of threads for each matrix dimension separately
for n in n_values:
    plt.figure(figsize=(12, 8))
    for threadnum in threadnum_values:
        if threadnum in execution_times[n]:
            times = execution_times[n][threadnum]
            plt.plot([threadnum] * len(times), times, marker='o', linestyle='-', label=f'Threadnum = {threadnum}')

    plt.xlabel('Number of Threads')
    plt.ylabel('Execution Time (seconds)')
    plt.title(f'Execution Time vs Number of Threads for Matrix Dimension n = {n}')
    plt.legend(title="Thread Counts")
    plt.grid(True)
    plot_filename = os.path.join(output_dir, f'execution_time_vs_threads_n_{n}.png')
    plt.savefig(plot_filename)
    plt.close()

print(f"Plots saved to {output_dir}")
