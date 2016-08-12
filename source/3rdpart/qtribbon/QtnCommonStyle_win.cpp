#if defined(PRO_CORE)
#include "precomp.h"
#endif

#include "QtnCommonStylePrivate.h"
#include <qt_windows.h>

using namespace QtRibbon;


#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
QT_BEGIN_NAMESPACE
	Q_GUI_EXPORT QPixmap qt_pixmapFromWinHBITMAP(HBITMAP, int hbitmapFormat = 0);
	Q_GUI_EXPORT HBITMAP qt_pixmapToWinHBITMAP(const QPixmap& p, int hbitmapFormat = 0);
	Q_GUI_EXPORT HICON qt_pixmapToWinHICON(const QPixmap& p);
	Q_GUI_EXPORT QPixmap qt_pixmapFromWinHICON(HICON icon);
QT_END_NAMESPACE

enum HBitmapFormat 
{
	HBitmapNoAlpha,
	HBitmapPremultipliedAlpha,
	HBitmapAlpha
};
#endif

static QImage qtn_fromWinHBITMAP(HDC hdc, HBITMAP bitmap, int w, int h)
{
    BITMAPINFO bmi;
    memset(&bmi, 0, sizeof(bmi));
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = w;
    bmi.bmiHeader.biHeight      = -h;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage   = w * h * 4;

    QImage image(w, h, QImage::Format_ARGB32_Premultiplied);
    image.fill(0);
    if (image.isNull())
        return image;

    // Get bitmap bits
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    uchar* data = (uchar*)::malloc(bmi.bmiHeader.biSizeImage);
#else
    uchar* data = (uchar*)qMalloc(bmi.bmiHeader.biSizeImage);
#endif

    if (GetDIBits(hdc, bitmap, 0, h, data, &bmi, DIB_RGB_COLORS)) 
    {

        // Create image and copy data into image.
        for (int y = 0; y < h; ++y) 
        {
            void* dest = (void*) image.scanLine(y);
            void* src = data + y * image.bytesPerLine();
            memcpy(dest, src, image.bytesPerLine());
        }
    } 
    else 
    {
        qWarning("qtn_fromWinHBITMAP(), failed to get bitmap bits");
    }
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_ASSERT(false); //TODO
#else
    qFree(data);
#endif

    return image;
}

QPixmap StyleHelper::alphaBlend(const QPixmap& src)
{
//    HBITMAP winBitmap1 = src.toWinHBITMAP(QPixmap::Alpha);
//    return QPixmap::fromWinHBITMAP(winBitmap1, QPixmap::PremultipliedAlpha);

    bool foundAlpha = false;
    HDC screenDevice = GetDC(0);
    HDC hdc = CreateCompatibleDC(screenDevice);
    ReleaseDC(0, screenDevice);

    int w = src.width();
    int h = src.height();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	HBITMAP winBitmap = qt_pixmapToWinHBITMAP(src, HBitmapAlpha);
#else
    HBITMAP winBitmap = src.toWinHBITMAP(QPixmap::Alpha);
#endif

    HGDIOBJ oldhdc = (HBITMAP)SelectObject(hdc, winBitmap);

    QImage image = qtn_fromWinHBITMAP(hdc, winBitmap, w, h);

    for (int y = 0 ; y < h && !foundAlpha ; y++) 
    {
        QRgb* scanLine = reinterpret_cast<QRgb*>(image.scanLine(y));
        for (int x = 0; x < w ; x++) 
        {
            if (qAlpha(scanLine[x]) != 0) 
            {
                foundAlpha = true;
                break;
            }
        }
    }

    if (!foundAlpha) 
    {
        //If no alpha was found, we use the mask to set alpha values
        QImage mask = qtn_fromWinHBITMAP(hdc, winBitmap, w, h);

        for (int y = 0 ; y < h ; y++)
        {
            QRgb* scanlineImage = reinterpret_cast<QRgb*>(image.scanLine(y));
            QRgb* scanlineMask = mask.isNull() ? 0 : reinterpret_cast<QRgb*>(mask.scanLine(y));
            for (int x = 0; x < w ; x++)
            {
                if (scanlineMask && qRed(scanlineMask[x]) != 0)
                    scanlineImage[x] = 0; //mask out this pixel
                else
                    scanlineImage[x] |= 0xff000000; // set the alpha channel to 255
            }
        }
    }

    SelectObject(hdc, oldhdc); //restore state
    DeleteObject(winBitmap);
    DeleteDC(hdc);

    return QPixmap::fromImage(image);
}

