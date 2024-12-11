APP0_MPI=./build/app0_mpi
APP1_MPI=./build/app1_mpi

APP0_OMP=./build/app0_omp
APP3_OMP=./build/app3_omp

WORKLOAD="5000 5000"

for app in $APP0_OMP $APP3_OMP; do
    echo -e 
    for num_threads in 1 2 3 4 5 6 8 12; do
        run_cmd="$app $WORKLOAD $num_threads"
        $run_cmd
    done
done

for app in $APP0_MPI $APP1_MPI; do
    echo -e 
    for num_threads in 1 2 3 4 5 6; do
        run_cmd="mpirun -np $num_threads $app $WORKLOAD"
        $run_cmd
    done
done