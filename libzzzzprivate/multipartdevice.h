#ifndef MULTIPARTDEVICE_H
#define MULTIPARTDEVICE_H

#include "zzzzprivate_export.h"
#include <QIODevice>
#include <QMultiMap>

class ZZZZPRIVATE_EXPORT MultiPartDevice : public QIODevice
{
public:
    explicit MultiPartDevice(QIODevice* dev, QObject* parent = 0);
    explicit MultiPartDevice(const QString& file, QObject* parent = 0);
    virtual ~MultiPartDevice();
    virtual bool atEnd() const;
    virtual qint64 bytesAvailable() const;
    virtual bool canReadLine() const;
    virtual void close();
    virtual bool isSequential() const;
    virtual bool open(OpenMode mode);
    virtual qint64 pos() const;
    virtual bool reset();
    virtual bool seek(qint64 pos);
    virtual qint64 size() const;
    void setBoundary(const QByteArray& b);
    void setFormData(const QMultiMap<QByteArray, QByteArray>& formdata);
    void setContentDispositionHeader(const QByteArray& cd);
    void setContentTypeHeader(const QByteArray& ct);
protected:
    virtual qint64 readData(char* data, qint64 maxSize);
    virtual qint64 writeData(const char* data, qint64 maxSize);
private:
    QByteArray m_boundary;
    QByteArray m_contentDispositionHeader;
    QByteArray m_contentTypeHeader;
    QIODevice* m_contentDevice;
    QByteArray m_formdata;
    qint64 m_pos;
};

#endif // MULTIPARTDEVICE_H
