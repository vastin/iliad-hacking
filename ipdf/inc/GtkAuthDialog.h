/* 
 * Copyright (C) 2006, iRex Technologies B.V.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef GTK_AUTH_DIALOG_H_
#define GTK_AUTH_DIALOG_H_
#include <gtk/gtk.h>

#define MAX_PASSWORD_LENGTH     1024

// gtk pin code screen. 
#define GTK_PINCODE_SCREEN_TYPE                 (gtk_pincode_screen_get_type ())
#define GTK_PINCODE_SCREEN(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_PINCODE_SCREEN_TYPE, GtkPincodeScreen))
#define GTK_PINCODE_SCREEN_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_PINCODE_SCREEN_TYPE, GtkPincodeScreenClass))
#define IS_GTK_PINCODE_SCREEN(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_PINCODE_SCREEN_TYPE))
#define IS_GTK_PINCODE_SCREEN_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_PINCODE_SCREEN_TYPE))

typedef struct _GtkPincodeScreen GtkPincodeScreen;
typedef struct _GtkPincodeScreenClass GtkPincodeScreenClass;

// widget
class GtkAuthDialog;
struct _GtkPincodeScreen
{
    GtkEventBox box;                // container and background
    GtkWidget * window;             // container window
    GtkWidget *clientArea;          // GtkVBox
    GtkWidget *messageWidget;       // message widget
    GtkWidget *inputWidget;         // input widget
    GtkButton *okButton;            // GtkToggleButton
    GtkWidget *errorMsg;            // error message
    GtkAuthDialog * authDialog;
};

// widget class
struct _GtkPincodeScreenClass
{
    GtkEventBoxClass parent_class;
};

GType gtk_pincode_screen_get_type(void);
GtkWidget *gtk_pincode_screen_new();

//////////////////////////////////////////////////////////////////////////
// Gtk authentication dialog
//////////////////////////////////////////////////////////////////////////
class CPDFView;

class GtkAuthDialog
{
private:
    GtkPincodeScreen *  dialog;
    CPDFView *          view;
    char                password[MAX_PASSWORD_LENGTH];
    gboolean            bOk;
    gboolean            bUpdate;        // refresh screen?

private:
    void initPincodeDialog();

public:

    ////////////////////// event handler //////////////////////
    static gboolean exposeCbk(GtkPincodeScreen * dialog,
        GdkEventExpose * event, gpointer user_data);
        
    static gboolean destroyCbk(GtkPincodeScreen * dialog, 
        GdkEvent * event, gpointer user_data);


    //////////////////// input event handler /////////////////////////
    static gboolean focusInCbk(GtkWidget *widget, 
        GdkEventFocus * event, gpointer data);

    static gboolean focusOutCbk(GtkWidget *widget, 
        GdkEventFocus * event, gpointer data);

    static gboolean focusCbk(GtkWidget * widget, 
        GtkDirectionType arg1, gpointer data);

    static void changedCbk(GtkEditable * widget, 
        gpointer data);

    static void moveCursorCbk(GtkEntry * widget, 
        GtkMovementStep arg1, gint arg2, gboolean arg3, gpointer data);

    /////////////////////// button event handler ////////////////////////
    static void buttonClickedCbk(GtkToggleButton * button, 
        gpointer user_data);

    static void buttonStateChangedCbk(GtkWidget * button, 
        GtkStateType state, gpointer user_data);

public:
    GtkAuthDialog(void);
    ~GtkAuthDialog(void);

public:
    gboolean  popupAuthDialog(CPDFView *);
    char *    getPassword() { return password; }
};

#endif
