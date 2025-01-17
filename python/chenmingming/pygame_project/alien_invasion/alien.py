import pygame
from pygame.sprite import Sprite
class Alien(Sprite):
    def __init__(self, ai_settings, screen):
        super().__init__()
        self.screen = screen
        self.ai_settings = ai_settings

        self.image = pygame.image.load("images/alien.bmp")
        self.image = pygame.transform.rotozoom(self.image, 270, 0.2)
        self.rect = self.image.get_rect()

        self.rect.x = self.screen.get_rect().width-2*self.rect.width
        self.rect.y = self.rect.height

        self.x = float(self.rect.x)

    def blitme(self):
        self.screen.blit(self.image, self.rect)
    def check_edges(self):
        screen_rect = self.screen.get_rect()
        if self.rect.top <= 0:
            return True
        elif self.rect.bottom >= screen_rect.bottom:
            return True
    def uupdate(self):
        self.y += (self.ai_settings.alien_speed_factor * self.ai_settings.fleet_direction)
        self.rect.y = self.y