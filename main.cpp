#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fs.h>
#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <boost/asio.hpp>
#include "server.hpp"
#include "threadpool.hpp"
#include "log.h"

int main(int argc, char* argv[])
{
  try
  {
   FILE_LOG(logINFO) << "Starting...";
    int opt = 0;
    std::string addr;
    std::string port;
    std::string dir;
    while ((opt = getopt(argc, argv, "hpd")) != -1) {
      switch (opt) {
      case 'h':
        addr = argv[optind];
        break;
      case 'p':
        port = argv[optind];
        break;
      case 'd':
        dir = argv[optind];
        break;
      default: /* '?' */
        printf("failed to parse parameter %c", (char) opt);
        exit(EXIT_FAILURE);
      }
    }

    if (daemon(0, 0) == -1) {
      perror("daemon failed");
      return -1;
    }
    
    FILE_LOG(logINFO) << "address " << addr << " port " << port
                      << " directory " << dir;

    FILE_LOG(logINFO) << "thread count "
                      << boost::thread::hardware_concurrency();
    thread_pool pool(boost::thread::hardware_concurrency());
    // Initialise the server.
    http::server::server s(pool.get_io_service(), addr, port, dir);

    // Run the server until stopped.
    pause();
  }
  catch (std::exception& e)
  {
    FILE_LOG(logINFO) << "exception: " << e.what();
  }

  return 0;
}
