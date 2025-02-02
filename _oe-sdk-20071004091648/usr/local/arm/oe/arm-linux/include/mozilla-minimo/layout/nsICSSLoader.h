/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1999
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 2 or later (the "GPL"),
 * or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
#ifndef nsICSSLoader_h___
#define nsICSSLoader_h___

#include "nsISupports.h"
#include "nsSubstring.h"
#include "nsCompatibility.h"

class nsIAtom;
class nsIURI;
class nsICSSParser;
class nsICSSStyleSheet;
class nsPresContext;
class nsIContent;
class nsIParser;
class nsIDocument;
class nsIUnicharInputStream;
class nsICSSLoaderObserver;
class nsMediaList;
class nsICSSImportRule;

// IID for the nsICSSLoader interface
// ff43802a-b355-41f2-919d-5c7ab3dbfb91
#define NS_ICSS_LOADER_IID     \
{0xff43802a, 0xb355, 0x41f2, {0x91, 0x9d, 0x5c, 0x7a, 0xb3, 0xdb, 0xfb, 0x91}}

typedef void (*nsCSSLoaderCallbackFunc)(nsICSSStyleSheet* aSheet, void *aData, PRBool aDidNotify);

class nsICSSLoader : public nsISupports {
public:
  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ICSS_LOADER_IID)

  NS_IMETHOD Init(nsIDocument* aDocument) = 0;
  NS_IMETHOD DropDocumentReference(void) = 0; // notification that doc is going away

  NS_IMETHOD SetCaseSensitive(PRBool aCaseSensitive) = 0;
  NS_IMETHOD SetCompatibilityMode(nsCompatibility aCompatMode) = 0;
  NS_IMETHOD SetPreferredSheet(const nsAString& aTitle) = 0;
  NS_IMETHOD GetPreferredSheet(nsAString& aTitle) = 0;

  // Get/Recycle a CSS parser for general use
  NS_IMETHOD GetParserFor(nsICSSStyleSheet* aSheet,
                          nsICSSParser** aParser) = 0;
  NS_IMETHOD RecycleParser(nsICSSParser* aParser) = 0;

  // XXXbz sort out what the deal is with events!  When should they fire?
  
  /**
   * Load an inline style sheet.  If a successful result is returned and
   * *aCompleted is false, then aObserver is guaranteed to be notified
   * asynchronously once the sheet is marked complete.  If an error is
   * returned, or if *aCompleted is true, aObserver will not be notified.  In
   * addition to parsing the sheet, this method will insert it into the
   * stylesheet list of this CSSLoader's document.
   *
   * @param aElement the element linking to the stylesheet.  This must not be
   *                 null and must implement nsIStyleSheetLinkingElement.
   * @param aStream the character stream that holds the stylesheet data.
   * @param aLineNumber the line number at which the stylesheet data started.
   * @param aTitle the title of the sheet.
   * @param aMedia the media string for the sheet.
   * @param aParserToUnblock the parser to unblock when the load completes.
   *        Only loads that returned false for both aIsAlternate and
   *        aCompleted will unblock the parser.
   * @param aObserver the observer to notify when the load completes.
   *        May be null.
   * @param [out] aCompleted whether parsing of the sheet completed.
   * @param [out] aIsAlternate whether the stylesheet ended up being an
   *        alternate sheet.
   */
  NS_IMETHOD LoadInlineStyle(nsIContent* aElement,
                             nsIUnicharInputStream* aStream, 
                             PRUint32 aLineNumber,
                             const nsSubstring& aTitle,
                             const nsSubstring& aMedia,
                             nsIParser* aParserToUnblock,
                             nsICSSLoaderObserver* aObserver,
                             PRBool* aCompleted,
                             PRBool* aIsAlternate) = 0;

  /**
   * Load a linked (document) stylesheet.  If a successful result is returned,
   * aObserver is guaranteed to be notified asynchronously once the sheet is
   * loaded and marked complete.  If an error is returned, aObserver will not
   * be notified.  In addition to loading the sheet, this method will insert it
   * into the stylesheet list of this CSSLoader's document.
   *
   * @param aElement the element linking to the the stylesheet.  May be null.
   * @param aURL the URL of the sheet.
   * @param aTitle the title of the sheet.
   * @param aMedia the media string for the sheet.
   * @param aHasAlternateRel whether the rel for this link included
   *        "alternate".
   * @param aParserToUnblock the parser to unblock when the load completes.
   *        Only loads that returned false for aIsAlternate will unblock
   *        the parser.
   * @param aObserver the observer to notify when the load completes.
   *                  May be null.
   * @param [out] aIsAlternate whether the stylesheet actually ended up beinga
   *        an alternate sheet.  Note that this need not match
   *        aHasAlternateRel.
   */
  NS_IMETHOD LoadStyleLink(nsIContent* aElement,
                           nsIURI* aURL, 
                           const nsSubstring& aTitle,
                           const nsSubstring& aMedia,
                           PRBool aHasAlternateRel,
                           nsIParser* aParserToUnblock,
                           nsICSSLoaderObserver* aObserver,
                           PRBool* aIsAlternate) = 0;

  /**
   * Load a child (@import-ed) style sheet.  In addition to loading the sheet,
   * this method will insert it into the child sheet list of aParentSheet.  If
   * there is no sheet currently being parsed and the child sheet is not
   * complete when this method returns, then when the child sheet becomes
   * complete aParentSheet will be QIed to nsICSSLoaderObserver and
   * asynchronously notified, just like for LoadStyleLink.  Note that if the
   * child sheet is already complete when this method returns, no
   * nsICSSLoaderObserver notification will be sent.
   *
   * @param aParentSheet the parent of this child sheet
   * @param aURL the URL of the child sheet
   * @param aMedia the already-parsed media list for the child sheet
   * @param aRule the @import rule importing this child.  This is used to
   *              properly order the child sheet list of aParentSheet.
   */
  NS_IMETHOD LoadChildSheet(nsICSSStyleSheet* aParentSheet,
                            nsIURI* aURL, 
                            nsMediaList* aMedia,
                            nsICSSImportRule* aRule) = 0;

  /**
   * Synchronously load and return the stylesheet at aURL.  Any child sheets
   * will also be loaded synchronously.  Note that synchronous loads over some
   * protocols may involve spinning up a new event loop, so use of this method
   * does NOT guarantee not receiving any events before the sheet loads.  This
   * method can be used to load sheets not associated with a document.
   *
   * @param aURL the URL of the sheet to load
   * @param [out] aSheet the loaded, complete sheet.
   *
   * NOTE: At the moment, this method assumes the sheet will be UTF-8, but
   * ideally it would allow arbitrary encodings.  Callers should NOT depend on
   * non-UTF8 sheets being treated as UTF-8 by this method.
   *
   * NOTE: A successful return from this method doesn't indicate anything about
   * whether the data could be parsed as CSS and doesn't indicate anything
   * about the status of child sheets of the returned sheet.
   */
  NS_IMETHOD LoadSheetSync(nsIURI* aURL, nsICSSStyleSheet** aSheet) = 0;

  /**
   * Asynchronously load the stylesheet at aURL.  If a successful result is
   * returned, aObserver is guaranteed to be notified asynchronously once the
   * sheet is loaded and marked complete.  This method can be used to load
   * sheets not associated with a document.
   *
   * @param aURL the URL of the sheet to load
   * @param aObserver the observer to notify when the load completes.
   *                  Must not be null.
   */
  NS_IMETHOD LoadSheet(nsIURI* aURL, nsICSSLoaderObserver* aObserver) = 0;

  /**
   * Stop loading all sheets.  All nsICSSLoaderObservers involved will be
   * notified with NS_BINDING_ABORTED as the status, possibly synchronously.
   */
  NS_IMETHOD Stop(void) = 0;

  /**
   * Stop loading one sheet.  The nsICSSLoaderObserver involved will be
   * notified with NS_BINDING_ABORTED as the status, possibly synchronously.
   */
  NS_IMETHOD StopLoadingSheet(nsIURI* aURL) = 0;

  /**
   * Whether the loader is enabled or not.
   * When disabled, processing of new styles is disabled and an attempt
   * to do so will fail with a return code of
   * NS_ERROR_NOT_AVAILABLE. Note that this DOES NOT disable
   * currently loading styles or already processed styles.
   */
  NS_IMETHOD GetEnabled(PRBool *aEnabled) = 0;
  NS_IMETHOD SetEnabled(PRBool aEnabled) = 0;
};

nsresult 
NS_NewCSSLoader(nsIDocument* aDocument, nsICSSLoader** aLoader);

nsresult 
NS_NewCSSLoader(nsICSSLoader** aLoader);

#endif /* nsICSSLoader_h___ */
