all:
	g++ source/volVis.cpp source/model.cpp -w -lGL -lglut -lGLU -o p
	
run:
	g++ source/model.cpp source/volVis.cpp -w -lGL -lglut -lGLU -o p
	p ${ARGS}
	
