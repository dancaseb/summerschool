#!/bin/bash
#SBATCH --job-name=tasks-dag
#SBATCH --account=project_462001452
#SBATCH --reservation=SummerSchoolCPU
#SBATCH --partition=small
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=8
#SBATCH --mem-per-cpu=1G
#SBATCH --time=00:05:00

# Set the number of threads based on cpus-per-task
# export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-1}
export OMP_NUM_THREADS=8

export SCOREP_ENABLE_TRACING=1                    # record and output traces
export SCOREP_TOTAL_MEMORY=1G                     # more memory for Score-P (tracing can be memory hungry)
export SCOREP_EXPERIMENT_DIRECTORY=scorep_output  # Score-P output directory


# Run the program
srun ./tasks
