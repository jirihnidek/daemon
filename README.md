# Example of Linux Daemon

This repository contains simple example of daemon for Linux OS.
This repository also contains examples of starting scripts.

## Requirements

To build example of the daemon you have to have following tools

* CMake
* GCC/CLang

## Build

To build example of daemon you have to type following commands:

    git clone https://github.com/jirihnidek/daemon.git
    cd daemon
    mkdir build
    cd build
    cmake ../
    make

## Usage

You can test running daemon from command line:

    ./bin/daemon

But running the app in this way is not running running daemon. Let
have a look at command line parameters and arguments

    Usage: ./bin/daemon [OPTIONS]

     Options:
      -h --help                 Print this help
      -c --conf_file filename   Read configuration from the file
      -t --test_conf filename   Test configuration file
      -l --log_file  filename   Write logs to the file
      -d --daemon               Daemonize this application
      -p --pid_file  filename   PID file used by daemonized app

When you will run `./bin/daemon` with parameter `--daemon` or `-d`, then
it will become real UNIX daemon.