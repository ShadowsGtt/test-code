lib := -lprotobuf -pthread -std=c++11 
obj := test.cpp test.pb.cc
cc := g++
exc := test
$(exc):$(obj)
	$(cc) $(obj) $(lib) -o $(exc)

