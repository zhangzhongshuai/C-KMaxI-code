# README

## 1. Data
| Filename       | Dataset Description                                                                 | Data Volume | Additional Parameters                          | Dataset Source                                                                 |
|----------------|-------------------------------------------------------------------------------------|-------------|------------------------------------------------|--------------------------------------------------------------------------------|
| Stock.txt      | Bitcoin Stock Price Dataset                                                        | 9,028,997   | -                                              | https://www.alphavantage.co/                                                   |
| Power.txt      | UK Electricity Dataset                                                            | 246,284     | -                                              | https://www.kaggle.com/datasets/thedevastator/gb-electrical-grid-half-hourly-data-2008-present |
| Genome.txt     | Scoring sequences derived from the *Haemophilus influenzae* genome                 | 1,837,622   | -                                              | https://www.ncbi.nlm.nih.gov/Traces/wgs/JAMLFG01                               |
| Uniform.txt    | Generated dataset following a uniform distribution                                | 1,000,000   | Uniform distribution with Min=-1000, Max=1000   | Generated dataset                                  |
| Normal.txt     | Generated dataset following a normal distribution                                 | 1,000,000   | Normal distribution with Mean=0, Std=1000       | Generated dataset                                     |


## 2. Algorithm Code
The algorithm code is divided into two parts, corresponding to different experimental scenarios in the paper:

### 2.1 Code for Overall and Throughput Experiments
The following C++ source files correspond to the five algorithms mentioned in the paper, used for Overall and Throughput experiments:
- `Overall_Throughput_Partition.cpp`
- `Overall_Throughput_Streaming_RT.cpp`
- `Overall_Throughput_Streaming_Tournament.cpp`
- `Overall_Throughput_RuzzoTompa.cpp`
- `Overall_Throughput_Tournament.cpp`

### 2.2 Code for Memory and Candidate Experiments
The following C++ source files are used for Memory and Candidate experiments. Note that the Tournament and Streaming_Tournament algorithms do not involve the concept of "Candidate" and thus are not included in Candidate experiments:
- `Memory_Candidate_Partition.cpp`
- `Memory_Candidate_RuzzoTompa.cpp`
- `Memory_Candidate_Streaming_RT.cpp`
- `Memory_Streaming_Tournament.cpp` (Only for Memory experiment)
- `Memory_Tournament.cpp` (Only for Memory experiment)


## 3. Python Scripts for Invoking Algorithm Code
Python scripts are categorized into three types, which can be run directly to save experimental results into corresponding txt files:

### 3.1 Scripts for Overall Experiments
- `Invoke_Overall_Partition.py`
- `Invoke_Overall_Streaming_RT.py`
- `Invoke_Overall_Streaming_Tournament.py`
- `Invoke_Overall_RuzzoTompa.py`
- `Invoke_Overall_Tournament.py`

### 3.2 Scripts for Throughput Experiments
These scripts generate results of the five algorithms under different parameters:
- `Invoke_Throughput_Partition.py`
- `Invoke_Throughput_Streaming_RT.py`
- `Invoke_Throughput_Streaming_Tournament.py`
- `Invoke_Throughput_RuzzoTompa.py`
- `Invoke_Throughput_Tournament.py`

### 3.3 Scripts for Memory and Candidate Experiments
These scripts generate results of the five algorithms under different parameters. The Tournament and Streaming_Tournament algorithms only output Memory experiment results:
- `Invoke_Memory_Candidate_Partition.py`
- `Invoke_Memory_Candidate_Streaming_RT.py`
- `Invoke_Memory_Tournament.py`
- `Invoke_Memory_Candidate_RuzzoTompa.py`
- `Invoke_Memory_Tournament.py`
