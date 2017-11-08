//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: loops.cpp
//**
//** Thomas A. Germer
//** Sensor Science Division, National Institute of Standards and Technology
//** 100 Bureau Dr. Stop 8443; Gaithersburg, MD 20899-8443
//** Phone: (301) 975-2876
//** Email: thomas.germer@nist.gov
//**
//** Version: 4.00 (January 2015) 
//**
//******************************************************************************
#include "loops.h"
#include "mist.h"

using namespace std;
using namespace SCATMECH;

table_loop_variable::table_loop_variable(const string& _filename, Variable_List& _variable_list)
	: filename(_filename), loop_variable(_variable_list)
{
	string fname = find_file(filename);
	ifstream_with_comments file(fname.c_str());
	if (!file) throw MIST_exception("Cannot open file " + filename);

	// Get header line...
	string line = file.getstrline();
	istringstream linestream(line);
	int ncol = 0;
	string str;
	while (linestream >> str, !linestream.fail()) {
		vlist.push_back(str);
		ncol++;
	}

	table.resize(ncol);
	npoints = 0;

	for (int i = 0; !file.fail();++i) {
		line = file.getstrline();

		if (file.fail() || file.eof() || line.size()==0) {
			if (npoints == 0) throw MIST_exception("No points in LIST loop.");
			return;
		}

		istringstream linestream(line);
		for (int j = 0;j < ncol;++j) {
			string temp;
			linestream >> temp;

			if (linestream.fail()||temp=="") {
				if (j==0) {
					if (npoints==0) throw MIST_exception("No points in LIST loop.");
					return;
				}
				throw MIST_exception("Error reading column in " + filename);
			}
			table[j].push_back(temp);
        }
		npoints++;
    } 
	if (npoints==0) throw MIST_exception("No points in LIST loop.");
	return;
}

void for_loop_variable::reset() {
	ValueVector vv = Arithmetic_Evaluator(from,variable_list.variables).Value();
	value = vv;
	progress = 0;
	set();
}

void for_loop_variable::operator ++() {
	ValueVector vv = Arithmetic_Evaluator(by,variable_list.variables).Value();
	value += vv;
	set();
}

bool for_loop_variable::test() {
	double begin = Arithmetic_Evaluator(from,variable_list.variables).Value();
	double end = Arithmetic_Evaluator(to,variable_list.variables).Value();
	progress = (value-begin)/(end-begin);
	if (end>begin) return value<=end*(1.00000000001);
	else return value>=end*(0.999999999999);
}

void for_loop_variable::set() {
		// Set the value of that parameter in the array of parameters...
		variable_list.assign(variable,to_string16(value));
	}


void table_loop_variable::set() {
		for (int i=0;i<vlist.size()&&test();++i) {
            if (vlist[i][0]=='$') {
    			string var = vlist[i];
	    		string val = table[i][icount];
		    	variable_list.assign(var,val);
            } else {
       			string var = vlist[i];
    			//double val = Arithmetic_Evaluator(table[i][icount],variable_list.variables).Value();
				//variable_list.assign(var,to_string16(val));
    			string val = (Arithmetic_Evaluator(table[i][icount],variable_list.variables).Value()).String();
				variable_list.assign(var,val);
            }
		}
	}

#ifdef GERMER_MODELS

void mpi_for_loop_variable::reset() {
	ValueVector _from = Arithmetic_Evaluator(from,variable_list.variables).Value();
	ValueVector _by = Arithmetic_Evaluator(by,variable_list.variables).Value();
	value = _from;
	for (int i=0;i<irank;++i) value += _by;
	progress = 0;
	
	set();
}

void mpi_for_loop_variable::operator ++() {
	ValueVector vv = Arithmetic_Evaluator(by,variable_list.variables).Value();
	for (int i=0;i<nrank;++i) {
		value += vv;
	}
	set();
}

bool mpi_for_loop_variable::test() {
	return for_loop_variable::test();
}
#endif
