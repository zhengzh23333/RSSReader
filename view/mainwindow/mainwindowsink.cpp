#include "mainwindowsink.h"
#include <QDebug>
#include <cassert>

MainWindowSink::MainWindowSink()  {
    loadpage = std::make_shared<loading_page>();
    mainwindow = std::make_shared<MainWindow>();

    connect(mainwindow.get(), SIGNAL(SIG_CLOSE()), this, SLOT(OnMainWindowClose()));
    connect(mainwindow.get(), SIGNAL(SIG_ADDSUB(const QString&)), this, SLOT(AddSubcription(const QString&)));

    loadpage->show();
}

void MainWindowSink::OnMainWindowClose() {
    qDebug() << "[MainWindow close]";
    emit SIG_TRI("channel", "exit", "");
}

void MainWindowSink::AddSubcription(const QString& url) {
    qDebug() << "[Add subscription] " << url;
    emit SIG_TRI("channel", "add", url);
    UpdateSub();
}

void MainWindowSink::UpdateSub() {
    qDebug() << "[Updating Subscription]";
    std::shared_ptr<QVector<QString>> allsubtitle = viewmodel->GetMeta("channel");

    for (QVector<QString>::iterator iter = allsubtitle->begin();iter != allsubtitle->end(); iter++) {
        qDebug() << *iter;
    }
    qDebug() << "[End]";
    mainwindow->UpdateLeft(allsubtitle);
}

void MainWindowSink::UpStreamReciever(const QString& _data, const QString& msg, const QString& target) {
    qDebug() << "[=== QAQ ===] " << _data << ' ' << msg << ' ' << target;
    if(_data == "channel" && msg == "init") {
        if(target == "ok") {
            qDebug() << "[Initial ok]";
            loadpage->close();
            mainwindow->show();
            UpdateSub();
        }
        else {
            qDebug() << "[initial failed]";
            exit(1);
        }
    }
    else if(_data == "channel" && msg == "add") {
        if(target == "ok") {
            UpdateSub();
        }
        else {
            qDebug() << "[Add subscription ok]";
            exit(1);
        }
    }
    else {
        assert(false);
    }
}
