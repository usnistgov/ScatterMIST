//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: mist.cpp
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
#include <time.h>
#include <sstream>
#include <fstream>
#include "scatmech.h"
#include "brdf.h"
#include "vector3d.h"
#include "evaluate.h"
#include "mist.h"
#include "mist0.h"
#include "shapes.h"

//
// Germer has his own models, which he links into the program
//
#ifdef GERMER_MODELS
#include "getmodel.h" 
#endif

//
// A compiler may need this function defined...
//
#ifdef NEED_ISWSPACE
inline bool iswspace(char c) {return (( c>=0x09&&c<=0x0D ) || c==0x20);}
#endif


using namespace SCATMECH;
using namespace std;

void set_variable(const string& variable,const string& value);

//
// This function gets the next token in the stream...
//
string MIST_istream::get_token()
{
	string result; 
	bool found=false;

	while (1) {
		// Get the next character in the stream
		char c = get();
		if (iswspace(c))  {
			// Whitespace after beginning of token indicates end of token...
			if (found) {
				return result;
			}
			// Whitespace before beginning of token is ignored
		} else {
			// If the token starts with a double quote, then consider it a string...
			if (!found&&c=='\"') {
				while ((c = get())!='\"') result+=c;
				return result;
			}
			result += c;
			found=true;
		}
	}
}

//
// The following converts a string "(I,Q,U,V)" into a Stokes vector...
//
StokesVector string_to_Stokes(const string& str,const ValueVectorMap& variables)
{
	//string str2 = str.substr(1,str.size()-2);
	Arithmetic_Evaluator ae(str,variables);
	StokesVector result(ae.Value()[0],ae.Value()[1],ae.Value()[2],ae.Value()[3]);
	return result;
}

//
// The following converts a string "(theta,phi)" into a direction...
//
Direction string_to_direction(const string& str,const ValueVectorMap& variables)
{
	//string str2 = str.substr(1,str.size()-2);
	Arithmetic_Evaluator ae(str,variables);
	return Direction(ae.Value()[0],ae.Value()[1]);
}

//
// The following extracts a variable name...
//
string MIST_istream::get_variable()
{
	string result; 
	bool found=false;
	while (1) {
		char c = peek();
		if (iswspace(c)||c=='='||eof())  {
			if (found) {
				return result;
			}
			else get();
		} else {
			c=get();
			result += c;
			found=true;
		}
	}
}

//
// The following makes a variable assignment...
//
void MIST_istream::get_assignment(string& variable,string& value)
{
	variable = get_variable();
	if (same(variable,"end")) return;
	char c;
    while (c=peek(),c!='=') {
        if (fail()) throw MIST_exception("Expecting assignment, i.e., variable = value");
        get();
    }
	get();
	value = get_token();
}

//
// The following sets the value of a variable...
//
void 
Variable_List::
assign(const string& variable,const string& value)
{
	for (int i=0;i<name.size();++i) {
		if (same(name[i],variable)) {
			str[i]=value;
			return;
		}
	}
	throw MIST_exception("Invalid variable: " + variable);
}

//
// The following creates a new variable...
//
void
Variable_List::
new_variable(const string& var,const string& val)
{
	//for (int i=0;i<name.size();++i) {
	//	if (same(var,name[i])) 
	//		throw MIST_exception("Variable " + var + " already defined.");
	//}
	name.push_back(var);
	str.push_back(val);
	if (var[0]!='$') variables[var]=Arithmetic_Evaluator(val,variables).Value();
	last.push_back("#@$");
}

//
// The following extracts the variables from the input stream...
//
void 
Variable_List::
get(MIST_istream& is)
{
	if (!same(is.get_token(),"variables:")) throw MIST_exception("Missing VARIABLES section cue");


	string var;
	string val;
	while (is.get_assignment(var,val),!same(var,"end")) {
		if (same(var,"incidentpol")) mist->incidentpol_string=val;
		new_variable(var,val);
	}
}
 
//
// The following extracts the optical elements from the input stream...
//
void MIST::INTEGRALS_get(MIST_istream& is)
{
	if (!same(is.get_token(),"integrals:")) throw MIST_exception("Missing INTEGRALS section cue");

	string variable;
	string value;
	while (is.get_assignment(variable,value),!same(variable,"end")) {

		if (same(value,"differential")) {
			INTEGRALS_shape.push_back(new Differential(this));
		} else if (same(value,"circle")) {
			INTEGRALS_shape.push_back(new Circle(this));
		} else if (same(value,"hemi")) {
			INTEGRALS_shape.push_back(new Hemisphere(this));
		} else if (same(value,"annulus")) {
			INTEGRALS_shape.push_back(new Annulus(this));
		} else if (same(value,"polygon")) {
			INTEGRALS_shape.push_back(new Polygon(this));
		} else throw MIST_exception("Invalid integral shape: " + value);

		for (int i=0;i<VARIABLES.name.size();++i) {
			if (same(variable,VARIABLES.name[i])) 
				throw MIST_exception("Variable " + variable + " already defined.");
		}
		
		INTEGRALS_name.push_back(variable);
	}
}

//
// The following gets the functions that are computed from the signals...
//
void MIST::OUTPUTS_get(MIST_istream& is)
{
	int i = 1;
	if (!same(is.get_token(),"outputs:")) throw MIST_exception("Missing OUTPUTS section cue");

	string token;

	while (!same((token = is.get_token()) , "end")) {
		if (i!=from_string<int>(token.c_str())) throw MIST_exception("Missing or incorrect OUTPUTS section number");
		string function=is.get_token();
		int pos = function.find('=');
		if (pos==string::npos) {
			OUTPUTS_str.push_back(function);
			OUTPUTS_label.push_back(function);
		} else {
			string label=function.substr(0,pos);
			OUTPUTS_label.push_back(label);
			OUTPUTS_str.push_back(function.substr(pos+1,function.size()));
		}
		++i;
	}
}

//
// The following extracts the vary statement parameters...
//
void MIST::VARY_get(MIST_istream& is)
{
	if (!same(is.get_token(),"vary:")) throw MIST_exception("Missing VARY section cue");

	string token;

	while (!same((token = is.get_token()) , "end")) {
		
		if (same(token,"for")) {
			string name = is.get_token();
			if (same(is.get_token(),"from")) {
				string from = is.get_token();
				if (!same(is.get_token(),"to")) throw MIST_exception("Missing TO in vary statement");
				string to = is.get_token();
				if (!same(is.get_token(),"by")) throw MIST_exception("Missing BY in vary statement");
				string by = is.get_token();
				VARY_vars.push(new for_loop_variable(name,from,to,by,VARIABLES));
			} else {
				throw MIST_exception("Missing FROM in vary statement");
			}
		} else if (same(token,"list")) {
			string name = is.get_token();
			VARY_vars.push(new table_loop_variable(name,VARIABLES));
#ifdef GERMER_MODELS		
		} else if (same(token,"mpi_list")) {
			string name = is.get_token();
			VARY_vars.push(new mpi_table_loop_variable(name,VARIABLES));
		} else 	if (same(token,"mpi_for")) {
			string name = is.get_token();
			if (same(is.get_token(),"from")) {
				string from = is.get_token();
				if (!same(is.get_token(),"to")) throw MIST_exception("Missing TO in mpi_vary statement");
				string to = is.get_token();
				if (!same(is.get_token(),"by")) throw MIST_exception("Missing BY in mpi_vary statement");
				string by = is.get_token();
				VARY_vars.push(new mpi_for_loop_variable(name,from,to,by,VARIABLES));
			} else {
				throw MIST_exception("Missing FROM in mpi_vary statement");
			}
#endif
		} else {
			throw MIST_exception("Missing FOR or LIST statement in VARY section");
		}
	}
}

//
// The following gets and initializes the SCATMECH scattering model...
//
void MIST::MODEL_get(MIST_istream& is)
{
	string name,value;
	is.get_assignment(name,value);
	if (!same(name,"model")) {
		cerr << endl << name << endl;
		throw MIST_exception("Missing MODEL statement. Found " + name + " instead.");
	}
	MODEL_model = value;
	MODEL_name = value;
	if (MODEL_model.get()==NULL) throw MIST_exception("Invalid model: " + value);
}

void MIST::store_samples(double theta,double phi,double signal)
{
	double x = sin(theta)*cos(phi);
	double y = sin(theta)*sin(phi);
	double z = cos(theta);
	data->samples_x[order].push_back(x);
	data->samples_y[order].push_back(y);
	data->samples_i[order].push_back(signal);
	
	if (FILES_samples_bool) {
		FILES_samples << order << tab << theta << tab << phi << tab << x << tab << y << tab << z << tab << signal/z << endl;
	}
}

//
// The following puts a banner on the screen...
//
void banner(ostream& out)
{
    out << "Modeled Integrated Scatter Tool (MIST)" << sendl
		<< "Sensor Science Division" << sendl
		<< "National Institute of Standards and Technology" << sendl
		<< "Version " << MIST_VERSION << " (Compiled " << __DATE__ << ' ' << __TIME__ << ")" << sendl
		<< "http://physics.nist.gov/scatmech" << sendl << sendl;
}

//
// The following gets information about output files...
//
void MIST::FILES_get(MIST_istream& is)
{
	if (!same(is.get_token(),"files:")) throw MIST_exception("Missing files statement");
	FILES_results_name="";
	FILES_samples_name="";
	FILES_listing_name="";
	FILES_results_bool=true;
	FILES_listing_bool=true;
	FILES_samples_bool=false;
    OUTPUT_format="%10g\t";

	string variable;
	string value;
	while (is.get_assignment(variable,value),!same(variable,"end")) {
		if (same(variable,"results")) FILES_results_name=value;
		else if (same(variable,"samples")) FILES_samples_name=value;
		else if (same(variable,"listing")) FILES_listing_name=value;
		else if (same(variable,"header")) FILES_header.push_back(value);
        else if (same(variable,"format")) OUTPUT_format=value;
		else if (same(variable,"basis")) {
			if (value=="psps") polarizationbasis = BRDF_Model::psps;
			else if (value=="xyxy") polarizationbasis = BRDF_Model::xyxy;
			else if (value=="plane") polarizationbasis = BRDF_Model::plane;
			else throw MIST_exception("Invalid polarization basis: " + value);
		}
		else throw MIST_exception("Invalid output statement");
	}

    int loc;
    while ((loc=OUTPUT_format.find("\\t"))!=string::npos) {
        OUTPUT_format.replace(loc,2,"\t");
    }

	if (FILES_results_name=="") FILES_results_name=data->jobname+".dat";
	if (FILES_listing_name=="") FILES_listing_name=data->jobname+".lst";

#ifdef GERMER_MODELS
	if (FILES_samples_name!=""&&MyMPI_GetSize()==1) FILES_samples_bool=true;
#else
	if (FILES_samples_name!="") FILES_samples_bool=true;
#endif

#ifdef GERMER_MODELS
	MyMPI_WaitForMyTurn();
	open_files(MyMPI_GetRank()==0);
#else
	open_files(true);
#endif

#ifdef GERMER_MODELS
	if (MyMPI_GetSize()!=1) cerr << "MPI Rank = " << MyMPI_GetRank() << "/" << MyMPI_GetSize() << endl;
#endif

#ifdef GERMER_MODELS
	if (MyMPI_GetRank()==0) {
#else
	{
#endif
		for (deque<string>::iterator p = FILES_header.begin(); p!=FILES_header.end() ; ++p) {
			FILES_results << *p << endl;
		}
		
		std::deque<loop_variable*>::iterator pvars;
		for (pvars=VARY_vars.vars.begin();pvars!=VARY_vars.vars.end();++pvars) 
		{
			for (int i=0;i<(*pvars)->get_nvar();++i) {
				string var = (*pvars)->get_variable(i);
				FILES_results << var << tab;
			}
		}

		for (int i=0;i<OUTPUTS_label.size();++i) {
			FILES_results << OUTPUTS_label[i] << tab;
		}

		FILES_results << endl;

		banner(FILES_listing);
		FILES_listing << "Integrating model: " << MODEL_name << endl
				  << "Listing file: " << FILES_listing_name << endl
				  << "Samples file: " << (FILES_samples_bool ? FILES_samples_name : "none") << endl
				  << "Results file: " << FILES_results_name << endl;

		for (deque<string>::iterator pp = FILES_header.begin(); pp!=FILES_header.end() ; ++pp) {
			FILES_listing << "Header: " << *pp << endl;
		}

		if (FILES_samples_bool) {
			FILES_samples << "order\tthetas\tphis\tx\ty\tz\tBRDF" << endl;
		}
	}
	

	for (deque<string>::iterator p = FILES_header.begin(); p!=FILES_header.end() ; ++p) {
		if (data->save_internal) data->results << *p << sendl;
	}
	
	std::deque<loop_variable*>::iterator pvars;
	for (pvars=VARY_vars.vars.begin();pvars!=VARY_vars.vars.end();++pvars) 
	{
		for (int i=0;i<(*pvars)->get_nvar();++i) {
			string var = (*pvars)->get_variable(i);
			if (data->save_internal) data->results << var << tab;
			data->xnames.push_back(var);
		}
	}

	for (int i=0;i<OUTPUTS_label.size();++i) {
		if (data->save_internal) data->results << OUTPUTS_label[i] << tab;
		data->ynames.push_back(OUTPUTS_label[i]);
	}

	if (data->save_internal) data->results << sendl;

	if (data->save_internal) {
		banner(data->listing);
		data->listing << "Integrating model: " << MODEL_name << sendl
				  << "Listing file: " << (FILES_listing_bool ? FILES_listing_name : "none") << sendl
				  << "Samples file: " << (FILES_samples_bool ? FILES_samples_name : "none") << sendl
				  << "Results file: " << (FILES_results_bool ? FILES_results_name : "none") << sendl;
	}

	{
		for (deque<string>::iterator p = FILES_header.begin(); p!=FILES_header.end() ; ++p) {
			if (data->save_internal) data->listing << "Header: " << *p << sendl;
		}
	}

	data->xvalues.resize(data->xnames.size());
	data->yvalues.resize(data->ynames.size());

	close_files();
#ifdef GERMER_MODELS
	MyMPI_DoneWithMyTurn();
#endif
}

void 
MIST::
set_parameter(const string& param,const string& value) 
{
	if (data->save_internal) {
		data->listing << "Setting parameter " << param << " to " << value << sendl;
	}
	if (FILES_listing_bool) {
	  if (!FILES_listing) throw MIST_exception("Running set_parameter with closed listing file");
#ifdef GERMER_MODELS
	  if (MyMPI_GetSize()!=1) FILES_listing << "Node " << MyMPI_GetRank() << ": ";
#endif
	  FILES_listing << "Setting parameter " << param << " to " << value << endl;
	}
	MODEL_model->set_parameter(param,value);
}

void MIST::open_files(bool firsttime) 
{
  if (firsttime) {
	FILES_results.open(FILES_results_name.c_str(),ios::out);
	FILES_listing.open(FILES_listing_name.c_str(),ios::out);
	if (FILES_samples_bool) FILES_samples.open(FILES_samples_name.c_str(),ios::out);

  } else {
#ifdef GERMER_MODELS
	  if (MyMPI_GetSize()!=1) {
#else
	  {
#endif
		FILES_results.open(FILES_results_name.c_str(),ios::out|ios::app);
		FILES_listing.open(FILES_listing_name.c_str(),ios::out|ios::app);
		if (FILES_samples_bool) FILES_samples.open(FILES_samples_name.c_str(),ios::out|ios::app);
	  }
  }
  if (!FILES_results) throw MIST_exception("Problem opening " + FILES_results_name);
  if (!FILES_listing) throw MIST_exception("Problem opening " + FILES_listing_name);
  if (FILES_samples_bool && !FILES_samples) throw MIST_exception("Problem opening " + FILES_samples_name);

}

void MIST::close_files() 
{
#ifdef GERMER_MODELS
	if (MyMPI_GetSize()!=1) {
#else
	{
#endif
		FILES_results.close();
		FILES_listing.close();
		if (FILES_samples_bool) FILES_samples.close();
	}
}

void 
MIST::
run_calculation() 
{
	// Loop through all of the parameters to calculate their values and 
	// to send to the SCATMECH model...
	string current_variable;
	try {
		VARIABLES.variables["integrationmode"]=2.;
		VARIABLES.variables["polygonsides"]=360.;
		VARIABLES.variables["pi"]=pi;
		VARIABLES.variables["deg"]=pi/180.;

#ifdef GERMER_MODELS
		MyMPI_WaitForMyTurn();
#endif
		open_files();

		for (int j=0;j<VARIABLES.name.size();++j) {
			// If this parameter is the varied parameter, assign the value from above...
			//if (same(VARY.name,VARIABLES.name[j])) VARIABLES.value[j]=param;

			current_variable = VARIABLES.name[j];
			string send;
			// If the name of the parameter starts with '#' evaluate the expression and send to BRDF_Model... 
			if (current_variable[0]=='#') {
				// Evaluate the expression...
				Arithmetic_Evaluator ae(VARIABLES.str[j],VARIABLES.variables);

				// Get the string equivalent...
				send = ae.Value().String();
 
				VARIABLES.variables[current_variable] = ae.Value();

				// If it has changed from the previous time...
				if (send!=VARIABLES.last[j]) {
					// Send it to the model...
					set_parameter(current_variable.substr(1),send);
				}
			// If the name of the parameter starts with '$' then send the value string directly to BRDF_Model...
			} else if (current_variable[0]=='$') {
				send = VARIABLES.str[j];
				// If it has changed from the previous time...
				if (send!=VARIABLES.last[j]) {
					// Send it to the model...
					set_parameter(current_variable.substr(1),send);
				}
			} else {
				// Evaluate the expression and assign to the variable if it is an ordinary MIST variable...
				VARIABLES.variables[current_variable] = Arithmetic_Evaluator(VARIABLES.str[j],VARIABLES.variables).Value();
			}
			// Keep track of the last values of those parameters which get sent to BRDF_Model...
			VARIABLES.last[j]=send;
		}

		// Get the values of incident angle, solid angle, and evaluate the input stokes parameter...
		current_variable = "incidentangle";
		thetai = Arithmetic_Evaluator(current_variable,VARIABLES.variables).Value();
		current_variable = "differential";
		differential = Arithmetic_Evaluator(current_variable,VARIABLES.variables).Value();
		current_variable = "incidentpol";
		inputlight = string_to_Stokes(incidentpol_string,VARIABLES.variables);
		current_variable = "minsamples";
		minsamples = Arithmetic_Evaluator(current_variable,VARIABLES.variables).Value();
		current_variable = "rotation";
		rotation = Arithmetic_Evaluator(current_variable,VARIABLES.variables).Value();
		current_variable = "integrationmode";
		integrationmode = (int)Arithmetic_Evaluator(current_variable,VARIABLES.variables).Value();
		current_variable = "polygonsides";
		polygonsides = (int)Arithmetic_Evaluator(current_variable,VARIABLES.variables).Value();

		// The extended arrays contain the integral values, in addition to the other variables...
		//vector<ValueVector> extended_values=VARIABLES.value;
		//vector<string> extended_names=VARIABLES.name;

		close_files();

#ifdef GERMER_MODELS
		MyMPI_DoneWithMyTurn();
#endif

		// Evaluate each of the integrals...
		for (int k=0;k<INTEGRALS_shape.size();++k) {
			current_variable = INTEGRALS_name[k];
			// Set the parameters for the integral domain (shape)...
			INTEGRALS_shape[k]->set_parameters(VARIABLES); 
			// Perform the integration...
			// Check and indicate progress...
			if (data->notifier) {
				if (data->notifier->Stop()) throw MIST_exception("Simulation Cancelled");
				data->notifier->SetProgress(VARY_vars.get_progress());
			}
			// Record the values...
			double x = INTEGRALS_shape[k]->integrate(differential,(int)minsamples,rotation);
			VARIABLES.variables[current_variable]=x;
		}

		// Output the results to the results file...
#ifdef GERMER_MODELS
		MyMPI_WaitForMyTurn();
#endif
		open_files();

		int ix=0;
		for (std::deque<loop_variable*>::iterator pvars=VARY_vars.vars.begin();pvars!=VARY_vars.vars.end();++pvars) {
			for (int i=0;i<(*pvars)->get_nvar();++i) {
                std::string vstring = (*pvars)->get_value(i); 
                if ((*pvars)->get_variable(i)[0]=='$') {
			        if (data->save_internal) {
				        data->results << vstring << tab;
			        }
			        if (FILES_results_bool) {
				        FILES_results << vstring << tab;
			        }
   				    data->xvalues[ix++].push_back(0.);
                } else {
                    //double x = Arithmetic_Evaluator(vstring,VARIABLES.value,VARIABLES.name).Value();
                    //string xstr = format(OUTPUT_format,x);
					Arithmetic_Evaluator eval(vstring,VARIABLES.variables);
                    string xstr = eval.Value().String(false);
					double x = eval.Value();
				    if (data->save_internal) {
					    //data->results.width(10);
					    data->results << xstr << tab;
				    }
				    if (FILES_results_bool) {
					    //FILES_results.width(10);
					    FILES_results << xstr << tab;
				    }
   				    data->xvalues[ix++].push_back(x);
                }
			}
		}

		for (int iy=0;iy<OUTPUTS_str.size();++iy) {
			// Evaluate the expression...
			current_variable = OUTPUTS_str[iy];
			double y = Arithmetic_Evaluator(current_variable,VARIABLES.variables).Value();
            string ystr = format(OUTPUT_format,y);
			// Send it to the file...
			if (data->save_internal) {
				//data->results.width(10);
				data->results << ystr;
			}
			if (FILES_results_bool) {
				//FILES_results.width(10);
				FILES_results << ystr;
			}
			data->yvalues[iy].push_back(y);
			VARIABLES.variables[OUTPUTS_label[iy]] =  y;
		}
		if (data->save_internal) data->results << sendl;
		if (FILES_results_bool) FILES_results << endl;

		data->samples_thetai.push_back(thetai);
		++order;
		data->samples_x.push_back(vdouble());
		data->samples_y.push_back(vdouble());
		data->samples_i.push_back(vdouble());

		close_files();
#ifdef GERMER_MODELS
		MyMPI_DoneWithMyTurn();
#endif

	} catch (exception& e) {
		if (FILES_listing_bool) {
			FILES_listing << "Error evaluating " << current_variable << ": " << e.what() << endl;
		}
		throw MIST_exception("Error evaluating " + current_variable + "\r\n" + e.what());
	} catch (...) {
			if (FILES_listing_bool) {
				FILES_listing << "Uncaught exception in run_calculation()" << endl;
			}
	        throw MIST_exception("Uncaught exception in run_calculation()");
	}
}

MIST::
MIST(MIST_Calculation_Data* _data) : InputStream(*(_data->is)),out(_data->os)
{
	data=_data;

	data->reset();

	order = 0;

	time_t start,finish;
	
	time(&start);

	Register((Model*)0);

#ifdef GERMER_MODELS
	Register_Germer_Models(); 
	//Register_Special_Models();
#endif

	ClearTableCache();

	VARIABLES.set_MIST(this);
	
	FILES_samples_bool=FILES_listing_bool=FILES_results_bool=false;

	polarizationbasis = BRDF_Model::psps;

	differential = 0.001;
	thetai = 0;
	inputlight = StokesVector(1,-1,0,0);
	minsamples = 1;
	rotation = 0;

	last_thetai = -1000;
	last_thetas = -1000;
	last_phis = -1000;
	last_rotation = -1000;
	last_mueller = MuellerZero();

	//SCATMECH_set_istream(InputStream.stream);
	set_ostream(*out);

	MODEL_get(InputStream);
	VARIABLES.get(InputStream);
	VARY_get(InputStream);
	INTEGRALS_get(InputStream);
	OUTPUTS_get(InputStream);
	FILES_get(InputStream);

	order = 0;
	data->samples_x.push_back(vdouble());
	data->samples_y.push_back(vdouble());
	data->samples_i.push_back(vdouble());

	for (VARY_vars.reset();VARY_vars.test();++VARY_vars) {
		run_calculation();
	}

	time(&finish);
	double seconds = difftime(finish,start);
	double sec = fmod(seconds,60.);
	int min = ((int)seconds/60)%60;
	int hrs = (int)seconds/3600;

#ifdef GERMER_MODELS
	MyMPI_Checkpoint();
#endif

#ifdef GERMER_MODELS
	if (MyMPI_GetRank()==0) {
#else
	{
#endif

		*out << endl 
			<< "Elapsed time: " << seconds << " s (" << hrs << " hrs. " << min << " min. " << sec << " s)"<< endl;
		if (data->save_internal) data->listing << "Elapsed time: " << seconds << " s (" << hrs << " hrs. " << min << " min. " << sec << " s)"<< sendl; 
		
		open_files();
	
		FILES_listing << "Elapsed time: " << seconds << " s (" << hrs << " hrs. " << min << " min. " << sec << " s)"<< endl; 
	}

	if (FILES_results.is_open()) FILES_results.close();
	if (FILES_samples.is_open()) FILES_samples.close();
	if (FILES_listing.is_open()) FILES_listing.close();
}


class CMD_Line_Notifier : public MIST_Notifier {
public:
#ifdef USE_MPI
	void SetProgress(double fraction) {}
#else
	CMD_Line_Notifier() : last(0) {}
	void SetProgress(double fraction) {
		int prog=(int)(fraction*100.);
		if (prog==last) return;
		cerr << "Working: " << prog << " %" << ret; 
		last=prog;
	}
private:
	int last;
#endif
};

string stripextension(const char* a)
{
	string result = a;
	if (result.find('.')==string::npos) return result;
	result = result.substr(0,result.find_last_of('.'));
	return result;
}

//#include "FEM.h"

int main(int argv,char **argc)
{

#ifdef GERMER_MODELS
	MyMPI_Init(argv,argc);
#endif

  CMD_Line_Notifier notifier;
	banner(cerr);

    try {
		if (argv>1) {
			string jobname = stripextension(argc[1]);
			ifstream file(argc[1]);
			if (!file) throw MIST_exception(string("Cannot open file: ") + argc[1]);
		
			if (argv==3) {
				jobname=argc[2];
			}
			MIST_Calculation_Data data(jobname,&file,&cerr,&notifier);
			MIST mist(&data);
		} else {
			string filename = AskUser("MIST program:","");
			ifstream file(filename.c_str());
			string jobname = stripextension(filename.c_str());
			if (!file) throw MIST_exception("Cannot open file: " + filename);
			MIST_Calculation_Data data(jobname,&file,&cerr,&notifier);
			MIST mist(&data);
		}
	} 
	catch (exception& e) {
        cerr << e.what() << endl;
		cerr << "Press return to continue";
		cin.get();
        }
    catch (...) {
	  cerr << "Uncaught exception in main()" << endl;
	}

#ifdef GERMER_MODELS
	MyMPI_Finalize();
#endif

	return 0;
}


