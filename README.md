<img src="https://github.com/petergramenides/huskydb/blob/master/img/huskydb-logo.png?raw=true" width="250">

## Introduction

HuskyDB is a lightweight document-oriented database optimized for Windows :)
It's written entirely in C++ and has no external dependencies. The target is
small applications and video games that do not need large databases.

HuskyDB is:

- **lightweight:** HuskyDB was built to maximize efficiency and size. HuskyDB is 
one of the smallest and fastest NoSQL databases (~1200 lines code).

- **super-fast:** Need your database indexed in a few microseconds? HuskyDB offers a linear time complexity and only takes
approximately 20 microseconds per file indexed. We plan on improving speed even more!

- **document oriented:** Inspired by [TinyDB](https://github.com/msiemens/tinydb), HuskyDB is document oriented and can store
any kind of document. In addition, we offer document-based support for priority indexing and leverage os-level file caching.

- **easy to use:** HuskyDB is designed to be very simple, efficient, and extendable. We
made our utility so it is easily integrated with your apps and can be used in education.

- **written in C++:** Our database does not require any external server or configuration.
All features are offered through our library, which relies on the Windows API.

- **windows native:** Although not ideal for most, at the moment, our library only supports
the Microsoft Windows operating system. We are currently working on native versions for
Mac and Linux as well as a Docker container. Stay tuned for more updates!

- **easily extendable:** HuskyDB is incredibly easy to extend and readable. All libraries
we used are highly documented and all of our code is, equally, highly documented.

- **automatically synced:** One of the most annoying features of many databases is having
to add which documents you want to track. On startup, we sync all your files and configurations!
Making a game? All of your assets management can be instantly handled by HuskyDB.

### Example Code

```
#include "huskydb.h"

// Starting the database
CHuskyDB db = CHuskyDB("/path/to/db");

// Making a package
db.MakePackage("maps");

// Making a table
db.MakeTable("maps", "world-01");

// Adding a file from a C++ object to the table
db.MakeEntry("maps", "world-01", "level.json", data);
```

### Querying

```
// Get specific table from package
CTable* tbl = db.QueryTables("maps", "world-01");

// Get files in table
vector<CFile*> files = tbl->GetFiles();

```

## Priority Loading

You can automate your entity-loading with our prioritization features! To enable, just do the following on startup:

```
CHuskyDB("/path/to/db", true);
```

"priority" files will be generated in all file folders. Add the following to these files to configure priority values:

```
file_name.xxx=#;
```
Be sure to restart the database after making changes to the priority files. In consecutive releases, we will be
adding the ability to have this done without restarting the database.

## Database Structure

HuskyDB was designed with a very specific data management model. Many NoSQL databases are very unstructured, leading to increased query times. HuskyDB adopts a hybrid model, we utilize a custom hierarchical structure to maximize query speeds and minimize transition from
conventional RDBMS SQL-based databases.

<img src="https://github.com/petergramenides/huskydb/blob/master/img/huskydb-structure.JPG?raw=true" width="800">

## Additional Information

You can read our entire documentation [here](https://github.com/petergramenides/huskydb/tree/master/documentation).

Want to contribute or make a suggestion? Send me an email at gramep@rpi.edu!

## Future Updates

Please note that HuskyDB is still in its very early stages of development.
At times it may not be entirely stable and there may be bugs. Expect the
following updates soon:

* Built-in REST API
* Better function styling
* Automated testing
* Security features
* Mac & Linux native builds
* Docker container

## Special Thanks

```
Daniel Parente: For working with me late at night to concepualize, build, and debug HuskyDB.
```
