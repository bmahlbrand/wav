
// WaveEditView.h : interface of the CWaveEditView class
#include "clipboard.h"

#pragma once
class CWaveEditView : public CScrollView
{
	bool zoomzoom;
	bool mousePressed;
    int selectionStart; // Selected sample start
    int selectionEnd; // Selected sample end
	double scale;
	WaveFile * clipboard;

protected: // create from serialization only
	CWaveEditView();
	DECLARE_DYNCREATE(CWaveEditView)

// Attributes
public:
	CWaveEditDoc *GetDocument() const;
	void GetSelection(int &begin, int &end) const;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CWaveEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void SetView(CWaveEditView *view);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditCut();

	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnToolsPlay();
	afx_msg void OnToolsStop();
	afx_msg void OnToolsSpeedup();
	afx_msg void OnToolsSlowdown();
	afx_msg void OnToolsEcho();

	afx_msg void OnToolsTone();
	afx_msg void OnToolsZoom();
	afx_msg void OnToolsZoomout();
	afx_msg void OnToolsSave();
	//virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	afx_msg void OnFileSave();

	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnToolsReverse();
	afx_msg void OnToolsZoomout32786();
};

#ifndef _DEBUG  // debug version in WaveEditView.cpp
inline CWaveEditDoc* CWaveEditView::GetDocument() const
   { return reinterpret_cast<CWaveEditDoc*>(m_pDocument); }
#endif

