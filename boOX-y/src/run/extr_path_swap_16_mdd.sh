ROBOT_LIST=`cat robots_16`
SEED_LIST=`cat seeds_10`
SIZE=16

for ROBOTS in $ROBOT_LIST;
do
  for SEED in $SEED_LIST;	
  do
    echo $ROBOTS,$SEED
    grep "machine TIME" 'swap-mdd_path_'$SIZE'_a'$ROBOTS'_'$SEED'.txt'
    grep "clauses" 'swap-mdd_path_'$SIZE'_a'$ROBOTS'_'$SEED'.txt'
  done
done
