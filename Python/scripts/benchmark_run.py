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
RUNS = 10
INSTANCE = "DC"

def compute_structures():
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "create", "-m", "ch", "-i", f"../thesisTestsData/{INSTANCE}", "-o", f"../thesisTestsData/{INSTANCE}/{INSTANCE}"])
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "create", "-m", "tnr", "--preprocessing-mode", "slow", "--tnodes-cnt", "5000", "-i", f"../thesisTestsData/{INSTANCE}", "-o", f"../thesisTestsData/{INSTANCE}/{INSTANCE}5000tnodes", "--int-size", "16"])
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "create", "-m", "tnraf", "--preprocessing-mode", "slow", "--tnodes-cnt", "5000", "-i", f"../thesisTestsData/{INSTANCE}", "-o", f"../thesisTestsData/{INSTANCE}/{INSTANCE}5000tnodes", "--int-size", "16"])
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "create", "-m", "dm", "--preprocessing-mode", "fast", "-i", f"../thesisTestsData/{INSTANCE}", "-o", f"../thesisTestsData/{INSTANCE}/{INSTANCE}", "--output-format", "hdf", "--int-size", "16"])
    print("computing structures finished")


# returns memory usage for the method in MiB and average time per query in microseconds
def run_benchmark(method, input_structure, queries_file):
    with open(queries_file, "r") as file:
        # the first line of the file contains number of queries
        queries_count = int(file.readline())

    total_time = 0
    total_mem = 0
    for _ in range(RUNS):
        command = [PREPROCESSOR_EXECUTABLE, "benchmark", "-m", method,
            "--query-set", queries_file,
            "--input-structure", input_structure,
            "--output-path", f"out_{method}.txt"]
        process = subprocess.Popen(command, preexec_fn=os.setsid)
        print(" ".join(command))
        process.communicate()

        with open("benchmark.txt", "r") as f:
            time, mem = f.readlines()
            total_time += float(time.strip())
            total_mem += int(mem.strip())
        os.remove("benchmark.txt")
    
    mem_mb = total_mem/(RUNS*1024)
    query_time_us = (total_time*1_000_000)/(RUNS*queries_count)
    return mem_mb, query_time_us

compute_structures()

queries = f"../thesisTestsData/{INSTANCE}/{INSTANCE}100000randomQueries.txt"
df_data = []

mem, time = run_benchmark("dijkstra", f"../thesisTestsData/{INSTANCE}", queries)
df_data.append(["Dijkstra", mem, time])

mem, time = run_benchmark("astar", f"../thesisTestsData/{INSTANCE}", queries)
df_data.append(["A*", mem, time])

mem, time = run_benchmark("ch", f"../thesisTestsData/{INSTANCE}/{INSTANCE}.ch", queries)
df_data.append(["CH", mem, time])

mem, time = run_benchmark("tnr", f"../thesisTestsData/{INSTANCE}/{INSTANCE}5000tnodes.tnrg", queries)
df_data.append(["TNR", mem, time])

mem, time = run_benchmark("tnraf", f"../thesisTestsData/{INSTANCE}/{INSTANCE}5000tnodes.tgaf", queries)
df_data.append(["TNRAF", mem, time])

mem, time = run_benchmark("dm", f"../thesisTestsData/{INSTANCE}/{INSTANCE}.hdf5", queries)
df_data.append(["DM", mem, time])

df = pandas.DataFrame(df_data, columns=["label", "memory", "time"])

df.to_csv(f"benchmark_{INSTANCE}.csv")
