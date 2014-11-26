CXXFLAGS=-std=c++11
LDFLAGS=-stdlib=libc++
OBJS=test.o ../colladafile_scene.o
PROGRAM=test.out

all: $(PROGRAM)

test: test.out
	$(RM) scene.dae
	faketime "2014-11-25 12:34:56+00:00" ./test.out
	xmllint --noout --schema http://www.khronos.org/files/collada_schema_1_4 scene.dae
	diff -u scene.dae fixtures/scene.dae

test.out: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(PROGRAM)

