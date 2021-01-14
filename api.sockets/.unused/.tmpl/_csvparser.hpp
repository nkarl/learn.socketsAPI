#ifndef _CSVPARSER_HPP
#define _CSVPARSER_HPP

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

/*****************************************************************
 * NAME: CSVPARSER LIBRARY
 * AUTHOR: CHARLES NORDEN
 * DESCRIPTION: a csv data processor.
 * FUNCTIONS:
 * This processor does the following tasks:
 *  1. OPEN FILE STREAM:
 *      - Accept an filepath (passed as std::string)
 *      - Open a *.csv file-stream at that path
 *          (MUST check if opening was SUCCESSFULLY)
 *
 *  2.A EXTRACT A LINE OF RAW STRING:
 *      - Scan each line with the delimiter '\n'
 *
 *  2.B PROCESS RAW STRING INTO STRINGS OF DATA TYPES:
 *      - Process the line into smaller strings separated by ','
 *
 *  2.C STORE THE STRING PACKETS:
 *      - Push each std::string to its own <std::string>vector
 *      - Push the <std::string>vector into a container vector of
 *<std::string>vectors
 *
 *
 * ---------------------------------------------------------------
 * FUTURE PLAN: REWRITE THE DATABASE WITH HASHING AND MAPPING.
 * ---------------------------------------------------------------
 * VER: 0.2.3     2020/11/11
 *      Fixed namespace std error. Prefixing all containers with <std::>.
 *
 * VER: 0.2.2.1   2020/11/05
 *      Implemented clearer naming convention for various functions
 *
 * VER: 0.2.2     2020/11/02
 *      Changed the format of the hidden functions.
 *
 * VER: 0.2.1     2020/11/01
 *      Added an overloaded constructor in case delimiter not
 *      supplied.
 *
 * VER: 0.2.0     2020/11/01
 *      FIXED THE RETURN CARRIAGE BUG.
 *
 * PROTOTYPE VERSION 0.1 COMPLETED 2020/10/31.
 *
 *****************************************************************/

// "ERROR: Could not locate source csv at path.";
const std::string ERROR_WRONG_FILEPATH =
    "ERROR: Could not locate source csv at path.";
// "ERROR: Incorrect number of data columns given.";
const std::string ERROR_NO_DATA_FOUND =
    "ERROR: Incorrect number of data columns given.";

const char DEFAULT_COLUMNS_SEPARATOR = ',';

/* Class Container for streaming *.csv as input. Its path cannot be
 * changed once set.
 */
class csvStream {
 protected:
  /****************************SEMI-HIDDEN***************************/
  std::string fpath;
  std::fstream inStream;
  // Check if stream is open.
  void e_isOpenStream() {
    if (!inStream.is_open()) {
      std::cout << ERROR_WRONG_FILEPATH;
      exit(1);
    }
  }
  void _openStream() { inStream.open(fpath, std::ios::in); }
  void _resetStream() {
    inStream.close();
    _openStream();
    e_isOpenStream();
  }

 public:
  /****************************PUBLIC-API****************************/
  // Constructor: set up new csv input stream.
  explicit csvStream(const std::string& path) : fpath{path} {
    _openStream();
    e_isOpenStream();
  }
  // Destructor: close stream and unload memory at exit.
  ~csvStream() { inStream.close(); }
  csvStream(const csvStream&) = delete;
  void operator=(const csvStream&) = delete;
  csvStream(csvStream&&) = delete;
  void operator=(csvStream&&) = delete;
};

/* Class Container for extracting raw data from an open csvStream. Data are
 * extracted into packets of strings.
 */
template <typename D>
class csvParser : protected csvStream {
  /******************************HIDDEN******************************/
 private:
  int numSplitters;
  char colSplitter;
  std::string rawLine;

  // Database of extracted packets of data as strings.
  std::vector<std::vector<std::string>*> database;

  // Unload database from memory.
  void _clearDatabase() {
    for (auto i : database) {
      delete i;
    }
  }
  // Get the number of column splitters.
  void _getNumsColSplitters() {
    // std::vector<int> fileInfo{0, 0}; // future implementation for csv size
    std::string line;
    getline(inStream, line);
    for (auto i : line) {
      if (i == colSplitter) ++numSplitters;
    }
  }

  // Check if the number of columns of data is a positive integer.
  void e_validateRawData() {
    if (numSplitters <= 0) {
      std::cout << ERROR_NO_DATA_FOUND;
      exit(1);
    }
  }

  // Extract a packet of strings and Return a reference to it.
  std::vector<std::string>& _rawToStringPacket() {
    std::istringstream line_stream;
    static std::string token;

    // Create new packet on the Heap
    static std::vector<std::string>* packet = nullptr;
    packet = new std::vector<std::string>;

    // Handle the return carriage character.
    size_t tail = rawLine.size() - 1;
    if (rawLine.at(tail) == '\r') rawLine.erase(tail);

    // Extract data packet from the line.
    line_stream.str(rawLine);
    for (auto i = 1; i <= numSplitters + 1; ++i) {
      getline(line_stream, token, colSplitter);
      packet->push_back(token);
    }
    return *packet;
  }

  /****************************PUBLIC-API****************************/
 public:
  // Constructor: Must supply path to a*.csv file, number of columns and
  // optional columns splitter.
  csvParser(const std::string& path, const int ntypes, const char sptr = ',')
      : csvStream(path), numSplitters{ntypes - 1}, colSplitter{sptr} {}
  // Constructor: If number of columns not given, open file and check.
  explicit csvParser(const std::string& path, const char sptr = ',')
      : csvStream(path) {
    colSplitter = sptr;
    numSplitters = 0;
    _getNumsColSplitters();
    _resetStream();
  }

  // Destructor: Clear database at exit.
  ~csvParser() { _clearDatabase(); }
  csvParser(const csvParser&) = delete;
  void operator=(const csvParser&) = delete;
  csvParser(csvParser&&) = delete;
  void operator=(csvParser&&) = delete;

  // Extract all data while stream is open.
  void extractAllRaw() {
    this->e_isOpenStream();
    this->e_validateRawData();
    // Continue to extract data until the end of file:
    while (getline(inStream, rawLine)) {
      database.push_back(&_rawToStringPacket());
    }
  }

  bool DEBUG_isOpenStream() { return this->inStream.is_open(); }
  // Stream the first 10 data elements to the standard output.
  void printFirst10() const {
    for (auto i : database) {
      if (i > database.at(9)) return;
      for (auto j : *i)
        std::cout << j << std::setw(20);
      std::cout << '\n';
    }
  }

  // Access the database interface.
  std::vector<std::vector<std::string>*>& accessDatabase() {
    return this->database;
  }

  // Template function for processing raw to real operating data.
  void rawToRealData(D&, int);
};

#endif