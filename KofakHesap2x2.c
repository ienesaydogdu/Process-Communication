#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
 #include <stdlib.h>
#include <math.h>
int main()
{
    int pipeRead,pipeWrite;
    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    int degerler[6];
    
    pipeRead = open(myfifo,O_RDONLY);
    read(pipeRead, &degerler, sizeof(degerler));
    close(pipeRead); 

    printf("KofakHesap2x2 gelen degerler:\n");//matrisi yazdır
     for (int row = 0; row<6; row++){
        printf("%d ",degerler[row]);
    }   
    printf("\n");

    double ust = degerler[0] + degerler[1];
    double usluSayi = pow(-1.0, ust);
    int determinantSonuc = (degerler[2]*degerler[5]) - (degerler[3]*degerler[4]);
    double toplamSonuc = usluSayi * determinantSonuc;
    int b = (int) toplamSonuc;
    printf("KofakHesap2x2 Sonuç: %d----------------------------\n",b);

    pipeWrite = open(myfifo,O_WRONLY);
    write(pipeWrite, &b, sizeof(b));    
    close(pipeWrite);
return 0;
}
