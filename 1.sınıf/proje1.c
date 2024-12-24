#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maksimum yemek sayısı ve isim uzunluğunun sabitleri
#define MAX_ÜRÜN 15    // Maksimum ürün sayısı
#define MAX_İSİM_UZUNLUĞU 30 // Maksimum yemek veya içecek ismi uzunluğu

// Yemek yapısını tanımla
typedef struct {
    char ad[MAX_İSİM_UZUNLUĞU]; // Yemek adı
    int kalori;               // Bir porsiyon veya bardak kalorisi
} Yemek;

// Kalori verilerini dosyadan yükle
int kaloriYukle(const char *dosyaAdi, Yemek yemekler[], int *sayac) {
    // Dosyayı okumak için aç
    FILE *dosya = fopen(dosyaAdi, "r");
    if (dosya == NULL) {
        // Eğer dosya açılamazsa hata mesajı yazdır
        perror("Kalori dosyasi acilamadi");
        return 0; // Dosya yüklenemezse 0 döndür
    }

    *sayac = 0; // Ürün sayısını sıfırla
    // Dosyayı satır satır okuyarak yemek bilgilerini al
    while (fscanf(dosya, "%s %d", yemekler[*sayac].ad, &yemekler[*sayac].kalori) == 2) {
        (*sayac)++; // Eğer doğru formatta veri okunduysa sayacı arttır
    }

    fclose(dosya); // Dosyayı kapat
    return 1; // Dosya başarıyla okunduysa 1 döndür
}

// Sipariş dosyasını işleyerek toplam kaloriyi hesapla
float toplamKaloriHesapla(const char *siparisDosyaAdi, Yemek yemekler[], int yemekSayisi) {
    // Sipariş dosyasını okumak için aç
    FILE *siparisDosyasi = fopen(siparisDosyaAdi, "r");
    if (siparisDosyasi == NULL) {
        // Eğer dosya açılamazsa hata mesajı yazdır
        perror("Siparis dosyasi acilamadi");
        return 0; // Dosya yüklenemezse 0 döndür
    }

    float toplamKalori = 0.0; // Toplam kalori miktarını başlat
    char yemekAdi[MAX_İSİM_UZUNLUĞU]; // Sipariş edilen yemeğin adı
    float miktar; // Porsiyon miktarı (artık float olacak)

    // Sipariş dosyasını satır satır oku
    while (fscanf(siparisDosyasi, "%s %f", yemekAdi, &miktar) == 2) {
        int bulundu = 0; // Yemek bulunup bulunmadığını takip et

        // Yemek adını kalori listesinden kontrol et
        for (int i = 0; i < yemekSayisi; i++) {
            if (strcmp(yemekAdi, yemekler[i].ad) == 0) {
                // Yemek bulunduysa, toplam kaloriye bu yemeğin kalorisi * porsiyon miktarını ekle
                toplamKalori += yemekler[i].kalori * miktar;
                bulundu = 1; // Yemek bulunduğunu işaretle
                break; // Döngüden çık, çünkü yemeği bulduk
            }
        }

        // Yemek bulunamazsa kullanıcıya hata mesajı ver
        if (!bulundu) {
            printf("Hata: Siparis edilen yemek bulunamadi: %s\n", yemekAdi);
        }
    }

    fclose(siparisDosyasi); // Sipariş dosyasını kapat
    return toplamKalori; // Toplam kalori miktarını geri döndür
}

int main() {
    Yemek yemekler[MAX_ÜRÜN]; // Kalori listesini tutacak dizi
    int yemekSayisi;          // Liste içindeki ürün sayısını tutacak değişken

    // Kalori bilgilerini yükle
    if (!kaloriYukle("kalori.txt", yemekler, &yemekSayisi)) {
        return 1; // Dosya yüklenemezse programı sonlandır
    }

    // Sipariş dosyasındaki toplam kaloriyi hesapla
    float toplamKalori = toplamKaloriHesapla("siparis.txt", yemekler, yemekSayisi);

    // Sonucu ekrana yazdır
    printf("Toplam Kalori: %.2f\n", toplamKalori);

    return 0; // Program başarıyla sonlandırıldı
}