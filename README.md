# Example of Linux Daemon

This repository contains simple example of daemon for Linux OS.
This repository also contains examples of starting scripts.

When you want to create super simple daemon, then it is very easy.
You can write something like this in C and call it `daemon.c`:

```c
/* Compile this with gcc -o daemon daemon.c */

#include <unistd.h>

int main(void)
{
    while(1) {
        /* TODO: do something usefull here ;-) */
        sleep(1);
    }
}
```

and write some super simple systemd service file called `simple-daemon.service`:

    [Unit]
    Description=Super simple daemon

    [Service]
    Type=simple
    ExecStart=/usr/bin/daemon

    [Install]
    WantedBy=multi-user.target

and then you can run it as UNIX daemon, but such daemon do not have some
nice features like reloadin configure files, loging, etc. This repository
and sources can help you to understand how UNIX daemons works. 

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
    cmake -DCMAKE_INSTALL_PREFIX=/usr ../
    make
    sudo make install

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
it will become real UNIX daemon. But this is not the way, how UNIX daemons
are started nowdays. Some init scripts or service files are used for
this purpose.

When you use Linux distribution using systemd, then you can try start daemon using

    systemctl start simple-daemon
    systemctl status simple-daemon
    systemctl reload simple-daemon
    systemctl stop simple-daemon

> Note: The unit files `simple-daemon.service` and `forking-daemon.service` 
are copied to the directory `/usr/lib/systemd/system` during installation using
`make install` command.

When you use RedHat 4/5/6 or CentOS, then you can try to use init script:

    cp daemon.init /etc/rc.d/init.d/daemond

Then it should be possible to control daemon using:

    service daemon start
    service daemon status
    service daemon reload
    service daemon stop
