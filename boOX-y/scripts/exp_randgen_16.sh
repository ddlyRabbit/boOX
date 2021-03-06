ROBOT_LIST=`cat robots_16`
SIZE=16

for ROBOTS in $ROBOT_LIST;
do
  echo 'Generating random '$SIZE' with '$ROBOTS' agents ...'
  ./randgen_boOX '--walk' '--edge-probability=0.2' '--N-vertices='$SIZE '--N-agents='$ROBOTS '--mpf-file=random_'$SIZE'_a'$ROBOTS'.mpf' '--cpf-file=random_'$SIZE'_a'$ROBOTS'.cpf'
done
