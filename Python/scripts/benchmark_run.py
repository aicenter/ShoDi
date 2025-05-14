#
# MIT License
#
# Copyright (c) 2024 Czech Technical University in Prague
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.#
import os
import subprocess
import pandas

PREPROCESSOR_EXECUTABLE = "./shortestPathsPreprocessor"
BENCHMARK_EXECUTABLE = "./benchmark"
RUNS = 10
INSTANCE = "DC"

def compute_structures():
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "-m", "ch", "-i", f"../thesisTestsData/{INSTANCE}", "-o", f"../thesisTestsData/{INSTANCE}/{INSTANCE}"])
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "-m", "tnr", "--preprocessing-mode", "slow", "--tnodes-cnt", "5000", "-i", f"../thesisTestsData/{INSTANCE}", "-o", f"../thesisTestsData/{INSTANCE}/{INSTANCE}5000tnodes", "--int-size", "16"])
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "-m", "tnraf", "--preprocessing-mode", "slow", "--tnodes-cnt", "5000", "-i", f"../thesisTestsData/{INSTANCE}", "-o", f"../thesisTestsData/{INSTANCE}/{INSTANCE}5000tnodes", "--int-size", "16"])
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "-m", "dm", "--preprocessing-mode", "fast", "-i", f"../thesisTestsData/{INSTANCE}", "-o", f"../thesisTestsData/{INSTANCE}/{INSTANCE}", "--output-format", "hdf", "--int-size", "16"])
    print("computing structures finished")


# returns memory usage for the method in MiB, average time per query in microseconds and a list of computed distances
def run_benchmark(method, input_structure, queries_file, reference_distances=None):
    with open(queries_file, "r") as file:
        # the first line of the file contains number of queries
        queries_count = int(file.readline())

    total_time = 0
    total_mem = 0
    out_values_set = set()
    for i in range(RUNS):
        out_file = f"out_{method}.txt"
        command = [BENCHMARK_EXECUTABLE, "-m", method,
            "--query-set", queries_file,
            "--input-structure", input_structure,
            "--output-path", out_file]
        process = subprocess.Popen(command, preexec_fn=os.setsid)
        print(" ".join(command))
        process.communicate()

        with open("benchmark.txt", "r") as f:
            time, mem = f.readlines()
            total_time += float(time.strip())
            total_mem += int(mem.strip())
        os.remove("benchmark.txt")
    
        with open(out_file, "r") as f:
            out_values_set.add([int(_.strip()) for _ in f.readlines()[1:]])
        
        if len(out_values_set) > 1:
            print(f"WARNING: {method} returned different distances across multiple runs on the same query set!")
    
    mem_mb = total_mem/(RUNS*1024)
    query_time_us = (total_time*1_000_000)/(RUNS*queries_count)
    out_values = list(out_values_set)[0]

    if reference_distances is not None and out_values != reference_distances:
        print(f"WARNING: {method} returned different distances than Dijkstra's algorithm!")

    return mem_mb, query_time_us, out_values

compute_structures()

queries = f"../thesisTestsData/{INSTANCE}/{INSTANCE}100000randomQueries.txt"
df_data = []

# keep Dijkstra-computed distances and compare with other methods to confirm that the results are identic
mem, time, dijkstra_distances = run_benchmark("dijkstra", f"../thesisTestsData/{INSTANCE}", queries)
df_data.append(["Dijkstra", mem, time])

mem, time, distances = run_benchmark("astar", f"../thesisTestsData/{INSTANCE}", queries, dijkstra_distances)
df_data.append(["A*", mem, time])

mem, time, distances = run_benchmark("ch", f"../thesisTestsData/{INSTANCE}/{INSTANCE}.ch", queries, dijkstra_distances)
df_data.append(["CH", mem, time])

mem, time, distances = run_benchmark("tnr", f"../thesisTestsData/{INSTANCE}/{INSTANCE}5000tnodes.tnrg", queries, dijkstra_distances)
df_data.append(["TNR", mem, time])

mem, time, distances = run_benchmark("tnraf", f"../thesisTestsData/{INSTANCE}/{INSTANCE}5000tnodes.tgaf", queries, dijkstra_distances)
df_data.append(["TNRAF", mem, time])

mem, time, distances = run_benchmark("dm", f"../thesisTestsData/{INSTANCE}/{INSTANCE}.hdf5", queries, dijkstra_distances)
df_data.append(["DM", mem, time])

df = pandas.DataFrame(df_data, columns=["label", "memory", "time"])

df.to_csv(f"benchmark_{INSTANCE}.csv")
