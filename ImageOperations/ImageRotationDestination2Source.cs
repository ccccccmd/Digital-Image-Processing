﻿using System;
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;

public class ImageRotation
{
    public Bitmap DoImageRotationCertainAngle(double angle, string path)
    {
        Bitmap source = GetImage2Bitmap(path);
        return DoImageRotationCertainAngle(angle, source);
    }

    public Bitmap DoImageRotationCertainAngle(double angle, Bitmap source)
    {
        int iw = 0, ih = 0;
        int sW = 0, sH = 0;
        int nW = 0, nH = 0;
        int rposH = 0, rposW = 0;
        int i = 0, m = 0, n = 0;
        int tR = 0, tG = 0, tB = 0;
        int cur = 0, pos = 0, mask = 255;
        double CH = 0.0, CW = 0.0;

        int[] validKey = new int[9];
        int[] validValue = new int[9];

        double correct = 0.5;
        double sinA = 0.0, cosA = 0.0;

        while (angle < 0) { angle += 360; }

        angle %= 360;
        if (angle > 180) { angle = angle - 360; }

        double radian = TranslateAngleToRadian(angle);
        sinA = Math.Sin(radian);
        cosA = Math.Cos(radian);

        sW = source.Width;
        sH = source.Height;

        nW = Convert.ToInt32(correct + Math.Abs(sH * sinA) + Math.Abs(sW * cosA));
        nH = Convert.ToInt32(correct + Math.Abs(sH * cosA) + Math.Abs(sW * sinA));

        CH = 0.5 * (sH - nH * cosA + nW * sinA);
        CW = 0.5 * (sW - nH * sinA - nW * cosA);

        while (nW % 4 != 0) { ++nW; }

        Bitmap destin = new Bitmap(nW, nH);

        BitmapData sourceData = source.LockBits(new Rectangle(0, 0, sW, sH), ImageLockMode.ReadOnly, PixelFormat.Format24bppRgb);
        BitmapData destinData = destin.LockBits(new Rectangle(0, 0, nW, nH), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);

        unsafe
        {
            byte* sourcePtr = (byte*)sourceData.Scan0;
            byte* destinPtr = (byte*)destinData.Scan0;

            for (ih = 0; ih < nH; ++ih)
            {
                for (iw = 0; iw < nW; ++iw)
                {
                    cur = 0;
                    pos = 0;

                    rposH = Convert.ToInt32(ih * cosA - iw * sinA + CH);
                    rposW = Convert.ToInt32(ih * sinA + iw * cosA + CW);

                    for (i = 0; i < 9; validKey[i] = validValue[i] = -1, ++i) ;

                    for (m = rposH - 1; m <= rposH + 1; ++m)
                    {
                        if (m >= 0 && m < sH)
                        {
                            for (n = rposW - 1; n <= rposW + 1; ++n)
                            {
                                if (n >= 0 && n < sW)
                                {
                                    cur = m * sourceData.Stride + n * 3;
                                    tR = sourcePtr[cur + 0];
                                    tG = sourcePtr[cur + 1];
                                    tB = sourcePtr[cur + 2];
                                    cur = (tR << 16) | (tG << 8) | tB;
                                    for (i = 0; i < 9; ++i)
                                    {
                                        if (validKey[i] == cur)
                                        {
                                            validValue[i] += 1;
                                            break;
                                        }
                                    }
                                    if (i == 9)
                                    {
                                        validKey[pos] = cur;
                                        validValue[pos] = 1;
                                        pos += 1;
                                    }
                                }
                            }
                        }
                    }

                    cur = 0;
                    m = 16777215;

                    for (i = 0; i < pos; ++i)
                    {
                        if (validValue[i] > cur)
                        {
                            m = validKey[i];
                            cur = validValue[i];
                        }
                    }
                    cur = ih * destinData.Stride + iw * 3;
                    destinPtr[cur + 0] = (byte)(m >> 16);
                    destinPtr[cur + 1] = (byte)((m >> 8) & mask);
                    destinPtr[cur + 2] = (byte)(m & mask);
                }
            }
            source.UnlockBits(sourceData);
            destin.UnlockBits(destinData);
        }

        GC.Collect(2);

        return destin;
    }

    public Bitmap GetImage2Bitmap(string path)
    {
        if (!File.Exists(path))
        {
            throw new Exception("File is not exist.");
        }

        Bitmap source = new Bitmap(path);

        return source;
    }

    public double TranslateAngleToRadian(double angle)
    {
        return Math.PI * angle / 180.0;
    }
}
