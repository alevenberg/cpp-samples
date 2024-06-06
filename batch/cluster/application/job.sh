#!/bin/bash
#SBATCH -N 1
#SBATCH --ntasks 4
#SBATCH --job-name parallel_hello
#SBATCH --constraint ib
#SBATCH --partition atesting
#SBATCH --time 0:01:00
#SBATCH --output parallel_hello_world.out

module purge

module load gcc
module load openmpi

mpirun -np 4 ./hello_world_mpi.exe