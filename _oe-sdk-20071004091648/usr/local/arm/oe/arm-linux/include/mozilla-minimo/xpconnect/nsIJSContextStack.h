/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/js/src/xpconnect/idl/nsIJSContextStack.idl
 */

#ifndef __gen_nsIJSContextStack_h__
#define __gen_nsIJSContextStack_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIJSContextStack */
#define NS_IJSCONTEXTSTACK_IID_STR "c67d8270-3189-11d3-9885-006008962422"

#define NS_IJSCONTEXTSTACK_IID \
  {0xc67d8270, 0x3189, 0x11d3, \
    { 0x98, 0x85, 0x00, 0x60, 0x08, 0x96, 0x24, 0x22 }}

class NS_NO_VTABLE nsIJSContextStack : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IJSCONTEXTSTACK_IID)

  /* readonly attribute PRInt32 count; */
  NS_IMETHOD GetCount(PRInt32 *aCount) = 0;

  /* JSContext peek (); */
  NS_IMETHOD Peek(JSContext * *_retval) = 0;

  /* JSContext pop (); */
  NS_IMETHOD Pop(JSContext * *_retval) = 0;

  /* void push (in JSContext cx); */
  NS_IMETHOD Push(JSContext * cx) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIJSCONTEXTSTACK \
  NS_IMETHOD GetCount(PRInt32 *aCount); \
  NS_IMETHOD Peek(JSContext * *_retval); \
  NS_IMETHOD Pop(JSContext * *_retval); \
  NS_IMETHOD Push(JSContext * cx); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIJSCONTEXTSTACK(_to) \
  NS_IMETHOD GetCount(PRInt32 *aCount) { return _to GetCount(aCount); } \
  NS_IMETHOD Peek(JSContext * *_retval) { return _to Peek(_retval); } \
  NS_IMETHOD Pop(JSContext * *_retval) { return _to Pop(_retval); } \
  NS_IMETHOD Push(JSContext * cx) { return _to Push(cx); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIJSCONTEXTSTACK(_to) \
  NS_IMETHOD GetCount(PRInt32 *aCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCount(aCount); } \
  NS_IMETHOD Peek(JSContext * *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Peek(_retval); } \
  NS_IMETHOD Pop(JSContext * *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Pop(_retval); } \
  NS_IMETHOD Push(JSContext * cx) { return !_to ? NS_ERROR_NULL_POINTER : _to->Push(cx); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsJSContextStack : public nsIJSContextStack
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIJSCONTEXTSTACK

  nsJSContextStack();

private:
  ~nsJSContextStack();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsJSContextStack, nsIJSContextStack)

nsJSContextStack::nsJSContextStack()
{
  /* member initializers and constructor code */
}

nsJSContextStack::~nsJSContextStack()
{
  /* destructor code */
}

/* readonly attribute PRInt32 count; */
NS_IMETHODIMP nsJSContextStack::GetCount(PRInt32 *aCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* JSContext peek (); */
NS_IMETHODIMP nsJSContextStack::Peek(JSContext * *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* JSContext pop (); */
NS_IMETHODIMP nsJSContextStack::Pop(JSContext * *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void push (in JSContext cx); */
NS_IMETHODIMP nsJSContextStack::Push(JSContext * cx)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIThreadJSContextStack */
#define NS_ITHREADJSCONTEXTSTACK_IID_STR "a1339ae0-05c1-11d4-8f92-0010a4e73d9a"

#define NS_ITHREADJSCONTEXTSTACK_IID \
  {0xa1339ae0, 0x05c1, 0x11d4, \
    { 0x8f, 0x92, 0x00, 0x10, 0xa4, 0xe7, 0x3d, 0x9a }}

class NS_NO_VTABLE nsIThreadJSContextStack : public nsIJSContextStack {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ITHREADJSCONTEXTSTACK_IID)

  /* attribute JSContext safeJSContext; */
  NS_IMETHOD GetSafeJSContext(JSContext * *aSafeJSContext) = 0;
  NS_IMETHOD SetSafeJSContext(JSContext * aSafeJSContext) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITHREADJSCONTEXTSTACK \
  NS_IMETHOD GetSafeJSContext(JSContext * *aSafeJSContext); \
  NS_IMETHOD SetSafeJSContext(JSContext * aSafeJSContext); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITHREADJSCONTEXTSTACK(_to) \
  NS_IMETHOD GetSafeJSContext(JSContext * *aSafeJSContext) { return _to GetSafeJSContext(aSafeJSContext); } \
  NS_IMETHOD SetSafeJSContext(JSContext * aSafeJSContext) { return _to SetSafeJSContext(aSafeJSContext); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITHREADJSCONTEXTSTACK(_to) \
  NS_IMETHOD GetSafeJSContext(JSContext * *aSafeJSContext) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSafeJSContext(aSafeJSContext); } \
  NS_IMETHOD SetSafeJSContext(JSContext * aSafeJSContext) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSafeJSContext(aSafeJSContext); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsThreadJSContextStack : public nsIThreadJSContextStack
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSITHREADJSCONTEXTSTACK

  nsThreadJSContextStack();

private:
  ~nsThreadJSContextStack();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsThreadJSContextStack, nsIThreadJSContextStack)

nsThreadJSContextStack::nsThreadJSContextStack()
{
  /* member initializers and constructor code */
}

nsThreadJSContextStack::~nsThreadJSContextStack()
{
  /* destructor code */
}

/* attribute JSContext safeJSContext; */
NS_IMETHODIMP nsThreadJSContextStack::GetSafeJSContext(JSContext * *aSafeJSContext)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsThreadJSContextStack::SetSafeJSContext(JSContext * aSafeJSContext)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIJSContextStack_h__ */
