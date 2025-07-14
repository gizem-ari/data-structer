#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// AVL düðüm yapýsý
typedef struct Dugum {
    int veri;
    struct Dugum* sol;
    struct Dugum* sag;
    int yukseklik;
} Dugum;

// Yeni düðüm oluþturma
Dugum* yeni_dugum(int veri) {
    Dugum* dugum = (Dugum*)malloc(sizeof(Dugum));
    dugum->veri = veri;
    dugum->sol = NULL;
    dugum->sag = NULL;
    dugum->yukseklik = 1;
    return dugum;
}

// Bir düðümün yüksekliðini hesapla
int yukseklik(Dugum* dugum) {
    return dugum ? dugum->yukseklik : 0;
}

// Maksimum deðeri döndür
int maksimum(int a, int b) {
    return (a > b) ? a : b;
}

// Sað döndürme
Dugum* sag_dondur(Dugum* y) {
    Dugum* x = y->sol;
    Dugum* T2 = x->sag;

    x->sag = y;
    y->sol = T2;

    y->yukseklik = maksimum(yukseklik(y->sol), yukseklik(y->sag)) + 1;
    x->yukseklik = maksimum(yukseklik(x->sol), yukseklik(x->sag)) + 1;

    return x;
}

// Sol döndürme
Dugum* sol_dondur(Dugum* x) {
    Dugum* y = x->sag;
    Dugum* T2 = y->sol;

    y->sol = x;
    x->sag = T2;

    x->yukseklik = maksimum(yukseklik(x->sol), yukseklik(x->sag)) + 1;
    y->yukseklik = maksimum(yukseklik(y->sol), yukseklik(y->sag)) + 1;

    return y;
}

// Düðümün denge faktörünü hesapla
int denge(Dugum* dugum) {
    return dugum ? yukseklik(dugum->sol) - yukseklik(dugum->sag) : 0;
}

// AVL aðacýna eleman ekleme
Dugum* ekle(Dugum* dugum, int veri) {
    if (!dugum) return yeni_dugum(veri);

    if (veri < dugum->veri)
        dugum->sol = ekle(dugum->sol, veri);
    else if (veri > dugum->veri)
        dugum->sag = ekle(dugum->sag, veri);
    else
        return dugum;

    dugum->yukseklik = 1 + maksimum(yukseklik(dugum->sol), yukseklik(dugum->sag));
    int denge_faktor = denge(dugum);

    if (denge_faktor > 1 && veri < dugum->sol->veri)
        return sag_dondur(dugum);
    if (denge_faktor < -1 && veri > dugum->sag->veri)
        return sol_dondur(dugum);
    if (denge_faktor > 1 && veri > dugum->sol->veri) {
        dugum->sol = sol_dondur(dugum->sol);
        return sag_dondur(dugum);
    }
    if (denge_faktor < -1 && veri < dugum->sag->veri) {
        dugum->sag = sag_dondur(dugum->sag);
        return sol_dondur(dugum);
    }

    return dugum;
}

// En küçük deðeri bulma
Dugum* en_kucuk_dugum(Dugum* dugum) {
    Dugum* mevcut = dugum;
    while (mevcut->sol)
        mevcut = mevcut->sol;
    return mevcut;
}

// AVL aðacýndan eleman silme
Dugum* sil(Dugum* dugum, int veri) {
    if (!dugum) return dugum;

    if (veri < dugum->veri)
        dugum->sol = sil(dugum->sol, veri);
    else if (veri > dugum->veri)
        dugum->sag = sil(dugum->sag, veri);
    else {
        if (!dugum->sol || !dugum->sag) {
            Dugum* gecici = dugum->sol ? dugum->sol : dugum->sag;
            if (!gecici) {
                gecici = dugum;
                dugum = NULL;
            } else
                *dugum = *gecici;
            free(gecici);
        } else {
            Dugum* gecici = en_kucuk_dugum(dugum->sag);
            dugum->veri = gecici->veri;
            dugum->sag = sil(dugum->sag, gecici->veri);
        }
    }

    if (!dugum) return dugum;

    dugum->yukseklik = 1 + maksimum(yukseklik(dugum->sol), yukseklik(dugum->sag));
    int denge_faktor = denge(dugum);

    if (denge_faktor > 1 && denge(dugum->sol) >= 0)
        return sag_dondur(dugum);
    if (denge_faktor > 1 && denge(dugum->sol) < 0) {
        dugum->sol = sol_dondur(dugum->sol);
        return sag_dondur(dugum);
    }
    if (denge_faktor < -1 && denge(dugum->sag) <= 0)
        return sol_dondur(dugum);
    if (denge_faktor < -1 && denge(dugum->sag) > 0) {
        dugum->sag = sag_dondur(dugum->sag);
        return sol_dondur(dugum);
    }

    return dugum;
}

// Aðaç içeriðini yazdýr (Inorder)
void yazdir(Dugum* kok) {
    if (kok) {
        yazdir(kok->sol);
        printf("%d ", kok->veri);
        yazdir(kok->sag);
    }
}

// Dosyadan verileri oku
void dosya_verilerini_oku(const char* dosya_adi, int* ekle_listesi, int* sil_listesi, int* ekle_sayi, int* sil_sayi) {
    FILE* dosya = fopen("veri.txt", "r");
    if (!dosya) {
        printf("Dosya acýlamadi!\n");
        exit(1);
    }

    char satir[256];
    if (fgets(satir, sizeof(satir), dosya)) {
        *ekle_sayi = 0;
        char* token = strtok(satir, " ");
        while (token) {
            ekle_listesi[(*ekle_sayi)++] = atoi(token);
            token = strtok(NULL, " ");
        }
    }

    if (fgets(satir, sizeof(satir), dosya)) {
        *sil_sayi = 0;
        char* token = strtok(satir, " ");
        while (token) {
            sil_listesi[(*sil_sayi)++] = atoi(token);
            token = strtok(NULL, " ");
        }
    }

    fclose(dosya);
}

// Aðaç düðümlerini DOT formatýnda yazdýrma
void agaci_dot_yazdir(Dugum* dugum, FILE* dosya) {
    if (!dugum) return;

    // Düðüm yazdýr
    if (dugum->sol)
        fprintf(dosya, "    %d -> %d;\n", dugum->veri, dugum->sol->veri);
    if (dugum->sag)
        fprintf(dosya, "    %d -> %d;\n", dugum->veri, dugum->sag->veri);

    // Alt aðaçlarý yazdýr
    agaci_dot_yazdir(dugum->sol, dosya);
    agaci_dot_yazdir(dugum->sag, dosya);
}

// DOT dosyasý oluþturma
void dot_dosyasi_olustur(Dugum* kok, const char* dosya_adi) {
    FILE* dosya = fopen(dosya_adi, "w");
    if (!dosya) {
        printf("Dosya oluþturulamadý!\n");
        return;
    }

    fprintf(dosya, "digraph AVLTree {\n");
    fprintf(dosya, "    node [fontname=\"Arial\", shape=circle];\n");

    if (kok)
        agaci_dot_yazdir(kok, dosya);
    else
        fprintf(dosya, "    NULL;\n");

    fprintf(dosya, "}\n");
    fclose(dosya);
    printf("DOT dosyasý oluþturuldu: %s\n", dosya_adi);
}


int main()
{
    Dugum* kok = NULL;
    int ekle_listesi[100], sil_listesi[100];
    int ekle_sayi = 0, sil_sayi = 0, sil_index = 0;

    dosya_verilerini_oku("veri.txt", ekle_listesi, sil_listesi, &ekle_sayi, &sil_sayi);

   // İlk satırdaki sayıları AVL ağacına ekle
    for (int i = 0; i < ekle_sayi; i++) {
        kok = ekle(kok, ekle_listesi[i]);
    }

    // İkinci satırdaki sayıları sırasıyla AVL ağacından sil
    for (int i = 0; i < sil_sayi; i++) {
        kok = sil(kok, sil_listesi[i]);
    }

    // Son durumu yazdır
    printf("AVL Ağacı (Inorder): ");
    yazdir(kok);
    printf("\n");

    // AVL ağacını DOT dosyasına yaz
    dot_dosyasi_olustur(kok, "avl_agaci.dot");

    printf("AVL ağacı DOT dosyasına yazıldı. Görselleştirmek için 'dot -Tpng avl_agaci.dot -o avl_agaci.png' komutunu kullanın.\n");


    return 0;
}
