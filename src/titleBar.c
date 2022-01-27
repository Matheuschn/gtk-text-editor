#include "textView.h"
#include <gtk/gtk.h>

char *OPEN_FILE_PATH = "";

static void onFileChooserResponse(GtkNativeDialog *fileChooser, int response,
                                  GtkWidget *window) {
    GtkFileChooserAction action =
        gtk_file_chooser_get_action(GTK_FILE_CHOOSER(fileChooser));

    if (response == GTK_RESPONSE_ACCEPT) {
        GFile *file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(fileChooser));

        if (action == GTK_FILE_CHOOSER_ACTION_SAVE) {
            saveFile(file);
        } else if (action == GTK_FILE_CHOOSER_ACTION_OPEN) {
            openFile(file);
        }
        gtk_window_set_title(GTK_WINDOW(window), g_file_get_basename(file));

        OPEN_FILE_PATH = g_file_get_path(file);

        g_object_unref(file);
    }

    g_object_unref(fileChooser);
}

static void saveButtonClicked(GtkWidget *saveButton, GtkWidget *window) {
    if (OPEN_FILE_PATH[0]) {
        saveFile(g_file_new_for_path(OPEN_FILE_PATH));

        return;
    }

    GtkFileChooserNative *fileChooser = gtk_file_chooser_native_new(
        "Save File", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE, "Save",
        "Cancel");

    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(fileChooser),
                                      "Untitled Document");

    g_signal_connect(fileChooser, "response", G_CALLBACK(onFileChooserResponse),
                     window);

    gtk_native_dialog_show(GTK_NATIVE_DIALOG(fileChooser));
}

static void openButtonClicked(GtkWidget *saveButton, GtkWidget *window) {
    GtkFileChooserNative *fileChooser = gtk_file_chooser_native_new(
        "Open File", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, "Open",
        "Cancel");

    g_signal_connect(fileChooser, "response", G_CALLBACK(onFileChooserResponse),
                     window);

    gtk_native_dialog_show(GTK_NATIVE_DIALOG(fileChooser));
}

void buildTitleBar(GtkWidget *window) {
    GtkWidget *headerBar = gtk_header_bar_new();
    GtkWidget *saveButton = gtk_button_new_with_label("Save");
    g_signal_connect(saveButton, "clicked", G_CALLBACK(saveButtonClicked),
                     window);

    GtkWidget *saveMenu = gtk_menu_button_new();

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_prepend(GTK_BOX(box), saveButton);
    gtk_box_append(GTK_BOX(box), saveMenu);

    GtkStyleContext *context = gtk_widget_get_style_context(box);
    gtk_style_context_add_class(context, "linked");

    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), box);

    GtkWidget *openButton = gtk_button_new_with_label("Open");
    g_signal_connect(openButton, "clicked", G_CALLBACK(openButtonClicked),
                     window);

    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), openButton);

    gtk_window_set_titlebar(GTK_WINDOW(window), headerBar);
}
