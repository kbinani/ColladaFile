CXXFLAGS=-I../ext/gtest -I../ext/gtest/include -std=c++11
LDFLAGS=-stdlib=libc++
OBJS=test.o ../colladafile_scene.o
PROGRAM=test.out

all: $(PROGRAM)

test: test.out
	$(RM) scene.dae
	./test.out
	xmllint --noout scene.dae
	diff -u scene.dae fixtures/scene.dae

test.out: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(PROGRAM)

