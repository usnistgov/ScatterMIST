//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: NewIntegralDialog.cpp
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
// NewIntegralDialog.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "NewIntegralDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NewIntegralDialog dialog


NewIntegralDialog::NewIntegralDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NewIntegralDialog::IDD, pParent)
{
	type = 'u';
	//{{AFX_DATA_INIT(NewIntegralDialog)
	//}}AFX_DATA_INIT
}


void NewIntegralDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewIntegralDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NewIntegralDialog, CDialog)
	//{{AFX_MSG_MAP(NewIntegralDialog)
	ON_BN_CLICKED(IDC_CIRCLE, OnCircle)
	ON_BN_CLICKED(IDC_HEMI, OnHemi)
	ON_BN_CLICKED(IDC_POLYGON, OnPolygon)
	ON_BN_CLICKED(IDC_DIFFERENTIAL, OnDifferential)
	ON_BN_CLICKED(IDC_ANNULUS,OnAnnulus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NewIntegralDialog message handlers
/////////////////////////////////////////////////////////////////////////////
// NewCircleIntegralDialog dialog


NewCircleIntegralDialog::NewCircleIntegralDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NewCircleIntegralDialog::IDD, pParent) , result(NULL)
	, m_incpol(_T(""))
{
	//{{AFX_DATA_INIT(NewCircleIntegralDialog)
	m_theta = _T("");
	m_phi = _T("");
	m_alpha = _T("");
	m_pol = _T("");
	m_name = _T("");
	m_Comment = _T("");
	//}}AFX_DATA_INIT
}


void NewCircleIntegralDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewCircleIntegralDialog)
	DDX_Text(pDX, IDC_EDIT1, m_theta);
	DDX_Text(pDX, IDC_EDIT2, m_phi);
	DDX_Text(pDX, IDC_EDIT3, m_alpha);
	DDX_Text(pDX, IDC_POLARIZATION, m_pol);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_COMMENT, m_Comment);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT4, m_incpol);
}


BEGIN_MESSAGE_MAP(NewCircleIntegralDialog, CDialog)
	//{{AFX_MSG_MAP(NewCircleIntegralDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NewCircleIntegralDialog message handlers
/////////////////////////////////////////////////////////////////////////////
// NewPolygonDialog dialog


NewPolygonDialog::NewPolygonDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NewPolygonDialog::IDD, pParent), result(NULL)
	, m_incpol(_T(""))
{
	//{{AFX_DATA_INIT(NewPolygonDialog)
	m_phi = _T("");
	m_pol = _T("");
	m_theta = _T("");
	m_newtheta = _T("");
	m_newphi = _T("");
	m_name = _T("");
	m_Comment = _T("");
	//}}AFX_DATA_INIT
	dragging=false;
}


void NewPolygonDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewPolygonDialog)
	DDX_Control(pDX, IDC_ADDVERTEX, m_add);
	DDX_Control(pDX, IDC_MOVEUP, m_moveup);
	DDX_Control(pDX, IDC_MOVEDOWN, m_movedown);
	DDX_Control(pDX, IDC_DELETE, m_delete);
	DDX_Control(pDX, IDC_EDIT, m_edit);
	DDX_Control(pDX, IDC_VERTICES, m_vertices);
	DDX_Text(pDX, IDC_PHI, m_phi);
	DDX_Text(pDX, IDC_POL, m_pol);
	DDX_Text(pDX, IDC_THETA, m_theta);
	DDX_Text(pDX, IDC_EDIT1, m_newtheta);
	DDX_Text(pDX, IDC_EDIT2, m_newphi);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_COMMENT, m_Comment);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_COMMENT2, m_incpol);
}


BEGIN_MESSAGE_MAP(NewPolygonDialog, CDialog)
	//{{AFX_MSG_MAP(NewPolygonDialog)
	ON_BN_CLICKED(IDC_ADDVERTEX, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_NOTIFY(NM_CLICK, IDC_VERTICES, OnClickVertices)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMovedown)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NewPolygonDialog message handlers



BOOL NewPolygonDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_vertices.ModifyStyle(NULL,LVS_REPORT);

	m_vertices.InsertColumn( 0, "Theta [rad]",200);
	m_vertices.InsertColumn( 1, "Phi [rad]",200);

	m_vertices.SetColumnWidth(0,200);
	m_vertices.SetColumnWidth(1,200);
		
	m_phi = result.phip;
	m_pol = result.pol;
	m_theta = result.thetap;
	m_name = result.name;
	m_incpol = result.inpol;

	int i=0;
	for (PolygonIntegral::vlist::iterator p = result.vertices.begin();p!=result.vertices.end();++p) {
		m_vertices.InsertItem(i,p->first);
		m_vertices.SetItemText(i,1,p->second);
		++i;
	}
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NewPolygonDialog::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	int count = m_vertices.GetItemCount();
	result.vertices.clear();
	for (int i=0;i<count;++i) {
		CString t = m_vertices.GetItemText(i,0);
		CString p = m_vertices.GetItemText(i,1);
		result.vertices.push_back(std::pair<CString,CString>(t,p));
	}
	result.phip = m_phi;
	result.pol = m_pol;
	result.thetap = m_theta;
	result.name = m_name;
	result.inpol = m_incpol;
	CDialog::OnOK();
}

void NewPolygonDialog::OnClickVertices(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_vertices.GetFirstSelectedItemPosition();
	
	if (pos) {
		EnableEditButton(FALSE);
		EnableAddButton(TRUE);
		EnableDeleteButton(TRUE);
		EnableMoveButtons(TRUE);
		int i = m_vertices.GetNextSelectedItem(pos);

		m_newtheta = m_vertices.GetItemText(i,0);
		m_newphi = m_vertices.GetItemText(i,1);
	} else {
		EnableEditButton(FALSE);
		EnableAddButton(FALSE);
		EnableDeleteButton(FALSE);
		EnableMoveButtons(FALSE);
		m_newtheta = "";
		m_newphi = "";
	}


	UpdateData(FALSE);
}

void NewPolygonDialog::OnAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	POSITION pos = m_vertices.GetFirstSelectedItemPosition();
	int i = pos ? m_vertices.GetNextSelectedItem(pos) : 0;
	m_vertices.InsertItem(i,m_newtheta);
	m_vertices.SetItemText(i,1,m_newphi);
	m_vertices.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
	EnableEditButton(FALSE);
	EnableAddButton(TRUE);
	EnableDeleteButton(TRUE);
	EnableMoveButtons(TRUE);
}

void NewPolygonDialog::OnDelete() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	POSITION pos = m_vertices.GetFirstSelectedItemPosition();
	while (pos) {
		pos = m_vertices.GetFirstSelectedItemPosition();
		int nItem = m_vertices.GetNextSelectedItem(pos);
		m_vertices.DeleteItem(nItem);
	}
	EnableEditButton(FALSE);
	EnableAddButton(FALSE);
	EnableDeleteButton(FALSE);
	EnableMoveButtons(FALSE);
}

void NewPolygonDialog::OnEdit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	POSITION pos = m_vertices.GetFirstSelectedItemPosition();
	while (pos) {
		int i = m_vertices.GetNextSelectedItem(pos);
		m_vertices.SetItemText(i,0,m_newtheta);
		m_vertices.SetItemText(i,1,m_newphi);
		m_vertices.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
	}
	EnableEditButton(FALSE);
	EnableAddButton(TRUE);
	EnableDeleteButton(TRUE);
	EnableMoveButtons(TRUE);
}

BOOL NewCircleIntegralDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_theta = result.theta;
	m_phi = result.phi;
	m_alpha = result.alpha;
	m_pol = result.pol;
	m_name = result.name;
    m_incpol = result.inpol;
	UpdateData(FALSE);	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NewCircleIntegralDialog::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	result.theta = m_theta;
	result.phi = m_phi;
	result.alpha = m_alpha;
	result.pol = m_pol;
	result.name = m_name;
	result.inpol = m_incpol;
	CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
// NewHemisphere dialog


NewHemisphere::NewHemisphere(CWnd* pParent /*=NULL*/)
	: CDialog(NewHemisphere::IDD, pParent),result(NULL)
{
	//{{AFX_DATA_INIT(NewHemisphere)
	m_name = _T("");
	m_incpol = _T("");
	m_Comment = _T("");
	//}}AFX_DATA_INIT
}


void NewHemisphere::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewHemisphere)
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_EDIT2, m_incpol);
	DDX_Text(pDX, IDC_COMMENT, m_Comment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NewHemisphere, CDialog)
	//{{AFX_MSG_MAP(NewHemisphere)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NewHemisphere message handlers

void NewHemisphere::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	result.name = m_name;
	result.inpol = m_incpol;
	CDialog::OnOK();
}

BOOL NewHemisphere::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_name = result.name;
	m_incpol = result.inpol;
	UpdateData(FALSE);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// OutputDialog dialog


OutputDialog::OutputDialog(CWnd* pParent /*=NULL*/)
	: CDialog(OutputDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(OutputDialog)
	m_value = _T("");
	m_Comment = _T("");
	m_label = _T("");
	//}}AFX_DATA_INIT
}


void OutputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OutputDialog)
	DDX_Text(pDX, IDC_EDIT1, m_value);
	DDX_Text(pDX, IDC_COMMENT, m_Comment);
	DDX_Text(pDX, IDC_LABEL, m_label);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OutputDialog, CDialog)
	//{{AFX_MSG_MAP(OutputDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OutputDialog message handlers

void OutputDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL OutputDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NewPolygonDialog::OnMoveup() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	std::list<CString> thetalist;
	std::list<CString> philist;
	std::list<int> indexlist;

	POSITION pos = m_vertices.GetFirstSelectedItemPosition();
	int firsti = m_vertices.GetNextSelectedItem(pos);
	if (firsti==0) return;
	firsti--;

	int count=0;
	
	pos = m_vertices.GetFirstSelectedItemPosition();
	while (pos) {
		count++;
		int i = m_vertices.GetNextSelectedItem(pos);
		thetalist.push_back(m_vertices.GetItemText(i,0));
		philist.push_back(m_vertices.GetItemText(i,1));
		indexlist.push_back(i);
	}

	while (!indexlist.empty()) {
		m_vertices.DeleteItem(indexlist.back());
		indexlist.pop_back();
	}
	
	for (int j=0;j<count;++j) {
		m_vertices.InsertItem(j+firsti,thetalist.front());
		m_vertices.SetItemText(j+firsti,1,philist.front());
		m_vertices.SetItemState(j+firsti,LVIS_SELECTED,LVIS_SELECTED);
		thetalist.erase(thetalist.begin());
		philist.erase(philist.begin());
	}
	EnableEditButton(FALSE);
	EnableAddButton(TRUE);
	EnableDeleteButton(TRUE);
	EnableMoveButtons(TRUE);
	UpdateData(FALSE);
}

void NewPolygonDialog::OnMovedown() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	std::list<CString> thetalist;
	std::list<CString> philist;
	std::list<int> indexlist;

	POSITION pos = m_vertices.GetFirstSelectedItemPosition();
	int firsti = m_vertices.GetNextSelectedItem(pos);
	firsti++;

	int count=0;
	
	pos = m_vertices.GetFirstSelectedItemPosition();
	while (pos) {
		count++;
		int i = m_vertices.GetNextSelectedItem(pos);
		thetalist.push_back(m_vertices.GetItemText(i,0));
		philist.push_back(m_vertices.GetItemText(i,1));
		indexlist.push_back(i);
	}
	
	if (firsti+count>m_vertices.GetItemCount()) return;

	while (!indexlist.empty()) {
		m_vertices.DeleteItem(indexlist.back());
		indexlist.pop_back();
	}
	
	for (int j=0;j<count;++j) {
		m_vertices.InsertItem(j+firsti,thetalist.front());
		m_vertices.SetItemText(j+firsti,1,philist.front());
		m_vertices.SetItemState(j+firsti,LVIS_SELECTED,LVIS_SELECTED);
		thetalist.erase(thetalist.begin());
		philist.erase(philist.begin());
	}
	EnableEditButton(FALSE);
	EnableAddButton(TRUE);
	EnableDeleteButton(TRUE);
	EnableMoveButtons(TRUE);
	UpdateData(FALSE);
}
/////////////////////////////////////////////////////////////////////////////
// NewDifferentialDialog dialog


NewDifferentialDialog::NewDifferentialDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NewDifferentialDialog::IDD, pParent), result(NULL)
{
	//{{AFX_DATA_INIT(NewDifferentialDialog)
	m_name = _T("");
	m_phi = _T("");
	m_theta = _T("");
	m_pol = _T("");
	m_Comment = _T("");
	m_incpol = _T("");
	//}}AFX_DATA_INIT
}


void NewDifferentialDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewDifferentialDialog)
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_AZIMUTH, m_phi);
	DDX_Text(pDX, IDC_POLAR, m_theta);
	DDX_Text(pDX, IDC_POLARIZATION, m_pol);
	DDX_Text(pDX, IDC_COMMENT, m_Comment);
	DDX_Text(pDX, IDC_INCPOL, m_incpol);
	//}}AFX_DATA_MAP
}

BOOL NewDifferentialDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_theta = result.theta;
	m_phi = result.phi;
	m_pol = result.pol;
	m_name = result.name;
    m_incpol = result.inpol;
	UpdateData(FALSE);	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NewDifferentialDialog::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	result.theta = m_theta;
	result.phi = m_phi;
	result.pol = m_pol;
	result.name = m_name;
    result.inpol = m_incpol;
	CDialog::OnOK();
}
BEGIN_MESSAGE_MAP(NewDifferentialDialog, CDialog)
	//{{AFX_MSG_MAP(NewDifferentialDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void NewPolygonDialog::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	POSITION pos = m_vertices.GetFirstSelectedItemPosition();
	if (pos) {
		EnableEditButton(TRUE);
	}
	EnableAddButton(TRUE);
}

void NewPolygonDialog::OnChangeEdit2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	POSITION pos = m_vertices.GetFirstSelectedItemPosition();
	if (pos) {
		EnableEditButton(TRUE);
	}
	EnableAddButton(TRUE);
}

void NewIntegralDialog::OnCircle() 
{
	type = 'c';
}

void NewIntegralDialog::OnHemi() 
{
	type = 'h';	
}

void NewIntegralDialog::OnPolygon() 
{
	type = 'p';
}

void NewIntegralDialog::OnDifferential() 
{
	type = 'd';
}

void NewIntegralDialog::OnAnnulus()
{
	type = 'a';
}

// C:\Programming\mist\newintegraldialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinMIST2.h"
#include "C:\Programming\mist\newintegraldialog.h"


// NewSEHemisphere dialog

IMPLEMENT_DYNAMIC(NewAnnulus, CDialog)

NewAnnulus::NewAnnulus(CWnd* pParent /*=NULL*/)
	: CDialog(NewAnnulus::IDD, pParent),result(NULL)
	, m_name(_T(""))
	, m_inpol(_T(""))
	, m_pol(_T(""))
	, m_comment(_T(""))
	, m_outerpolar(_T(""))
	, m_outerazimuthal(_T(""))
	, m_outeralpha(_T(""))
	, m_innerpolar(_T(""))
	, m_innerazimuthal(_T(""))
	, m_inneralpha(_T(""))
{
	//{{AFX_DATA_INIT(NewHemisphere)
	m_outerpolar = _T("");
	m_outerazimuthal = _T("");
	m_outeralpha = _T("");
	m_innerpolar = _T("");
	m_innerazimuthal = _T("");
	m_inneralpha = _T("");
	m_name = _T("");
	m_inpol = _T("");
	m_pol = _T("");
	m_comment = _T("");
	//}}AFX_DATA_INIT

}

NewAnnulus::~NewAnnulus()
{
}

void NewAnnulus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_INCPOL, m_inpol);
	DDX_Text(pDX, IDC_POL, m_pol);
	DDX_Text(pDX, IDC_COMMENT, m_comment);
	DDX_Text(pDX, IDC_OUTERPOLAR, m_outerpolar);
	DDX_Text(pDX, IDC_OUTERAZIMUTHAL, m_outerazimuthal);
	DDX_Text(pDX, IDC_OUTERALPHA, m_outeralpha);
	DDX_Text(pDX, IDC_INNERPOLAR, m_innerpolar);
	DDX_Text(pDX, IDC_INNERAZIMUTHAL, m_innerazimuthal);
	DDX_Text(pDX, IDC_INNERALPHA, m_inneralpha);
}

BOOL NewAnnulus::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_name = result.name;
	m_inpol = result.inpol;
	m_pol = result.pol;
	m_comment = result.comment;
	m_outerpolar = result.outerpolar;
	m_outerazimuthal = result.outerazimuthal;
	m_outeralpha = result.outeralpha;
	m_innerpolar = result.innerpolar;
	m_innerazimuthal = result.innerazimuthal;
	m_inneralpha = result.inneralpha;
	UpdateData(FALSE);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}


BEGIN_MESSAGE_MAP(NewAnnulus, CDialog)
	ON_BN_CLICKED(IDOK, &NewAnnulus::OnBnClickedOk)
END_MESSAGE_MAP()


// NewSEHemisphere message handlers

void NewAnnulus::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	result.outeralpha = m_outeralpha;
	result.outerazimuthal = m_outerazimuthal;
	result.outerpolar = m_outerpolar;
	result.inneralpha = m_inneralpha;
	result.innerazimuthal = m_innerazimuthal;
	result.innerpolar = m_innerpolar;
	result.name = m_name;
    result.inpol = m_inpol;
    result.pol = m_pol;
	result.comment = m_comment;
	OnOK();
}

