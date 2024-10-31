#include "raylib.h"
#include <algorithm>
#include <chrono>
#include <string>

int screenWidth = 1920;
int screenHeight = 1000;

// Function to check if the array is sorted
bool isSorted(const int* array, int size) {
    if (array != nullptr) {
        for (int i = 0; i < size - 1; ++i) {
            if (array[i] > array[i + 1]) {
                return false; // Array is not sorted
            }
        }
        return true; // Array is sorted
    }
    return false;
}

// Function to visualize selection sort
void visualizeSelectionSort(int userInput, double *sortTime, int*& array) {
    int barWidth = screenWidth / userInput;

    // Initialize window for visualization
    CloseWindow();
    InitWindow(screenWidth - screenWidth % userInput, screenHeight, "Selection Sort Visualization");

    auto startTime = std::chrono::high_resolution_clock::now();

    // Selection sort visualization
    for (int i = 0; i < userInput - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < userInput; j++) {
            if (array[j] < array[minIndex]) {
                minIndex = j;
            }
        }
        std::swap(array[i], array[minIndex]);

        // Draw the sorting process
        BeginDrawing();
        ClearBackground(BLACK);

        for (int k = 0; k < userInput; k++) {
            Color recColor = (k <= i) ? GREEN : WHITE; // Draw green rectangles for sorted elements
            DrawRectangle(k * barWidth, screenHeight - array[k], barWidth, array[k], recColor);
        }
        EndDrawing();
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    *sortTime = duration.count();
}

// Function to perform selection sort without visualization
void SelectionSortNoUI(int userInput, double* sortTime, int*& array) {
    auto startTime = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < userInput - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < userInput; j++) {
            if (array[j] < array[minIndex]) {
                minIndex = j;
            }
        }
        std::swap(array[i], array[minIndex]);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    *sortTime = duration.count();
}

// Function to handle user input for the number of elements
void UpdateUserInput(std::string& userInput) {
    int key = GetCharPressed();

    // Only allow numeric keys (0-9)
    if ((key >= 48) && (key <= 57) && (userInput.length() <= 7)) {
        userInput.push_back(static_cast<char>(key));
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !userInput.empty()) {
        userInput.pop_back();
    }

    DrawText("Enter amount of elements:", (GetScreenWidth() - MeasureText("Enter amount of elements:", 40)) / 2, GetScreenHeight() / 2 - 80, 40, GRAY);
    DrawText(userInput.c_str(), (GetScreenWidth() - MeasureText(userInput.c_str(), 60)) / 2, GetScreenHeight() / 2, 60, WHITE);

    if (!userInput.empty() && std::stoi(userInput) > 0) {
        DrawText("Press Enter", (GetScreenWidth() - MeasureText("Press Enter", 40)) / 2, GetScreenHeight() / 2 + 100, 40, GRAY);
    }
}

int main() {
    InitWindow(screenWidth, screenHeight, "Selection Sort Visualization");
    SetTargetFPS(60);

    int numElements;
    int* array = nullptr;
    std::string userInput;
    bool sorting = false;
    double sortTime = 0.0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("NOTE: if the number of elements exceeds 1920 there will be no visualization",
                 (GetScreenWidth() - MeasureText("NOTE: if the number of elements exceeds 1920 there will be no visualization", 40)) / 2,
                 GetScreenHeight() - 100, 40, GRAY);

        if (sorting) {
            numElements = std::stoi(userInput);

            delete[] array;
            array = new int[numElements];
            for (int i = 0; i < numElements; i++) {
                array[i] = GetRandomValue(5, screenHeight);
            }

            if (numElements > 1920) {
                SelectionSortNoUI(numElements, &sortTime, array);
            } else {
                visualizeSelectionSort(numElements, &sortTime, array);
            }

            userInput.clear();
            sorting = false;

        } else {
            std::string timeText = "Time taken for last sorting: " + std::to_string(sortTime) + " seconds";
            DrawText(timeText.c_str(), (GetScreenWidth() - MeasureText(timeText.c_str(), 40)) / 2, GetScreenHeight() / 2 + 300, 40, WHITE);
            UpdateUserInput(userInput);

            if (array != nullptr) {
                if (isSorted(array, numElements)) {
                    DrawText("Array is sorted", (GetScreenWidth() - MeasureText("Array is sorted", 40)) / 2, 50, 40, GREEN);
                } else {
                    DrawText("Array is not sorted", (GetScreenWidth() - MeasureText("Array is not sorted", 40)) / 2, 50, 40, RED);
                }
            }

            if (IsKeyPressed(KEY_ENTER) && std::stoi(userInput) > 0) {
                sorting = true;
            }
        }

        EndDrawing();
    }

    delete[] array;
    CloseWindow();
    return 0;
}
