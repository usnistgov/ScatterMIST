//******************************************************************************
//** Modeled Integrated Scattering Tool (MIST)
//** 
//** File: ParameterWizard.cpp
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
// ParameterWizard.cpp : implementation file
//

#include "stdafx.h"
#include "winmist2.h"
#include "WinMIST2Doc.h"
#include "ParameterWizard.h"
#include "brdf.h"
#include "NewIntegralDialog.h"
#include "MainFrm.h"
#include <list>
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ParameterWizard dialog

extern CMultiDocTemplate* pWinMIST2DocTemplate;


using namespace SCATMECH;
using namespace std;

string ClipBoardPrefix = "MISTClipBoardData ";

void CopyToClipboard(const string& str)
{
	if (OpenClipboard(NULL)) {
		EmptyClipboard();

		HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE,str.size()+ClipBoardPrefix.size()+1);

		char* pchData = (char*)GlobalLock(hClipboardData);
		
		for (int i=0;i<ClipBoardPrefix.size();++i) {
			pchData[i]=ClipBoardPrefix[i];
		}

		for (int i=0;i<str.size();++i) {
			pchData[ClipBoardPrefix.size()+i]=str[i];
		}
		pchData[ClipBoardPrefix.size()+str.size()]=0;
		  
		GlobalUnlock(hClipboardData);
		 
		SetClipboardData(CF_TEXT,hClipboardData);
		 
		CloseClipboard();
	} else {
		throw SCATMECH_exception("Cannot open clipboard");
	}
}

string GetFromClipboard()
{
	if (OpenClipboard(NULL)) {
		HANDLE hClipboardData = GetClipboardData(CF_TEXT);

		char *pchData = (char*)GlobalLock(hClipboardData);

		char* prefix = &ClipBoardPrefix[0];

		for (int i=0;i<ClipBoardPrefix.size();++i) {
			if (pchData[i]!=prefix[i]) {
				GlobalUnlock(hClipboardData);
				CloseClipboard();
				throw SCATMECH_exception("Invalid data on clipboard");
			}
		}

		string strFromClipboard = &pchData[ClipBoardPrefix.size()];

		GlobalUnlock(hClipboardData);

		CloseClipboard();
		return strFromClipboard;
	} else {
		throw SCATMECH_exception("Cannot open clipboard");
	}
}

HTREEITEM reg(HTREEITEM parent,WizardElement *p,bool read) {
	ParameterWizard2 *pw = p->pw;
	if (read) {
		p->item = pw->m_Tree->InsertItem(p->show(),parent,TVI_LAST);
	} else {
		HTREEITEM current = pw->m_Tree->GetSelectedItem( );
		if (current==parent && p->can_remove()) {
			p->item = pw->m_Tree->InsertItem(p->show(),parent,TVI_FIRST);			
		} else {
			p->item = pw->m_Tree->InsertItem(p->show(),parent,current);			
		}
	}
	pw->treemap.insert(WizardElementPair(p->item,p));
	pw->m_Tree->EnsureVisible(p->item);
	pw->UpdateData();
	pw->Elements.push_back(counted_ptr<WizardElement>(p));
	return p->item;
}

WizardElement* GetWizardElement(ParameterWizard2* _pw,WizardElementType code)
{
	switch (code) {
	case WET_USERVARIABLE:	return new UserVariable(_pw);
	case WET_REQUIREDVARIABLE: return new RequiredVariable(_pw);
	case WET_MODELVARIABLE: return new ModelVariable(_pw);
	case WET_MODELMODELVARIABLE: return new ModelModelVariable(_pw);
	case WET_MODELHEADING: return _pw->treemap[_pw->modelitem];
	case WET_FORLOOP: return new ForLoop(_pw);
	case WET_HEMIINTEGRAL: return new HemiIntegral(_pw);
	case WET_CIRCLEINTEGRAL: return new CircleIntegral(_pw);
	case WET_POLYGONINTEGRAL: return new PolygonIntegral(_pw);
	case WET_DIFFERENTIALINTEGRAL: return new DifferentialIntegral(_pw);
	case WET_OUTPUT: return new Output(_pw);
	case WET_FILESELEMENT: return new FilesElement(_pw);
	case WET_USERSECTIONHEADING: return new UserSectionHeading(_pw);
	case WET_REQUIREDSECTIONHEADING: return new RequiredSectionHeading(_pw);
	case WET_PARAMETERSECTIONHEADING: return new ParameterSectionHeading(_pw);
	case WET_FORLOOPSECTIONHEADING: return new ForLoopSectionHeading(_pw);
	case WET_INTEGRALSECTIONHEADING: return new IntegralSectionHeading(_pw);
	case WET_OUTPUTSSECTIONHEADING: return new OutputsSectionHeading(_pw);
	case WET_FILESSECTIONHEADING: return new FilesSectionHeading(_pw);
	case WET_LISTLOOP: return new ListLoop(_pw);
	case WET_SEHEMIINTEGRAL: throw SCATMECH_exception("Specular excluded hemi integral no longer supported");
	case WET_ANNULUSINTEGRAL: return new AnnulusIntegral(_pw);
	default: return NULL;
	}
}

HTREEITEM ParameterWizard2::GetTopParentItem(HTREEITEM item)
{
	HTREEITEM parent;
	while ((parent=m_Tree->GetParentItem(item))!=NULL) {
		item = parent;
	}
	return item;
}

CString
ParameterDescription(const ModelParameterBase* modelparameter)
{
	CString text;
	text = "Parameter: ";
	text += modelparameter->name.c_str();
	text += "\r\nDescription: ";
	text += modelparameter->description.c_str();
	text += "\r\nType: ";
	text += modelparameter->type.c_str();
	text += "\r\nDefault: ";
	text += modelparameter->defaultvalue.c_str();
	return text;
}

CString
RequiredVariableDescription(const CString& param)
{
	if (param=="minsamples") {
		return "The minimum number of integration points for any integral";
	}
	if (param=="differential") {
		return "The differential solid angle of integration in steradians";
	}
	if (param=="incidentangle") {
		return "The incident angle in radians\r\nHint: Use *deg to convert from degrees to radians";
	}
	if (param=="incidentpol") {
		return "The incident Stokes vector\r\n"
			   "Hint: Use (1,0,0,0) for unpolarized light,\r\n"
			   "      Use (1,1,0,0) for s-polarized light,\r\n"
			   "      Use (1,-1,0,0) for p-polarized light";
	}
	if (param=="rotation") {
		return "The sample rotation about the surface normal in radians\r\nHint: Use *deg to convert from degrees to radians";
	}

	else return "";
}

void ParameterWizard2::Finalize(HTREEITEM item,CString& result)
{
	HTREEITEM child;
	child = m_Tree->GetChildItem(item);
	while (child != NULL) {
		result += treemap[child]->final();
		//m_Tree->DeleteItem(child);
		//treemap.erase(child);
		Finalize(child,result);
		child = m_Tree->GetNextSiblingItem(child);
	}
}

void ParameterWizard2::WriteElement(HTREEITEM item,CArchive& ar)
{
	HTREEITEM child;
	child = m_Tree->GetChildItem(item);
	while (child != NULL) {
		treemap[child]->write(ar);
		WriteElement(child,ar);
		child = m_Tree->GetNextSiblingItem(child);
	}
}

void ParameterWizard2::WriteElementText(HTREEITEM item,ostream& ar)
{
	HTREEITEM child;
	child = m_Tree->GetChildItem(item);
	while (child != NULL) {
		treemap[child]->writeText(ar);
		ar << endl;
		WriteElementText(child,ar);
		child = m_Tree->GetNextSiblingItem(child);
	}
}

HTREEITEM ParameterWizard2::MoveTreeItem(HTREEITEM item,HTREEITEM movebefore)
{
	//AfxMessageBox("About to move " + m_Tree.GetItemText(item) + " to " + m_Tree->GetItemText(moveafter));
	GetDocument()->SetModifiedFlag();

	WizardElement *p = treemap[item];
	CString t = m_Tree->GetItemText(item);
	HTREEITEM parent = m_Tree->GetParentItem(item);
	HTREEITEM nextparent = m_Tree->GetNextSiblingItem(parent);

	HTREEITEM newparent = m_Tree->GetParentItem(movebefore);
	if (parent != newparent && nextparent!=movebefore) return NULL;

	m_Tree->DeleteItem(item);
	treemap.erase(item);
	
	HTREEITEM moveafter = m_Tree->GetNextItem(movebefore,TVGN_PREVIOUSVISIBLE);
	
	if (moveafter==parent) moveafter = TVI_FIRST;
	if (movebefore==nextparent) moveafter = TVI_LAST;
	item = m_Tree->InsertItem(t,parent,moveafter);
	treemap[item]=p;
	m_Tree->SelectItem(item);

	p->item = item;
	p->update();
	return item;
}

void ParameterWizard2::MakeDocument(CString& result) 
{
	// TODO: Add extra validation here
	outputcount=1;

	result += treemap[modelitem]->final();
	result += "VARIABLES: \r\n";
	Finalize(useritem,result);
	result += "\r\n";
	Finalize(requireditem,result);
	result += "\r\n";
	Finalize(paramitem,result);
	result += "END \r\n\r\n";

	result += "VARY: \r\n";
	Finalize(forloopitem,result);
	result += "END \r\n\r\n";

	result += "INTEGRALS: \r\n";
	Finalize(integralitem,result);
	result += "END \r\n\r\n";
	
	result += "OUTPUTS: \r\n";
	Finalize(outputsitem,result);
	result += "END \r\n\r\n";
	
	result += "FILES: \r\n";
	Finalize(filesitem,result);
	result += "END \r\n\r\n";
}

void ParameterWizard2::OnAdd() 
{
	GetDocument()->SetModifiedFlag();
	// TODO: Add your control notification handler code here
	HTREEITEM item = m_Tree->GetSelectedItem( );
	HTREEITEM parent = GetTopParentItem(item);
	WizardElement* element = treemap[item];

	element->add();
}

void ParameterWizard2::OnRemove() 
{
	GetDocument()->SetModifiedFlag();
	// TODO: Add your control notification handler code here
	HTREEITEM item = m_Tree->GetSelectedItem( );
	HTREEITEM parent = GetTopParentItem(item);
	WizardElement* element = treemap[item];

	element->remove();
}

void ParameterWizard2::OnCopy() 
{
	// TODO: Add your command handler code here
	HTREEITEM item = m_Tree->GetSelectedItem( );
	HTREEITEM parent = GetTopParentItem(item);
	WizardElement* element = treemap[item];
	
	ostringstream os;
	element->writeText(os);
	CopyToClipboard(os.str());
}

void ParameterWizard2::OnPaste() 
{
	// TODO: Add your command handler code here

	GetDocument()->SetModifiedFlag();

	try {
		string clip = GetFromClipboard();
		istringstream clipstream(clip);
			
		int wet;
		clipstream >> wet;
		while (!clipstream.eof()) {
			WizardElement* we = GetWizardElement(this,(WizardElementType)wet);
			we->readText(clipstream);
			clipstream >> wet;
		}
	} 
	catch (exception& e) {
		AfxMessageBox("Failure pasting from clipboard");
	}
	catch (CException* e) {
		AfxMessageBox("Failure pasting from clipboard");
		e->Delete();
	}
}

void ParameterWizard2::OnCut() 
{
	// TODO: Add your command handler code here
	OnCopy();
	OnRemove();
}

bool WizardElement::can_paste() 
{
	HTREEITEM item = pw->m_Tree->GetSelectedItem( );
	HTREEITEM parent = pw->GetTopParentItem(item);
	return true;
	//else return false;	
}

void ParameterWizard2::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEdit();	
	*pResult = 0;
}

void ParameterWizard2::OnEnter() 
{
	// TODO: Add your command handler code here
	OnEdit();		
}

void ParameterWizard2::OnEdit() 
{
	// TODO: Add your control notification handler code here
	HTREEITEM item = m_Tree->GetSelectedItem( );
	HTREEITEM parent = GetTopParentItem(item);
	WizardElement* element = treemap[item];

	element->edit(); 
}

/////////////////////////////////////////////////////////////////////////////
// NewUserVariableDialog dialog

NewUserVariableDialog::NewUserVariableDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NewUserVariableDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(NewUserVariableDialog)
	m_name = _T("");
	m_value = _T("");
	m_description = _T("");
	m_Comment = _T("");
	//}}AFX_DATA_INIT

	m_readonly = false;
	m_title = "Variable";
}


void NewUserVariableDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewUserVariableDialog)
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_EDIT2, m_value);
	DDX_Text(pDX, IDC_DESCRIPTION, m_description);
	DDX_Text(pDX, IDC_COMMENT, m_Comment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NewUserVariableDialog, CDialog)
	//{{AFX_MSG_MAP(NewUserVariableDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NewUserVariableDialog dialog

ModelVariableDialog::ModelVariableDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ModelVariableDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(ModelVariableDialog)
	m_name = _T("");
	m_value = _T("");
	m_description = _T("");
	m_string = FALSE;
	m_Comment = _T("");
	//}}AFX_DATA_INIT
}


void ModelVariableDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ModelVariableDialog)
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_EDIT2, m_value);
	DDX_Text(pDX, IDC_DESCRIPTION, m_description);
	DDX_Check(pDX, IDC_STRING, m_string);
	DDX_Text(pDX, IDC_COMMENT, m_Comment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ModelVariableDialog, CDialog)
	//{{AFX_MSG_MAP(ModelVariableDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NewUserVariableDialog message handlers
/////////////////////////////////////////////////////////////////////////////
// NewForLoopDialog dialog


NewForLoopDialog::NewForLoopDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NewForLoopDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(NewForLoopDialog)
	m_by = _T("");
	m_from = _T("");
	m_to = _T("");
	m_variable = _T("");
	m_Comment = _T("");
	//}}AFX_DATA_INIT
}


void NewForLoopDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewForLoopDialog)
	DDX_Text(pDX, IDC_BY, m_by);
	DDX_Text(pDX, IDC_FROM, m_from);
	DDX_Text(pDX, IDC_TO, m_to);
	DDX_Text(pDX, IDC_VARIABLE, m_variable);
	DDX_Text(pDX, IDC_COMMENT, m_Comment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NewForLoopDialog, CDialog)
	//{{AFX_MSG_MAP(NewForLoopDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NewForLoopDialog message handlers



/////////////////////////////////////////////////////////////////////////////
// ChooseModelDialog dialog


ChooseModelDialog::ChooseModelDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ChooseModelDialog::IDD, pParent)
	, m_maintain(false)
{
	//{{AFX_DATA_INIT(ChooseModelDialog)
	m_description = _T("");
	m_Comment = _T("");
	m_maintain = true;
	//}}AFX_DATA_INIT
}


void ChooseModelDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ChooseModelDialog)
	DDX_Control(pDX, IDC_COMBO, m_combo);
	DDX_Text(pDX, IDC_DESCRIPTION, m_description);
	DDX_Text(pDX, IDC_COMMENT, m_Comment);
	DDX_Check(pDX, IDC_CHECK1, m_maintain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ChooseModelDialog, CDialog)
	//{{AFX_MSG_MAP(ChooseModelDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ChooseModelDialog message handlers

int ChooseModelDialog::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	

	return CDialog::DoModal();
}

BOOL ChooseModelDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	string temp = (LPCSTR)parentmodel;

	const Inheritance *parent = Model::inheritance.get_named_inheritance(temp,true);

	InheritanceList inheritancelist = parent->get_progeny();
	for (InheritanceList::iterator p=inheritancelist.begin();p!=inheritancelist.end();++p) {
		if ((*p)->is_instantiable()) m_combo.AddString((*p)->get_name().c_str());
	}

	//m_combo.SetWindowText(returnedmodel);
	m_combo.SelectString(-1,returnedmodel);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ChooseModelDialog::OnOK() 
{
	// TODO: Add extra validation here
	m_combo.GetWindowText(returnedmodel);
		
	CDialog::OnOK();
}


int UserVariable::edit()
{
	int result;
	NewUserVariableDialog dialog;
	dialog.m_name = name;
	dialog.m_value = value;
	dialog.m_readonly=false;
	dialog.m_title = "User Variable";
	dialog.m_description="Enter variable definition here.\r\n"
						 "Hint: Any arithmetic expression, using previously defined variables, can be used.";
	dialog.m_Comment = comment;

	if (IDOK==(result=dialog.DoModal())) {
		name=dialog.m_name;
		value=dialog.m_value;
		comment = dialog.m_Comment;
		pw->GetDocument()->SetModifiedFlag();
	}
	update();
	return result;
}	

int FilesElement::edit()
{
	int result;
	NewUserVariableDialog dialog;
	dialog.m_name = name;
	dialog.m_value = value;
	dialog.m_readonly=false;
	dialog.m_title = "Data File Parameter";
	dialog.m_description="Enter output files here\r\n";
	dialog.m_Comment = comment;

	if (IDOK==(result=dialog.DoModal())) {
		name=dialog.m_name;
		value=dialog.m_value;
		comment = dialog.m_Comment;
		pw->GetDocument()->SetModifiedFlag();
	}
	update();
	return result;
}	

int RequiredVariable::edit()
{
	int result;
	NewUserVariableDialog dialog;
	dialog.m_name = name;
	dialog.m_value = value;
	dialog.m_readonly=true;
	dialog.m_title = "Required Variable";
	dialog.m_description=RequiredVariableDescription(name);
	dialog.m_Comment = comment;

	if (IDOK==(result=dialog.DoModal())) {
		name=dialog.m_name;
		value=dialog.m_value;
		comment = dialog.m_Comment;
		pw->GetDocument()->SetModifiedFlag();
	}
	update();
	return result;
}

int ModelVariable::edit() 
{
	int result;
	ModelVariableDialog dialog;
	dialog.m_name = name;
	dialog.m_value = value;
	dialog.m_string = stringvar;
	dialog.m_description=ParameterDescription(modelparameter);
	dialog.m_Comment = comment;

	if (IDOK==(result=dialog.DoModal())) {
		value=dialog.m_value;
		comment = dialog.m_Comment;
		stringvar = dialog.m_string;
		pw->GetDocument()->SetModifiedFlag();
	}

	update();
	return result;
}

CString ModelVariable::final() 
{
	CString comment = "\t; " + CString(modelparameter->description.c_str()) + " <" + CString(modelparameter->type.c_str())+ ">";
	comment.Remove('\n');
	comment.Remove('\r');
	if (stringvar) {
		return comment + "\r\n\t" + show() + sret + sret;
	} else {	
		return comment + "\r\n\t" + show() + sret + "\t#" + name + " = " + name + sret + sret;
	}
}

void ModelModelVariable::BuildSubParameters()
{
	pw->DeleteChildren(item);

	ModelParameterList mpl;
	mpl.clear();
	const Inheritance *inheritance = Model::inheritance.get_named_inheritance((LPCSTR)value,true);
	if (inheritance) {			
		mpl = inheritance->get_parameters();
		
		for (ModelParameterList::iterator q=mpl.begin();q!=mpl.end();++q) {				
			if ((*q)->get_inheritance()) {
				ModelModelVariable *p = 
					new ModelModelVariable(pw,*q,name + "."+(*q)->name.c_str(),(*q)->defaultvalue.c_str());
				::reg(item,p,true);
				p->BuildSubParameters();
			} else {
				::reg(item,new ModelVariable(pw,*q, name +"."+(*q)->name.c_str(),(*q)->defaultvalue.c_str()),true);
			}
		}
	}
	update();
}

int ModelModelVariable::edit() 
{
	int result;
	ChooseModelDialog dialog;
	dialog.parentmodel = modelparameter->get_inheritance()->get_name().c_str();
	dialog.returnedmodel = value;
	//dialog.m_description = modelparameter->get_inheritance()->get_description().c_str();
	dialog.m_description = ParameterDescription(modelparameter);
	dialog.m_Comment = comment;
	dialog.m_maintain = false;
	if (IDOK==(result=dialog.DoModal())) {
		value = dialog.returnedmodel;
		
		ostringstream os;
		if (dialog.m_maintain) {
			pw->WriteElementText(item,os);
		}
		pw->GetDocument()->SetModifiedFlag();
		comment = dialog.m_Comment;

		BuildSubParameters();
		if (dialog.m_maintain) {
			istringstream clipstream(os.str());
			
			int wet;
			clipstream >> wet;
			while (!clipstream.eof()) {
				WizardElement* we = GetWizardElement(pw,(WizardElementType)wet);
				we->readText(clipstream);
				clipstream >> wet;
			}
		}
	}
	update();
	return result;
}

int ModelHeading::edit()
{
	int result;
	ChooseModelDialog dialog;
	dialog.parentmodel = "BRDF_Model";
	dialog.returnedmodel = value;
	dialog.m_description = "BRDF model to be integrated\r\nType: BRDF_Model";
	dialog.m_Comment = comment;
	dialog.m_maintain = maintain;
	if (IDOK==(result=dialog.DoModal())) {
		value = dialog.returnedmodel;
		comment = dialog.m_Comment;
		maintain = dialog.m_maintain;

		pw->GetDocument()->SetModifiedFlag();

		ostringstream os;
		if (dialog.m_maintain) {
			//WizardElement* element = pw->treemap[pw->paramitem];

			pw->WriteElementText(pw->paramitem,os);
		}

		set_model();

		if (dialog.m_maintain) {
			istringstream clipstream(os.str());
			
			int wet;
			clipstream >> wet;
			while (!clipstream.eof()) {
				WizardElement* we = GetWizardElement(pw,(WizardElementType)wet);
				we->readText(clipstream);
				clipstream >> wet;
			}
		}
	}
	update();
	return result;
}

void ParameterWizard2::DeleteChildren(HTREEITEM parent)
{
	HTREEITEM child;
	while ((child = m_Tree->GetChildItem(parent))!=NULL) {
		DeleteChildren(child);
		m_Tree->DeleteItem(child);
		treemap.erase(child);
	}
}


void ModelHeading::set_model()
{
	pw->DeleteChildren(pw->paramitem);

	ModelParameterList mpl;
	mpl.clear();
	const Inheritance *inheritance = BRDF_Model::inheritance.get_named_inheritance((LPCSTR)value,true);
	if (inheritance) {			
		mpl = inheritance->get_parameters();
		
		for (ModelParameterList::iterator q=mpl.begin();q!=mpl.end();++q) {				
			if ((*q)->get_inheritance()) {
				ModelModelVariable *p = 
					new ModelModelVariable(pw,*q,(*q)->name.c_str(),(*q)->defaultvalue.c_str());
				::reg(pw->paramitem,p,true);
				p->BuildSubParameters();
			} else {
				::reg(pw->paramitem,new ModelVariable(pw,*q,(*q)->name.c_str(),(*q)->defaultvalue.c_str()),true);
			}
		}
	}
	pw->m_Tree->UpdateData();
	pw->m_Tree->Select(pw->modelitem,TVGN_FIRSTVISIBLE);
	update();
}
	
int ForLoop::edit()
{
	int result;
	NewForLoopDialog dialog;
	dialog.m_variable=variable;
	dialog.m_from=from;
	dialog.m_to=to;
	dialog.m_by=by;
	dialog.m_Comment = comment;

	if (IDOK==(result=dialog.DoModal())) {
		variable = dialog.m_variable;
		from = dialog.m_from;
		to = dialog.m_to;
		by = dialog.m_by;
		comment = dialog.m_Comment;
		pw->GetDocument()->SetModifiedFlag();
		update();
	}
	return result;
}

int ListLoop::edit()
{
	int result;
	NewListLoopDialog dialog;
	dialog.m_filename=filename;
	dialog.m_Comment = comment;

	if (IDOK==(result=dialog.DoModal())) {
		filename = dialog.m_filename;
		comment = dialog.m_Comment;
		pw->GetDocument()->SetModifiedFlag();
		update();
	}
	return result;
}

int HemiIntegral::edit()
{ 
	int result;
	NewHemisphere dialog;
	dialog.result = *this;
	dialog.m_Comment = comment;
	if (IDOK==(result=dialog.DoModal())) {
		*this = dialog.result;
		comment = dialog.m_Comment;
		pw->GetDocument()->SetModifiedFlag();
		update();
	}
	return result;
}

int AnnulusIntegral::edit()
{ 
	int result;
	NewAnnulus dialog;
	dialog.result = *this;
	dialog.m_comment = comment;
	dialog.m_inpol = inpol;
	dialog.m_pol = pol;
	dialog.m_outeralpha = outeralpha;
	dialog.m_outerpolar = outerpolar;
	dialog.m_outerazimuthal = outerazimuthal;
	dialog.m_inneralpha = inneralpha;
	dialog.m_innerpolar = innerpolar;
	dialog.m_innerazimuthal = innerazimuthal;
	if (IDOK==(result=dialog.DoModal())) {
		*this = dialog.result;
		comment = dialog.m_comment;
		inpol = dialog.m_inpol;
		pol = dialog.m_pol;
		outeralpha = dialog.m_outeralpha;
		outerpolar = dialog.m_outerpolar;
		outerazimuthal = dialog.m_outerazimuthal;
		inneralpha = dialog.m_inneralpha;
		innerpolar = dialog.m_innerpolar;
		innerazimuthal = dialog.m_innerazimuthal;
		pw->GetDocument()->SetModifiedFlag();
		update();
	}
	return result;
}

int CircleIntegral::edit()
{
	int result;
	NewCircleIntegralDialog dialog;
	dialog.result = *this;
	dialog.m_Comment = comment;
	if (IDOK==(result=dialog.DoModal())) {
		*this = dialog.result;
		comment = dialog.m_Comment;
		pw->GetDocument()->SetModifiedFlag();
		update();
	}
	return result;
}

int DifferentialIntegral::edit()
{
	int result;
	NewDifferentialDialog dialog;
	dialog.result = *this;
	dialog.m_Comment = comment;
	if (IDOK==(result=dialog.DoModal())) {
		*this = dialog.result;
		comment = dialog.m_Comment;
		pw->GetDocument()->SetModifiedFlag();
		update();
	}
	return result;
}

int PolygonIntegral::edit()
{
	int result;
	NewPolygonDialog dialog;
	dialog.result = *this;
	dialog.m_Comment = comment;
	if (IDOK==(result=dialog.DoModal())) {
		*this = dialog.result;
		comment = dialog.m_Comment;
		pw->GetDocument()->SetModifiedFlag();
		update();
	}
	return result;
}

int Output::edit()
{
	int result;
	OutputDialog dialog;
	dialog.m_label = name;
	dialog.m_value = value;
	dialog.m_Comment = comment;
	if (IDOK==(result=dialog.DoModal())) {
		name = dialog.m_label;
		value = dialog.m_value;
		comment = dialog.m_Comment;
		pw->GetDocument()->SetModifiedFlag();
		update();
	}
	return result;
}

void UserSectionHeading::add()
{
	UserVariable *p = new UserVariable(pw);
	reg(item,p);
	if (IDCANCEL==p->edit()) p->remove();
}

void ForLoopSectionHeading::add()
{
	NewLoopDialog dialog;
	if (IDOK==dialog.DoModal()) {
		WizardElement *p;
		if (dialog.type=='l') {
			p = new ListLoop(pw);
		} else if (dialog.type=='f') {
			p = new ForLoop(pw);
		} else {
			return;
		}
		
		reg(item,p);
		if (IDCANCEL==p->edit()) p->remove();
		update();
	}
}

void FilesSectionHeading::add()
{
	FilesElement *p = new FilesElement(pw);
	reg(item,p);
	if (IDCANCEL==p->edit()) p->remove();
}

void IntegralSectionHeading::add() 
{
	NewIntegralDialog dialog;
	if (IDOK==dialog.DoModal()) {
		Integral *p;
		if (dialog.type=='c') {
			p = new CircleIntegral(pw);
		} else if (dialog.type=='h') {
			p = new HemiIntegral(pw);
		} else if (dialog.type=='p') {
			p = new PolygonIntegral(pw);
		} else if (dialog.type=='d') {
			p = new DifferentialIntegral(pw);
		} else if (dialog.type=='a') {
			p = new AnnulusIntegral(pw);
		} else {
			return;
		}
		reg(item,p);
		if (IDCANCEL==p->edit()) p->remove();
		update();
	}
};

void OutputsSectionHeading::add() 
{
	Output *p = new Output(pw);
	reg(item,p);
	if (IDCANCEL==p->edit()) p->remove();
	update();
};

BOOL NewUserVariableDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if (m_readonly) {
		//((CEdit*)GetDlgItem(IDC_EDIT1))->ModifyStyle(NULL,WS_DISABLED);
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetReadOnly( TRUE );
	}
	SetWindowText(m_title);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL ModelVariableDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ParameterWizard2::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	UINT pflags;
	dragitem = m_Tree->HitTest(pNMTreeView->ptDrag,&pflags);
	//m_Tree->SelectItem(dragitem);
	
	dragging = TRUE;
	SetCapture();

	*pResult = 0;
}

void ParameterWizard2::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (dragging) {

		HTREEITEM ontop = m_Tree->GetDropHilightItem();

		if (ontop) {
			MoveTreeItem(dragitem,ontop);
		}
		
		dragging=FALSE;
		ReleaseCapture();
		m_Tree->SelectDropTarget(NULL);
	} 
}

void ParameterWizard2::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (dragging) {

		UINT pflags;
		HTREEITEM ontop = m_Tree->HitTest(point,&pflags);

		if (ontop) {
			m_Tree->SelectDropTarget (ontop);
		}
	} 
}

/////////////////////////////////////////////////////////////////////////////
// ParameterWizard2

IMPLEMENT_DYNCREATE(ParameterWizard2, CTreeView)

ParameterWizard2::ParameterWizard2()
{
	outputcount=1;
	dragging=FALSE;
}

ParameterWizard2::~ParameterWizard2()
{
}


BEGIN_MESSAGE_MAP(ParameterWizard2, CTreeView)
	//{{AFX_MSG_MAP(ParameterWizard2)
	ON_COMMAND(ID_ADD, OnAdd)
	ON_COMMAND(ID_EDIT, OnEdit)
	ON_COMMAND(ID_REMOVE, OnRemove)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_ENTER, OnEnter)
	ON_COMMAND(ID_EDIT_COPY, OnCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnPaste)
	ON_COMMAND(ID_EDIT_CUT, OnCut)
	ON_WM_DESTROY()
	ON_COMMAND(IDC_MOVEUP, OnMoveup)
	ON_COMMAND(IDC_MOVEDOWN, OnMovedown)
	ON_COMMAND(ID_DELETE, OnRemove)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ParameterWizard2 drawing

void ParameterWizard2::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// ParameterWizard2 diagnostics

#ifdef _DEBUG
void ParameterWizard2::AssertValid() const
{
	CTreeView::AssertValid();
}

void ParameterWizard2::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ParameterWizard2 message handlers

BOOL ParameterWizard2::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class	
	CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	GetDocument()->SetModifiedFlag(FALSE);
	
	m_Tree = &GetTreeCtrl();

	using namespace std;
	using namespace SCATMECH;

	m_Tree->DeleteAllItems();

	modelitem = reg(NULL,new ModelHeading(this),true);

	useritem = reg(NULL,new UserSectionHeading(this),true);
	requireditem = reg(NULL,new RequiredSectionHeading(this),true);
	paramitem = reg(NULL,new ParameterSectionHeading(this),true);
	forloopitem = reg(NULL,new ForLoopSectionHeading(this),true);
	integralitem = reg(NULL,new IntegralSectionHeading(this),true);
	outputsitem = reg(NULL,new OutputsSectionHeading(this),true);
	filesitem = reg(NULL,new FilesSectionHeading(this),true);

	reg(useritem,new UserVariable(this,"pi","4*atan(1)"),true);
	reg(useritem,new UserVariable(this,"deg","pi/180"),true);
	reg(useritem,new UserVariable(this,"thetai","60"),true);
	reg(useritem,new UserVariable(this,"thetas","0"),true);
	reg(useritem,new UserVariable(this,"inpol","0"),true);

	reg(requireditem,new RequiredVariable(this,"minsamples","10"),true);
	reg(requireditem,new RequiredVariable(this,"differential","(2*deg)^2"),true);
	reg(requireditem,new RequiredVariable(this,"incidentangle","thetai*deg"),true);
	reg(requireditem,new RequiredVariable(this,"incidentpol","(1,cos(2*inpol*deg),sin(2*inpol*deg),0)"),true);
	reg(requireditem,new RequiredVariable(this,"rotation","0*deg"),true);


	//reg(filesitem,new FilesElement(this,"results",""),true);
	//reg(filesitem,new FilesElement(this,"listing",""),true);
	//reg(filesitem,new FilesElement(this,"samples","\"\""),true);
	reg(filesitem,new FilesElement(this,"header","\"\""),true);

	m_Tree->ModifyStyle(NULL,TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT);	
	CFont *font = new CFont;
	font->CreatePointFont(140,"SYSTEM");
	m_Tree->SetFont(font,TRUE);

	return TRUE;
}

void ParameterWizard2::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CTreeView::OnRButtonDown(nFlags, point);
	// TODO: Add your message handler code here and/or call default
	// TODO: Add your message handler code here


	UINT pflags;
	HTREEITEM item = m_Tree->HitTest(point,&pflags);

	m_Tree->SelectItem(item);

	WizardElement *element = treemap[item];
	ClientToScreen(&point);


	CMenu popupMenu;
	//popupMenu.LoadMenu(IDR_WIZARDMENU);

	popupMenu.CreatePopupMenu();

	if (element->can_edit()) {
		popupMenu.AppendMenu(MF_STRING,ID_EDIT,"Edit (Enter)");
	} else {
		popupMenu.AppendMenu(MF_STRING|MF_GRAYED,NULL,"Edit (Enter)");
	}

	if (element->can_add()) {
		popupMenu.AppendMenu(MF_STRING,ID_ADD,"Add (A or Insert)");
	} else {
		popupMenu.AppendMenu(MF_STRING|MF_GRAYED,NULL,"Add (A or Insert)");
	}

	if (element->can_remove()) {
		popupMenu.AppendMenu(MF_STRING,ID_REMOVE,"Remove (Delete)");
	} else {
		popupMenu.AppendMenu(MF_STRING|MF_GRAYED,NULL,"Remove (Delete)");
	}
	
	popupMenu.AppendMenu(MF_SEPARATOR);

	if (element->can_copy()&&element->can_remove()) {
		popupMenu.AppendMenu(MF_STRING,ID_EDIT_CUT,"Cut (Ctrl+X)");
	} else {
		popupMenu.AppendMenu(MF_STRING|MF_GRAYED,NULL,"Cut (Ctrl+X)");
	}

	if (element->can_copy()) {
		popupMenu.AppendMenu(MF_STRING,ID_EDIT_COPY,"Copy (Ctrl+C)");
	} else {
		popupMenu.AppendMenu(MF_STRING|MF_GRAYED,NULL,"Copy (Ctrl+C)");
	}

	if (element->can_paste()) {
		popupMenu.AppendMenu(MF_STRING,ID_EDIT_PASTE,"Paste (Ctrl+V)");
	} else {
		popupMenu.AppendMenu(MF_STRING|MF_GRAYED,NULL,"Paste (Ctrl+V)");
	}
	
	popupMenu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y,AfxGetMainWnd(), NULL);

	popupMenu.DestroyMenu();
	
}

HTREEITEM ParameterWizard2::FindItem(HTREEITEM parent, CString& name)
{
	HTREEITEM child;

	child = m_Tree->GetChildItem(parent);
	while (child != NULL) {
		if (treemap[child]->name==name) {
			return child;
		}
		HTREEITEM subitem = FindItem(child,name);
		if (subitem != NULL) return subitem;
		child = m_Tree->GetNextSiblingItem(child);
	}
	return NULL;
}

void ModelVariable::read(CArchive &ar)
{
	ar >> name >> value >> stringvar >> comment;
		
	item = pw->FindItem(pw->paramitem,name);
	if (item!=NULL) {
		modelparameter = ((ModelVariable*)pw->treemap[item])->modelparameter;
		pw->treemap[item]=this;
		pw->Elements.push_back(counted_ptr<WizardElement>(this));
		update();
	} else {
		string message = "Error reading ModelVariable: name = '";
		message += name;
		message += "' value = '";
		message += value;
		message += "' stringvar = ";
		message += (stringvar ? 'T' : 'F');
		message += " comment = '";
		message += comment;
		message += "'";

		AfxMessageBox(message.c_str());
	}

}

void ModelVariable::readText(istream &ar)
{
	r(ar,name); r(ar,value); ar >> stringvar; r(ar,comment);		
	item = pw->FindItem(pw->paramitem,name);
	if (item!=NULL) {
		modelparameter = ((ModelVariable*)pw->treemap[item])->modelparameter;
		pw->treemap[item]=this;
		pw->Elements.push_back(counted_ptr<WizardElement>(this));
		update();
	} else {
		//
		//string message = "Error reading ModelVariable: name = '";
		//message += name;
		//message += "' value = '";
		//message += value;
		//message += "' stringvar = ";
		//message += (stringvar ? 'T' : 'F');
		//message += " comment = '";
		//message += comment;
		//message += "'";

		//AfxMessageBox(message.c_str());
	}

}

void RequiredVariable::readText(istream& ar) 
{	
	r(ar,name); 
	r(ar,value); 
	r(ar,comment); 
	//reg(pw->requireditem,this,true); 
	item = pw->FindItem(pw->requireditem,name);
	if (item!=NULL) {
		pw->treemap[item]=this;
		pw->Elements.push_back(counted_ptr<WizardElement>(this));
		update();
	} else {
		string message = "Error reading RequiredVariable: name = '";
		message += name;
		message += "' value = '";
		message += value;
		message += "' comment = '";
		message += comment;
		message += "'";

		AfxMessageBox(message.c_str());
	}
}

void ModelModelVariable::read(CArchive &ar)
{
	ar >> name >> value >> comment;
	item = pw->FindItem(pw->paramitem,name);
	if (item!=NULL) {
		modelparameter = ((ModelModelVariable*)pw->treemap[item])->modelparameter;
		pw->treemap[item]=this;
		pw->Elements.push_back(counted_ptr<WizardElement>(this));
		BuildSubParameters();
		update();
	} else {
		string message = "Error reading ModelModelVariable: name = '";
		message += name;
		message += "' value = '";
		message += value;
		message += "' comment = '";
		message += comment;
		message += "'";

		AfxMessageBox(message.c_str());
	}
}

void ModelModelVariable::writeText(ostream &ar) 
{
	writeTextA(ar,WET_MODELMODELVARIABLE); 
	w(ar,name); 
	w(ar,value); 
	w(ar,comment);

	pw->WriteElementText(item,ar);
	//HTREEITEM child;
	//while ((child = pw->m_Tree->GetChildItem(item))!=NULL) {
	//	pw->WriteElementText(child,ar);
	//}
}


void ModelModelVariable::readText(istream &ar)
{
	r(ar,name); r(ar,value); r(ar,comment);
	item = pw->FindItem(pw->paramitem,name);
	if (item!=NULL) {
		modelparameter = ((ModelModelVariable*)pw->treemap[item])->modelparameter;
		pw->treemap[item]=this;
		pw->Elements.push_back(counted_ptr<WizardElement>(this));
		BuildSubParameters();
		update();
	} else {
		/*
		string message = "Error reading ModelModelVariable: name = '";
		message += name;
		message += "' value = '";
		message += value;
		message += "' comment = '";
		message += comment;
		message += "'";

		AfxMessageBox(message.c_str());
		*/
	}
}


void ParameterWizard2::OnDestroy() 
{
	CTreeView::OnDestroy();
	
	// TODO: Add your message handler code here
	CDocument *pDoc = GetDocument();
	POSITION pos = pDoc->GetFirstViewPosition();
	CView* pFirstView = pDoc->GetNextView( pos );

	while (pos != NULL)
		{
			CView* pView = pDoc->GetNextView(pos);
			pView->GetParent()->DestroyWindow();
		}
}

struct PWPrintData
{
	deque<CString> stringlist;
	deque<int> indentlist;
	BOOL done;
};

void add_to_item_list(CDC* pDC, CPrintInfo* pInfo, 
					  CTreeCtrl* tree, HTREEITEM item, int xloc, int xskip, int& yloc, int yskip,int ymax,int& page)
{
	PWPrintData* printdata = (PWPrintData*)(pInfo->m_lpUserData);
	printdata->stringlist.push_back(tree->GetItemText(item));
	printdata->indentlist.push_back(xloc);

	HTREEITEM child = tree->GetNextItem(item,TVGN_CHILD);
	while (child) {
		yloc += yskip;
		if (yloc>ymax-yskip) {
			yloc=0;
			++page;
		}
		add_to_item_list(pDC,pInfo,tree,child,xloc+xskip,xskip,yloc,yskip,ymax,page);
		child = tree->GetNextItem(child,TVGN_NEXT);
	}
}

void ParameterWizard2::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	PWPrintData* printdata = new PWPrintData;
	pInfo->m_lpUserData = (LPVOID)printdata;

	printdata->done = FALSE;
	pInfo->m_bContinuePrinting = TRUE;

	int yloc=0,xloc=0;
	CSize size = pDC->GetTextExtent("XXXXX");
	int yskip = size.cy*1.25;
	int xskip = size.cx;
	CRect rect = pInfo->m_rectDraw;
	int ymax = rect.bottom-rect.top;
	HTREEITEM item = m_Tree->GetRootItem();
	int page=1;
	while (item) {
		add_to_item_list(pDC,pInfo,m_Tree,item,xloc,xskip,yloc,yskip,ymax,page);
		item = m_Tree->GetNextItem(item,TVGN_NEXT);
		yloc+=yskip;
		if (yloc>ymax-yskip) {
			yloc=0;
			++page;
		}
	}
	pInfo->SetMaxPage(page);
	CTreeView::OnBeginPrinting(pDC, pInfo);
}

void ParameterWizard2::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	PWPrintData* printdata = (PWPrintData*)(pInfo->m_lpUserData);

	int yloc=0,xloc=0;
	CSize size = pDC->GetTextExtent("XXXXX");
	int yskip = size.cy*1.25;
	int xskip = size.cx;
	CRect rect = pInfo->m_rectDraw;
	int ymax = rect.bottom-rect.top-size.cy;
	int page=1;
	int nlines = printdata->stringlist.size();
	for (int i=0;i<nlines;++i) {
		if (page == pInfo->m_nCurPage) {
			pDC->TextOut(printdata->indentlist[i],yloc,printdata->stringlist[i]);
			if (i==nlines-1) {
				printdata->done = TRUE;
			}
		}
		yloc += yskip;
		if (yloc>ymax) {
			yloc=0;
			++page;
		}
	}
}

void ParameterWizard2::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	PWPrintData* printdata = (PWPrintData*)(pInfo->m_lpUserData);
	delete printdata;
	
	CTreeView::OnEndPrinting(pDC, pInfo);
}

BOOL ParameterWizard2::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	return CTreeView::DoPreparePrinting(pInfo);
}

void ParameterWizard2::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	PWPrintData* printdata = (PWPrintData*)(pInfo->m_lpUserData);
	if (printdata->done == TRUE) {
		pInfo->m_bContinuePrinting = FALSE;
	} else {
		pInfo->m_bContinuePrinting = TRUE;
	}
	//CTreeView::OnPrepareDC(pDC, pInfo);
}

void ParameterWizard2::OnMoveup() 
{
	// TODO: Add your command handler code here
	HTREEITEM here = m_Tree->GetSelectedItem();
	HTREEITEM there = m_Tree->GetPrevVisibleItem(here);
	MoveTreeItem(here,there);
}

void ParameterWizard2::OnMovedown() 
{
	// TODO: Add your command handler code here
	HTREEITEM here = m_Tree->GetSelectedItem();
	HTREEITEM there = m_Tree->GetNextVisibleItem(here);
	there = m_Tree->GetNextVisibleItem(there);
	MoveTreeItem(here,there);
}
/////////////////////////////////////////////////////////////////////////////
// NewListLoopDialog dialog


NewListLoopDialog::NewListLoopDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NewListLoopDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(NewListLoopDialog)
	m_filename = _T("");
	m_Comment = _T("");
	//}}AFX_DATA_INIT
}


void NewListLoopDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewListLoopDialog)
	DDX_Text(pDX, IDC_EDIT1, m_filename);
	DDX_Text(pDX, IDC_EDIT2, m_Comment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NewListLoopDialog, CDialog)
	//{{AFX_MSG_MAP(NewListLoopDialog)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NewListLoopDialog message handlers
/////////////////////////////////////////////////////////////////////////////
// NewLoopDialog dialog


NewLoopDialog::NewLoopDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NewLoopDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(NewLoopDialog)
	//}}AFX_DATA_INIT
	type='u';
}


void NewLoopDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewLoopDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NewLoopDialog, CDialog)
	//{{AFX_MSG_MAP(NewLoopDialog)
	ON_BN_CLICKED(IDC_FOR, OnFor)
	ON_BN_CLICKED(IDC_LIST, OnList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NewLoopDialog message handlers

void NewLoopDialog::OnFor() 
{
	type='f';
}

void NewLoopDialog::OnList() 
{
	type='l';	
}

void NewListLoopDialog::OnBrowse() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dialog(TRUE,NULL,m_filename);
	if (dialog.DoModal()==IDOK) {
		CEdit *cedit = (CEdit*)(this->GetDlgItem(IDC_EDIT1));
		cedit->SetWindowText(dialog.GetPathName());
	}
}



