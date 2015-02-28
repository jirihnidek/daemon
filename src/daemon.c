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
#include <string.h>

static int running = 0;
static int delay = 1;
static int counter = 0;
static char *conf_file_name = NULL;
static char *app_name = NULL;
static FILE *log_stream;

/**
 * \brief Read configuration from config file
 */
int read_conf_file(void)
{
	FILE *conf_file = NULL;
	int ret = -1;

	if (conf_file_name == NULL) return 0;

	conf_file = fopen(conf_file_name, "r");

	if(conf_file == NULL) return 0;

	ret = fscanf(conf_file, "%d", &delay);

	if(ret > 0) {
		syslog(LOG_INFO, "Reloaded configuration file %s of %s",
			conf_file_name,
			app_name);
	}

	fclose(conf_file);

	return ret;
}

/**
 * \brief This function tries to test config file
 */
int test_conf_file(char *_conf_file_name)
{
	FILE *conf_file = NULL;
	int ret = -1;

	conf_file = fopen(_conf_file_name, "r");

	if(conf_file == NULL) {
		fprintf(stderr, "Can't read config file %s\n",
			_conf_file_name);
		return EXIT_FAILURE;
	}

	ret = fscanf(conf_file, "%d", &delay);

	if(ret <= 0) {
		fprintf(stderr, "Wrong config file %s\n",
			_conf_file_name);
	}

	fclose(conf_file);

	if(ret > 0)
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

/**
 * \brief Callback function for handling signals.
 * \param	sig	identifier of signal
 */
void vs_handle_signal(int sig)
{
	if(sig == SIGINT) {
		fprintf(log_stream, "Debug: stopping daemon ...\n");
		running = 0;
		/* Reset signal handling to default behavior */
		signal(SIGINT, SIG_DFL);
	} else if(sig == SIGHUP) {
		fprintf(log_stream, "Debug: reloading daemon config file ...\n");
		read_conf_file();
	}
}

/**
 * \brief Print help for this application
 */
void print_help(void)
{
	printf("\n Usage: %s [OPTIONS]\n\n", app_name);
	printf("  Options:\n");
	printf("   -h --help               Print this help\n");
	printf("   -c --confile filename   Read configuration from the file\n");
	printf("   -t --testconf filename  Test configuration file\n");
	printf("   -l --logfile filename   Write logs to the file\n");
	printf("\n");
}

/* Main function */
int main(int argc, char *argv[])
{
	static struct option long_options[] = {
		{"confile", required_argument, 0, 'c'},
		{"testconf", required_argument, 0, 't'},
		{"logfile", required_argument, 0, 'l'},
		{"help", no_argument, 0, 'h'},
		{NULL, 0, 0, 0}
	};
	int value, option_index = 0;
	char *log_file_name = NULL;

	app_name = argv[0];

	/* Try to process all command line arguments */
	while( (value = getopt_long(argc, argv, "c:l:t:h", long_options, &option_index)) != -1) {
		switch(value) {
			case 'c':
				conf_file_name = strdup(optarg);
				break;
			case 't':
				return test_conf_file(optarg);
			case 'l':
				log_file_name = strdup(optarg);
				break;
			case 'h':
				print_help();
				return EXIT_SUCCESS;
			case '?':
				print_help();
				return EXIT_FAILURE;
			default:
				break;
		}
	}

	/* Daemon will handle two signals */
	signal(SIGINT, vs_handle_signal);
	signal(SIGHUP, vs_handle_signal);

	/* Open system log and write message to it */
	openlog(argv[0], LOG_PID|LOG_CONS, LOG_USER);
	syslog(LOG_INFO, "Started %s", app_name);

	/* Try to open log file to this daemon */
	if(log_file_name != NULL) {
		log_stream = fopen(log_file_name, "a+");
		if (log_stream == NULL)
		{
			log_stream = stdout;
		}
	} else {
		log_stream = stdout;
	}

	/* Read configuration from config file */
	read_conf_file();

	/* This global variable can be changed in function handling signal */
	running = 1;

	/* Never ending loop of server */
	while(running == 1) {
		/* Debug print */
		fprintf(log_stream, "Debug: %d\n", counter++);

		/* TODO: dome something useful here */

		/* Real server should use select() or poll() for waiting at
		 * asynchronous event. Note: sleep() is interrupted, when
		 * signal is received. */
		sleep(delay);
	}

	/* Close log file, when it is used. */
	if (log_stream != stdout)
	{
		fclose(log_stream);
	}

	/* Write system log and close it. */
	syslog(LOG_INFO, "Stopped %s", app_name);
	closelog();

	/* Free allocated memory */
	if(conf_file_name != NULL) free(conf_file_name);
	if(log_file_name != NULL) free(log_file_name);

	return EXIT_SUCCESS;
}
