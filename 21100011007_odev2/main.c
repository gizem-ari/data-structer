//Fatma Gizem ARI 211000110007 Veri Odev2

#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    char data [15];
    int adet;
    struct Node *next;
    struct Node *prev;
} node;

node * start=NULL;
node * prev=NULL;
node * temp= NULL;
node * temp2=NULL;

int syc=0;

void Ekle(char *kelime)
{
    if(start==NULL)
    {
        node *eleman=(node*)malloc(sizeof(node));
        strcpy(eleman->data,kelime);
        start=eleman;
        start->next=start;
        start->prev=start;
        eleman->adet=1;
        syc=1;
    }
    else
    {
        node *temp=start;
        do
        {
            // kelime listede var adet++
            if(strcmp(temp->data,kelime)==0)
            {
                temp->adet++;
                return;
            }
            temp=temp->next;
        }
        while(temp!=start);

        //kelime listede yok
        node *eleman=(node*)malloc(sizeof(node));
        strcpy(eleman->data,kelime);
        eleman->adet=1;
        eleman->next=start;
        eleman->prev=start->prev;
        start->prev->next=eleman;
        start->prev=eleman;

        syc++;
    }
}

void listele()
{
    if(start==NULL)
    {
        printf("liste bos!\n");
    }
    else
    {
        temp=start;
        while(temp->next !=start)
        {
            printf("%s(%d) -> ", temp->data, temp->adet);
            temp=temp->next;
        }
        printf("%s(%d)\n", temp->data,temp->adet);//son eleman
    }
}

void sil(char * silinecek)
{
    temp=start;
    if(start==NULL)
    {
        printf("liste bos,silinemez\n");
    }
    int kelimebulundu=0;
    node *onceki=NULL;

    do
    {
        if(strncmp(temp->data,silinecek,strlen(silinecek))==0)
        {
            kelimebulundu=1;
            onceki=temp->prev;
            if (temp == start && temp->next == start)
            {
                free(temp);
                start=NULL;
                return;
            }
            onceki->next=temp->next;
            temp->next->prev=onceki;

            node *silinen=temp;
            temp=temp->next;
            free(silinen);
        }
        else
        {
            temp=temp->next;

        }
    }
    while (temp != start);

    if (!kelimebulundu)
    {
        printf("Kelime bulunamadi\n");
    }
    else
    {
        printf("\nSilme islemi basarili!\n");
    }
}

void YogunlugaGoreSirala()
{
    if (start == NULL) {
        printf("Liste bos!\n");
        return;
    }

    node *temp1=start;
    node *temp2=NULL;

    do {
        temp2=temp1->next;
        while (temp2 != start) {
            if (temp1->adet < temp2->adet) {
                // Adete göre degisim
                char tempData[15];
                int tempAdet=temp1->adet;

                strcpy(tempData,temp1->data);
                temp1->adet=temp2->adet;
                strcpy(temp1->data,temp2->data);

                temp2->adet=tempAdet;
                strcpy(temp2->data,tempData);
            }
            temp2=temp2->next;
        }
        temp1=temp1->next;
    } while (temp1 != start);

    printf("\nKelimeler yogunluga gore siralandi:\n");
    listele();
}
void AlfabeyeGoreSirala()
{
    if (start == NULL) {
        printf("Liste bos!\n");
        return;
    }

    node *temp1 = start;
    node *temp2 = NULL;

    do {
        temp2 = temp1->next;
        while (temp2 != start) {
            if (strcmp(temp1->data, temp2->data) > 0) { // Alfabetik sıraya göre yanlış yerdeyse
                // Kelimeyi ve adetlerini değiştir
                char tempData[15];
                int tempAdet = temp1->adet;

                strcpy(tempData, temp1->data);
                strcpy(temp1->data, temp2->data);
                temp1->adet = temp2->adet;

                strcpy(temp2->data, tempData);
                temp2->adet = tempAdet;
            }
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    } while (temp1 != start);

    printf("\nKelimeler alfabeye gore siralandi:\n");
    listele();
}

int main()
{
    char kelime[15];

    FILE *dosya=fopen("metin.txt","r");

    if(dosya== NULL)
    {
        printf("Dosya acilamiyor\n");
        return 1;
    }

    char satir[256];
    const char *ayraclar=",.-\n";

    printf("metindeki kelimeler:\n");
    while(fscanf(dosya, "%14s", kelime) == 1)
    {
        char *token = strtok(kelime, ayraclar);

        while(token!=NULL)
        {
            printf("%s ", kelime);
            Ekle(token);
            token=strtok(NULL,ayraclar);
        }
    }
    fclose(dosya);



    int secim,sayi;

    while(1)
    {
        printf("\n\n1)Yogunluga gore sirala \n2)Alfabeye gore sirala \n3)Sil \n4)Tumunu listele");
        printf("\nsecim:");
        scanf("%d",&secim);

        switch (secim)
        {
        case 1:
            YogunlugaGoreSirala();
            break;

        case 2:
            AlfabeyeGoreSirala();
            break;

        case 3:
            printf("silinecek kelime:");
            scanf("%s",&kelime);
            sil(kelime);
            break;
        case 4:
            listele();
            break;
        default:
            printf("Gecersiz secim!\n");
            break;
        }

    }

    return 0;
}
