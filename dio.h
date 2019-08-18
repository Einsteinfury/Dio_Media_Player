#ifndef DIO_H
#define DIO_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QGraphicsVideoItem>
#include <QFileDialog>
#include <QVideoWidget>
#include <QBoxLayout>
#include <QDateTime>
#include <QUrl>
#include <QDir>
#include <QKeyEvent>
#include <QThread>

namespace Ui {
class Dio;
}

class Dio : public QMainWindow
{
    Q_OBJECT

public:
    explicit Dio(QWidget *parent = nullptr);
    ~Dio();
    QString msToTime(qint64 msTime);
    void updateTime();
    void checkAvalible();
    void playPause();
    void loadVideo(QString path);

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void positionChanged(qint64 position);
    void durationChanged(qint64 position);
    void videoAvalibe(bool avalible);

private slots:
    void on_nextButton_clicked();

    void on_actionOpen_file_triggered();

    void on_actionOpen_folder_triggered();

    void on_previousButton_clicked();

    void on_shuffleButton_clicked();

    void on_loopButton_clicked();

    void on_playPause_clicked();

    void on_playbackSlider_sliderMoved(int position);

    void on_playbackSlider_sliderReleased();

    void on_volumeSlider_sliderMoved(int position);

private:
    Ui::Dio *ui;
    QString videoname;
    QMediaPlayer *player;
    QVideoWidget *video;
    QMediaPlaylist *playlist;
    QGraphicsVideoItem *subtitles;

    bool looped = false;
    bool playListLoaded = false;
    bool firstVideo;
    bool isPaused = false;
};

#endif // DIO_H
