project2: matrixMult.cpp
	nvcc main.cu matrix.cpp help.cpp test.cpp -o project2

run: project2
	./project2

clean: project2
	rm project2
