
// WaveEditDoc.cpp : implementation of the CWaveEditDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WaveEdit.h"
#endif
#include "WaveEditDoc.h"
#include "WaveEditView.h"
#include <propkey.h>
#include "fstream"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CWaveEditDoc

IMPLEMENT_DYNCREATE(CWaveEditDoc, CDocument)

BEGIN_MESSAGE_MAP(CWaveEditDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CWaveEditDoc::OnFileSave)
END_MESSAGE_MAP()


// CWaveEditDoc construction/destruction

CWaveEditDoc::CWaveEditDoc()
{
	// TODO: add one-time construction code here
	wave = new WaveFile();
	std::stack<WaveFile *> undoStack;
	std::stack<WaveFile *> redoStack;
} 

void CWaveEditDoc::DeleteStack(std::stack<WaveFile *> &stack){
	while(!stack.empty()){
		delete stack.top();
		stack.pop();
	}
}
CWaveEditDoc::~CWaveEditDoc()
{
	DeleteStack(undoStack);
	DeleteStack(redoStack);
	
	delete wave;
}

BOOL CWaveEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CWaveEditDoc serialization

void CWaveEditDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		wave->read(ar.GetFile());
		//wave.play();
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CWaveEditDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CWaveEditDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWaveEditDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWaveEditDoc diagnostics

#ifdef _DEBUG
void CWaveEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWaveEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWaveEditDoc commands

void CWaveEditDoc::SetView(CWaveEditView *view){
	this->view = view;
}

void CWaveEditDoc::Play(int start, int end)
{
	// TODO: Add your command handler code here

	wave->play(start, end);
}

void CWaveEditDoc::Stop()
{
	// TODO: Add your command handler code here
	wave->stop();
}
void CWaveEditDoc::Save()
{
	//HANDLE hFile = CreateFile(_T("CFile_File.dat"),
	//GENERIC_WRITE, FILE_SHARE_READ,
	//NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//HANDLE *mFile = NULL;
	//		mFile = &hFile;
		
		//CFileDialog fileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, "Wave Files (*.wav)|*.wav||", this);
	//CWaveEditDoc *doc = GetDocument();
	//CFile *mahWave();
	//int fsize = WaveHeader->GetLength();
	WaveFile *w2 = new WaveFile();
	w2= wave;

	CFile theFile((LPCWSTR)hdr, CFile::modeWrite);
	
	w2->save(&theFile);
}


void CWaveEditDoc::Speedup(int start, int end)
{
    WaveFile * w2 = wave->multiply_freq(2, 0, start, end); 
	
	undoStack.push(wave);
	DeleteStack(redoStack);
	// TODO: Add your command handler code here
	wave = w2;
}


void CWaveEditDoc::Slowdown(int start, int end)
{
	WaveFile * w2 = wave->multiply_freq(.5, 0, start, end); 

	undoStack.push(wave);
	DeleteStack(redoStack);
	wave = w2;

}


void CWaveEditDoc::Echo(int start, int end)
{
	WaveFile * w2 = wave->echo(0.5, 1000,start, end);
	undoStack.push(wave);
	DeleteStack(redoStack);
	wave = w2;
}

void CWaveEditDoc::Customspeed() 
{
	// TODO: Add your command handler code here

//	WaveFile * w2 = wave.multiply_freq(k, startms); //, endms
}

void CWaveEditDoc::Copy(int start, int end)
{

	if(clipboard)
		delete clipboard;

	undoStack.push(wave);
	redoStack.push(wave);

	clipboard = wave->get_fragment(start, end);
}

void CWaveEditDoc::Cut(int start, int end)
{
	Copy(start, end);
	

    WaveFile * wave2 = wave->remove_fragment(start, end);

	undoStack.push(wave);
	DeleteStack(redoStack);

    wave = wave2;
}

void CWaveEditDoc::Paste(int index)
{
	WaveFile * wave2 = wave->insert_fragment(clipboard, index);

	undoStack.push(wave);
	DeleteStack(redoStack);
	wave = wave2;
}

void CWaveEditDoc::Tone(int start, int end) {

	WaveFile * w2 = wave->tone(550, 10000, start, end);
}


void CWaveEditDoc::Undo()
{
	if(undoStack.empty()) 
		return;

	redoStack.push(wave);
	wave = undoStack.top();
	undoStack.pop();
}

void CWaveEditDoc::Redo()
{
	if(redoStack.empty()){
		return;
	}
	
	undoStack.push(wave);
	wave = redoStack.top();
	redoStack.pop();
}

void CWaveEditDoc::Reverse(int start, int ending)
{
	undoStack.push(wave);
	redoStack.push(wave);

	WaveFile * w2 = wave->reversal(start, ending);
	
	wave = w2;
}