//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: ParameterWizardDoc.cpp
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
// ParameterWizardDoc.cpp : implementation file
//
#include <cstdio>
#include <fstream>
#include <process.h>

#include "stdafx.h"
#include "winmist2.h"
#include "ParameterWizardDoc.h"
#include "ParameterWizard.h"
#include "ViewRawMIST.h"
#include "WinMIST2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// ParameterWizardDoc

IMPLEMENT_DYNCREATE(ParameterWizardDoc, CWinMIST2Doc)

ParameterWizardDoc::ParameterWizardDoc()
{
}

BOOL ParameterWizardDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

ParameterWizardDoc::~ParameterWizardDoc()
{
}


BEGIN_MESSAGE_MAP(ParameterWizardDoc, CWinMIST2Doc)
	//{{AFX_MSG_MAP(ParameterWizardDoc)
	ON_COMMAND(ID_SIMULATE, OnSimulate)
	ON_COMMAND(ID_VIEWRAW, OnViewRawMISTText)
	ON_COMMAND(ID_PURGE, OnPurge)
	ON_COMMAND(ID_MIST0, OnMist0)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ParameterWizardDoc diagnostics

#ifdef _DEBUG
void ParameterWizardDoc::AssertValid() const
{
	CWinMIST2Doc::AssertValid();
}

void ParameterWizardDoc::Dump(CDumpContext& dc) const
{
	CWinMIST2Doc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ParameterWizardDoc serialization

CArchive& operator<<(CArchive& ar,const vdouble& v)
{
	int size = v.size();
	ar.Write(&size, sizeof(int));
	for (vdouble::const_iterator p = v.begin(); p != v.end(); ++p) {
		double x = *p;
		ar.Write(&x, sizeof(double));
	}
	return ar;
}

CArchive& operator>>(CArchive& ar,vdouble& v) 
{
	int size;
	ar.Read(&size, sizeof(int));
	v.resize(size);
	for (int i=0; i<size; ++i) {
		double x;
		ar.Read(&x,sizeof(double));
		v[i] = x;
	}
	return ar;
}

CArchive& operator<<(CArchive& ar,const vvdouble& v)
{
	int size = v.size();
	ar.Write(&size, sizeof(int));
	for (vvdouble::const_iterator p = v.begin(); p != v.end(); ++p) {
		ar << *p;
	}
	return ar;
}

CArchive& operator>>(CArchive& ar,vvdouble& v) 
{
	int size;
	ar.Read(&size, sizeof(int));
	v.resize(size);
	for (int i=0; i<size; ++i) {
		vdouble x;
		ar >> x;
		v[i] = x;
	}
	return ar;	
}

CArchive& operator<<(CArchive& ar,const vstring& v)
{
	int size = v.size();
	ar.Write(&size, sizeof(int));
	for (vstring::const_iterator p = v.begin(); p != v.end(); ++p) {
		ar << CString(p->c_str());
	}
	return ar;
}

CArchive& operator>>(CArchive& ar,vstring& v) 
{
	int size;
	ar.Read(&size, sizeof(int));
	v.resize(size);
	for (int i=0;i<size;++i) {
		CString x;
		ar >> x;
		v[i] = string((const char*)x);
		//v[i] = string((LPCTSTR)x);
		//v.push_back(string((LPCTSTR)x));
	}
	return ar;	
}

void ParameterWizardDoc::Serialize(CArchive& ar)
{
	ParameterWizard2* pw = ((ParameterWizard2*)GetTopView());

	if (ar.IsStoring()) {
		ar << CString("MISTWizardFile1.0");
		pw->treemap[pw->modelitem]->write(ar);
		pw->WriteElement(pw->useritem,ar);
		pw->WriteElement(pw->requireditem,ar);
		pw->WriteElement(pw->paramitem,ar);
		pw->WriteElement(pw->forloopitem,ar);
		pw->WriteElement(pw->integralitem,ar);
		pw->WriteElement(pw->outputsitem,ar);
		pw->WriteElement(pw->filesitem,ar);
		ar << (int)WET_END;

		ar << CString("MoreData");

		ar << CString(data.results.str().c_str());
		ar << CString(data.listing.str().c_str());
		ar << CString(data.samples.str().c_str());

		ar << data.xvalues;
		ar << data.yvalues;
		ar << data.xnames;
		ar << data.ynames;
		ar << data.samples_x;
		ar << data.samples_y;
		ar << data.samples_i;
		ar << data.samples_thetai;

	} else {

		pw->DeleteChildren(pw->useritem);
		pw->DeleteChildren(pw->requireditem);
		pw->DeleteChildren(pw->filesitem);

		int wet;
		ar >> wet;
		while (wet!=(int)WET_END) {
		
			WizardElement* we = GetWizardElement(pw,(WizardElementType)wet);
			we->read(ar);
			ar >> wet;
		}

		try {
			CString head;
			ar >> head;
			if (head=="MoreData") {
				CString temp;
				ar >> temp;
				data.results.str((LPCTSTR)temp);
				ar >> temp;
				data.listing.str((LPCTSTR)temp);
				ar >> temp;
				data.samples.str((LPCTSTR)temp);			
				ar >> data.xvalues;
				ar >> data.yvalues;
				ar >> data.xnames;
				ar >> data.ynames;
				ar >> data.samples_x;
				ar >> data.samples_y;
				ar >> data.samples_i;
				ar >> data.samples_thetai;
			}
		} 
		catch (CException* e) {e->Delete();}

		UpdateAllViews(NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// ParameterWizardDoc commands

void ParameterWizardDoc::OnSimulate() 
{
	// TODO: Add your command handler code here

	using namespace std;

	CString cstring;

	ParameterWizard2* view;
	view = (ParameterWizard2*)m_viewList.GetHead();
	view->MakeDocument(cstring);

	SetModifiedFlag();

	Simulate(cstring);
}

void ParameterWizardDoc::OnViewRawMISTText() 
{
	// TODO: Add your command handler code here
	CString cstring;
	ParameterWizard2* view;
	view = (ParameterWizard2*)m_viewList.GetHead();
	view->MakeDocument(cstring);

	CWinMIST2Doc *pDoc = (CWinMIST2Doc *)(GetApp()->pWinMIST2DocTemplate->CreateNewDocument());
	CFrameWnd *frame = GetApp()->pWinMIST2DocTemplate->CreateNewFrame(pDoc,NULL);
	GetApp()->pWinMIST2DocTemplate->InitialUpdateFrame(frame,pDoc, TRUE );

	CWinMIST2View* view2;
	view2 = (CWinMIST2View*)pDoc->GetTopView();
	view2->SetWindowText(cstring);


	//ViewRawMIST *dialog = new ViewRawMIST();
	//dialog->m_Text = cstring;
	//dialog->DoModal();
}
/////////////////////////////////////////////////////////////////////////////
// RawMISTDoc

IMPLEMENT_DYNCREATE(RawMISTDoc, CWinMIST2Doc)

RawMISTDoc::RawMISTDoc()
{
}

BOOL RawMISTDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

RawMISTDoc::~RawMISTDoc()
{
}


BEGIN_MESSAGE_MAP(RawMISTDoc, CWinMIST2Doc)
	//{{AFX_MSG_MAP(RawMISTDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RawMISTDoc diagnostics

#ifdef _DEBUG
void RawMISTDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void RawMISTDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// RawMISTDoc serialization

void RawMISTDoc::Serialize(CArchive& ar)
{
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

void ParameterWizardDoc::OnPurge() 
{
	// TODO: Add your command handler code here
	data.reset();
	SetModifiedFlag();
	UpdateAllViews(NULL);
}

void ParameterWizardDoc::OnMist0() 
{
	// TODO: Add your command handler code here
	using namespace std;

	CString cstring;

	ParameterWizard2* view;
	view = (ParameterWizard2*)m_viewList.GetHead();
	view->MakeDocument(cstring);

	string filename=(LPCTSTR)(GetTitle());
	if (filename.find('.')!=string::npos) {
		filename=filename.substr(0,filename.find_last_of('.'));
	}
	filename+=".txt";

	ofstream file(filename.c_str());
	file << (LPCSTR)(cstring) << endl;
	file.close();

	//AfxMessageBox(command.c_str());
	_spawnl(_P_NOWAIT,"mist0.exe","mist0.exe",filename.c_str(),NULL);	
}
