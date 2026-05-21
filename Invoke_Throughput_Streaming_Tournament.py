import subprocess
import os
from itertools import product

print(f"path: {os.getcwd()}")

EXECUTABLE_PATH = "Overall_Throughput_Streaming_Tournament.exe"

n_values = [246284,1837622,9028997,10000000,9999999]
param_sets = [
    [{"w": 1, "k": 50, "s": s} for s in [10,1,0.5,0.1,0.01]],
    
    [{"w": w, "k": 50, "s": 0.1} for w in [10,5,1,0.5,0.1]],
    
    [{"k": k, "s": 0.1, "w": 1} for k in [5,10,50,100,1000]]
]
all_params = [p for subset in param_sets for p in subset]

total_runs = total_runs = len(n_values) * len(all_params)*5
current_run = 0

for n in n_values:
    for params in all_params:
        w, k, s = params["w"], params["k"], params["s"]
        
        for iteration in range(1, 6):
            current_run += 1
            print(f"execute [{current_run}/{total_runs}]: n={n}, w={w}, k={k}, s={s}, 迭代={iteration}")
            
            cmd = [EXECUTABLE_PATH, str(n), str(w), str(s), str(k),str(1.0),str(1)]

            result = subprocess.run(
                    cmd,
                    capture_output=True,
                    text=True,
                )