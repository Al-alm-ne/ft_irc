# ft_irc

A minimal IRC server implementation based on RFC 1459, built for the 42 school project.

## Features

- User registration and authentication (`PASS`, `NICK`, `USER`)
- Channel lifecycle and membership (`JOIN`, `PART`, `QUIT`)
- Private and channel messaging (`PRIVMSG`)
- Channel moderation (`TOPIC`, `INVITE`, `KICK`, `MODE`)

## Supported Commands

The server currently supports:

`PASS`, `USER`, `NICK`, `JOIN`, `PRIVMSG`, `QUIT`, `PART`, `TOPIC`, `INVITE`, `KICK`, `MODE`

Unknown commands return numeric reply `421`.

## Build and Run

```bash
make
./ircserv <port> <password>
```

Example:

```bash
./ircserv 6667 secret
```

Connect with a client (or raw socket):

```bash
nc localhost 6667
```

## Registration Flow

After connecting, register in this order:

```text
PASS secret
NICK alice
USER alice 0 * :Alice Liddell
```

## Command Test Matrix

| Command | Example | What to test | Expected behavior |
| --- | --- | --- | --- |
| `PASS` | `PASS secret` | Try wrong password, then correct password. | Wrong: `464`. Correct: password step accepted. |
| `NICK` | `NICK alice` | Test empty, invalid, and duplicate nickname. | `431`, `432`, `433` depending on case. |
| `USER` | `USER alice 0 * :Alice Liddell` | Send after `PASS` and `NICK`. | Completes registration and may trigger `001`. |
| `JOIN` | `JOIN #general` | Join a new channel and a restricted one. | Creates channel if needed; replies include `JOIN`, `353`, `366`. |
| `PRIVMSG` | `PRIVMSG #general :hello` | Missing recipient, missing text, unknown target, no membership. | `411`, `412`, `401`, `404`. |
| `PART` | `PART #general :leaving` | Leave valid channel, invalid channel, not a member. | `461`, `403`, `442`. |
| `TOPIC` | `TOPIC #general :new topic` | Query vs set topic. | Query: `331` or `332`; set: broadcast to channel. |
| `INVITE` | `INVITE bob #general` | Member vs non-operator behavior. | Requires 2 params; success returns `341`. |
| `KICK` | `KICK #general bob :off-topic` | Operator-only kick and target checks. | Non-member target returns `441`; success removes user. |
| `MODE` | `MODE #general` | Read and mutate channel modes. | Query returns `324`; unknown mode returns `472`. |
| `QUIT` | `QUIT :bye` | Disconnect client cleanly. | Connection is closed safely. |

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

1. Start the server with valid port and password.
2. Connect with `nc localhost <port>`.
3. Complete registration (`PASS`, `NICK`, `USER`).
4. Create a channel with `JOIN #general`.
5. Test messaging with `PRIVMSG`.
6. Test moderation with `TOPIC`, `INVITE`, `KICK`, and `MODE`.
7. Leave with `PART` and finish with `QUIT`.