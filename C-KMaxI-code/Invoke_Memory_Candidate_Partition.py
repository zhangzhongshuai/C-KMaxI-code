import subprocess
import os
from itertools import product

print(f"path: {os.getcwd()}")

EXECUTABLE_PATH = "Memory_Candidate_Partition.exe"

n_values = [246284,1837622,9028997,10000000,9999999]
total_runs = 5
current_run = 0

for n in n_values:
    current_run += 1
    cmd = [EXECUTABLE_PATH, str(n), str(1.0), str(0.1), str(50)]
    result = subprocess.run(cmd,capture_output=True,text=True,)