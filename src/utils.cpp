#include "utils.h"
#include <QDir>

QString utils::new_dir(const QString& path, const QString& file_name) {
    QDir dir;
    QString base_name, extension, ret;

    if (!dir.exists(path)) {
        dir.mkpath(path);
    }

    size_t last_index = file_name.lastIndexOf('.');

    if (last_index != -1) {
        base_name = file_name.first(last_index);
        extension = file_name.sliced(last_index);
    }
    else {
        base_name = file_name;
        extension = "";
    }

    for (int i = 0; ; ++i) {
        ret = base_name;
        if (i != 0) {
            ret += (" (" + QString::number(i) + ")");
        }

        ret += extension;

        if (!QFile::exists(path + ret))
            break;
    }

    return path + ret;
}