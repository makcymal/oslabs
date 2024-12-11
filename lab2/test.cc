#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "forkexec.h"


namespace test {

namespace internal {

void test_output(const char *cmd[], std::string expected) {
  std::system("rm -f output.txt");
  assert(forkexec(const_cast<char **>(cmd)) == 0);

  std::ifstream output("output.txt");
  std::string line;
  std::stringstream ss;
  while (std::getline(output, line)) {
    ss << line << "\n";
  }
  
  if (ss.str() != expected) {
    std::cout << ss.str() << "\n" << expected << "\n";
  }
  assert(ss.str() == expected);
  std::system("rm -f output.txt");
}

}  // namespace internal

void bash_echo_env() {
  const char *cmd[] = {
    "/bin/bash", "-c", "echo $BACKBONE_LAPTOP >> output.txt", NULL};
  std::string expected = "1\n";
  internal::test_output(cmd, expected);
}

void bash_ls() {
  const char *cmd[] = {"/bin/bash", "-c", "ls >> output.txt", NULL};
  std::string expected =
    "CMakeLists.txt\nforkexec.cc\nforkexec.h\noutput.txt\ntest.cc\n";
  internal::test_output(cmd, expected);
}

void bash_ls_grep() {
  const char *cmd[] = {"/bin/bash", "-c", "ls | grep .cc >> output.txt", NULL};
  std::string expected = "forkexec.cc\ntest.cc\n";
  internal::test_output(cmd, expected);
}

void bash_pwd() {
  const char *cmd[] = {"/bin/bash", "-c", "pwd >> output.txt", NULL};
  std::string expected = "/home/makcymal/prog/os/lab2\n";
  internal::test_output(cmd, expected);
}

}  // namespace test


int main(int argc, char *argv[]) {
  test::bash_echo_env();
  test::bash_ls();
  test::bash_ls_grep();
  test::bash_pwd();
  return EXIT_SUCCESS;
}
