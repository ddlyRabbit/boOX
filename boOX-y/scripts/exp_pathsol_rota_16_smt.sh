ROBOT_LIST=`cat robots_16`
SIZE=16

for ROBOTS in $ROBOT_LIST;
do
    echo 'Solving path instance '$SIZE' with '$ROBOTS' agents ...'
   ./rota_solver_boOX --algorithm=smtcbs --timeout=64 '--input-file=path_'$SIZE'_a'$ROBOTS'.mpf' '--output-file=grid_'$SIZE'x'$SIZE'_a'$ROBOTS'.out' > 'rota-smt_path_'$SIZE'_a'$ROBOTS'.txt'
done
