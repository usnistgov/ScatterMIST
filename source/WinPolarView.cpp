//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: WinPolarView.cpp
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
// CWinPolarView.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "WinPolarView.h"
#include "WinMIST2Doc.h"
#include <algorithm>
#include "float.h"
#include <math.h>
#include "vector3d.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinPolarView

IMPLEMENT_DYNCREATE(CWinPolarView, CView)

CWinPolarView::CWinPolarView()
{
} 

CWinPolarView::~CWinPolarView()
{
}


BEGIN_MESSAGE_MAP(CWinPolarView, CView)
	//{{AFX_MSG_MAP(CWinPolarView)
	ON_EN_KILLFOCUS(IDC_POLARMIN, OnKillfocusPolarmin)
	ON_EN_KILLFOCUS(IDC_POLARMAX, OnKillfocusPolarmax)
	ON_BN_CLICKED(IDC_OPTIONS, OnOptions)
	ON_BN_CLICKED(IDC_AUTOSCALE, OnAutoscale)
	ON_EN_CHANGE(IDC_ORDER, OnChangeOrder)
	ON_EN_KILLFOCUS(IDC_ORDER, OnKillfocusOrder)
	ON_EN_UPDATE(IDC_ORDER, OnUpdateOrder)
	ON_EN_UPDATE(IDC_POLARMIN, OnKillFocusPolarmin)
	ON_EN_CHANGE(IDC_POLARMIN, OnKillFocusPolarmin)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinPolarView drawing

class rotator {
public:
	rotator(double _theta,double _phi,const scaler& _sx,const scaler& _sy) : sx(_sx),sy(_sy)  {
		theta=_theta;
		phi=_phi;
		costheta = cos(theta);
		cosphi = cos(phi);
		sintheta = sin(theta);
		sinphi = sin(phi);
		cosphicostheta = cosphi*costheta;
		sinphicostheta = sinphi*costheta;
		cosphisintheta = cosphi*sintheta;
		sinphisintheta = sinphi*sintheta;
		hide=true;
	}
	POINT operator()(const SCATMECH::Vector& v) {
		SCATMECH::Vector vr(v.x*cosphi + v.y*sinphi, 
			        v.y*cosphicostheta-v.x*sinphicostheta+v.z*sintheta,
					v.z*costheta-v.y*cosphisintheta+v.x*sinphisintheta);
		POINT result;

		if (vr.z>=0 || !hide) {
			result.x = sx(vr.x);
			result.y = sy(vr.y);
		} else {
			result.x=-1;
			result.y=-1;
		}
		return result;
	}
	double theta,phi;
	double costheta,sintheta,cosphi,sinphi;
	double cosphicostheta,sinphicostheta,cosphisintheta,sinphisintheta;
	bool hide;
	const scaler &sx,&sy;
};

double inten_to_BRDF(const MIST_Calculation_Data* data,int order, int m,BOOL convert)
{
	if (convert) {
		double x = data->samples_x[order][m];
		double y = data->samples_y[order][m];
		double z = sqrt(1.-x*x-y*y);
		return data->samples_i[order][m]/z;
	} else {
		return data->samples_i[order][m];
	}
}

void CWinPolarView::OnDraw(CDC* pDC)
{
	const double deg = 4*atan(1.)/180.;

	CWinMIST2Doc* pDoc = GetDocument();
	MIST_Calculation_Data *data = &(pDoc->data);

	CSliderCtrl* slider1 = (CSliderCtrl*)(m_wndDialogBar.GetDlgItem(IDC_SLIDER1));
	CSliderCtrl* slider2 = (CSliderCtrl*)(m_wndDialogBar.GetDlgItem(IDC_SLIDER2));

	BOOL showintensity = options.m_ShowIntensity;
	BOOL invertscale = options.m_InvertScale;
	BOOL alwaysautoscale = options.m_AlwaysAutoScale;
	BOOL fixzero = options.m_FixZero;
	BOOL showBRDF = options.m_ShowBRDF;

	CEdit* Min = (CEdit*)(m_wndDialogBar.GetDlgItem(IDC_POLARMIN));
	CEdit* Max = (CEdit*)(m_wndDialogBar.GetDlgItem(IDC_POLARMAX));
	CEdit* Order = (CEdit*)(m_wndDialogBar.GetDlgItem(IDC_ORDER));
	CSpinButtonCtrl* Spinner = (CSpinButtonCtrl*)(m_wndDialogBar.GetDlgItem(IDC_SPIN2));

	int maxorder = data->samples_i.size()-2;
	Spinner->SetRange(0,maxorder);

	int order;
	{ 
		CString temp;
		Order->GetWindowText(temp);
		sscanf((LPCTSTR)temp,"%d",&order);
	}
	if (order>=data->samples_x.size()-1) order = data->samples_x.size()-2;
	if (order<0) return;
	if (data->samples_x.size()==0) return;

	slider1->SetRange(0,90);
	slider1->SetTicFreq(10);
	slider1->SetPageSize(10);
	slider2->SetRange(0,360);
	slider2->SetTicFreq(10);
	slider2->SetPageSize(10);
	
	double theta = slider1->GetPos()*deg;
	double phi = slider2->GetPos()*deg;

	// TODO: add draw code here
	COLORREF white = RGB(255,255,255);
	COLORREF black = RGB(0,0,0);
	COLORREF brown = RGB(128,64,64);
	COLORREF blue = RGB(0,0,127);
	COLORREF red = RGB(255,0,0);
	COLORREF green = RGB(0,127,0);

	CPen solidpen;
	solidpen.CreatePen(PS_SOLID,1,brown);
	CPen dotpen;
	dotpen.CreatePen(PS_DOT,1,brown);
	CPen boldpen;
	boldpen.CreatePen(PS_SOLID,2,black);
	CPen boldbluepen;
	boldbluepen.CreatePen(PS_SOLID,2,blue);
	
	CBrush brush;
	brush.CreateSolidBrush(red);

	CPen *pOldPen = pDC->SelectObject(&solidpen);
	CBrush *pOldBrush = pDC->SelectObject(&brush);

	CPen boldgreenpen;
	boldgreenpen.CreatePen(PS_SOLID,2,green);
	CBrush whitebrush;
	whitebrush.CreateSolidBrush(white);

	int L;
	CRect rect;
	if (pDC->IsPrinting()) rect = showrect;
	else GetClientRect( &rect );
	L = (rect.right-rect.left > rect.bottom-rect.top) ?  rect.bottom-rect.top : rect.right-rect.left;

	double xmin=-1,xmax=1,ymin=-1,ymax=1;
	scaler _x(rect.left,rect.left+L,-1.05,1.05);
	scaler _y(rect.top+L,rect.top,-1.05,1.05);
	rotator r(theta,phi,_x,_y);		


	// Draw sampled points...
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,0,128));
	pDC->SetTextAlign( TA_BASELINE | TA_CENTER );
	pDC->SelectObject(&boldbluepen);

	double bmin= data->samples_i[order].size()>0 ? inten_to_BRDF(data,order,0,showBRDF) : 0;
	double bmax= bmin;
	if (!_finite(bmin)) bmin=0;
	if (!_finite(bmax)) bmax=0;
	for (int mm=0;mm<data->samples_x[order].size();++mm) {
		double inten = inten_to_BRDF(data,order,mm,showBRDF);
		if (_finite(inten)) {
			if (inten<bmin) bmin=inten;
			if (inten>bmax) bmax=inten;
		}
	}

	if (alwaysautoscale) {
		char temp[32];
		if (fixzero) bmin=0;
		sprintf(temp,"%g",bmin);
		Min->SetWindowText(temp);
		sprintf(temp,"%g",bmax);
		Max->SetWindowText(temp);
	} else {
		CString temp;
		Min->GetWindowText(temp);
		sscanf((LPCTSTR)temp,"%lf",&bmin);
		Max->GetWindowText(temp);
		sscanf((LPCTSTR)temp,"%lf",&bmax);
	}

	if (invertscale) {
		std::swap(bmin,bmax);
	}

	scaler brightness(255,0,bmin,bmax,true);
	
	int size = options.m_PointSize;
	size = (size/100.)*L/200.;

	for (int m=0;m<data->samples_x[order].size();++m) {
		double x = data->samples_x[order][m];
		double y = data->samples_y[order][m];
		double z = sqrt(1.-x*x-y*y);
		SCATMECH::Vector v(x,y,z);
		POINT _r = r(v);
		if (_r.x!=-1) {
			double inten = data->samples_i[order][m];
			if (showBRDF) inten/=z;
			int b = (showintensity) ? brightness(inten) : 0;
			CBrush brush;
			brush.CreateSolidBrush(RGB(b,b,b));
			pDC->SelectObject(&brush);
			CPen pen;
			pen.CreatePen(PS_SOLID,2,RGB(b,b,b));
			pDC->SelectObject(&pen);

			pDC->Ellipse(_r.x-size,_r.y-size,_r.x+size,_r.y+size);
			//pDC->Rectangle(_r.x-size,_r.y-size,_r.x+size,_r.y+size);
		}
	}
	// Draw longitude lines
	pDC->SelectObject(&dotpen);
	bool lastgood=false;
	for (int j=-180;j<=170;j+=10) {
		lastgood=false;
		for (int i=0;i<=90;i+=3) {
			SCATMECH::Vector v = SCATMECH::polar(1.,i*deg,j*deg);
			POINT _r = r(v);
			if (_r.x!=-1) {
				if (lastgood==true) pDC->LineTo(_r);
				else pDC->MoveTo(_r);
				lastgood=true;
			}
			else {
				pDC->MoveTo(_r);
				lastgood=false;
			}
		}
	}
	// Draw latitude lines...
	lastgood=false;
	for (int i=0;i<=90;i+=10) {
		lastgood=false;
		for (int j=-180;j<=180;j+=3) {
			SCATMECH::Vector v = SCATMECH::polar(1.,i*deg,j*deg);
			POINT _r = r(v);
			if (_r.x!=-1) {
				if (lastgood==true) pDC->LineTo(_r);
				else pDC->MoveTo(_r);
				lastgood=true;
			}
			else {
				pDC->MoveTo(_r);
				lastgood=false;
			}
		}
	}

	// Draw plane of incidence...
	lastgood=false;
	pDC->SelectObject(&boldpen);
	for (int k=0;k<=90;k+=10) {
			SCATMECH::Vector v = SCATMECH::polar(1.,k*deg,180*deg);
			POINT _r = r(v);
			if (_r.x!=-1) {
				if (lastgood==true) pDC->LineTo(_r);
				else pDC->MoveTo(_r);
				lastgood=true;
			}
			else {
				pDC->MoveTo(_r);
				lastgood=false;
			}
	}
	// Draw horizon...
	lastgood=false;
	pDC->SelectObject(&boldpen);
	for (int l=0;l<=360;l+=3) {
			SCATMECH::Vector v = SCATMECH::polar(1.,90*deg,l*deg);
			POINT _r = r(v);
			if (_r.x!=-1) {
				if (lastgood==true) pDC->LineTo(_r);
				else pDC->MoveTo(_r);
				lastgood=true;
			}
			else {
				pDC->MoveTo(_r);
				lastgood=false;
			}
	}
	
	double startthetai = data->samples_thetai[order];
	double costhetai = cos(startthetai);
	double sinthetai = sin(startthetai);

	SCATMECH::Vector beginbeam(sinthetai,0,costhetai);
	SCATMECH::Vector endbeam(-sinthetai,0,costhetai);

	pDC->SelectObject(&boldgreenpen);
	pDC->SelectObject(&whitebrush);

	SCATMECH::Vector origin(0,0,0);
	POINT originPOINT = r(origin);
	pDC->Ellipse(originPOINT.x-5,originPOINT.y-5,originPOINT.x+5,originPOINT.y+5);

	originPOINT = r(beginbeam);
	if (originPOINT.x!=-1) 	pDC->Ellipse(originPOINT.x-5,originPOINT.y-5,originPOINT.x+5,originPOINT.y+5);
	originPOINT = r(endbeam);
	if (originPOINT.x!=-1) 	pDC->Ellipse(originPOINT.x-5,originPOINT.y-5,originPOINT.x+5,originPOINT.y+5);


	SCATMECH::Vector arrow(0.95*sinthetai,0,0.95*costhetai);
	pDC->SelectObject(&brush);
	POINT triangle[3];
	triangle[0] = originPOINT;
	triangle[1] = originPOINT;
	triangle[2] = originPOINT;
	
	r.hide=false;
	pDC->MoveTo(r(beginbeam));
	pDC->LineTo(r(origin));
	pDC->LineTo(r(endbeam));

	pDC->Polygon(triangle,3);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

/////////////////////////////////////////////////////////////////////////////
// CWinPolarView diagnostics

#ifdef _DEBUG
void CWinPolarView::AssertValid() const
{
	CView::AssertValid();
}

void CWinPolarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinPolarView message handlers


void CWinPolarView::OnKillfocusPolarmin() 
{
	// TODO: Add your control notification handler code here
	GetDocument()->UpdateAllViews(NULL,0L,NULL);
	
}

void CWinPolarView::OnKillfocusPolarmax() 
{
	// TODO: Add your control notification handler code here
	GetDocument()->UpdateAllViews(NULL,0L,NULL);
	
}
/////////////////////////////////////////////////////////////////////////////
// PolarViewOptions dialog


PolarViewOptions::PolarViewOptions(CWnd* pParent /*=NULL*/)
	: CDialog(PolarViewOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(PolarViewOptions)
	m_InvertScale = FALSE;
	m_ShowIntensity = TRUE;
	m_PointSize = 100;
	m_AlwaysAutoScale = FALSE;
	m_FixZero = FALSE;
	m_ShowBRDF = FALSE;
	//}}AFX_DATA_INIT
}


void PolarViewOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PolarViewOptions)
	DDX_Check(pDX, IDC_INVERTSCALE, m_InvertScale);
	DDX_Check(pDX, IDC_SHOWINTENSITY, m_ShowIntensity);
	DDX_Text(pDX, IDC_EDIT1, m_PointSize);
	DDV_MinMaxInt(pDX, m_PointSize, 1, 1000);
	DDX_Check(pDX, IDC_ALWAYSAUTOSCALE, m_AlwaysAutoScale);
	DDX_Check(pDX, IDC_FIXZERO, m_FixZero);
	DDX_Check(pDX, IDC_SHOWBRDF, m_ShowBRDF);
	//}}AFX_DATA_MAP
}

PolarViewOptions CWinPolarView::options;

BEGIN_MESSAGE_MAP(PolarViewOptions, CDialog)
	//{{AFX_MSG_MAP(PolarViewOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PolarViewOptions message handlers


void CWinPolarView::OnOptions() 
{
	// TODO: Add your control notification handler code here
	options.DoModal();
	GetDocument()->UpdateAllViews(NULL,0L,NULL);
}

void CWinPolarView::OnAutoscale() 
{
	// TODO: Add your control notification handler code here
	CWinMIST2Doc* pDoc = GetDocument();
	MIST_Calculation_Data *data = &(pDoc->data);

	CEdit* Order = (CEdit*)(m_wndDialogBar.GetDlgItem(IDC_ORDER));
	BOOL fixzero = options.m_FixZero;

	int order;
	{ 
		CString temp;
		Order->GetWindowText(temp);
		sscanf((LPCTSTR)temp,"%d",&order);
	}
	if (order>=data->samples_i.size()-1) order = data->samples_i.size()-2;
	if (order<0) return;

	double bmin= data->samples_i[order].size()>0 ? data->samples_i[order][0] : 0;
	double bmax= bmin;
	if (!_finite(bmin)) bmin=0;
	if (!_finite(bmax)) bmax=0;

	for (int mm=0;mm<data->samples_i[order].size();++mm) {
		double inten = data->samples_i[order][mm];
		if (_finite(inten)) {
			if (inten<bmin) bmin=inten;
			if (inten>bmax) bmax=inten;
		}
	}
	if (fixzero) bmin=0;

	CEdit* Min = (CEdit*)(m_wndDialogBar.GetDlgItem(IDC_POLARMIN));
	CEdit* Max = (CEdit*)(m_wndDialogBar.GetDlgItem(IDC_POLARMAX));
	char temp[64];
	sprintf(temp,"%g",bmin);
	Min->SetWindowText(temp);
	sprintf(temp,"%g",bmax);
	Max->SetWindowText(temp);
	GetDocument()->UpdateAllViews(NULL,0L,NULL);
}

void CWinPolarView::OnChangeOrder() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
}

void CWinPolarView::OnKillfocusOrder() 
{
	// TODO: Add your control notification handler code here
	
}

void CWinPolarView::OnUpdateOrder() 
{
	// TODO: If this is a RICHEDIT control, the control will not 
	// send this notification unless you override the CView::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here

	GetDocument()->UpdateAllViews(NULL,0L,NULL);
	
}


BOOL CWinPolarView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CView::PreCreateWindow(cs);
}

void CWinPolarView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class

	CView::OnBeginPrinting(pDC, pInfo);

}

BOOL CWinPolarView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	return DoPreparePrinting(pInfo );	
}

void CWinPolarView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CView::OnEndPrinting(pDC, pInfo);
}

void CWinPolarView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CView::OnPrepareDC(pDC, pInfo);
}

void CWinPolarView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pInfo!=NULL) {
		showrect = pInfo->m_rectDraw;
	} else {
		GetClientRect( &showrect );
	}

	CView::OnPrint(pDC, pInfo);
}

void CWinPolarView::OnKillFocusPolarmin() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CView::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}
