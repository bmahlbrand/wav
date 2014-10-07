
// WaveEditDoc.h : interface of the CWaveEditDoc class
#include <stack>
#pragma once



class CWaveEditDoc : public CDocument
{
	friend class CWaveEditView;
    WaveFile *wave;
	CWaveEditView *view;
	WaveHeader *hdr;


	std::stack<WaveFile *> undoStack;
	std::stack<WaveFile *> redoStack;


private:
	afx_msg void DeleteStack(std::stack<WaveFile *> &stack);
protected: // create from serialization only
	CWaveEditDoc();
	DECLARE_DYNCREATE(CWaveEditDoc)

// Attributes
public:
	afx_msg void SetView(CWaveEditView *view);
	afx_msg void Play(int start, int end);
	afx_msg void Speedup(int start, int end);
	afx_msg void Slowdown(int start, int end);
	afx_msg void Echo(int start, int end);
	afx_msg void Customspeed();
	afx_msg void Cut(int start, int end);
	afx_msg void Copy(int start, int end);
	afx_msg void Paste(int index);
	afx_msg void Stop();	
	afx_msg void Save();
	afx_msg void Tone(int start, int end);
	afx_msg void Undo();
	afx_msg void Redo();
	afx_msg void Reverse(int start, int ending);

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CWaveEditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
		
};
