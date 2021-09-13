/*-
 * This file is part of mneca project. For license details, see the file
 * 'LICENSE' in this package.
 */
#include "Local.h"

namespace mneca {

class Application final : public IUIApplication {
private:
  Application() = delete;
  Application(Window* wnd, Ribbon* pRibbon);
  ~Application();
public:// IUnknown
  IFACEMETHODIMP QueryInterface(REFIID iid, void** ppv) override;
  IFACEMETHODIMP_(ULONG) AddRef() override;
  IFACEMETHODIMP_(ULONG) Release() override;
public:// IUIApplication methods
  STDMETHOD(OnCreateUICommand)(
    UINT nCmdID, UI_COMMANDTYPE typeID,
    IUICommandHandler** ppCommandHandler) override;
  STDMETHOD(OnViewChanged)(
    UINT viewId, UI_VIEWTYPE typeId, IUnknown* pView, UI_VIEWVERB verb,
    INT uReasonCode) override;
  STDMETHOD(OnDestroyUICommand)(
    UINT32 commandId, UI_COMMANDTYPE typeID,
    IUICommandHandler* commandHandler) override;
public:
  static HRESULT CreateInstance(
    Application** ppApp, Window* hParentWnd, Ribbon* pRibbon);
public:
  INT GetWidth() const noexcept;
  INT GetHeight() const noexcept;
private:
  LONG m_cRef;                           ///< Reference count.
  IUICommandHandler* m_pCommandHandler;  ///< Command handler
  IUIEventLogger* m_pEventLogger;        ///< Event logger
  Window* m_hParentWnd;
  INT m_nRibbonWidth;
  INT m_nRibbonHeight;
};

class CommandHandler final : public IUICommandHandler {
private:
  CommandHandler();
  ~CommandHandler();
public:
  static HRESULT CreateInstance(IUICommandHandler** ppCommandHandler);
public:// IUnknown methods
  STDMETHODIMP_(ULONG) AddRef() override;
  STDMETHODIMP_(ULONG) Release() override;
  STDMETHODIMP QueryInterface(REFIID iid, void** ppv) override;
public:// IUICommandHandler methods
  STDMETHOD(UpdateProperty)(
    UINT nCmdID, REFPROPERTYKEY key, const PROPVARIANT* ppropvarCurrentValue,
    PROPVARIANT* ppropvarNewValue) override;
  STDMETHOD(Execute)(
    UINT nCmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key,
    const PROPVARIANT* ppropvarValue,
    IUISimplePropertySet* pCommandExecutionProperties);
private:
  LONG m_cRef;                        // Reference count.
};

CommandHandler::CommandHandler() : m_cRef(1) { }

CommandHandler::~CommandHandler() { }

// Static method to create an instance of the object.
HRESULT
CommandHandler::CreateInstance(IUICommandHandler** ppCommandHandler) {
  HRESULT hr = S_OK;
  CommandHandler* tmp = nullptr;

  if(!ppCommandHandler) {
    return E_POINTER;
  }
  tmp = new CommandHandler();
  if(tmp) {
    *ppCommandHandler = static_cast<IUICommandHandler*>(tmp);
  }
  else {
    hr = E_OUTOFMEMORY;
  }

  return hr;
}

STDMETHODIMP_(ULONG)
CommandHandler::AddRef() {
  return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG)
CommandHandler::Release() {
  LONG cRef = InterlockedDecrement(&m_cRef);
  if(cRef == 0) {
    delete this;
  }
  return cRef;
}

STDMETHODIMP
CommandHandler::QueryInterface(REFIID iid, void** ppv) {
  if(iid == __uuidof(IUnknown)) {
    *ppv = static_cast<IUnknown*>(this);
  }
  else if(iid == __uuidof(IUICommandHandler)) {
    *ppv = static_cast<IUICommandHandler*>(this);
  }
  else {
    *ppv = NULL;
    return E_NOINTERFACE;
  }
  this->AddRef();
  return S_OK;
}

STDMETHODIMP
CommandHandler::UpdateProperty(
  UINT nCmdID, REFPROPERTYKEY key, const PROPVARIANT* ppropvarCurrentValue,
  PROPVARIANT* ppropvarNewValue) {
  UNREFERENCED_PARAMETER(nCmdID);
  UNREFERENCED_PARAMETER(key);
  UNREFERENCED_PARAMETER(ppropvarCurrentValue);
  UNREFERENCED_PARAMETER(ppropvarNewValue);
  return E_NOTIMPL;
}

STDMETHODIMP
CommandHandler::Execute(
  UINT nCmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key,
  const PROPVARIANT* ppropvarValue,
  IUISimplePropertySet* pCommandExecutionProperties) {
  UNREFERENCED_PARAMETER(pCommandExecutionProperties);
  UNREFERENCED_PARAMETER(ppropvarValue);
  UNREFERENCED_PARAMETER(key);
  UNREFERENCED_PARAMETER(verb);
  UNREFERENCED_PARAMETER(nCmdID);
  return S_OK;
}

/* ------------------------------------------------------------------------- */

Application::Application(Window* wnd, Ribbon* pRibbon)
  : m_cRef(1), m_pCommandHandler(nullptr), m_pEventLogger(nullptr),
    m_hParentWnd(wnd), m_nRibbonWidth(0), m_nRibbonHeight(0) { }

Application::~Application() {
  if(m_pCommandHandler) {
    m_pCommandHandler->Release();
    m_pCommandHandler = nullptr;
  }
}

STDMETHODIMP_(ULONG)
Application::AddRef() {
  return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG)
Application::Release() {
  LONG cRef = InterlockedDecrement(&m_cRef);

  if(cRef == 0) {
    delete this;
  }

  return cRef;
}

STDMETHODIMP
Application::QueryInterface(REFIID iid, void** ppv) {
  if(iid == __uuidof(IUnknown)) {
    *ppv = static_cast<IUnknown*>(this);
  }
  else if(iid == __uuidof(IUIApplication)) {
    *ppv = static_cast<IUIApplication*>(this);
  }
  else {
    *ppv = NULL;
    return E_NOINTERFACE;
  }
  this->AddRef();

  return S_OK;
}

STDMETHODIMP
Application::OnCreateUICommand(
  UINT nCmdID, UI_COMMANDTYPE typeID, IUICommandHandler** ppCommandHandler) {
  UNREFERENCED_PARAMETER(typeID);
  UNREFERENCED_PARAMETER(nCmdID);

  if (!m_pCommandHandler) {
    HRESULT hr = CommandHandler::CreateInstance(&m_pCommandHandler);
    if (FAILED(hr)) {
      return hr;
    }
  }

  return m_pCommandHandler->QueryInterface(IID_PPV_ARGS(ppCommandHandler));
}

STDMETHODIMP
Application::OnViewChanged(
  UINT viewId, UI_VIEWTYPE typeId, IUnknown* pView, UI_VIEWVERB verb,
  INT uReasonCode) {
  UNREFERENCED_PARAMETER(uReasonCode);
  UNREFERENCED_PARAMETER(viewId);

  HRESULT hr = E_NOTIMPL;

  // Checks to see if the view that was changed was a Ribbon view.
  if(UI_VIEWTYPE_RIBBON == typeId) {
    switch(verb) {
      // The view was newly created.
    case UI_VIEWVERB_CREATE:
      hr = S_OK;
      break;
      // The view has been resized. For the Ribbon view, the application should
      // call GetHeight to determine the height of the ribbon.
    case UI_VIEWVERB_SIZE:
    {
      IUIRibbon* pRibbon = NULL;
      UINT uRibbonHeight = 0;
      UINT uRibbonWidth = 0;
      RECT rcClient ={ 0 };

      hr = pView->QueryInterface(IID_PPV_ARGS(&pRibbon));
      if(SUCCEEDED(hr)) {
        ::GetClientRect(m_hParentWnd->GetWindowHandle(), &rcClient);

        hr = pRibbon->GetHeight(&uRibbonHeight);
        pRibbon->Release();

        m_nRibbonWidth = static_cast<INT>(rcClient.right - rcClient.left);
        m_nRibbonHeight = static_cast<INT>(uRibbonHeight);

        ::SendMessage(
          m_hParentWnd->GetWindowHandle(),
          WM_SIZE, 0, MAKELPARAM(rcClient.right, rcClient.bottom));
      }
      break;
    }
    // The view was destroyed.
    case UI_VIEWVERB_DESTROY:
      hr = S_OK;
      break;
    }
    InvalidateRect(this->m_hParentWnd->GetWindowHandle(), NULL, TRUE);
  }

  return hr;
}

STDMETHODIMP
Application::OnDestroyUICommand(
  UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler* commandHandler) {
  UNREFERENCED_PARAMETER(commandHandler);
  UNREFERENCED_PARAMETER(typeID);
  UNREFERENCED_PARAMETER(commandId);

  return E_NOTIMPL;
}

HRESULT
Application::CreateInstance(
  Application** ppApp, Window* hParentWnd, Ribbon* pRibbon) {
  HRESULT hr = S_OK;
  Application* tmp = nullptr;

  if(!ppApp) {
    return E_POINTER;
  }
  tmp = new Application(hParentWnd, pRibbon);
  if(tmp) {
    *ppApp = tmp;
  }
  else {
    hr = E_OUTOFMEMORY;
  }

  return hr;
}

INT
Application::GetWidth() const noexcept {
  return m_nRibbonWidth;
}

INT
Application::GetHeight() const noexcept {
  return m_nRibbonHeight;
}

/* ------------------------------------------------------------------------- */

class Ribbon::Impl final {
public:
  Impl() = delete;
  Impl(Window* pWnd, Ribbon* pRibbon);
  Impl(const Impl& other) = delete;
  Impl(Impl&& other) noexcept = delete;
  ~Impl();
public:
  Impl& operator=(const Impl& other) = delete;
  Impl& operator=(Impl&& other) noexcept = delete;
public:
  INT GetWidth() const noexcept;
  INT GetHeight() const noexcept;
private:
  IUIFramework* m_pFramework;
  Application* m_pApplication;
};

Ribbon::Impl::Impl(Window* pWnd, Ribbon* pRibbon)
 : m_pFramework(nullptr), m_pApplication(nullptr) {

  HRESULT hr = S_OK;

  // Here we instantiate the Ribbon framework object.
  hr = CoCreateInstance(
    CLSID_UIRibbonFramework, NULL, CLSCTX_INPROC_SERVER,
    IID_PPV_ARGS(&m_pFramework));
  if(FAILED(hr)) {
    throw std::runtime_error("CoCreateInstance() has been failed");
  }
  // Next, we create the application object (IUIApplication) and call the
  // framework Initialize method, passing the application object and the host
  // HWND that the Ribbon will attach itself to.
  hr = Application::CreateInstance(&m_pApplication, pWnd, pRibbon);
  if(FAILED(hr)) {
    throw std::runtime_error("Failed to create a Ribbon framework");
  }
  hr = m_pFramework->Initialize(pWnd->GetWindowHandle(), m_pApplication);
  if(FAILED(hr)) {
    throw std::runtime_error("Failed to initialize a Ribbon framework");
  }
  // Finally, we load the binary markup. This will initiate callbacks to the
  // IUIApplication object that was provided to the framework earlier, allowing
  // command handlers to be bound to individual commands.
  hr = m_pFramework->LoadUI(GetModuleHandle(nullptr), L"MNECA_UI_RIBBON");
  if(FAILED(hr)) {
    throw std::runtime_error("Failed to load a Ribbon UI");
  }
}

Ribbon::Impl::~Impl() {
  if(m_pFramework) {
    m_pFramework->Destroy();
    m_pFramework->Release();
    m_pFramework = nullptr;
  }
  if(m_pApplication) {
    m_pApplication->Release();
    m_pApplication = nullptr;
  }
}

INT
Ribbon::Impl::GetWidth() const noexcept {
  return m_pApplication->GetWidth();
}

INT
Ribbon::Impl::GetHeight() const noexcept {
  return m_pApplication->GetHeight();
}

Ribbon::Ribbon(Window* pParentWnd)
  : Window(IDR_RIBBON, pParentWnd), m_pImpl(new Impl(pParentWnd, this)) { }

Ribbon::~Ribbon() { }

INT
Ribbon::GetID() const noexcept {
  return IDR_RIBBON;
}

INT
Ribbon::GetWidth() const noexcept {
  return m_pImpl->GetWidth();
}

INT
Ribbon::GetHeight() const noexcept {
  return m_pImpl->GetHeight();
}

LRESULT
Ribbon::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  return static_cast<LRESULT>(0);
}

}  // namespace mneca
