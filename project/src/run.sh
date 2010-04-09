#/bin/sh

array=( 50 100 200 300 400 500 600 700 800 900 1000 )

for n in $(seq 0 10)
do
  printf "matrix size: %d\n" "${array[n]}"
  ./mm 8 ${array[n]} 
done
