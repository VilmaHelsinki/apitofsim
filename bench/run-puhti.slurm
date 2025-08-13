#!/bin/bash
#SBATCH -t 1:00:00
#SBATCH --partition=small
#SBATCH --account=project_2014955
#SBATCH --exclusive

cd ..
make clean-all
module load gcc/14.2.0
make all all-gcc
module load intel-oneapi-compilers/2025.0.4
make all-icx
VMS_BIN=$(pwd)/../bin BASE_INPUTS=$(pwd)/../inputs/example COMPILERS='icx,gcc' python3.11 bench.py