#ifndef __CDIB_H
#define __CDIB_H

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 

class CDib : public CObject
{
public:
    RGBQUAD* m_pRGB;
    BYTE* m_pData,*m_pData2;
    UINT m_numberOfColors;
    BOOL m_valid;
    BITMAPFILEHEADER bitmapFileHeader;

    BITMAPINFOHEADER* m_pBitmapInfoHeader;
    BITMAPINFO* m_pBitmapInfo;
    int byBitCount;
    DWORD dwWidthBytes;
    BYTE* pDib;
    DWORD size;
    
public:
    CDib();
    ~CDib();

    char m_fileName[256];
    char* GetFileName();
    BOOL IsValid();
    DWORD GetSize();
    UINT GetWidth();
    UINT GetHeight();
    UINT GetNumberOfColors();
    RGBQUAD* GetRGB();
    BYTE* GetData();
    BYTE* GetData2();
    DWORD GetDibWidthBytes();
    BITMAPINFO* GetInfo();
    WORD PaletteSize(LPBYTE lpDIB);
    WORD DIBNumColors(LPBYTE lpDIB);
    void SaveFile(const CString filename);

public:
    void ToShowBuf();
    void ToGrayBuf();
    void LoadFile(const char* dibFileName);  
};

#endif
