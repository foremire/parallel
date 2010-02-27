minimal:
	mpicc -o minimal minimal.c
clean:
	rm -rf *.o *~
	rm -rf minimal

hello:
	mpicc -o hello hello.c

runhello:
	mpirun -np 8 ./hello

telephone:
	mpicc -o phone telephone.c

runtelephone:
	mpirun -np 2 phone
		
