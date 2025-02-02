/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/dom/public/idl/ls/nsIDOMLSSerializer.idl
 */

#ifndef __gen_nsIDOMLSSerializer_h__
#define __gen_nsIDOMLSSerializer_h__


#ifndef __gen_domstubs_h__
#include "domstubs.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMDOMConfiguration; /* forward declaration */

class nsIDOMLSOutput; /* forward declaration */

class nsIDOMLSSerializerFilter; /* forward declaration */

class LSException; /* forward declaration */


/* starting interface:    nsIDOMLSSerializer */
#define NS_IDOMLSSERIALIZER_IID_STR "96ea4792-d362-4c28-a8c2-0337790d648a"

#define NS_IDOMLSSERIALIZER_IID \
  {0x96ea4792, 0xd362, 0x4c28, \
    { 0xa8, 0xc2, 0x03, 0x37, 0x79, 0x0d, 0x64, 0x8a }}

class NS_NO_VTABLE nsIDOMLSSerializer : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IDOMLSSERIALIZER_IID)

  /* readonly attribute nsIDOMDOMConfiguration domConfig; */
  NS_IMETHOD GetDomConfig(nsIDOMDOMConfiguration * *aDomConfig) = 0;

  /* attribute DOMString newLine; */
  NS_IMETHOD GetNewLine(nsAString & aNewLine) = 0;
  NS_IMETHOD SetNewLine(const nsAString & aNewLine) = 0;

  /* attribute nsIDOMLSSerializerFilter filter; */
  NS_IMETHOD GetFilter(nsIDOMLSSerializerFilter * *aFilter) = 0;
  NS_IMETHOD SetFilter(nsIDOMLSSerializerFilter * aFilter) = 0;

  /* boolean write (in nsIDOMNode nodeArg, in nsIDOMLSOutput destination)  raises (LSException); */
  NS_IMETHOD Write(nsIDOMNode *nodeArg, nsIDOMLSOutput *destination, PRBool *_retval) = 0;

  /* boolean writeToURI (in nsIDOMNode nodeArg, in DOMString uri)  raises (LSException); */
  NS_IMETHOD WriteToURI(nsIDOMNode *nodeArg, const nsAString & uri, PRBool *_retval) = 0;

  /* DOMString writeToString (in nsIDOMNode nodeArg)  raises (DOMException, LSException); */
  NS_IMETHOD WriteToString(nsIDOMNode *nodeArg, nsAString & _retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOMLSSERIALIZER \
  NS_IMETHOD GetDomConfig(nsIDOMDOMConfiguration * *aDomConfig); \
  NS_IMETHOD GetNewLine(nsAString & aNewLine); \
  NS_IMETHOD SetNewLine(const nsAString & aNewLine); \
  NS_IMETHOD GetFilter(nsIDOMLSSerializerFilter * *aFilter); \
  NS_IMETHOD SetFilter(nsIDOMLSSerializerFilter * aFilter); \
  NS_IMETHOD Write(nsIDOMNode *nodeArg, nsIDOMLSOutput *destination, PRBool *_retval); \
  NS_IMETHOD WriteToURI(nsIDOMNode *nodeArg, const nsAString & uri, PRBool *_retval); \
  NS_IMETHOD WriteToString(nsIDOMNode *nodeArg, nsAString & _retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOMLSSERIALIZER(_to) \
  NS_IMETHOD GetDomConfig(nsIDOMDOMConfiguration * *aDomConfig) { return _to GetDomConfig(aDomConfig); } \
  NS_IMETHOD GetNewLine(nsAString & aNewLine) { return _to GetNewLine(aNewLine); } \
  NS_IMETHOD SetNewLine(const nsAString & aNewLine) { return _to SetNewLine(aNewLine); } \
  NS_IMETHOD GetFilter(nsIDOMLSSerializerFilter * *aFilter) { return _to GetFilter(aFilter); } \
  NS_IMETHOD SetFilter(nsIDOMLSSerializerFilter * aFilter) { return _to SetFilter(aFilter); } \
  NS_IMETHOD Write(nsIDOMNode *nodeArg, nsIDOMLSOutput *destination, PRBool *_retval) { return _to Write(nodeArg, destination, _retval); } \
  NS_IMETHOD WriteToURI(nsIDOMNode *nodeArg, const nsAString & uri, PRBool *_retval) { return _to WriteToURI(nodeArg, uri, _retval); } \
  NS_IMETHOD WriteToString(nsIDOMNode *nodeArg, nsAString & _retval) { return _to WriteToString(nodeArg, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOMLSSERIALIZER(_to) \
  NS_IMETHOD GetDomConfig(nsIDOMDOMConfiguration * *aDomConfig) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDomConfig(aDomConfig); } \
  NS_IMETHOD GetNewLine(nsAString & aNewLine) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetNewLine(aNewLine); } \
  NS_IMETHOD SetNewLine(const nsAString & aNewLine) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetNewLine(aNewLine); } \
  NS_IMETHOD GetFilter(nsIDOMLSSerializerFilter * *aFilter) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFilter(aFilter); } \
  NS_IMETHOD SetFilter(nsIDOMLSSerializerFilter * aFilter) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetFilter(aFilter); } \
  NS_IMETHOD Write(nsIDOMNode *nodeArg, nsIDOMLSOutput *destination, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Write(nodeArg, destination, _retval); } \
  NS_IMETHOD WriteToURI(nsIDOMNode *nodeArg, const nsAString & uri, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->WriteToURI(nodeArg, uri, _retval); } \
  NS_IMETHOD WriteToString(nsIDOMNode *nodeArg, nsAString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->WriteToString(nodeArg, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDOMLSSerializer : public nsIDOMLSSerializer
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMLSSERIALIZER

  nsDOMLSSerializer();

private:
  ~nsDOMLSSerializer();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDOMLSSerializer, nsIDOMLSSerializer)

nsDOMLSSerializer::nsDOMLSSerializer()
{
  /* member initializers and constructor code */
}

nsDOMLSSerializer::~nsDOMLSSerializer()
{
  /* destructor code */
}

/* readonly attribute nsIDOMDOMConfiguration domConfig; */
NS_IMETHODIMP nsDOMLSSerializer::GetDomConfig(nsIDOMDOMConfiguration * *aDomConfig)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute DOMString newLine; */
NS_IMETHODIMP nsDOMLSSerializer::GetNewLine(nsAString & aNewLine)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMLSSerializer::SetNewLine(const nsAString & aNewLine)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsIDOMLSSerializerFilter filter; */
NS_IMETHODIMP nsDOMLSSerializer::GetFilter(nsIDOMLSSerializerFilter * *aFilter)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDOMLSSerializer::SetFilter(nsIDOMLSSerializerFilter * aFilter)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean write (in nsIDOMNode nodeArg, in nsIDOMLSOutput destination)  raises (LSException); */
NS_IMETHODIMP nsDOMLSSerializer::Write(nsIDOMNode *nodeArg, nsIDOMLSOutput *destination, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean writeToURI (in nsIDOMNode nodeArg, in DOMString uri)  raises (LSException); */
NS_IMETHODIMP nsDOMLSSerializer::WriteToURI(nsIDOMNode *nodeArg, const nsAString & uri, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* DOMString writeToString (in nsIDOMNode nodeArg)  raises (DOMException, LSException); */
NS_IMETHODIMP nsDOMLSSerializer::WriteToString(nsIDOMNode *nodeArg, nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDOMLSSerializer_h__ */
