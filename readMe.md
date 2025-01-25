# README for Fitbit Data Analysis Program

## Apology to the Teaching Assistant
Dear Jaehong or other Teaching Assistant,

I sincerely apologize for the potential inconvenience caused by my decision to use the CLion IDE to develop this program. I understand that this might differ from the development tools typically used in the course, such as Visual Studio on Windows. I hope the provided instructions make it easy for you to run and evaluate my work regardless of the development environment. Thank you for your patience and understanding!

## How to Run the Program on Visual Studio (Windows)
If you are using Visual Studio on Windows, please follow these steps to run the Fitbit Data Analysis program:

1. **Download and Extract the Files**:
    - Download all the project files, including the `main.c`, `FitbitData.csv`, and `Results.csv` files.
    - Save them in a folder on your computer.

2. **Open Visual Studio**:
    - Launch Visual Studio and create a new project.
    - Select **Console Application** from the project templates (C language).

3. **Add the Source Code**:
    - Copy and paste the contents of `main.c` into the `main.c` file in your Visual Studio project.
    - Alternatively, you can add the `main.c` file directly to the project by right-clicking the `Source Files` folder in the Solution Explorer and selecting **Add > Existing Item**.

4. **Place the Input File**:
    - Ensure the `FitbitData.csv` file is placed in the same directory as the Visual Studio project executable (typically in the `Debug` or `Release` folder after building).

5. **Build the Project**:
    - Click on **Build > Build Solution** (or press `Ctrl+Shift+B`) to compile the program.
    - Resolve any compiler errors that may arise due to differences between CLion and Visual Studio settings.

6. **Run the Program**:
    - After building successfully, run the program by clicking **Debug > Start Without Debugging** (or pressing `Ctrl+F5`).
    - The program will process the `FitbitData.csv` file and create a `Results.csv` file in the same directory as the executable.

## Notes
- If any errors occur during compilation or execution, please feel free to reach out, and I will address them promptly.
- The `Results.csv` file will contain both the computed summary statistics and the filtered, deduplicated, and cleansed Fitbit data.

Thank you for your time and understanding!

