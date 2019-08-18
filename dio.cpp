#include "dio.h"
#include "ui_dio.h"

Dio::Dio(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Dio)
{
    ui->setupUi(this);

    playlist = new QMediaPlaylist;
    player = new QMediaPlayer;
    video = new QVideoWidget;
    subtitles = new QGraphicsVideoItem;

    connect(player, &QMediaPlayer::positionChanged, this, &Dio::positionChanged);
    connect(player, &QMediaPlayer::videoAvailableChanged, this, &Dio::videoAvalibe);
    connect(player, &QMediaPlayer::durationChanged, this, &Dio::durationChanged);

    player->setVideoOutput(video);
    ui->horizontalLayout->addWidget(video);

    if  (QApplication::arguments().size() >= 2) {
        loadVideo(QApplication::arguments().at(1));
    }

    checkAvalible();
}

Dio::~Dio()
{
    delete ui;
}

void Dio::checkAvalible(){
    if (playlist->isEmpty()){
        ui->shuffleButton->setEnabled(false);
        ui->previousButton->setEnabled(false);
        ui->nextButton->setEnabled(false);
    }
    else {
        ui->nextButton->setEnabled(true);

        if (playlist->currentIndex() == 0) {
            ui->previousButton->setEnabled(false);
        }

        else if (playlist->currentIndex() == playlist->mediaCount() - 1) {
            ui->nextButton->setEnabled(false);
        }

        else {
            ui->nextButton->setEnabled(true);
            ui->previousButton->setEnabled(true);
        }

        ui->shuffleButton->setEnabled(true);
    }
}

void Dio::loadVideo(QString path){
    videoname = path;
    player->setMedia(QUrl::fromLocalFile(path));
    player->play();
}

void Dio::playPause(){
    if (isPaused) {
        player->play();
        ui->playPause->setText("Pause");
    }
    else {
        player->pause();
        ui->playPause->setText("Play");
    }

    isPaused = !isPaused;
}

void Dio::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
        case Qt::Key_Space:
            playPause();
            break;
    }
}

void Dio::positionChanged(qint64 postion){
    ui->playbackSlider->setValue(postion);
    ui->durationLabel->setText(msToTime(postion) + " / " + msToTime(player->duration()));

    if (looped == true){
        if(player->duration() > 0 && player->position() >= player->duration() - 1000){
            player->setPosition(1);
            emit positionChanged(1);
        }

        else if (player->duration() > 0 && postion != 0 && postion < 4){
           //still in work

        }
    }
}

void Dio::durationChanged(qint64 duration){
    ui->playbackSlider->setRange(0, player->duration());
    ui->durationLabel->setText(msToTime(player->position()) + " / " + msToTime(duration));
}

void Dio::videoAvalibe(bool avalible){
    if (avalible){
        ui->playPause->setText("Pause");
        Dio::setWindowTitle("DiO Media Player: " + videoname);
    }
}

QString Dio::msToTime(qint64 msTime){
    return QDateTime::fromTime_t(msTime / 1000).toUTC().toString("mm:ss");
}

void Dio::on_nextButton_clicked()
{
    playlist->next();
    checkAvalible();
    qDebug() <<playlist->currentIndex() <<endl;
}

void Dio::on_actionOpen_file_triggered()
{
    playlist->clear();
    checkAvalible();
    QString filename = QFileDialog::getOpenFileName(this);
    if (filename != NULL){
        loadVideo(filename);
    }
}

void Dio::on_actionOpen_folder_triggered()
{
    QString foldername = QFileDialog::getExistingDirectory(this);
    QDir folder (foldername);
    QStringList folderContents = folder.entryList();
    for (int i = 2; i < folderContents.size(); i++){
        QString pathToFile = foldername + "/" + folderContents.at(i);
        playlist->addMedia(QUrl::fromLocalFile(pathToFile));
        qDebug() << pathToFile <<endl;
    }

    playlist->setCurrentIndex(0);
    player->setPlaylist(playlist);
    checkAvalible();
    player->play();
}

void Dio::on_previousButton_clicked()
{
    playlist->previous();
    qDebug() <<playlist->currentIndex() <<endl;
    checkAvalible();
}

void Dio::on_shuffleButton_clicked()
{
    playlist->shuffle();
    playlist->setCurrentIndex(0);
    checkAvalible();
}

void Dio::on_loopButton_clicked()
{
    looped = !looped;
}

void Dio::on_playPause_clicked()
{
    playPause();
}

void Dio::on_playbackSlider_sliderMoved(int position)
{
    player->pause();
    player->setPosition(position);
}

void Dio::on_playbackSlider_sliderReleased()
{
    if (isPaused == false){
        player->play();
    }
}

void Dio::on_volumeSlider_sliderMoved(int position)
{
    player->setVolume(position);
}
