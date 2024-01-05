#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int fd = open("/tmp/file", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(fd >= 0);
    char buffer[20];
    sprintf(buffer, "hello world\n");
    // write hello world to fd
    int rc = write(fd, buffer, strlen(buffer));
    assert(rc == (strlen(buffer)));
    // make all changes to fd appear on disk
    fsync(fd);
    close(fd);
    return 0;
}
