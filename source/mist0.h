//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: mist0.h
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
#ifndef MIST0_H
#define MIST0_H

#include <iostream>
#include <deque>
#include <sstream>
#include <fstream>

class SolidShape;
struct Direction;
class MIST_istream;
struct Variable_List;
class Differential;
class MIST;
class loop_variable;

typedef std::deque<double> vdouble;
typedef std::deque<vdouble> vvdouble;
typedef std::deque<std::string> vstring;

class MIST_Notifier {
public:
	MIST_Notifier() {}

	virtual void SetProgress(double fraction) {return;}
	virtual bool Stop() {return false;}
	virtual void SetDone() {}
};

class MIST_Calculation_Data {
public:

	MIST_Calculation_Data(std::string& _jobname,std::istream* _is,std::ostream* _os,MIST_Notifier* _notifier=NULL) 
		: jobname(_jobname),is(_is),os(_os),notifier(_notifier),save_internal(true) { reset();}

	MIST_Calculation_Data() : is(&std::cin),os(&std::cerr),notifier(NULL) {reset();}

	void reset() {	
		results.str("");
		listing.str("");
		samples.str("");
		results.seekp(0,std::ios::beg);
		listing.seekp(0,std::ios::beg);
		samples.seekp(0,std::ios::beg);
		results.clear();
		listing.clear();
		samples.clear();

		xvalues.resize(0);
		yvalues.resize(0);
		xnames.resize(0);
		ynames.resize(0);

		samples_x.resize(0);
		samples_y.resize(0);
		samples_i.resize(0);
		samples_thetai.resize(0);
	}
		
public: 

	MIST_Notifier* notifier;
	std::istream* is;
	std::ostream* os; 
	std::string jobname;

	std::ostringstream results;
	std::ostringstream listing;
	std::ostringstream samples;

	vvdouble xvalues;
	vvdouble yvalues;
	vstring xnames;
	vstring ynames;

	vvdouble samples_x;
	vvdouble samples_y;
	vvdouble samples_i;
	vdouble samples_thetai;

	bool save_internal;
};

//unsigned int MIST_Calculation(MIST_Calculation_Dat* p);
extern "C" {
	unsigned int MIST_Calculation(void* p);
}

#endif
