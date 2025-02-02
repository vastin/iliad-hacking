/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/toolkit/components/gnome/nsIGnomeVFSService.idl
 */

#ifndef __gen_nsIGnomeVFSService_h__
#define __gen_nsIGnomeVFSService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIUTF8StringEnumerator; /* forward declaration */

class nsIURI; /* forward declaration */


/* starting interface:    nsIGnomeVFSMimeApp */
#define NS_IGNOMEVFSMIMEAPP_IID_STR "99ae024f-e869-4973-958b-54768a84295a"

#define NS_IGNOMEVFSMIMEAPP_IID \
  {0x99ae024f, 0xe869, 0x4973, \
    { 0x95, 0x8b, 0x54, 0x76, 0x8a, 0x84, 0x29, 0x5a }}

class NS_NO_VTABLE nsIGnomeVFSMimeApp : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IGNOMEVFSMIMEAPP_IID)

  enum { EXPECTS_URIS = 0 };

  enum { EXPECTS_PATHS = 1 };

  enum { EXPECTS_URIS_FOR_NON_FILES = 2 };

  /* readonly attribute AUTF8String id; */
  NS_IMETHOD GetId(nsACString & aId) = 0;

  /* readonly attribute AUTF8String name; */
  NS_IMETHOD GetName(nsACString & aName) = 0;

  /* readonly attribute AUTF8String command; */
  NS_IMETHOD GetCommand(nsACString & aCommand) = 0;

  /* readonly attribute boolean canOpenMultipleFiles; */
  NS_IMETHOD GetCanOpenMultipleFiles(PRBool *aCanOpenMultipleFiles) = 0;

  /* readonly attribute long expectsURIs; */
  NS_IMETHOD GetExpectsURIs(PRInt32 *aExpectsURIs) = 0;

  /* readonly attribute nsIUTF8StringEnumerator supportedURISchemes; */
  NS_IMETHOD GetSupportedURISchemes(nsIUTF8StringEnumerator * *aSupportedURISchemes) = 0;

  /* readonly attribute boolean requiresTerminal; */
  NS_IMETHOD GetRequiresTerminal(PRBool *aRequiresTerminal) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIGNOMEVFSMIMEAPP \
  NS_IMETHOD GetId(nsACString & aId); \
  NS_IMETHOD GetName(nsACString & aName); \
  NS_IMETHOD GetCommand(nsACString & aCommand); \
  NS_IMETHOD GetCanOpenMultipleFiles(PRBool *aCanOpenMultipleFiles); \
  NS_IMETHOD GetExpectsURIs(PRInt32 *aExpectsURIs); \
  NS_IMETHOD GetSupportedURISchemes(nsIUTF8StringEnumerator * *aSupportedURISchemes); \
  NS_IMETHOD GetRequiresTerminal(PRBool *aRequiresTerminal); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIGNOMEVFSMIMEAPP(_to) \
  NS_IMETHOD GetId(nsACString & aId) { return _to GetId(aId); } \
  NS_IMETHOD GetName(nsACString & aName) { return _to GetName(aName); } \
  NS_IMETHOD GetCommand(nsACString & aCommand) { return _to GetCommand(aCommand); } \
  NS_IMETHOD GetCanOpenMultipleFiles(PRBool *aCanOpenMultipleFiles) { return _to GetCanOpenMultipleFiles(aCanOpenMultipleFiles); } \
  NS_IMETHOD GetExpectsURIs(PRInt32 *aExpectsURIs) { return _to GetExpectsURIs(aExpectsURIs); } \
  NS_IMETHOD GetSupportedURISchemes(nsIUTF8StringEnumerator * *aSupportedURISchemes) { return _to GetSupportedURISchemes(aSupportedURISchemes); } \
  NS_IMETHOD GetRequiresTerminal(PRBool *aRequiresTerminal) { return _to GetRequiresTerminal(aRequiresTerminal); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIGNOMEVFSMIMEAPP(_to) \
  NS_IMETHOD GetId(nsACString & aId) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetId(aId); } \
  NS_IMETHOD GetName(nsACString & aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD GetCommand(nsACString & aCommand) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCommand(aCommand); } \
  NS_IMETHOD GetCanOpenMultipleFiles(PRBool *aCanOpenMultipleFiles) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCanOpenMultipleFiles(aCanOpenMultipleFiles); } \
  NS_IMETHOD GetExpectsURIs(PRInt32 *aExpectsURIs) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetExpectsURIs(aExpectsURIs); } \
  NS_IMETHOD GetSupportedURISchemes(nsIUTF8StringEnumerator * *aSupportedURISchemes) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSupportedURISchemes(aSupportedURISchemes); } \
  NS_IMETHOD GetRequiresTerminal(PRBool *aRequiresTerminal) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetRequiresTerminal(aRequiresTerminal); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsGnomeVFSMimeApp : public nsIGnomeVFSMimeApp
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIGNOMEVFSMIMEAPP

  nsGnomeVFSMimeApp();

private:
  ~nsGnomeVFSMimeApp();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsGnomeVFSMimeApp, nsIGnomeVFSMimeApp)

nsGnomeVFSMimeApp::nsGnomeVFSMimeApp()
{
  /* member initializers and constructor code */
}

nsGnomeVFSMimeApp::~nsGnomeVFSMimeApp()
{
  /* destructor code */
}

/* readonly attribute AUTF8String id; */
NS_IMETHODIMP nsGnomeVFSMimeApp::GetId(nsACString & aId)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AUTF8String name; */
NS_IMETHODIMP nsGnomeVFSMimeApp::GetName(nsACString & aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AUTF8String command; */
NS_IMETHODIMP nsGnomeVFSMimeApp::GetCommand(nsACString & aCommand)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean canOpenMultipleFiles; */
NS_IMETHODIMP nsGnomeVFSMimeApp::GetCanOpenMultipleFiles(PRBool *aCanOpenMultipleFiles)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute long expectsURIs; */
NS_IMETHODIMP nsGnomeVFSMimeApp::GetExpectsURIs(PRInt32 *aExpectsURIs)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIUTF8StringEnumerator supportedURISchemes; */
NS_IMETHODIMP nsGnomeVFSMimeApp::GetSupportedURISchemes(nsIUTF8StringEnumerator * *aSupportedURISchemes)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean requiresTerminal; */
NS_IMETHODIMP nsGnomeVFSMimeApp::GetRequiresTerminal(PRBool *aRequiresTerminal)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIGnomeVFSService */
#define NS_IGNOMEVFSSERVICE_IID_STR "4d6b9f23-8682-41b3-bbff-937a958e6496"

#define NS_IGNOMEVFSSERVICE_IID \
  {0x4d6b9f23, 0x8682, 0x41b3, \
    { 0xbb, 0xff, 0x93, 0x7a, 0x95, 0x8e, 0x64, 0x96 }}

class NS_NO_VTABLE nsIGnomeVFSService : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IGNOMEVFSSERVICE_IID)

  /*** Application registry methods ***/
  enum { APP_KEY_COMMAND = 0 };

  enum { APP_KEY_NAME = 1 };

  enum { APP_KEY_SUPPORTED_URI_SCHEMES = 2 };

  enum { APP_KEY_EXPECTS_URIS = 3 };

  /* void setAppStringKey (in AUTF8String id, in long key, in AUTF8String value); */
  NS_IMETHOD SetAppStringKey(const nsACString & id, PRInt32 key, const nsACString & value) = 0;

  enum { APP_KEY_CAN_OPEN_MULTIPLE = 4 };

  enum { APP_KEY_REQUIRES_TERMINAL = 5 };

  /* void setAppBoolKey (in AUTF8String id, in long key, in boolean value); */
  NS_IMETHOD SetAppBoolKey(const nsACString & id, PRInt32 key, PRBool value) = 0;

  /* void addMimeType (in AUTF8String id, in AUTF8String mimeType); */
  NS_IMETHOD AddMimeType(const nsACString & id, const nsACString & mimeType) = 0;

  /* void syncAppRegistry (); */
  NS_IMETHOD SyncAppRegistry(void) = 0;

  /*** MIME registry methods ***/
  /* AUTF8String getMimeTypeFromExtension (in AUTF8String extension); */
  NS_IMETHOD GetMimeTypeFromExtension(const nsACString & extension, nsACString & _retval) = 0;

  /* nsIGnomeVFSMimeApp getAppForMimeType (in AUTF8String mimeType); */
  NS_IMETHOD GetAppForMimeType(const nsACString & mimeType, nsIGnomeVFSMimeApp **_retval) = 0;

  /* void setAppForMimeType (in AUTF8String mimeType, in AUTF8String id); */
  NS_IMETHOD SetAppForMimeType(const nsACString & mimeType, const nsACString & id) = 0;

  /* AUTF8String getDescriptionForMimeType (in AUTF8String mimeType); */
  NS_IMETHOD GetDescriptionForMimeType(const nsACString & mimeType, nsACString & _retval) = 0;

  /* void setIconForMimeType (in AUTF8String mimeType, in AUTF8String iconPath); */
  NS_IMETHOD SetIconForMimeType(const nsACString & mimeType, const nsACString & iconPath) = 0;

  /* void setMimeExtensions (in AUTF8String mimeType, in AUTF8String extensionsList); */
  NS_IMETHOD SetMimeExtensions(const nsACString & mimeType, const nsACString & extensionsList) = 0;

  /*** Misc. methods ***/
  /* void showURI (in nsIURI uri); */
  NS_IMETHOD ShowURI(nsIURI *uri) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIGNOMEVFSSERVICE \
  NS_IMETHOD SetAppStringKey(const nsACString & id, PRInt32 key, const nsACString & value); \
  NS_IMETHOD SetAppBoolKey(const nsACString & id, PRInt32 key, PRBool value); \
  NS_IMETHOD AddMimeType(const nsACString & id, const nsACString & mimeType); \
  NS_IMETHOD SyncAppRegistry(void); \
  NS_IMETHOD GetMimeTypeFromExtension(const nsACString & extension, nsACString & _retval); \
  NS_IMETHOD GetAppForMimeType(const nsACString & mimeType, nsIGnomeVFSMimeApp **_retval); \
  NS_IMETHOD SetAppForMimeType(const nsACString & mimeType, const nsACString & id); \
  NS_IMETHOD GetDescriptionForMimeType(const nsACString & mimeType, nsACString & _retval); \
  NS_IMETHOD SetIconForMimeType(const nsACString & mimeType, const nsACString & iconPath); \
  NS_IMETHOD SetMimeExtensions(const nsACString & mimeType, const nsACString & extensionsList); \
  NS_IMETHOD ShowURI(nsIURI *uri); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIGNOMEVFSSERVICE(_to) \
  NS_IMETHOD SetAppStringKey(const nsACString & id, PRInt32 key, const nsACString & value) { return _to SetAppStringKey(id, key, value); } \
  NS_IMETHOD SetAppBoolKey(const nsACString & id, PRInt32 key, PRBool value) { return _to SetAppBoolKey(id, key, value); } \
  NS_IMETHOD AddMimeType(const nsACString & id, const nsACString & mimeType) { return _to AddMimeType(id, mimeType); } \
  NS_IMETHOD SyncAppRegistry(void) { return _to SyncAppRegistry(); } \
  NS_IMETHOD GetMimeTypeFromExtension(const nsACString & extension, nsACString & _retval) { return _to GetMimeTypeFromExtension(extension, _retval); } \
  NS_IMETHOD GetAppForMimeType(const nsACString & mimeType, nsIGnomeVFSMimeApp **_retval) { return _to GetAppForMimeType(mimeType, _retval); } \
  NS_IMETHOD SetAppForMimeType(const nsACString & mimeType, const nsACString & id) { return _to SetAppForMimeType(mimeType, id); } \
  NS_IMETHOD GetDescriptionForMimeType(const nsACString & mimeType, nsACString & _retval) { return _to GetDescriptionForMimeType(mimeType, _retval); } \
  NS_IMETHOD SetIconForMimeType(const nsACString & mimeType, const nsACString & iconPath) { return _to SetIconForMimeType(mimeType, iconPath); } \
  NS_IMETHOD SetMimeExtensions(const nsACString & mimeType, const nsACString & extensionsList) { return _to SetMimeExtensions(mimeType, extensionsList); } \
  NS_IMETHOD ShowURI(nsIURI *uri) { return _to ShowURI(uri); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIGNOMEVFSSERVICE(_to) \
  NS_IMETHOD SetAppStringKey(const nsACString & id, PRInt32 key, const nsACString & value) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetAppStringKey(id, key, value); } \
  NS_IMETHOD SetAppBoolKey(const nsACString & id, PRInt32 key, PRBool value) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetAppBoolKey(id, key, value); } \
  NS_IMETHOD AddMimeType(const nsACString & id, const nsACString & mimeType) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddMimeType(id, mimeType); } \
  NS_IMETHOD SyncAppRegistry(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->SyncAppRegistry(); } \
  NS_IMETHOD GetMimeTypeFromExtension(const nsACString & extension, nsACString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMimeTypeFromExtension(extension, _retval); } \
  NS_IMETHOD GetAppForMimeType(const nsACString & mimeType, nsIGnomeVFSMimeApp **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAppForMimeType(mimeType, _retval); } \
  NS_IMETHOD SetAppForMimeType(const nsACString & mimeType, const nsACString & id) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetAppForMimeType(mimeType, id); } \
  NS_IMETHOD GetDescriptionForMimeType(const nsACString & mimeType, nsACString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDescriptionForMimeType(mimeType, _retval); } \
  NS_IMETHOD SetIconForMimeType(const nsACString & mimeType, const nsACString & iconPath) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetIconForMimeType(mimeType, iconPath); } \
  NS_IMETHOD SetMimeExtensions(const nsACString & mimeType, const nsACString & extensionsList) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetMimeExtensions(mimeType, extensionsList); } \
  NS_IMETHOD ShowURI(nsIURI *uri) { return !_to ? NS_ERROR_NULL_POINTER : _to->ShowURI(uri); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsGnomeVFSService : public nsIGnomeVFSService
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIGNOMEVFSSERVICE

  nsGnomeVFSService();

private:
  ~nsGnomeVFSService();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsGnomeVFSService, nsIGnomeVFSService)

nsGnomeVFSService::nsGnomeVFSService()
{
  /* member initializers and constructor code */
}

nsGnomeVFSService::~nsGnomeVFSService()
{
  /* destructor code */
}

/* void setAppStringKey (in AUTF8String id, in long key, in AUTF8String value); */
NS_IMETHODIMP nsGnomeVFSService::SetAppStringKey(const nsACString & id, PRInt32 key, const nsACString & value)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setAppBoolKey (in AUTF8String id, in long key, in boolean value); */
NS_IMETHODIMP nsGnomeVFSService::SetAppBoolKey(const nsACString & id, PRInt32 key, PRBool value)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void addMimeType (in AUTF8String id, in AUTF8String mimeType); */
NS_IMETHODIMP nsGnomeVFSService::AddMimeType(const nsACString & id, const nsACString & mimeType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void syncAppRegistry (); */
NS_IMETHODIMP nsGnomeVFSService::SyncAppRegistry()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AUTF8String getMimeTypeFromExtension (in AUTF8String extension); */
NS_IMETHODIMP nsGnomeVFSService::GetMimeTypeFromExtension(const nsACString & extension, nsACString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIGnomeVFSMimeApp getAppForMimeType (in AUTF8String mimeType); */
NS_IMETHODIMP nsGnomeVFSService::GetAppForMimeType(const nsACString & mimeType, nsIGnomeVFSMimeApp **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setAppForMimeType (in AUTF8String mimeType, in AUTF8String id); */
NS_IMETHODIMP nsGnomeVFSService::SetAppForMimeType(const nsACString & mimeType, const nsACString & id)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AUTF8String getDescriptionForMimeType (in AUTF8String mimeType); */
NS_IMETHODIMP nsGnomeVFSService::GetDescriptionForMimeType(const nsACString & mimeType, nsACString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setIconForMimeType (in AUTF8String mimeType, in AUTF8String iconPath); */
NS_IMETHODIMP nsGnomeVFSService::SetIconForMimeType(const nsACString & mimeType, const nsACString & iconPath)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setMimeExtensions (in AUTF8String mimeType, in AUTF8String extensionsList); */
NS_IMETHODIMP nsGnomeVFSService::SetMimeExtensions(const nsACString & mimeType, const nsACString & extensionsList)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void showURI (in nsIURI uri); */
NS_IMETHODIMP nsGnomeVFSService::ShowURI(nsIURI *uri)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_GNOMEVFSSERVICE_CONTRACTID "@mozilla.org/gnome-vfs-service;1"

#endif /* __gen_nsIGnomeVFSService_h__ */
