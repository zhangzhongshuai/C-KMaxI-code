import subprocess
import os
from itertools import product

print(f"path: {os.getcwd()}")

EXECUTABLE_PATH = "Overall_Throughput_Streaming_RT.exe"

n_values = [246284,1837622,9028997,10000000,9999999]
param_sets = [
    [{"percent":percent} for percent in [0.2,0.4,0.6,0.8,1.0]],
]
all_params = [p for subset in param_sets for p in subset]

total_runs = len(n_values) * len(all_params)
current_run = 0

for n in n_values:
    for params in all_params:
        p = params["percent"]
        current_run += 1
        print(f"execute [{current_run}/{total_runs}]: n={n}, w={1.0}, k={50}, s={0.1},p={p}")
        cmd = [EXECUTABLE_PATH, str(n), str(1.0), str(0.1), str(50),str(p),str(2)]
        result = subprocess.run(cmd,capture_output=True,text=True,)