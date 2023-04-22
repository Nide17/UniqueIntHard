/*
 * homework.cpp
 *
 *  Created on: Dec 13, 2020
 *      Author: kompalli
 */

#include "homework.h"

int main(int argc, char** argv) {
  LogManager::resetLogFile();
  LogManager::writePrintfToLog(LogManager::Level::Status, "hw01::processFile", "In main file.");
  

  if (argc < 3){
    printf("\nUsage:\n\nTo use this code, please enter the input and output file path\n\n");
    printf("./homework inFilePath outFilePath\n\n");
    return 0;
  }


  char inputPath[1024];
  char outputPath[1024];

  unsigned long peakMem1 = getPeakRSS();
  unsigned long currMem1 = getCurrentRSS();
  printf("peakRSS = %lu, currMem=%lu\n", peakMem1, currMem1);
  sprintf(inputPath, "%s", argv[1]);
  sprintf(outputPath, "%s", argv[2]);
  
  auto start = std::chrono::high_resolution_clock::now();
  
  UniqueInt::processFile(inputPath, outputPath);
  
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  printf("run time =%d microseconds\n", (int) duration.count());
  
  unsigned long peakMem2 = getPeakRSS();
  unsigned long currMem2 = getCurrentRSS();
  printf("peakRSS = %lu, currMem=%lu\n", peakMem2, currMem2);
  printf("Diff peakRSS = %lu, currMem=%lu\n", peakMem2-peakMem1, currMem2-currMem1);
  
}
