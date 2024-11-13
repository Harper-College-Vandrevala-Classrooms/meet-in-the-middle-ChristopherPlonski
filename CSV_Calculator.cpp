#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <optional>

using namespace std;

optional<int> try_get_int_from_string(const string stringToConvert)
{
    //cout << "stringToConvert: " << stringToConvert;
    try
    {
        // Convert string to int
        int convertedIntFromString = stoi(stringToConvert);

        return convertedIntFromString;
    }
    catch (const invalid_argument& e)
    {
        return nullopt;
    }
    catch (const out_of_range& e)
    {
        return nullopt;
    }

    return nullopt;
}

string vector_data_as_string(vector<int> vector) {
    string stringVectorData = "";

    stringVectorData += "{";
    const int VECTOR_SIZE = vector.size();
    for (int i = 0; i < VECTOR_SIZE; i++) {
        stringVectorData += to_string(vector.at(i));

        if (i < VECTOR_SIZE - 1) {
            stringVectorData += ", ";
        }
    }

    stringVectorData += "}";

    return stringVectorData;
}

bool is_value_in_vector(vector<int> vector, int valueToFind) {
    for (int i = 0; i < vector.size(); i++) {
        if (vector.at(i) == valueToFind) {
            return true;
        }
    }

    return false;
}

void add_string_line_int_values_to_vector(string line, vector<int> &vectorToOutputTo) {
    bool keepAddingValues = true;
    
    int startIndex = 0;
    while (keepAddingValues) {
        size_t nextCommaIndex = line.find_first_of(',', startIndex);
        //cout << "Start index: " << startIndex << " | endIndex: " << nextCommaIndex << endl;

        if (nextCommaIndex == string::npos) {
            // If no other comma, get the value of start -> the end of the string.
            keepAddingValues = false;
            nextCommaIndex = line.size();
        }

        optional<int> optionalInt = try_get_int_from_string(line.substr(startIndex, nextCommaIndex - startIndex));

        if (optionalInt.has_value()) {
            vectorToOutputTo.push_back(optionalInt.value());
        }

        startIndex = nextCommaIndex + 1;
    }
}

vector<int> get_csv_file_values(string filePath) {
    cout << "Analyzing csv file with path: " << filePath << endl;

    ifstream csvFile;
    vector<int> csvFileValues = vector<int>();

    csvFile.open(filePath);

    int numberOfGamesNamesWithCAsFirstLetter = 0;

    if (csvFile.is_open())
    {
        string line;
        while (getline(csvFile, line))
        {
            add_string_line_int_values_to_vector(line, csvFileValues);
        }
    }
    else
    {
        cout << "Sorry, the file could not be openend." << endl;
    }

    return csvFileValues;
}

// Return type as double as if return type was int and dataSet had an even amount of contents
// and the 2 middle values are odd, division would get returned as the decimal value turned converted to int (ex: 15.5 -> 15).
double get_vector_median(const vector<int> dataVector) {
    bool oddNumberOfData = dataVector.size() % 2 == 1;

    if (oddNumberOfData) {
        int medianIndex = dataVector.size() / 2; // Ex: size = 5 | median = 5/2 = 2 | 0, 1, 2, 3, 4 -> 2 is median!
        return dataVector[medianIndex];
    }
    else {
        // Ex: size = 6 | median1 = 6/2 = 3 | median2 = 3 - 1 = 2 | 0, 1, 2, 3, 4, 5 -> 2 and 3 is median!
        int firstMedianIndex = dataVector.size() / 2;
        int secondMedianIndex = firstMedianIndex - 1;

        return ((double)(dataVector[firstMedianIndex] + dataVector[secondMedianIndex])) / 2;
    }
}

vector<int> get_vector_mode(const vector<int> dataVector) {
    vector<int> modeVector = vector<int>();
    int numberOfTimeModesAppear = 0;

    int currentAnalyzingIntValue = 0;
    int numberOfTimesCurrentAnalyzingValueAppears = 0;

    for (int i = 0; i < dataVector.size(); i++) {
        int valueAtIndex = dataVector.at(i);

        if (currentAnalyzingIntValue != valueAtIndex) {
            currentAnalyzingIntValue = valueAtIndex;
            numberOfTimesCurrentAnalyzingValueAppears = 0;
        }

        numberOfTimesCurrentAnalyzingValueAppears++;

        if (is_value_in_vector(modeVector, valueAtIndex)) {
            // This value is now in the data set more than all the other modes, so clear it and have this as the only mode
            modeVector.clear();
            modeVector.push_back(valueAtIndex);
            numberOfTimeModesAppear++;
        }
        else if (numberOfTimesCurrentAnalyzingValueAppears == numberOfTimeModesAppear) {
            // This value is now at the same level as all the other modes, so add it to the list.
            modeVector.push_back(valueAtIndex);
        }
        else if (numberOfTimeModesAppear == 0) {
            // This is the beginning of the mode analyzing, which means this value has to be the current mode no matter what.
            modeVector.push_back(valueAtIndex);
            numberOfTimeModesAppear++;
        }
    }

    return modeVector;
}

void sort_csv_file_values(vector<int> &csvFileDataVector) {
    cout << "Data before sort: " << vector_data_as_string(csvFileDataVector) << endl;

    // Ex: {10, 5, 3, 7, 1, 12} - Size = 6
    const int DATA_SIZE = csvFileDataVector.size();

    for (int i = 0; i < DATA_SIZE - 1; i++) {
        bool isDataPerfectlySorted = true;

        for (int j = 0; j < DATA_SIZE - 1; j++) {
            int value1 = csvFileDataVector.at(j);
            int value2 = csvFileDataVector.at(j + 1);

            if (value2 < value1) {
                isDataPerfectlySorted = false;
                csvFileDataVector[j] = value2;
                csvFileDataVector[j + 1] = value1;
                //csvFileDataVector.assign(j, value2);
                //csvFileDataVector.assign(j + 1, value1);
            }
        }

        if (isDataPerfectlySorted) {
            break;
        }
    }

    cout << "Data after sort: " << vector_data_as_string(csvFileDataVector) << endl;
}

int main() {
	cout << "Enter the .csv filePath: ";
	string userFilePathInput;
	cin >> userFilePathInput;

    vector<int> csvFileDataVector = get_csv_file_values(userFilePathInput);
    sort_csv_file_values(csvFileDataVector);

    cout << "Data median: " << get_vector_median(csvFileDataVector) << endl;
    cout << "Data mode: " << vector_data_as_string(get_vector_mode(csvFileDataVector)) << endl;
}