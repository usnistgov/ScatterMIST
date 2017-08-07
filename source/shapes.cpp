//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: shapes.cpp
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
#include "shapes.h"
#include "mist.h"
#include <string>
#include "matrix3d.h"
#include "random.h"

using namespace std;
using namespace SCATMECH;

Random_Number random1= Random_Number::uniform(-1.,1.);

Direction::Direction(double theta,double phi) 
{
	t=theta;
	p=phi;
	sint = sin(t);
	cost = cos(t);
	sinp = sin(p);
	cosp = cos(p);
	x = sint*cosp;
	y = sint*sinp;
	z = cost;
}

Direction::Direction() {
	t=0;
	p=0;
	sint = 0;
	cost = 1;
	sinp = 0;
	cosp = 1;
	x = 0;
	y = 0;
	z = 1;
}


SolidShape::SolidShape(MIST* _mist) : mist(_mist)
{
	string variable;
	string value;
	while (mist->InputStream.get_assignment(variable,value),!same(variable,"end")) {
		parameter_names.push_back(variable);
		parameter_values.push_back(value);
	}
	isdownward=false;
}


MuellerMatrix ModelMueller(MIST* mist,double thetai,double thetas,double phis,double rotation)
{
	if (thetai!=mist->last_thetai ||
		thetas!=mist->last_thetas ||
		phis!=mist->last_phis ||
		rotation!=mist->last_rotation ||
		mist->MODEL_model->get_recalc()) {
			mist->last_mueller = mist->MODEL_model->Mueller(thetai,thetas,phis,rotation);
			mist->last_thetai=thetai;
			mist->last_thetas=thetas;
			mist->last_phis=phis;
			mist->last_rotation=rotation;
	}
	return mist->last_mueller;
}

double SolidShape::integrate(double domega,int minsamples,double rotation)
{
	minsamples = (int)(sqr((double)(int)(sqrt((double)minsamples)+0.9999999)));

	isdownward = mist->MODEL_model->get_type()==1 || mist->MODEL_model->get_type()==2; 

	if (solidangle/domega<minsamples) {
		domega=solidangle/minsamples;
	}

	int mode = mist->integrationmode%10;
	bool normalize = mist->integrationmode/10 == 0;

	double alpha = acos(1.-solidangle/2./pi);
	double result = 0;
	double norm = 0;
	double dxdy;

	switch (mode) {
	case 1: {
			//
			// 
			// This modes makes the change of variables in integrates on an evenly spaced grid
			// xi = theta*cos(phi), eta = theta*sin(phi)
			//
			double dx = sqrt(domega),dy=dx;

			double xymax = alpha;
			int n = (int)(2*xymax/dx+0.5)/2;
			for (int i=-n;i<=n;++i) {
				for (int j=-n;j<=n;++j) {
					if (mist->data->notifier) if (mist->data->notifier->Stop()) throw MIST_exception("Simulation Cancelled");

					double x = i*dx ;
					double y = j*dy;
					double z2 = sqr(x)+sqr(y);
					if (z2<=sqr(xymax)) {
						double theta = sqrt(z2);
						double phi = (i!=0||j!=0) ? atan2(y,x) : 0;
						get_signal(theta,phi,rotation); 
						if (wasinside && costheta>0) {
							double sinc = (theta!=0.) ? sin(theta)/theta : 1.;
							norm += sinc*costheta;
							result += sinc*signal;
						}
					} 
				}
			}
			dxdy = dx*dy;
		} 
		break;
	case 2: {
			//
			// This modes makes the change of variables in integrates on an evenly spaced grid
			// xi = sqrt(1-cos(theta))*cos(phi), eta = sqrt(1-cos(theta))*sin(phi)
			// In this case, solid angle subtended by points is a constant.
			//
			double dx = sqrt(domega/2.),dy=dx;

			double xymax = sqrt(1.-cos(alpha));
			int n = (int)(2*xymax/dx+0.5)/2;
			for (int i=-n;i<=n;++i) {
				for (int j=-n;j<=n;++j) {
					if (mist->data->notifier) if (mist->data->notifier->Stop()) throw MIST_exception("Simulation Cancelled");

					double x = i*dx ;
					double y = j*dy;
					double z2 = sqr(x)+sqr(y);
					if (z2<=sqr(xymax)) {
						double theta = acos(1.-z2);
						double phi = (i!=0||j!=0) ? atan2(y,x) : 0;
						get_signal(theta,phi,rotation); 
						if (wasinside && costheta > 0) {
							norm += costheta;
							result += signal;
						}
					} 
				}
			}
			dxdy = 2.*dx*dy;
		} 
		break;
	case 3: {
			//
			// This modes makes the change of variables in integrates on an evenly spaced grid
			// xi = sin(theta)*cos(phi), eta = sin(theta)*sin(phi)
			// In this case, projected solid angle subtended by points is constant. This method does not
			// converge very quickly when the scattering is at large angles.
			//
			double dx = sqrt(domega),dy=dx;

			double xymax = sin(alpha);
			int n = (int)(2*xymax/dx+0.5)/2;
			for (int i=-n;i<=n;++i) {
				for (int j=-n;j<=n;++j) {
					if (mist->data->notifier) if (mist->data->notifier->Stop()) throw MIST_exception("Simulation Cancelled");

					double x = i*dx;
					double y = j*dy;
					double z2 = sqr(x)+sqr(y);
					if (z2<=sqr(xymax)) {
						double theta = asin(sqrt(z2));
						double phi = (i!=0||j!=0) ? atan2(y,x) : 0;
						get_signal(theta,phi,rotation); 
						if (wasinside && costheta > 0 ) {
							norm += costheta/cos(theta);
							result += signal/cos(theta);
						}
					} 
				}
			}
			dxdy = dx*dy;
		} 
		break;
	case 4: {
			//
			// 
			// This modes makes the change of variables and Monte Carlo integrates on 
			// xi = theta*cos(phi), eta = theta*sin(phi)
			//
			double dx = sqrt(domega),dy=dx;

			double xymax = alpha;
			int N = pi*sqr(xymax)/(dx*dy);
			for (int i=0;i<N;) {
				double x = random1()*xymax;
				double y = random1()*xymax;
				double z2 = sqr(x)+sqr(y);
				if (z2<sqr(xymax)) {
					if (mist->data->notifier) if (mist->data->notifier->Stop()) throw MIST_exception("Simulation Cancelled");

					if (z2<=sqr(xymax)) {
						++i;
						double theta = sqrt(z2);
						double phi = (theta!=0.) ? atan2(y,x) : 0;
						get_signal(theta,phi,rotation); 
						if (wasinside && costheta>0) {
							double sinc = (theta!=0.) ? sin(theta)/theta : 1.;
							norm += sinc*costheta;
							result += sinc*signal;
						}
					} 
				}
			}
			dxdy = pi*sqr(xymax)/N;
		} 
		break;
	case 5: {
			//
			// 
			// This modes makes the change of variables and Monte Carlo integrates on 
			// xi = theta*cos(phi), eta = theta*sin(phi)
			//
			double dx = sqrt(domega/2.),dy=dx;

			double xymax = sqrt(1-cos(alpha));
			int N = pi*sqr(xymax)/(dx*dy);
			for (int i=0;i<N;) {
				double x = random1()*xymax;
				double y = random1()*xymax;
				double z2 = sqr(x)+sqr(y);
				if (z2<sqr(xymax)) {
					if (mist->data->notifier) if (mist->data->notifier->Stop()) throw MIST_exception("Simulation Cancelled");

					if (z2<=sqr(xymax)) {
						++i;
						double theta = acos(1.-z2);
						double phi = (theta!=0.) ? atan2(y,x) : 0;
						get_signal(theta,phi,rotation); 
						if (wasinside && costheta>0) {
							norm += costheta;
							result += signal;
						}
					} 
				}
			}
			dxdy = 2*pi*sqr(xymax)/N;
		}
		break;
	case 6: {
			//
			// This modes makes the change of variables and Monte Carlo integrates on 
			// xi = sin(theta)*cos(phi), eta = sin(theta)*sin(phi)
			// In this case, projected solid angle subtended by points is constant. This method does not
			// converge very quickly when the scattering is at large angles.
			//
			double dx = sqrt(domega),dy=dx;

			double xymax = sin(alpha);
			int N = pi*sqr(xymax)/(dx*dy);
			for (int i=0;i<N;) {
				double x = random1()*xymax;
				double y = random1()*xymax;
				double z2 = sqr(x)+sqr(y);
				if (z2<sqr(xymax)) {
					if (mist->data->notifier) if (mist->data->notifier->Stop()) throw MIST_exception("Simulation Cancelled");

					if (z2<=sqr(xymax)) {
						++i;
						double theta = asin(sqrt(z2));
						double phi = (theta!=0.) ? atan2(y,x) : 0;
						get_signal(theta,phi,rotation); 
						if (wasinside && costheta>0) {
							norm += costheta/cos(theta);
							result += signal/cos(theta);
						}
					} 
				}
			}
			dxdy = pi*sqr(xymax)/N;
		}
		break;
	default:
		throw MIST_exception("Invalid value for integralmode");
	}

	if (!normalize) {
		result *= dxdy;
	} else {
		norm /= projected();		
		result /= norm;
	}

	return result;
}

void SolidShape::get_signal(double t,double p,double rotation) 
{
	signal = 0;
	costheta = 1;
	wasinside = false;

	Direction R(t,p);

    // First...rotate about y axis 
    double twistz =  center.cost*R.z-center.sint*R.x;
    double twistx =  center.sint*R.z+center.cost*R.x;
    double twisty = R.y;
     
    // Next...rotate about z axis 
    double rotx= center.cosp*twistx-center.sinp*twisty;
    double roty= center.sinp*twistx+center.cosp*twisty;
    double rotz = twistz;
    
	if (rotz<=0) {
		return;
	}

	double thetas = acos(rotz);
	double phis = atan2(roty,rotx);

	// R is the new direction
	R = Direction(thetas,phis);

	if (inside(R)) {
		
		//double beta = rotation_angle(polcenter.cost,polcenter.sint,polcenter.cosp,polcenter.sinp,R.cost,R.sint,R.cosp,R.sinp);
		double beta = rotation_angle(polcenter,R);
		if (isdownward) beta = -beta;
		StokesVector sens_rot = sens.rotate(beta);

		double brdf = sens_rot*(ModelMueller(mist,mist->thetai,thetas,phis,rotation)*inpol);

		signal = brdf*rotz;
		mist->store_samples(thetas,phis,signal);

		costheta = rotz;
		wasinside = true;
	} 
}

bool SolidShape::inside(const Direction& A) const
{
	int count=0;
	for (int i=0;i<vertex.size();++i) {
		int ii= (i!=vertex.size()-1) ? i+1 : 0;
		
		double x1 = vertex[i].x;
		double x2 = vertex[ii].x;
		double y1 = vertex[i].y;
		double y2 = vertex[ii].y;

		if (x1<A.x && x2<=A.x) {}
		else if (x1>A.x && x2>=A.x) {}
		else if (y1<A.y && y2<=A.y) {}
		else if (y1>=A.y && y2>A.y) {
			if ((x1>=A.x && x2<A.x)||(x1<=A.x && x2>A.x)) ++count;
		}
		else {
			double yc = y1+(y2-y1)*(A.x-x1)/(x2-x1);
			if (yc>A.y) ++count;
		}
	}

	return count%2 == 1;
}

double SolidShape::projected()
{
	double area=0;

	for (int i=0;i<vertex.size();++i) {

		int ii= (i!=vertex.size()-1) ? i+1 : 0;
		
		double x1 = vertex[i].x;
		double x2 = vertex[ii].x;
		double y1 = vertex[i].y;
		double y2 = vertex[ii].y;

		area += x1*y2-x2*y1;
	}

	//if (mist->data->save_internal) mist->data->listing << "Projected area: " << 0.5*fabs(area) << sendl;
	//if (mist->FILES_listing_bool) mist->FILES_listing << "Projected area: " << 0.5*fabs(area) << endl;
	return 0.5*fabs(area);
}

double Circle::projected() 
{
	if (center.t+alpha<=pi/2) return pi*sqr(sin(alpha))*cos(center.t);
	else return SolidShape::projected();
}

void Circle::set_parameters(const Variable_List& variable_list)
{
	// Initialize defaults...
	center = Direction(0,0);
	alpha = 0;
	sens = StokesVector(1,0,0,0);
    inpol = mist->inputlight;

	// Interpret all of the parameters...
	for (int i=0;i<parameter_names.size();++i) {
		string variable = parameter_names[i];
		//double value = Arithmetic_Evaluator(parameter_values[i],variable_list.value,variable_list.name).Value();
		if (same(variable,"center")) center = string_to_direction(parameter_values[i],variable_list.variables);
		else if (same(variable,"alpha")) alpha = Arithmetic_Evaluator(parameter_values[i],variable_list.variables).Value();
		else if (same(variable,"pol")) sens = string_to_Stokes(parameter_values[i],variable_list.variables);
        else if (same(variable,"inpol")) inpol = string_to_Stokes(parameter_values[i],variable_list.variables);
		else throw MIST_exception("Unknown variable " + variable);
	}

	// Create the vertices...
	vertex.clear();
	double pstep = 2.*pi/mist->polygonsides;
	for (double p=0;p<=2*pi;p+=pstep) {
		Direction R(alpha,p);

		// First...rotate about y axis 
		double twistz =  center.cost*R.z-center.sint*R.x;
		double twistx =  center.sint*R.z+center.cost*R.x;
		double twisty = R.y;
     
		// Next...rotate about z axis 
		double rotx= center.cosp*twistx-center.sinp*twisty;
		double roty= center.sinp*twistx+center.cosp*twisty;
		double rotz = twistz;
    
		// If the vertex is below the horizon...
		if (rotz<=0) {
			rotx -=rotz;
			roty -=rotz;
			rotz = 0;
		}
		
		// Get the angles...
		double thetas = acos(rotz);
		double phis = atan2(roty,rotx);

		// Add this vertex to the list...
		vertex.push_back(Direction(thetas,phis));
	}

	polcenter = center;
	solidangle = 2.*pi*(1.-cos(alpha));
}

double rotation_angle(double costi,double sinti,double cospi,double sinpi,
                      double costo,double sinto,double cospo,double sinpo)
{
    
    double sinpimpo=cospo*sinpi-cospi*sinpo;
    double cospimpo=cospo*cospi+sinpi*sinpo;
    double temp1=cospi*cospo*costi*costo+costi*costo*sinpi*sinpo+sinti*sinto;
    double temp2=-(cospo*costi*sinpi) + cospi*costi*sinpo;
    
    double cosalpha = 
        (cospi*cospo + sinpi*sinpo)/ 
            sqrt(sqr(-(costi*cospi* (temp2)) + cospi*cospimpo) +     
                 sqr(-(cospi*costi*(temp2)) - sinpi*cospimpo) +   
                 sqr((temp2)*sinti));

    double sinalpha = 
         costo*sinpimpo/
         sqrt(sqr(sinti)*sqr(temp1) +
              sqr(costo*sinpi*sinpimpo+cospi*costi*temp1) +
              sqr(cospi*costo*sinpimpo-sinpi*costi*temp1));
    
    return -atan2(sinalpha,cosalpha);
}

double rotation_angle(const Direction& c,const Direction& v) 
{
	if (fabs(c.x-v.x)<1E-10&&fabs(c.y-v.y)<1E-10) return 0;
	if (fabs(c.x)<1E-10&&fabs(c.y)<1E-10) {
		return atan2(v.y,v.x);
	}
	if (fabs(v.x)<1E-10&&fabs(v.y)<1E-10) {
		return -atan2(c.y,c.x);
	}
	return -atan2(c.z*(c.x*v.x + c.y*v.y) - (sqr(c.x) + sqr(c.y))*v.z,-(c.y*v.x) + c.x*v.y) +
		   atan2(c.z*(sqr(v.x) + sqr(v.y)) - (c.x*v.x + c.y*v.y)*v.z,-(c.y*v.x) + c.x*v.y);
}

void Hemisphere::set_parameters(const Variable_List& variable_list)

{
    inpol = mist->inputlight;

	// Interpret the parameters...for which there are none for a Hemisphere...
	for (int i=0;i<parameter_names.size();++i) {
		string variable = parameter_names[i];
		//double value = Arithmetic_Evaluator(parameter_values[i],variable_list.variables).Value();

      	if (same(variable,"inpol")) inpol = string_to_Stokes(parameter_values[i],variable_list.variables);
        else throw MIST_exception("Unknown variable " + variable);
	}

	solidangle=2.*pi; 

	// Make list of vertices...
	vertex.clear();
	double pstep = 2.*pi/mist->polygonsides;
	for (double t=0;t<=2*pi;t+=pstep) {
		vertex.push_back(Direction(pi/2.,t));
	}
	center = Direction(0,0);

	polcenter = Direction(0,0);
	sens = StokesVector(1,0,0,0);
}

void Annulus::set_parameters(const Variable_List& variable_list)

{
    inpol = mist->inputlight;
	double alpha=0;
	sens = StokesVector(1,0,0,0);
    inpol = mist->inputlight;

	outercenter = Direction(0,0);
	innercenter = Direction(0,0);

	outeralpha=0;
	inneralpha=0;

	int divide = mist->polygonsides;
	// Interpret the parameters...
	for (int i=0;i<parameter_names.size();++i) {
		string variable = parameter_names[i];

      	if (same(variable,"inpol")) inpol = string_to_Stokes(parameter_values[i],variable_list.variables);
      	else if (same(variable,"pol")) sens = string_to_Stokes(parameter_values[i],variable_list.variables);
      	else if (same(variable,"outercenter")) outercenter = string_to_direction(parameter_values[i],variable_list.variables);
      	else if (same(variable,"innercenter")) innercenter = string_to_direction(parameter_values[i],variable_list.variables);
		else if (same(variable,"outeralpha")) outeralpha = Arithmetic_Evaluator(parameter_values[i],variable_list.variables).Value();
		else if (same(variable,"inneralpha")) inneralpha = Arithmetic_Evaluator(parameter_values[i],variable_list.variables).Value();
        else throw MIST_exception("Unknown variable " + variable);
	}

	solidangle=2.*pi*(1.-cos(max(inneralpha,outeralpha)));

	// Make list of vertices...
	vertex.clear();
	
	overtilt=false;

	{
		//Unit vector in the center of the inner circle...
		Vector vspec(innercenter.x,innercenter.y,innercenter.z);
		//Vectors perpendicular to it...
		Vector pspec = perpto(Vector(0,0,1),vspec);
		Vector qspec = perpto(vspec,pspec);

		Matrix pp = outer(pspec,pspec);
		Matrix pq = outer(pspec,qspec);
		Matrix qp = outer(qspec,pspec);
		Matrix qq = outer(qspec,qspec);
		Matrix vv = outer(vspec,vspec);
		Matrix qv = outer(qspec,vspec);
		Matrix vq = outer(vspec,qspec);
		// Vector along the "edge" of the hole...
		Vector sweep = (pp + sin(inneralpha)*(qv-vq) + cos(inneralpha)*(vv+qq))*vspec;
		Matrix pqmqp = pq-qp;
		Matrix pppqq = pp+qq;

		for (int i=0;i<=divide;++i) {
			double t=2*pi/(double)(divide)*i+0.01*deg;
			Vector swept = (cos(t)*pppqq+sin(t)*pqmqp+vv)*sweep;
			if (swept.z<0.) {
				swept.z=0.;
				overtilt = true;
			} 
			double tt = acos(swept.z);
			double pp = atan2(swept.y,swept.x);
			vertex.push_back(Direction(tt,pp));
		}
	}
	{
		//Unit vector in the center of the inner circle...
		Vector vspec(outercenter.x,outercenter.y,outercenter.z);
		//Vectors perpendicular to it...
		Vector pspec = perpto(Vector(0,0,1),vspec);
		Vector qspec = perpto(vspec,pspec);

		Matrix pp = outer(pspec,pspec);
		Matrix pq = outer(pspec,qspec);
		Matrix qp = outer(qspec,pspec);
		Matrix qq = outer(qspec,qspec);
		Matrix vv = outer(vspec,vspec);
		Matrix qv = outer(qspec,vspec);
		Matrix vq = outer(vspec,qspec);
		// Vector along the "edge" of the hole...
		Vector sweep = (pp + sin(outeralpha)*(qv-vq) + cos(outeralpha)*(vv+qq))*vspec;
		Matrix pqmqp = pq-qp;
		Matrix pppqq = pp+qq;

		for (int i=0;i<=divide;++i) {
			double t=2*pi/(double)(divide)*i+0.01*deg;
			Vector swept = (cos(t)*pppqq+sin(t)*pqmqp+vv)*sweep;
			if (swept.z<0.) {
				swept.z=0.;
				overtilt = true;
			} 
			double tt = acos(swept.z);
			double pp = atan2(swept.y,swept.x);
			vertex.push_back(Direction(tt,pp));
		}
	}

	center = outercenter;
	polcenter = outercenter;
}

double Annulus::projected() 
{
	if (!overtilt) return pi*fabs(sqr(sin(outeralpha))*outercenter.z-sqr(sin(inneralpha))*innercenter.z);
	else return SolidShape::projected();
}


bool 
cross(const Direction& v1,const Direction& v2, const Direction& v3, const Direction& v4)
{
	double c = -((v1.y - v2.y)*(v3.x - v4.x)) + (v1.x - v2.x)*(v3.y - v4.y);
	double a = (-(v2.y*v4.x) + v1.y*(-v2.x + v4.x) + v1.x*(v2.y - v4.y) + v2.x*v4.y);
	double b = (v2.y*(v3.x - v4.x) + v3.y*v4.x - v3.x*v4.y + v2.x*(-v3.y + v4.y));

	//if (c==0) return true;
	if (c>0) {
		if (a>0. && a<c && b>0. && b<c) return true;
	} else if (c<0.) {
		if (a<0. && a>c && b<0. && b>c) return true;	
	}
	return false;
}

void Polygon::set_parameters(const Variable_List& variable_list)
{
	vertex.clear();

	sens = StokesVector(1,0,0,0);
	center = Direction(0,0);
	polcenter = Direction(0,0);
    inpol = mist->inputlight;

	for (int i=0;i<parameter_names.size();++i) {
		string variable = parameter_names[i];
		//double value = Arithmetic_Evaluator(parameter_values[i],variable_list.variables).Value();
		
		if (same(variable,"vertex")) {
			Direction R = string_to_direction(parameter_values[i],variable_list.variables);
			if (R.z < 0.) throw MIST_exception("Invalid vertex direction - do not use vertices beyond the horizon");
			vertex.push_back(R);
		} 
		else if (same(variable,"pol")) sens = string_to_Stokes(parameter_values[i],variable_list.variables);
		else if (same(variable,"polcenter")) polcenter = string_to_direction(parameter_values[i],variable_list.variables);
		else if (same(variable,"inpol")) inpol = string_to_Stokes(parameter_values[i],variable_list.variables);
		else throw MIST_exception("Unknown variable " + variable);
	}

	double xmin=1;
	double xmax=-1;
	double ymin=1;
	double ymax=-1;
	for (int j=0;j<vertex.size();++j) {
		if (vertex[j].x<xmin) xmin=vertex[j].x;
		if (vertex[j].x>xmax) xmax=vertex[j].x;
		if (vertex[j].y<ymin) ymin=vertex[j].y;
		if (vertex[j].y>ymax) ymax=vertex[j].y;
		// Check that this segment does not cross another segment...
		int jj = (j!=0) ? j-1 : vertex.size()-1;
		for (int i=j+2;i<vertex.size()-1;++i) {
			int ii = i-1;
			if (cross(vertex[j],vertex[jj],vertex[i],vertex[ii])) 
					throw MIST_exception("Segments cross in polygon");
		}
	}

	Vector t1 = Vector(xmin,ymin,1);
	Vector t2 = Vector(xmax,ymax,1);
	Vector t = t1+t2;
	t = unit(t);

	center = Direction(acos(t.z),atan2(t.y,t.x));

	double dotmin=1;
	for (int k=0;k<vertex.size();++k) {
		Vector v(vertex[k].x,vertex[k].y,vertex[k].z);
		double dot = t*v;
		if (dot<dotmin) dotmin=dot;
	}

	solidangle = 2.*pi*(1.-dotmin);

}


double Differential::integrate(double domega,int minsamples,double rotation)
{
	if (fabs(center.t)>=pi/2.) return 0.;
	if (fabs(mist->thetai)>=pi/2.) return 0.;
	double brdf = sens*(ModelMueller(mist,mist->thetai,center.t,center.p,rotation)*inpol);
	double signal = brdf*cos(center.t);
	mist->store_samples(center.t,center.p,signal);
	return signal;
}

void Differential::set_parameters(const Variable_List& variable_list)
{
	// Initialize defaults...
	center = Direction(0,0);
	sens = StokesVector(1,0,0,0);
    inpol = mist->inputlight;

	// Interpret all of the parameters...
	for (int i=0;i<parameter_names.size();++i) {
		string variable = parameter_names[i];
		//double value = Arithmetic_Evaluator(parameter_values[i],variable_list.variables).Value();
		if (same(variable,"center")) center = string_to_direction(parameter_values[i],variable_list.variables);
		else if (same(variable,"pol")) sens = string_to_Stokes(parameter_values[i],variable_list.variables);
		else if (same(variable,"inpol")) inpol = string_to_Stokes(parameter_values[i],variable_list.variables);
		else throw MIST_exception("Unknown variable " + variable);
	}
}
