// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_convolve:
//
// Apply image convolution on I1. Output is in I2.
//
// Written by: Weifan Lin, 2016
//

#include <QVector>

extern void copyRowToBuffer(ChannelPtr<uchar>, short*, int, int); // see implementation in HW_errDiffusion.cpp

void
HW_convolve(ImagePtr I1, ImagePtr Ikernel, ImagePtr I2)
{
    int sz = Ikernel->width();
    // if(sz % 2 == 0) sz++; // set sz to always be an odd number

    IP_copyImageHeader(I1, I2);
    int w = I1->width();
    int h = I1->height();
    int total = w*h;

    int type;
    ChannelPtr<uchar> p1, p2, endd;

    if (sz == 1)
    {
        for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
            IP_getChannel(I2, ch, p2, type);
            for(endd = p1 + total; p1<endd;) *p2++ = *p1++;
        }
    }
    else if (sz > 1) {

        ChannelPtr<float> pKernel; // kernel value pointer
        IP_getChannel(Ikernel, 0, pKernel, type);

        int bufSz = sz+w-1; // size of buffer for each padded row
        QVector<QVector<short>> buffers(sz); // array of sz pointers
        for (int i=0; i<sz; i++)
            buffers[i] = QVector<short>(bufSz);

        for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++)
        {
            IP_getChannel(I2, ch, p2, type);
            endd = p1+total;

            // copy first row to first sz/2 buffers, where are top padded rows
            for (int i=0; i<sz/2; i++) copyRowToBuffer(p1, buffers[i].data(), w, sz);

            // continue to rest of buffers, note here we still begin copying from frist row of I1
            for (int i=sz/2; i<sz; i++) {
                copyRowToBuffer(p1, buffers[i].data(), w, sz);
                if (p1 < endd-w) p1+=w;
            }

            for (int y=0; y<h; y++) {  // visit each row
                // visit each pixel in row
                for (int x=0; x<w; x++) {
                    float sum = 0.0;
                    for (int i=0; i<sz; i++) {
                        for (int j=0; j<sz; j++) {
                          sum += (buffers[i][j+x] * *pKernel++);
                        }
                    }
                    pKernel -= sz*sz;
                    *p2++ = (int)CLIP(sum, 0, 255);
                }

                // circular quque
                // remove buffers[0] and push all other buffers up by one index
                for (int i = 0; i<sz-1; i++) {
                  for (int j = 0; j<bufSz; j++) {
                    buffers[i][j] = buffers[i+1][j];
                  }
                }
                // copy next row to last buffer
                copyRowToBuffer(p1, buffers[sz-1].data(), w, sz); // copy new row to the last buffer
                if (p1 < endd-w) p1+=w;
            }
        }
    }
}
