/* gtkcellrenderertoggle.h
 * Copyright (C) 2000  Red Hat, Inc.,  Jonathan Blandford <jrb@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GTK_CELL_RENDERER_TOGGLE_H__
#define __GTK_CELL_RENDERER_TOGGLE_H__

#include <gtk/gtkcellrenderer.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define GTK_TYPE_CELL_RENDERER_TOGGLE			(gtk_cell_renderer_toggle_get_type ())
#define GTK_CELL_RENDERER_TOGGLE(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CELL_RENDERER_TOGGLE, GtkCellRendererToggle))
#define GTK_CELL_RENDERER_TOGGLE_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_CELL_RENDERER_TOGGLE, GtkCellRendererToggleClass))
#define GTK_IS_CELL_RENDERER_TOGGLE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CELL_RENDERER_TOGGLE))
#define GTK_IS_CELL_RENDERER_TOGGLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_CELL_RENDERER_TOGGLE))
#define GTK_CELL_RENDERER_TOGGLE_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_CELL_RENDERER_TOGGLE, GtkCellRendererToggleClass))

typedef struct _GtkCellRendererToggle GtkCellRendererToggle;
typedef struct _GtkCellRendererToggleClass GtkCellRendererToggleClass;

struct _GtkCellRendererToggle
{
  GtkCellRenderer parent;

  /*< private >*/
  guint active : 1;
  guint activatable : 1;
  guint radio : 1;
};

struct _GtkCellRendererToggleClass
{
  GtkCellRendererClass parent_class;

  void (* toggled) (GtkCellRendererToggle *cell_renderer_toggle,
		    const gchar                 *path);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

GType            gtk_cell_renderer_toggle_get_type  (void) G_GNUC_CONST;
GtkCellRenderer *gtk_cell_renderer_toggle_new       (void);

gboolean         gtk_cell_renderer_toggle_get_radio (GtkCellRendererToggle *toggle);
void             gtk_cell_renderer_toggle_set_radio (GtkCellRendererToggle *toggle,
						     gboolean               radio);

gboolean        gtk_cell_renderer_toggle_get_active (GtkCellRendererToggle *toggle);
void            gtk_cell_renderer_toggle_set_active (GtkCellRendererToggle *toggle,
                                                     gboolean               setting);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __GTK_CELL_RENDERER_TOGGLE_H__ */
