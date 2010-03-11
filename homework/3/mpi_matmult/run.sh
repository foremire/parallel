#/bin/sh

array=( 360 421 514 609 737 920 1160 1420 1770 2210 2850)

for n in $(seq 2 10)
do
  printf "\nnp: %d\n" "$n"
  for i in $(seq 0 10)
  do
    printf "matrix size: %d\n" "${array[i]}"
    mpirun -np $n ./mm ${array[size]} | grep -E "^Tt"
  done
done
