#include <gtk/gtk.h>
#include "Skeleton.h"

/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;
static bool in_color=false;
static Skeleton *skl = NULL;

static void
clear_surface ()
{
    cairo_t *cr;

    cr = cairo_create (surface);

    cairo_set_source_rgb (cr, 1, 1, 1);

    cairo_paint(cr);

    cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
    cairo_set_line_width(cr, 0.5);

    double start;
    for (int i=1; i< 20; ++i) {
        start = double(i)*40;
        cairo_move_to(cr, 0, start);
        cairo_line_to(cr, 800, start);
    }

    for (int i=1; i< 20; ++i) {
        start = double(i)*40 + 20;
        cairo_move_to(cr, start, 0);
        cairo_line_to(cr, start, 800);
    }

    cairo_stroke(cr);

    cairo_set_line_width(cr, 2);
    cairo_set_source_rgb(cr, 0, 0, 0);

    if (skl) skl->draw(cr, in_color);

    cairo_stroke(cr);

    cairo_destroy (cr);
}

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean
configure_event_cb (GtkWidget         *widget,
                    GdkEventConfigure *event,
                    gpointer           data)
{
    if (surface)
        cairo_surface_destroy (surface);

    surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                                 CAIRO_CONTENT_COLOR,
                                                 gtk_widget_get_allocated_width (widget),
                                                 gtk_widget_get_allocated_height (widget));

    /* Initialize the surface to white */
    clear_surface ();

    /* We've handled the configure event, no need for further processing. */
    return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean
draw_cb (GtkWidget *widget,
         cairo_t   *cr,
         gpointer   data)
{
    cairo_set_source_surface (cr, surface, 0, 0);

    cairo_paint (cr);

    return FALSE;
}

/* Handle button press events by either drawing a rectangle
 * or clearing the surface, depending on which button was pressed.
 * The ::button-press signal handler receives a GdkEventButton
 * struct which contains this information.
 */
static gboolean
button_press_event_cb (GtkWidget      *widget,
                       GdkEventButton *event,
                       gpointer        data)
{
    /* paranoia check, in case we haven't gotten a configure event */
    if (surface == NULL)
        return FALSE;

    if (event->button == GDK_BUTTON_SECONDARY)
    {
        in_color = !in_color;
        clear_surface ();
        gtk_widget_queue_draw (widget);
    }

    /* We've handled the event, stop processing */
    return true;
}

static void
close_window (void)
{
    if (surface)
        cairo_surface_destroy (surface);

    gtk_main_quit ();
}

void
set_file (GtkFileChooserButton *widget,
               gpointer              user_data)
{

    const char *fname = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(widget));
    Skeleton *x = new Skeleton();
    if (x->load(fname)) {
        if (skl) delete skl;
        skl = x;
        clear_surface();
        gtk_widget_queue_draw(GTK_WIDGET(user_data));
    }
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
    GtkWidget *window;
    GtkWidget *frame;
    GtkWidget *drawing_area;
    GtkWidget *chooser_button;

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Drawing Area");

    g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);

    gtk_container_set_border_width (GTK_CONTAINER (window), 8);

    frame = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
    gtk_container_add (GTK_CONTAINER (window), frame);

    chooser_button = gtk_file_chooser_button_new("Select problem...", GTK_FILE_CHOOSER_ACTION_OPEN);

    drawing_area = gtk_drawing_area_new ();
    /* set a minimum size */
    gtk_widget_set_size_request (drawing_area, 800, 800);

    gtk_paned_add1(GTK_PANED(frame), drawing_area);
    gtk_paned_add2(GTK_PANED(frame), chooser_button);

    /* Signals used to handle the backing surface */
    g_signal_connect (drawing_area, "draw",
                      G_CALLBACK (draw_cb), NULL);
    g_signal_connect (drawing_area,"configure-event",
                      G_CALLBACK (configure_event_cb), NULL);
    g_signal_connect(drawing_area, "button_press_event",
                      G_CALLBACK(button_press_event_cb), NULL);
    g_signal_connect(chooser_button, "file-set",
                         G_CALLBACK(set_file), chooser_button);

    /* Ask to receive events the drawing area doesn't normally
     * subscribe to. In particular, we need to ask for the
     * button press and motion notify events that want to handle.
     */
    gtk_widget_set_events (drawing_area, gtk_widget_get_events (drawing_area)
                                         | GDK_BUTTON_PRESS_MASK
                                         | GDK_POINTER_MOTION_MASK);

    gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
