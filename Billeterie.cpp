// Billeterie.cpp : Définit le point d'entrée de l'application.
//

#include "framework.h"
#include "Billeterie.h"

#define MAX_LOADSTRING 100

// Variables globales :
CHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
CHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale
WNDCLASSEX wcex;
HWND hWnd;
IWebBrowser2* pIWeb;
INITCOMMONCONTROLSEX iccex;
WCHAR url[256];
HDC hdc;
RECT rc;

EXTERN_C const IID DIID_DWebBrowserEvents2;
EXTERN_C const IID IID_IShellDispatch2;

class Evenement : public IDispatch {
private:
    long ref;
    HWND fenetre;
    BSTR url;

public:
    Evenement(HWND fenet) {
        fenetre = fenet;
    }
    ~Evenement() {
        SysFreeString(url);
    }
    STDMETHODIMP QueryInterface(REFIID iid, void** ppvObject) {
        if (iid == IID_IUnknown || iid == IID_IDispatch || iid == DIID_DWebBrowserEvents2) {
            *ppvObject = this; AddRef();
            return S_OK;
        }
        else return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() {
        return InterlockedIncrement(&ref);
    }
    ULONG STDMETHODCALLTYPE Release() {
        int tmp = InterlockedDecrement(&ref);
        if (tmp == 0) delete this;
        return tmp;
    }
    HRESULT STDMETHODCALLTYPE GetTypeInfoCount(unsigned int FAR* pctinfo) {
        return E_NOTIMPL;
    }
    HRESULT STDMETHODCALLTYPE GetTypeInfo(unsigned int iTInfo, LCID  lcid, ITypeInfo FAR* FAR* ppTInfo) {
        return E_NOTIMPL;
    }
    HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, OLECHAR FAR* FAR* rgszNames, unsigned int cNames, LCID lcid, DISPID FAR* rgDispId) {
        return E_NOTIMPL;
    }
    HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR* pDispParams, VARIANT FAR* parResult, EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr) {
        IUnknown* pIUnk; VARIANT* vurl;
        if (!pDispParams) return E_INVALIDARG;
        switch (dispIdMember) {
        case 0xFA: {
            pIUnk = pDispParams->rgvarg[6].pdispVal;
            SendMessage(fenetre, BEFORENAVIGATE2, (WPARAM)pIUnk, 0);
        }break;
        case 0x6A: {
            SendMessage(fenetre, DOWNLOADBEGIN, 0, 0);
        }break;
        case 0x68: {
            SendMessage(fenetre, DOWNLOADCOMPLETE, 0, 0);
        }break;
        case 0xFC: {
            pIUnk = pDispParams->rgvarg[1].pdispVal;
            vurl = pDispParams->rgvarg[0].pvarVal;
            url = vurl->bstrVal;
            SendMessage(fenetre, NAVIGATECOMPLETE2, (WPARAM)pIUnk, (LPARAM)url);
        }break;
        case 0x103: {
            pIUnk = pDispParams->rgvarg[1].pdispVal;
            SendMessage(fenetre, DOCUMENTCOMPLETE, (WPARAM)pIUnk, 0);
        }break;
        case 0x69: {
            long command; command = pDispParams->rgvarg[1].lVal;
            VARIANT_BOOL etat = pDispParams->rgvarg[0].boolVal;
            SendMessage(fenetre, COMMANDSTATECHANGE, (WPARAM)command, (LPARAM)etat);
        }break;
        case 0xFB: {
            pDispParams->rgvarg[0].pvarVal->vt = VT_BOOL;
            pDispParams->rgvarg[0].pvarVal->boolVal = VARIANT_TRUE;
        }break;
        default:break;
        }return S_OK;
    }
};

// Déclarations anticipées des fonctions incluses dans ce module de code :
LRESULT CALLBACK    ProcedurePrincipale(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ProcedurePropos(HWND, UINT, WPARAM, LPARAM);
int MsgBox(HWND hDlg, char* lpszText, char* lpszCaption, DWORD dwStyle, int lpszIcon);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR    lpCmdLine,_In_ int       nCmdShow){
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	iccex.dwICC=ICC_WIN95_CLASSES;
	iccex.dwSize=sizeof(iccex);
	InitCommonControlsEx(&iccex);
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_BILLETERIE, szWindowClass, MAX_LOADSTRING);
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = ProcedurePrincipale;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BILLETERIE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_BILLETERIE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL)); 
    if (!RegisterClassEx(&wcex)) {
        MsgBox(GetDesktopWindow(), (char*)"Incapable de creer la classe principale", NULL, MB_OK, IDI_SMALL);
    }
    GetClientRect(GetDesktopWindow(), &rc);
    hWnd = CreateWindowEx(WS_EX_CLIENTEDGE| WS_EX_DLGMODALFRAME| WS_EX_WINDOWEDGE |WS_EX_CONTEXTHELP,szWindowClass, szTitle, WS_OVERLAPPED |WS_CAPTION |WS_SYSMENU |WS_THICKFRAME, rc.left-0x9, rc.top, rc.right+0x12, rc.bottom-0x10, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd){
        MsgBox(GetDesktopWindow(), (char*)"Incapable de creer la fenetre principale", NULL, MB_OK, IDI_SMALL);
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    static BSTR titre;
    Evenement* pEvnm;
    IConnectionPointContainer* pCPContainer;
    IConnectionPoint* pConnectionPoint;
    DWORD dwCookie = 0; HINSTANCE hDLL = 0;
    hDLL = LoadLibraryA("atl.dll");
    typedef HRESULT(WINAPI* PAttachControl)(IUnknown*, HWND, IUnknown**);
    PAttachControl AtlAxAttachControl = (PAttachControl)GetProcAddress(hDLL, "AtlAxAttachControl");
    CoInitialize(0);
    CoCreateInstance(CLSID_WebBrowser, 0, CLSCTX_ALL, IID_IWebBrowser2, (void**)&pIWeb);
    AtlAxAttachControl(pIWeb, hWnd, NULL);
    pIWeb->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPContainer);
    pCPContainer->FindConnectionPoint(DIID_DWebBrowserEvents2, &pConnectionPoint);
    pEvnm = new Evenement(hWnd);
    pConnectionPoint->Advise(pEvnm, &dwCookie);
    if (pCPContainer) pCPContainer->Release();
    MultiByteToWideChar(CP_ACP, 0, (LPCCH)"https://csi.csvdc.qc.ca/", -1, url, 256);
    pIWeb->Navigate(url, 0, 0, 0, 0);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BILLETERIE));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int) msg.wParam;
}
int MsgBox(HWND hDlg, char* lpszText, char* lpszCaption, DWORD dwStyle, int lpszIcon) {
    MSGBOXPARAMS lpmbp;
    lpmbp.hInstance = wcex.hInstance;
    lpmbp.cbSize = sizeof(MSGBOXPARAMS);
    lpmbp.hwndOwner = hDlg;
    lpmbp.dwLanguageId = MAKELANGID(0x0800, 0x0800); //par defaut celui du systeme
    lpmbp.lpszText = lpszText;
    if (lpszCaption != NULL) {
        lpmbp.lpszCaption = lpszCaption;
    }
    else {
        lpmbp.lpszCaption = szTitle;
    }
    lpmbp.dwStyle = dwStyle | 0x00000080L;
    lpmbp.lpszIcon = (LPCTSTR)lpszIcon;
    lpmbp.lpfnMsgBoxCallback = 0;
    return  MessageBoxIndirect(&lpmbp);
}
LRESULT CALLBACK ProcedurePrincipale(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SYSCOMMAND: {
        switch (LOWORD(wParam)) {
        case SC_CONTEXTHELP:ShellAbout(hWnd, wcex.lpszClassName, "Projet moteur Web : Billeterie\n© Patrice Waechter-Ebling 2022", wcex.hIcon);
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analyse les sélections de menu :
            switch (wmId)
            {
            case ID_PRECEDENT: {
                pIWeb->GoBack();
            }break;
            case ID_SUIVANTE:{
                pIWeb->GoForward();
            }break;
            case ID_MAISON: {
                MultiByteToWideChar(CP_ACP, 0, (LPCCH)"https://csi.csvdc.qc.ca/", -1, url, 256);
                pIWeb->Navigate(url, 0, 0, 0, 0);
            }break;
            case ID_REACTUALISER: {
                pIWeb->Refresh();
            }break;
            case ID_ARRETERLANAVIGATION: {
                pIWeb->Stop();
            }break;

            case IDM_ABOUT:
                DialogBox(wcex.hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, ProcedurePropos);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            hdc = BeginPaint(hWnd, &ps);
            GetClientRect(hWnd, &rc);
            rc.bottom = 0x20;
            DrawCaption(hWnd, hdc, &rc, DC_ACTIVE | DC_GRADIENT | DC_ICON | DC_SMALLCAP | DC_TEXT);
            DrawIcon(hdc,rc.right-32,0,wcex.hIcon);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CLOSE: {
        DialogBox(wcex.hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, ProcedurePropos);
        PostQuitMessage(0);
    }break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK ProcedurePropos(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
