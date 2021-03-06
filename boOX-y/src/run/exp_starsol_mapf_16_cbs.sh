ROBOT_LIST=`cat robots_16`
SIZE=16

for ROBOTS in $ROBOT_LIST;
do
    echo 'Solving star instance '$SIZE' with '$ROBOTS' agents ...'
    ../main/mapf_solver_boOX --timeout=128 --algorithm=cbs++ '--input-file=star_'$SIZE'_a'$ROBOTS'.mpf' '--output-file=mapf-cbs_star_'$SIZE'_a'$ROBOTS'.out' > 'mapf-cbs_star_'$SIZE'_a'$ROBOTS'.txt'
done
