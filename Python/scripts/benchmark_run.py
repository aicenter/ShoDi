import os
import subprocess
import pandas

PREPROCESSOR_EXECUTABLE = './shortestPathsPreprocessor'

def compute_structures():
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "create", "-m", "ch", "-i", "../thesisTestsData/Chicago", "-o", "../thesisTestsData/Chicago/Chicago"])
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "create", "-m", "tnr", "--preprocessing-mode", "slow", "--tnodes-cnt", "1000", "-i", "../thesisTestsData/Chicago", "-o", "../thesisTestsData/Chicago/Chicago1000tnodes"])
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "create", "-m", "tnraf", "--preprocessing-mode", "slow", "--tnodes-cnt", "1000", "-i", "../thesisTestsData/Chicago", "-o", "../thesisTestsData/Chicago/Chicago1000tnodes"])
    process = subprocess.run([PREPROCESSOR_EXECUTABLE, "create", "-m", "dm", "--preprocessing-mode", "fast", "-i", "../thesisTestsData/Chicago", "-o", "../thesisTestsData/Chicago/Chicago", "--output-format", "xdm"])
    print('computing structures finished')

def run_benchmark(method, input_structure, queries_file):
    command = [PREPROCESSOR_EXECUTABLE, "benchmark", "-m", method,
        "--query-set", queries_file,
        "--input-structure", input_structure,
        "--output-path", f"out_{method}.txt"]
    process = subprocess.Popen(command, preexec_fn=os.setsid)
    print(' '.join(command))
    process.communicate()

    with open("benchmark.txt", "r") as f:
        time, mem = f.readlines()
    os.remove("benchmark.txt")

    return mem.strip(), float(time)

compute_structures()

queries = "../thesisTestsData/Chicago/Chicago100000randomQueries.txt"
df_data = []

mem, time = run_benchmark('dijkstra', '../thesisTestsData/Chicago', queries)
df_data.append(['Dijkstra', mem, time])

mem, time = run_benchmark('astar', '../thesisTestsData/Chicago', queries)
df_data.append(['Astar', mem, time])

mem, time = run_benchmark('ch', '../thesisTestsData/Chicago/Chicago.ch', queries)
df_data.append(['CH', mem, time])

mem, time = run_benchmark('tnr', '../thesisTestsData/Chicago/Chicago1000tnodes.tnrg', queries)
df_data.append(['TNR', mem, time])

mem, time = run_benchmark('tnraf', '../thesisTestsData/DC/DC1000tnodes.tgaf', queries)
df_data.append(['TNRAF', mem, time])

mem, time = run_benchmark('dm', '../thesisTestsData/DC/DC.xdm', queries)
df_data.append(['DM', mem, time])

df = pandas.DataFrame(df_data, columns=['label', 'memory', 'time'])

print(df)
df.to_csv('benchmark_data.csv')
