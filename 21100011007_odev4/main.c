// 211000110007 Fatma Gizem ARI - Odev 4

#include <stdio.h>
#include <stdlib.h>

#define URL_LENGTH 100  // URL uzunluðu

struct Node{
    char site[URL_LENGTH];
    struct Node *next;
};

typedef struct{
    struct Node *geri_stack;
    struct Node *ileri_stack;
    char mevcut_site[URL_LENGTH];
}Gecmis;

void init(Gecmis *gecmis) {
    gecmis->geri_stack=NULL;
    gecmis->ileri_stack=NULL;
    strcpy(gecmis->mevcut_site,"");//mevcut site bos
}

void push(struct Node **stack,const char *site)
{
    struct Node *yeni_eleman=(struct Node *)malloc(sizeof(struct Node));
    strcpy(yeni_eleman->site,site);

    yeni_eleman->next=*stack;
    *stack=yeni_eleman;
}

void pop(struct Node **stack,char *site)
{
    if (*stack!=NULL)
    {
        struct Node *temp=*stack;
        strcpy(site,temp->site);
        *stack=temp->next;
        free(temp);
    }
    else
    {
        strcpy(site, "");   //stack bossa site bos
    }
}

void visit(Gecmis *gecmis,const char *site)
{
    if(strlen(gecmis->mevcut_site)>0)
    {
        push(&gecmis->geri_stack,gecmis->mevcut_site);  //mevcut siteyi stacke ekle
    }
    strcpy(gecmis->mevcut_site,site);//yeni site mevcut site oldu

    while(gecmis->ileri_stack!=NULL) //stack temizleme
    {
        char temp_site[URL_LENGTH];
        pop(&gecmis->ileri_stack,temp_site);
    }
    printf("%s sitesine gidildi.\n",gecmis->mevcut_site);
}

void Geri(Gecmis *gecmis)
{
    if(gecmis->geri_stack!=NULL)
    {
        push(&gecmis->ileri_stack,gecmis->mevcut_site);
        pop(&gecmis->geri_stack,gecmis->mevcut_site);
        printf("Geri gidildi: su anki site %s.\n",gecmis->mevcut_site);
    }
    else
    {
        printf("Geri gidilecek site yok\n");
    }
}

void ileri(Gecmis *gecmis)
{
    if(gecmis->ileri_stack!=NULL)
    {
        push(&gecmis->geri_stack,gecmis->mevcut_site);
        pop(&gecmis->ileri_stack,gecmis->mevcut_site);
        printf("\nileri gidildi: su anki site %s\n",gecmis->mevcut_site);
    }
    else
    {
        printf("ileri gidilecek site yok\n");
    }
}
int main()
{
    Gecmis gecmis;
    init(&gecmis);

    int secim;
    char site[URL_LENGTH];

    while(1)
    {
        printf("\n1. Geri git\n2. ileri git\n3. Yeni siteye git\n4. Cikis\n");
        printf("ornek:1.google.com 2.youtube.com 3.spotify.com\n");
        printf("\nYapmak istediginiz islemi secin:");
        scanf("%d",&secim);

         switch (secim) {
            case 1:
                Geri(&gecmis);
                break;
            case 2:
                ileri(&gecmis);
                break;
            case 3:
                printf("Gidilecek site URL'sini girin: ");
                scanf("%s",site);
                visit(&gecmis,site);
                break;
            case 4:
                printf("Programdan cikiliyor...\n");
                exit(0);
                break;
            default:
                printf("Gecersiz secim,tekrar deneyin.\n");
        }
    }
    return 0;
}
