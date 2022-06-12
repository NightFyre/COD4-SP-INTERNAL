#include "helper.h"
#include "Console.hpp"
#include "Game.hpp"
#include "D3D9Window.hpp"
#include "Hooking.hpp"
#include "Menu.hpp"

int COUNT = 0;
void BackgroundWorker()
{
    using namespace COD4_SP;
    while (g_Running) {
        if (COUNT >= 5000) {
            
            COUNT = NULL;
        }
        g_Menu->Loops();
        COUNT++;
        std::this_thread::sleep_for(1ms);
    }
    return;
}

DWORD WINAPI MainThread()
{
    using namespace COD4_SP;

    g_Console = std::make_unique<Console>();
#if DEBUG
    g_Console->InitializeConsole("COD4 SP INTERNAL - DEBUG");
    g_Console->printdbg("COD4 Single Player :: Internal v1.0\n\nInitializing . . .\n", g_Console->color.DEFAULT);
#endif

    g_GameVariables = std::make_unique<GameVariables>();
    g_GameData = std::make_unique<GameData>();

    g_D3D9Window = std::make_unique<D3D9Window>();
    g_Hooks = std::make_unique<Hooking>();
    g_Menu = std::make_unique<Menu>();
    
    g_Hooks->Hook();


    std::thread UPDATE(BackgroundWorker);
    g_Running = TRUE;
    while (g_Running)
    {
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            g_GameVariables->m_ShowMenu = !g_GameVariables->m_ShowMenu;
           *(bool*)(dwGameBase + g_GameData->offsets.mouse) = !g_GameVariables->m_ShowMenu;
        }
    }


    //  EXIT PROCESS
    UPDATE.join();

    //  RESTORE OUR MOSE
    if (g_GameVariables->m_ShowMenu)
        *(bool*)(dwGameBase + g_GameData->offsets.mouse) = TRUE;
    FreeLibraryAndExitThread(g_hModule, EXIT_SUCCESS);
    return EXIT_SUCCESS;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    using namespace COD4_SP;
    
    //  Store our module. This is so that we can ExitThread later if the need should arise.
    g_hModule = hModule;

    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH: {
            DisableThreadLibraryCalls(hModule);
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, g_hModule, NULL, NULL);
            break;
        }
        case DLL_PROCESS_DETACH: {
            // Instead of killing or thread immediately , 
            //  We instead set off a chain of events to exit the thread 
            //  as well as restore everything to the way it was before we attached to the process
            g_Killswitch = TRUE;    
            break;
        }
    }
    return TRUE;
}