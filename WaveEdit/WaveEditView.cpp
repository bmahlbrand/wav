
// WaveEditView.cpp : implementation of the CWaveEditView class
//

#include "stdafx.h"

// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WaveEdit.h"
#endif

#include "WaveEditDoc.h"
#include "WaveEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWaveEditView

IMPLEMENT_DYNCREATE(CWaveEditView, CScrollView)

BEGIN_MESSAGE_MAP(CWaveEditView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CWaveEditView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EDIT_CUT, &CWaveEditView::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CWaveEditView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CWaveEditView::OnEditPaste)
	ON_COMMAND(ID_TOOLS_PLAY, &CWaveEditView::OnToolsPlay)
	ON_COMMAND(ID_TOOLS_STOP, &CWaveEditView::OnToolsStop)
	ON_COMMAND(ID_TOOLS_SPEEDUP, &CWaveEditView::OnToolsSpeedup)
	ON_COMMAND(ID_TOOLS_SLOWDOWN, &CWaveEditView::OnToolsSlowdown)
	ON_COMMAND(ID_TOOLS_ECHO, &CWaveEditView::OnToolsEcho)

	ON_COMMAND(ID_TOOLS_TONE, &CWaveEditView::OnToolsTone)
	ON_COMMAND(ID_TOOLS_ZOOM, &CWaveEditView::OnToolsZoom)
	ON_COMMAND(ID_TOOLS_ZOOMOUT, &CWaveEditView::OnToolsZoomout)
	ON_COMMAND(ID_TOOLS_SAVE, &CWaveEditView::OnToolsSave)
	ON_COMMAND(ID_FILE_SAVE, &CWaveEditView::OnFileSave)
	ON_COMMAND(ID_EDIT_UNDO, &CWaveEditView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CWaveEditView::OnEditRedo)
	ON_COMMAND(ID_TOOLS_REVERSE, &CWaveEditView::OnToolsReverse)
END_MESSAGE_MAP()

// CWaveEditView construction/destruction

CWaveEditView::CWaveEditView()
{

	// TODO: add construction code here
	zoomzoom = false;
	mousePressed = false;
    selectionStart = 0;
    selectionEnd = 0;
	//CWaveEditDoc* pDoc = GetDocument();
	clipboard = new WaveFile();
}


CWaveEditView::~CWaveEditView()
{

}

BOOL CWaveEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}
// CWaveEditView drawing

void CWaveEditView::OnDraw(CDC* pDC)
{
    CWaveEditDoc* pDoc = GetDocument();
	
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    WaveFile * wave = pDoc->wave;
	
    if (wave->hdr==NULL) {
        return;
    }
	// Get dimensions of the window.
    CRect rect;
    GetClientRect(rect);

     // Set color in pen and brush for selection
    COLORREF color = RGB( 255, 200, 200 );
	//COLORREF color2 = RGB(0, 0, 255);

    CPen pen1( PS_SOLID, 0, color );
    pDC->SelectObject( &pen1 );
    CBrush brush1(color);
    pDC->SelectObject( &brush1 );

	
    // Draw selection if any 
    if (selectionStart != selectionEnd) {
		pDC->MoveTo(0, 0);
		pDC->Rectangle(selectionStart, 0, selectionEnd, rect.Height());
	}
	else {
		pDC->MoveTo(0, 0);
		pDC->FillSolidRect(selectionStart, 0, 1, rect.Height(), RGB(0, 0, 0));
	}

		// Set color in pen and brush for wave
		COLORREF color2 = RGB(0, 255, 0);
		CPen pen2(PS_SOLID, 0, color2);
		pDC->SelectObject(&pen2);
		CBrush brush2(color);
		pDC->SelectObject(&brush2);


		int x;

			// Draw the wave selection
			pDC->MoveTo(0, 0);
			for (x = 0; x < rect.Width(); x++) {
				// Assuming the whole file will be fit in the window, for every x value in the window
				// we need to find the equivalent sample in the wave file.
				float val = wave->get_sample((int)(x*wave->lastSample / rect.Width()));

				// We need to fit the sound also in the y axis. The y axis goes from 0 in the
				// top of the window to rect.Height at the bottom. The sound goes from -32768 to 32767
				// we scale it in that way.
				int y = (int)((val + 32768) * (rect.Height() - 1) / (32767 + 32768));
				pDC->LineTo(x, rect.Height() - y);
		
		}
		CSize sizeTotal;
		sizeTotal.cx = x;
		SetScrollSizes(MM_TEXT, sizeTotal);
}

void CWaveEditView::OnInitialUpdate()
{
    CScrollView::OnInitialUpdate();

    // Initial scroll sizes
    CSize sizeTotal;

	CWaveEditDoc *doc = GetDocument();
	WaveFile * wave = doc->wave;
	doc->SetView(this);

    sizeTotal.cx = 10000;
    sizeTotal.cy = 10000;
    SetScrollSizes(MM_TEXT, sizeTotal);
}


void CWaveEditView::GetSelection(int &begin, int &end) const{
	CWaveEditDoc* pDoc = GetDocument();
    
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    WaveFile * wave = pDoc->wave;

    if (wave->hdr==NULL) {
        return;
    }

	CSize size = GetTotalSize();

	begin = wave->lastSample * (double)this->selectionStart/(double)size.cx;
	end = wave->lastSample * (double)this->selectionEnd/(double)size.cx;
}

// CWaveEditView printing
void CWaveEditView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CWaveEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWaveEditView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWaveEditView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CWaveEditView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWaveEditView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWaveEditView diagnostics

#ifdef _DEBUG
void CWaveEditView::AssertValid() const
{
	CView::AssertValid();
}

void CWaveEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWaveEditDoc* CWaveEditView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWaveEditDoc)));
	return (CWaveEditDoc*)m_pDocument;
}
#endif //_DEBUG


// CWaveEditView message handlers


void CWaveEditView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	mousePressed = true;

    selectionStart = point.x;
    selectionEnd = point.x;
	CScrollView::OnLButtonDown(nFlags, point);
}


void CWaveEditView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	mousePressed = false;
    this->selectionEnd = point.x;
    this->RedrawWindow();

	CScrollView::OnLButtonUp(nFlags, point);
}


void CWaveEditView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CScrollView::OnMouseMove(nFlags, point);
	if (mousePressed) {
        selectionEnd = point.x;
        RedrawWindow();
    }
}

void CWaveEditView::OnEditCopy()
{
	CWaveEditDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	WaveFile * wave = pDoc->wave;

	if (wave->hdr == NULL) {
		return;
	}

	// Get dimensions of the window.
	CRect rect;
	GetClientRect(rect);

	CSize size = GetTotalSize();

	double startms = wave->lastSample * (double)this->selectionStart / (double)size.cx;
	double endms = wave->lastSample * (double)this->selectionEnd / (double)size.cx;

	pDoc->Copy(startms, endms);
}
void CWaveEditView::OnEditCut()
{
    CWaveEditDoc* pDoc = GetDocument();
    
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    WaveFile * wave = pDoc->wave;

    if (wave->hdr==NULL) {
        return;
    }

    // Get dimensions of the window.
    CRect rect;
    GetClientRect(rect);
	CSize size = GetTotalSize();

    // Scale the start and end of the selection.
    double startms = (1000.0 * wave->lastSample /wave->sampleRate) * this->selectionStart/rect.Width();

    // Scale the start and end of the selection.
    double endms = (1000.0 * wave->lastSample /wave->sampleRate) * this->selectionEnd/rect.Width();
    
    // Copy first the fragment
    //WaveFile * clipboard = wave->get_fragment(startms, endms);

    // Copy the clipboard
    WaveFile * w2 = wave->remove_fragment(startms, endms);

    // Remove old wave
    //delete wave;

    // Substitute old wave with new one
    pDoc->Cut(startms, endms);

    // Update window
    this->RedrawWindow();
}


void CWaveEditView::OnEditPaste()
{
	CWaveEditDoc* pDoc = GetDocument();
   // WaveFile * w2 = new WaveFile;
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
	if (clipboard == NULL)
		return;
    // Get dimensions of the window.
    CRect rect;
    GetClientRect(rect);
	CSize size = GetTotalSize();

	// Scale the start and end of the selection.
	double startms = pDoc->wave->lastSample * this->selectionStart / size.cx;

	WaveFile * wave = pDoc->wave;
	
	if (wave->hdr == NULL) {
		return;
	}

	pDoc->Paste(startms);

	this->RedrawWindow();
}


void CWaveEditView::OnToolsPlay()
{
	CWaveEditDoc *doc = GetDocument();
	int start, end;
	GetSelection(start, end);
	doc->Play(start, end);
}

void CWaveEditView::OnToolsStop()
{
	CWaveEditDoc *doc = GetDocument();
	doc->Stop();
}

void CWaveEditView::OnToolsSpeedup()
{
	CWaveEditDoc *doc = GetDocument();
	int start, end;
	GetSelection(start, end);
	doc->Speedup(start, end);
	this->RedrawWindow();
}


void CWaveEditView::OnToolsSlowdown()
{
	CWaveEditDoc *doc = GetDocument();
	int start, end;
	GetSelection(start, end);
	doc->Slowdown(start, end);
	this->RedrawWindow();
}


void CWaveEditView::OnToolsEcho()
{
	CWaveEditDoc *doc = GetDocument();
	int start, end;
	GetSelection(start, end);
	doc->Echo(start, end);
	this->RedrawWindow();
}
	
void CWaveEditView::OnToolsTone()
{
	CWaveEditDoc *doc = GetDocument();
	int start, end;
	GetSelection(start, end);
	doc->Tone(start, end);
	this->RedrawWindow();
	//doc->Play();
	// TODO: Add your command handler code here
}


void CWaveEditView::OnToolsZoom()
{
	CWaveEditDoc *doc = GetDocument();
	zoomzoom = true;
	int start, end;
	GetSelection(start, end);
	doc->Copy(start, end);
	this->RedrawWindow();
	// TODO: Add your command handler code here
}


void CWaveEditView::OnToolsZoomout()
{
	if (zoomzoom == false)
		return;
	zoomzoom = false;
	this->RedrawWindow();
}


void CWaveEditView::OnToolsSave()
{
	CWaveEditDoc *doc = GetDocument();
	doc->Save();
}



void CWaveEditView::OnFileSave()
{
	CWaveEditDoc *doc = GetDocument();
	doc->Save();
}


void CWaveEditView::OnEditUndo()
{
	CWaveEditDoc *doc = GetDocument();
	doc->Undo();
	this->RedrawWindow();
}


void CWaveEditView::OnEditRedo()
{
	CWaveEditDoc *doc = GetDocument();
	doc->Redo();
	this->RedrawWindow();
}


void CWaveEditView::OnToolsReverse()
{
	CWaveEditDoc *doc = GetDocument();
	int start, end;
	GetSelection(start, end);
	doc->Reverse(start, end);
	this->RedrawWindow();
}