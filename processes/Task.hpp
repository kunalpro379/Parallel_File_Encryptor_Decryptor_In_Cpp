#ifndef TASK_HPP
#define TASK_HPP
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "../file_handling/IO.hpp"
#include <cstddef> 
///Custom Task datatype
using namespace std;
enum class Action
{
     ENCRYPT,
     DECRYPT
};

struct Task
{
     string file_path;
     fstream f_stream;
     Action action;
     Task(fstream &&stream, Action act, string file_path){ //&&--> volatile, passed just to assign, no copy made
          this->f_stream = move(stream);
          this->action = act;
          this->file_path = file_path;
     }
     string toString(){
          ostringstream oss;
          oss << file_path << "," << (action == Action::ENCRYPT ? "ENCRYPT" : "DECRYPT"); // Serialization
          return oss.str();
     }
     static Task fromString(const string &taskData)
     {
          istringstream iss(taskData);
          string file_path;
          string actionStr;
          if (getline(iss, file_path, ',') && getline(iss, actionStr))
          {
               Action action = (actionStr == "ENCRYPT" ? Action::ENCRYPT : Action::DECRYPT);

               IO io(file_path);
               fstream f_stream = move(io.getFileStream());
               // move in cpp means : transfer ownership of the object to the new owner
               if (f_stream.is_open())
                    return Task(move(f_stream), action, file_path);
               else
                    throw runtime_error("Failed to open File: " + file_path);
          }
          else
               throw runtime_error("Invalid task data");
     }
};
#endif
