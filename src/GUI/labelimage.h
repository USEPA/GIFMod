#ifndef LABELIMAGE_H
#define LABELIMAGE_H

#include <QLabel>

class LabelImage: public QLabel {

  private:
    QPixmap _qPixmap, _qPixmapScaled;
    bool _bKeepOriginalSize;

  public:
    void setPixmap(const QPixmap &qPixmap, bool keepOriginalSize) { _bKeepOriginalSize = keepOriginalSize; setPixmap(qPixmap, _bKeepOriginalSize ? qPixmap.size() : size()); }

  protected:
    virtual void resizeEvent(QResizeEvent *pQEvent);

  private:
    void setPixmap(const QPixmap &qPixmap, const QSize &size);
};

void LabelImage::resizeEvent(QResizeEvent *pQEvent)
{
  QLabel::resizeEvent(pQEvent);
  if (!_bKeepOriginalSize)
      setPixmap(_qPixmap, pQEvent->size());
}

void LabelImage::setPixmap(const QPixmap &qPixmap, const QSize &size)
{
  _qPixmap = qPixmap;
  _qPixmapScaled = _qPixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  QLabel::setPixmap(_qPixmapScaled);
}

#endif // LABELIMAGE_H
