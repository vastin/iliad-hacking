/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/dom/public/idl/base/nsIBrowserDOMWindow.idl
 */

#ifndef __gen_nsIBrowserDOMWindow_h__
#define __gen_nsIBrowserDOMWindow_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMWindow; /* forward declaration */

class nsIURI; /* forward declaration */


/* starting interface:    nsIBrowserDOMWindow */
#define NS_IBROWSERDOMWINDOW_IID_STR "af25c296-aaec-4f7f-8885-dd37a1cc0a13"

#define NS_IBROWSERDOMWINDOW_IID \
  {0xaf25c296, 0xaaec, 0x4f7f, \
    { 0x88, 0x85, 0xdd, 0x37, 0xa1, 0xcc, 0x0a, 0x13 }}

/**
 * The C++ source has access to the browser script source through
 * nsIBrowserWindow. It is intended to be attached to the chrome
 * DOMWindow of a browser window. A DOMWindow that does not happen to
 * be a browser chrome window will simply have no access to any such
 * interface.
 */
class NS_NO_VTABLE nsIBrowserDOMWindow : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IBROWSERDOMWINDOW_IID)

  /**
   * values for openURI's aWhere parameter
   */
  enum { OPEN_DEFAULTWINDOW = 0 };

  enum { OPEN_CURRENTWINDOW = 1 };

  enum { OPEN_NEWWINDOW = 2 };

  enum { OPEN_NEWTAB = 3 };

  /**
   * values for openURI's aContext parameter
   */
  enum { OPEN_EXTERNAL = 1 };

  enum { OPEN_NEW = 2 };

  /**
   * Load a URI
   * @param aURI the URI to open. null is allowed; it means about:blank.
   * @param aWhere see possible values described above.
   * @param aOpener window requesting the open (can be null).
   * @param aContext the context in which the URI is being opened. This
   *                 is used only when aWhere == OPEN_DEFAULTWINDOW.
   * @return the window into which the URI was opened.
  */
  /* nsIDOMWindow openURI (in nsIURI aURI, in nsIDOMWindow aOpener, in short aWhere, in short aContext); */
  NS_IMETHOD OpenURI(nsIURI *aURI, nsIDOMWindow *aOpener, PRInt16 aWhere, PRInt16 aContext, nsIDOMWindow **_retval) = 0;

  /**
   * @param  aWindow the window to test.
   * @return whether the window is the main content window for any
   *         currently open tab.
   */
  /* boolean isTabContentWindow (in nsIDOMWindow aWindow); */
  NS_IMETHOD IsTabContentWindow(nsIDOMWindow *aWindow, PRBool *_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIBROWSERDOMWINDOW \
  NS_IMETHOD OpenURI(nsIURI *aURI, nsIDOMWindow *aOpener, PRInt16 aWhere, PRInt16 aContext, nsIDOMWindow **_retval); \
  NS_IMETHOD IsTabContentWindow(nsIDOMWindow *aWindow, PRBool *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIBROWSERDOMWINDOW(_to) \
  NS_IMETHOD OpenURI(nsIURI *aURI, nsIDOMWindow *aOpener, PRInt16 aWhere, PRInt16 aContext, nsIDOMWindow **_retval) { return _to OpenURI(aURI, aOpener, aWhere, aContext, _retval); } \
  NS_IMETHOD IsTabContentWindow(nsIDOMWindow *aWindow, PRBool *_retval) { return _to IsTabContentWindow(aWindow, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIBROWSERDOMWINDOW(_to) \
  NS_IMETHOD OpenURI(nsIURI *aURI, nsIDOMWindow *aOpener, PRInt16 aWhere, PRInt16 aContext, nsIDOMWindow **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->OpenURI(aURI, aOpener, aWhere, aContext, _retval); } \
  NS_IMETHOD IsTabContentWindow(nsIDOMWindow *aWindow, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->IsTabContentWindow(aWindow, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsBrowserDOMWindow : public nsIBrowserDOMWindow
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIBROWSERDOMWINDOW

  nsBrowserDOMWindow();

private:
  ~nsBrowserDOMWindow();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsBrowserDOMWindow, nsIBrowserDOMWindow)

nsBrowserDOMWindow::nsBrowserDOMWindow()
{
  /* member initializers and constructor code */
}

nsBrowserDOMWindow::~nsBrowserDOMWindow()
{
  /* destructor code */
}

/* nsIDOMWindow openURI (in nsIURI aURI, in nsIDOMWindow aOpener, in short aWhere, in short aContext); */
NS_IMETHODIMP nsBrowserDOMWindow::OpenURI(nsIURI *aURI, nsIDOMWindow *aOpener, PRInt16 aWhere, PRInt16 aContext, nsIDOMWindow **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean isTabContentWindow (in nsIDOMWindow aWindow); */
NS_IMETHODIMP nsBrowserDOMWindow::IsTabContentWindow(nsIDOMWindow *aWindow, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIBrowserDOMWindow_h__ */
