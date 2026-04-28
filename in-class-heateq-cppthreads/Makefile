CXX = c++
CXX_FLAGS = --std=c++23 -Wall -Wextra -march=native -g

all: sequential_implementation student_submission

sequential_implementation: Utility.h sequential_implementation.cpp
	$(CXX) $(CXX_FLAGS) -o sequential_implementation sequential_implementation.cpp 

student_submission: Utility.h student_submission.cpp 
	$(CXX) $(CXX_FLAGS) -o student_submission student_submission.cpp

clean:
	rm -f sequential_implementation student_submission
