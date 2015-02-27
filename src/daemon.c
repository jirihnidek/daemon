/*
 *
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * ***** END GPL LICENSE BLOCK *****
 *
 * Contributor(s): Jiri Hnidek <jiri.hnidek@tul.cz>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <getopt.h>

static int running = 0;
static int delay = 1;
static int counter = 0;

/**
 * \brief Callback function for handling signals.
 * \param	sig	identifier of signal
 */
void vs_handle_signal(int sig)
{
	if(sig == SIGINT) {
		running = 0;

		/* Reset signal handling to default behavior */
		signal(SIGINT, SIG_DFL);
	}
}

int main(int argc, char *argv[])
{
	static struct option long_options[] = {
		{"confile", required_argument, 0, 'c'},
		{NULL, 0, 0, 0}
	};
	int value, option_index = 0;

	while( (value = getopt_long(argc, argv, "c:", long_options, &option_index)) != -1) {
		switch(value) {
			case 'c':
				printf("option -c %s\n", optarg);
				break;
			case '?':
				break;
			default:
				break;
		}
	}

	running = 1;

	openlog(argv[0], LOG_PID|LOG_CONS, LOG_USER);
	signal(SIGINT, vs_handle_signal);

	syslog(LOG_INFO, "Started %s", argv[0]);
	while(running == 1) {
		fprintf(stdout, "Debug: %d\n", counter++);
		sleep(delay);
	}

	syslog(LOG_INFO, "Stoped %s", argv[0]);
	closelog();

	return EXIT_SUCCESS;
}
