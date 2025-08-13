#!/bin/bash
#SBATCH -t 1:00:00
#SBATCH --partition=small
#SBATCH --account=project_2014955
#SBATCH --exclusive

module load python-data/3.10-24.04
module load intel-oneapi-compilers/2025.0.4
module load gcc/14.2.0
cd .. && make all-icx all-gcc
python bench.py