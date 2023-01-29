#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <signal.h>

const int READ = 0;
const int WRITE = 1;

int main(int argc, char** argv) {

  int ps2grep[2];
  pipe(ps2grep);

  int pid2, pid1 = fork();
  if (pid1 == 0) {
    close(ps2grep[READ]);
    dup2(ps2grep[WRITE], WRITE);

    execlp("ps", "ps", "-eaf", NULL);

    exit(0);
  } else {

    int grep2kill[2];
    pipe(grep2kill);
    pid2 = fork();

    if (pid2 == 0) {
      close(ps2grep[WRITE]);
      close(grep2kill[READ]);

      dup2(ps2grep[READ], READ);
      dup2(grep2kill[WRITE], WRITE);

      execlp("grep", "grep", "java", NULL);

      exit(0);
    } else {
      dup2(grep2kill[READ], READ);
      
      waitpid(pid1, nullptr, 0);

      close(ps2grep[WRITE]);
      close(grep2kill[WRITE]);

      waitpid(pid2, nullptr, 0);

      std::string line;
      while (std::getline(std::cin, line)) {
        int uid, pid, ppid, c;
        std::string stime, tty, time, cmd;

        std::stringstream ss(line);
        ss >> uid >> pid >> ppid >> c >> stime >> time >> cmd;

        std::cout << "killing PID: " << pid << std::endl;
        kill(pid, 15);
      }
    }

  }

  return 0;
}  // End of the 'main' function

// END OF FILE
