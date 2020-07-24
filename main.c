/*
Copyright (C) 2019 Oliver Smith

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gdk/gdk.h>
#include <gdk/gdkwayland.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <stdlib.h>

#define DEFAULT_WINDOW_WIDTH 512
#define DEFAULT_WINDOW_HEIGHT 70 * (8 + 1)

static void onclick(GtkWidget *widget, gpointer command)
{
	system((const char *)command);
}

void adjust_window_size(GtkWindow *window)
{
	int width = DEFAULT_WINDOW_WIDTH;
	int height = DEFAULT_WINDOW_HEIGHT;
	GdkDisplay *display = gdk_display_get_default();
	// clang-format off
#ifdef GDK_WINDOWING_WAYLAND
	if (GDK_IS_WAYLAND_DISPLAY(display)) {
		GdkRectangle geometry;
		GdkMonitor *monitor = gdk_display_get_monitor(display, 0);
		gdk_monitor_get_geometry(monitor, &geometry);
		width = (width < geometry.width) ? width : geometry.width;
		height = (height < geometry.height) ? height : geometry.height;
	} else
#endif
#ifdef GDK_WINDOWING_X11
	if (GDK_IS_X11_DISPLAY(display)) {
		GdkScreen *screen = gdk_screen_get_default();
		int max_width = gdk_screen_get_width(screen);
		int max_height = gdk_screen_get_height(screen);
		printf("%d %d", max_width, max_height);
		if (width > max_width)
			width = max_width;

		if (height > max_height)
			height = max_height;
	} else
#endif
		g_error("Unsupported GDK backend");
	// clang-format on

	gtk_window_resize(window, width, height);
}

static void activate(GtkApplication *app, gpointer user_data)
{
	GtkWidget *window = gtk_application_window_new(app);
	GtkWidget *scr_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_window_set_title(GTK_WINDOW(window), "postmarketOS demos");
	gtk_container_add(GTK_CONTAINER(window), scr_window);
	GtkWidget *list_box = gtk_list_box_new();
	gtk_container_add(GTK_CONTAINER(scr_window), list_box);

	const char *programs[] = {
		"GTK3 Demo",
		"gtk3-demo &",
		"Firefox (XWayland, needs to be installed)",
		"firefox &",
		"weston-presentation-shm (Animation)",
		"weston-presentation-shm &",
		"weston-simple-damage (Animation)",
		"weston-simple-damage &",
		"weston-smoke (Touch)",
		"weston-smoke &",
		"weston-editor (Touch)",
		"weston-editor &",
		"htop (Terminal)",
		"weston-terminal -f --shell=/usr/bin/htop &",
		"Restart Weston",
		"killall weston &",
	};

	for (int i = 0; i < (sizeof(programs) / sizeof(const char *)); i += 2) {
		const char *title = programs[i];
		const char *command = programs[i + 1];

		GtkWidget *button = gtk_button_new_with_label(title);
		gtk_widget_set_size_request(button, 200, 70);
		g_signal_connect(button, "clicked", G_CALLBACK(onclick),
				 (void *)command);
		gtk_list_box_insert(GTK_LIST_BOX(list_box), button, -1);
	}
	adjust_window_size(GTK_WINDOW(window));
	gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
	GtkApplication *app = gtk_application_new("org.postmarketos.demos",
						  G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return status;
}
