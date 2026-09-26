# MiniGit

A lightweight version control system implemented in **C++17** as a final project for CSCI 2275: Data Structures at the University of Colorado Boulder.

MiniGit recreates core functionality of a version control system, including file tracking, commits, version history, commit-message searching, and checking out previous versions of files.

## Features

* **Repository initialization** — Creates a `.minigit` directory for storing file versions and repository data.
* **File tracking** — Add and remove files from the current working set.
* **Commits** — Save changes to tracked files along with a commit message.
* **File versioning** — Maintains separate versions of files when they are modified between commits.
* **Commit history** — Stores commits in a doubly linked list.
* **Checkout** — Restore files from a previous commit.
* **Commit search** — Search commit messages by keyword using a hash table.
* **Change detection** — Compares current files against their previously stored versions before creating new versions.
* **Interactive command-line interface** — Provides a menu-driven interface for repository operations.

## Data Structures

MiniGit uses several data structures to manage repository history and efficiently search commits.

### Doubly Linked List — Commit History

Each commit is stored as a node in a **doubly linked list**. A commit contains information such as:

* Commit number
* Commit message
* List of files associated with the commit
* Links to the previous and next commits

This structure allows MiniGit to maintain an ordered history of repository states.

### Singly Linked List — Tracked Files

Each commit maintains a **singly linked list** of files. File nodes contain information needed to track the files and their corresponding versions.

The file list is copied when a new commit is created so that each commit maintains its own representation of the tracked files.

### Hash Table — Commit Search

MiniGit uses a **hash table with separate chaining** to index words appearing in commit messages.

Each hash entry stores:

* A keyword
* The commit numbers containing that keyword
* A pointer to the next entry in the bucket

This allows commit searches to locate relevant commit numbers without traversing the entire commit history.

## How It Works

### 1. Initialize a Repository

MiniGit creates a `.minigit` directory to store versions of tracked files.

### 2. Add Files

When a file is added, MiniGit verifies that the file exists and is not already being tracked. The file is then stored in the repository with its initial version.

### 3. Commit Changes

When a commit is created, MiniGit:

1. Checks the tracked files.
2. Compares each file with its previously stored version.
3. Creates a new file version when a file has changed.
4. Leaves unchanged files untouched.
5. Adds the commit message's words to the hash table.
6. Creates a new commit node containing the current file list.

### 4. Search Commit Messages

A keyword can be searched in the hash table to find the commit numbers whose messages contain that word.

For example:

```text
Commit 0: "implemented file tracking"
Commit 1: "implemented commit search"
```

Searching for:

```text
implemented
```

can return:

```text
0 1
```

### 5. Checkout

MiniGit can restore the working directory to the versions of files associated with a specified commit.

This allows the user to inspect an earlier state of the repository.

## Repository Storage

Tracked file versions are stored inside the `.minigit` directory rather than replacing previous versions.

For example:

```text
.minigit/
├── main_00.cpp
├── main_01.cpp
├── main_02.cpp
└── ...
```

If `main.cpp` changes between commits, MiniGit can store the new contents as a separate version while preserving the previous version.

## Example Workflow

A typical MiniGit session follows this general workflow:

```text
Initialize repository
        ↓
     Add files
        ↓
   Make changes
        ↓
      Commit
        ↓
   Make changes
        ↓
      Commit
        ↓
 Search history / Checkout
```

## Project Structure

```text
MiniGit/
├── app_1/
│   └── main_1.cpp
├── code_1/
│   ├── miniGit.cpp
│   ├── miniGit.hpp
│   ├── hash.cpp
│   └── hash.hpp
├── build/
├── CMakeLists.txt
├── CMakeLists.txt.in
├── .gitignore
└── README.md
```

## Technologies

* **C++17**
* **CMake**
* C++ standard library
* `std::filesystem`
* Custom linked-list and hash-table implementations

## Key Concepts Demonstrated

* Data structures and pointers
* Doubly linked lists
* Singly linked lists
* Hash tables
* Separate chaining
* File I/O
* File comparison and versioning
* Dynamic memory management
* Object-oriented programming
* C++17 filesystem operations
* Command-line interfaces
* Version-control system design

## Project Background

This project was developed as the final project for **CSCI 2275: Data Structures** at the University of Colorado Boulder.

The project focused on applying data-structure concepts to a practical system by implementing a simplified version-control system from scratch.
