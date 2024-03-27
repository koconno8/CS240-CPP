#include <queue>
#include <bits/stdc++.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include "Waitlist.cpp"
#include "ScheduleCheck.cpp"

using namespace std;


int main () {
	string semester;
  	string prereqfile;
  	string schedsfile;
  	string enrollmentfile;
  	Students s;

  	cin >> semester >> prereqfile >> schedsfile >> enrollmentfile;

  	regex semSeason ("^[0-9]{4}(Spring|Summer|Fall)$");
  	if (!(regex_match (semester, semSeason))) {
   	cout << "Invalid semester" << endl;
      return 1;
   }

  	string line;
  	ifstream schedulesFile(schedsfile);
  	if (schedulesFile.is_open()) {
    	ScheduleCheck sch_check = ScheduleCheck(prereqfile);
		regex bNumber("^(B[0-9]{8}$)");
      while (getline(schedulesFile, line)) {
      	if(regex_match(line.substr(0, 9), bNumber)) {
      		s.insert(line.substr(0, 9), 0);
      	} else {
				cout << "Incorrect BNumber: " << line.substr(0, 9) << endl;
				return 0;	
      	}
	  		string filename = line.substr(line.find(" ") + 1);
			cout << "Processing file: " << filename << "||" << endl;  			  		
	  		sch_check.readFile(filename);
	  		sch_check.reset();
		}
   } else {
   	cout << "Unable to open file";
   	return 0;
   }
   schedulesFile.close();
  	
  	ifstream enrollment (enrollmentfile);
  	if (!enrollment.is_open ()) {
     	cout << "Failed to open file: " << enrollmentfile << endl;
      return 0;
   }

  // Map to store max heaps for each course
  unordered_map<string, Students> courses;

  // Read file line by line
  string theLine;
  while (getline(enrollment, theLine)) {
      // Split line by space
      vector<string> separatedCommand;
      size_t locationOfSpace;

      while ((locationOfSpace = theLine.find (" ")) != string::npos) {
	  		separatedCommand.push_back (theLine.substr (0, locationOfSpace));
	  		theLine.erase(0, locationOfSpace + 1);
		}
		separatedCommand.push_back(theLine);
      // Check command and perform appropriate operation on corresponding max heap
      if (separatedCommand[0] == "newlist") {
      	//cout << separatedCommand.size();
			if (separatedCommand.size() != 2) {
	     		cout << "Error: Insufficient arguments for newlist command." << endl;
	      	exit(0);
	    	}
			
			Students ss = Students();		  
		  courses[separatedCommand[1]] = ss;
		} else if (separatedCommand[0] == "add") {
		  // Check if there are enough arguments
		  if (separatedCommand.size () != 4) {
	     	cout << "Error: Insufficient arguments for add command." << endl;
	      exit(0);
	    }
		  // Parse arguments
		  string bnumber = separatedCommand[1];
		  string course = separatedCommand[2];
		  int weight = stoi(separatedCommand[3]);
			
		if(courses.find(course) == courses.end()) {
			cout << "The course is not yet created\nEnding program...\n";
			exit(0);
		}
		  // Add student to corresponding max heap
			if(s.find(bnumber)) {
				courses[course].insert(bnumber, weight);
			} else {
				cout << "The Student was not mentioned before\nEnding program...\n";
				exit(0);
			}
		} else if (separatedCommand[0] == "promote") {
		  if (separatedCommand.size () != 4) {
	      cout << "Error: Insufficient arguments for promote command." << endl;
	      continue;
	    }
		  // Parse arguments
		  string bnumber = separatedCommand[1];
		  string course = separatedCommand[2];
		  int weight = stoi(separatedCommand[3]);
	
			if(courses.find(course) == courses.end()) {
				cout << "The course is not yet created\nEnding program...\n";
				exit(0);
			}
		  // Add student to corresponding max heap
			if(s.find(bnumber)) {
				courses[course].promote(weight, bnumber);
			} else {
				cout << "The Student was not mentioned before\nEnding program...\n";
				exit(0);
			}
		} else if (separatedCommand[0] == "enroll") {
	  		if (separatedCommand.size () != 2) {
	      	cout << "Error: Insufficient arguments for enroll command." << endl;
	      	continue;
	    	}
	    	string course = separatedCommand[1];
	    	
	    	if(courses.find(course) == courses.end()) {
				cout << "The course is not yet created\nEnding program...\n";
				exit(0);
			}

	    	Student lucky = courses[course].extractMax();
	    	cout << "Enrolling student " << lucky.bnum << " in course " << course << endl;

	}
   }
};