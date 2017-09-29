//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: evaluate.cpp
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

#include "evaluate.h"
#include "mist.h"
#include "scattabl.h"
#include "scateval.h"

using namespace std;
using SCATMECH::deg;

static const char binops[] = ",&|><=+-*/^";
static const int precs[] = {0,1,1,2,2,2,3,3,5,5,6};

// This function returns the position of the character c in the string p...
static int member(char c,const char* p) {
	for (int i=0;*p;p++,i++) if (*p == c) return i;
	return -1;
}

// This function reads in characters which are whitespace...
void Arithmetic_Evaluator::ignore_whitespace() 
{
	if (isspace(input.peek())) {input.ignore();}
}

// Check for start of a variable or function name
static bool isvstart(char c)
{
	if (isalpha(c)) return true;
	if (c=='#') return true;
	if (c=='$') return true;
	if (c=='@') return true;
	return false;
}

// Check to see if the character is a continuation of a variable or function name...
static bool isvcont(char c)
{
	if (isvstart(c)) return true;
	if (isalnum(c)) return true;
	if (c=='.') return true;
	if (c=='_') return true;
	return false;
}

// Evaluate quantities enclosed in parentheses...
vector<double>
Arithmetic_Evaluator::
get_paren(bool allow_empty) 
{
	if (input.peek()!='(') {
		throw EVAL_exception("Expected left parentheses: " + input.str());
	}
	input.ignore();

	ignore_whitespace();

	if (allow_empty && input.peek()==')') {
		input.ignore();
		return vector<double>();
	}

	string contents;
	int level=1;
	while (level>0) {
		if (input.eof()) throw EVAL_exception("Mismatched parentheses");
		char next = input.get();
		if (next==')') --level;
		if (next=='(') ++level;
			
		if (level>0) contents += next;		
	}

	return Arithmetic_Evaluator(contents,variables).result;
}

void
Arithmetic_Evaluator::
get_value() 
{
	ignore_whitespace();

	char pre = input.peek();
	int sign = 1;
	if (pre == '-') {
		if (val_stack.size()==0) {
			val_stack.push(0.);
			get_operator();
		} else {
			input.get();
			sign = -1;
		}
	} else if (pre == '+') {
		input.get();
	}

	// First...see if the next token is a number
	double x;
	input >> x;
	if (!input.fail()) {
		// If so, push the number onto the value stack...
		val_stack.push(sign*x);
	} else {
		// If not, fix error and...
		input.clear();

		// check to see if there is a paren group...
		if (input.peek()=='(') {
			vector<double> temp = get_paren();
			val_stack.push(sign*temp[0]);
			for (int i=1;i<temp.size();++i) result.push_back(sign*temp[i]);
		} else {	
			// If not, make sure its's not a right parentheses...
			if (input.peek()==')') throw EVAL_exception("Mismatched parentheses");

			// Check to see if it is a variable...
			if (isvstart(input.peek())) {
				string a;
				while (isvcont(input.peek())) {
					a += input.get();
				}
				ignore_whitespace();
				vector<double> args;
				if (input.peek()=='(') args = get_paren(true);

				ValueVector vv = call_function(a,args);
				val_stack.push(sign*vv[0]);
				for (int i=1;i<vv.size();++i) {
					double x = vv[i];
					result.push_back(sign*vv[i]);
				}
			} else throw EVAL_exception("Invalid value");
		}
	}
	ignore_whitespace();
}

void
Arithmetic_Evaluator::
get_operator() 
{
	ignore_whitespace();
	char next = input.peek();
	int prec=member(next,binops);

	if (prec>=0) {
		while (lower_prec(precs[prec])) operate();
		op_stack.push(input.get());
		prec_stack.push(precs[prec]);
	} 
	ignore_whitespace();
}

void
Arithmetic_Evaluator::
evaluate()
{
	get_value();

	while (!input.eof()) {

		get_operator();
		get_value();
	}

	while (!op_stack.empty()) {
		operate();
	}

	if (!val_stack.empty()) result.insert(result.begin(),val_stack.top());
}

void
Arithmetic_Evaluator::
operate()
{
	int op = op_stack.top();
	op_stack.pop();
	prec_stack.pop();

	double y = val_stack.top();
	val_stack.pop();
	double x = val_stack.top();
	val_stack.pop();

	switch (op) {
	case '+': val_stack.push(x+y); break;
	case '-': val_stack.push(x-y); break;
	case '*': val_stack.push(x*y); break;
	case '/': val_stack.push(x/y); break;
	case '&': val_stack.push(x&&y); break;
	case '|': val_stack.push(x||y); break;
	case '<': val_stack.push(x<y); break;
	case '>': val_stack.push(x>y); break;
	case '=': val_stack.push(x==y); break;
	case '^': val_stack.push(pow(x,y)); break;
	case ',': val_stack.push(x); result.push_back(y); break;
	
	default: throw EVAL_exception("Invalid binary operator");
	}
}

ValueVector
Arithmetic_Evaluator::
call_function(const string& s,const vector<double>& args)
{
	if (s[0]=='@') {
		string filename = s.substr(1); // Remove '@'
		int asize  = args.size();
		if (asize!=1 && asize !=2) 
			throw EVAL_exception("File function " + filename + " requires 1 or 2 arguments");
		int column = asize==2 ? (int)(args[1]) : 2;
		SCATMECH::Table table;
		table.set(filename,column);
		return table.value(args[0]);
	}

    /*
	if (args.size()==1) {
		if (same(s,"exp")) { return exp(args[0]); }
		if (same(s,"sin")) { return sin(args[0]); }
		if (same(s,"cos")) { return cos(args[0]); }
		if (same(s,"tan")) { return tan(args[0]); }
		if (same(s,"sind")) { return sin(args[0]*deg); }
		if (same(s,"cosd")) { return cos(args[0]*deg); }
		if (same(s,"tand")) { return tan(args[0]*deg); }
		if (same(s,"asin")) { return asin(args[0]); }
		if (same(s,"acos")) { return acos(args[0]); }
		if (same(s,"atan")) { return atan(args[0]); }
		if (same(s,"asind")) { return asin(args[0])/deg; }
		if (same(s,"acosd")) { return acos(args[0])/deg; }
		if (same(s,"atand")) { return atan(args[0])/deg; }
		if (same(s,"sinh")) { return sinh(args[0]); }
		if (same(s,"cosh")) { return cosh(args[0]); }
		if (same(s,"tanh")) { return tanh(args[0]); }
		if (same(s,"log")) { return log(args[0]); }
		if (same(s,"log10")) { return log10(args[0]); }
		if (same(s,"sqrt")) { return sqrt(args[0]); }
		if (same(s,"abs")) {return fabs(args[0]); }
		if (same(s,"not")) {return args[0]==0. ?  1. : 0.; }
		if (same(s,"vars")) { 
			if (args[0]>=vars.size()) throw EVAL_exception("Variable " + s + " does not exist");
			return vars[(int)(args[0])]; 
		}
	}

	if (args.size()==2) {
		if (same(s,"atan2")) { return atan2(args[0],args[1]); }
		if (same(s,"atan2d")) { return atan2(args[0],args[1])/deg; }
		if (same(s,"min")) { return (args[0]<args[1]) ? args[0] : args[1]; }
		if (same(s,"max")) { return (args[0]>args[1]) ? args[0] : args[1]; }
		if (same(s,"or")) {return args[0]||args[1];}
		if (same(s,"nor")) {return !(args[0]||args[1]);}
		if (same(s,"and")) {return args[0]&&args[1];}
		if (same(s,"nand")) {return !(args[0]&&args[1]);}
        if (same(s,"resqrt")) return real(sqrt(SCATMECH::COMPLEX(args[0],args[1])));
        if (same(s,"imsqrt")) return imag(sqrt(SCATMECH::COMPLEX(args[0],args[1])));
        if (same(s,"resqr")) return real(SCATMECH::sqr(SCATMECH::COMPLEX(args[0],args[1])));
        if (same(s,"imsqr")) return imag(SCATMECH::sqr(SCATMECH::COMPLEX(args[0],args[1])));
	}

    if (args.size()==3) {
        if (same(s,"if")) { return args[0] ? args[1] : args[2]; }
    }
    */
    double result;
    if (SCATMECH::Evaluate_Function(&result,s,args)) return result;

	if (args.size()==0) {
		ValueVectorMap::const_iterator q = variables.find(s);
		if (q!=variables.end()) return q->second;
	}
	throw EVAL_exception("Invalid function or value: " + s);
}

bool 
Arithmetic_Evaluator::
lower_prec(int prec)
{
	if (prec_stack.empty()) return false;
	if (prec<=prec_stack.top()) return true;
	return false;
}

string
ValueVector::String(bool highprec) const
{
	if (highprec) {
		std::ostringstream oss;
		oss.precision(16);
		if (size()==1) {
			double x = (*this)[0];
			oss << x;
		} else {
			oss << '(';
			for (int i=0;i<size();++i) {
				double x = (*this)[i];
				oss << x;
				if (i!=size()-1) oss << ',';
			}
			oss << ')';
		}
		return oss.str();
	} else {
		std::ostringstream oss;
		if (size()==1) {
			double x = (*this)[0];
			oss << x;
		} else {
			oss << '(';
			for (int i=0;i<size();++i) {
				double x = (*this)[i];
				oss <<  x;
				if (i!=size()-1) oss << ',';
			}
			oss << ')';
		}
		return oss.str();
	}
}


bool same(const std::string& a,const std::string& b)
{
	if (a.size()!=b.size()) return false;
	for (int i=0;i<a.size();++i) 
		if (tolower(a[i])!=tolower(b[i])) return false;
	return true;
}
