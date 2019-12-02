#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "BingXingBianJieDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"
 
BingXingBianJieDib::BingXingBianJieDib()
{
}
BingXingBianJieDib::~BingXingBianJieDib()
{
}

// ��ɫͼ��ҶȻ�
void BingXingBianJieDib::RgbToGray()
{
    if (GetRGB())
        return;

    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_data = GetData();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            BYTE b = p_data[lineBytes * y + x * 3];
            BYTE g = p_data[lineBytes * y + x * 3 + 1];
            BYTE r = p_data[lineBytes * y + x * 3 + 2];

            BYTE gray = (BYTE)(0.11 * r + 0.59 * g + 0.3 * b); // RGB -> gray

            p_data[lineBytes * y + x * 3] = gray;
            p_data[lineBytes * y + x * 3 + 1] = gray;
            p_data[lineBytes * y + x * 3 + 2] = gray;
        }
    }
}

void BingXingBianJieDib::Lunkuotiqu2(int T)
{
    this->RgbToGray();
    //this->Lunkuotiqu(T);

    int width = this->GetWidth(); // ԭͼ���
    int height = this->GetHeight(); // ԭͼ�߶�
    int lineBytes = this->GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��
    int size = lineBytes * height;
    LPBYTE p_data = this->GetData (); //ԭͼ������ָ��
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    // ��ֵ��
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            if (p_data[index] > T)
                p_data[index] = 255;
            else
                p_data[index] = 0;
        }
    }

    // �жϺڵ���Χ8�����Ƿ�ȫΪ�ڣ����ȫ�����ð�
    for (int y = 1; y < height - 1; y++) {
        for (int x = 3; x < width - 3; x++) {
            if (p_data[lineBytes * y + x * 3] == 0) {
                int n1 = p_data[lineBytes * (y - 1) + (x - 3) * 3];
                int n2 = p_data[lineBytes * (y - 1) + (x) * 3];
                int n3 = p_data[lineBytes * (y - 1) + (x + 3) * 3];
                int n4 = p_data[lineBytes * (y) + (x - 3) * 3];
                int n5 = p_data[lineBytes * (y) + (x + 3) * 3];
                int n6 = p_data[lineBytes * (y + 1) + (x - 3) * 3];
                int n7 = p_data[lineBytes * (y + 1) + (x) * 3];
                int n8 = p_data[lineBytes * (y + 1) + (x + 3) * 3];

                if (n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8 == 0) {
                    p_temp[lineBytes * y + x * 3] = 255; // �Ϳ��ڲ���
                    p_temp[lineBytes * y + x * 3 + 1] = 255;
                    p_temp[lineBytes * y + x * 3 + 2] = 255;
                }
                else {
                    p_temp[lineBytes * y + x * 3] = 0;
                    p_temp[lineBytes * y + x * 3 + 1] = 0;
                    p_temp[lineBytes * y + x * 3 + 2] = 0;
                }
            }
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

///***************************************************************/           
/*�������ƣ�Lunkuotiqu(int T)                                      
/*�������ͣ�void  
/*����������int T ��ֵ����ֵ
/*���ܣ���ͼ�����������ȡ���Զ�ֵͼ�������Ϳ��ڲ��㷨����            
/***************************************************************/ 
void BingXingBianJieDib::Lunkuotiqu(int T)
{
    int width = this->GetWidth(); // ԭͼ���
    int height = this->GetHeight(); // ԭͼ�߶�
    int lineBytes = this->GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��
    int size = lineBytes * height;
    LPBYTE p_data = this->GetData (); //ԭͼ������ָ��
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    width = GetRGB() ? width : lineBytes;

    // ��ֵ��
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = lineBytes * y + x;
            if (p_data[index] > T)
                p_data[index] = 255;
            else
                p_data[index] = 0;
        }
    }

    // �жϺڵ���Χ8�����Ƿ�ȫΪ�ڣ����ȫ�����ð�
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (p_data[lineBytes * y + x] == 0) {
                int n1 = p_data[lineBytes * (y - 1) + (x - 1)];
                int n2 = p_data[lineBytes * (y - 1) + (x)];
                int n3 = p_data[lineBytes * (y - 1) + (x + 1)];
                int n4 = p_data[lineBytes * (y) + (x - 1)];
                int n5 = p_data[lineBytes * (y) + (x + 1)];
                int n6 = p_data[lineBytes * (y + 1) + (x - 1)];
                int n7 = p_data[lineBytes * (y + 1) + (x)];
                int n8 = p_data[lineBytes * (y + 1) + (x + 1)];
                
                if (n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8 == 0)
                    p_temp[lineBytes * y + x] = 255; // �Ϳ��ڲ���
                else
                    p_temp[lineBytes * y + x] = 0;
            }
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

///***************************************************************/           
/*�������ƣ�Lunkuogenzong()                                      
/*�������ͣ�void      
/*����������int T ��ֵ����ֵ
/*���ܣ���ͼ����б߽���١�            
/***************************************************************/ 
void BingXingBianJieDib::Lunkuogenzong(int T)
{
    if (GetRGB() == NULL)
        return;

    int width = this->GetWidth(); // ԭͼ���
    int height = this->GetHeight(); // ԭͼ�߶�
    int lineBytes = this->GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��
    int size = lineBytes * height;
    LPBYTE p_data = this->GetData (); //ԭͼ������ָ��
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    ////width = GetRGB() ? width : lineBytes;

    // ��ֵ��
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            if (p_data[index] > T)
                p_data[index] = 255;
            else
                p_data[index] = 0;
        }
    }

    // �˸��������ʼɨ�跽��
    const int direction[8][2]={
        {-1, 1},  // ����
        {0, 1},   // ��
        {1, 1},   // ����
        {1, 0},   // ��
        {1, -1},  // ����
        {0, -1},  // ��
        {-1, -1}, // ���� 
        {-1, 0}   // ��
    };

    Point startPoint; // ��ʼ�߽��

    // 1.���ҵ������Ϸ��ı߽��
    bool bFindStartPoint = false;
    for (int y = 0; y < height && !bFindStartPoint; y++) {
        for (int x = 0; x < width && !bFindStartPoint; x++) {
            if (p_data[lineBytes * y + x] == 0) {
                p_temp[lineBytes * y + x] = 0;
                bFindStartPoint = true;
                startPoint.Width = x;
                startPoint.Height = y;
            }
        }
    }

    // 2.������ʼ���������·�������ʼɨ�������Ϸ���
    int beginDirect = 0;

    // ����ʼ�㿪ʼɨ��
    Point pt;
    Point currentPoint = startPoint;
    while (true) {
        // ��ɨ�跽��鿴һ������
        pt.Width = currentPoint.Width  + direction[beginDirect][0];
        pt.Height = currentPoint.Height + direction[beginDirect][1];
        if (pt.Width < 0 || pt.Width >= width || pt.Height < 0 || pt.Height >= height) {
            // ɨ�跽��˳ʱ����תһ��
            beginDirect++;
            if(beginDirect == 8)
                beginDirect = 0;
            continue;
        }
 
        int pixel = p_data[lineBytes * pt.Height + pt.Width]; 
        if (pixel == 0) {
            currentPoint = pt;
            p_temp[lineBytes * pt.Height + pt.Width] = 0; // �߽��
            if(currentPoint.Height == startPoint.Height && currentPoint.Width == startPoint.Width)
                break;

            // ɨ��ķ�����ʱ����ת����
            beginDirect--;
            if(beginDirect == -1)
                beginDirect = 7;
            beginDirect--;
            if(beginDirect == -1)
                beginDirect = 7;
        }
        else {
            // ɨ�跽��˳ʱ����תһ��
            beginDirect++;
            if(beginDirect == 8)
                beginDirect = 0;
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp; 
}

bool PointExist(Seed *pSeeds, int stackPoint, int iCurrentPixelx, int iCurrentPixely)
{
    for (int i = 0; i < stackPoint; i++)
    {
        if (pSeeds[i].Width == iCurrentPixelx && pSeeds[i].Width == iCurrentPixely) 
            return true;
    }
    
    return false;
}

///***************************************************************/           
/*�������ƣ�Zhongzitianchong(CPoint SeedPoint)                                      
/*�������ͣ�void
/*����˵����SeedPoint  ---ѡ�����ӵ�  
           int T ��ֵ����ֵ
/*���ܣ���ͼ�����������䡣            
/***************************************************************/ 
void BingXingBianJieDib::Zhongzitianchong(CPoint SeedPoint, int T)
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    int size = lineBytes * height;
    LPBYTE p_data = GetData();

    // ���Ӷ�ջ��ָ��
    Seed *pSeeds;
    int stackPoint;
    
    // ��ǰ����λ��
    int iCurrentPixelx, iCurrentPixely;

    if (GetRGB()) {
        // ��ֵ��
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < lineBytes; x++) {
                int index = lineBytes * y + x;
                if (p_data[index] > T)
                    p_data[index] = 255;
                else
                    p_data[index] = 0;
            }
        }

        //��ʼ������
        pSeeds = new Seed[size];
        pSeeds[1].Width = SeedPoint.x;
        pSeeds[1].Height = SeedPoint.y;
        stackPoint = 1;

        while (stackPoint != 0) {
            //ȡ������
            iCurrentPixelx = pSeeds[stackPoint].Width;
            iCurrentPixely = pSeeds[stackPoint].Height;
            stackPoint--;

            //����ǰ��Ϳ��
            p_data[lineBytes * (height - iCurrentPixely - 1) + iCurrentPixelx] = 0;

            // �ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
            if (iCurrentPixelx > 1) { //ע���ֹԽ��
                int left_pixel = p_data[lineBytes * (height - iCurrentPixely - 1) + (iCurrentPixelx - 1)];
                if (left_pixel == 255) {
                    stackPoint++;
                    pSeeds[stackPoint].Width = iCurrentPixelx - 1;
                    pSeeds[stackPoint].Height = iCurrentPixely;  
                }
            }

            // �ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
            if (iCurrentPixely > 1) { //ע���ֹԽ��
                int up_pixel = p_data[lineBytes * (height - iCurrentPixely) + iCurrentPixelx];
                if (up_pixel == 255) {
                    stackPoint++;
                    pSeeds[stackPoint].Width = iCurrentPixelx;
                    pSeeds[stackPoint].Height = iCurrentPixely - 1;
                }
            }

            // �ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
            if (iCurrentPixelx < width - 1) { //ע���ֹԽ��
                int right_pixel = p_data[lineBytes * (height - iCurrentPixely - 1) + (iCurrentPixelx + 1)];
                if (right_pixel == 255) {
                    stackPoint++;
                    pSeeds[stackPoint].Width = iCurrentPixelx + 1;
                    pSeeds[stackPoint].Height = iCurrentPixely;
                }
            }

            // �ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
            if (iCurrentPixely < height - 1) { //ע���ֹԽ��
                int down_pixel = p_data[lineBytes * (height - iCurrentPixely - 2) + iCurrentPixelx];
                if (down_pixel == 255) {
                    stackPoint++;
                    pSeeds[stackPoint].Width = iCurrentPixelx;
                    pSeeds[stackPoint].Height = iCurrentPixely + 1;
                }
            }
        }

        //�ͷŶ�ջ
        delete pSeeds;
    }
    else {
        //this->RgbToGray();
        // ��ֵ��
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < lineBytes; x++) {
                int index = lineBytes * y + x;
                if (p_data[index] > T)
                    p_data[index] = 255;
                else
                    p_data[index] = 0;
            }
        }

        pSeeds = new Seed[size];
        for (int k = 0; k < 3; k++) { // bgr ��ͨ��
            //��ʼ������
            stackPoint = k + 3;
            pSeeds[stackPoint].Width = SeedPoint.x * 3 + k;
            pSeeds[stackPoint].Height = SeedPoint.y;
        
            while (stackPoint != k) {
                //ȡ������
                iCurrentPixelx = pSeeds[stackPoint].Width;
                iCurrentPixely = pSeeds[stackPoint].Height;
                stackPoint -= 3;

                //����ǰ��Ϳ��
                p_data[lineBytes * (height - iCurrentPixely - 1) + iCurrentPixelx] = 0;

                // �ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
                if (iCurrentPixelx > 3) { //ע���ֹԽ��
                    int left_pixel = p_data[lineBytes * (height - iCurrentPixely - 1) + (iCurrentPixelx - 3)];
                    if (left_pixel == 255) {
                        stackPoint += 3;
                        pSeeds[stackPoint].Width = iCurrentPixelx - 3;
                        pSeeds[stackPoint].Height = iCurrentPixely;  
                    }
                }

                // �ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
                if (iCurrentPixely > 1) { //ע���ֹԽ��
                    int up_pixel = p_data[lineBytes * (height - iCurrentPixely) + iCurrentPixelx];
                    if (up_pixel == 255) {
                        stackPoint += 3;
                        pSeeds[stackPoint].Width = iCurrentPixelx;
                        pSeeds[stackPoint].Height = iCurrentPixely - 1;
                    }
                }

                // �ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
                if (iCurrentPixelx < width * 3 - 3) { //ע���ֹԽ��
                    int right_pixel = p_data[lineBytes * (height - iCurrentPixely - 1) + (iCurrentPixelx + 3)];
                    if (right_pixel == 255) {
                        stackPoint += 3;
                        pSeeds[stackPoint].Width = iCurrentPixelx + 3;
                        pSeeds[stackPoint].Height = iCurrentPixely;
                    }
                }

                // �ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
                if (iCurrentPixely < height - 1) { //ע���ֹԽ��
                    int down_pixel = p_data[lineBytes * (height - iCurrentPixely - 2) + iCurrentPixelx];
                    if (down_pixel == 255) {
                        stackPoint += 3;
                        pSeeds[stackPoint].Width = iCurrentPixelx;
                        pSeeds[stackPoint].Height = iCurrentPixely + 1;
                    }
                }
            }
        }

        ////��ʼ������
        //pSeeds = new Seed[size];
        //pSeeds[1].Width = SeedPoint.x * 3;
        //pSeeds[1].Height = SeedPoint.y;
        //stackPoint = 1;

        //while (stackPoint != 0) {
        //    //ȡ������
        //    iCurrentPixelx = pSeeds[stackPoint].Width;
        //    iCurrentPixely = pSeeds[stackPoint].Height;
        //    stackPoint--;

        //    //����ǰ��Ϳ��
        //    p_data[lineBytes * (height - iCurrentPixely - 1) + iCurrentPixelx] = 0;

        //    // �ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
        //    if (iCurrentPixelx > 1) { //ע���ֹԽ��
        //        int left_pixel = p_data[lineBytes * (height - iCurrentPixely - 1) + (iCurrentPixelx - 1)];
        //        if (left_pixel == 255) {
        //            stackPoint++;
        //            pSeeds[stackPoint].Width = iCurrentPixelx - 1;
        //            pSeeds[stackPoint].Height = iCurrentPixely;  
        //        }
        //    }

        //    // �ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
        //    if (iCurrentPixely > 1) { //ע���ֹԽ��
        //        int up_pixel = p_data[lineBytes * (height - iCurrentPixely) + iCurrentPixelx];
        //        if (up_pixel == 255) {
        //            stackPoint++;
        //            pSeeds[stackPoint].Width = iCurrentPixelx;
        //            pSeeds[stackPoint].Height = iCurrentPixely - 1;
        //        }
        //    }

        //    // �ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
        //    if (iCurrentPixelx < width * 3 - 1) { //ע���ֹԽ��
        //        int right_pixel = p_data[lineBytes * (height - iCurrentPixely - 1) + (iCurrentPixelx + 1)];
        //        if (right_pixel == 255) {
        //            stackPoint++;
        //            pSeeds[stackPoint].Width = iCurrentPixelx + 1;
        //            pSeeds[stackPoint].Height = iCurrentPixely;
        //        }
        //    }

        //    // �ж�����ĵ㣬���Ϊ�ף���ѹ���ջ
        //    if (iCurrentPixely < height - 1) { //ע���ֹԽ��
        //        int down_pixel = p_data[lineBytes * (height - iCurrentPixely - 2) + iCurrentPixelx];
        //        if (down_pixel == 255) {
        //            stackPoint++;
        //            pSeeds[stackPoint].Width = iCurrentPixelx;
        //            pSeeds[stackPoint].Height = iCurrentPixely + 1;
        //        }
        //    }
        //}

        //�ͷŶ�ջ
        delete pSeeds;
    }
}

///***************************************************************/           
/*�������ƣ�Quyuzengzhang(CPoint point)                                      
/*�������ͣ�void
/*����˵����point  ---���������                                     
/*���ܣ���ͼ���������������            
/***************************************************************/
void BingXingBianJieDib::Quyuzengzhang(CPoint point)
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();

    LPBYTE p_data = GetData();

    if (GetRGB()) {
        // �������ӵ�ĻҶ�ֵ
        BYTE zhongzhi = p_data[lineBytes * (height - point.y - 1) + point.x]; // ע�⣺ͼ����

        // ������������
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                BYTE temp = p_data[lineBytes * y + x];
                if (abs(temp - zhongzhi) < 10)
                    p_data[lineBytes * y + x] = zhongzhi; // ��ǰ��ͬ���ӻҶ�ֵ�ȽϽӽ�,�����ӵ�Ҷȴ���
                else
                    p_data[lineBytes * y + x] = 255;
            }
        }
    }
    else {
        // �������ӵ�ĻҶ�ֵ
        BYTE zhongzhi_b = p_data[lineBytes * (height - point.y - 1) + point.x * 3];
        BYTE zhongzhi_g = p_data[lineBytes * (height - point.y - 1) + point.x * 3 + 1];
        BYTE zhongzhi_r = p_data[lineBytes * (height - point.y - 1) + point.x * 3 + 2];

        // ������������
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                BYTE temp_b = p_data[lineBytes * y + x * 3];
                BYTE temp_g = p_data[lineBytes * y + x * 3 + 1];
                BYTE temp_r = p_data[lineBytes * y + x * 3 + 2];
                if (abs(temp_b - zhongzhi_b) < 10 && abs(temp_g - zhongzhi_g) < 10 && abs(temp_r - zhongzhi_r) < 10) {
                    p_data[lineBytes * y + x * 3] = zhongzhi_b; // ��ǰ��ͬ���ӻҶ�ֵ�ȽϽӽ�,�����ӵ�Ҷȴ���
                    p_data[lineBytes * y + x * 3 + 1] = zhongzhi_g;
                    p_data[lineBytes * y + x * 3 + 2] = zhongzhi_r;
                }
                else {
                    p_data[lineBytes * y + x * 3] = 255;
                    p_data[lineBytes * y + x * 3 + 1] = 255;
                    p_data[lineBytes * y + x * 3 + 2] = 255;
                }
            }
        }
    }
}

///***************************************************************/           
/*�������ƣ�Lunkuotiqu(CPoint SeedPoint)                                      
/*�������ͣ�void
/*����˵����SeedPoint  ---���ӵ�ѡȡ                                     
/*���ܣ���24λ��ɫͼ�����������ȡ��            
/***************************************************************/
void BingXingBianJieDib::Lunkuotiqu(CPoint SeedPoint)
{
    LPBYTE  p_data ;     //ԭͼ������ָ��
    int R,G,B,R1,G1,B1;
    int wide,height;    //ԭͼ������
    // ָ��Դͼ���ָ��
    LPBYTE    lpSrc;    
    // ָ�򻺴�ͼ���ָ��
    LPBYTE    lpDst;    
    // ָ�򻺴�DIBͼ���ָ��
    LPBYTE    temp;    
    //ѭ������
    int i;
    int j;
    p_data=GetData();
    wide=GetWidth();
    height=GetHeight();
    temp = new BYTE[wide * height*3];
    // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
    memset(temp,  255, wide * height*3);
    lpSrc=p_data+wide*3*(height-SeedPoint.y)+3*SeedPoint.x;
    R1=*lpSrc;
    lpSrc++;
    G1=*lpSrc;
    lpSrc++;
    B1=*lpSrc;
    for(j=0;j<height;j++)
    {
        for(i=0;i<wide;i++)
        {
            lpSrc = (LPBYTE)p_data + wide * j*3 + i*3;
            // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��            
            lpDst = (LPBYTE)temp + wide * j*3 + i*3;
            int I;
            I=0;
            for(int k=i-1;k<i+2;k++)
            {
                for(int l=j-1;l<j+2;l++)
                {
                    if(k>=0&&l>=0&&k<wide&&l<height)
                    {
                        R=*(p_data+l*wide*3+k*3);
                        G=*(p_data+l*wide*3+k*3+1);
                        B=*(p_data+l*wide*3+k*3+2);
                        if(abs(R-R1)<10&&abs(G-G1)<10&&abs(B-B1)<10)
                            I++;
                    }
                }
            }
            if(I!=9)
            {    
                *lpDst=*lpSrc;
                *(lpDst+1)=*(lpSrc+1);
                *(lpDst+2)=*(lpSrc+2);
            } 
            else
            {
                *lpDst=255;
                *(lpDst+1)=255;
                *(lpDst+2)=255;
            }
        }
    }
    memcpy(p_data,temp, wide * height*3);
    delete temp;
}

/*************************************************************/
//���㷨ֻ����������б߽��ͼ��//
/*************************************************************/
void BingXingBianJieDib::Lunkuogenzong(CPoint SeedPoint)
{
    // ָ��Դͼ���ָ��
    LPBYTE    lpSrc;
    LPBYTE   p_data ;
    int R1,G1,B1;
    int R,G,B;
    // ָ�򻺴�ͼ���ָ��
    LPBYTE    lpDst;
    // ָ�򻺴�DIBͼ���ָ��
    LPBYTE    temp;
    int wide;
    int height; 
    //ѭ������
    int i;
    int j;
    //����ֵ
    int pixel;
    //�Ƿ��ҵ���ʼ�㼰�ص���ʼ��
    bool bFindStartPoint;
    //�Ƿ�ɨ�赽һ���߽��
    bool bFindPoint;
    //��ʼ�߽���뵱ǰ�߽��
    Point StartPoint,CurrentPoint;
    //�˸��������ʼɨ�跽��
    int Direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};
    int BeginDirect;
    p_data=GetData();
    wide=this->GetWidth();
    height=this->GetHeight();
    // ��ʱ�����ڴ棬�Ա�����ͼ��
    temp = new BYTE[wide*height*3];
    // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
    lpDst = temp;
    memset(temp, (BYTE)255, wide * height*3);
    //���ҵ����󷽵ı߽��
    lpSrc=p_data+wide*3*(height-SeedPoint.y)+3*SeedPoint.x;//ȷ�������ɫ��ֵ
    R1=*lpSrc;
    lpSrc++;
    G1=*lpSrc;
    lpSrc++;
    B1=*lpSrc;
    bFindStartPoint = false;
    int s= height-SeedPoint.y;
    for(i=SeedPoint.x;i>1;i--)  //�����㿪ʼ����Ѱ�ұ߽���ʼ��
    {
        lpSrc = (LPBYTE)p_data + wide * (height-SeedPoint.y)*3 + i*3;
        // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��            
        lpDst = (LPBYTE)temp + wide * (height-SeedPoint.y)*3 + i*3;
        int count;
        count=0;
        for(int k=i-1;k<i+2;k++)
        {
            for(int l=s-1;l<s+2;l++)
            {
                if(k>=0&&l>=0&&k<wide&&l<height)
                {
                    R=*(p_data+l*wide*3+k*3);
                    G=*(p_data+l*wide*3+k*3+1);
                    B=*(p_data+l*wide*3+k*3+2);
                    if(abs(R-R1)<10&&abs(G-G1)<10&&abs(B-B1)<10)
                        count++;
                }
            }
        }
        if(count!=9)
        {
            bFindStartPoint = true;
            StartPoint.Height = s;
            StartPoint.Width = i;
            // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��            
            lpDst = (LPBYTE)(temp + wide * s*3 + i*3);    
            *lpDst = 0;
            *(lpDst+1)=0;
            *(lpDst+2)=0;
            break;
        } 
    }            
    //������ʼ���������·�������ʼɨ�������Ϸ���
    BeginDirect = 0;
    //���ٱ߽�
    bFindStartPoint = false;
    //�ӳ�ʼ�㿪ʼɨ��
    CurrentPoint.Height = StartPoint.Height;
    CurrentPoint.Width = StartPoint.Width;
    while(!bFindStartPoint)
    {
        bFindPoint = false;
        while(!bFindPoint)
        {
            //��ɨ�跽��鿴һ������
            lpSrc = (LPBYTE)(p_data + 3*wide * ( CurrentPoint.Height + Direction[BeginDirect][1])
                + 3*(CurrentPoint.Width + Direction[BeginDirect][0]));
            R =*lpSrc;
            G=*(lpSrc+1);
            B=*(lpSrc+2);
            if(abs(R-R1)<10&&abs(G-G1)<10&&abs(B-B1)<10)
            {
                bFindPoint = true;
                CurrentPoint.Height = CurrentPoint.Height + Direction[BeginDirect][1];
                CurrentPoint.Width = CurrentPoint.Width + Direction[BeginDirect][0];
                if(CurrentPoint.Height == StartPoint.Height && CurrentPoint.Width == StartPoint.Width)
                {
                    bFindStartPoint = true;
                }
                lpDst =  (LPBYTE)(temp + 3*wide * CurrentPoint.Height + 3*CurrentPoint.Width);
                *lpDst = *lpSrc;
                *(lpDst+1) = *(lpSrc+1);
                *(lpDst+2) = *(lpSrc+2);
                //ɨ��ķ�����ʱ����ת����
                BeginDirect--;
                if(BeginDirect == -1)
                    BeginDirect = 7;
                BeginDirect--;
                if(BeginDirect == -1)
                    BeginDirect = 7;
            }
            else
            {
                //ɨ�跽��˳ʱ����תһ��
                BeginDirect++;
                if(BeginDirect == 8)
                    BeginDirect = 0;
            }
        }
    }
    // ����ͼ��
    memcpy(p_data, temp, 3*wide * height);
    // �ͷ��ڴ�
    delete temp; 
}

///**************************************************************/           
/*�������ƣ�Fenbutongji(int *huidu)                               
/*�������ͣ�void                                         
/*����˵����huidu �Ҷȷֲ�ͳ��                                                        
/*���ܣ���ͼ����лҶ�ֱ��ͼͳ�ơ�                     
/***************************************************************/
void BingXingBianJieDib::Fenbutongji(int *huidu)
{
#if _DEBUG
    long sum = 0;
    memset(huidu, 0, sizeof(int) * 256);
    for (int i = 0; i < 256; i++)
        sum += huidu[i];
    ASSERT(sum == 0);
#endif

    int width = this->GetWidth();   // ��     
    int height = this->GetHeight(); // ��
    int lineBytes = this->GetDibWidthBytes(); // 4 �ֽڶ����Ŀ��

    LPBYTE p_data = this->GetData(); // ����ָ��

    // �Ը����ؽ��лҶ�ͳ��
    if (this->GetRGB()) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                BYTE gray = p_data[lineBytes * y + x];
                huidu[gray]++; // �Ҷ�ͳ�Ƽ���
            }
        }
    } else {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                BYTE b = p_data[lineBytes * y + x * 3];
                BYTE g = p_data[lineBytes * y + x * 3 + 1];
                BYTE r = p_data[lineBytes * y + x * 3 + 2];
                BYTE gray = (BYTE)(0.11 * r + 0.59 * g + 0.3 * b); // 24λ��ɫͼת�Ҷ�
                huidu[gray]++; // �Ҷ�ͳ�Ƽ���
            }
        }
    }
}

///**************************************************************/           
/*�������ƣ�Zhifangtu(float *tongji)                               
/*�������ͣ�void                                         
/*����˵����tongji �Ҷȷֲ��ܶ�ͳ��                                                        
/*���ܣ���ͼ����лҶ�ֱ��ͼͳ�ơ�                     
/***************************************************************/
void BingXingBianJieDib::Zhifangtu(float *tongji)
{
    int huidu[256] = {};  // �Ҷȼ���   
    int width = this->GetWidth();   // ��     
    int height = this->GetHeight(); // ��
    int lineBytes = this->GetDibWidthBytes(); // 4 �ֽڶ����Ŀ��

    LPBYTE p_data = this->GetData(); // ����ָ��

    // �Ը����ؽ��лҶ�ͳ��
    if (this->GetRGB()) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                BYTE gray = p_data[lineBytes * y + x];
                huidu[gray]++; // �Ҷ�ͳ�Ƽ���
            }
        }
    } else {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                BYTE b = p_data[lineBytes * y + x * 3];
                BYTE g = p_data[lineBytes * y + x * 3 + 1];
                BYTE r = p_data[lineBytes * y + x * 3 + 2];
                BYTE gray = (BYTE)(0.11 * r + 0.59 * g + 0.3 * b); // 24λ��ɫͼת�Ҷ�
                huidu[gray]++; // �Ҷ�ͳ�Ƽ���
            }
        }
    }
    
    // ����Ҷȷֲ��ܶ�
    for(int i = 0; i < 256; i++) {
        tongji[i] = huidu[i] / (height * width * 1.0f);
    }
}

///***************************************************************/           
/*�������ƣ�BanYuZhi(int Yuzhi)                                      
/*�������ͣ�void
/*����˵����Yuzhi  ---��ֵѡȡ                                     
/*���ܣ���ͼ����а���ֵ�ָ            
/***************************************************************/
void BingXingBianJieDib::BanYuZhi(int Yuzhi)
{
    this->RgbToGray();

    int width = this->GetWidth(); // ԭͼ���
    int height = this->GetHeight(); // ԭͼ�߶�
    int lineBytes = this->GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��
    LPBYTE p_data = this->GetData (); //ԭͼ������ָ��

    for (int i = 0; i < lineBytes * height; i++) {
        if(p_data[i] - Yuzhi < 30)
            ;               // ���ֲ���
        else
            p_data[i] = 255;
    }
}

///***************************************************************/           
/*�������ƣ�Yuzhifenge(int Yuzhi)                                      
/*�������ͣ�void
/*����˵����Yuzhi  ---��ֵѡȡ                                     
/*���ܣ���ͼ�������ֵ�ָ            
/***************************************************************/
void BingXingBianJieDib::Yuzhifenge(int Yuzhi)
{
    this->RgbToGray();

    int width = this->GetWidth(); // ԭͼ���
    int height = this->GetHeight(); // ԭͼ�߶�
    int lineBytes = this->GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��
    LPBYTE p_data = this->GetData (); //ԭͼ������ָ��

    for (int i = 0; i < lineBytes * height; i++) {
        if(abs(p_data[i] - Yuzhi) < 30)
            p_data[i] = Yuzhi; // ǰ��
        else
            p_data[i] = 255;   // ����
    }
}

///***************************************************************/           
/*�������ƣ�Diedaifazhi(int *tongji)                                       
/*�������ͣ�void                                      
/*���ܣ���ͼ����е�����ֵѡȡ��            
/***************************************************************/ 
void BingXingBianJieDib::Diedaifazhi()
{
    int huidu[256];
    this->Fenbutongji(huidu);  // ֱ��ͼ�Ҷ�ֵͳ��
    
    int width = this->GetWidth();   // ��     
    int height = this->GetHeight(); // ��
    int lineBytes = this->GetDibWidthBytes(); // 4 �ֽڶ����Ŀ��
    LPBYTE p_data = this->GetData(); // ����ָ��

    int T1 = 127; // ��ʼ��ֵ
    int T2 = 0; // ������ֵ

    int Temp0, Temp1, Temp2, Temp3;
    Temp0 = Temp1 = Temp2 = Temp3 = 0;

    while (true)
    {
        // ������һ��������ֵ
        for (int i = 0; i < T1 + 1; i++) // ����1
        {
            Temp0 += huidu[i] * i;
            Temp1 += huidu[i];
        }
        for (int i = T1 + 1; i < 256; i++) // ����2
        {
            Temp2 += huidu[i] * i;
            Temp3 += huidu[i];
        }
        T2 = (Temp0 / Temp1 + Temp2 / Temp3) / 2;
        // ����������Ƿ�������
        if (T1 == T2)
            break;
        else
            T1 = T2;

        if (T2 < 0)
            break;

        Temp0 = Temp1 = Temp2 = Temp3 = 0;
    }

    this->RgbToGray();

    // �Ը����ؽ��ж�ֵ��
    for (int k = 0; k < lineBytes * height; k++)
    { 
        if (p_data[k] > T1)
            p_data[k] = 255;
        else
            p_data[k] = 0;
    }
}
