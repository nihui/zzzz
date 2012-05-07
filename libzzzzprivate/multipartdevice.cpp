/*
 *  This file is part of kio_kuaipan, KIO slave for KingSoft KuaiPan
 *  Copyright (C) 2012 Ni Hui <shuizhuyuanluo@126.com>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License or (at your option) version 3 or any later version
 *  accepted by the membership of KDE e.V. (or its successor approved
 *  by the membership of KDE e.V.), which shall act as a proxy
 *  defined in Section 14 of version 3 of the license.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "multipartdevice.h"

#include <QFile>
#include <KDebug>

MultiPartDevice::MultiPartDevice(QIODevice* dev, QObject* parent)
    : QIODevice(parent)
{
    m_contentDevice = dev;
    m_pos = 0;
    connect(m_contentDevice, SIGNAL(readyRead()), this, SIGNAL(readyRead()));
}

MultiPartDevice::MultiPartDevice(const QString& file, QObject* parent)
    : QIODevice(parent)
{
    m_contentDevice = new QFile(file, this);
    m_pos = 0;
    connect(m_contentDevice, SIGNAL(readyRead()), this, SIGNAL(readyRead()));
}

MultiPartDevice::~MultiPartDevice()
{
}

bool MultiPartDevice::atEnd() const
{
//     kWarning();
    return m_pos >= size();
}

qint64 MultiPartDevice::bytesAvailable() const
{
//     kWarning();
    return size() - m_pos;
}

bool MultiPartDevice::canReadLine() const
{
//     kWarning();
    return false;
}

void MultiPartDevice::close()
{
//     kWarning();
    m_contentDevice->close();
}

bool MultiPartDevice::isSequential() const
{
//     kWarning() << m_contentDevice->isSequential();
    return m_contentDevice->isSequential();
}

bool MultiPartDevice::open(OpenMode mode)
{
    QIODevice::open(mode);
//     kWarning();
    return m_contentDevice->open(mode);
}

qint64 MultiPartDevice::pos() const
{
//     kWarning();
    return QIODevice::pos();
}

bool MultiPartDevice::reset()
{
//     kWarning();
    m_pos = 0;
    return m_contentDevice->reset();
}

bool MultiPartDevice::seek(qint64 pos)
{
//     kWarning();
    QIODevice::seek(pos);
    m_pos = pos;

    qint64 devpos = m_pos;
    devpos -= m_boundary.size();
    devpos -= m_contentDispositionHeader.size();
    devpos -= m_contentTypeHeader.size();
    devpos -= 2;// header to content separator "\r\n"
    if (devpos >= 0 && devpos < m_contentDevice->size()) {
        return m_contentDevice->seek(devpos);
    }

    return m_contentDevice->seek(0);
}

qint64 MultiPartDevice::size() const
{
    qint64 s = 0;
    s += m_boundary.size();
    s += 2;// boundary to header separator "\r\n"
    s += m_contentDispositionHeader.size();
    s += m_contentTypeHeader.size();
    s += 2;// header to content separator "\r\n"
    s += m_contentDevice->size();
    s += 2;// content to boundary separator "\r\n"
    s += m_formdata.size();
    s += m_boundary.size();
    s += 4;// trailing "--\r\n"
//     kWarning() << s;
    return s;
}

void MultiPartDevice::setBoundary(const QByteArray& b)
{
    m_boundary = QByteArray("--") + b;
}

void MultiPartDevice::setFormData(const QMultiMap<QByteArray, QByteArray>& formdata)
{
    m_formdata.clear();
    QMultiMap<QByteArray, QByteArray>::ConstIterator it = formdata.constBegin();
    QMultiMap<QByteArray, QByteArray>::ConstIterator end = formdata.constEnd();
    while (it != end) {
        m_formdata += m_boundary;
        m_formdata += "\r\n";
        m_formdata += "Content-Disposition: form-data; name=\"" + it.key() + "\"\r\n";
        m_formdata += "Content-Transfer-Encoding: 8bit\r\n";
        m_formdata += "\r\n";
        m_formdata += it.value();
        m_formdata += "\r\n";
        ++it;
    }
}

void MultiPartDevice::setContentDispositionHeader(const QByteArray& cd)
{
    m_contentDispositionHeader = QByteArray("Content-Disposition: ") + cd + QByteArray("\r\n");
}

void MultiPartDevice::setContentTypeHeader(const QByteArray& ct)
{
    m_contentTypeHeader = QByteArray("Content-Type: ") + ct + QByteArray("\r\n");
}

qint64 MultiPartDevice::readData(char* data, qint64 maxSize)
{
    qint64 p = m_pos;
    qint64 toRead = maxSize;
//     kWarning() << m_pos << maxSize;

    if (p < m_boundary.size()) {
        // start from boundary
        if (p + toRead < m_boundary.size()) {
            // end in boundary
            memcpy(data, m_boundary.constData() + p, toRead);
            m_pos += toRead;
            return maxSize;
        }
        // copy boundary
        memcpy(data, m_boundary.constData() + p, m_boundary.size() - p);
        data += m_boundary.size() - p;
        toRead -= m_boundary.size() - p;
        m_pos += m_boundary.size() - p;
        p = 0;
    }
    else {
        p -= m_boundary.size();
    }

    if (p < 2) {
        // start from separator
        if (p + toRead < 2) {
            // end in separator
            memcpy(data, "\r\n", toRead);
            m_pos += toRead;
            return maxSize;
        }
        // copy separator
        memcpy(data, "\r\n", 2 - p);
        data += 2 - p;
        toRead -= 2 - p;
        m_pos += 2 - p;
        p = 0;
    }
    else {
        p -= 2;
    }

    if (p < m_contentDispositionHeader.size()) {
        // start from contentDispositionHeader
        if (p + toRead < m_contentDispositionHeader.size()) {
            // end in contentDispositionHeader
            memcpy(data, m_contentDispositionHeader.constData(), toRead);
            m_pos += toRead;
            return maxSize;
        }
        // copy contentDispositionHeader
        memcpy(data, m_contentDispositionHeader.constData(), m_contentDispositionHeader.size() - p);
        data += m_contentDispositionHeader.size() - p;
        toRead -= m_contentDispositionHeader.size() - p;
        m_pos += m_contentDispositionHeader.size() - p;
        p = 0;
    }
    else {
        p -= m_contentDispositionHeader.size();
    }

    if (p < m_contentTypeHeader.size()) {
        // start from contentTypeHeader
        if (p + toRead < m_contentTypeHeader.size()) {
            // end in contentTypeHeader
            memcpy(data, m_contentTypeHeader.constData(), toRead);
            m_pos += toRead;
            return maxSize;
        }
        // copy contentTypeHeader
        memcpy(data, m_contentTypeHeader.constData(), m_contentTypeHeader.size() - p);
        data += m_contentTypeHeader.size() - p;
        toRead -= m_contentTypeHeader.size() - p;
        m_pos += m_contentTypeHeader.size() - p;
        p = 0;
    }
    else {
        p -= m_contentTypeHeader.size();
    }

    if (p < 2) {
        // start from separator
        if (p + toRead < 2) {
            // end in separator
            memcpy(data, "\r\n", toRead);
            m_pos += toRead;
            return maxSize;
        }
        // copy separator
        memcpy(data, "\r\n", 2 - p);
        data += 2 - p;
        toRead -= 2 - p;
        m_pos += 2 - p;
        p = 0;
    }
    else {
        p -= 2;
    }

    if (p < m_contentDevice->size()) {
        // start from contentDevice
        if (p + toRead < m_contentDevice->size()) {
            // end in contentDevice
            qint64 nr = m_contentDevice->read(data, toRead);
            m_pos += nr;
            return maxSize - toRead + nr;
        }
        // copy contentDevice
        qint64 sr = m_contentDevice->size() - m_contentDevice->pos();
        qint64 nr = m_contentDevice->read(data, sr);
        if (nr < sr) {
            m_pos += nr;
            return maxSize - sr + nr;
        }
        data += sr;
        toRead -= sr;
        m_pos += sr;
        p = 0;
    }
    else {
        p -= m_contentDevice->size();
    }

    if (p < 2) {
        // start from separator
        if (p + toRead < 2) {
            // end in separator
            memcpy(data, "\r\n", toRead);
            m_pos += toRead;
            return maxSize;
        }
        // copy separator
        memcpy(data, "\r\n", 2 - p);
        data += 2 - p;
        toRead -= 2 - p;
        m_pos += 2 - p;
        p = 0;
    }
    else {
        p -= 2;
    }

    if (p < m_formdata.size()) {
        // start from formdata
        if (p + toRead < m_formdata.size()) {
            // end in formdata
            memcpy(data, m_formdata.constData() + p, toRead);
            m_pos += toRead;
            return maxSize;
        }
        // copy formdata
        memcpy(data, m_formdata.constData() + p, m_formdata.size() - p);
        data += m_formdata.size() - p;
        toRead -= m_formdata.size() - p;
        m_pos += m_formdata.size() - p;
        p = 0;
    }
    else {
        p -= m_formdata.size();
    }

    if (p < m_boundary.size()) {
        // start from boundary
        if (p + toRead < m_boundary.size()) {
            // end in boundary
            memcpy(data, m_boundary.constData() + p, toRead);
            m_pos += toRead;
            return maxSize;
        }
        // copy boundary
        memcpy(data, m_boundary.constData() + p, m_boundary.size() - p);
        data += m_boundary.size() - p;
        toRead -= m_boundary.size() - p;
        m_pos += m_boundary.size() - p;
        p = 0;
    }
    else {
        p -= m_boundary.size();
    }

    if (p < 4) {
        // start from trailing
        if (p + toRead < 4) {
            // end in trailing
            memcpy(data, "--\r\n", toRead);
            m_pos += toRead;
            return maxSize;
        }
        // copy trailing
        memcpy(data, "--\r\n", 4 - p);
        data += 4 - p;
        toRead -= 4 - p;
        m_pos += 4 - p;
        p = 0;
    }
    else {
        p -= 4;
    }
    // p should be zero here

//     kWarning() << "r" << maxSize - toRead;
    return maxSize - toRead;
}

qint64 MultiPartDevice::writeData(const char* data, qint64 maxSize)
{
    return 0;
}
