/* GTK - The GIMP Toolkit
 * Copyright 1998-2002 Tim Janik, Red Hat, Inc., and others.
 * Copyright (C) 2003 Alex Graveley
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GTK_MODULES_H__
#define __GTK_MODULES_H__

#include <gtk/gtksettings.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Functions for use within GTK+
 */
gchar * _gtk_find_module        (const gchar *name,
			         const gchar *type);
gchar **_gtk_get_module_path    (const gchar *type);

void    _gtk_modules_init             (gint         *argc,
				       gchar      ***argv,
				       const gchar  *gtk_modules_args);
void    _gtk_modules_settings_changed (GtkSettings  *settings, 
				       const gchar  *modules);

typedef void	 (*GtkModuleInitFunc)        (gint	  *argc,
					      gchar      ***argv);
typedef void	 (*GtkModuleDisplayInitFunc) (GdkDisplay   *display);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTK_MODULES_H__ */
