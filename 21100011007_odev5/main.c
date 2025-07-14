

#include <stdio.h>
#include <stdlib.h>

struct Dugum {
    int veri;
    struct Dugum *sol,*sag,*ebeveyn;
};

//yeni dugum
struct Dugum* yeniDugum(int veri,struct Dugum* ebeveyn)
{
    struct Dugum* dugum=(struct Dugum*)malloc(sizeof(struct Dugum));
    dugum->veri=veri;
    dugum->sol=dugum->sag=NULL;
    dugum->ebeveyn=ebeveyn;
    return dugum;
}

//ekleme
struct Dugum* ekle(struct Dugum* kok,int veri)
{
    if (kok == NULL) return yeniDugum(veri,NULL);
    struct Dugum* ebeveyn=NULL;
    struct Dugum* mevcut=kok;

    //travers
    while (mevcut!=NULL)
    {
        ebeveyn=mevcut;
        if (veri < mevcut->veri)
            mevcut=mevcut->sol;
        else if (veri> mevcut->veri)
            mevcut=mevcut->sag;
        else
            return kok;
    }
    struct Dugum* yeni = yeniDugum(veri, ebeveyn);
    if (veri < ebeveyn->veri)
        ebeveyn->sol = yeni;
    else
        ebeveyn->sag = yeni;

    return kok;

}

//arama
int araVeYoluYazdir(struct Dugum* kok,int anahtar)
{
    if (kok == NULL)
    {
        printf("Dugum bulunamadý.\n");
        return 0;
    }
    if (anahtar == kok->veri)
    {
        printf("%d ",kok->veri);
        if (kok->ebeveyn)
            printf("(Ebeveyn: %d)",kok->ebeveyn->veri);
        else
            printf("(Bu dugum kok dugum)");
        printf("\n");
        return 1;
    }

    printf("%d ",kok->veri);
    if (anahtar < kok->veri)
        return araVeYoluYazdir(kok->sol,anahtar);
    else
        return araVeYoluYazdir(kok->sag,anahtar);
}

// min degeri bulma
struct Dugum* enKucukDugum(struct Dugum* dugum)
{
    struct Dugum* mevcut=dugum;
    while (mevcut && mevcut->sol !=NULL)
        mevcut=mevcut->sol;
    return mevcut;
}

//silme
struct Dugum* sil(struct Dugum* kok,int anahtar)
{
    if (kok == NULL) return kok;

    if (anahtar < kok->veri)
        kok->sol=sil(kok->sol, anahtar);
    else if (anahtar > kok->veri)
        kok->sag=sil(kok->sag, anahtar);
    else {
        if (kok->sol == NULL)
        {
            struct Dugum* gecici=kok->sag;
            if (gecici) gecici->ebeveyn = kok->ebeveyn;
            free(kok);
            return gecici;
        } else if (kok->sag == NULL)
        {
            struct Dugum* gecici=kok->sol;
            if (gecici) gecici->ebeveyn = kok->ebeveyn;
            free(kok);
            return gecici;
        }
        struct Dugum* gecici=enKucukDugum(kok->sag);
        kok->veri = gecici->veri;
        kok->sag = sil(kok->sag, gecici->veri);
    }
    return kok;
}

//Parent-Child iliskisini
void parentChildIliskisi(struct Dugum* kok)
{
    if (kok !=NULL)
    {
        if (kok->ebeveyn)
            printf("Düðüm: %d, Ebeveyn: %d\n",kok->veri,kok->ebeveyn->veri);
        else
            printf("Düðüm: %d (Bu kök düðüm)\n",kok->veri);

        if (kok->sol)
            printf("  Sol Çocuk: %d\n",kok->sol->veri);
        if (kok->sag)
            printf("  Sað Çocuk: %d\n",kok->sag->veri);

        parentChildIliskisi(kok->sol);
        parentChildIliskisi(kok->sag);
    }
}

//dosyadan okuma ve olusturma
struct Dugum* dosyadanOku(const char* dosya)
{
    FILE* dosyaPtr=fopen(dosya,"r");
    if (!dosyaPtr)
    {
        printf("Dosya acýlamadý.\n");
        return NULL;
    }
    struct Dugum* kok=NULL;
    int veri;
    while (fscanf(dosyaPtr,"%d",&veri)!= EOF)
    {
        kok=ekle(kok,veri);
    }
    fclose(dosyaPtr);
    return kok;
}

//sirali yazdirma
void siraliYazdir(struct Dugum* kok)
{
    if (kok != NULL)
    {
        siraliYazdir(kok->sol);
        printf("%d ",kok->veri);
        siraliYazdir(kok->sag);
    }
}

int main()
{
    struct Dugum* kok=dosyadanOku("veriler.txt");
    if (kok == NULL) return 1;

    printf("Agac sirali yazdiriliyor: ");
    siraliYazdir(kok);
    printf("\n");

    int secim,deger;
    while(1)
    {
        printf("\n1.Ekleme\n2.Arama\n3.Silme\n4.CIKIS\nSecim: ");
        scanf("%d", &secim);

        switch (secim)
        {
            case 1:
                printf("Eklenecek degeri girin: ");
                scanf("%d", &deger);
                kok = ekle(kok, deger);
                printf("Guncel agac: ");
                siraliYazdir(kok);
                printf("\n");
                break;
            case 2:
                printf("Aranacak degeri girin: ");
                scanf("%d", &deger);
                printf("Arama yolu: ");
                araVeYoluYazdir(kok, deger);
                break;
            case 3:
                printf("Silinecek degeri girin: ");
                scanf("%d", &deger);
                kok = sil(kok, deger);
                printf("Guncellenmis agac: ");
                siraliYazdir(kok);
                printf("\n");
                break;
            case 4:
                printf("CIKIS yapiliyor.\n");
                return 0;
                break;
            default:
                printf("Gecersiz secim.\n");
        }

    }

    return 0;
}
