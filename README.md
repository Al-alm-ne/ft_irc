*This project has been created as part of the 42 curriculum by alde-alm and ggoncalv.*

# ft_irc

## Description

`ft_irc` is a C++ IRC server developed for the 42 curriculum. Its goal is to implement the core behavior of an IRC daemon: accept multiple clients over TCP, authenticate them, manage channels, and handle the common IRC commands used for chatting and moderation.

The project currently covers registration, channel membership, private and channel messages, and basic moderation features such as topics, invites, kicks, and modes. The server is designed to be exercised with standard IRC clients or simple socket tools such as `telnet` and `nc`.

## Instructions

### Compilation

Build the server from the repository root:

```bash
make
```

This produces the `ircserv` executable.

### Execution

Run the server with a port and a password:

```bash
./ircserv <port> <password>
```

Example:

```bash
./ircserv 6667 tert
```

### Basic usage

Connect with a client such as `telnet` or `nc`, then register in this order:

```text
PASS tert
NICK alice
USER alice 0 * :Alice Silva
```

Once registered, you can test commands such as `JOIN`, `PRIVMSG`, `TOPIC`, `INVITE`, `KICK`, `MODE`, `PART`, and `QUIT`.
```
## Resources

Classic references and project background material:

- RFC 1459: Internet Relay Chat Protocol
- RFC 2812: Internet Relay Chat: Client Protocol
- IRC command reference and numeric replies from the official RFCs
- `man 2 socket`, `man 2 bind`, `man 2 listen`, `man 2 accept`, `man 2 poll`, `man 2 recv`, `man 2 send`
- `telnet` and `nc` documentation for raw TCP testing

AI usage in this project:

- AI was used to review the repository structure, summarize the build and execution flow.
- AI was also used to help rewrite this README so it matches the required 42 format and includes practical run and validation instructions.

## Features

- User registration and authentication with `PASS`, `NICK`, and `USER`
- Channel lifecycle and membership management with `JOIN`, `PART`, and `QUIT`
- Private and channel messaging with `PRIVMSG`
- Channel moderation with `TOPIC`, `INVITE`, `KICK`, and `MODE`

## Supported Commands

The server currently supports:

`PASS`, `USER`, `NICK`, `JOIN`, `PRIVMSG`, `QUIT`, `PART`, `TOPIC`, `INVITE`, `KICK`, `MODE`

Unknown commands return numeric reply `421`.

## MODE Command Reference

### Syntax

```text
MODE #channel <+|-><modes> [parameters...]
```

### Supported Channel Modes

| Mode | Description | Needs parameter | Examples |
| --- | --- | --- | --- |
| `i` | Invite-only channel | No | `MODE #general +i`, `MODE #general -i` |
| `t` | Only operators can change topic | No | `MODE #general +t`, `MODE #general -t` |
| `k` | Channel key (password) | `+k` needs key | `MODE #general +k secret123`, `MODE #general -k` |
| `o` | Grant/revoke operator | Yes (nickname) | `MODE #general +o alice`, `MODE #general -o alice` |
| `l` | User limit | `+l` needs limit | `MODE #general +l 20`, `MODE #general -l` |

### Notes

- Only channel operators can change channel modes.
- `+k` and `+l` require parameters.
- `-k` and `-l` do not require parameters.
- `+o` and `-o` require a target nickname.
- Unknown mode flags should produce numeric `472`.

## Quick Validation Checklist

1. Start the server with a valid port and password.
2. Connect with `telnet` or `nc 127.0.0.1 <port>`.
3. Complete registration with `PASS`, `NICK`, and `USER`.
4. Create a channel with `JOIN #general`.
5. Test messaging with `PRIVMSG`.
6. Test moderation with `TOPIC`, `INVITE`, `KICK`, and `MODE`.
7. Leave with `PART` and finish with `QUIT`.