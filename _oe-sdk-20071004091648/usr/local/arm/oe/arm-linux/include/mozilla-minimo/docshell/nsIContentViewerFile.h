/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/docshell/base/nsIContentViewerFile.idl
 */

#ifndef __gen_nsIContentViewerFile_h__
#define __gen_nsIContentViewerFile_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIPrintSettings_h__
#include "nsIPrintSettings.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMWindowInternal; /* forward declaration */

class nsIWebProgressListener; /* forward declaration */

#include <stdio.h>

/* starting interface:    nsIContentViewerFile */
#define NS_ICONTENTVIEWERFILE_IID_STR "6317f32c-9bc7-11d3-bccc-0060b0fc76bd"

#define NS_ICONTENTVIEWERFILE_IID \
  {0x6317f32c, 0x9bc7, 0x11d3, \
    { 0xbc, 0xcc, 0x00, 0x60, 0xb0, 0xfc, 0x76, 0xbd }}

/**
 * The nsIDocShellFile    
 */
class NS_NO_VTABLE nsIContentViewerFile : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ICONTENTVIEWERFILE_IID)

  /* readonly attribute boolean printable; */
  NS_IMETHOD GetPrintable(PRBool *aPrintable) = 0;

  /* [noscript] void print (in boolean aSilent, in FILE aDebugFile, in nsIPrintSettings aPrintSettings); */
  NS_IMETHOD Print(PRBool aSilent, FILE * aDebugFile, nsIPrintSettings *aPrintSettings) = 0;

  /* [noscript] void printWithParent (in nsIDOMWindowInternal aParentWin, in nsIPrintSettings aThePrintSettings, in nsIWebProgressListener aWPListener); */
  NS_IMETHOD PrintWithParent(nsIDOMWindowInternal *aParentWin, nsIPrintSettings *aThePrintSettings, nsIWebProgressListener *aWPListener) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICONTENTVIEWERFILE \
  NS_IMETHOD GetPrintable(PRBool *aPrintable); \
  NS_IMETHOD Print(PRBool aSilent, FILE * aDebugFile, nsIPrintSettings *aPrintSettings); \
  NS_IMETHOD PrintWithParent(nsIDOMWindowInternal *aParentWin, nsIPrintSettings *aThePrintSettings, nsIWebProgressListener *aWPListener); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICONTENTVIEWERFILE(_to) \
  NS_IMETHOD GetPrintable(PRBool *aPrintable) { return _to GetPrintable(aPrintable); } \
  NS_IMETHOD Print(PRBool aSilent, FILE * aDebugFile, nsIPrintSettings *aPrintSettings) { return _to Print(aSilent, aDebugFile, aPrintSettings); } \
  NS_IMETHOD PrintWithParent(nsIDOMWindowInternal *aParentWin, nsIPrintSettings *aThePrintSettings, nsIWebProgressListener *aWPListener) { return _to PrintWithParent(aParentWin, aThePrintSettings, aWPListener); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICONTENTVIEWERFILE(_to) \
  NS_IMETHOD GetPrintable(PRBool *aPrintable) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPrintable(aPrintable); } \
  NS_IMETHOD Print(PRBool aSilent, FILE * aDebugFile, nsIPrintSettings *aPrintSettings) { return !_to ? NS_ERROR_NULL_POINTER : _to->Print(aSilent, aDebugFile, aPrintSettings); } \
  NS_IMETHOD PrintWithParent(nsIDOMWindowInternal *aParentWin, nsIPrintSettings *aThePrintSettings, nsIWebProgressListener *aWPListener) { return !_to ? NS_ERROR_NULL_POINTER : _to->PrintWithParent(aParentWin, aThePrintSettings, aWPListener); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsContentViewerFile : public nsIContentViewerFile
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSICONTENTVIEWERFILE

  nsContentViewerFile();

private:
  ~nsContentViewerFile();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsContentViewerFile, nsIContentViewerFile)

nsContentViewerFile::nsContentViewerFile()
{
  /* member initializers and constructor code */
}

nsContentViewerFile::~nsContentViewerFile()
{
  /* destructor code */
}

/* readonly attribute boolean printable; */
NS_IMETHODIMP nsContentViewerFile::GetPrintable(PRBool *aPrintable)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void print (in boolean aSilent, in FILE aDebugFile, in nsIPrintSettings aPrintSettings); */
NS_IMETHODIMP nsContentViewerFile::Print(PRBool aSilent, FILE * aDebugFile, nsIPrintSettings *aPrintSettings)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void printWithParent (in nsIDOMWindowInternal aParentWin, in nsIPrintSettings aThePrintSettings, in nsIWebProgressListener aWPListener); */
NS_IMETHODIMP nsContentViewerFile::PrintWithParent(nsIDOMWindowInternal *aParentWin, nsIPrintSettings *aThePrintSettings, nsIWebProgressListener *aWPListener)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIContentViewerFile_h__ */
