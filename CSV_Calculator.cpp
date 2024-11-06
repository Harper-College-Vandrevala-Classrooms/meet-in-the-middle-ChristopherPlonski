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

int main() {
	cout << "Enter the .csv filePath: ";
	string userFilePathInput;
	cin >> userFilePathInput;

    vector<int> csvFileDataVector = get_csv_file_values(userFilePathInput);

    //cout << "Data: {";
    //for (int integer : csvFileData) {
    //    cout << integer << ", ";
    //}
    //cout << "}" << endl;

    cout << "Data median: " << get_vector_median(csvFileDataVector);
}