//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: evaluate.h
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

#ifndef EVALUATE_H
#define EVALUATE_H

#include "scatmech.h"
#include <string>
#include <sstream>
#include <stack>
#include <vector>
#include <map>

class EVAL_exception : public std::exception
{
public:
	
    EVAL_exception(const std::string& m)
	{
		message = "EVALUATE: " + m;
	}
    virtual ~EVAL_exception() throw() {};
	
    virtual const char *what() const throw() {
		return message.c_str();
	}
	
private:
    std::string message;
};


class ValueVector : public std::vector<double> 
{
public:
	typedef std::vector<double> dvector; 

	ValueVector() {}
	ValueVector(const dvector& x) : std::vector<double>(x) {}
	ValueVector(double x) { push_back(x); }
	
	operator double() const {return back();}
	const double& operator[](int i) const {return at(i);}
	std::string String(bool highprec=true) const;
};

class MIST;
class MIST_istream;

typedef std::map<std::string,ValueVector> ValueVectorMap;

struct Variable_List {

	void set_MIST(MIST* _mist) {mist = _mist;} 
	//std::vector<ValueVector> value;
	ValueVectorMap variables;
	std::vector<std::string> name; // This is the table of variable names, in the order in which they are defined
	std::vector<std::string> str;  // This is the table of the expressions, in the order in which they are defined
	std::vector<std::string> last; // This is the table of the last string expression sent to the model

	void get(MIST_istream& inputstream);
	void new_variable(const std::string& name,const std::string& _value);
	void assign(const std::string& _name,const std::string& _value);

	MIST* mist;
};



class Arithmetic_Evaluator {
public:
	Arithmetic_Evaluator(const char* str,const ValueVectorMap& _variables=ValueVectorMap()) 
			: input(std::string(str)), variables(_variables) {evaluate();}
	Arithmetic_Evaluator(const std::string& str, const ValueVectorMap& _variables=ValueVectorMap()) 
			: input(str), variables(_variables) {evaluate();}

	const ValueVector& Value() const {return result;}
	
private:

	std::istringstream input;
	
	void evaluate();

private:

	const ValueVectorMap& variables;
	//const std::vector<ValueVector>& vars;
	//const std::vector<std::string>& vnames;

	std::stack<double> val_stack;
	std::stack<int> op_stack;
	std::stack<int> prec_stack;
	ValueVector result;

	void operate();
	ValueVector call_function(const std::string& s,const std::vector<double>& args);

	bool lower_prec(int prec);
	void ignore_whitespace();
	void get_value();
	void get_operator();
	std::vector<double> get_paren(bool allow_empty=false);
}; 

bool same(const std::string& a,const std::string& b);


#endif
