//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: WinMIST2View.cpp
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
// WinMIST2View.cpp : implementation of the CWinMIST2View class
//

#include "stdafx.h"
#include "WinMIST2.h"
#include "WinMIST2View.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CWinMIST2View

IMPLEMENT_DYNCREATE(CWinMIST2View, CEditView)

BEGIN_MESSAGE_MAP(CWinMIST2View, CEditView)
	//{{AFX_MSG_MAP(CWinMIST2View)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinMIST2View construction/destruction

CWinMIST2View::CWinMIST2View()
{
	// TODO: add construction code here

}

CWinMIST2View::~CWinMIST2View()
{
}

BOOL CWinMIST2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	//cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CWinMIST2View drawing

void CWinMIST2View::OnDraw(CDC* pDC)
{
	CWinMIST2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CWinMIST2View printing

BOOL CWinMIST2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CWinMIST2View::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CWinMIST2View::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CWinMIST2View diagnostics

#ifdef _DEBUG
void CWinMIST2View::AssertValid() const
{
	CEditView::AssertValid();
}

void CWinMIST2View::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CWinMIST2Doc* CWinMIST2View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinMIST2Doc)));
	return (CWinMIST2Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinMIST2View message handlers

#include <iostream>
#include <sstream>
#include <string>
#include "mist0.h"
#include "childfrm.h"
#include "WinTextView.h"
#include "MainFrm.h"

class StatusBarBuf : public std::streambuf {
public:
	StatusBarBuf() {setp(NULL,NULL);}
protected:
	virtual int sync();
	virtual int overflow(int n);
private:
	char buf[256];
	std::string buffer;
};


class PopUpMessageBuf : public std::streambuf {
public:
	PopUpMessageBuf() {	setp(buf,buf+256);}
protected:
	virtual int sync();
	virtual int overflow(int n) { sync(); sputc(n); return 0;}
private:
	char buf[256];
};

int StatusBarBuf::sync()
{
	return 0;
}

int StatusBarBuf::overflow(int c)
{
	if (c=='\n'||c=='\r') {
	    MSG msg;

		while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) theApp.PumpMessage( );
		pMainFrame->SetMessageText(buffer.c_str());
		//statusbar->SetWindowText(buffer.c_str());
		buffer="";
	} else {
		buffer += c;
	}
	return 0;
}

int PopUpMessageBuf::sync()
{
	std::string temp;
	for (char* p=pbase();p<pptr();++p) {
		char c = *p;
		if (c=='\r') temp="";
		else temp += c;
	}
	AfxMessageBox(temp.c_str());
	setp(buf,buf+256);
	return 0;
}


StatusBarBuf statusbarbuf;
std::ostream sout(&statusbarbuf);

PopUpMessageBuf popupmessagebuf;
std::ostream serr(&popupmessagebuf);


void CWinMIST2View::OnDestroy() 
{
	// TODO: Add your message handler code here
	
	CWinMIST2Doc *pDoc = GetDocument();
	POSITION pos = pDoc->GetFirstViewPosition();
	CView* pFirstView = pDoc->GetNextView( pos );

	while (pos != NULL)
		{
			CView* pView = pDoc->GetNextView(pos);
			pView->GetParent()->DestroyWindow();
		}

	CEditView::OnDestroy();
	
}
