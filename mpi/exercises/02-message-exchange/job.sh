#!/bin/bash
#SBATCH --job-name=mpi-message-exchange
#SBATCH --account=project_462001452
#SBATCH --partition=standard
#SBATCH --exclusive
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=2
#SBATCH --cpus-per-task=1
#SBATCH --mem=0
#SBATCH --time=00:10:00
#SBATCH --output=slurm-%x-%J.out

# Run the program
srun ./exchange
