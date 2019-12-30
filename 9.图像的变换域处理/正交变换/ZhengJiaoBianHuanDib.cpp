#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "ZhengJiaoBianHuanDib.h"
#include "malloc.h"
#include "MainFrm.h"
#include "DSplitView.h"
#include "Cdib.h"

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 



#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
#define pi 3.14159265359
#define PI 3.1415926535 
 
ZhengJiaoBianHuanDib::ZhengJiaoBianHuanDib()
{
}
ZhengJiaoBianHuanDib::~ZhengJiaoBianHuanDib()
{
}

CNumber ZhengJiaoBianHuanDib::Add(const CNumber &c1, const CNumber &c2)
{
    CNumber c;
    c.re = c1.re + c2.re;
    c.im = c1.im + c2.im;
    return c;
}
CNumber ZhengJiaoBianHuanDib::Sub(const CNumber &c1, const CNumber &c2)
{
    CNumber c;
    c.re = c1.re - c2.re;
    c.im = c1.im - c2.im;
    return c;
}
CNumber ZhengJiaoBianHuanDib::Mul(const CNumber &c1, const CNumber &c2)
{
    CNumber c;
    c.re = c1.re * c2.re - c1.im * c2.im;
    c.im = c1.re * c2.im + c2.re * c1.im;
    return c;
}


/*************************************************************************
 *
 * �������ƣ�FFT(const CNumber *t, const CNumber *f, int r)
 *
 * ����:
 *   CNumber * t                - ָ��ʱ��ֵ��ָ��
 *   CNumber * f                - ָ��Ƶ��ֵ��ָ��
 *   r                         ��2������
 *
 * ����ֵ:
 *   �ޡ�
 *
 * ˵��:
 *   �˺���ʵ��һά���ٸ���Ҷ�任��
 *
 ************************************************************************/
void ZhengJiaoBianHuanDib::FFT(const CNumber *t, CNumber *f, int r)
{
    long count; // ����Ҷ�任����
    int i, j, k, p, bfsize;
    CNumber *w, *x, *a, *b; // �����ṹ���͵�ָ�����������wָ���Ȩϵ��
    double angle; // �����Ȩϵ�����õĽǶ�ֵ

    count = 1 << r; // ���㸵��Ҷ�任����
    w = new CNumber[count / 2];
    a = new CNumber[count];
    b = new CNumber[count];

    // �����Ȩϵ��
    for (i = 0; i < count / 2; i++)
    {
        angle = - i * pi * 2 / count;
        w[i].re = cos(angle);
        w[i].im = sin(angle);
    }

    memcpy(a, t, sizeof(CNumber) * count);

    // ����Ƶ�ʷֽⷨ���е�������
    for (k = 0; k < r; k++) // ��������(1, 2, 3...)
    {
        for (j = 0; j < (1 << k); j++) // ����ÿ��DFT(1, 2, 4...)
        {
            bfsize = 1 << (r - k); // ��������(N, N/2, N/4...)
            for (i = 0; i < bfsize / 2; i++) // �۰����(N/2, N/4, N/8...)
            {
                p = j * bfsize;
                b[i + p] = Add(a[i + p], a[i + p + bfsize / 2]); // ǰһ��
                b[i + p + bfsize / 2] = Mul(Sub(a[i + p], a[i + p + bfsize / 2]), w[i * (1 << k)]); // ��һ��
            }
        }

        // ��1�������������Ϊ��2�������룬���յĵ�����������a��
        x = a;
        a = b;
        b = x;
    }

    // ������ı任������������
    for (j = 0; j < count; j++)
    {
        p = 0;
        for (i = 0; i < r; i++) // ����������λ
        {
            if (j & (1 << i))
            {
                p += 1 << (r - i - 1); // ���ط�ת
            }
        } //  for i
        f[j] = a[p];
    } // for j

    delete [] w;
    delete [] a;
    delete [] b;
}

///////////////////////////////////////////////
//�˺�������ʵ��ͼ��ĸ���Ҷ�任��ԭʼ�任��
//���ε��ÿ��ٸ���Ҷ�任FFT()ʵ�ֶ�ά����Ҷ�任
///////////////////////////////////////////////
void ZhengJiaoBianHuanDib::QuickFourier()
{
    LONG width = GetWidth();     // DIB�Ŀ��
    LONG height = GetHeight();   // DIB�ĸ߶�
    LONG dibWidth = WIDTHBYTES(width * 8);   // ȡ��ԭͼ��ÿ���ֽ�����4�ֽڶ��룩

    LPBYTE p_data = GetData(); // ָ��DIB����ָ��

    LONG w = 1; // ���и���Ҷ�任�Ŀ�ȣ�2�������η���
    LONG h = 1; // ���и���Ҷ�任�ĸ߶ȣ�2�������η���

    int wp = 0; // �����������
    int hp = 0; // �߷����������
    
    CNumber *t; // ʱ������
    CNumber *f; // Ƶ������

    // ������и���Ҷ�任�Ŀ�ȣ�2�������η���
    while (w * 2 <= width)
    {
        w *= 2;
        wp++;
    }
    // ������и���Ҷ�任�ĸ߶ȣ�2�������η���
    while (h * 2 <= width)
    {
        h *= 2;
        hp++;
    }

    // �������ݴ洢���ռ�
    t = new CNumber[w * h];
    f = new CNumber[w * h];

    // ��ʱ��ֵ
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            t[x + w * y].re = p_data[dibWidth * (height - y - 1) + x];
            t[x + w * y].im = 0;
        }
    }
    
    // �ڴ�ֱ�����Ͻ��п��ٸ���Ҷ�任
    for (int y = 0; y < h; y++)
    {
        this->FFT(&t[y * w], &f[y * w], wp);
    }
    
    // ת���任���
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            t[y + h * x] = f[x + w * y]; // ��DIB��������
        }
    }

    // ��ˮƽ�����Ͻ��п��ٸ���Ҷ�任
    for (int x = 0; x < w; x++)
    {
        this->FFT(&t[x * h], &f[x * h], hp);
    }
    
    // ����Ҷ�任�任���
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            int index = y + x * h;
            int temp = sqrt(f[index].re * f[index].re + f[index].im * f[index].im) / 100; // ����
            if (temp > 255)
                temp=255;
            p_data[dibWidth * (height - y - 1) + x] = (BYTE)(temp); 
        }
    }

    delete [] t;
    delete [] f;
}

///////////////////////////////////////////////
//��ά����Ҷ�任ԭ��ƽ�ƣ�ʵ��һ��ʱ�� * ƽ�����ӣ�
///////////////////////////////////////////////
void ZhengJiaoBianHuanDib::QuickFourier1()
{
    LONG width = GetWidth();     // DIB�Ŀ��
    LONG height = GetHeight();   // DIB�ĸ߶�
    LONG dibWidth = WIDTHBYTES(width * 8);   // ȡ��ԭͼ��ÿ���ֽ�����4�ֽڶ��룩

    LPBYTE p_data = GetData(); // ָ��DIB����ָ��

    LONG w = 1; // ���и���Ҷ�任�Ŀ�ȣ�2�������η���
    LONG h = 1; // ���и���Ҷ�任�ĸ߶ȣ�2�������η���

    int wp = 0; // �����������
    int hp = 0; // �߷����������

    CNumber *t; // ʱ������
    CNumber *f; // Ƶ������

    // ������и���Ҷ�任�Ŀ�ȣ�2�������η���
    while (w * 2 <= width)
    {
        w *= 2;
        wp++;
    }
    // ������и���Ҷ�任�ĸ߶ȣ�2�������η���
    while (h * 2 <= width)
    {
        h *= 2;
        hp++;
    }

    // �������ݴ洢���ռ�
    t = new CNumber[w * h];
    f = new CNumber[w * h];

    // ��ʱ��ֵ
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            /////////////////////////////////
            int deltaXY = (x + y) % 2 ? -1 : 1; // (-1)^(x + y) ƽ������
            /////////////////////////////////
            t[x + w * y].re = p_data[dibWidth * (height - y - 1) + x] * deltaXY;
            t[x + w * y].im = 0;
        }
    }

    // �ڴ�ֱ�����Ͻ��п��ٸ���Ҷ�任
    for (int y = 0; y < h; y++)
    {
        this->FFT(&t[y * w], &f[y * w], wp);
    }

    // ת���任���
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            t[y + h * x] = f[x + w * y]; // ��DIB��������
        }
    }

    // ��ˮƽ�����Ͻ��п��ٸ���Ҷ�任
    for (int x = 0; x < w; x++)
    {
        this->FFT(&t[x * h], &f[x * h], hp);
    }

    // ����Ҷ�任�任���
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            int index = y + x * h;
            int temp = sqrt(f[index].re * f[index].re + f[index].im * f[index].im) / 100; // ����
            if (temp > 255)
                temp=255;
            p_data[dibWidth * (height - y - 1) + x] = (BYTE)(temp); 
        }
    }

    delete [] t;
    delete [] f;
}

///////////////////////////////////////////////
//��ά����Ҷ�任ԭ��ƽ�ƣ�ʵ�ֶ�����ԭʼ�任ֱ��ת����
///////////////////////////////////////////////
void ZhengJiaoBianHuanDib::QuickFourier2()
{
    LONG width = GetWidth();     // DIB�Ŀ��
    LONG height = GetHeight();   // DIB�ĸ߶�
    LONG dibWidth = WIDTHBYTES(width * 8);   // ȡ��ԭͼ��ÿ���ֽ�����4�ֽڶ��룩

    LPBYTE p_data = GetData(); // ָ��DIB����ָ��

    LONG w = 1; // ���и���Ҷ�任�Ŀ�ȣ�2�������η���
    LONG h = 1; // ���и���Ҷ�任�ĸ߶ȣ�2�������η���

    int wp = 0; // �����������
    int hp = 0; // �߷����������

    CNumber *t; // ʱ������
    CNumber *f; // Ƶ������

    // ������и���Ҷ�任�Ŀ�ȣ�2�������η���
    while (w * 2 <= width)
    {
        w *= 2;
        wp++;
    }
    // ������и���Ҷ�任�ĸ߶ȣ�2�������η���
    while (h * 2 <= width)
    {
        h *= 2;
        hp++;
    }

    // �������ݴ洢���ռ�
    t = new CNumber[w * h];
    f = new CNumber[w * h];

    // ��ʱ��ֵ
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            t[x + w * y].re = p_data[dibWidth * (height - y - 1) + x];
            t[x + w * y].im = 0;
        }
    }

    // �ڴ�ֱ�����Ͻ��п��ٸ���Ҷ�任
    for (int y = 0; y < h; y++)
    {
        this->FFT(&t[y * w], &f[y * w], wp);
    }

    // ת���任���
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            t[y + h * x] = f[x + w * y]; // ��DIB��������
        }
    }

    // ��ˮƽ�����Ͻ��п��ٸ���Ҷ�任
    for (int x = 0; x < w; x++)
    {
        this->FFT(&t[x * h], &f[x * h], hp);
    }

    ///////////////////////////////////////////////////////////
    // ������Ҷ�任���ԭ���ƶ������� (0, 0) -> (w/2, h/2)
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            int index = y + x * h;
            int temp = sqrt(f[index].re * f[index].re + f[index].im * f[index].im) / 100; // ����
            if (temp > 255)
                temp=255;
            int W = (x < w / 2) ?  (x + w / 2) : (x - w / 2);
            int H = (y < h / 2) ?  (y + h / 2) : (y - h / 2);
            p_data[dibWidth * (height - H - 1) + W] = (BYTE)(temp); // ˮƽ��ֱ����
        }
    }
    ///////////////////////////////////////////////////////////

    delete [] t;
    delete [] f;
}


/*************************************************************************
 *
 * �������ƣ�FCT(const double *t, double *f, int r)
 *
 * ����:
 *   double * t                - ָ��ʱ��ֵ��ָ��
 *   double * f                - ָ��Ƶ��ֵ��ָ��
 *   r                        ��2������
 *
 * ����ֵ:
 *   �ޡ�
 *
 * ˵��:
 *   �ú�������ʵ�ֿ�����ɢ���ұ任���ú�������2N��Ŀ��ٸ���Ҷ�任
 * ��ʵ����ɢ���ұ任��
 *
 ************************************************************************/
void ZhengJiaoBianHuanDib::FCT(const double *t, double *f, int r)
{
    // ��ɢ���ұ任����
    long    count;    
    // ѭ������
    int        i;    
    // �м����
    double    dTemp;
    CNumber *X;    
    // ������ɢ���ұ任����
    count = 1<<r;
    // �����ڴ�
    X=new CNumber[count*2];
    // ����ֵΪ0
    memset(X, 0, sizeof(CNumber) * count * 2);
    // ��ʱ���д������X
    for(i=0;i<count;i++)
    {
        X[i].re=t[i];
        X[i].im=0;
    }
    // ���ÿ��ٸ���Ҷ�任
    FFT(X,X,r+1);
    // ����ϵ��
    dTemp = 1/sqrt((double)count);
    // ��F[0]
    f[0] = X[0].re * dTemp;
    dTemp *= sqrt((double)2);
    // ��F[u]    
    for(i = 1; i < count; i++)
    {
        f[i]=(X[i].re * cos(i*PI/(count*2)) + X[i].im * sin(i*PI/(count*2))) * dTemp;
    }    
    // �ͷ��ڴ�
    delete X;
}

/*************************************************************************
 *
 * �������ƣ�
 *   WALSH()
 *
 * ����:
 *   double * t                - ָ��ʱ��ֵ��ָ��
 *   double * f                - ָ��Ƶ��ֵ��ָ��
 *   r                        ��2������
 *
 * ����ֵ:
 *   �ޡ�
 *
 * ˵��:
 *   �ú�������ʵ�ֿ����ֶ�ʲ-������任��
 *
 ************************************************************************/

void ZhengJiaoBianHuanDib::WALSH(double *t, double *f, int r)
{
    // �ֶ�ʲ-������任����
    long   count;    
    // ѭ������
    int        i,j,k;    
    // �м����
    int        bfsize,p;
    double *X1,*X2,*X;    
    // ��������ֶ�ʲ�任����
    count = 1 << r;
    // �����������������
    X1 = new double[count];
    X2 = new double[count];
    // ��ʱ���д������X1
    memcpy(X1, t, sizeof(double) * count);
    // ��������
    for(k = 0; k < r; k++)
    {
        for(j = 0; j < 1<<k; j++)
        {
            bfsize = 1 << (r-k);
            for(i = 0; i < bfsize / 2; i++)
            {
                p = j * bfsize;
                X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
                X2[i + p + bfsize / 2] = X1[i + p] - X1[i + p + bfsize / 2];
            }
        }
        // ����X1��X2  
        X = X1;
        X1 = X2;
        X2 = X;
    }
    // ����ϵ��
    for(j = 0; j < count; j++)
    {
        p = 0;
        for(i = 0; i < r; i++)
        {
            if (j & (1<<i))
            {
                p += 1 << (r-i-1);
            }
        }
        f[j] = X1[p] / count;
    }    
    // �ͷ��ڴ�
    delete X1;
    delete X2;
}


/*************************************************************************
 *
 * �������ƣ�DIBLiSanYuXuan(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight)
 *
 * ����:
 *   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *
 * ����ֵ:
 *  bool               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������ͼ�������ɢ���ұ任��
 *
 ************************************************************************/
bool ZhengJiaoBianHuanDib::DIBLiSanYuXuan(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight)
{
    // ָ��Դͼ���ָ��
    LPBYTE    lpSrc;
    // ѭ������
    LONG    i;
    LONG    j;
    // ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
    LONG    w;
    LONG    h;
    // �м����
    double    dTemp;
    int        wp;
    int        hp;
    // ͼ��ÿ�е��ֽ���
    LONG    lLineBytes;
    // ����ͼ��ÿ�е��ֽ���
    lLineBytes = WIDTHBYTES(lWidth * 8);
    // ����ֵ
    w = 1;
    h = 1;
    wp = 0;
    hp = 0;
    // ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
    while(w * 2 <= lWidth)
    {
        w *= 2;
        wp++;
    }
    while(h * 2 <= lHeight)
    {
        h *= 2;
        hp++;
    }
    // �����ڴ�
    double *f = new double[w * h];
    double *F = new double[w * h];
    // ��
    for(j = 0; j < h; j++)
    {
        // ��
        for(i = 0; i < w; i++)
        {
            // ָ��DIB��j�У���i�����ص�ָ��
            lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;
            // ��ʱ��ֵ
            f[i + j * w] = *(lpSrc);
        }
    }
    
    for(j = 0; j < h; j++)
    {
        // ��y���������ɢ���ұ任
        FCT(&f[w * j], &F[w * j], wp);
    }
    // ���������
    for(j = 0; j < h; j++)
    {
        for(i = 0; i < w; i++)
        {
            f[i * h + j] = F[i + w * j];
        }
    }
    for(i = 0; i < w; i++)
    {
        // ��x���������ɢ���ұ任
        FCT(&f[i * h], &F[i * h], hp);
    }
    // ��
    for(j = 0; j < h; j++)
    {
        // ��
        for(i = 0; i < w; i++)
        {
            // ����Ƶ��
            dTemp = fabs(F[i*h+j]);
            // �ж��Ƿ񳬹�255
            if (dTemp > 255)
            {
                // ���ڳ����ģ�ֱ������Ϊ255
                dTemp = 255;
            }
            // ָ��DIB��y�У���x�����ص�ָ��
            lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;
            // ����Դͼ��
            * (lpSrc) = (BYTE)(dTemp);
        }
    }    
    // �ͷ��ڴ�
    delete f;
    delete F;
    //����
    return true;
}



/*************************************************************************
 *
 * �������ƣ�
 *   DIBWalsh()
 *
 * ����:
 *   LPBYTE lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *
 * ����ֵ:
 *  bool               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������ͼ������ֶ�ʲ-������任�������治ͬ���ǣ��˴��ǽ���ά
 * ����ת����һ����������Ȼ��Ը�����������һ��һά�ֶ�ʲ-������任��
 *
 ************************************************************************/

bool ZhengJiaoBianHuanDib::DIBWalsh(LPBYTE lpDIBBits, LONG lWidth, LONG lHeight)
{    
    // ָ��Դͼ���ָ��
    LPBYTE    lpSrc;
    // ѭ������
    LONG    i;
    LONG    j;
    // ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
    LONG    w;
    LONG    h;
    // �м����
    double    dTemp;
    int        wp;
    int        hp;
    // ͼ��ÿ�е��ֽ���
    LONG    lLineBytes;
    // ����ͼ��ÿ�е��ֽ���
    lLineBytes = WIDTHBYTES(lWidth * 8);
    // ����ֵ
    w = 1;
    h = 1;
    wp = 0;
    hp = 0;
    // ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
    while(w * 2 <= lWidth)
    {
        w *= 2;
        wp++;
    }    
    while(h * 2 <= lHeight)
    {
        h *= 2;
        hp++;
    }
    // �����ڴ�
    double *f = new double[w * h];
    double *F = new double[w * h];
    // ��
    for(j = 0; j < w; j++)
    {
        // ��
        for(i = 0; i < h; i++)
        {
            // ָ��DIB��j�У���i�����ص�ָ��
            lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
            // ��ʱ��ֵ
            f[i + j * w] = *(lpSrc);
        }
    }    
    // ���ÿ����ֶ�ʲ��������任
    WALSH(f, F, wp + hp);
    // ��
    for(j = 0; j < w; j++)
    {
        // ��
        for(i = 0; i < h; i++)
        {
            // ����Ƶ��
            dTemp = fabs(F[j * w + i] * 1000);
            // �ж��Ƿ񳬹�255
            if (dTemp > 255)
            {
                // ���ڳ����ģ�ֱ������Ϊ255
                dTemp = 255;
            }
            // ָ��DIB��j�У���i�����ص�ָ��
            lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
            // ����Դͼ��
            * (lpSrc) = (BYTE)(dTemp);
        }
    }    
    //�ͷ��ڴ�
    delete f;
    delete F;
    //����
    return true;
}
