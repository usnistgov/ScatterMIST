//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: mist.h
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
#ifndef MIST_H
#define MIST_H

#include <vector>
#include <string>
#include <fstream>

#include "scatmech.h"
#include "mueller.h"
#include "brdf.h"

#ifdef GERMER_MODELS
#include "tagmpi.h"
#endif

#include "mist0.h"
#include "evaluate.h"
#include "loops.h"


class MIST_istream : public SCATMECH::istream_with_comments
{
public:
	MIST_istream(std::istream& _stream) : istream_with_comments(_stream.rdbuf()) {}

	std::string get_token();
	std::string get_variable();
	void get_assignment(std::string& variable,std::string& value); 
}; 

Direction string_to_direction(const std::string& str,const ValueVectorMap& variables);
SCATMECH::StokesVector string_to_Stokes(const std::string& str,const ValueVectorMap& variables);

void banner(std::ostream& out);

class MIST
{
public:
	MIST(MIST_Calculation_Data* _data);

	void run_calculation();
	//void banner(std::ostream& out);
	std::string get_variable();
	void store_samples(double thetas,double phis,double brdf);
	void set_parameter(const std::string& param,const std::string& value);
		
	Variable_List VARIABLES;

	double differential;			// The differential solid angle of integration
	double thetai;					// The incident angle
	double minsamples;				// The number of subdivisions of integral
	double rotation;				// The rotation of the sample (for anisotropic surfaces)
	std::string incidentpol_string;		// String defining the incident polarization
	
	SCATMECH::BRDF_Model::Coordinate_System polarizationbasis;
	std::string MODEL_name;
	SCATMECH::BRDF_Model_Ptr MODEL_model;			// The scattering model
	void MODEL_get(MIST_istream& inputstream);

	// Parameters in the vary statement...
	loop VARY_vars;
	void VARY_get(MIST_istream& inputstream);

	vstring INTEGRALS_name;
	std::vector<SolidShape*> INTEGRALS_shape;
	void INTEGRALS_get(MIST_istream& inputstream);

	vstring OUTPUTS_str;	// A vector of strings containing the arithmetic for calculating signal
	vstring OUTPUTS_label;
	void OUTPUTS_get(MIST_istream& inputstream);

	std::string FILES_results_name;
	std::string FILES_samples_name;
	std::string FILES_listing_name;
    std::string OUTPUT_format;
	bool FILES_samples_bool;
	bool FILES_listing_bool;
	bool FILES_results_bool;
	std::ofstream FILES_samples;
	std::ofstream FILES_results;
	std::ofstream FILES_listing;
	void FILES_get(MIST_istream& inputstream);
	std::deque<std::string> FILES_header;
	void remove_files();
	void concat_files();
	void open_files(bool firsttime=false);
	void close_files();

	MIST_istream InputStream;
	std::ostream* out;
public:
	SCATMECH::StokesVector inputlight;		// The Stokes vector of the incident light

    friend class SolidShape;
	friend struct Variable_List;
	friend class Differential;

	double last_thetai;
	double last_thetas;
	double last_phis;
	double last_rotation;
	SCATMECH::MuellerMatrix last_mueller;
	int polygonsides;
	int integrationmode;

private:
	int order;
	MIST_Calculation_Data* data;
};

inline std::ostream& sendl(std::ostream& os) 
{
	os << '\r' << '\n';
	return os;
}

#endif
