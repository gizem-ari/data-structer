// 21100011007 Fatma Gizem ARI veri yapilari Odev1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Ders
{
    char ders_Ad[10];
    int vize_not,final_not;
};
struct Ogrenci
{
    char Ad[15], Soyad[15];
    int Numara;
    struct Ders* ders;
    struct Ogrenci* sonraki;
};

struct Ogrenci* BagliListe_Olustur(char* Ad,char* Soyad,char* ders_Ad,int Numara,int vize_not,int final_not)
{
    struct Ogrenci* yeni_ogrenci=(struct Ogrenci*)malloc(sizeof(struct Ogrenci));
    strcpy(yeni_ogrenci->Ad, Ad);
    strcpy(yeni_ogrenci->Soyad, Soyad);
    yeni_ogrenci->Numara=Numara;

    yeni_ogrenci->ders=(struct Ders*)malloc(sizeof(struct Ders));
    strcpy(yeni_ogrenci->ders->ders_Ad, ders_Ad);
    yeni_ogrenci->ders->vize_not=vize_not;
    yeni_ogrenci->ders->final_not=final_not;

    yeni_ogrenci->sonraki=NULL;

    return yeni_ogrenci;
}
void BagliListe_yazdir(struct Ogrenci* n)
{
    struct Ogrenci* temp=n;
    while(temp!=NULL)
    {
        printf("%s  %s  %d ",temp->Ad,temp->Soyad,temp->Numara);
        printf("%s, Vize: %d, Final: %d\n", temp->ders->ders_Ad, temp->ders->vize_not, temp->ders->final_not);
        temp=temp->sonraki;
    }
}

void OgrenciEkleme(struct Ogrenci** bagliListe,char*Ad,char*Soyad,int Numara,char* ders_Ad,int vize_not,int final_not)
{
    struct Ogrenci* yeni_ogrenci=BagliListe_Olustur(Ad,Soyad,ders_Ad,Numara,vize_not,final_not);


    if (*bagliListe == NULL)
    {       // liste bos, yeni ogr ilk dugum olur
        *bagliListe=yeni_ogrenci;
        return;
    }
    struct Ogrenci* temp=*bagliListe;
    while(temp->sonraki!=NULL)
    {
        temp=temp->sonraki;
    }
    temp->sonraki=yeni_ogrenci;

    BagliListe_Sirala(bagliListe);
}

void DerseGoreListele(struct Ogrenci* root,const char* ders_adi)
{
    struct Ogrenci* temp=root;
    printf("Dersi alan ogrenciler:\n");
    while (temp != NULL)
    {
        if (strcmp(temp->ders->ders_Ad,ders_adi) == 0)
        {
            printf("%s %s %d, Vize: %d, Final: %d\n",temp->Ad,temp->Soyad,temp->Numara,temp->ders->vize_not,temp->ders->final_not);
        }
        temp=temp->sonraki;
    }
}


void OgrenciyeGoreListele(struct Ogrenci* root,int numara)
{
    struct Ogrenci* temp=root;
    printf("ogrenci bilgileri:\n");
    while (temp != NULL)
    {
        if (temp->Numara == numara)
        {
            printf("Ad: %s, Soyad: %s, Numara: %d\n",temp->Ad,temp->Soyad,temp->Numara);
            printf("Ders: %s, Vize: %d, Final: %d\n",temp->ders->ders_Ad,temp->ders->vize_not,temp->ders->final_not);
            return;
        }
        temp=temp->sonraki;
    }
    printf("ogrenci bulunamadi.\n");
}
void BagliListe_Sirala(struct Ogrenci** bagliListe)
{
    if (*bagliListe == NULL) return; // bos liste

    struct Ogrenci* temp1=*bagliListe;
    struct Ogrenci* temp2;
    struct Ogrenci* sortedList=NULL;

    while (temp1 != NULL)
    {
        temp2=temp1->sonraki;
        // yeni dugum
        struct Ogrenci* yeniDugum=BagliListe_Olustur(temp1->Ad,temp1->Soyad,temp1->ders->ders_Ad,temp1->Numara,temp1->ders->vize_not,temp1->ders->final_not);

        // sirali listeye ekleme
        if (sortedList == NULL || strcmp(sortedList->Soyad,yeniDugum->Soyad) > 0)
        {
            yeniDugum->sonraki=sortedList;
            sortedList=yeniDugum;
        }
        else
        {
            struct Ogrenci* current=sortedList;
            while (current->sonraki!= NULL && strcmp(current->sonraki->Soyad,yeniDugum->Soyad) < 0)
            {
                current=current->sonraki;
            }
            yeniDugum->sonraki=current->sonraki;
            current->sonraki=yeniDugum;
        }

        temp1=temp2;
    }

    //listeyi dondur
    *bagliListe=sortedList;
}

void OgrenciSoyadinaGoreSil(struct Ogrenci** bagliListe,const char* soyad)
{
    struct Ogrenci* temp=*bagliListe;
    struct Ogrenci* prev=NULL;

    while (temp != NULL)
    {
        if (strcmp(temp->Soyad, soyad) == 0)
        {
            if (prev == NULL)   // Silinecek olan ilk eleman
            {
                *bagliListe=temp->sonraki;
            }
            else
            {
                prev->sonraki=temp->sonraki;
            }
            struct Ogrenci* silinecek=temp;
            temp=temp->sonraki;
            free(silinecek->ders); // bellek serbesti
            free(silinecek);
        }
        else
        {
            prev=temp;
            temp=temp->sonraki;
        }
    }
}
int main()
{
    FILE *dosya=fopen("Bilgiler.txt","r");

    if(dosya== NULL)
    {
        printf("Dosya acilamiyor\n");
        return 1;
    }

    struct Ogrenci* root=NULL;

    char satir[150];
    printf("Tum bilgiler:\n");

    while (fgets(satir, sizeof(satir), dosya) != NULL)
    {
        printf("%s", satir); //okunan satiri yazdirdim bilgileri onden gormek icin
    }
    while (fgets(satir,sizeof(satir),dosya) != NULL)
    {
        char Ad[20], Soyad[20], ders_Ad[20];
        int Numara, vize_not, final_not;

        char* token=strtok(satir,",");
        strcpy(Ad,token);
        token=strtok(NULL,",");
        strcpy(Soyad, token);
        token=strtok(NULL,"-");
        Numara=atoi(token);

        token=strtok(NULL, "-");  // ilk ders
        while (token != NULL)
        {
            sscanf(token, "%[^:]:%d:%d",ders_Ad,&vize_not,&final_not);


            OgrenciEkleme(&root,Ad,Soyad,Numara,ders_Ad,vize_not,final_not);

            // Sonraki dersi al
            token=strtok(NULL, "-");
        }
    }
    fclose(dosya);


    int secim;
    while(1)
    {
        printf("\n1.Ogrenci Ekleme\n2.Derse gore Listele\n3.Ogrenciye gore Listele \n4.Siralama\n5.Silme\n6)CIKIS\n");

        printf("islem seciniz: ");
        scanf("%d",&secim);
        switch(secim)
        {
        case 1:
        {
            char Ad[20], Soyad[20], ders_Ad[20];
            int Numara, vize_not, final_not;
            printf("Ad: ");
            scanf("%s", Ad);
            printf("Soyad: ");
            scanf("%s", Soyad);
            printf("Numara: ");
            scanf("%d", &Numara);
            printf("Ders Ad: ");
            scanf("%s", ders_Ad);
            printf("Vize Notu: ");
            scanf("%d", &vize_not);
            printf("Final Notu: ");
            scanf("%d", &final_not);
            OgrenciEkleme(&root, Ad, Soyad, Numara, ders_Ad, vize_not, final_not);
            break;
        }


        case 2:
        {
            char ders_adi[10];
            printf("Ders adi girin: ");
            scanf("%s", ders_adi);
            DerseGoreListele(root, ders_adi);
            break;
        }

        case 3:
        {
            int numara;
            printf("ogrenci numarasi girin: ");
            scanf("%d", &numara);
            OgrenciyeGoreListele(root, numara);
            break;
        }

        case 4:
        {
            BagliListe_Sirala(&root);
            printf("ogrenciler numaralara gore siralandi.\n");
            break;
        }

        case 5:
        {
            char soyad[15];
            printf("Silmek istediginiz soyadi girin: ");
            scanf("%s", soyad);
            OgrenciSoyadinaGoreSil(&root, soyad);
            printf("%s soyadina sahip ogrenciler silindi.\n", soyad);
            break;
        }

        case 6:
            exit(0);
            break;

        default:
            printf("Gecersiz secim, lütfen tekrar deneyin.\n");
            break;
        }

    }


    return 0;
}
