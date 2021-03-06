ROBOT_LIST=`cat robots_08x08`
SIZE=8

for ROBOTS in $ROBOT_LIST;
do
    echo 'Solving grid instance '$SIZE'x'$SIZE' with '$ROBOTS' agents ...'
  ./insolver_reLOC --total-timeout=64 --minisat-timeout=64 --encoding=mdd '--input-file=grid_'$SIZE'x'$SIZE'_a'$ROBOTS'.cpf' '--output-file=grid_'$SIZE'x'$SIZE'_a'$ROBOTS'.out' > 'mapf-mdd_grid_'$SIZE'x'$SIZE'_a'$ROBOTS'.txt'
done
