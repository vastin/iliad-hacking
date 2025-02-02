/* gtkiconview.h
 * Copyright (C) 2002, 2004  Anders Carlsson <andersca@gnome.org>
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
#ifndef __GTK_ICON_VIEW_H__
#define __GTK_ICON_VIEW_H__

#include <gtk/gtkcontainer.h>
#include <gtk/gtktreemodel.h>

G_BEGIN_DECLS

#define GTK_TYPE_ICON_VIEW		(gtk_icon_view_get_type ())
#define GTK_ICON_VIEW(obj)		(GTK_CHECK_CAST ((obj), GTK_TYPE_ICON_VIEW, GtkIconView))
#define GTK_ICON_VIEW_CLASS(klass)	(GTK_CHECK_CLASS_CAST ((klass), GTK_TYPE_ICON_VIEW, GtkIconViewClass))
#define GTK_IS_ICON_VIEW(obj)		(GTK_CHECK_TYPE ((obj), GTK_TYPE_ICON_VIEW))
#define GTK_IS_ICON_VIEW_CLASS(klass)	(GTK_CHECK_CLASS_TYPE ((klass), GTK_TYPE_ICON_VIEW))
#define GTK_ICON_VIEW_GET_CLASS(obj)    (GTK_CHECK_GET_CLASS ((obj), GTK_TYPE_ICON_VIEW, GtkIconViewClass))

typedef struct _GtkIconView           GtkIconView;
typedef struct _GtkIconViewClass      GtkIconViewClass;
typedef struct _GtkIconViewPrivate    GtkIconViewPrivate;

typedef void (* GtkIconViewForeachFunc)     (GtkIconView      *icon_view,
					     GtkTreePath      *path,
					     gpointer          data);

struct _GtkIconView
{
  GtkContainer parent;

  GtkIconViewPrivate *priv;
};

struct _GtkIconViewClass
{
  GtkContainerClass parent_class;

  void    (* set_scroll_adjustments) (GtkIconView      *icon_view,
				      GtkAdjustment    *hadjustment,
				      GtkAdjustment    *vadjustment);
  
  void    (* item_activated)         (GtkIconView      *icon_view,
				      GtkTreePath      *path);
  void    (* selection_changed)      (GtkIconView      *icon_view);

  /* Key binding signals */
  void    (* select_all)             (GtkIconView      *icon_view);
  void    (* unselect_all)           (GtkIconView      *icon_view);
  void    (* select_cursor_item)     (GtkIconView      *icon_view);
  void    (* toggle_cursor_item)     (GtkIconView      *icon_view);
  gboolean (* move_cursor)           (GtkIconView      *icon_view,
				      GtkMovementStep   step,
				      gint              count);
  gboolean (* activate_cursor_item)  (GtkIconView      *icon_view);
};

GType      gtk_icon_view_get_type       (void) G_GNUC_CONST;
GtkWidget *gtk_icon_view_new            (void);
GtkWidget *gtk_icon_view_new_with_model (GtkTreeModel *model);

void          gtk_icon_view_set_model         (GtkIconView  *icon_view,
					       GtkTreeModel *model);
GtkTreeModel *gtk_icon_view_get_model         (GtkIconView  *icon_view);
void          gtk_icon_view_set_text_column   (GtkIconView  *icon_view,
					       gint          column);
gint          gtk_icon_view_get_text_column   (GtkIconView  *icon_view);
void          gtk_icon_view_set_markup_column (GtkIconView  *icon_view,
					       gint          column);
gint          gtk_icon_view_get_markup_column (GtkIconView  *icon_view);
void          gtk_icon_view_set_pixbuf_column (GtkIconView  *icon_view,
					       gint          column);
gint          gtk_icon_view_get_pixbuf_column (GtkIconView  *icon_view);

void           gtk_icon_view_set_orientation   (GtkIconView    *icon_view,
	   			                GtkOrientation  orientation);
GtkOrientation gtk_icon_view_get_orientation   (GtkIconView    *icon_view);
void          gtk_icon_view_set_columns        (GtkIconView    *icon_view,
					        gint            columns);
gint          gtk_icon_view_get_columns        (GtkIconView    *icon_view);
void          gtk_icon_view_set_item_width     (GtkIconView    *icon_view,
					        gint            item_width);
gint          gtk_icon_view_get_item_width     (GtkIconView    *icon_view);
void          gtk_icon_view_set_spacing        (GtkIconView    *icon_view, 
					        gint            spacing);
gint          gtk_icon_view_get_spacing        (GtkIconView    *icon_view);
void          gtk_icon_view_set_row_spacing    (GtkIconView    *icon_view, 
					        gint            row_spacing);
gint          gtk_icon_view_get_row_spacing    (GtkIconView    *icon_view);
void          gtk_icon_view_set_column_spacing (GtkIconView    *icon_view, 
					        gint            column_spacing);
gint          gtk_icon_view_get_column_spacing (GtkIconView    *icon_view);
void          gtk_icon_view_set_margin         (GtkIconView    *icon_view, 
					        gint            margin);
gint          gtk_icon_view_get_margin         (GtkIconView    *icon_view);


GtkTreePath *    gtk_icon_view_get_path_at_pos    (GtkIconView            *icon_view,
						   gint                    x,
						   gint                    y);
void             gtk_icon_view_selected_foreach   (GtkIconView            *icon_view,
						   GtkIconViewForeachFunc  func,
						   gpointer                data);
void             gtk_icon_view_set_selection_mode (GtkIconView            *icon_view,
						   GtkSelectionMode        mode);
GtkSelectionMode gtk_icon_view_get_selection_mode (GtkIconView            *icon_view);
void             gtk_icon_view_select_path        (GtkIconView            *icon_view,
						   GtkTreePath            *path);
void             gtk_icon_view_unselect_path      (GtkIconView            *icon_view,
						   GtkTreePath            *path);
gboolean         gtk_icon_view_path_is_selected   (GtkIconView            *icon_view,
						   GtkTreePath            *path);
GList           *gtk_icon_view_get_selected_items (GtkIconView            *icon_view);
void             gtk_icon_view_select_all         (GtkIconView            *icon_view);
void             gtk_icon_view_unselect_all       (GtkIconView            *icon_view);
void             gtk_icon_view_item_activated     (GtkIconView            *icon_view,
						   GtkTreePath            *path);
G_END_DECLS

#endif /* __GTK_ICON_VIEW_H__ */
