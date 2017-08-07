//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: WinMIST2Doc.cpp
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
// WinMIST2Doc.cpp : implementation of the CWinMIST2Doc class
//

#include "stdafx.h"
#include <cstdio>
#include <fstream>
#include <process.h>

#include "WinMIST2.h"
#include "PolarDoc.h"
#include "ChildFrm.h"
#include "WinMIST2Doc.h"
#include "WinTextView.h"
#include "WinPolarView.h"
#include "mist0.h"
#include "mist.h"
#include "progressbar.h"
#include <sstream>
#include "TreeWizard.h"
#ifdef GERMER_MODELS
#include "getmodel.h"
#endif
#include "ModelBrowser.h"
#include "MainFrm.h"
#include "ParameterWizard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinMIST2Doc

IMPLEMENT_DYNCREATE(CWinMIST2Doc, CDocument)

BEGIN_MESSAGE_MAP(CWinMIST2Doc, CDocument)
	//{{AFX_MSG_MAP(CWinMIST2Doc)
	ON_COMMAND(ID_VIEW_RESULTS_TEXT, OnViewResultsText)
	ON_COMMAND(ID_VIEW_SAMPLES_TEXT, OnViewSamplesText)
	ON_COMMAND(ID_VIEW_LISTING, OnViewListing)
	ON_COMMAND(ID_VIEW_RESULTS_GRAPH, OnViewResultsGraph)
	ON_COMMAND(ID_VIEW_SAMPLES_GRAPH, OnViewSamplesGraph)
	ON_COMMAND(ID_SIMULATE, OnSimulate)
	ON_COMMAND(ID_MODELWIZARD, OnModelwizard)
	ON_COMMAND(ID_MIST0, OnMist0)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinMIST2Doc construction/destruction

CWinMIST2Doc::CWinMIST2Doc()
{
	// TODO: add one-time construction code here
	progress = 0;
}

CWinMIST2Doc::~CWinMIST2Doc()
{
}

BOOL CWinMIST2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here

	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWinMIST2Doc serialization

void CWinMIST2Doc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CWinMIST2Doc diagnostics

#ifdef _DEBUG
void CWinMIST2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWinMIST2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinMIST2Doc commands


void CWinMIST2Doc::OnViewResultsText() 
{
	// TODO: Add your command handler code here
   CFrameWnd *frame = GetApp()->pTextResultsTemplate->CreateNewFrame(this,NULL);
   GetApp()->pTextResultsTemplate->InitialUpdateFrame(frame,this, TRUE );
}

void CWinMIST2Doc::OnViewSamplesText() 
{
	// TODO: Add your command handler code here
   CFrameWnd *frame = GetApp()->pTextSamplesTemplate->CreateNewFrame(this,NULL);
   GetApp()->pTextSamplesTemplate->InitialUpdateFrame(frame,this, TRUE );
}

void CWinMIST2Doc::OnViewListing() 
{
	// TODO: Add your command handler code here
   CFrameWnd *frame = GetApp()->pTextListingTemplate->CreateNewFrame(this,NULL);
   GetApp()->pTextListingTemplate->InitialUpdateFrame(frame,this, TRUE );
}

void CWinMIST2Doc::OnViewResultsGraph() 
{
	// TODO: Add your command handler code here
   CFrameWnd *frame = GetApp()->pGraphTemplate->CreateNewFrame(this,NULL);
   GetApp()->pGraphTemplate->InitialUpdateFrame(frame,this, TRUE );

	CGraphResultsView *view = (CGraphResultsView *)(frame->GetActiveView());

    if (!(view->m_wndDialogBar.Create(frame,IDD_AXISFORM,CBRS_TOP,IDD_AXISFORM))) {
		TRACE0("Failed to open dialog bar");
	}
	frame->RecalcLayout();

}

void CWinMIST2Doc::OnViewSamplesGraph() 
{
	// TODO: Add your command handler code here
   CFrameWnd *frame = GetApp()->pPolarTemplate->CreateNewFrame(this,NULL);
   GetApp()->pPolarTemplate->InitialUpdateFrame(frame,this, TRUE );

	CWinPolarView *view = (CWinPolarView *)(frame->GetActiveView());

    if (!(view->m_wndDialogBar.Create(frame,IDD_POLARVIEWFORM,CBRS_TOP,IDD_POLARVIEWFORM))) {
		TRACE0("Failed to open dialog bar");
	}
	frame->RecalcLayout();
}

/*void CWinMIST2Doc::LoadData(const std::string& samplestring)
{
	using namespace std;
	{
		istringstream strm(samplestring);
		char c[256];
		strm.getline(c,255);
		x.clear();
		y.clear();
		z.clear();
		inten.clear();
		while (!strm.eof()) {
			double t,p,_x,_y,_z,b;
			strm >> t >> p >> _x >> _y >> _z >> b;
			if (!strm.eof()) {
				x.push_back(_x);
				y.push_back(_y);
				z.push_back(_z);
				inten.push_back(b);
			}
		}
	}
}*/

#include <afxmt.h>

static std::string dtos(double value)
{
	std::ostringstream out;
	out << value;
	return out.str();
}

unsigned int MIST_Calculation(void *q)
{

	MIST mist((MIST_Calculation_Data*)q);
	return 0;
/*
	try {
		MIST mist((MIST_Calculation_Data*)q);
	} catch (std::exception& e) {
		AfxMessageBox(e.what());
	} catch (...) {
		AfxMessageBox("Unknown exception");
	}
	return 0;
*/
}


void CWinMIST2Doc::Simulate(CString& commands)
{
	using namespace std;

	sout << "Working..." << endl; 

	std::istringstream *instream=new std::istringstream((const char*)(commands));
	ProgressBar* bar =  new ProgressBar;
	progress = new WinMIST_Notifier(bar);

	bar->Create(IDD_RUNNING,NULL);

	data.is = instream;
	data.os = &sout;
	data.notifier = progress;
	data.reset();
	data.save_internal=true;
	data.jobname = (LPCTSTR)(GetTitle());

	if (data.jobname.find('.')!=string::npos) {
		data.jobname=data.jobname.substr(0,data.jobname.find_last_of('.'));
	}

	CWaitCursor wait;

	try {
		MIST_Calculation(&data);
		UpdateAllViews(NULL);	
	} catch (exception& e) {
		AfxMessageBox(e.what());
	} catch (...) {
		AfxMessageBox("Unknown exception");
	}
/*
	try {
		AfxBeginThread(MIST_Calculation,&data);
		UpdateAllViews(NULL);	
	} catch (exception& e) {
		AfxMessageBox(e.what());
	} catch (...) {
		AfxMessageBox("Unknown exception");
	}
*/
	bar->DestroyWindow();	
	delete progress;
	delete instream;
	progress = 0;
}

void CWinMIST2Doc::OnSimulate() 
{
	// TODO: Add your command handler code here
	using namespace std;

	CString cstring;
	CEditView* view;
	view = (CEditView*)m_viewList.GetHead();
	view->GetWindowText(cstring);

	Simulate(cstring);
}


void CWinMIST2Doc::OnModelwizard() 
{
	// TODO: Add your command handler code here
	
	// TODO: Add your command handler code here
   CFrameWnd *frame = GetApp()->pWizardTemplate->CreateNewFrame(this,NULL);
   GetApp()->pWizardTemplate->InitialUpdateFrame(frame,this, TRUE );
}


void CWinMIST2Doc::OnCancel() 
{
	// TODO: Add extra cleanup here
	if (progress) progress->stop = true;

	//CDocument::OnCancel();
}

void CWinMIST2Doc::OnMist0() 
{
	// TODO: Add your command handler code here
	using namespace std;

	CString cstring;
	CEditView* view;
	view = (CEditView*)m_viewList.GetHead();
	view->GetWindowText(cstring);

	char filename[]="MIST0TempInputFile";
	ofstream file(filename);
	file << (LPCSTR)(cstring) << endl;
	file.close();

	_spawnl(_P_NOWAIT,"mist0.exe","mist0.exe",filename,NULL);	
}
