#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <poll.h>

#define GPIO 4

int main(int argc, char *argv[])
{
   char str[256];
   struct timeval tv;
   struct pollfd pfd;
   int fd, gpio;
   char buf[8];

   /*
      Prior calls assumed.
      sudo sh -c "echo 4      >/sys/class/gpio/export"
      sudo sh -c "echo in     >/sys/class/gpio/gpio4/direction"
      sudo sh -c "echo rising >/sys/class/gpio/gpio4/edge"
   */

   if (argc > 1) gpio = atoi(argv[1]);
   else          gpio = GPIO;

   sprintf(str, "/sys/class/gpio/gpio%d/value", gpio);

   if ((fd = open(str, O_RDONLY)) < 0)
   {
      fprintf(stderr, "Failed, gpio %d not exported.\n", gpio);
      exit(1);
   }

   pfd.fd = fd;

   pfd.events = POLLPRI;

   lseek(fd, 0, SEEK_SET);    /* consume any prior interrupt */
   read(fd, buf, sizeof buf);

   poll(&pfd, 1, -1);         /* wait for interrupt */

   lseek(fd, 0, SEEK_SET);    /* consume interrupt */
   read(fd, buf, sizeof buf);

   exit(0);
}
