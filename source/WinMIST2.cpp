//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: WinMIST2.cpp
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
// WinMIST2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WinMIST2.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "WinMIST2Doc.h"
#include "WinMIST2View.h"
#include "WinTextDoc.h"
#include "WinTextView.h"
#include "PolarDoc.h"
#include "WinPolarView.h"
#include "NullDoc.h"
#include "ModelBrowser.h"
#include "ParameterWizard.h"
#include "ParameterWizardDoc.h"
#include "inherit.h"  

#ifdef GERMER_MODELS
#include "getmodel.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;  
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinMIST2App

BEGIN_MESSAGE_MAP(CWinMIST2App, CWinApp)
	//{{AFX_MSG_MAP(CWinMIST2App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_HELP_MISTWEB, OnHelpMistweb)
	ON_COMMAND(ID_HELP_SCATMECHWEB, OnHelpScatmechweb)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinMIST2App::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinMIST2App construction

CWinMIST2App::CWinMIST2App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CMultiDocTemplate *pTextListingTemplate,*pTextResultsTemplate,*pTextSamplesTemplate;
CMultiDocTemplate *pPolarTemplate,*pGraphTemplate,*pBrowserTemplate,*pWizardTemplate;

/////////////////////////////////////////////////////////////////////////////
// The one and only CWinMIST2App object

CWinMIST2App theApp;

/////////////////////////////////////////////////////////////////////////////
// CWinMIST2App initialization
CMultiDocTemplate* pWinMIST2DocTemplate;

BOOL CWinMIST2App::InitInstance()
{

	SCATMECH::Register((SCATMECH::Model*)0);

#ifdef GERMER_MODELS 
	Register_Germer_Models(); 
	//Register_Special_Models();
#endif

	AfxEnableControlContainer();
	//AfxOleInit();
	
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("NIST-WinMIST"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	pWizardTemplate = new CMultiDocTemplate(
		IDR_WINMISTYPE,
		RUNTIME_CLASS(ParameterWizardDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(ParameterWizard2));
	AddDocTemplate(pWizardTemplate);

	pWinMIST2DocTemplate = new CMultiDocTemplate(
		IDR_RAWMISTYPE,
		RUNTIME_CLASS(RawMISTDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CWinMIST2View));
	//AddDocTemplate(pWinMIST2DocTemplate);

	pTextResultsTemplate = new CMultiDocTemplate(
		IDR_RESULTSVIEW,
		RUNTIME_CLASS(CWinMIST2Doc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTextResultsView));
	//AddDocTemplate(pTextResultsTemplate);

	pTextListingTemplate = new CMultiDocTemplate(
		IDR_LISTINGVIEW,
		RUNTIME_CLASS(CWinMIST2Doc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTextListingView));
	//AddDocTemplate(pTextListingTemplate);

	pTextSamplesTemplate = new CMultiDocTemplate(
		IDR_SAMPLESVIEW,
		RUNTIME_CLASS(CWinMIST2Doc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTextSamplesView));
	//AddDocTemplate(pTextSamplesTemplate);

	pPolarTemplate = new CMultiDocTemplate(
		IDR_POLARVIEW,
		RUNTIME_CLASS(CWinMIST2Doc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CWinPolarView));
	//AddDocTemplate(pPolarTemplate);

	pGraphTemplate = new CMultiDocTemplate(
		IDR_GRAPHVIEW,
		RUNTIME_CLASS(CWinMIST2Doc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CGraphResultsView));
	//AddDocTemplate(pGraphTemplate);

	// create main MDI Frame window
	pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_bShowSplash) {
		OnAppAbout();
	}

	// Dispatch commands specified on the command line
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;
    
	// enable file manager drag/drop and DDE Execute open
    EnableShellOpen(); 
    RegisterShellFileTypes(TRUE);
    // ...

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	if (cmdInfo.m_strFileName.GetLength()!=0) {
		OpenDocumentFile(cmdInfo.m_strFileName);
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_text;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_text = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_EDIT, m_text);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CWinMIST2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.m_text = "MIST was developed by: \r\n"
					  "\r\n"
					  "   Thomas A. Germer, Ph.D. \r\n"
					  "\r\n"
					  "   Sensor Science Division\r\n"
					  "   National Institute of Standards and Technology\r\n"
					  "   100 Bureau Drive Stop 8443\r\n"
					  "   Gaithersburg, Maryland 20899-8443\r\n"
					  "   Telephone: (+1) 301-975-2876 \r\n"
					  "   E-mail: thomas.germer@nist.gov \r\n"
					  "\r\n";

#ifdef GERMER_MODELS
	aboutDlg.m_text+="NIST Internal Version - Not for Distribution Outside of NIST\r\n\r\n"; 
#else 
	aboutDlg.m_text+="This software was developed at the National Institute of Standards and\r\n"
					  "Technology by employees of the Federal Government in the course of \r\n"
					  "their official duties. Pursuant to Title 17 Section 105 of the United\r\n"
					  "States Code this software is not subject to copyright protection and\r\n"
					  "is in the public domain. MIST, and SCATMECH are experimental systems.\r\n"
					  "NIST assumes no responsibility whatsoever for their use by other \r\n"
					  "parties, and makes no guarantees, expressed or implied, about its \r\n"
					  "quality, reliability, or any other characteristic. We would appreciate\r\n"
					  "acknowledgment if the software is used. This software can be \r\n"
					  "redistributed and/or modified freely provided that any derivative\r\n"
					  "works bear some notice that they are derived from it, and any modified\r\n"
					  "versions bear some notice that they have been modified.\r\n"
					  "\r\n"; 
#endif

	aboutDlg.m_text += CString("Build: ") + CString(__DATE__) + CString(" ") + CString(__TIME__);
	aboutDlg.m_text += CString("\r\nUsing: ") + Get_SCATMECH_Version().c_str();

	aboutDlg.DoModal(); 
}

/////////////////////////////////////////////////////////////////////////////
// CWinMIST2App message handlers

/*void CWinMIST2App::OnFileNew() 
{
	// TODO: Add your command handler code here

	CDialog dialog(IDD_NEWDOCUMENT);
	if (IDOK==dialog.DoModal()) {
		ParameterWizardDoc *pDoc = (ParameterWizardDoc *)pWizardTemplate->CreateNewDocument();
		CFrameWnd *frame = pWizardTemplate->CreateNewFrame(pDoc,NULL);
		pWizardTemplate->InitialUpdateFrame(frame,pDoc, TRUE );
	} else {
		CWinMIST2Doc *pDoc = (CWinMIST2Doc *)pWinMIST2DocTemplate->CreateNewDocument();
		CFrameWnd *frame = pWinMIST2DocTemplate->CreateNewFrame(pDoc,NULL);
		pWinMIST2DocTemplate->InitialUpdateFrame(frame,pDoc, TRUE );
	}
}
*/
void CWinMIST2App::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CWinApp::OnFileOpen();
}


CDocument* CWinMIST2App::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	// TODO: Add your specialized code here and/or call the base class

	CFile file(lpszFileName,CFile::modeRead);
	CArchive archive(&file,CArchive::load);

	CString marker;	
	archive >> marker;

	if (marker=="MISTWizardFile1.0") {
		ParameterWizardDoc *pDoc = (ParameterWizardDoc *)pWizardTemplate->CreateNewDocument();
		CFrameWnd *frame = pWizardTemplate->CreateNewFrame(pDoc,NULL);
		pWizardTemplate->InitialUpdateFrame(frame,pDoc, TRUE );
		pDoc->Serialize(archive);	
		pDoc->SetTitle(lpszFileName);
		pDoc->SetPathName(lpszFileName);
		return pDoc;
	}

	archive.Close();
	file.Close();

	CFile rawfile(lpszFileName,CFile::modeRead);
	CArchive rawarchive(&rawfile,CArchive::load);

	CWinMIST2Doc *pDoc = (CWinMIST2Doc *)pWinMIST2DocTemplate->CreateNewDocument();
	CFrameWnd *frame = pWinMIST2DocTemplate->CreateNewFrame(pDoc,NULL);
	pWinMIST2DocTemplate->InitialUpdateFrame(frame,pDoc, TRUE );
	pDoc->Serialize(rawarchive);		
	pDoc->SetTitle(lpszFileName);
	pDoc->SetPathName(lpszFileName);

	rawarchive.Close();
	rawfile.Close();

	return pDoc;
	//return CWinApp::OpenDocumentFile(lpszFileName);
}

void CWinMIST2App::OnHelpMistweb() 
{
	// TODO: Add your command handler code here
	ShellExecute(NULL, "open", "http://www.nist.gov/pml/div685/grp06/scattering_mist.cfm", NULL, NULL, SW_SHOWNORMAL);
}

void CWinMIST2App::OnHelpScatmechweb() 
{
	// TODO: Add your command handler code here
	ShellExecute(NULL, "open", "http://pml.nist.gov/Scatmech/html/frontpage.htm", NULL, NULL, SW_SHOWNORMAL);
}

int CWinMIST2App::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWinApp::ExitInstance();
}
