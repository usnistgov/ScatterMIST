//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: ModelBrowser.cpp
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
// ModelBrowser.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "ParameterWizard.h"
#include "ModelBrowser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ModelBrowser

IMPLEMENT_DYNCREATE(ModelBrowser, CMDIChildWnd)

ModelBrowser::ModelBrowser() : itree(SCATMECH::Model::inheritance)
{
}

ModelBrowser::~ModelBrowser()
{
}


BEGIN_MESSAGE_MAP(ModelBrowser, CMDIChildWnd)
	//{{AFX_MSG_MAP(ModelBrowser)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ModelBrowser message handlers

BOOL ModelBrowser::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	//m_splitterwnd.CreateStatic(cv,1,2);
	// create a splitter with 1 row, 2 columns
	if (!m_splitterwnd1.CreateStatic(this, 1, 2))
	{
		AfxMessageBox("Failed to CreateStaticSplitter  #1\n");
		return FALSE;
	}

	// add the first splitter pane - the default view in column 0
	if (!m_splitterwnd1.CreateView(0, 0,
		RUNTIME_CLASS(ModelBrowserTree),CSize(300, 10), pContext))
	{
		AfxMessageBox("Failed to create first pane\n");
		return FALSE;
	} 
	pTreeView = (ModelBrowserTree*)m_splitterwnd1.GetPane(0,0);
	pTreeView->SetParentBrowser(this);

	// add the second splitter pane - which is a nested splitter with 2 rows
	if (!m_splitterwnd2.CreateStatic(
		&m_splitterwnd1,     // our parent window is the first splitter
		2, 1,               // the new splitter is 2 rows, 1 column
		WS_CHILD | WS_VISIBLE | WS_BORDER,  // style, WS_BORDER is needed
		m_splitterwnd1.IdFromRowCol(0, 1)
			// new splitter is in the first row, 2nd column of first splitter
	   ))
	{
		TRACE0("Failed to create nested splitter\n");
		return FALSE;
	}

	// add the second splitter pane - an input view in column 1
	if (!m_splitterwnd2.CreateView(0, 0,
		RUNTIME_CLASS(ModelBrowserForm), CSize(10, 100), pContext))
	{
		AfxMessageBox("Failed to create second pane\n");
		return FALSE;
	}
	pFormView = (ModelBrowserForm*)m_splitterwnd2.GetPane(0,0);
	pFormView->SetParentBrowser(this);

	// add the second splitter pane - an input view in column 1
	if (!m_splitterwnd2.CreateView(1, 0,
		RUNTIME_CLASS(ModelBrowserList), CSize(10, 100), pContext))
	{
		AfxMessageBox("Failed to create second pane\n");
		return FALSE;
	}
	pListView = (ModelBrowserList*)m_splitterwnd2.GetPane(1,0);
	pListView->SetParentBrowser(this);

	// activate the input view
	SetActiveView((CView*)m_splitterwnd1.GetPane(0,0));

	pTreeView->ModifyStyle(NULL,TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP );
	itree.BuildTreeCtrl(pTreeView->GetTreeCtrl(),NULL);

	CListCtrl& list = pListView->GetListCtrl();

	list.ModifyStyle(NULL,LVS_REPORT);

	list.InsertColumn( 0, "Parameter",300);
	list.InsertColumn( 1, "Description",300);
	list.InsertColumn( 2, "Datatype",300);
	list.InsertColumn( 3, "Default Value",300);

	list.SetColumnWidth(0,100);
	list.SetColumnWidth(1,300);
	list.SetColumnWidth(2,200);
	list.SetColumnWidth(3,200);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// ModelBrowserForm

IMPLEMENT_DYNCREATE(ModelBrowserForm, CFormView)

ModelBrowserForm::ModelBrowserForm()
	: CFormView(ModelBrowserForm::IDD)
{
	//{{AFX_DATA_INIT(ModelBrowserForm)
	//}}AFX_DATA_INIT
}

ModelBrowserForm::~ModelBrowserForm()
{
}

void ModelBrowserForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ModelBrowserForm)
	DDX_Control(pDX, IDC_MODELNAME, m_ModelName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ModelBrowserForm, CFormView)
	//{{AFX_MSG_MAP(ModelBrowserForm)
	ON_BN_CLICKED(IDC_CREATEMODEL, OnCreateModel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ModelBrowserForm diagnostics

#ifdef _DEBUG
void ModelBrowserForm::AssertValid() const
{
	CFormView::AssertValid();
}

void ModelBrowserForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ModelBrowserForm message handlers
/////////////////////////////////////////////////////////////////////////////
// ModelBrowserTree

IMPLEMENT_DYNCREATE(ModelBrowserTree, CTreeView)

ModelBrowserTree::ModelBrowserTree()
{
}

ModelBrowserTree::~ModelBrowserTree()
{
}


BEGIN_MESSAGE_MAP(ModelBrowserTree, CTreeView)
	//{{AFX_MSG_MAP(ModelBrowserTree)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ModelBrowserTree drawing

void ModelBrowserTree::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// ModelBrowserTree diagnostics

#ifdef _DEBUG
void ModelBrowserTree::AssertValid() const
{
	CTreeView::AssertValid();
}

void ModelBrowserTree::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ModelBrowserTree message handlers

void ModelBrowserTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	using namespace SCATMECH;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
		
	HTREEITEM item = GetTreeCtrl().GetSelectedItem( );
	const SCATMECH::Inheritance* inheritance = parentbrowser->itree.get_inheritance(item);
	
	if (inheritance!=NULL) { 
		SCATMECH::ModelParameterList mpl;
		CListCtrl& list = parentbrowser->pListView->GetListCtrl();
		
		list.DeleteAllItems();
		mpl = inheritance->get_parameters();
		int i=0;
		for (ModelParameterList::iterator q=mpl.begin();q!=mpl.end();++q) {
			list.InsertItem(i,(*q)->name.c_str());
			//list.SetItemText(0,0,"World");
			list.SetItemText(i,1,(*q)->description.c_str());
			list.SetItemText(i,2,(*q)->type.c_str());
			list.SetItemText(i,3,(*q)->defaultvalue.c_str());
			i++;
		}
		CEdit& modelname = parentbrowser->pFormView->m_ModelName;
		CString text = "Model: ";
		text += inheritance->get_name().c_str();
		text += "\r\nDescription: ";
		text += inheritance->get_description().c_str();

		modelname.SetWindowText(text);

	} else throw SCATMECH_exception("Problem here!");

	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////
// ModelBrowserList

IMPLEMENT_DYNCREATE(ModelBrowserList, CListView)

ModelBrowserList::ModelBrowserList()
{
}

ModelBrowserList::~ModelBrowserList()
{
}


BEGIN_MESSAGE_MAP(ModelBrowserList, CListView)
	//{{AFX_MSG_MAP(ModelBrowserList)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ModelBrowserList drawing

void ModelBrowserList::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// ModelBrowserList diagnostics

#ifdef _DEBUG
void ModelBrowserList::AssertValid() const
{
	CListView::AssertValid();
}

void ModelBrowserList::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ModelBrowserList message handlers

void ModelBrowserForm::OnCreateModel() 
{
	// TODO: Add your control notification handler code here
	
}
