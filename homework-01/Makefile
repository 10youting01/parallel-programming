CXX = g++
CXX_FLAGS = --std=c++23 -Wall -Wextra -march=native -O3

all: reference_implementation sequential_implementation student_submission

reference_implementation: reference_implementation.cpp
	$(CXX) $(CXX_FLAGS) -o reference_implementation  reference_implementation.cpp

sequential_implementation: sequential_implementation.cpp
	$(CXX) $(CXX_FLAGS) -o sequential_implementation sequential_implementation.cpp

student_submission: student_submission.cpp
	$(CXX) $(CXX_FLAGS) -o student_submission student_submission.cpp

remove_reference:
	rm -f *reference*