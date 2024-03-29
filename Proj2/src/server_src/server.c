#include "server.h"

void init(int argc, char* argv[], Settings* settings) {
    if (parseCMDArgs(argc, argv, settings))
        exit(1);

    if (mkfifo(settings->fifoname, FIFO_PUBLIC_PERMS) && errno != EEXIST) {
        fprintf(stderr, "[server] Error opening fifo\n");
        exit(2);
    }
    settings->fd = -1;
}

void exitProgram(Settings* settings) {
    close(settings->fd);
    unlink(settings->fifoname);
}

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 6) {
        fprintf(stderr, "%s%s", "Wrong number of arguments\n",
        "Call with s <-t nsecs> [-l bufsz] <fifoname>\n");
        exit(1);
    }

    Settings settings;
    init(argc, argv, &settings);

    listenAndRespond(&settings);

    return 0;
}
