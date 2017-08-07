//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: ParameterWizard.h
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
#if !defined(AFX_PARAMETERWIZARD_H__8F33B2F7_BB6B_47E4_928F_3979316645A6__INCLUDED_)
#define AFX_PARAMETERWIZARD_H__8F33B2F7_BB6B_47E4_928F_3979316645A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParameterWizard.h : header file
//
#include <map>
#include <list>
#include "inherit.h"
#include <iostream>
#include <afxcview.h>
#include "afxwin.h"

using namespace SCATMECH;
using std::istream;
using std::ostream;

/////////////////////////////////////////////////////////////////////////////
// ParameterWizard dialog
struct WizardElement;
typedef std::map<HTREEITEM,WizardElement*> WizardElementMap;
typedef std::pair<HTREEITEM,WizardElement*> WizardElementPair;

const CString sret( "\r\n" );

/////////////////////////////////////////////////////////////////////////////
// ParameterWizard2 view

enum WizardElementType {
	WET_USERVARIABLE,
	WET_REQUIREDVARIABLE,
	WET_MODELVARIABLE,
	WET_MODELMODELVARIABLE,
	WET_MODELHEADING,
	WET_FORLOOP,
	WET_HEMIINTEGRAL,
	WET_CIRCLEINTEGRAL,
	WET_POLYGONINTEGRAL,
	WET_OUTPUT,
	WET_FILESELEMENT,
	WET_USERSECTIONHEADING,
	WET_REQUIREDSECTIONHEADING,
	WET_PARAMETERSECTIONHEADING,
	WET_FORLOOPSECTIONHEADING,
	WET_INTEGRALSECTIONHEADING,
	WET_OUTPUTSSECTIONHEADING,
	WET_FILESSECTIONHEADING,
	WET_END,
	WET_DIFFERENTIALINTEGRAL,
	WET_LISTLOOP,
	WET_SEHEMIINTEGRAL, // No longer used...
	WET_ANNULUSINTEGRAL
};

template <class X> class counted_ptr
{
public:
    typedef X element_type;

    explicit counted_ptr(X* p = 0) // allocate a new counter
        : itsCounter(0) {if (p) itsCounter = new counter(p);}
    ~counted_ptr()
        {release();}
    counted_ptr(const counted_ptr& r) throw()
        {acquire(r.itsCounter);}
    counted_ptr& operator=(const counted_ptr& r)
    {
        if (this != &r) {
            release();
            acquire(r.itsCounter);
        }
        return *this;
    }

    X& operator*()  const throw()   {return *itsCounter->ptr;}
    X* operator->() const throw()   {return itsCounter->ptr;}
    X* get()        const throw()   {return itsCounter ? itsCounter->ptr : 0;}
    bool unique()   const throw()
        {return (itsCounter ? itsCounter->count == 1 : true);}

private:

    struct counter {
        counter(X* p = 0, unsigned c = 1) : ptr(p), count(c) {}
        X*          ptr;
        unsigned    count;
    }* itsCounter;

    void acquire(counter* c) throw()
    { // increment the count
        itsCounter = c;
        if (c) ++c->count;
    }

    void release()
    { // decrement the count, delete if it is 0
        if (itsCounter) {
            if (--itsCounter->count == 0) {
                delete itsCounter->ptr;
                delete itsCounter;
            }
            itsCounter = 0;
        }
    }
};

class ParameterWizard2 : public CTreeView
{
protected:
	ParameterWizard2();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ParameterWizard2)

// Attributes
public:
	WizardElementMap treemap;	

	CString result;
	const SCATMECH::Inheritance* inheritance;

	HTREEITEM AddItem(HTREEITEM parentitem,const WizardElement& element);
	
	HTREEITEM GetTopParentItem(HTREEITEM item);

	HTREEITEM modelitem;
	HTREEITEM useritem;
	HTREEITEM requireditem;
	HTREEITEM paramitem;
	HTREEITEM integralitem;
	HTREEITEM forloopitem;
	HTREEITEM filesitem;
	HTREEITEM outputsitem;

	std::list<counted_ptr<WizardElement> > Elements;
	void Finalize(HTREEITEM item,CString& result);

	HTREEITEM MoveTreeItem(HTREEITEM item,HTREEITEM moveafter);
	void MakeDocument(CString& result);

	BOOL dragging;
	HTREEITEM dragitem;

	int outputcount;

	CString ModelName;
	void DeleteChildren(HTREEITEM parent);
	void WriteElement(HTREEITEM item,CArchive& ar);
	void WriteElementText(HTREEITEM item,ostream& ar);
	HTREEITEM FindItem(HTREEITEM parent, CString& name);
	HTREEITEM readsection;

	CTreeCtrl* m_Tree;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ParameterWizard2)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~ParameterWizard2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(ParameterWizard2)
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnRemove();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEnter();
	afx_msg void OnCopy();
	afx_msg void OnPaste();
	afx_msg void OnCut();
	afx_msg void OnDestroy();
	afx_msg void OnMoveup();
	afx_msg void OnMovedown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

struct WizardElement {
	WizardElement(ParameterWizard2* _pw) : pw(_pw) {}
	ParameterWizard2 *pw;
	CString name,value,comment;
	HTREEITEM item;
	virtual CString show() = 0;
	virtual CString help() {return "";}
	virtual CString final() {return "\t; " + comment + sret + "\t" + show() + sret + sret;}
	virtual int edit() {return IDCANCEL;};
	virtual void add() {}
	virtual void remove() {
			pw->m_Tree->DeleteItem(item);
			pw->treemap.erase(item);
			pw->UpdateData();
	}

	void update() {	
		pw->m_Tree->SetItemText(item,show()); 
		pw->m_Tree->UpdateData();
	}
	virtual bool can_edit() {return false;}
	virtual bool can_add() {return false;}
	virtual bool can_remove() {return can_add();}
	virtual bool can_copy() {return can_add();}
	bool can_paste();

	void writeA(CArchive& ar, WizardElementType t) {ar << (int)t;}
	virtual void write(CArchive& ar)=0;
	virtual void read(CArchive& ar)=0;

	void w(ostream& os, const CString& str) {
	       os << str.GetLength() << space;
		   for (int i=0;i<str.GetLength();++i) {
			   os.put(str[i]);
		   }
		   os << space;
	}
	void r(istream& is, CString& str) {
		int length;
		is >> length;
		is.get();
		str = CString(' ',length);
		for (int i=0;i<length;++i) {
			str.SetAt(i,is.get());
			if (is.fail()) throw SCATMECH_exception("Error reading stream");
		}
	}
	void writeTextA(ostream& os, WizardElementType t) {os << (int)t << space;}
	virtual void writeText(ostream& ar)=0;
	virtual void readText(istream& ar)=0;

	CString quoter(const CString& str) {
		if (str.IsEmpty()) return "\"\"";
		for (int i=0;i<str.GetLength();++i) {
			if (isspace(str[i])) return "\"" + str + "\"";
		}
		return str;
	}
	CString commenter() {
		if (comment.IsEmpty()) return "";
		return "    ; " + comment;
	}
};

HTREEITEM reg(HTREEITEM parent,WizardElement *p,bool read=false);

WizardElement* GetWizardElement(ParameterWizard2* _pw,WizardElementType code);

struct UserVariable  : public WizardElement {
	UserVariable(ParameterWizard2* _pw,const CString& _n="",const CString& _v="") : WizardElement(_pw) {name=_n;value=_v;}
	CString show() { return name + " = " + quoter(value) + commenter();}
	int edit();
	void add() {
		pw->treemap[pw->useritem]->add();
	}
	virtual bool can_edit() {return true;}
	virtual bool can_add() {return true;}
	virtual void write(CArchive& ar) {writeA(ar,WET_USERVARIABLE); ar << name << value << comment;}
	virtual void read(CArchive& ar) {
		ar >> name >> value >> comment;
		reg(pw->useritem,this,true);
	}
	virtual void writeText(ostream& ar) {writeTextA(ar,WET_USERVARIABLE); w(ar,name); w(ar,value); w(ar,comment);}
	virtual void readText(istream& ar) {r(ar,name); r(ar,value); r(ar,comment);	reg(pw->useritem,this,true);}
};

CString RequiredVariableDescription(const CString& param);

struct RequiredVariable  : public WizardElement {
	RequiredVariable(ParameterWizard2* _pw,const CString& _n="",const CString& _v="") : WizardElement(_pw) {name=_n;value=_v;}	
	CString show() { return name + " = " + quoter(value) + commenter(); }
	int edit();
	void remove() {}
	virtual bool can_edit() {return true;}
	virtual bool can_add() {return false;}
	virtual bool can_copy() {return true;}
	virtual void write(CArchive& ar) {writeA(ar,WET_REQUIREDVARIABLE); ar << name << value << comment;}
	virtual void read(CArchive& ar) {
		ar >> name >> value >> comment;
		reg(pw->requireditem,this,true);
	}
	virtual void writeText(ostream& ar) {writeTextA(ar,WET_REQUIREDVARIABLE); w(ar,name); w(ar,value); w(ar,comment);}
	virtual void readText(istream& ar);

};

CString ParameterDescription(const ModelParameterBase* modelparameter);


struct ModelVariable  : public WizardElement {
	ModelVariable(ParameterWizard2* _pw,const ModelParameterBase* m=NULL,const CString& n="",const CString& v="") 
		: WizardElement(_pw),modelparameter(m) 
	{
		name=n;
		value=v;
		if (modelparameter!=NULL) {
			if (modelparameter->type=="int") stringvar=false;
			else if (modelparameter->type=="double") stringvar=false;
			else if (modelparameter->type=="float") stringvar=false;
			else if (modelparameter->type=="long") stringvar=false;
			else if (modelparameter->type=="bool") stringvar=false;
			else if (modelparameter->type=="dielectric_function") stringvar=false;
			else if (modelparameter->type=="Table") stringvar=false;
			else stringvar=true;
		} else {stringvar=true;}
	}
	const ModelParameterBase* modelparameter;
	BOOL stringvar;
	CString show() { 
		if (stringvar) {
			return '$' + name + " = " + quoter(value) + commenter(); 
		} else {
			return name + " = " + quoter(value) + commenter(); 
		}
	}
	CString final();
	int edit();
	void remove() {}
	virtual bool can_edit() {return true;}
	virtual bool can_add() {return false;}
	virtual bool can_copy() {return true;}
	virtual void write(CArchive& ar) {writeA(ar,WET_MODELVARIABLE); ar << name << value << stringvar << comment;}
	virtual void read(CArchive& ar);
	virtual void writeText(ostream& ar) {writeTextA(ar,WET_MODELVARIABLE); w(ar,name); w(ar,value); ar<<stringvar<<space; w(ar,comment);}
	virtual void readText(istream& ar);
};

struct ModelModelVariable  : public WizardElement {
	ModelModelVariable(ParameterWizard2* _pw,const ModelParameterBase* m=NULL,const CString& n="",const CString& v="") : WizardElement(_pw),modelparameter(m) {name=n;value=v;}
	const ModelParameterBase* modelparameter;
	CString show() { return name + " = " + quoter(value) + commenter(); }
	CString final() {
		CString comment = "\t; " + CString(modelparameter->description.c_str()) + " <" + CString(modelparameter->type.c_str())+ ">";
		comment.Remove('\n');
		comment.Remove('\r');
		return comment + "\r\n\t$" + show() + sret + sret;
	}
	int edit();
	void remove() {}
	virtual bool can_edit() {return true;}
	virtual bool can_add() {return false;}
	virtual bool can_copy() {return true;}
	virtual void write(CArchive& ar) {writeA(ar,WET_MODELMODELVARIABLE); ar << name << value << comment;} 
	virtual void read(CArchive& ar);
	void BuildSubParameters();
	virtual void writeText(ostream& ar); 
	virtual void readText(istream& ar);

};

struct ModelHeading : public WizardElement {
	ModelHeading(ParameterWizard2* _pw,const CString& v="") : WizardElement(_pw), maintain(true) {value = v;}
	CString show() { return CString("MODEL = ") + quoter(value) + commenter(); }
	CString final() {
		const Inheritance *inherit = Model::inheritance.get_named_inheritance((LPCSTR)value,true);
		if (inherit) {
			CString comment = "; " + CString(inherit->get_description().c_str());
			comment.Remove('\n');
			comment.Remove('\r');
			return comment + sret + show() + sret + sret;
		} else {
			return "MODEL = *****\r\n\r\n";
		}
	}
	BOOL maintain;
	int edit();
	void set_model();
	void remove() {};
	virtual bool can_edit() {return true;}
	virtual bool can_add() {return false;}
	virtual bool can_copy() {return true;}
	virtual void write(CArchive& ar) {writeA(ar,WET_MODELHEADING); ar << value << comment;}
	virtual void read(CArchive& ar) {
		ar >> value >> comment;
		set_model();
		update();
	}
	virtual void writeText(ostream& ar) {writeTextA(ar,WET_MODELHEADING); w(ar,value); w(ar,comment); pw->WriteElementText(pw->paramitem,ar);}
	virtual void readText(istream& ar) {r(ar,value); r(ar,comment); set_model(); update();}
};

struct ForLoop  : public WizardElement {
	ForLoop(ParameterWizard2* _pw,const CString& _variable="",const CString& _from="",const CString& _to="",const CString& _by="") : WizardElement(_pw),variable(_variable),from(_from),to(_to),by(_by) {}
	CString variable,from,to,by;
	CString show() {
		return CString("FOR ") + variable + " FROM " + quoter(from) + " TO " + quoter(to) + " BY " + quoter(by) + commenter();
	}
	int edit();
	void add() {
		pw->treemap[pw->forloopitem]->add();	
	}
	virtual bool can_edit() {return true;}
	virtual bool can_add() {return true;}
	virtual void write(CArchive& ar) {writeA(ar,WET_FORLOOP); ar << variable << from << to << by << comment;}
	virtual void read(CArchive& ar) {
		ar >> variable >> from >> to >> by >> comment;
		reg(pw->forloopitem,this,true);
	}
	virtual void writeText(ostream& ar) {writeTextA(ar,WET_FORLOOP); w(ar,variable); w(ar,from); w(ar,to); w(ar,by); w(ar,comment);}
	virtual void readText(istream& ar) {r(ar,variable); r(ar,from); r(ar,to); r(ar,by); r(ar,comment);reg(pw->forloopitem,this,true);}
};

struct ListLoop  : public WizardElement {
	ListLoop(ParameterWizard2* _pw,const CString& _filename="") : WizardElement(_pw),filename(_filename) {}
	CString filename;
	CString show() {
		return CString("LIST ") + quoter(filename) + commenter();
	}
	int edit();
	void add() {
		pw->treemap[pw->forloopitem]->add();	
	}
	virtual bool can_edit() {return true;}
	virtual bool can_add() {return true;}
	virtual void write(CArchive& ar) {writeA(ar,WET_LISTLOOP); ar << filename << comment;}
	virtual void read(CArchive& ar) {
		ar >> filename >> comment;
		reg(pw->forloopitem,this,true);
	}
	virtual void writeText(ostream& ar) {writeTextA(ar,WET_LISTLOOP); w(ar,filename); w(ar,comment);}
	virtual void readText(istream& ar) {r(ar,filename); r(ar,comment);reg(pw->forloopitem,this,true);}
};

struct Integral  : public WizardElement {
	Integral(ParameterWizard2* _pw,int _type) : WizardElement(_pw),type(_type) {}
	enum { CIRCLE,DIFFERENTIAL,HEMI,POLYGON,ANNULUS};
	int type;
	CString show() {
		switch (type) {
		case CIRCLE: return name + " = CIRCLE (double click to view)" + commenter();
		case DIFFERENTIAL: return name + " = DIFFERENTIAL (double click to view)" + commenter();
		case HEMI: return name + " = HEMI (double click to view)" + commenter();
		case POLYGON: return name + " = POLYGON (double click to view)" + commenter();
		case ANNULUS: return name + " = ANNULUS (double click to view)" + commenter();
		default: return name + " = ?";
		}
	}
	void add() {
		pw->treemap[pw->integralitem]->add();
	}
	virtual bool can_edit() {return true;}
	virtual bool can_add() {return true;}
};

struct HemiIntegral : public Integral {
	HemiIntegral(ParameterWizard2* _pw) : Integral(_pw,HEMI) {
		name = "integ";
		inpol = "incidentpol";
	}
	int edit();
	CString inpol;
	virtual CString final() {
		CString result="\t" + name + " = HEMI" + sret;
		result += "\t\tINPOL = " + inpol + sret;
		result += "\t\tEND" + sret;
		return result;
	}
	virtual bool can_edit() {return true;}
	virtual bool can_add() {return true;}
	virtual void write(CArchive& ar) {
		writeA(ar,WET_HEMIINTEGRAL); 
		ar << CString("DI$%wIP");	
		ar << name << inpol << comment;
	}
	virtual void read(CArchive& ar) {
		CString key;
		ar >> key;
		if (key=="DI$%wIP") {
			ar >> name >> inpol >> comment;  
		} else {
			name = key;
			ar >> comment;
			inpol = "incidentpol";
		}
		reg(pw->integralitem,this,true);
	}
	virtual void writeText(ostream& ar) {writeTextA(ar,WET_HEMIINTEGRAL); w(ar,name); w(ar,inpol); w(ar,comment);}
	virtual void readText(istream& ar) {r(ar,name); r(ar,inpol); r(ar,comment);reg(pw->integralitem,this,true);}

};

struct AnnulusIntegral : public Integral {
	AnnulusIntegral(ParameterWizard2* _pw) : Integral(_pw,ANNULUS) {
		name = "integ";
		inpol = "incidentpol";
		pol = "(1,0,0,0)";
		innerpolar = "0*deg";
		innerazimuthal = "0*deg";
		inneralpha = "25*deg";
		outerpolar = "0*deg";
		outerazimuthal = "0*deg";
		outeralpha = "60*deg";
	}
	int edit();
	CString inpol;
	CString pol;
	CString innerpolar;
	CString innerazimuthal;
	CString inneralpha;
	CString outerpolar;
	CString outerazimuthal;
	CString outeralpha;
	virtual CString final() {
		CString result="\t" + name + " = ANNULUS" + sret;
		result += "\t\tINPOL = " + inpol + sret;
		result += "\t\tPOL = " + pol + sret;
		result += "\t\tINNERCENTER = (" + innerpolar + "," + innerazimuthal + ")" + sret;
		result += "\t\tINNERALPHA = " + inneralpha + sret;
		result += "\t\tOUTERCENTER = (" + outerpolar + "," + outerazimuthal + ")" + sret;
		result += "\t\tOUTERALPHA = " + outeralpha + sret;
		result += "\t\tEND" + sret;
		return result;
	}
	virtual bool can_edit() {return true;}
	virtual bool can_add() {return true;}
	virtual void write(CArchive& ar) {
		writeA(ar,WET_ANNULUSINTEGRAL); 	
		ar << name << inpol << pol << comment << innerpolar << innerazimuthal << outerpolar << outerazimuthal << inneralpha << outeralpha;
	}
	virtual void read(CArchive& ar) {
		ar >> name >> inpol >> pol >> comment >> innerpolar >> innerazimuthal >> outerpolar >> outerazimuthal >> inneralpha >> outeralpha;
		reg(pw->integralitem,this,true);
	}
	virtual void writeText(ostream& ar) {writeTextA(ar,WET_ANNULUSINTEGRAL); w(ar,name); w(ar,inpol); w(ar,pol); w(ar,comment); w(ar,innerpolar); w(ar,innerazimuthal); w(ar,outerpolar); w(ar,outerazimuthal); w(ar,inneralpha); w(ar,outeralpha);}
	virtual void readText(istream& ar) {r(ar,name); r(ar,inpol); r(ar,pol); r(ar,comment); r(ar,innerpolar); r(ar,innerazimuthal); r(ar,outerpolar); r(ar,outerazimuthal); r(ar,inneralpha); r(ar,outeralpha);reg(pw->integralitem,this,true);}
};

struct CircleIntegral : public Integral  {
	CircleIntegral(ParameterWizard2* _pw) : Integral(_pw,CIRCLE) {
		name = "integ";
		inpol = "incidentpol";
		pol = "(1,0,0,0)";
		theta = "0*deg";
		phi = "0*deg";
		alpha = "45*deg";
	}
	CString theta,phi,alpha,inpol,pol;
	virtual CString final() {
		CString result= "\t" + name + " = CIRCLE" + sret;
		result += "\t\tALPHA = " + alpha + sret;
		result += "\t\tCENTER = (" + theta + ',' + phi + ")" + sret;
		result += "\t\tINPOL = " + inpol + sret;
		result += "\t\tPOL = " + pol + sret;
		result += "\t\tEND" + sret;
		return result;
	}
	int edit();
	virtual bool can_edit() {return true;}
	virtual bool can_add() {return true;}
	virtual void write(CArchive& ar) {
		writeA(ar,WET_CIRCLEINTEGRAL); 
		ar << CString("DI$%wIP");
		ar << name << theta << phi<< alpha << inpol << pol << comment;
	}
	virtual void read(CArchive& ar) {
		CString key;
		ar >> key;
		if (key=="DI$%wIP") {
			ar >> name >> theta >> phi >> alpha >> inpol >> pol >> comment;
		} else {
			name = key;
			ar >> theta >> phi >> alpha >> pol >> comment;
			inpol = "incidentpol";
		}
		reg(pw->integralitem,this,true);
	}
	virtual void writeText(ostream& ar) {writeTextA(ar,WET_CIRCLEINTEGRAL); w(ar,name); w(ar,theta); w(ar,phi); w(ar,alpha); w(ar,inpol); w(ar,pol); w(ar,comment);}
	virtual void readText(istream& ar) {r(ar,name); r(ar,theta); r(ar,phi); r(ar,alpha); r(ar,inpol); r(ar,pol); r(ar,comment);reg(pw->integralitem,this,true);}
};

struct DifferentialIntegral : public Integral  {
	DifferentialIntegral(ParameterWizard2* _pw) : Integral(_pw,DIFFERENTIAL) {
		name = "integ";
		inpol = "incidentpol";
		pol = "(1,0,0,0)";
		theta = "0*deg";
		phi = "0*deg";
	}
	CString theta,phi,inpol,pol;
	virtual CString final() {
		CString result= "\t" + name + " = DIFFERENTIAL" + sret;
		result += "\t\tCENTER = (" + theta + ',' + phi + ")" + sret;
        result += "\t\tINPOL = " + inpol + sret;
		result += "\t\tPOL = " + pol + sret;
		result += "\t\tEND" + sret;
		return result;
	}
	int edit();
	virtual bool can_edit() {return true;}
	virtual bool can_add() {return true;}
	virtual void write(CArchive& ar) {
        writeA(ar,WET_DIFFERENTIALINTEGRAL); 
        ar << CString("DI$%wIP");
        ar << name << theta << phi << inpol << pol << comment;
    }
	virtual void read(CArchive& ar) {
        CString key;
        ar >> key;
        if (key=="DI$%wIP") {
    		ar >> name >> theta >> phi >> inpol >> pol >> comment;
        } else {
            name = key;
    		ar >> theta >> phi >> pol >> comment;
            inpol = "incidentpol";
        }
        reg(pw->integralitem,this,true);
	}
	virtual void writeText(ostream& ar) {writeTextA(ar,WET_DIFFERENTIALINTEGRAL); w(ar,name); w(ar,theta); w(ar,phi); w(ar,inpol); w(ar,pol); w(ar,comment);}
	virtual void readText(istream& ar) {r(ar,name); r(ar,theta); r(ar,phi); r(ar,inpol); r(ar,pol); r(ar,comment);reg(pw->integralitem,this,true);}
};

struct PolygonIntegral : public Integral  {
	PolygonIntegral(ParameterWizard2* _pw) : Integral(_pw,POLYGON) {
		name = "integ";
		thetap = "0*deg";
		phip = "0*deg";
		inpol = "incidentpol";
		pol = "(1,0,0,0)";
	}
	CString thetap,phip,inpol,pol;
	typedef std::list<std::pair<CString,CString> > vlist;
	vlist vertices;
	virtual CString final() {
		CString result="\t" + name + " = POLYGON" + sret;
		result += "\t\tPOLCENTER = ""(" + thetap + ',' + phip + ")""" + sret;
		result += "\t\tPOL = " + pol + sret;
		for (vlist::iterator p=vertices.begin();p!=vertices.end();++p) {
			result += "\t\tVERTEX = ""(" + p->first + ',' + p->second + ")""" + sret;
		}
		result += "\t\tINPOL = " + inpol;
		result += "\t\tEND" + sret;
		return result;
	}
	int edit();
	virtual bool can_edit() {return true;}
	virtual bool can_add() {return true;}
	virtual void write(CArchive& ar) {
		writeA(ar,WET_POLYGONINTEGRAL); 
        ar << CString("DI$%wIP");
		ar << name << thetap << phip << pol << comment;
		int size= vertices.size();
		ar.Write(&size,sizeof(int));
		for (vlist::iterator p = vertices.begin();p!=vertices.end();++p) {
			ar << p->first << p->second;
		}
		ar << inpol;
	}
	virtual void read(CArchive& ar) {
        CString key;
        ar >> key;
        if (key=="DI$%wIP") {
			ar >> name;
		} else {
			name = key;
		}
		int n;
		ar >> thetap >> phip >> pol >> comment;
		ar.Read(&n,sizeof(int));
		vertices.clear();
		for (int i=0;i<n;++i) {
			CString theta,phi;
			ar >> theta >> phi;
			vertices.push_front(std::pair<CString,CString>(theta,phi));
		}
        if (key=="DI$%wIP") {
			ar >> inpol;
		} else {
			inpol = "incidentpol";
		}
		reg(pw->integralitem,this);
	}
	virtual void writeText(ostream& ar) {
		writeTextA(ar,WET_POLYGONINTEGRAL); 
		w(ar,name); w(ar,thetap); w(ar,phip); w(ar,pol); w(ar,comment);
		int size= vertices.size();
		ar << size << space;
		for (vlist::iterator p = vertices.begin();p!=vertices.end();++p) {
			w(ar,p->first);
			w(ar,p->second);
		}
		w(ar,inpol);
	}
	virtual void readText(istream& ar) {
		int n;
		r(ar,thetap);r(ar,phip);r(ar,pol);r(ar,comment);
		ar >> n;
		vertices.clear();
		for (int i=0;i<n;++i) {
			CString theta,phi;
			r(ar,theta);
			r(ar,phi);
			vertices.push_front(std::pair<CString,CString>(theta,phi));
		}
		r(ar,inpol);
		reg(pw->integralitem,this);
	}

};

struct Output  : public WizardElement {
	Output(ParameterWizard2* _pw,const CString& _value="") : WizardElement(_pw) {value=_value;name="";}
	CString show() {
		if (name.GetLength()!=0) return quoter(name) + " = " + quoter(value) + commenter();
		else return quoter(value) + commenter();
	}
	int edit();
	void add() {
		pw->treemap[pw->outputsitem]->add();
	}
	CString final() {
		char s[12];
		sprintf(s,"\t%4d ",pw->outputcount++);
		if (name.GetLength()!=0) return s + name + "=" + quoter(value) + sret;
		else return s + quoter(value) + sret;
	}
	virtual bool can_edit() {return true;}
	virtual bool can_add() {return true;}
	virtual void write(CArchive& ar) {
		writeA(ar,WET_OUTPUT); 
		CString _value = name + "=" + value;
		ar << _value << comment;
	}
	virtual void read(CArchive& ar) {
		CString _value;
		ar >> _value >> comment;
		int loc = _value.Find('='); 
		if (loc!=-1) {
			name = _value.Left(loc);
			int len=_value.GetLength();
			value = _value.Right(len-loc-1);
		} else {
			name = "";
			value = _value;
		}
		reg(pw->outputsitem,this,true);
	}
	virtual void writeText(ostream& ar) {
		writeTextA(ar,WET_OUTPUT); 
		w(ar,name);
		w(ar,value);
		w(ar,comment);
	}
	virtual void readText(istream& ar) {
		r(ar,name);
		r(ar,value);
		r(ar,comment);
		reg(pw->outputsitem,this,true);
	}
};

struct FilesElement  : public WizardElement {
	FilesElement(ParameterWizard2* _pw,const CString& _n="",const CString& _v="") : WizardElement(_pw) {name=_n;value=_v;}
	CString show() { return name + " = " + quoter(value) + commenter(); }
	int edit();
	void add() { pw->treemap[pw->filesitem]->add(); }
	virtual bool can_edit() {return true;}
	virtual bool can_add() {return true;}
	virtual void write(CArchive& ar) {writeA(ar,WET_FILESELEMENT); ar << name << value << comment;}
	virtual void read(CArchive& ar) {
		ar >> name >> value >> comment;
		reg(pw->filesitem,this,true);
	}
	virtual void writeText(ostream& ar) {writeTextA(ar,WET_FILESELEMENT); w(ar,name); w(ar,value); w(ar,comment);}
	virtual void readText(istream& ar) {
		r(ar,name); r(ar,value); r(ar,comment);
		reg(pw->filesitem,this,true);
	}
};

struct SectionHeading : public WizardElement {
	SectionHeading(ParameterWizard2* _pw, const CString& v) : WizardElement(_pw) {value=v;}
	CString show() { return value; }
	void remove() {}
	virtual bool can_edit() {return false;}
	virtual bool can_add() {return true;}
	virtual bool can_remove() {return false;}
	virtual void write(CArchive& ar) {}
	virtual void read(CArchive& ar){}
	virtual void writeText(ostream& ar) {}
	virtual void readText(istream& ar) {}
};

struct UserSectionHeading : public SectionHeading {
	UserSectionHeading(ParameterWizard2* _pw) : SectionHeading(_pw,"USER VARIABLES") {}
	void add();
	virtual bool can_edit() {return false;}
	virtual bool can_add() {return true;}
	virtual bool can_remove() {return false;}
	virtual bool can_copy() {return true;}
	virtual void write(CArchive& ar) {}
	virtual void read(CArchive& ar){}
	virtual void writeText(ostream& ar) {pw->WriteElementText(pw->useritem,ar);}
	virtual void readText(istream& ar) {}
};

struct RequiredSectionHeading : public SectionHeading {
	RequiredSectionHeading(ParameterWizard2* _pw) : SectionHeading(_pw,"REQUIRED VARIABLES") {}
	void add() {}
	virtual bool can_edit() {return false;}
	virtual bool can_add() {return false;}
	virtual bool can_remove() {return false;}
	virtual bool can_copy() {return true;}
	virtual void write(CArchive& ar) {}
	virtual void read(CArchive& ar){}
	virtual void writeText(ostream& ar) {pw->WriteElementText(pw->requireditem,ar);}
	virtual void readText(istream& ar) {}
};

struct ParameterSectionHeading : public SectionHeading {
	ParameterSectionHeading(ParameterWizard2* _pw) : SectionHeading(_pw,"MODEL PARAMETERS") {}
	void add() {}
	virtual bool can_edit() {return false;}
	virtual bool can_add() {return false;}
	virtual bool can_remove() {return false;}
	virtual bool can_copy() {return true;}
	virtual void write(CArchive& ar) {}
	virtual void read(CArchive& ar){}
	virtual void writeText(ostream& ar) {pw->WriteElementText(pw->paramitem,ar);}
	virtual void readText(istream& ar) {}
};

struct ForLoopSectionHeading : public SectionHeading {
	ForLoopSectionHeading(ParameterWizard2* _pw) : SectionHeading(_pw,"VARIED PARAMETERS") {}
	void add();
	virtual bool can_edit() {return false;}
	virtual bool can_add() {return true;}
	virtual bool can_remove() {return false;}
	virtual bool can_copy() {return true;}
	virtual void write(CArchive& ar) {}
	virtual void read(CArchive& ar){}
	virtual void writeText(ostream& ar) {pw->WriteElementText(pw->forloopitem,ar);}
	virtual void readText(istream& ar) {}
};

struct IntegralSectionHeading : public SectionHeading {
	IntegralSectionHeading(ParameterWizard2* _pw) : SectionHeading(_pw,"INTEGRALS") {}
	void add();
	virtual bool can_edit() {return false;}
	virtual bool can_add() {return true;}
	virtual bool can_remove() {return false;}
	virtual bool can_copy() {return true;}
	virtual void write(CArchive& ar) {}
	virtual void read(CArchive& ar){}
	virtual void writeText(ostream& ar) {pw->WriteElementText(pw->integralitem,ar);}
	virtual void readText(istream& ar) {}
};

struct OutputsSectionHeading : public SectionHeading {
	OutputsSectionHeading(ParameterWizard2* _pw) : SectionHeading(_pw,"OUTPUTS") {}
	void add();
	virtual bool can_edit() {return false;}
	virtual bool can_add() {return true;}
	virtual bool can_remove() {return false;}
	virtual bool can_copy() {return true;}
	virtual void write(CArchive& ar) {}
	virtual void read(CArchive& ar){}
	virtual void writeText(ostream& ar) {pw->WriteElementText(pw->outputsitem,ar);}
	virtual void readText(istream& ar) {}
};

struct FilesSectionHeading : public SectionHeading {
	FilesSectionHeading(ParameterWizard2* _pw) : SectionHeading(_pw,"FILES") {}
	void add();
	virtual bool can_edit() {return false;}
	virtual bool can_add() {return true;}
	virtual bool can_remove() {return false;}
	virtual bool can_copy() {return true;}
	virtual void write(CArchive& ar) {}
	virtual void read(CArchive& ar){}
	virtual void writeText(ostream& ar) {pw->WriteElementText(pw->filesitem,ar);}
	virtual void readText(istream& ar) {}
};



/////////////////////////////////////////////////////////////////////////////
// NewUserVariableDialog dialog

class NewUserVariableDialog : public CDialog
{
// Construction
public:
	NewUserVariableDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NewUserVariableDialog)
	enum { IDD = IDD_NEWUSERVARIABLE };
	CString	m_name;
	CString	m_value;
	CString	m_description;
	CString	m_Comment;
	//}}AFX_DATA

	bool m_readonly;
	CString m_title;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NewUserVariableDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NewUserVariableDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// ModelVariableDialog dialog

class ModelVariableDialog : public CDialog
{
// Construction
public:
	ModelVariableDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ModelVariableDialog)
	enum { IDD = IDD_MODELVARIABLE };
	CString	m_name;
	CString	m_value;
	CString	m_description;
	BOOL	m_string;
	CString	m_Comment;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ModelVariableDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ModelVariableDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_PARAMETERWIZARD_H__8F33B2F7_BB6B_47E4_928F_3979316645A6__INCLUDED_)
/////////////////////////////////////////////////////////////////////////////
// NewForLoopDialog dialog

class NewForLoopDialog : public CDialog
{
// Construction
public:
	NewForLoopDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NewForLoopDialog)
	enum { IDD = IDD_NEWFORLOOP };
	CString	m_by;
	CString	m_from;
	CString	m_to;
	CString	m_variable;
	CString	m_Comment;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NewForLoopDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NewForLoopDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// ChooseModelDialog dialog

class ChooseModelDialog : public CDialog
{
// Construction
public:
	ChooseModelDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ChooseModelDialog)
	enum { IDD = IDD_CHOOSEMODEL };
	CComboBox	m_combo;
	CString	m_description;
	CString	m_Comment;
	BOOL m_maintain;
	//}}AFX_DATA

	CString parentmodel;
	CString returnedmodel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ChooseModelDialog)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ChooseModelDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ParameterWizard2 window


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// NewListLoopDialog dialog

class NewListLoopDialog : public CDialog
{
// Construction
public:
	NewListLoopDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NewListLoopDialog)
	enum { IDD = IDD_LISTLOOPDIALOG };
	CString	m_filename;
	CString	m_Comment;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NewListLoopDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NewListLoopDialog)
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// NewLoopDialog dialog

class NewLoopDialog : public CDialog
{
// Construction
public:
	NewLoopDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NewLoopDialog)
	enum { IDD = IDD_NEWLOOPDIALOG };
	//}}AFX_DATA

	char type;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NewLoopDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NewLoopDialog)
	afx_msg void OnFor();
	afx_msg void OnList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
