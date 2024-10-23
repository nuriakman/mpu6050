import pygame
import sys
import math

# Pygame'i başlat
pygame.init()

# Ekran boyutları
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Kağıt Uçağı")

# Renk tanımları
WHITE = (255, 255, 255)
BLUE = (0, 0, 255)

# Uçak sınıfı
class PaperPlane:
    def __init__(self, x, y):
        self.image = pygame.Surface((50, 30), pygame.SRCALPHA)  # Şeffaf arka plana sahip yüzey
        pygame.draw.polygon(self.image, BLUE, [(25, 0), (0, 30), (50, 30)])  # Uçağı çiz
        self.rect = self.image.get_rect(center=(x, y))
        self.angle = 0  # Başlangıç açısı

    def draw(self, screen):
        # Uçak görüntüsünü döndürerek çizin
        rotated_image = pygame.transform.rotate(self.image, self.angle)
        rotated_rect = rotated_image.get_rect(center=self.rect.center)
        screen.blit(rotated_image, rotated_rect.topleft)

    def update(self, acc, rotation):
        # Dönüş açısını güncelle
        self.angle += rotation  # Açıyı güncelle (saniye başına)

        # Uçak sabit bir noktada kalacak, bu yüzden konum güncellenmeyecek
        # Ancak eğer hareket etmek istiyorsanız, acc değerini kullanarak x, y koordinatlarını güncelleyebilirsiniz
        # Örneğin, sadece görsel olarak y ekseninde yukarı veya aşağı hareket etmek için:
        # self.rect.y -= acc  # Y ekseninde yukarı doğru hareket

# Ana oyun döngüsü
def main():
    clock = pygame.time.Clock()
    plane = PaperPlane(WIDTH // 2, HEIGHT // 2)  # Uçağı ortada başlat

    # Örnek veriler (bu değerleri dışardan alabilirsiniz)
    acc = 0  # Hız değeri (sabit)
    rotation = 1  # Her döngüde açıyı artır

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

        # Uçağı güncelle (acc ve rotation değerlerini dışarıdan alabilirsiniz)
        plane.update(acc, rotation)

        # Ekranı temizle
        screen.fill(WHITE)
        
        # Uçağı çiz
        plane.draw(screen)

        # Ekranı güncelle
        pygame.display.flip()
        clock.tick(60)  # FPS ayarla

if __name__ == "__main__":
    main()
