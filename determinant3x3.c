#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
    char newTransactionControl;
    int pipeRead, pipeWrite;
    int satSucSecVal;
    ssize_t bufsize = 32;
    char *input = NULL;
    char *array[9] = {" "," "," "," "," "," "," "," "," "};
    int matris [3][3];
    int smallMatrix[2][2];
    char * word;
    int frk,execvReturn;

    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    
 //----------------------------------------  
 while(1){
    printf("matrisi girin>>");
    getline(&input, &bufsize, stdin);
    
    word= strtok(input," ");
    int i = 0;
        
    while(word != NULL){
            array[i]= word;
            i++; 
            word= strtok(NULL," ");
    }
    
//-----------------------------------bu kısım kullanıcıdan matris değerlerini alıyor

    int temp = 0;
    for (int row = 0; row<3; row++){// burada array dizisinin içindeki elemanları matrise atıyorum
        for (int col = 0; col<3; col++){
            matris[row][col] = atoi(array[temp]);
            temp++;
        }
    }
 
    printf("matris:\n");//matrisi yazdır
     for (int row = 0; row<3; row++){
        for (int col = 0; col<3; col++){
            printf("%d ",matris[row][col]);
        }
        printf("\n");
    }
//fork işlemi başlıyor
    frk = fork();
    if(frk == 0){
        execvReturn = execv("SatSutSec",NULL);
       
    }
    else{// parent buradan devam ediyor

        pipeRead = open(myfifo, O_RDONLY);
        read(pipeRead, &satSucSecVal, sizeof(satSucSecVal));
         close(pipeRead);
        printf("satsucsec: %d\n",satSucSecVal);
        wait(&execvReturn);//wait i piperead in üstüne alınca program donuyor Neden? sor
    }


// BURADAN İTİBAREN RASTGELE SATIR VEYA SUTUN SECİLİP YENİ MATRİS OLUSTURULUYIR VE DİGER PROG. FORK EDİLİYOR.
  int totalResult = 0;

    if(satSucSecVal < 3){// deger 3 ten kücükse satir seciyorum deger 0 = 0.sutun. Deger büyükse sutundan secim yapacagım deger 3 = 0.sutun deger 4 = 1.sutun vs..
        int currentValCol = 0;//kolonu belirtiyor bir satirda 3 eleman oldugu icin currentval degeri artacak. Bu ksımı for içerisine sokup diger prog. pipe edecegim.
      
        for(int i = 0; i<3; i++){//seçilen satırın elemanlarını dolaşmak için

          int smRow = 0, smCol = 0;
          for (int row = 0; row<3; row++){
              if(row == satSucSecVal) continue;
              for (int col = 0; col<3; col++){
                     if(col == currentValCol) continue;
                        smallMatrix[smRow][smCol] = matris[row][col];
                        smCol++;
               }//col for
               smRow++;
               smCol = 0;   
           }// row for    

           int gonderilenDegerler[6] = {satSucSecVal+1,currentValCol+1,smallMatrix[0][0],smallMatrix[0][1],smallMatrix[1][0],smallMatrix[1][1]};

            //fork işlemi başlıyor
           frk = fork();
           if(frk == 0){
              execvReturn = execv("KofakHesap2x2",NULL);
            }
           else{// parent buradan devam ediyor

                pipeWrite = open(myfifo, O_WRONLY);
                write(pipeWrite, &gonderilenDegerler, sizeof(gonderilenDegerler));
                close(pipeWrite);

                int temp= 0;
                pipeRead = open(myfifo, O_RDONLY);
                read(pipeRead, &temp, sizeof(temp));
                close(pipeRead);
                temp *= matris[satSucSecVal][currentValCol];//kofaktör formülü işlemi aij X Aij
                totalResult += temp;
               
                wait(&execvReturn);//wait i piperead in üstüne alınca program donuyor Neden? sor
    }

           
         
           
            
           currentValCol++; 
        }//seçilen satırın elemanlarını dolaşmak için
    }
    else{// rastgele sütun seçimi için
        if(satSucSecVal == 3)satSucSecVal = 0;// 0.kolon
        if(satSucSecVal == 4)satSucSecVal = 1;// 1.kolon
        if(satSucSecVal == 5)satSucSecVal = 2;// 2.kolon
        int currentValRow = 0;// bunun döngüye girerek diğer elemanlar için artması lazım.

        for(int i = 0; i<3; i++){//seçilen sütunun elemanların dolaşmak için

            int smRow = 0, smCol = 0;
            for (int row = 0; row<3; row++){
                 if(row == currentValRow) continue;
                 for (int col = 0; col<3; col++){
                     if(col == satSucSecVal) continue;
                     smallMatrix[smRow][smCol] = matris[row][col];
                     smCol++;
                  }//col for
                  smRow++;
                  smCol = 0;   
              }// row for
              int gonderilenDegerler[6] = {currentValRow+1,satSucSecVal+1,smallMatrix[0][0],smallMatrix[0][1],smallMatrix[1][0],smallMatrix[1][1]};
   //fork işlemi başlıyor
           frk = fork();
           if(frk == 0){
              execvReturn = execv("KofakHesap2x2",NULL);
              
            }
           else{// parent buradan devam ediyor

                pipeWrite = open(myfifo, O_WRONLY);
                write(pipeWrite, &gonderilenDegerler, sizeof(gonderilenDegerler));
                close(pipeWrite);

                int temp= 0;
                pipeRead = open(myfifo, O_RDONLY);
                read(pipeRead, &temp, sizeof(temp));
                close(pipeRead);
                temp *= matris[currentValRow][satSucSecVal];//kofaktör formülü işlemi aij X Aij
                totalResult += temp;
              
                wait(&execvReturn);//wait i piperead in üstüne alınca program donuyor Neden? sor
    }

            currentValRow++;
        }//seçilen sütunun elemanların dolaşmak için    
    }
    
    printf("Cevap: %d\n",totalResult);
    printf("Yeni bir işlem yapmak istiyor musunuz?(E/H):");
    newTransactionControl = getchar();
    if(newTransactionControl == 'H' || newTransactionControl == 'h') break;
    system("clear");
    getchar(); 

 }
return 0;
}
