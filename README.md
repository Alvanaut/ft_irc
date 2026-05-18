*This project has been created as part of the 42 curriculum by alvanaut, lbolens, lucorrei*

# Description
The goal of this project is to create a **C++** implementation of a simple IRC server, without the need for server-server connections. In modern terms it's the equivalent of a text-only Slack or Discord server with channels, permissions, multiple connections etc.

We elected to use the `epoll_wait()` syscall as it is nonblocking, simplified the managemement of multiple open TCP connections, and is a pleasant interface to work with. Especially when limited to a single call per loop, as the subject dictates.

We chose `irssi` as our client of choice as it is available on school machines, terminal-based, and very robust.

We architected the project in a Object-Oriented way, with a `Server` class responsible for all of the connection logic, inside it `std::map` containers for both `Client`s and `Channel`s and a *Message*-ish pattern for the various commands an IRC client can send. (They all inherit from the abstract `Command` class and implement its `virtual void execute(...)` method). While this makes the code very intuitive to read and write, and contains the behaviour well within each `Command`, if we were to redo the project we would take a more imperative approach to better keep track of execution state.

# Instructions
1. Ensure you have the LLVM C++ compiler aliased to  `c++`, its libraries, and `make`.
2. `git clone` this repository and `cd` into it.
3. run `make` if you just want the executable or `make test` to have a simple `bash` script run tests for most error codes.
4. You can now call `./ircserv <PORT> <PASSWORD>` and connect to it in another terminal with `irssi -c 0.0.0.0 -p <PORT> -w <PASSWORD>` (or through their TUI).

# Resources
We based our work on the original Internet Relay Chat Protocol [RFC](https://www.rfc-editor.org/rfc/rfc1459), its [update](https://www.rfc-editor.org/rfc/rfc2812) and the modern [documentation](https://modern.ircdocs.horse/).

AI was used at all stages of this project as a helper, not a crutch:

- In the beginning to clarify concepts both in architecturing a medium-sized C++ project and in networking using the `epoll` syscall.
- In the thick of it it helped implement a tonne of the boilerplate required by C++ classes (getters, setters, declarations, etc).
- At the end we used it to write the tests in `tests.sh` to verify correct behaviour.
