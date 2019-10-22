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

#include <gtk/gtk.h>
#include <stdlib.h>

static void onclick(GtkWidget *widget, gpointer command)
{
	system((const char *)command);
}

static void activate(GtkApplication *app, gpointer user_data)
{
	GtkWidget *window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "postmarketOS demos");

	GtkWidget *button_box = gtk_button_box_new(
		GTK_ORIENTATION_VERTICAL);
	gtk_container_add(GTK_CONTAINER(window), button_box);

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
		gtk_container_add(GTK_CONTAINER(button_box), button);
	}
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
