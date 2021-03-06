#pragma once
// Install UI Header
// UI Functions
#include <windows.h>
#include <string>
#include "GeneralUtil.hpp"
#include "IPackageHandler.hpp"

// Child window identifiers
#define IDC_LAUNCHCHECKBOX 1
#define IDC_INSTALLBUTTON 2
#define IDC_CANCELBUTTON 3
#define IDC_LAUNCHBUTTON 4
#define WM_INSTALLCOMPLETE_MSG (WM_APP+1)

// Global variables
static HWND hWnd = NULL; // parent window hwnd
static HWND g_buttonHWnd = NULL;
static HWND g_checkboxHWnd = NULL;
static HWND g_progressHWnd = NULL;
static HWND g_CancelbuttonHWnd = NULL;
static HWND g_LaunchbuttonHWnd = NULL;
static bool g_installed = false;
static bool g_launchCheckBoxState = true; // launch checkbox is checked by default

class UI
{
public:
    HRESULT ShowUI();
	HRESULT LaunchInstalledApp();

    static HRESULT Make(_In_ MsixRequest* msixRequest, _Out_ UI** instance);
    ~UI() {}
private:
    MsixRequest* m_msixRequest = nullptr;

    std::wstring m_displayName = L"";
    std::wstring m_publisherCommonName = L"";
    ComPtr<IStream> m_logoStream;
    std::wstring m_version = L"";
    int m_numberOfFiles = 0;
    HRESULT m_loadingPackageInfoCode = 0;
    HANDLE m_buttonClickedEvent;

    UI() {}
    UI(_In_ MsixRequest* msixRequest) : m_msixRequest(msixRequest) 
	{
		m_buttonClickedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
    
    HRESULT ParseInfoFromPackage();

public:
    HRESULT DrawPackageInfo(HWND hWnd, RECT windowRect);
	int CreateInitWindow(HINSTANCE hInstance, int nCmdShow, const std::wstring& windowClass, const std::wstring& title);
	void LoadInfo();
	int GetNumberOfFiles() { return m_numberOfFiles; }
    void SetButtonClicked() { SetEvent(m_buttonClickedEvent); }

    // FUNCTION: CreateProgressBar(HWND parentHWnd, RECT parentRect, int count)
    //
    // PURPOSE: Creates the progress bar
    //
    // parentHWnd: the HWND of the window to add the progress bar to
    // parentRect: the dimmensions of the parent window
    // count: the number of objects to be iterated through in the progress bar
    BOOL CreateProgressBar(HWND parentHWnd, RECT parentRect, int count);

    // FUNCTION: LaunchButton(HWND parentHWnd, RECT parentRect)
    //
    // PURPOSE: Create the lower right install button
    // 
    // parentHWnd: the HWND of the window to add the button to
    // parentRect: the specs of the parent window
    BOOL LaunchButton(HWND parentHWnd, RECT parentRect);

    // FUNCTION: CreateCheckbox(HWND parentHWnd, RECT parentRect)
    //
    // PURPOSE: Create the launch when ready checkbox on the bottom left
    // 
    // parentHWnd: the HWND of the window to add the checkbox to
    // parentRect: the specs of the parent window
    BOOL CreateCheckbox(HWND parentHWnd, RECT parentRect);

    // FUNCTION: CreateCancelButton(HWND parentHWnd, RECT parentRect)
    //
    // PURPOSE: Create the cancel button on the bottom right corner when user clicks on install
    // 
    // parentHWnd: the HWND of the window to add the checkbox to
    // parentRect: the specs of the parent window
    BOOL CreateCancelButton(HWND parentHWnd, RECT parentRect);

    // FUNCTION: CreateLaunchButton(HWND parentHWnd, RECT parentRect)
    //
    // PURPOSE: Create the launch button on the botton right after app has been installed
    // 
    // parentHWnd: the HWND of the window to add the checkbox to
    // parentRect: the specs of the parent window
    BOOL CreateLaunchButton(HWND parentHWnd, RECT parentRect);

    // FUNCTION: ChangeButtonText(LPARAM newMessage)
    //
    // PURPOSE: Changes the text of the lower right button
    //
    // newMessage: the message to change the button to
    BOOL ChangeButtonText(const std::wstring& newMessage);

    // FUNCTION: HideButtonWindow()
    //
    // PURPOSE: Hides the lower right button
    //
    BOOL HideButtonWindow();

    // FUNCTION: ChangeText(HWND parentHWnd, std::wstring& windowText)
    //
    // PURPOSE: Change the text of the installation window based on the given input
    //
    // parentHWnd: the HWND of the window to be changed
    // windowText: the text to change the window to
    BOOL ChangeText(HWND parentHWnd, std::wstring displayText, std::wstring  messageText, IStream* logoStream = nullptr);

    // FUNCTION: UpdateProgressBar
    //
    // PURPOSE: Increment the progress bar one tick based on preset tick
    void UpdateProgressBar();

    // FUNCTION: SendInstallCompleteMsg
    //
    // PURPOSE: Sends the WM_INSTALLCOMPLETE_MSG message to the main window when app installation is complete
    void SendInstallCompleteMsg();
};

class CreateAndShowUI : IPackageHandler
{
public:
    HRESULT ExecuteForAddRequest();

    static const PCWSTR HandlerName;
    static HRESULT CreateHandler(_In_ MsixRequest* msixRequest, _Out_ IPackageHandler** instance);
    ~CreateAndShowUI() {}
private:
    MsixRequest* m_msixRequest = nullptr;

    CreateAndShowUI() {}
    CreateAndShowUI(_In_ MsixRequest* msixRequest) : m_msixRequest(msixRequest) {}
};
