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
 * Portions created by the Initial Developer are Copyright (C) 1998
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
#ifndef nsIObjectFrame_h___
#define nsIObjectFrame_h___

#include "nsIFrame.h"

class nsIPluginInstance;

// {6D10B07D-E75B-11d4-9885-00C04FA0CF4B}
#define NS_IOBJECTFRAME_IID \
{ 0x6d10b07d, 0xe75b, 0x11d4, { 0x98, 0x85, 0x0, 0xc0, 0x4f, 0xa0, 0xcf, 0x4b } }

class nsIObjectFrame : public nsISupports {
public:
  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IOBJECTFRAME_IID)

  NS_IMETHOD GetPluginInstance(nsIPluginInstance*& aPluginInstance) = 0;

  /**
   * Instantiate a plugin for a channel, returning a stream listener for the
   * data.
   */
  virtual nsresult Instantiate(nsIChannel* aChannel, nsIStreamListener** aStreamListener) = 0;

  /**
   * Instantiate a plugin that loads the data itself.
   * @param aMimeType Type of the plugin to instantiate. May be null.
   *                  This argument is ignored if a classid is present on the
   *                  plugin.
   * @param aURI      URI of the plugin data. May be null.
   * @note XXX this method is here only temporarily, until plugins are loaded
   *       from content.
   */
  virtual nsresult Instantiate(const char* aMimeType, nsIURI* aURI) = 0;
};


#endif /* nsIObjectFrame_h___ */
