#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
 #include <stdlib.h>
int main()
{
    srand(time(NULL));
   // printf("child proses içindeyim!!\n");
    int fd,random_number;
    random_number = rand() % 6 + 0;

    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    fd = open(myfifo, O_WRONLY);
    write(fd, &random_number, sizeof(random_number));
    close(fd);


  //  printf("child proses bitti!!\n");
return 0;
}
