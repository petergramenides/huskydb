<img src="https://github.com/petergramenides/huskydb/blob/master/huskydb-logo.png" width="250">

## Introduction

HuskyDB is a lightweight document-oriented database optimized for Windows :)
It's written entirely in C++ and has no external dependencies. The target is
small applications and video games that do not need large databases.

HuskyDB is:

- **lightweight:** The current source code has 500 lines of code, which is smaller than
any NoSQL database I know. (please let me know if you find one) 

- **document oriented:** Inspired by [TinyDB](https://github.com/msiemens/tinydb), HuskyDB is document oriented and can store
any kind of document. In addition, we offer document-based support for priority indexing.

- **easy to use:** HuskyDB is designed to be very simple, efficient, and extendable. We
made our utility so it is easily integrated with your apps and can be used in education.

- **written in C++:** Our database does not require any external server or configuration.
All features are offered through our library, which relies on the Windows API.

- **windows native:** Although not ideal for most, at the moment, our library only supports
the ©Microsoft Windows operating system. I am currently working on native versions for
Mac and Linux as well as a Docker container. Stay tuned for more updates!

- **easily extendable:** HuskyDB is incredibly easy to extend and readable. All libraries
we used are highly documented and all of our code is, equally, highly documented.

- **automatically synced:** One of the most annoying features of most databases is having
to add files. The most radical and useful feature of HuskyDB is that it automatically
updates and syncs all your files. Making a game? All of your assets are instantly avaliable
when you start the database!

### Example Code

```
#include <huskydb.cpp>

// Starting the database
db = huskydb("/path/to/db");

// Making a package
db.make_package("maps");

// Making a table
db.make_table("maps", "world-01");

// Adding a file from a C++ object to the table
db.make_entry("maps", "world-01", "level.json", data);
```

### Querying

```
// Get all packages
vector<string> packages = db.query_packages();

// Get tables in package
vector<string> tables = db.query_tables("maps");

// Get files and priority index
unordered_map<string, char> files = db.query_files("maps", "world-01");
```

## Additional Information
****************
You can read our entire documentation [here](https://github.com/petergramenides/huskydb/wiki/Documentation).

Want to contribute? Send me and email at gramep@rpi.edu! Or, just make a
branch and a pull request and I will review it.

## Future Updates
****************

Please note that HuskyDB is still in its very early stages of development.
At times it may not be entirely stable and there may be bugs. Expect the
following updates soon:

* Built-in REST API
* Better function styling
* Automated testing
* Security features
* Mac & Linux Native builds
* Docker container