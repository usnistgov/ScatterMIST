//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: shapes.h
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
#ifndef MIST_SHAPES_H
#define MIST_SHAPES_H

#include <vector>
#include <cmath>

#include "scatmech.h"
#include "mueller.h"
#include "mist0.h"

struct Direction {
	Direction(double theta,double phi);
	Direction();

	double t;
	double p;
	double x;
	double y;
	double z;
	double cost;
	double sint;
	double cosp;
	double sinp;
};

class SolidShape {		
    public:
		SolidShape(MIST* mist);

        void get_signal(double t,double p,double r);

		virtual void set_parameters(const Variable_List& variable_list)=0;
		
		virtual double integrate(double domega,int minsamples,double rotation);

		virtual double projected();

		bool inside(const Direction& A) const;
		
		std::vector<Direction> vertex;  

	protected:

		Direction center;
		Direction polcenter;

		SCATMECH::StokesVector sens;

		double solidangle;

		bool isdownward;

		vstring parameter_names;
		vstring parameter_values;
		
		double signal;
		double costheta;
		bool wasinside;

        SCATMECH::StokesVector inpol;

		MIST* mist;
};

class Circle: public SolidShape {
	public:
		Circle(MIST* mist) : SolidShape(mist) {}

    private:
		double alpha;

    public:
		void set_parameters(const Variable_List& variable_list);
		double projected();
};

class Hemisphere: public SolidShape {
    public:
		Hemisphere(MIST* mist) : SolidShape(mist) {}
		void set_parameters(const Variable_List& variable_list);
		double projected() {return SCATMECH::pi;}
	private:
};

class Polygon: public SolidShape {
    private:

    public:
		Polygon(MIST* mist) : SolidShape(mist) {}
		void set_parameters(const Variable_List& variable_list);
};

class Differential: public SolidShape {
    public:
		Differential(MIST* mist) : SolidShape(mist) {}
		void set_parameters(const Variable_List& variable_list);
		double integrate(double domega,int minsamples,double rotation);

	private:	
};

class Annulus: public SolidShape {
    public:
		Annulus(MIST* mist) : SolidShape(mist) {}
		void set_parameters(const Variable_List& variable_list);
		double projected();
	private:
		Direction outercenter;
		Direction innercenter;

		double outeralpha;
		double inneralpha;
		bool overtilt;
};

double rotation_angle(double costi,double sinti,double cospi,double sinpi,
                      double costo,double sinto,double cospo,double sinpo);

double rotation_angle(const Direction& d1,const Direction& d2);

#endif
