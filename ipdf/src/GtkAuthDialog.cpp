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

#include "GtkAuthDialog.h"
#include "liberdm/erdm.h"
#include "PDFView.h"
#include "controller.h"
#include <glib.h>
#include <gtk/gtk.h>
#include <libintl.h>
#include <locale.h>

// define macro for string resource. i18n/g11n
#define _(string)       gettext(string)
#define N_(string)      string

// for the whole dialog
#define PINCODE_SCREEN_WIDTH        768         
#define PINCODE_SCREEN_HEIGHT       SCREEN_HEIGHT

// for client area
#define PINCODE_BACKGROUND_WIDTH    707         
#define PINCODE_BACKGROUND_HEIGHT   347
#define PINCODE_BG_PADDING_LEFT     (PINCODE_SCREEN_WIDTH - PINCODE_BACKGROUND_WIDTH)/2
#define PINCODE_BG_PADDING_RIGHT    PINCODE_BG_PADDING_LEFT
#define PINCODE_BG_PADDING_BOTTOM   270
#define PINCODE_BG_PADDING_TOP      540

// title message "Enter the password to open the protected document."
#define PINCODE_MESSAGE_PADDING     5
#define PINCODE_MESSAGE_LABEL_WIDTH  PINCODE_BACKGROUND_WIDTH
#define PINCODE_MESSAGE_LABEL_HEIGHT 30

// input entry
#define PINCODE_INPUT_XPOS          150
#define PINCODE_INPUT_YPOS          0
#define PINCODE_INPUT_WIDTH         200
#define PINCODE_INPUT_HEIGHT        30

// button
#define PINCODE_BUTTON_XPOS         450
#define PINCODE_BUTTON_YPOS         0
#define PINCODE_BUTTON_WIDTH        100
#define PINCODE_BUTTON_HEIGHT       30
#define PINCODE_BUTTON_OFFSET       30

// error message
#define PINCODE_ERRORMSG_XPOS       150
#define PINCODE_ERRORMSG_YPOS       0
#define PINCODE_ERRORMSG_WIDTH      400
#define PINCODE_ERRORMSG_HEIGHT     30


#define GTK_PINCODE_SCREEN_TYPE                 (gtk_pincode_screen_get_type ())
#define GTK_PINCODE_SCREEN(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_PINCODE_SCREEN_TYPE, GtkPincodeScreen))
#define GTK_PINCODE_SCREEN_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_PINCODE_SCREEN_TYPE, GtkPincodeScreenClass))
#define IS_GTK_PINCODE_SCREEN(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_PINCODE_SCREEN_TYPE))
#define IS_GTK_PINCODE_SCREEN_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_PINCODE_SCREEN_TYPE))


static void gtk_pincode_screen_class_init(GtkPincodeScreenClass * klass);
static void gtk_pincode_screen_init(GtkPincodeScreen * screen);
static int  count = -1;  

GtkWidget *gtk_pincode_screen_new()
{
    GtkPincodeScreen *screen;
    screen = (GtkPincodeScreen *) g_object_new(GTK_PINCODE_SCREEN_TYPE, NULL);
    return GTK_WIDGET(screen);
}


GType gtk_pincode_screen_get_type(void)
{
    static GType pincode_screen_type = 0;

    if (!pincode_screen_type)
    {
        static const GTypeInfo pincode_screen_info = {
            sizeof(GtkPincodeScreenClass),
            NULL,               /* base_init */
            NULL,               /* base_finalize */
            (GClassInitFunc) gtk_pincode_screen_class_init,
            NULL,               /* class_finalize */
            NULL,               /* class_data */
            sizeof(GtkPincodeScreen),
            0,                  /* n_preallocs */
            (GInstanceInitFunc) gtk_pincode_screen_init,
        };
        pincode_screen_type = g_type_register_static(GTK_TYPE_EVENT_BOX, "PincodeScreen", 
                    &pincode_screen_info, (GTypeFlags)0);
    }
    return pincode_screen_type;
}

// message widget "Enter the password to open the protected document".
static GtkWidget *create_pincode_message_widget(GtkPincodeScreen * screen)
{
    GtkWidget *messageLabel = NULL;
    
    // messageLabel 
    messageLabel = gtk_label_new(_("Enter the password to open the protected document"));
    gtk_box_pack_start(GTK_BOX(screen->clientArea), messageLabel, FALSE, FALSE, PINCODE_MESSAGE_PADDING);
    gtk_widget_set_name(messageLabel, "pincode_message_title_text");
    gtk_label_set_line_wrap(GTK_LABEL(messageLabel), TRUE);
    gtk_widget_set_size_request(messageLabel, PINCODE_MESSAGE_LABEL_WIDTH, PINCODE_MESSAGE_LABEL_HEIGHT);
    gtk_label_set_justify(GTK_LABEL(messageLabel), GTK_JUSTIFY_CENTER);
    
    return messageLabel;
}

// pin code input: input widget and ok/cancel buttons
static GtkWidget *create_pincode_input_widget(GtkPincodeScreen * screen)
{
    GtkWidget *fixed;

    // fixed
    fixed = gtk_fixed_new();
    gtk_box_pack_start(GTK_BOX(screen->clientArea), fixed, FALSE, FALSE, PINCODE_MESSAGE_PADDING);
    gtk_widget_set_size_request(GTK_WIDGET(fixed), -1, PINCODE_INPUT_HEIGHT);
    
    // input entry
    screen->inputWidget = gtk_entry_new();
    gtk_widget_set_name(GTK_WIDGET(screen->inputWidget), "pin_code_entry");
    gtk_entry_set_visibility(GTK_ENTRY(screen->inputWidget), FALSE);
    gtk_fixed_put(GTK_FIXED(fixed), screen->inputWidget, PINCODE_INPUT_XPOS, PINCODE_INPUT_YPOS);
    gtk_widget_set_size_request(GTK_WIDGET(screen->inputWidget), PINCODE_INPUT_WIDTH, -1);
   
    // ok Button
    screen->okButton = (GtkButton *)gtk_toggle_button_new();
    gtk_fixed_put(GTK_FIXED(fixed), GTK_WIDGET(screen->okButton), PINCODE_BUTTON_XPOS, PINCODE_BUTTON_YPOS);
    gtk_widget_set_size_request(GTK_WIDGET(screen->okButton), PINCODE_BUTTON_WIDTH, -1);
    gtk_button_set_label(GTK_BUTTON(screen->okButton), _("Confirm"));

    return fixed;
}

// error message widget "Password incorrect. Please try again."
static GtkWidget *create_pincode_error_msg_widget(GtkPincodeScreen * screen)
{
    if (count <= 0) 
    {
        screen->errorMsg = 0;
        return NULL;
    } 
           
    GtkWidget *fixed;
    fixed = gtk_fixed_new();
    gtk_box_pack_start(GTK_BOX(screen->clientArea), fixed, FALSE, FALSE, PINCODE_MESSAGE_PADDING);
    gtk_widget_set_size_request(GTK_WIDGET(fixed), -1, PINCODE_ERRORMSG_HEIGHT);

    // error msg label
    screen->errorMsg = gtk_label_new(_("Password incorrect. Please try again."));
    gtk_widget_set_name(screen->errorMsg, "pincode_message_error_text");
    gtk_label_set_line_wrap(GTK_LABEL(screen->errorMsg), TRUE);
    gtk_widget_set_size_request(screen->errorMsg, PINCODE_ERRORMSG_WIDTH, -1);
    gtk_label_set_justify(GTK_LABEL(screen->errorMsg), GTK_JUSTIFY_CENTER);
    gtk_fixed_put(GTK_FIXED(fixed), screen->errorMsg, PINCODE_ERRORMSG_XPOS, PINCODE_ERRORMSG_YPOS);
    return screen->errorMsg;
}


// create gtkPincodeScreen - set the default values 
//
// object hierarchy:
// screen (GtkPincodeScreen background)
//    |--alignment
//       |--clientArea(GtkVBox)
//          |--messageWidget
//          |--inputWidget
static void gtk_pincode_screen_init(GtkPincodeScreen * screen)
{
    // tmp
    GtkWidget *alignment = NULL;
    GtkWidget *clientArea = NULL;      // GtkVBox
    GtkWidget *messageWidget = NULL;   // message widget

    // screen style
    gtk_widget_set_name(GTK_WIDGET(screen), "pincode_screen_background");
    gtk_widget_set_size_request(GTK_WIDGET(screen), PINCODE_SCREEN_WIDTH, PINCODE_SCREEN_HEIGHT);

    // alignment 
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_container_add(GTK_CONTAINER(screen), GTK_WIDGET(alignment));
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment),
                              PINCODE_BG_PADDING_TOP,
                              PINCODE_BG_PADDING_BOTTOM, 
                              PINCODE_BG_PADDING_LEFT, 
                              PINCODE_BG_PADDING_RIGHT);
    
    // background, use event box to set bk image
    GtkWidget * bk = gtk_event_box_new();
    gtk_widget_set_size_request(GTK_WIDGET(bk), PINCODE_BACKGROUND_WIDTH, PINCODE_BACKGROUND_HEIGHT);
    gtk_widget_set_name(bk, "pincode_input_background");
    gtk_container_add(GTK_CONTAINER(alignment), bk);

    // client area
    clientArea = gtk_vbox_new(FALSE, PINCODE_MESSAGE_PADDING);
    gtk_container_add(GTK_CONTAINER(bk), clientArea);
    screen->clientArea = clientArea;

    // messageWidget
    messageWidget = create_pincode_message_widget(screen);
        
    // inputWidget
    create_pincode_input_widget(screen);

    // error message
    create_pincode_error_msg_widget(screen);    

    // signals 
    g_signal_connect(G_OBJECT(screen), "expose_event", 
        G_CALLBACK(GtkAuthDialog::exposeCbk), screen);
    g_signal_connect(G_OBJECT(screen), "destroy", 
        G_CALLBACK(GtkAuthDialog::destroyCbk), screen);

    // input
    g_signal_connect_after(G_OBJECT(screen->inputWidget), "focus-in-event",       
        G_CALLBACK(GtkAuthDialog::focusInCbk), screen);
    g_signal_connect_after(G_OBJECT(screen->inputWidget), "focus-out-event",      
        G_CALLBACK(GtkAuthDialog::focusOutCbk), screen);
    g_signal_connect_after(G_OBJECT(screen->inputWidget), "focus", 
        G_CALLBACK(GtkAuthDialog::focusCbk),  screen);
    g_signal_connect_after(G_OBJECT(screen->inputWidget), "move-cursor", 
        G_CALLBACK(GtkAuthDialog::moveCursorCbk), screen);
    g_signal_connect_after(G_OBJECT(screen->inputWidget), "changed",              
        G_CALLBACK(GtkAuthDialog::changedCbk), screen);

    // button
    g_signal_connect(G_OBJECT(screen->okButton), "clicked", 
        G_CALLBACK(GtkAuthDialog::buttonClickedCbk), screen);
    g_signal_connect(G_OBJECT(screen->okButton), "state-changed", 
        G_CALLBACK(GtkAuthDialog::buttonStateChangedCbk), screen);    

    // show widget 
    gtk_widget_show_all(GTK_WIDGET(screen));
}


// init class
static void gtk_pincode_screen_class_init(GtkPincodeScreenClass * klass)
{
    // nothing needs to be done here 
}

//////////////////////////////////////////////////////////////////////////
// gtk auth dialog
GtkAuthDialog::GtkAuthDialog(void)
: dialog(NULL)
, view(NULL)
, bOk(gFalse)
, bUpdate(gFalse)
{
}

GtkAuthDialog::~GtkAuthDialog(void)
{
}

void GtkAuthDialog::initPincodeDialog()
{
    dialog = (GtkPincodeScreen *)g_object_new(GTK_PINCODE_SCREEN_TYPE, NULL);
    gtk_container_add(GTK_CONTAINER(view->getWindow()), GTK_WIDGET(dialog));
    gtk_widget_show_all(GTK_WIDGET(view->getWindow()));
    dialog->authDialog = this;
}

int GtkAuthDialog::popupAuthDialog(CPDFView * p)
{
    ++count;
    view = p;
    initPincodeDialog();
    gtk_main();
    return bOk;
}

static gboolean updateScreen(gpointer data)
{
    CPDFView * view = (CPDFView *)data;
    XSync(view->ctrl->gtkMgr.display, gTrue);
    view->refreshDeviceScreen(dmQTyping);
    return FALSE;
}

gboolean GtkAuthDialog::exposeCbk(GtkPincodeScreen * dialog,
                                  GdkEventExpose * event, gpointer user_data)
{
    // grab focus, do not refresh screen now. 
    // focusInCbk will be invoked
    dialog->authDialog->bUpdate = gFalse;
    gtk_widget_grab_focus(dialog->inputWidget);

    // popup keyboard
    if (count <= 0)
    {
        dialog->authDialog->view->ctrl->channels.popupKeyboard();
    }
    else
    {
        gtk_idle_add(updateScreen, dialog->authDialog->view);
        
    }        
    dialog->authDialog->view->ctrl->channels.busy_off();
    return FALSE;
}

gboolean GtkAuthDialog::focusInCbk(GtkWidget *widget, 
        GdkEventFocus * event, gpointer data)
{
    GtkPincodeScreen * screen = (GtkPincodeScreen *)data;
    if (!screen->authDialog->bUpdate)
    {
        screen->authDialog->bUpdate = gTrue;
        return FALSE;
    }
    screen->authDialog->view->refreshDeviceScreen(dmQTyping);
    return FALSE;
}

gboolean GtkAuthDialog::focusOutCbk(GtkWidget *widget, 
        GdkEventFocus * event, gpointer data)
{
    GtkPincodeScreen * screen = (GtkPincodeScreen *)data;
    if (!screen->authDialog->bUpdate)
    {
        return FALSE;
    }        
    screen->authDialog->view->refreshDeviceScreen(dmQTyping);
    return FALSE;
}        

gboolean GtkAuthDialog::focusCbk(GtkWidget * widget, 
        GtkDirectionType arg1, gpointer data)
{
    GtkPincodeScreen * screen = (GtkPincodeScreen *)data;
    screen->authDialog->view->refreshDeviceScreen(dmQTyping);
    return FALSE;
}

void GtkAuthDialog::changedCbk(GtkEditable * widget, 
        gpointer data)
{
    GtkPincodeScreen * screen = (GtkPincodeScreen *)data;
    screen->authDialog->view->refreshDeviceScreen(dmQTyping);
    return;
}

void GtkAuthDialog::moveCursorCbk(GtkEntry * widget, 
        GtkMovementStep arg1, gint arg2, gboolean arg3, gpointer data)
{
    GtkPincodeScreen * screen = (GtkPincodeScreen *)data;
    screen->authDialog->view->refreshDeviceScreen(dmQTyping);
    return;
}


void GtkAuthDialog::buttonClickedCbk(GtkToggleButton * button, 
        gpointer user_data)
{
    GtkPincodeScreen * screen = (GtkPincodeScreen *)user_data;
    screen->authDialog->view->ctrl->channels.busy_blink();
    screen->authDialog->bOk = gFalse;
    screen->authDialog->bUpdate = gFalse;
    if (GTK_BUTTON(button) == screen->okButton)
    {
        screen->authDialog->bOk = gTrue;
        memset(screen->authDialog->password, 0, MAX_PASSWORD_LENGTH);
        strncpy(screen->authDialog->password, gtk_entry_get_text(GTK_ENTRY(screen->inputWidget)), MAX_PASSWORD_LENGTH);
    }
    gtk_container_remove(GTK_CONTAINER(screen->authDialog->view->getWindow()), GTK_WIDGET(screen));
    gtk_main_quit();
    return;
}        

void GtkAuthDialog::buttonStateChangedCbk(GtkWidget * button, 
        GtkStateType state, gpointer data)
{
   
    GtkStateType betterstate = GTK_STATE_NORMAL;
    GtkStyle *style = NULL;
    GtkWidget *button_label = NULL;

    g_return_if_fail(GTK_IS_TOGGLE_BUTTON(button));

    button_label = gtk_bin_get_child(GTK_BIN(button));
    style = gtk_widget_get_style(button_label);

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)) || (state == GTK_STATE_ACTIVE))
    {
        betterstate = GTK_STATE_ACTIVE;
    }
    else
    {
        betterstate = GTK_STATE_NORMAL;
    }

    if (style->fg[GTK_STATE_PRELIGHT].red != style->fg[betterstate].red
        || style->fg[GTK_STATE_PRELIGHT].green != style->fg[betterstate].green
        || style->fg[GTK_STATE_PRELIGHT].blue != style->fg[betterstate].blue)
    {
        gtk_widget_modify_fg(button_label, GTK_STATE_PRELIGHT, &(style->fg[betterstate]));
    }
    
    GtkPincodeScreen * screen = (GtkPincodeScreen *)data;    
    screen->authDialog->view->refreshDeviceScreen(dmQTyping);
    return;
}

gboolean GtkAuthDialog::destroyCbk(GtkPincodeScreen * dialog, 
                                   GdkEvent * event, gpointer user_data)
{
    return FALSE;
}



