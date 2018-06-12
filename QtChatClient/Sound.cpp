#include "Sound.h"

Sound::Sound()
{
}

void Sound::playMessageSound() {
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("sound.mp3"));
    player->setVolume(50);
    player->play();
}
