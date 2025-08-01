// Dear ImGui: standalone example application for SDL2 + DirectX 11
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include "Bus.h"
#include "Cartridge.h"
#include "ppu.h"
#include "cpu.h"
#include <iostream>
#include "imgui_impl_sdl2.h"

// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
void InitNes(Bus& nes, CPU& cpu, PPU& ppu, std::shared_ptr<Cartridge>& cart);
void DrawGui(std::shared_ptr<Cartridge>& cart, bool& showDemo, bool& showHeader);

    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to the latest version of SDL is recommended!)
bool InitGui(SDL_Window*& window, HWND& hwnd) {
#ifdef _WIN32
    ::SetProcessDPIAware();
#endif
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return false;
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Setup window
    float main_scale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("Dear ImGui SDL2+DirectX11 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)(1280 * main_scale), (int)(720 * main_scale), window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return false;
    }
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    hwnd = (HWND)wmInfo.info.win.window;

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls



    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForD3D(window);


    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    return true;

};






    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //style.FontSizeBase = 20.0f;
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
    //IM_ASSERT(font != nullptr);
int main(){
    Bus nes;
    CPU cpu;
    PPU ppu;
    std::shared_ptr<Cartridge> cart;

    InitNes(nes, cpu, ppu, cart);

    SDL_Window* window = nullptr;
    HWND hwnd = nullptr;

    if (!InitGui(window, hwnd)) return -1;

    bool done = false;
    bool showDemo = true;
    bool showHeader = true;

    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) done = true;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        DrawGui(cart, showDemo, showHeader);

        ImGui::Render();
        const float clear_color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_pSwapChain->Present(1, 0);
    }

    // Shutdown ImGui
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
// Helper functions to use DirectX11
bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}


void InitNes(Bus& nes, CPU& cpu, PPU& ppu, std::shared_ptr<Cartridge>& cart) {

    cart = std::make_shared<Cartridge>("Super_mario_brothers.nes");
    nes.connectCPU(&cpu);
    cpu.ConnectBus(&nes);
    nes.connectPPU(&ppu);

    nes.inputCart(cart);
    nes.Reset();
}
void DrawGui(std::shared_ptr<Cartridge>& cart, bool& showDemo, bool& showHeader) {
    if (showDemo)
        ImGui::ShowDemoWindow(&showDemo);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {


        ImGui::Begin("Nes Inspector");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("Current game inserted: ");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Cart Window", &showHeader);


        ImGui::End();
    }

    // 3. Show another simple window.
    if (showHeader)
    {
        ImGui::Begin("Cartridge Info");

        ImGui::Text("PRG ROM Banks: %d", cart->getPRGSize());
        ImGui::Text("CHR ROM Banks: %d", cart->getCHRSize());

        if (cart->iNes20()) {
            ImGui::Text("Mapper ID 2.0: %d", cart->getMapper20());
            ImGui::Text("PRG ROM Size 2.0: %d", cart->getPRGRomSize20());
            ImGui::Text("CHR Rom Size 2.0: %d", cart->getCHRRomSize20());

        }
        else {

            ImGui::Text("MapperID 1.0: %d", cart->getMapper10());
            ImGui::Text("PRG ROM Size 1.0: %d", cart->getPRGRomSize10());
            ImGui::Text("CHR Rom Size 1.0: %d", cart->getCHRRomSize10());

        }

        ImGui::Text("PRG Ram Size Non Volitile: %d", cart->getPRGRamSizeNonVol());
        ImGui::Text("PRG Ram Size Volitile: %d", cart->getPRGRamSizeVol());
        ImGui::Text("CHR Ram Size Volitile: %d", cart->getCHRRamSizeVol());
        ImGui::Text("CHR Ram Size Non Volitile: %d", cart->getCHRRamSizeNonVol());
        ImGui::Text("Region: %d", cart->getTiming());

        ImGui::Text("Header values :");

        ImGui::End();
    
    }
};