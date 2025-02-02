/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/editor/idl/nsIEditorLogging.idl
 */

#ifndef __gen_nsIEditorLogging_h__
#define __gen_nsIEditorLogging_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIFile; /* forward declaration */


/* starting interface:    nsIEditorLogging */
#define NS_IEDITORLOGGING_IID_STR "4805e681-49b9-11d3-9ce4-ed60bd6cb5bc"

#define NS_IEDITORLOGGING_IID \
  {0x4805e681, 0x49b9, 0x11d3, \
    { 0x9c, 0xe4, 0xed, 0x60, 0xbd, 0x6c, 0xb5, 0xbc }}

class NS_NO_VTABLE nsIEditorLogging : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IEDITORLOGGING_IID)

  /** Start logging.
   * @param aLogFile     The file to which the log should be written.
    */
  /* void startLogging (in nsIFile aLogFile); */
  NS_IMETHOD StartLogging(nsIFile *aLogFile) = 0;

  /** Stop logging.
    */
  /* void stopLogging (); */
  NS_IMETHOD StopLogging(void) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIEDITORLOGGING \
  NS_IMETHOD StartLogging(nsIFile *aLogFile); \
  NS_IMETHOD StopLogging(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIEDITORLOGGING(_to) \
  NS_IMETHOD StartLogging(nsIFile *aLogFile) { return _to StartLogging(aLogFile); } \
  NS_IMETHOD StopLogging(void) { return _to StopLogging(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIEDITORLOGGING(_to) \
  NS_IMETHOD StartLogging(nsIFile *aLogFile) { return !_to ? NS_ERROR_NULL_POINTER : _to->StartLogging(aLogFile); } \
  NS_IMETHOD StopLogging(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->StopLogging(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsEditorLogging : public nsIEditorLogging
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIEDITORLOGGING

  nsEditorLogging();

private:
  ~nsEditorLogging();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsEditorLogging, nsIEditorLogging)

nsEditorLogging::nsEditorLogging()
{
  /* member initializers and constructor code */
}

nsEditorLogging::~nsEditorLogging()
{
  /* destructor code */
}

/* void startLogging (in nsIFile aLogFile); */
NS_IMETHODIMP nsEditorLogging::StartLogging(nsIFile *aLogFile)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void stopLogging (); */
NS_IMETHODIMP nsEditorLogging::StopLogging()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIEditorLogging_h__ */
