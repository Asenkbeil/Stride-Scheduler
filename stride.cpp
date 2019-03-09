// Andrew Senkbeil

#include <stdio.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

void sort(void);

struct jobs
{
	string name;
	int stride;
	int pass;
	int priority;

};

//some globals
vector<string> arguments;
vector<jobs> runnable;
vector<jobs> blocked;
jobs running;
jobs clean;
jobs newjob;


int main(int argc, char* argv[])
{
	//checking for text file
	if(argc > 2)
	{
		printf("too many agruments \n");
		exit(0);
	}
	if(argc < 2)
	{
		printf("argument expected \n");
		exit(0);
	}
	
	ifstream file(argv[1]);
	if (file.good());
	else
	{
		printf("file not good");
		exit(0);
	}




	string buffer;
	while(getline(file, buffer)) // puts all arguments into a vector
	{
	arguments.push_back(buffer);
	}
	
	for(int i = 0; i < arguments.size(); i++)
	{
		//newjob
		if(arguments[i].substr(0,6) == "newjob")
		{
			
			newjob.name = arguments[i].substr(7,1);
			stringstream stream(arguments[i].substr(9));
			stream >> newjob.priority;
			if(newjob.priority == 0)
				newjob.priority = 1;
			newjob.stride = 100000/ newjob.priority;

			cout << "New job: " << newjob.name << " added with priority: " << newjob.priority << endl;
			if(running.name == "")
			{
				running = newjob;
				newjob = clean;
				cout << "Job: " << running.name << " scheduled." << endl;
			}
			else
			{
				if(runnable.size() == 0)
				{
					newjob.pass = running.pass;
					runnable.push_back(newjob);
				}
				else
				{
					sort();
					newjob.pass = runnable[0].pass;
					runnable.push_back(newjob);
				}
			}
			newjob = clean;
		}

		//finish
		if(arguments[i].substr(0,6) == "finish")
		{
			if(running.name != "")
			{
				cout << "Job: " << running.name << " completed." << endl;
				running = clean;
				if(runnable.size() == 0)
					cout << "System is idle" << endl;
				else
				{
					sort();
					running = runnable[0];
					runnable.erase(runnable.begin());
					cout << "Job: " << running.name << " scheduled." << endl;
				}
			}
			else
				cout << "Error. System is idle." << endl;
		}

		//interrupt
		if(arguments[i].substr(0,6) == "interr")
		{
			if(running.name == "")
				cout << "Error. System is idle." << endl;
			else
			{
				running.pass = running.pass + running.stride;
				runnable.push_back(running);
				sort();
				running = clean;
				running = runnable[0];
				runnable.erase(runnable.begin());
				cout << "Job: " << running.name << " scheduled." << endl;
			}
		}

		//block
		if(arguments[i].substr(0,6) == "block")
		{
			if(running.name == "")
				cout << "Error. System is idle." << endl;
			else
			{	
				cout << "Job: " << running.name << " blocked." << endl;
				blocked.push_back(running);
				running = clean;
				if(runnable.size() == 0)
					cout << "System is idle." << endl;
				else
				{
					sort();
					running = runnable[0];
					runnable.erase(runnable.begin());
					cout << "Job: " << running.name << " scheduled." << endl;
				}
			}
		}

		//unblock
		if(arguments[i].substr(0,6) == "unbloc")
		{
			bool found = false;
			for(int j = 0; j < blocked.size(); j++)
			{
				if(blocked[j].name == arguments[i].substr(8))
				{
					if(running.name != "")
					{
						blocked[j].pass = running.pass;
					}
					else
					{
						if(runnable.size() == 0)
							blocked[j].pass = 0;
						else
						{
							sort();
							blocked[j].pass = runnable[0].pass;
						}
					}

					int passSet = blocked[j].pass;
					runnable.push_back(blocked[j]);
					blocked.erase(blocked.begin()+ j);
					found = true;
					cout << "Job: " << arguments[i].substr(8) << " has unblocked. Pass set to: " << passSet << endl;
			
				}
			}
			if(found == false)
				cout << "Error. Job: " << arguments[i].substr(8) << " not blocked." << endl;
		}


		//runnable
		if(arguments[i].substr(0,6) == "runnab")
		{
			cout << "Runnable:" << endl;

			if(runnable.size() == 0)
				cout << "None" << endl;
			else
			{
				sort();
				//cout << "NAME" << setw(10) << "STRIDE" << setw(6) << "PASS" << setw(5) << "PRI" << endl;
				cout << setw(8) << left << "NAME";
				cout << setw(8) << left << "STRIDE";
				cout << setw(6) << left << "PASS";
				cout << setw(6) << right << "PRI" << endl;
				for(int i = 0; i < runnable.size(); i++)
				{
					cout << setw(8) << left << runnable[i].name;
					cout << setw(8) << left << runnable[i].stride;
					cout << setw(6) << left << runnable[i].pass;
					cout << setw(6) << right << runnable[i].priority;
					cout << endl;
				}
			}
		}

		//running
		if(arguments[i].substr(0,6) == "runnin")
		{
			cout << "Running:" << endl;

			if(running.name == "")
				cout << "None" << endl;
			else
			{
				cout << setw(8) << left << "NAME";
				cout << setw(8) << left << "STRIDE";
				cout << setw(6) << left << "PASS";
				cout << setw(6) << right << "PRI" << endl;
				cout << setw(8) << left << running.name;
				cout << setw(8) << left << running.stride;
				cout << setw(6) << left << running.pass;
				cout << setw(6) << right << running.priority;
				cout << endl;
			}
		}

		// blocked
		if(arguments[i].substr(0,6) == "blocke")
		{
			cout << "Blocked:" << endl;

			if(blocked.size() == 0)
				cout << "None" << endl;
			else
			{
				//cout << "NAME" << setw(10) << "STRIDE" << setw(6) << "PASS" << setw(5) << "PRI" << endl;
				cout << setw(8) << left << "NAME";
				cout << setw(8) << left << "STRIDE";
				cout << setw(6) << left << "PASS";
				cout << setw(6) << right << "PRI" << endl;

				for(int i = 0; i < blocked.size(); i++)
				{
					cout << setw(8) << left << blocked[i].name;
					cout << setw(8) << left << blocked[i].stride;
					cout << setw(6) << left << blocked[i].pass;
					cout << setw(6) << right << blocked[i].priority;
					cout << endl;
				}
			}
		}
	}


	return 0;
}

void sort()
{
	if(runnable.size() < 2)
	{
		return;
	}
	else 
	{
		for(int i = 0; i < runnable.size(); i++)
		{
			for(int j = i + 1; j < runnable.size(); j++)
			{
				if(runnable[i].pass > runnable[j].pass)
					swap(runnable[i], runnable[j]);
				else if(runnable[i].pass == runnable[j].pass)
				{
					if(runnable[i].name[0] > runnable[j].name[0])
						swap(runnable[i], runnable[j]);
				}
			}
		}
	}
}
