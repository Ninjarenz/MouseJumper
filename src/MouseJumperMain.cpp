#include <iostream>
#include <windows.h>

// Global variable to store the saved mouse position
POINT saved_position = {0, 0};
bool position_saved = false;

// Define the shortcut keys (Virtual Key Codes)
const int CTRL_KEY = VK_CONTROL;
const int ALT_KEY  = VK_MENU;
const int SAVE_KEY = 0x53; // 'S' key (for Ctrl + Alt + S)

const int JUMP_KEY = VK_F5; // F5 key

const int EXIT_KEY = VK_ESCAPE; // Use ESC to exit the program

// --- NEW FUNCTION ---
// Function to simulate a Left Mouse Click at the current cursor position
void simulateLeftClick() {
    // 1. INPUT structure for the Left Mouse Button Down event
    INPUT input_down = {0};
    input_down.type = INPUT_MOUSE;
    input_down.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    
    // 2. INPUT structure for the Left Mouse Button Up event
    INPUT input_up = {0};
    input_up.type = INPUT_MOUSE;
    input_up.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    
    // An array to hold both events (Down and Up)
    INPUT inputs[] = {input_down, input_up};
    
    // Send the events to the system
    UINT result = SendInput(2, inputs, sizeof(INPUT));

    if (result != 2) {
        std::cerr << "WARNING: Failed to simulate mouse click!" << std::endl;
    } else {
        std::cout << "Simulated Left Click at target." << std::endl;
    }
}
// --------------------

void saveMousePosition() {
    if (GetCursorPos(&saved_position)) {
        position_saved = true;
        std::cout << "✅ Position Saved! ("
                  << saved_position.x << ", "
                  << saved_position.y << ")" << std::endl;
    } else {
        std::cerr << "Failed to get cursor position." << std::endl;
    }
}

void jumpToSavedPosition() {
    if (position_saved) {
        // Step 1: Jump the cursor to the saved position
        if (SetCursorPos(saved_position.x, saved_position.y)) {
            std::cout << "Jumping to Saved Position! ("
                      << saved_position.x << ", "
                      << saved_position.y << ")" << std::endl;
            
            // Step 2: Immediately simulate a click at the new location
            simulateLeftClick();
            
        } else {
            std::cerr << "Failed to set cursor position." << std::endl;
        }
    } else {
        std::cout << "No position saved yet. Press Ctrl+Alt+S first." << std::endl;
    }
}

int main() {
    SetConsoleTitleA("Mouse Jumper (Windows) - WITH CLICK");

    std::cout << "--- Mouse Jumper Project ---\n"
              << "   [Ctrl + Alt + S]: Save Position\n"
              << "   [F5]: Jump and CLICK!\n"
              << "   [ESC]: Exit Program\n"
              << "----------------------------\n"
              << "Running..." << std::endl;

    while (true) {
        Sleep(10); // 10 milliseconds delay

        // --- CHECK FOR SAVE SHORTCUT (Ctrl + Alt + S) ---
        if (GetAsyncKeyState(CTRL_KEY) & 0x8000 && 
            GetAsyncKeyState(ALT_KEY) & 0x8000 && 
            GetAsyncKeyState(SAVE_KEY) & 0x8000) 
        {
            saveMousePosition();
            // Wait for 'S' key release
            while (GetAsyncKeyState(SAVE_KEY) & 0x8000) {
                Sleep(50);
            }
        }

        // --- CHECK FOR JUMP SHORTCUT (F5) ---
        if (GetAsyncKeyState(JUMP_KEY) & 0x8000) {
            jumpToSavedPosition();
            // Wait for key release
            while (GetAsyncKeyState(JUMP_KEY) & 0x8000) {
                Sleep(50);
            }
        }
        
        // --- CHECK FOR EXIT SHORTCUT (ESC) ---
        if (GetAsyncKeyState(EXIT_KEY) & 0x8000) {
            std::cout << "\nProgram exiting. Goodbye!" << std::endl;
            break;
        }
    }

    return 0;
}