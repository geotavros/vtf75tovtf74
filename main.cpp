/**
    This code changes VTF file version from 7.5 to 7.4. It parses a specified 
    folder and all subfolders and changes all VTF files by setting 8 byte to 4.
    It doesn't modify files other then 7.5 version. 
*/
#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>

void processDirectory(const QString &dir)
{
    {
        QDir qdir(dir);
        qdir.setNameFilters(QStringList("*.vtf"));

        QFileInfoList files = qdir.entryInfoList(QDir::Files);
        for (int i = 0; i < files.count(); ++i)
        {
            qDebug() << files.at(i).absoluteFilePath();
            QFile file(files.at(i).absoluteFilePath());
            if (file.open(QIODevice::ReadWrite))
            {
                // set pos to 8 byte and overwrite this pos
                file.seek(8);

                char cur_version;
                if (file.read(&cur_version, 1) && 5 == cur_version)
                {
                    //qDebug() << one_byte << "\n";
                    const char version_override = 4;
                    file.seek(8);
                    file.write(&version_override, 1);
                }

                file.close();
            }
        }
    }

    {
        QDir directories(dir, "", QDir::Name | QDir::IgnoreCase , QDir::Dirs | QDir::NoDotAndDotDot);
        QFileInfoList dirs = directories.entryInfoList();
        for (int i = 0; i < dirs.count(); ++i)
        {
            qDebug() << dirs.at(i).absoluteFilePath();
            processDirectory(dirs.at(i).absoluteFilePath());
        }
    }

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    processDirectory(argv[1]);

    return a.exec();
}
