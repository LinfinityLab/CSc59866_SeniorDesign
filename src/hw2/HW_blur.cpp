// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_blur:
//
// Blur image I1 with a box filter (unweighted averaging).
// The filter has width filterW and height filterH.
// We force the kernel dimensions to be odd.
// Output is in I2.
//
// Written by: Weifan Lin, 2016
//
void blur1D(ChannelPtr<uchar>, int, int, int, ChannelPtr<uchar>);

void
HW_blur(ImagePtr I1, int filterW, int filterH, ImagePtr I2)
{
    IP_copyImageHeader(I1, I2);

    ImagePtr I3; // intermediate buffer
    IP_copyImageHeader(I1, I3);

    int w = I1->width();
    int h = I1->height();
    int total = w * h;

    int type;
    ChannelPtr<uchar> p1, p2, p3, endd;
    for(int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
        IP_getChannel(I2, ch, p2, type);
        IP_getChannel(I3, ch, p3, type);

        if (filterW == 1) for(endd = p1 + total; p1<endd;) *p3++ = *p1++;
        if (filterW > 1) {
            // blur rows one by one
            for (int y=0; y<h; y++) {
                blur1D(p1, w, 1, filterW, p3);
                p1+=w;
                p3+=w;
            }
        }

        p3=p3-total; // point back to 0

        if (filterH == 1) for(endd = p3 + total; p3<endd;) *p2++ = *p3++;
        if (filterH > 1) {
            // blur columns one by one
            for (int x=0; x<w; x++) {
                blur1D(p3, h, w, filterH, p2);
                p3+=1;
                p2+=1;
            }
        }
    }
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// blur1D:
//
// Blur src with 1D box filter of width ww.
//
void
blur1D(ChannelPtr<uchar> src, int len, int stride, int ww, ChannelPtr<uchar> dst)
{
    if(ww % 2 == 0) ww++; // set ww to always be an odd number
    int neighborSz = ww/2; // how many pixels on the left and right
    int bufSz = len+ww-1; // this is len for padded buffer
    short* buffer = new short[bufSz]; // buffer to store a padded rows or columns

    // copy to buffer
    for (int i = 0; i < neighborSz; i++) { buffer[i] = *src; }  // copy first pixel to left padded area
    int index = 0;
    for (int i = neighborSz; i < len+neighborSz; i++) {  // continue with pixel replication
        buffer[i] = src[index];
        index+=stride;  // next index
    }
    for (int i = len+neighborSz; i < bufSz; i++) { buffer[i] = src[index-stride]; } // copy last pixel to right padded area

    unsigned short sum = 0;
    for (int i = 0; i<ww ; i++) { sum+=buffer[i]; }  // sum of pixel values in the neighborhood
    for (int i = 0; i<len; i++) {
        dst[i*stride] = sum/ww;  // average
        sum+=(buffer[i+ww] - buffer[i]);  // substract outgoing pixel and add incoming pixel
    }

    delete [] buffer; // delete buffer
}
