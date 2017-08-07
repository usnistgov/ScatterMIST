//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: GraphResultsView.cpp
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
// GraphResultsView.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "GraphResultsView.h"
#include <vector>
#include "mist0.h"
#include "float.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphResultsView

IMPLEMENT_DYNCREATE(CGraphResultsView, CView)

CGraphResultsView::CGraphResultsView()
{
}

CGraphResultsView::~CGraphResultsView()
{
}


BEGIN_MESSAGE_MAP(CGraphResultsView, CView)
	//{{AFX_MSG_MAP(CGraphResultsView)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphResultsView drawing


#include <math.h>
#include <string>
#include <sstream>

static std::string dtos(double value)
{
	std::ostringstream out;
	out << value;
	return out.str();
}

void CGraphResultsView::OnDraw(CDC* pDC)
{
	CWinMIST2Doc* pDoc = (CWinMIST2Doc*)GetDocument();
	MIST_Calculation_Data *data = &(pDoc->data);

	// TODO: add draw code here
	CRect rect;
	if (pDC->IsPrinting()) rect = showrect;
	else GetClientRect( &rect );

	CComboBox* XAxisCombo = (CComboBox*)(m_wndDialogBar.GetDlgItem(IDC_XAxisCombo));
	CComboBox* YAxisCombo = (CComboBox*)(m_wndDialogBar.GetDlgItem(IDC_YAxisCombo));
	CButton* XLogBox = (CButton*)(m_wndDialogBar.GetDlgItem(IDC_XLOG));
	CButton* YLogBox = (CButton*)(m_wndDialogBar.GetDlgItem(IDC_YLOG));

	bool xlog = (XLogBox->GetState()&1)==1;
	bool ylog = (YLogBox->GetState()&1)==1;

	int ix = XAxisCombo->GetCurSel();
	int iy = YAxisCombo->GetCurSel();

	int nx = data->xnames.size();
	int ny = data->ynames.size();
	if (ix>=nx+ny || ix<0) ix=0;
	if (iy>=nx+ny || iy<0) iy=0;

	CPen pen(PS_SOLID|PS_GEOMETRIC,1,RGB(0,0,0));
	pDC->SelectObject(&pen);

	XAxisCombo->ResetContent();
	YAxisCombo->ResetContent();
	for (int i=0;i<nx;++i) {
		XAxisCombo->AddString(data->xnames[i].c_str());
		YAxisCombo->AddString(data->xnames[i].c_str());
	}
	for (int j=0;j<ny;++j)	{
		XAxisCombo->AddString(data->ynames[j].c_str());
		YAxisCombo->AddString(data->ynames[j].c_str());
	}
	
	XAxisCombo->SetCurSel(ix);
	YAxisCombo->SetCurSel(iy);

	if (data->xvalues.size()==0 || data->yvalues.size()==0) {
		pDC->SetTextAlign(TA_CENTER|TA_BASELINE);
		pDC->TextOut(rect.right/2,rect.bottom/2,"No Data.");
		return; 
	}

	vdouble &xvalues = (ix<nx) ? data->xvalues[ix] : data->yvalues[ix-nx];
	vdouble &yvalues = (iy<nx) ? data->xvalues[iy] : data->yvalues[iy-nx];
	std::string &xname =  (ix<nx) ? data->xnames[ix] : data->ynames[ix-nx];
	std::string &yname =  (iy<nx) ? data->xnames[iy] : data->ynames[iy-nx];

	if (xvalues.size()==0 || yvalues.size()==0) {
		pDC->SetTextAlign(TA_CENTER|TA_BASELINE);
		pDC->TextOut(rect.right/2,rect.bottom/2,"Data contains no points.");
		return; 
	}

	//if (xlog) for (int i=0;i<xvalues.size();++i) xvalues[i]=log10(xvalues[i]);
	//if (ylog) for (int i=0;i<yvalues.size();++i) yvalues[i]=log10(yvalues[i]);

	double xmin,ymin,xmax,ymax;
	xmin = ymin = 1E308;
	xmax = ymax = -1E308;
	for (int jx=0;jx<xvalues.size();++jx) {
		if (_finite(xvalues[jx])) {
			if (xvalues[jx]>0 || !xlog) {
				if (xvalues[jx]<xmin) xmin = xvalues[jx];
				if (xvalues[jx]>xmax) xmax = xvalues[jx];
			}
		}
	}
	for (int jy=0;jy<xvalues.size();++jy) {
		if (_finite(yvalues[jy])) {
			if (yvalues[jy]>0 || !ylog) {
				if (yvalues[jy]<ymin) ymin = yvalues[jy];
				if (yvalues[jy]>ymax) ymax = yvalues[jy];
			}
		}
	}

	if (xmin==xmax) {xmin = xmin-1;xmax=xmax+1;}
	if (ymin==ymax) {ymin = ymin-1;ymax=ymax+1;}

	//scaler _x(rect.left,rect.right,xmin,xmax,0.13,0.87);
	//scaler _y(rect.bottom,rect.top,ymin,ymax,0.10,0.9);

	if (xlog) {
		xmin = log10(xmin);
		xmax = log10(xmax);
	}
	if (ylog) {
		ymin = log10(ymin);
		ymax = log10(ymax);
	}

	scaler _x(rect.left,rect.right,xmin-0.13*(xmax-xmin),xmax+0.13*(xmax-xmin));
	scaler _y(rect.bottom,rect.top,ymin-0.10*(ymax-ymin),ymax+0.05*(ymax-ymin));

	pDC->MoveTo(_x(xmin),_y(ymin));
	pDC->LineTo(_x(xmin),_y(ymax));
	pDC->LineTo(_x(xmax),_y(ymax));
	pDC->LineTo(_x(xmax),_y(ymin));
	pDC->LineTo(_x(xmin),_y(ymin));

	pDC->SetTextAlign(TA_CENTER|TA_BASELINE);

	int rsize = (rect.left-rect.right)/300;

	bool started = false;
	pDC->MoveTo(_x(xvalues[0]),_y(yvalues[0]));
	for (int k=0;k<xvalues.size();++k) {
		double x = xvalues[k];
		double y = yvalues[k];
		if ( _finite(x) && _finite(y) ) {
			if ((x>0 || !xlog)&&(y>0 || !ylog)) {
				if (xlog) x = log10(x);
				if (ylog) y = log10(y);
				if (!started) {
					pDC->MoveTo(_x(x),_y(y));
					started=true;
				}

				pDC->Rectangle(_x(x)-rsize,_y(y)-rsize,_x(x)+rsize,_y(y)+rsize);
				pDC->LineTo(_x(x),_y(y));
			} else {
				started = false;
			}
		} else {
			started = false;
		}
	}

	CFont font,swfont;
	font.CreateFont(0,0,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS|CLIP_LH_ANGLES ,PROOF_QUALITY,DEFAULT_PITCH,"Times New Roman");
	swfont.CreateFont(0,0,900,900,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS|CLIP_LH_ANGLES ,PROOF_QUALITY,DEFAULT_PITCH,"Times New Roman");
	
	double hoffset = -(xmax-xmin)*0.02;
	double voffset = -(ymax-ymin)*0.02;

	// Y-axis...
	pDC->SelectObject(&swfont);
	pDC->SetTextAlign(TA_LEFT|TA_BOTTOM);
	pDC->TextOut(_x(xmin+hoffset),_y(ymin),dtos(ymin).c_str());
	pDC->SetTextAlign(TA_RIGHT|TA_BOTTOM);
	pDC->TextOut(_x(xmin+hoffset),_y(ymax),dtos(ymax).c_str());
	pDC->SetTextAlign(TA_CENTER|TA_BOTTOM);
	if (ylog) {
		pDC->TextOut(_x(xmin+2*hoffset),_y((ymin+ymax)/2),(std::string("log10(")+yname+std::string(")")).c_str());
	} else {
		pDC->TextOut(_x(xmin+2*hoffset),_y((ymin+ymax)/2),yname.c_str());
	}

	pDC->SelectObject(&font);
	pDC->SetTextAlign(TA_LEFT|TA_TOP);
	pDC->TextOut(_x(xmin),_y(ymin+voffset),dtos(xmin).c_str());
	pDC->SetTextAlign(TA_RIGHT|TA_TOP);
	pDC->TextOut(_x(xmax),_y(ymin+voffset),dtos(xmax).c_str());
	pDC->SetTextAlign(TA_CENTER|TA_TOP);
	if (xlog) {
		pDC->TextOut(_x((xmin+xmax)/2),_y(ymin+2*voffset)+2,(std::string("log10(")+xname+std::string(")")).c_str());
	} else {
		pDC->TextOut(_x((xmin+xmax)/2),_y(ymin+2*voffset)+2,xname.c_str());
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGraphResultsView diagnostics

#ifdef _DEBUG
void CGraphResultsView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphResultsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphResultsView message handlers

void CGraphResultsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CView::OnUpdate(pSender,lHint,pHint);
}

void CGraphResultsView::Update()
{
	OnUpdate(this,0L,NULL);
}


void CGraphResultsView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CView::OnEndPrinting(pDC, pInfo);
}

void CGraphResultsView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CView::OnBeginPrinting(pDC, pInfo);
}

BOOL CGraphResultsView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(1);
	DoPreparePrinting(pInfo );	
	return CView::OnPreparePrinting(pInfo);
}


void CGraphResultsView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pInfo!=NULL) {
		showrect = pInfo->m_rectDraw;
	} else {
		GetClientRect( &showrect );
	}

	CView::OnPrint(pDC, pInfo);
}
