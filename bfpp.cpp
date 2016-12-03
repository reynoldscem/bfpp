#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <cstdio>
#include <unordered_map>
#include <exception>

const std::string VALID_INSTRUCTIONS = "+-<>[],.";
const int TAPE_SIZE = 30000;

std::vector<char> load_program(std::string filename) {
  std::ifstream inputFile;

  inputFile.open(filename);

  if (!inputFile.is_open() || !inputFile.good()) {
    throw std::runtime_error("File opening failed!");
  }

  std::vector<char> programVector;

  char c = inputFile.get();
  if (VALID_INSTRUCTIONS.find(c) != std::string::npos)
    programVector.push_back(c);
  while (inputFile.good()) {
    c = inputFile.get();
    if (VALID_INSTRUCTIONS.find(c) != std::string::npos)
      programVector.push_back(c);
  }

  inputFile.close();

  return programVector;
}

void execute(const std::vector<char>& programVector) {
  int currentPosition = 0;
  int tapeIndex = 0;
  int programCounter = 0;
  int openBracket = 0;

  std::array<unsigned int, TAPE_SIZE> tape = {0};

  std::unordered_map<int, int> startToEnd;
  std::unordered_map<int, int> endToStart;

  while (programCounter < programVector.size()) {
    switch (programVector[programCounter]) {
      case '+':
        tape[tapeIndex] += 1;
        programCounter++;
        break;
      case '-':
        tape[tapeIndex] -= 1;
        programCounter++;
        break;
      case '>':
        tapeIndex = (tapeIndex + 1 % TAPE_SIZE);
        programCounter++;
        break;
      case '<':
        tapeIndex = (tapeIndex - 1 % TAPE_SIZE);
        programCounter++;
        break;
      case ',':
        tape[tapeIndex] = std::getchar();
        programCounter++;
        break;
      case '.':
        std::cout << (char) tape[tapeIndex];
        programCounter++;
        break;
      case '[':
        if (tape[tapeIndex] == 0) {
          if (startToEnd.count(programCounter) == 1) {
            programCounter = startToEnd[programCounter];
          } else {
            currentPosition = programCounter;
            openBracket = 1;
            while (openBracket >= 1) {
              currentPosition += 1;
              if (programVector[currentPosition] == '[') {
                openBracket += 1;
              } else if (programVector[currentPosition] == ']') {
                openBracket -= 1;
              }
            }
            startToEnd[programCounter] = currentPosition;
            programCounter = currentPosition;
          }
        }
        programCounter += 1;
        break;
      case ']':
        if (tape[tapeIndex] != 0) {
          if (endToStart.count(programCounter) == 1) {
            programCounter = endToStart[programCounter];
          } else {
            currentPosition = programCounter - 1;
            openBracket = 1;
            while (openBracket >= 1) {
              currentPosition -= 1;
              if (programVector[currentPosition] == '[') {
                openBracket -= 1;
              } else if (programVector[currentPosition] == ']') {
                openBracket += 1;
              }
            }
            endToStart[programCounter] = currentPosition;
            programCounter = currentPosition;
          }
        } else {
          programCounter += 1;
        }
        break;
    }
  }
  std::cout << std::flush;
}

int main(int argc, char** argv) {

  if (argc <= 1) {
    std::cout << "No filename given." << std::endl;
    return -1;
  }

  std::vector<char> programVector;
  try {
    programVector = load_program(argv[1]);
  } catch (std::runtime_error& e) {
    std::cout << e.what() << std::endl;
  }

  execute(programVector);

}
