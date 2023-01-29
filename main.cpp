#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <signal.h>

const int READ = 0;
const int WRITE = 1;

// prototypes
void killPids(int ps2grep[], int grep2kill[], int psPid, int grepPid);
void executeGrep(int ps2grep[], int grep2kill[]);
void executePs(int ps2grep[]);

void killPids(int ps2grep[], int grep2kill[], int psPid, int grepPid) {
  dup2(grep2kill[READ], READ);

  waitpid(psPid, nullptr, 0);

  close(ps2grep[WRITE]);
  close(grep2kill[WRITE]);

  waitpid(grepPid, nullptr, 0);

  std::string line;
  while (std::getline(std::cin, line)) {
    int uid, pid, ppid, c;
    std::string stime, tty, time, cmd;

    std::stringstream ss(line);
    ss >> uid >> pid >> ppid >> c >> stime >> time >> cmd;

    std::cout << "killing PID: " << pid << std::endl;
    kill(pid, 15);
  }
}  // End of the 'killPids' function

void executeGrep(int ps2grep[], int grep2kill[]) {
  close(ps2grep[WRITE]);
  close(grep2kill[READ]);

  dup2(ps2grep[READ], READ);
  dup2(grep2kill[WRITE], WRITE);

  execlp("grep", "grep", "java", NULL);
  exit(0);
}  // End of the 'executeGrep' function

void executePs(int ps2grep[]) {
  close(ps2grep[READ]);
  
  dup2(ps2grep[WRITE], WRITE);

  execlp("ps", "ps", "-eaf", NULL);
  exit(0);
}  // End of the 'executePs' function

int main(int argc, char** argv) {

  int ps2grep[2];
  pipe(ps2grep);

  int grepPid, psPid = fork();
  if (psPid == 0) {
    executePs(ps2grep);
  } else {

    int grep2kill[2];
    pipe(grep2kill);
    grepPid = fork();

    if (grepPid == 0) {
      executeGrep(ps2grep, grep2kill);
    } else {
      killPids(ps2grep, grep2kill, psPid, grepPid);
    }

  }

  return 0;
}  // End of the 'main' function

// END OF FILE
