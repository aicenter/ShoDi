import os
import subprocess
import pandas

PREPROCESSOR_EXECUTABLE = "./shortestPathsPreprocessor"
RUNS = 10
INSTANCE = "DC"

def compute_structures():
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "create", "-m", "ch", "-i", f"../thesisTestsData/{INSTANCE}", "-o", f"../thesisTestsData/{INSTANCE}/{INSTANCE}"])
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "create", "-m", "tnr", "--preprocessing-mode", "slow", "--tnodes-cnt", "5000", "-i", f"../thesisTestsData/{INSTANCE}", "-o", f"../thesisTestsData/{INSTANCE}/{INSTANCE}5000tnodes"])
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "create", "-m", "tnraf", "--preprocessing-mode", "slow", "--tnodes-cnt", "5000", "-i", f"../thesisTestsData/{INSTANCE}", "-o", f"../thesisTestsData/{INSTANCE}/{INSTANCE}5000tnodes"])
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "create", "-m", "dm", "--preprocessing-mode", "fast", "-i", f"../thesisTestsData/{INSTANCE}", "-o", f"../thesisTestsData/{INSTANCE}/{INSTANCE}", "--output-format", "hdf"])
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
df_data.append(["Astar", mem, time])

mem, time = run_benchmark("ch", f"../thesisTestsData/{INSTANCE}/{INSTANCE}.ch", queries)
df_data.append(["CH", mem, time])

mem, time = run_benchmark("tnr", f"../thesisTestsData/{INSTANCE}/{INSTANCE}5000tnodes.tnrg", queries)
df_data.append(["TNR", mem, time])

mem, time = run_benchmark("tnraf", f"../thesisTestsData/{INSTANCE}/{INSTANCE}5000tnodes.tgaf", queries)
df_data.append(["TNRAF", mem, time])

mem, time = run_benchmark("dm", f"../thesisTestsData/{INSTANCE}/{INSTANCE}.hdf5", queries)
df_data.append(["DM", mem, time])

df = pandas.DataFrame(df_data, columns=["label", "memory", "time"])

print(df)
df.to_csv(f"benchmark_{INSTANCE}.csv")
