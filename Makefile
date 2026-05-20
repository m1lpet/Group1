all: main

main: main.cpp employee.cpp system.cpp task.cpp
  g++ -o main main.cpp employee.cpp system.cpp task.cpp

clean:
  rm -f main
