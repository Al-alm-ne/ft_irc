# ft_irc

A minimal IRC server implementation following the RFC 1459 specification and the requirements of the 42 school project.  
Supports basic user registration, channel management, messaging, and channel modes.

---

## Supported Commands

The server currently registers the following IRC commands:

`PASS`, `USER`, `NICK`, `JOIN`, `PRIVMSG`, `QUIT`, `PART`, `TOPIC`, `INVITE`, `KICK`, and `MODE`.

Commands outside this list are treated as unknown and must return numeric `421`.

---

## How to Run

Build and start the server:

```bash
make
./ircserv <port> <password>

Example:

```bash
./ircserv 6667 secret
```

Then connect with any IRC client or a raw socket client such as 'nc'or 'telnet':

```bash
nc localhost 6667
```

## Manual Test Flow

Start with the registration sequence below:

PASS secret
NICK alice
USER alice 0 * :Alice Liddell
```

After that, test the commands below one by one.

| Command | Example | How to test | Expected behavior |
|---|---|---|---|
| `PASS` | `PASS secret` | Try a wrong password first, then the correct one. | Wrong password returns `464`. Correct password unlocks registration. |

| `NICK` | `NICK alice` | Test empty, invalid, and duplicated nicknames. | `431`, `432`, or `433` depending on the case. |

| `USER` | `USER alice 0 * :Alice Liddell` | Send it after `PASS` and `NICK`. | Completes registration and may trigger `001`. |

| `JOIN` | `JOIN #general` | Join a new channel and a restricted channel. | Joins the channel, creates it if needed, and replies with `JOIN`, `353`, and `366`. |

| `PRIVMSG` | `PRIVMSG #general :hello everyone` | Test missing recipient, missing text, unknown target, and sending to a channel without membership. | `411`, `412`, `401`, or `404`. |

| `PART` | `PART #general :leaving now` | Leave a valid channel and test invalid/non-member cases. | `461`, `403`, or `442` depending on the input. |

| `TOPIC` | `TOPIC #general :new topic` | Query the topic with one argument and change it with a second one. | `331` or `332` when querying; broadcasts the update when changing it. |

| `INVITE` | `INVITE bob #general` | Try inviting as a member and as a non-operator. | Requires two arguments and returns `341` on success. |

| `KICK` | `KICK #general bob :off-topic` | Try kicking as an operator and as a non-operator. | If the target is not on the channel, returns `441`. |

| `MODE` | `MODE #general` | Query current modes, then test changes such as `+i`, `+k`, `+l`, `+o`, and `+t`. | Query returns `324`; unknown mode flags return `472`. |

| `QUIT` | `QUIT :bye` | Send it at any time and confirm the socket closes cleanly. | Ends the session safely. |

## MODE Command — Channel Modes (i, t, k, o, l)
The server implements the following channel modes:

| Mode | Meaning | Parameter? | Description |
| --- | --- | --- | --- |
| ``i`` | Invite-only | No | Only invited users may join. |
| ``t`` | Protected topic | No | Only operators may change the topic. |
| ``k`` | Channel key | Yes (``+k ``<key>``) | Requires a password to join. |
| ``o`` | Operator | Yes (``+o ``<nick>``) | Grants/removes operator privileges. |
| ``l`` | User limit | Yes (``+l ``<number>``) | Sets maximum number of users. |

## Syntax
MODE #channel <+|-><modes> [parameters...]

## Querying Current Modes
MODE #general

Expected reply:
324 <nick> #general +itkl <key> <limit>

## Examples
Invite-only:
MODE #general +i
MODE #general -i

Protected topic:MODE
MODE #general +t
MODE #general -t

Channel key:
MODE #general +k secret123
MODE #general -k

Operator:
MODE #general +o alice
MODE #general -o alice

User limit:
MODE #general +l 20
MODE #general -l

## NOTES
-> Only channel operators may change modes.

-> +k and +l require parameters.

-> -k and -l do not take parameters.

-> +o and -o always require a nickname.

-> Unknown mode flags must return numeric 472.

## Quick Validation Checklist

1. Start the server with a valid port and password.
2. Connect with `nc localhost <port>`.
3. Run the registration sequence.
4. Create a channel with `JOIN #general`.
5. Test messaging and permissions with `PRIVMSG`, `TOPIC`, and `MODE`.
6. Test channel control with `INVITE`, `KICK`, and `PART`.
7. Finish with `QUIT`.