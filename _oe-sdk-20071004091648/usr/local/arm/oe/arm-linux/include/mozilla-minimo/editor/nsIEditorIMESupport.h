/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/editor/idl/nsIEditorIMESupport.idl
 */

#ifndef __gen_nsIEditorIMESupport_h__
#define __gen_nsIEditorIMESupport_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_domstubs_h__
#include "domstubs.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIPrivateTextRangeList;
struct nsTextEventReply;
struct nsReconversionEventReply;
struct nsQueryCaretRectEventReply;

/* starting interface:    nsIEditorIMESupport */
#define NS_IEDITORIMESUPPORT_IID_STR "205b3e49-aa58-499e-880b-aacab9dede01"

#define NS_IEDITORIMESUPPORT_IID \
  {0x205b3e49, 0xaa58, 0x499e, \
    { 0x88, 0x0b, 0xaa, 0xca, 0xb9, 0xde, 0xde, 0x01 }}

class NS_NO_VTABLE nsIEditorIMESupport : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IEDITORIMESUPPORT_IID)

  /**
   * beginComposition(nsTextEventReply* aReply) Handles the start of inline input composition.
   */
  /* [noscript] void beginComposition (in nsTextEventReplyPtr aReply); */
  NS_IMETHOD BeginComposition(nsTextEventReply * aReply) = 0;

  /**
   * setCompositionString() Sets the inline input composition string.
   * beginComposition must be called prior to this.
   */
  /* [noscript] void setCompositionString (in DOMString aCompositionString, in nsIPrivateTextRangeListPtr aTextRange, in nsTextEventReplyPtr aReply); */
  NS_IMETHOD SetCompositionString(const nsAString & aCompositionString, nsIPrivateTextRangeList * aTextRange, nsTextEventReply * aReply) = 0;

  /**
   * endComposition() Handles the end of inline input composition.
   */
  /* void endComposition (); */
  NS_IMETHOD EndComposition(void) = 0;

  /**
   * queryComposition()  Get the composition position
   */
  /* [noscript] void queryComposition (in nsTextEventReplyPtr aReply); */
  NS_IMETHOD QueryComposition(nsTextEventReply * aReply) = 0;

  /**
   * forceCompositionEnd() force the composition end
   */
  /* void forceCompositionEnd (); */
  NS_IMETHOD ForceCompositionEnd(void) = 0;

  /**
   * getReconversionString()  Get the reconvertion string
   */
  /* [noscript] void getReconversionString (in nsReconversionEventReplyPtr aReply); */
  NS_IMETHOD GetReconversionString(nsReconversionEventReply * aReply) = 0;

  /**
   * Notify for IME when the editor got focus.
   */
  /* void notifyIMEOnFocus (); */
  NS_IMETHOD NotifyIMEOnFocus(void) = 0;

  /**
   * Notify for IME when the editor lost focus.
   */
  /* void notifyIMEOnBlur (); */
  NS_IMETHOD NotifyIMEOnBlur(void) = 0;

  /**
   * getQueryCaretRect()  Get the query caret rect
   */
  /* [noscript] void getQueryCaretRect (in nsQueryCaretRectEventReplyPtr aReply); */
  NS_IMETHOD GetQueryCaretRect(nsQueryCaretRectEventReply * aReply) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIEDITORIMESUPPORT \
  NS_IMETHOD BeginComposition(nsTextEventReply * aReply); \
  NS_IMETHOD SetCompositionString(const nsAString & aCompositionString, nsIPrivateTextRangeList * aTextRange, nsTextEventReply * aReply); \
  NS_IMETHOD EndComposition(void); \
  NS_IMETHOD QueryComposition(nsTextEventReply * aReply); \
  NS_IMETHOD ForceCompositionEnd(void); \
  NS_IMETHOD GetReconversionString(nsReconversionEventReply * aReply); \
  NS_IMETHOD NotifyIMEOnFocus(void); \
  NS_IMETHOD NotifyIMEOnBlur(void); \
  NS_IMETHOD GetQueryCaretRect(nsQueryCaretRectEventReply * aReply); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIEDITORIMESUPPORT(_to) \
  NS_IMETHOD BeginComposition(nsTextEventReply * aReply) { return _to BeginComposition(aReply); } \
  NS_IMETHOD SetCompositionString(const nsAString & aCompositionString, nsIPrivateTextRangeList * aTextRange, nsTextEventReply * aReply) { return _to SetCompositionString(aCompositionString, aTextRange, aReply); } \
  NS_IMETHOD EndComposition(void) { return _to EndComposition(); } \
  NS_IMETHOD QueryComposition(nsTextEventReply * aReply) { return _to QueryComposition(aReply); } \
  NS_IMETHOD ForceCompositionEnd(void) { return _to ForceCompositionEnd(); } \
  NS_IMETHOD GetReconversionString(nsReconversionEventReply * aReply) { return _to GetReconversionString(aReply); } \
  NS_IMETHOD NotifyIMEOnFocus(void) { return _to NotifyIMEOnFocus(); } \
  NS_IMETHOD NotifyIMEOnBlur(void) { return _to NotifyIMEOnBlur(); } \
  NS_IMETHOD GetQueryCaretRect(nsQueryCaretRectEventReply * aReply) { return _to GetQueryCaretRect(aReply); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIEDITORIMESUPPORT(_to) \
  NS_IMETHOD BeginComposition(nsTextEventReply * aReply) { return !_to ? NS_ERROR_NULL_POINTER : _to->BeginComposition(aReply); } \
  NS_IMETHOD SetCompositionString(const nsAString & aCompositionString, nsIPrivateTextRangeList * aTextRange, nsTextEventReply * aReply) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCompositionString(aCompositionString, aTextRange, aReply); } \
  NS_IMETHOD EndComposition(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->EndComposition(); } \
  NS_IMETHOD QueryComposition(nsTextEventReply * aReply) { return !_to ? NS_ERROR_NULL_POINTER : _to->QueryComposition(aReply); } \
  NS_IMETHOD ForceCompositionEnd(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->ForceCompositionEnd(); } \
  NS_IMETHOD GetReconversionString(nsReconversionEventReply * aReply) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetReconversionString(aReply); } \
  NS_IMETHOD NotifyIMEOnFocus(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->NotifyIMEOnFocus(); } \
  NS_IMETHOD NotifyIMEOnBlur(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->NotifyIMEOnBlur(); } \
  NS_IMETHOD GetQueryCaretRect(nsQueryCaretRectEventReply * aReply) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetQueryCaretRect(aReply); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsEditorIMESupport : public nsIEditorIMESupport
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIEDITORIMESUPPORT

  nsEditorIMESupport();

private:
  ~nsEditorIMESupport();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsEditorIMESupport, nsIEditorIMESupport)

nsEditorIMESupport::nsEditorIMESupport()
{
  /* member initializers and constructor code */
}

nsEditorIMESupport::~nsEditorIMESupport()
{
  /* destructor code */
}

/* [noscript] void beginComposition (in nsTextEventReplyPtr aReply); */
NS_IMETHODIMP nsEditorIMESupport::BeginComposition(nsTextEventReply * aReply)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void setCompositionString (in DOMString aCompositionString, in nsIPrivateTextRangeListPtr aTextRange, in nsTextEventReplyPtr aReply); */
NS_IMETHODIMP nsEditorIMESupport::SetCompositionString(const nsAString & aCompositionString, nsIPrivateTextRangeList * aTextRange, nsTextEventReply * aReply)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void endComposition (); */
NS_IMETHODIMP nsEditorIMESupport::EndComposition()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void queryComposition (in nsTextEventReplyPtr aReply); */
NS_IMETHODIMP nsEditorIMESupport::QueryComposition(nsTextEventReply * aReply)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void forceCompositionEnd (); */
NS_IMETHODIMP nsEditorIMESupport::ForceCompositionEnd()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void getReconversionString (in nsReconversionEventReplyPtr aReply); */
NS_IMETHODIMP nsEditorIMESupport::GetReconversionString(nsReconversionEventReply * aReply)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void notifyIMEOnFocus (); */
NS_IMETHODIMP nsEditorIMESupport::NotifyIMEOnFocus()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void notifyIMEOnBlur (); */
NS_IMETHODIMP nsEditorIMESupport::NotifyIMEOnBlur()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void getQueryCaretRect (in nsQueryCaretRectEventReplyPtr aReply); */
NS_IMETHODIMP nsEditorIMESupport::GetQueryCaretRect(nsQueryCaretRectEventReply * aReply)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIEditorIMESupport_h__ */
