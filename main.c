#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define the Sleep enum
typedef enum sleep {
    NONE = 0, ASLEEP = 1, AWAKE = 2, REALLYAWAKE = 3
} Sleep;

// Define the FitbitData struct
typedef struct fitbit {
    char patient[10];
    char minute[9];
    double calories;
    double distance;
    unsigned int floors;
    unsigned int heartRate;
    unsigned int steps;
    Sleep sleepLevel;
} FitbitData;

#define MAX_RECORDS 1440
#define MAX_LINE_LENGTH 100

// Cleanse data because some records need a little love and care!
void cleanse_data(FitbitData *data) {
    if (data->calories < 0) data->calories = 0; // No burning negative calories, please!
    if (data->distance < 0) data->distance = 0; // Can’t walk negative miles either.
    if (data->floors == 0) data->floors = 0; // Default to zero for floors climbed.
    if (data->heartRate == 0) data->heartRate = 60; // Heart’s gotta beat—default it.
    if (data->steps == 0) data->steps = 0; // No phantom steps allowed.
    if (data->sleepLevel < NONE || data->sleepLevel > REALLYAWAKE) data->sleepLevel = NONE; // Catch invalid sleep states.
}

// Detect duplicate entries, because no one likes deja vu in data.
int is_duplicate(FitbitData *records, int count, FitbitData *current) {
    for (int i = 0; i < count; ++i) {
        if (strcmp(records[i].patient, current->patient) == 0 && strcmp(records[i].minute, current->minute) == 0) {
            return 1; // Found a duplicate. Oops!
        }
    }
    return 0;
}

// Data fixer-upper function to handle messy CSV entries!
void dataFix(FILE* readStream) {

    FILE* resultsFile = fopen("FixedData.csv", "w");
    if (resultsFile == NULL) {
        return; // Well, that’s unfortunate. No file for you!
    }

    int num = 0;
    char line[75];
    // Read the first two lines to skip headers. They’re not the juicy data.
    fgets(line, 75, readStream);
    fgets(line, 75, readStream);

    while (!feof(readStream)) {
        fgets(line, 75, readStream);

        for (int i = 0; i < 75; i++) {
            if (line[i] != '\n') {
                fprintf(resultsFile, "%c", line[i]);
                if (line[i] == ',') {
                    if (line[i + 1] == ',') {
                        fprintf(resultsFile, "%c", ' '); // Fill in those empty fields with spaces!
                    }
                }
            } else {
                fprintf(resultsFile, "%c", '\n'); // Newline means next record, folks!
                i = 75;
            }
        }
    }
    fclose(resultsFile); // Don’t forget to close the file—file leaks are a no-no!
}

int main() {
    FILE *inputFile = fopen("FitbitData.csv", "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Error: Could not open FitbitData.csv\n");
        return 1; // Abort mission—no input file!
    }

    // Fix the data first
    dataFix(inputFile);
    fclose(inputFile);

    // Reopen the fixed data file
    inputFile = fopen("FixedData.csv", "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Error: Could not open FixedData.csv\n");
        return 1; // Abort mission—no fixed data file!
    }

    FILE *outputFile = fopen("Results.csv", "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Error: Could not open Results.csv\n");
        fclose(inputFile);
        return 1; // Double fail—no output file!
    }

    FitbitData records[MAX_RECORDS];
    int recordCount = 0;

    char targetPatient[10];
    char line[MAX_LINE_LENGTH];

    // Read the target patient ID. This is the star of our show!
    if (fgets(line, sizeof(line), inputFile)) {
        sscanf(line, "%9[^,]", targetPatient);
    }

    // Process each line in the input file. Let’s dive into the data!
    while (fgets(line, sizeof(line), inputFile)) {
        FitbitData current;
        char sleepLevelStr[5];

        int fields = sscanf(line, "%9[^,],%8[^,],%lf,%lf,%u,%u,%u,%4s",
                            current.patient,
                            current.minute,
                            &current.calories,
                            &current.distance,
                            &current.floors,
                            &current.heartRate,
                            &current.steps,
                            sleepLevelStr);

        if (fields < 8 || strcmp(current.patient, targetPatient) != 0) {
            continue; // Skip invalid or unrelated records. Ain’t nobody got time for that!
        }

        current.sleepLevel = atoi(sleepLevelStr);
        cleanse_data(&current);

        if (!is_duplicate(records, recordCount, &current) && recordCount < MAX_RECORDS) {
            records[recordCount++] = current; // Store the unique and cleansed record.
        }
    }

    fclose(inputFile); // Bye-bye input file!

    // Analyze data like a pro data scientist (or something).
    double totalCalories = 0, totalDistance = 0;
    unsigned int totalFloors = 0, totalSteps = 0;
    unsigned int maxSteps = 0;
    unsigned int totalHeartRate = 0;
    unsigned int heartRateCount = 0;
    char maxMinute[9] = "";

    for (int i = 0; i < recordCount; ++i) {
        totalCalories += records[i].calories;
        totalDistance += records[i].distance;
        totalFloors += records[i].floors;
        totalSteps += records[i].steps;

        if (records[i].heartRate > 0) {
            totalHeartRate += records[i].heartRate;
            heartRateCount++;
        }

        if (records[i].steps >= maxSteps) {
            maxSteps = records[i].steps;
            strcpy(maxMinute, records[i].minute); // Update the max-minute too!
        }
    }

    double avgHeartRate = heartRateCount ? (double)totalHeartRate / heartRateCount : 0; // Average it out!

    // Write results to output file. Drumroll, please…
    fprintf(outputFile, "Total Calories,Total Distance,Total Floors,Total Steps,Avg Heartrate,Max Steps,Sleep\n");
    fprintf(outputFile, "%.2f,%.2f,%u,%u,%.2f,%u\n", totalCalories, totalDistance, totalFloors, totalSteps, avgHeartRate, maxSteps);

    // Output filtered and cleansed data because who doesn’t love clean data?
    for (int i = 0; i < recordCount; ++i) {
        fprintf(outputFile, "%s,%s,%.2f,%.2f,%u,%u,%u,%d\n",
                records[i].patient,
                records[i].minute,
                records[i].calories,
                records[i].distance,
                records[i].floors,
                records[i].heartRate,
                records[i].steps,
                records[i].sleepLevel);
    }

    fclose(outputFile); // See ya, output file!

    printf("Processing complete. Results saved to Results.csv\n"); // Victory message!
    return 0;
}
