#include "stdafx.h"
#include "cdib.h"
#include "windowsx.h"
#include "math.h"


CDib::CDib()
{

}

CDib::~CDib()
{
    GlobalFreePtr(m_pBitmapInfo);
}

void CDib::LoadFile(const char* dibFileName)
{
    strcpy(m_fileName,dibFileName);
    CFile dibFile(m_fileName, CFile::modeRead);
    
    dibFile.Read((void*)&bitmapFileHeader,sizeof(BITMAPFILEHEADER));

    if (bitmapFileHeader.bfType == 0x4d42)
    {
        DWORD fileLength = dibFile.GetLength();    
        DWORD size = fileLength -
            sizeof(BITMAPFILEHEADER);
        BYTE* pDib =
            (BYTE*)GlobalAllocPtr(GMEM_MOVEABLE, size);
        dibFile.Read((void*)pDib, size);
        dibFile.Close();

        m_pBitmapInfo = (BITMAPINFO*) pDib;
        m_pBitmapInfoHeader = (BITMAPINFOHEADER*) pDib;
        m_pRGB = (RGBQUAD*)(pDib +
            m_pBitmapInfoHeader->biSize);
        int m_numberOfColors = GetNumberOfColors();
        if (m_pBitmapInfoHeader->biClrUsed == 0)
            m_pBitmapInfoHeader->biClrUsed =
                m_numberOfColors;
        DWORD colorTableSize = m_numberOfColors *
            sizeof(RGBQUAD);
        m_pData = pDib + m_pBitmapInfoHeader->biSize
            + colorTableSize;
        if (m_pRGB == (RGBQUAD*)m_pData) // No color table
            m_pRGB = NULL;
        m_pBitmapInfoHeader->biSizeImage = GetSize();
        m_valid = TRUE;
    }    
    else
    {
        m_valid = FALSE;
        AfxMessageBox("This isn't a bitmap file!");
    }
}

BOOL CDib::IsValid()
{
    return m_valid;
}
        
char* CDib::GetFileName()
{
    return m_fileName;
}
        
UINT CDib::GetWidth()
{
    return (UINT) m_pBitmapInfoHeader->biWidth;
}
        
UINT CDib::GetHeight()
{
    return (UINT) m_pBitmapInfoHeader->biHeight;
}
        
DWORD CDib::GetSize()
{
    if (m_pBitmapInfoHeader->biSizeImage != 0)
        return m_pBitmapInfoHeader->biSizeImage;
    else
    {
        DWORD height = (DWORD) GetHeight();
        DWORD width = (DWORD) GetWidth();
        return height * width;
    }
}

DWORD CDib::GetDibWidthBytes()
{
     byBitCount=m_pBitmapInfoHeader->biBitCount;
    LONG nWidth=m_pBitmapInfoHeader->biWidth;

    dwWidthBytes = (DWORD)m_pBitmapInfoHeader->biWidth;    //8-bits
    if(byBitCount == 1) dwWidthBytes = (nWidth + 7) / 8;
    else if(byBitCount == 4) dwWidthBytes = (nWidth + 1) / 2;
    else if(byBitCount == 24) dwWidthBytes = 3 * nWidth ;
    
    while((dwWidthBytes & 3) != 0)dwWidthBytes++;

    return dwWidthBytes;
}

UINT CDib::GetNumberOfColors()
{
    int numberOfColors;

    if ((m_pBitmapInfoHeader->biClrUsed == 0) &&
          (m_pBitmapInfoHeader->biBitCount < 9))
    {
        switch (m_pBitmapInfoHeader->biBitCount)
        {
            case 1: numberOfColors = 2; break;
            case 4: numberOfColors = 16; break;
            case 8: numberOfColors = 256;
        }
    }
    else
        numberOfColors = (int) m_pBitmapInfoHeader->biClrUsed;

    return numberOfColors;
}
    
BYTE* CDib::GetData()
{
    return m_pData;
}
BYTE* CDib::GetData2()
{
    if(GetRGB())
       m_pData2=m_pData;
    return m_pData2;
}
RGBQUAD* CDib::GetRGB()
{
    return m_pRGB;
}

BITMAPINFO* CDib::GetInfo()
{
    return m_pBitmapInfo;
}

WORD CDib::PaletteSize(LPBYTE lpDIB) 
{ 
    return (DIBNumColors(lpDIB) * sizeof(RGBTRIPLE)); 
} 

WORD CDib::DIBNumColors(LPBYTE lpDIB) 
{ 
    WORD wBitCount;  // DIB bit count 
    wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount; 
    switch (wBitCount) 
    { 
        case 1: 
            return 2; 
        case 4: 
            return 16; 
        case 8: 
            return 256; 
        default: 
            return 0; 
    } 
} 

void CDib::SaveFile(const CString filename)
{
    BITMAPFILEHEADER    bmfHdr;     // Header for Bitmap file 
    LPBITMAPINFOHEADER  lpBI;       // Pointer to DIB info structure 
    DWORD               dwDIBSize; 
 
    bmfHdr.bfType = 0x4d42;  // "BM" 
    lpBI = (LPBITMAPINFOHEADER)m_pBitmapInfoHeader; 
    dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPBYTE)lpBI);   
    if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4)) 
        dwDIBSize += lpBI->biSizeImage; 
    else 
    { 
        DWORD dwBmBitsSize;  // Size of Bitmap Bits only 
        dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * 
                lpBI->biHeight; 
        dwDIBSize += dwBmBitsSize; 
        lpBI->biSizeImage = dwBmBitsSize; 
    } 
    bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER); 
    bmfHdr.bfReserved1 = 0; 
    bmfHdr.bfReserved2 = 0; 
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize + 
            PaletteSize((LPBYTE)lpBI); 
 
    CFile dibFile(filename, CFile::modeWrite|CFile::modeCreate);
    dibFile.Write(&bmfHdr, sizeof(BITMAPFILEHEADER));
    dibFile.Close();
}

void CDib::ToGrayBuf()
{ 
    if(GetRGB()) {
       m_pData2 = m_pData;
    }
    else { /// 将 24 位彩色图转换为 8 位灰度计算
        BYTE r, g, b;
        LONG rgbDibWidth, grayDibWidth;
        int width, height, size;

        width = GetWidth();
        height = GetHeight();
        rgbDibWidth = GetDibWidthBytes();

        m_pBitmapInfoHeader->biBitCount = 8; // 24位 -> 8位
        grayDibWidth = WIDTHBYTES(width * 8);
        size = height * grayDibWidth;

        m_pData2 = (BYTE *)GlobalAllocPtr(GMEM_MOVEABLE, size); // 重新分配内存
        
        for(int j = 0; j < height; j++) {
            for(int i = 0; i < width; i++) {
                b = m_pData[j * rgbDibWidth + 3 * i];
                g = m_pData[j * rgbDibWidth + 3 * i + 1];
                r = m_pData[j * rgbDibWidth + 3 * i + 2];
                m_pData2[j * grayDibWidth + i] = (BYTE)((30 * r + 59 * g + 11 * b) / 100);  
            }
        }
    }
}

void CDib::ToShowBuf()
{
   if(GetRGB()) {
       m_pData = m_pData2;
   }
   else { /// 将 8 位灰度图转换为 24 位灰度显示
       LONG rgbDibWidth, grayDibWidth;
       int width, height, size;

       width = GetWidth();
       height=GetHeight();
       grayDibWidth = WIDTHBYTES(width * 8);

       m_pBitmapInfoHeader->biBitCount = 24; // 8位 -> 24位
       rgbDibWidth = GetDibWidthBytes();
       size = height * rgbDibWidth;

       //m_pData = (BYTE*)GlobalAllocPtr(GMEM_MOVEABLE, size);

       for(int j = 0; j < height; j++) {
           for(int i = 0; i < width; i++) {
               m_pData[(height-j-1) * rgbDibWidth+3*i]   = m_pData2[(height-1-j) * grayDibWidth+i];
               m_pData[(height-j-1) * rgbDibWidth+3*i+1] = m_pData2[(height-1-j) * grayDibWidth+i];
               m_pData[(height-j-1) * rgbDibWidth+3*i+2] = m_pData2[(height-1-j) * grayDibWidth+i];
           }
       }
   }
}
