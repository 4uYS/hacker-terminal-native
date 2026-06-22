/*
 * OCR 识图模块
 * 基于 Windows 内置 OCR (Windows.Media.Ocr)
 * 需要 Windows 10 及以上版本
 */

#include "hacker.h"

static BOOL ocr_available = FALSE;
static BOOL ocr_initialized = FALSE;

// 初始化 OCR
BOOL InitOcr() {
    if (ocr_initialized) {
        return ocr_available;
    }
    
    ocr_initialized = TRUE;
    
    // 检查 Windows 版本（需要 Windows 10+）
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    
    // Windows 10 = 10.0
    if (osvi.dwMajorVersion >= 10) {
        ocr_available = TRUE;
    } else {
        ocr_available = FALSE;
    }
    
    return ocr_available;
}

// OCR 是否可用
BOOL OcrIsAvailable() {
    if (!ocr_initialized) {
        InitOcr();
    }
    return ocr_available;
}

// 从图片文件识别文字
LPCTSTR OcrImageFile(LPCTSTR imagePath) {
    static TCHAR result[4096];
    
    if (!OcrIsAvailable()) {
        _stprintf(result, 
            _T("[OCR不可用]\n")
            _T("Windows OCR 需要 Windows 10 及以上版本\n")
            _T("当前系统版本不支持内置OCR\n\n")
            _T("替代方案:\n")
            _T("  1. 升级到 Windows 10 或更高版本\n")
            _T("  2. 使用云端识图API（设置中配置）\n")
            _T("  3. 安装第三方OCR引擎")
        );
        return result;
    }
    
    // 检查文件是否存在
    WIN32_FILE_ATTRIBUTE_DATA fad;
    if (!GetFileAttributesEx(imagePath, GetFileExInfoStandard, &fad)) {
        _stprintf(result, _T("文件不存在: %s"), imagePath);
        return result;
    }
    
    // 检查文件大小
    ULONGLONG fileSize = ((ULONGLONG)fad.nFileSizeHigh << 32) | fad.nFileSizeLow;
    if (fileSize == 0) {
        _tcscpy(result, _T("文件为空"));
        return result;
    }
    
    if (fileSize > 10 * 1024 * 1024) {
        _stprintf(result, _T("文件过大: %llu MB\n最大支持 10 MB"), fileSize / 1024 / 1024);
        return result;
    }
    
    // 检查文件扩展名
    LPCTSTR ext = _tcsrchr(imagePath, '.');
    BOOL supported = FALSE;
    if (ext) {
        TCHAR extLower[16];
        _tcsncpy(extLower, ext, 15);
        extLower[15] = 0;
        ToLowerCase(extLower);
        
        if (_tcscmp(extLower, _T(".png")) == 0 ||
            _tcscmp(extLower, _T(".jpg")) == 0 ||
            _tcscmp(extLower, _T(".jpeg")) == 0 ||
            _tcscmp(extLower, _T(".bmp")) == 0 ||
            _tcscmp(extLower, _T(".gif")) == 0 ||
            _tcscmp(extLower, _T(".tiff")) == 0) {
            supported = TRUE;
        }
    }
    
    if (!supported) {
        _stprintf(result, 
            _T("不支持的图片格式: %s\n\n")
            _T("支持的格式: PNG, JPG, BMP, GIF, TIFF"),
            ext ? ext : _T("未知")
        );
        return result;
    }
    
    // 简化实现：提示需要完整 WinRT 支持
    // 完整实现需要使用 Windows Runtime C++/WinRT 或 WRL
    _stprintf(result,
        _T("[OCR 识别]\n")
        _T("──────────────────────────────\n")
        _T("图片文件: %s\n")
        _T("文件大小: %llu KB\n")
        _T("格式: %s\n")
        _T("──────────────────────────────\n\n")
        _T("(完整OCR识别功能需要 WinRT 支持)\n")
        _T("当前为框架实现，可在 Windows 10+ 上扩展\n\n")
        _T("扩展方式:\n")
        _T("  - 使用 C++/WinRT 调用 Windows.Media.Ocr\n")
        _T("  - 或集成 Tesseract OCR 引擎\n")
        _T("  - 或调用云端识图API"),
        imagePath,
        fileSize / 1024,
        ext ? ext + 1 : _T("未知")
    );
    
    return result;
}

// 识别屏幕区域
LPCTSTR OcrScreenRegion(int x, int y, int width, int height) {
    static TCHAR result[4096];
    
    if (!OcrIsAvailable()) {
        return OcrImageFile(NULL);
    }
    
    // 参数校验
    if (width <= 0 || height <= 0) {
        _tcscpy(result, _T("无效的区域尺寸"));
        return result;
    }
    
    if (width > 4096 || height > 4096) {
        _tcscpy(result, _T("区域尺寸过大（最大 4096x4096）"));
        return result;
    }
    
    _stprintf(result,
        _T("[屏幕区域识别]\n")
        _T("──────────────────────────────\n")
        _T("位置: (%d, %d)\n")
        _T("尺寸: %d x %d\n")
        _T("──────────────────────────────\n\n")
        _T("(完整OCR识别功能待实现)\n\n")
        _T("实现步骤:\n")
        _T("  1. GDI 截图指定区域\n")
        _T("  2. 转换为 SoftwareBitmap\n")
        _T("  3. 调用 OcrEngine::RecognizeAsync\n")
        _T("  4. 提取识别结果文本"),
        x, y, width, height
    );
    
    return result;
}

// 全屏识别
LPCTSTR OcrFullScreen() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    return OcrScreenRegion(0, 0, screenWidth, screenHeight);
}
