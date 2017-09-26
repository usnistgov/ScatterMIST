 //******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: loops.h
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
#ifndef MIST_LOOPS_H
#define MIST_LOOPS_H

#include "mist0.h"
#include "scatmech.h"

#ifdef GERMER_MODELS
#include "tagmpi.h"
#endif

template <class T>
std::string to_string16(const T& t)
{
    std::ostringstream oss;
    oss.width(16);
	oss.precision(16);
    oss << t;
    return oss.str();
}

class loop_variable 
{
public:
	loop_variable(Variable_List& _variable_list) : variable_list(_variable_list) { reset(); }

	virtual void reset() {};
	virtual void operator ++() =0;
	virtual bool test() =0;	
	virtual const std::string& get_variable(int i)=0;
    virtual std::string get_value(int i)=0;
	virtual int get_nvar()=0;
	virtual double get_progress()=0;
	virtual void set() =0;

protected:
	Variable_List& variable_list;
};

class for_loop_variable : public loop_variable
{
public:
	for_loop_variable(const std::string& _variable,
		const std::string& _from,
		const std::string& _to,
		const std::string& _by,
		Variable_List& _variable_list) 
		: variable(_variable),from(_from),to(_to),by(_by), loop_variable(_variable_list) { reset(); }

	void reset();
	void operator ++();
	virtual	bool test();
	const std::string& get_variable(int i) {return variable;}
    std::string get_value(int i) {return to_string16(value);}
	double get_progress() {return progress;}
	int get_nvar() {return 1;}

public:

	void set();
	const std::string variable;
	const std::string from;
	const std::string to;
	const std::string by;
	
	double value;
	double progress;
};

class table_loop_variable : public loop_variable
{
public:
	table_loop_variable(const std::string& _filename, Variable_List& _variable_list);

	virtual void reset() { 
		icount=0; 
		set(); 
	} 

	virtual void operator ++() {
		++icount;
		set();
	}
	virtual bool test() {
		return (icount<npoints);
	}
	virtual const std::string& get_variable(int i) {
		return vlist[i];
	}
    virtual std::string get_value(int i) {
		return table[i][icount];
	}
	virtual double get_progress() {
		if (npoints!=0) {
			return (double)icount/npoints;
		} else {
			return 0.;
		}
	}
	virtual void set();
	int get_nvar() {return vlist.size();}

protected:
	std::string filename;
	std::deque<std::string> vlist;
    std::deque<std::deque<std::string> > table;
	int icount;
	int npoints;
};

#ifdef GERMER_MODELS
class mpi_table_loop_variable : public table_loop_variable
{
public:
	mpi_table_loop_variable(const std::string& _filename, Variable_List& _variable_list) :
	  table_loop_variable(_filename,_variable_list) 
	  {
	 	  using namespace std;
		  using namespace SCATMECH;
		  irank = MyMPI_GetRank();
		  nrank = MyMPI_GetSize();
		  //cerr << "irank = " << irank << " nrank = " << nrank << endl;
	  };

	virtual void reset() { 
		icount=irank; 
		set(); 
	} 

	virtual void operator ++() {
	    using namespace std;
		icount+=nrank;
		cerr << "icount = " << icount << endl;
		set();
	}


private:
	int nrank;
	int irank;
};
#endif

#ifdef GERMER_MODELS
class mpi_for_loop_variable : public for_loop_variable
{
public:
	mpi_for_loop_variable(const std::string& _variable,
						  const std::string& _from,
						  const std::string& _to,
						  const std::string& _by,
						  Variable_List& _variable_list)
						  : for_loop_variable(_variable,_from,_to,_by,_variable_list) 
	{
 		using namespace std;
		using namespace SCATMECH;
		irank = MyMPI_GetRank();
		nrank = MyMPI_GetSize();
	};

	void reset();
	void operator ++();
	bool test();

private:
	int nrank;
	int irank;
};
#endif

class loop 
{
public:
	void push(loop_variable *x) {vars.push_front(x);}

	void reset() {
		for (rvars=vars.rbegin();rvars!=vars.rend();++rvars) (*rvars)->reset();
		ok = true;
	}

	void operator ++() 
	{
		for (pvars = vars.begin(); pvars!=vars.end() ; ++pvars) {
			++(**pvars);
			if ((*pvars)->test()) return;
			(*pvars)->reset();
		}
		ok = false;
	}

	bool test() {
		return ok;
	}

	double get_progress() {return (vars.empty()) ? 0. : vars[vars.size()-1]->get_progress();}

	~loop() {
		for (pvars = vars.begin();pvars!=vars.end();++pvars) delete *pvars;
	}

public:
	std::deque<loop_variable*> vars;
	std::deque<loop_variable*>::iterator pvars;
	std::deque<loop_variable*>::reverse_iterator rvars;

	bool ok;
};

#endif
