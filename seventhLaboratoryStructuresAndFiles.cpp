/****************************************************
 * Автор:   Сабитов Ильнур                          *
 * Задание: Структуры и работа с файлами            *
 * Вариант: 28                                      *
 ****************************************************/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Game {
  string name;
  string genre;
  int year;
  string processor;
  int ram;
};

vector<string> splitFields(const string& line) {
  vector<string> fields;
  string field;
  size_t symbolIndex;

  field = "";
  for (symbolIndex = 0; symbolIndex < line.size(); ++symbolIndex) {
    if (line[symbolIndex] == ';') {
      fields.push_back(field);
      field = "";
    }
    else {
      field = field + line[symbolIndex];
    }
  }
  fields.push_back(field);

  return fields;
}

bool readInt(const string& text, int& value) {
  istringstream parser(text);
  char extra;

  if (!(parser >> value)) {
    return false;
  }

  return !(parser >> extra);
}

bool readGames(const string& path, vector<Game>& games) {
  const int maxCount = 100;
  const int fieldCount = 5;
  const int minYear = 1950;
  const int maxYear = 2100;
  const int minRam = 1;
  const int maxRam = 1000000;

  ifstream input(path);
  string line;
  vector<string> fields;
  Game current;
  int lineNumber;

  if (!input) {
    cout << "ERROR input\n";
    return false;
  }

  lineNumber = 0;
  while (getline(input, line)) {
    ++lineNumber;

    if (!line.empty() && line[line.size() - 1] == '\r') {
      line.erase(line.size() - 1);
    }

    fields = splitFields(line);
    if (lineNumber > maxCount || fields.size() != fieldCount) {
      cout << "ERROR line " << lineNumber << "\n";
      return false;
    }

    if (!readInt(fields[2], current.year) || !readInt(fields[4], current.ram) ||
      fields[0].empty() || fields[1].empty() || fields[3].empty() ||
      current.year < minYear || current.year > maxYear ||
      current.ram < minRam || current.ram > maxRam) {
      cout << "ERROR line " << lineNumber << "\n";
      return false;
    }

    current.name = fields[0];
    current.genre = fields[1];
    current.processor = fields[3];
    games.push_back(current);
  }

  if (input.bad()) {
    cout << "ERROR read\n";
    return false;
  }

  return true;
}

bool findLatestGame(const vector<Game>& games, const string& genre, size_t& index) {
  size_t gameIndex;
  bool isFound;

  isFound = false;
  for (gameIndex = 0; gameIndex < games.size(); ++gameIndex) {
    if (games[gameIndex].genre == genre) {
      if (!isFound || games[gameIndex].year > games[index].year) {
        index = gameIndex;
        isFound = true;
      }
    }
  }

  return isFound;
}

bool findLeastRamGame(const vector<Game>& games, size_t& index) {
  size_t gameIndex;
  bool isFound;

  isFound = false;
  for (gameIndex = 0; gameIndex < games.size(); ++gameIndex) {
    if (!isFound || games[gameIndex].ram < games[index].ram) {
      index = gameIndex;
      isFound = true;
    }
  }

  return isFound;
}

string makeReport(const vector<Game>& games, const string& genre) {
  ostringstream report;
  size_t gameIndex;
  size_t latestIndex;
  size_t leastRamIndex;
  int genreCount;

  report << "Games of the genre " << genre << ":\n";
  genreCount = 0;
  for (gameIndex = 0; gameIndex < games.size(); ++gameIndex) {
    if (games[gameIndex].genre == genre) {
      report << games[gameIndex].name << "; " << games[gameIndex].genre << "; "
        << games[gameIndex].year << "; " << games[gameIndex].processor << "; "
        << games[gameIndex].ram << " MB\n";
      ++genreCount;
    }
  }
  if (genreCount == 0) {
    report << "EMPTY\n";
  }

  if (findLatestGame(games, genre, latestIndex)) {
    report << "Latest game of the genre: " << games[latestIndex].name << ", "
      << games[latestIndex].year << "\n";
  }
  else {
    report << "Latest game of the genre: NONE\n";
  }

  if (findLeastRamGame(games, leastRamIndex)) {
    report << "Game with the least RAM: " << games[leastRamIndex].name << ", "
      << games[leastRamIndex].ram << " MB\n";
  }
  else {
    report << "Game with the least RAM: NONE\n";
  }

  return report.str();
}

bool writeReport(const string& path, const string& text) {
  ofstream output(path);

  if (!output) {
    cout << "ERROR output\n";
    return false;
  }

  output << text;
  output.flush();
  if (!output) {
    cout << "ERROR write\n";
    return false;
  }

  return true;
}

int main() {
  vector<Game> games;
  string inputPath;
  string outputPath;
  string genre;
  string report;

  cout << "Enter the input file path: ";
  if (!getline(cin, inputPath) || inputPath.empty()) {
    cout << "INVALID\n";
    return 1;
  }

  cout << "Enter the output file path: ";
  if (!getline(cin, outputPath) || outputPath.empty()) {
    cout << "INVALID\n";
    return 1;
  }

  cout << "Enter the genre: ";
  if (!getline(cin, genre) || genre.empty()) {
    cout << "INVALID\n";
    return 1;
  }

  if (!readGames(inputPath, games)) {
    return 1;
  }

  report = makeReport(games, genre);

  if (!writeReport(outputPath, report)) {
    return 1;
  }
  cout << report;

  return 0;
}