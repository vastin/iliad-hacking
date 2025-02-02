/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/docshell/base/nsIContentViewer.idl
 */

#ifndef __gen_nsIContentViewer_h__
#define __gen_nsIContentViewer_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMDocument; /* forward declaration */

class nsISHEntry; /* forward declaration */

class nsIWidget;
class nsIDeviceContext;
struct nsRect;

/* starting interface:    nsIContentViewer */
#define NS_ICONTENTVIEWER_IID_STR "6a7ddb40-8a9e-4576-8ad1-71c5641d8780"

#define NS_ICONTENTVIEWER_IID \
  {0x6a7ddb40, 0x8a9e, 0x4576, \
    { 0x8a, 0xd1, 0x71, 0xc5, 0x64, 0x1d, 0x87, 0x80 }}

class NS_NO_VTABLE nsIContentViewer : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ICONTENTVIEWER_IID)

  /* [noscript] void init (in nsIWidgetPtr aParentWidget, in nsIDeviceContextPtr aDeviceContext, [const] in nsRectRef aBounds); */
  NS_IMETHOD Init(nsIWidget * aParentWidget, nsIDeviceContext * aDeviceContext, const nsRect & aBounds) = 0;

  /* attribute nsISupports container; */
  NS_IMETHOD GetContainer(nsISupports * *aContainer) = 0;
  NS_IMETHOD SetContainer(nsISupports * aContainer) = 0;

  /* void loadStart (in nsISupports aDoc); */
  NS_IMETHOD LoadStart(nsISupports *aDoc) = 0;

  /* void loadComplete (in unsigned long aStatus); */
  NS_IMETHOD LoadComplete(PRUint32 aStatus) = 0;

  /* boolean permitUnload (); */
  NS_IMETHOD PermitUnload(PRBool *_retval) = 0;

  /* void pageHide (in boolean isUnload); */
  NS_IMETHOD PageHide(PRBool isUnload) = 0;

  /**
   * All users of a content viewer are responsible for calling both
   * close() and destroy(), in that order. 
   *
   * close() should be called when the load of a new page for the next
   * content viewer begins, and destroy() should be called when the next
   * content viewer replaces this one.
   *
   * |historyEntry| sets the session history entry for the content viewer.  If
   * this is null, then Destroy() will be called on the document by close().
   * If it is non-null, the document will not be destroyed, and the following
   * actions will happen when destroy() is called (*):
   *  - Sanitize() will be called on the viewer's document
   *  - The content viewer will set the contentViewer property on the
   *    history entry, and release its reference (ownership reversal).
   *  - hide() will be called, and no further destruction will happen.
   *
   *  (*) unless the document is currently being printed, in which case
   *      it will never be saved in session history.
   *
   */
  /* void close (in nsISHEntry historyEntry); */
  NS_IMETHOD Close(nsISHEntry *historyEntry) = 0;

  /* void destroy (); */
  NS_IMETHOD Destroy(void) = 0;

  /* void stop (); */
  NS_IMETHOD Stop(void) = 0;

  /* attribute nsIDOMDocument DOMDocument; */
  NS_IMETHOD GetDOMDocument(nsIDOMDocument * *aDOMDocument) = 0;
  NS_IMETHOD SetDOMDocument(nsIDOMDocument * aDOMDocument) = 0;

  /* [noscript] void getBounds (in nsRectRef aBounds); */
  NS_IMETHOD GetBounds(nsRect & aBounds) = 0;

  /* [noscript] void setBounds ([const] in nsRectRef aBounds); */
  NS_IMETHOD SetBounds(const nsRect & aBounds) = 0;

  /**
   * The previous content viewer, which has been |close|d but not
   * |destroy|ed.
   */
  /* [noscript] attribute nsIContentViewer previousViewer; */
  NS_IMETHOD GetPreviousViewer(nsIContentViewer * *aPreviousViewer) = 0;
  NS_IMETHOD SetPreviousViewer(nsIContentViewer * aPreviousViewer) = 0;

  /* void move (in long aX, in long aY); */
  NS_IMETHOD Move(PRInt32 aX, PRInt32 aY) = 0;

  /* void show (); */
  NS_IMETHOD Show(void) = 0;

  /* void hide (); */
  NS_IMETHOD Hide(void) = 0;

  /* attribute boolean enableRendering; */
  NS_IMETHOD GetEnableRendering(PRBool *aEnableRendering) = 0;
  NS_IMETHOD SetEnableRendering(PRBool aEnableRendering) = 0;

  /* attribute boolean sticky; */
  NS_IMETHOD GetSticky(PRBool *aSticky) = 0;
  NS_IMETHOD SetSticky(PRBool aSticky) = 0;

  /* boolean requestWindowClose (); */
  NS_IMETHOD RequestWindowClose(PRBool *_retval) = 0;

  /**
   * Attach the content viewer to its DOM window and docshell.
   * @param aState A state object that might be useful in attaching the DOM
   *               window.
   */
  /* void open (in nsISupports aState); */
  NS_IMETHOD Open(nsISupports *aState) = 0;

  /**
   * Clears the current history entry.  This is used if we need to clear out
   * the saved presentation state.
   */
  /* void clearHistoryEntry (); */
  NS_IMETHOD ClearHistoryEntry(void) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICONTENTVIEWER \
  NS_IMETHOD Init(nsIWidget * aParentWidget, nsIDeviceContext * aDeviceContext, const nsRect & aBounds); \
  NS_IMETHOD GetContainer(nsISupports * *aContainer); \
  NS_IMETHOD SetContainer(nsISupports * aContainer); \
  NS_IMETHOD LoadStart(nsISupports *aDoc); \
  NS_IMETHOD LoadComplete(PRUint32 aStatus); \
  NS_IMETHOD PermitUnload(PRBool *_retval); \
  NS_IMETHOD PageHide(PRBool isUnload); \
  NS_IMETHOD Close(nsISHEntry *historyEntry); \
  NS_IMETHOD Destroy(void); \
  NS_IMETHOD Stop(void); \
  NS_IMETHOD GetDOMDocument(nsIDOMDocument * *aDOMDocument); \
  NS_IMETHOD SetDOMDocument(nsIDOMDocument * aDOMDocument); \
  NS_IMETHOD GetBounds(nsRect & aBounds); \
  NS_IMETHOD SetBounds(const nsRect & aBounds); \
  NS_IMETHOD GetPreviousViewer(nsIContentViewer * *aPreviousViewer); \
  NS_IMETHOD SetPreviousViewer(nsIContentViewer * aPreviousViewer); \
  NS_IMETHOD Move(PRInt32 aX, PRInt32 aY); \
  NS_IMETHOD Show(void); \
  NS_IMETHOD Hide(void); \
  NS_IMETHOD GetEnableRendering(PRBool *aEnableRendering); \
  NS_IMETHOD SetEnableRendering(PRBool aEnableRendering); \
  NS_IMETHOD GetSticky(PRBool *aSticky); \
  NS_IMETHOD SetSticky(PRBool aSticky); \
  NS_IMETHOD RequestWindowClose(PRBool *_retval); \
  NS_IMETHOD Open(nsISupports *aState); \
  NS_IMETHOD ClearHistoryEntry(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICONTENTVIEWER(_to) \
  NS_IMETHOD Init(nsIWidget * aParentWidget, nsIDeviceContext * aDeviceContext, const nsRect & aBounds) { return _to Init(aParentWidget, aDeviceContext, aBounds); } \
  NS_IMETHOD GetContainer(nsISupports * *aContainer) { return _to GetContainer(aContainer); } \
  NS_IMETHOD SetContainer(nsISupports * aContainer) { return _to SetContainer(aContainer); } \
  NS_IMETHOD LoadStart(nsISupports *aDoc) { return _to LoadStart(aDoc); } \
  NS_IMETHOD LoadComplete(PRUint32 aStatus) { return _to LoadComplete(aStatus); } \
  NS_IMETHOD PermitUnload(PRBool *_retval) { return _to PermitUnload(_retval); } \
  NS_IMETHOD PageHide(PRBool isUnload) { return _to PageHide(isUnload); } \
  NS_IMETHOD Close(nsISHEntry *historyEntry) { return _to Close(historyEntry); } \
  NS_IMETHOD Destroy(void) { return _to Destroy(); } \
  NS_IMETHOD Stop(void) { return _to Stop(); } \
  NS_IMETHOD GetDOMDocument(nsIDOMDocument * *aDOMDocument) { return _to GetDOMDocument(aDOMDocument); } \
  NS_IMETHOD SetDOMDocument(nsIDOMDocument * aDOMDocument) { return _to SetDOMDocument(aDOMDocument); } \
  NS_IMETHOD GetBounds(nsRect & aBounds) { return _to GetBounds(aBounds); } \
  NS_IMETHOD SetBounds(const nsRect & aBounds) { return _to SetBounds(aBounds); } \
  NS_IMETHOD GetPreviousViewer(nsIContentViewer * *aPreviousViewer) { return _to GetPreviousViewer(aPreviousViewer); } \
  NS_IMETHOD SetPreviousViewer(nsIContentViewer * aPreviousViewer) { return _to SetPreviousViewer(aPreviousViewer); } \
  NS_IMETHOD Move(PRInt32 aX, PRInt32 aY) { return _to Move(aX, aY); } \
  NS_IMETHOD Show(void) { return _to Show(); } \
  NS_IMETHOD Hide(void) { return _to Hide(); } \
  NS_IMETHOD GetEnableRendering(PRBool *aEnableRendering) { return _to GetEnableRendering(aEnableRendering); } \
  NS_IMETHOD SetEnableRendering(PRBool aEnableRendering) { return _to SetEnableRendering(aEnableRendering); } \
  NS_IMETHOD GetSticky(PRBool *aSticky) { return _to GetSticky(aSticky); } \
  NS_IMETHOD SetSticky(PRBool aSticky) { return _to SetSticky(aSticky); } \
  NS_IMETHOD RequestWindowClose(PRBool *_retval) { return _to RequestWindowClose(_retval); } \
  NS_IMETHOD Open(nsISupports *aState) { return _to Open(aState); } \
  NS_IMETHOD ClearHistoryEntry(void) { return _to ClearHistoryEntry(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICONTENTVIEWER(_to) \
  NS_IMETHOD Init(nsIWidget * aParentWidget, nsIDeviceContext * aDeviceContext, const nsRect & aBounds) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(aParentWidget, aDeviceContext, aBounds); } \
  NS_IMETHOD GetContainer(nsISupports * *aContainer) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetContainer(aContainer); } \
  NS_IMETHOD SetContainer(nsISupports * aContainer) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetContainer(aContainer); } \
  NS_IMETHOD LoadStart(nsISupports *aDoc) { return !_to ? NS_ERROR_NULL_POINTER : _to->LoadStart(aDoc); } \
  NS_IMETHOD LoadComplete(PRUint32 aStatus) { return !_to ? NS_ERROR_NULL_POINTER : _to->LoadComplete(aStatus); } \
  NS_IMETHOD PermitUnload(PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->PermitUnload(_retval); } \
  NS_IMETHOD PageHide(PRBool isUnload) { return !_to ? NS_ERROR_NULL_POINTER : _to->PageHide(isUnload); } \
  NS_IMETHOD Close(nsISHEntry *historyEntry) { return !_to ? NS_ERROR_NULL_POINTER : _to->Close(historyEntry); } \
  NS_IMETHOD Destroy(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Destroy(); } \
  NS_IMETHOD Stop(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Stop(); } \
  NS_IMETHOD GetDOMDocument(nsIDOMDocument * *aDOMDocument) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDOMDocument(aDOMDocument); } \
  NS_IMETHOD SetDOMDocument(nsIDOMDocument * aDOMDocument) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDOMDocument(aDOMDocument); } \
  NS_IMETHOD GetBounds(nsRect & aBounds) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBounds(aBounds); } \
  NS_IMETHOD SetBounds(const nsRect & aBounds) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetBounds(aBounds); } \
  NS_IMETHOD GetPreviousViewer(nsIContentViewer * *aPreviousViewer) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPreviousViewer(aPreviousViewer); } \
  NS_IMETHOD SetPreviousViewer(nsIContentViewer * aPreviousViewer) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPreviousViewer(aPreviousViewer); } \
  NS_IMETHOD Move(PRInt32 aX, PRInt32 aY) { return !_to ? NS_ERROR_NULL_POINTER : _to->Move(aX, aY); } \
  NS_IMETHOD Show(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Show(); } \
  NS_IMETHOD Hide(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Hide(); } \
  NS_IMETHOD GetEnableRendering(PRBool *aEnableRendering) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetEnableRendering(aEnableRendering); } \
  NS_IMETHOD SetEnableRendering(PRBool aEnableRendering) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetEnableRendering(aEnableRendering); } \
  NS_IMETHOD GetSticky(PRBool *aSticky) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSticky(aSticky); } \
  NS_IMETHOD SetSticky(PRBool aSticky) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSticky(aSticky); } \
  NS_IMETHOD RequestWindowClose(PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->RequestWindowClose(_retval); } \
  NS_IMETHOD Open(nsISupports *aState) { return !_to ? NS_ERROR_NULL_POINTER : _to->Open(aState); } \
  NS_IMETHOD ClearHistoryEntry(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->ClearHistoryEntry(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsContentViewer : public nsIContentViewer
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSICONTENTVIEWER

  nsContentViewer();

private:
  ~nsContentViewer();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsContentViewer, nsIContentViewer)

nsContentViewer::nsContentViewer()
{
  /* member initializers and constructor code */
}

nsContentViewer::~nsContentViewer()
{
  /* destructor code */
}

/* [noscript] void init (in nsIWidgetPtr aParentWidget, in nsIDeviceContextPtr aDeviceContext, [const] in nsRectRef aBounds); */
NS_IMETHODIMP nsContentViewer::Init(nsIWidget * aParentWidget, nsIDeviceContext * aDeviceContext, const nsRect & aBounds)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISupports container; */
NS_IMETHODIMP nsContentViewer::GetContainer(nsISupports * *aContainer)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsContentViewer::SetContainer(nsISupports * aContainer)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void loadStart (in nsISupports aDoc); */
NS_IMETHODIMP nsContentViewer::LoadStart(nsISupports *aDoc)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void loadComplete (in unsigned long aStatus); */
NS_IMETHODIMP nsContentViewer::LoadComplete(PRUint32 aStatus)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean permitUnload (); */
NS_IMETHODIMP nsContentViewer::PermitUnload(PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void pageHide (in boolean isUnload); */
NS_IMETHODIMP nsContentViewer::PageHide(PRBool isUnload)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void close (in nsISHEntry historyEntry); */
NS_IMETHODIMP nsContentViewer::Close(nsISHEntry *historyEntry)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void destroy (); */
NS_IMETHODIMP nsContentViewer::Destroy()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void stop (); */
NS_IMETHODIMP nsContentViewer::Stop()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsIDOMDocument DOMDocument; */
NS_IMETHODIMP nsContentViewer::GetDOMDocument(nsIDOMDocument * *aDOMDocument)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsContentViewer::SetDOMDocument(nsIDOMDocument * aDOMDocument)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void getBounds (in nsRectRef aBounds); */
NS_IMETHODIMP nsContentViewer::GetBounds(nsRect & aBounds)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void setBounds ([const] in nsRectRef aBounds); */
NS_IMETHODIMP nsContentViewer::SetBounds(const nsRect & aBounds)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] attribute nsIContentViewer previousViewer; */
NS_IMETHODIMP nsContentViewer::GetPreviousViewer(nsIContentViewer * *aPreviousViewer)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsContentViewer::SetPreviousViewer(nsIContentViewer * aPreviousViewer)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void move (in long aX, in long aY); */
NS_IMETHODIMP nsContentViewer::Move(PRInt32 aX, PRInt32 aY)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void show (); */
NS_IMETHODIMP nsContentViewer::Show()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void hide (); */
NS_IMETHODIMP nsContentViewer::Hide()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean enableRendering; */
NS_IMETHODIMP nsContentViewer::GetEnableRendering(PRBool *aEnableRendering)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsContentViewer::SetEnableRendering(PRBool aEnableRendering)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean sticky; */
NS_IMETHODIMP nsContentViewer::GetSticky(PRBool *aSticky)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsContentViewer::SetSticky(PRBool aSticky)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean requestWindowClose (); */
NS_IMETHODIMP nsContentViewer::RequestWindowClose(PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void open (in nsISupports aState); */
NS_IMETHODIMP nsContentViewer::Open(nsISupports *aState)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void clearHistoryEntry (); */
NS_IMETHODIMP nsContentViewer::ClearHistoryEntry()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIContentViewer_h__ */
